/* 
Module: ollybone.c
Purpose: Implement non-executable pages in process virtual memory
Author: Joe Stewart
Credits: Jamie Butler/Sherri Sparks from code found in
http://www.phrack.org/phrack/63/p63-0x08_/Raising_The_Bar_For_Windows_Rootkit_Detection.txt by Jamie Butler/Sherri Sparks), also some utility subroutines
by Sten stenri@mail.ru

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include "ntddk.h"
#include "stdio.h"
#include "stdlib.h"
#include "multicpu.h"

typedef BOOLEAN BOOL;
typedef unsigned long DWORD;
typedef DWORD * PDWORD;
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned long* PPTE;
typedef LARGE_INTEGER PHYSICAL_MEMORY;

PDEVICE_OBJECT g_PagefaultDevice;
void *g_OldInt0EHandler = NULL;
void *g_Int03Handler = NULL;
void *g_Int01Handler = NULL;
BOOL hooked = FALSE;
DWORD g_pidsaved = 0;

NTSTATUS PagefaultDispatch(IN PDEVICE_OBJECT, IN PIRP);
NTSTATUS PagefaultUnload(IN PDRIVER_OBJECT);
NTSTATUS PagefaultDeviceControl(IN PFILE_OBJECT, IN BOOLEAN, IN PVOID, 
				IN ULONG, OUT PVOID, IN ULONG, IN ULONG, 
				OUT PIO_STATUS_BLOCK, IN PDEVICE_OBJECT
				);

#define FILE_DEVICE_OLLYBONE      0x0000584e
#define IOCTL_OLLYBONE_SET            (ULONG) CTL_CODE(FILE_DEVICE_OLLYBONE, 0x01, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_OLLYBONE_RESTORE          (ULONG) CTL_CODE(FILE_DEVICE_OLLYBONE, 0x02, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_TRANSFER_TYPE( _iocontrol)   (_iocontrol & 0x3)

typedef struct _ioctlparams {
	DWORD pid;
	DWORD va;
	DWORD size;
} IOCTLPARAMS;

#define PROCESS_PAGE_DIR_BASE           0xC0300000
#define PROCESS_PAGE_TABLE_BASE         0xC0000000
#define KERNEL_ADDRESS_START		0x80000000

static DWORD MmSystemPteBase=0xc0000000;
const WCHAR deviceLinkBuffer[]  = L"\\DosDevices\\ollybone";
const WCHAR deviceNameBuffer[]  = L"\\Device\\ollybone";
char buf[100];
const char formatMsg[] = "Handling access fault (pte:%x) at %x for address %x\n";

VOID NTAPI KeAttachProcess( IN PEPROCESS );
VOID NTAPI KeDetachProcess( );
NTSTATUS NTAPI PsLookupProcessByProcessId( DWORD pid, PEPROCESS *eproc );

#define   DebugPrint		DbgPrint

PPTE GetPteAddress( PVOID VirtualAddress );
void HookMemoryPage( PVOID pReadWritePage );
void UnhookMemoryPage( PVOID pReadWritePage );
void MarkPageSupervisorOnly( void *pte );
void MarkPageUserMode( void *pte );
void FixCoW( void *pte );
void *SetInterruptHandler(int IntNo, void *HandlerPtr, BYTE flags);
void *GetInterruptHandler(DWORD IntNo, DWORD IdtNum);
void InstallHandler();
void RemoveHandler();

static __declspec( naked ) NewInt0EHandler()
{
        __asm
        {
                pushad
		push fs
		mov bx, 0x30
		mov fs, bx
                mov edx, dword ptr [esp+0x20+4]	//PageFault.ErrorCode
		test edx, 1		// if the fault was caused by a 
		je PassDown		// nonpresent page, pass it down

		test edx, 4
                je PassDown    		// it's a kernelmode pagefault

                mov eax, cr2 	// the faulting address
		cmp eax, KERNEL_ADDRESS_START
		jnb PassDown	// we don't hook kernel pages, pass it down

                mov esi, PROCESS_PAGE_DIR_BASE
                mov ebx, eax
                shr ebx, 22
                lea ebx, [esi + ebx*4]  //ebx = pPTE for large page
                test [ebx], 0x80        //check if its a large page
                jnz PassDown		//pass it down if it is

                mov esi, PROCESS_PAGE_TABLE_BASE
                mov ebx, eax
                shr ebx, 12
                lea ebx, [esi + ebx*4]  //ebx = pPTE
		mov ecx, [ebx]
		test ecx, 0x01	// valid?
		je PassDown

		mov ecx, [ebx]
		test ecx, 0x04	// user-accessible? if so it's not ours
		jnz PassDown

/*
		// now we hope we are dealing with one of our hooked pages
		mov ecx, [esp+0x24+4]
		// log the handler execution
		pushad
		push eax
		push ecx
		mov ecx, [ebx]
		push ecx
		lea ebx, formatMsg
		push ebx
		lea edx, buf
		push edx 		
		mov edi, [sprintf]
		call edi
		add esp, 0x14
		lea edx, buf
		push edx 		
		call DbgPrint
		add esp, 0x4
		popad
*/
                cmp [esp+0x24+4], eax     //Is due to an attempted execute?
                jne LoadDTLB

		pop fs
		popad
                add esp,4		// strip PF error code
		jmp [g_Int01Handler]	// divert to int1 trap handler

LoadDTLB:
                or dword ptr [ebx], 0x04           //mark the page user-acessible
                mov eax, dword ptr [eax]           //load the DTLB
                and dword ptr [ebx], 0xFFFFFFFB    //re-mark page supervisor-only
		pop fs
                popad
                add esp,4
		iretd

PassDown:
		pop fs
		popad
                jmp [g_OldInt0EHandler]

        }
}

NTSTATUS DriverEntry(
   IN PDRIVER_OBJECT  DriverObject,
   IN PUNICODE_STRING RegistryPath )
{
    NTSTATUS                ntStatus;
    UNICODE_STRING          deviceNameUnicodeString;
    UNICODE_STRING          deviceLinkUnicodeString;        
    
    RtlInitUnicodeString (&deviceNameUnicodeString,
                          deviceNameBuffer );
    RtlInitUnicodeString (&deviceLinkUnicodeString,
                          deviceLinkBuffer );
    ntStatus = IoCreateDevice ( DriverObject,
                                0, 
                                &deviceNameUnicodeString,
                                FILE_DEVICE_OLLYBONE,
                                0,
                                TRUE,
                                &g_PagefaultDevice );

    if(! NT_SUCCESS(ntStatus))
	{
        DebugPrint(("Failed to create device!\n"));
        return ntStatus;
    }
		
	ntStatus = IoCreateSymbolicLink (&deviceLinkUnicodeString,
                                        &deviceNameUnicodeString );
    if(! NT_SUCCESS(ntStatus)) 
	{
		IoDeleteDevice(DriverObject->DeviceObject);
        DebugPrint("Failed to create symbolic link!\n");
        return ntStatus;
    }

    DriverObject->MajorFunction[IRP_MJ_SHUTDOWN]        =
    DriverObject->MajorFunction[IRP_MJ_CREATE]          =
    DriverObject->MajorFunction[IRP_MJ_CLOSE]           =
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = PagefaultDispatch;
    DriverObject->DriverUnload                          = PagefaultUnload;

    return STATUS_SUCCESS;
}

NTSTATUS PagefaultUnload(IN PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING          deviceLinkUnicodeString;
	PDEVICE_OBJECT			p_NextObj;

	p_NextObj = DriverObject->DeviceObject;

	if (p_NextObj != NULL)
	{
		RtlInitUnicodeString( &deviceLinkUnicodeString, deviceLinkBuffer );
		IoDeleteSymbolicLink( &deviceLinkUnicodeString );
		IoDeleteDevice( DriverObject->DeviceObject );
		return STATUS_SUCCESS;
	}
	return STATUS_SUCCESS;
}



NTSTATUS 
PagefaultDispatch(
    IN PDEVICE_OBJECT DeviceObject, 
    IN PIRP Irp 
    )
{
    PIO_STACK_LOCATION      irpStack;
    PVOID                   inputBuffer;
    PVOID                   outputBuffer;
    ULONG                   inputBufferLength;
    ULONG                   outputBufferLength;
    ULONG                   ioControlCode;
    NTSTATUS				ntstatus;

    ntstatus = Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    irpStack = IoGetCurrentIrpStackLocation (Irp);
    inputBuffer             = Irp->AssociatedIrp.SystemBuffer;
    inputBufferLength       = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    outputBuffer            = Irp->AssociatedIrp.SystemBuffer;
    outputBufferLength      = irpStack->Parameters.DeviceIoControl.OutputBufferLength;
    ioControlCode           = irpStack->Parameters.DeviceIoControl.IoControlCode;

    switch (irpStack->MajorFunction) {
    case IRP_MJ_CREATE:
        break;

    case IRP_MJ_SHUTDOWN:
        break;

    case IRP_MJ_CLOSE:
        break;

    case IRP_MJ_DEVICE_CONTROL:

        if(IOCTL_TRANSFER_TYPE(ioControlCode) == METHOD_NEITHER) 
	{
            outputBuffer = Irp->UserBuffer;
        }
        ntstatus = PagefaultDeviceControl( irpStack->FileObject, TRUE,
		inputBuffer, inputBufferLength, 
		outputBuffer, outputBufferLength,
		ioControlCode, &Irp->IoStatus, DeviceObject );
        break;
    }
    IoCompleteRequest( Irp, IO_NO_INCREMENT );
    return ntstatus;   
}


NTSTATUS
PagefaultDeviceControl(
    IN PFILE_OBJECT FileObject, 
    IN BOOLEAN Wait,
    IN PVOID InputBuffer, 
    IN ULONG InputBufferLength, 
    OUT PVOID OutputBuffer, 
    IN ULONG OutputBufferLength, 
    IN ULONG IoControlCode, 
    OUT PIO_STATUS_BLOCK IoStatus, 
    IN PDEVICE_OBJECT DeviceObject 
    ) 
{
	NTSTATUS ntStatus;
	DWORD va;
	DWORD i;
	DWORD size;
	DWORD pid;
	PEPROCESS eproc = NULL;
    	IOCTLPARAMS 	*params;
	char buf[100];
	
	IoStatus->Status = STATUS_SUCCESS;
    	IoStatus->Information = 0;

	switch ( IoControlCode ) 
	{	
	case IOCTL_OLLYBONE_SET:
        	//DebugPrint(("**** Got OLLYBONE_SET IOCTL\n"));

			if ((InputBufferLength < sizeof(int) * 3) || (InputBuffer == NULL))
			{
				IoStatus->Status = STATUS_INVALID_BUFFER_SIZE;
				break;
			}
			params = (IOCTLPARAMS *)InputBuffer;
			pid =  params->pid;
			va =   params->va;
			size = params->size;

			if (PsLookupProcessByProcessId(pid, &eproc) == STATUS_SUCCESS)
			{
				KeAttachProcess(eproc);
				
				sprintf(buf,"Attached to PID %x, hooking %x (size=%x)\n",
						pid, va, size);
				DebugPrint(buf);
				
				g_pidsaved = pid;
				for (i = va; i < va+size; i+=4096)
					HookMemoryPage((void *)i);

				KeDetachProcess();

			}
			else 
			{
				IoStatus->Status = STATUS_UNSUCCESSFUL;
			}

	break;

	case IOCTL_OLLYBONE_RESTORE:
        	//DebugPrint(("**** Got OLLYBONE_RESTORE IOCTL\n"));
			
			if ((InputBufferLength < sizeof(int) * 3) || (InputBuffer == NULL))
			{
				IoStatus->Status = STATUS_INVALID_BUFFER_SIZE;
				break;
			}	
			params = (IOCTLPARAMS *)InputBuffer;
			pid =  params->pid;
			va =   params->va;
			size = params->size;
			
			if (PsLookupProcessByProcessId(pid, &eproc) == STATUS_SUCCESS)
			{
				KeAttachProcess(eproc);
				g_pidsaved = 0;

				for (i = va; i < va+size; i+=4096)
					UnhookMemoryPage((void *)i);

				KeDetachProcess();

			}
			else 
			{
				IoStatus->Status = STATUS_UNSUCCESSFUL;
			}
	break;
	
	default:
		IoStatus->Status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}

    return IoStatus->Status;
}

PPTE GetPteAddress( PVOID VirtualAddress )
{
        PPTE pPTE = 0;
        __asm
        {
                cli                     //disable interrupts
                pushad
                mov esi, PROCESS_PAGE_DIR_BASE
                mov edx, VirtualAddress
                mov eax, edx
                shr eax, 22
                lea eax, [esi + eax*4]  //pointer to page directory entry
                test [eax], 0x80        //is it a large page?
                jnz Is_Large_Page       //it's a large page
                mov esi, PROCESS_PAGE_TABLE_BASE
                shr edx, 12
                lea eax, [esi + edx*4]  //pointer to page table entry (PTE)
                mov pPTE, eax
                jmp Done

                //NOTE: There is not a page table for large pages because
                //the phys frames are contained in the page directory.
                Is_Large_Page:
                mov pPTE, eax

                Done:
                popad
                sti                    //reenable interrupts
        }//end asm

        return pPTE;

}//end GetPteAddress

void MarkPageSupervisorOnly( void *pte )
{
	*(DWORD *)pte &= 0xFFFFFFFB;
}
void MarkPageUserMode( void *pte )
{
	*(DWORD *)pte |= 0x4;
}
void FixCoW( void *pte )
{
	*(DWORD *)pte &= 0xFFFFFDFF;	// mark CoW off
	*(DWORD *)pte |= 0x2;		// mark writable
	*(DWORD *)pte |= 0x40;		// Dirty, dirty page
}

void HookMemoryPage( PVOID pPage )
{
	char buf[100];
	void *pReadWritePte;

        pReadWritePte = GetPteAddress( pPage );
	sprintf(buf, "Hooking page va:%x ppte:%x pte:%x ",
			pPage, pReadWritePte, *(DWORD *)pReadWritePte);
	
	DebugPrint(buf);
	__asm {	
		mov eax, pPage	// access page in case it is paged out
		mov eax, [eax]
		cli
	}

        if( hooked == FALSE )
        {      
		InstallHandler();
        	hooked = TRUE;
        }

        pReadWritePte = GetPteAddress( pPage );

	__asm {	
			mov eax, pPage
			invlpg [eax]
	}
	// Pages marked Copy-on-Write won't work, change them to normal perms
        FixCoW( pReadWritePte );

        //Mark the page supervisor mode only
        MarkPageSupervisorOnly( pReadWritePte );

	__asm {	
			mov eax, pPage
			invlpg [eax]
			sti  //reenable interrupts
	}
	sprintf(buf, "  New pte:%x\n", *(DWORD *)pReadWritePte);
	DebugPrint(buf);
	
}


void UnhookMemoryPage( PVOID pPage )
{
	void *pReadWritePte;

	__asm {	
		mov eax, pPage	// access page in case it is paged out
		mov eax, [eax]
		cli
	}

        pReadWritePte = GetPteAddress( pPage );

	__asm {	
			mov eax, pPage
			invlpg [eax]
	}
        MarkPageUserMode( pReadWritePte );

	__asm {	
			mov eax, pPage
			invlpg [eax]
			sti  //reenable interrupts
		}
}

void InstallHandler() 
{
	g_OldInt0EHandler = GetInterruptHandler(0x0e,0);
	g_Int03Handler = GetInterruptHandler(0x03,0);
	g_Int01Handler = GetInterruptHandler(0x01,0);
	SetInterruptHandler(0x0E,&NewInt0EHandler,0x8e);
}

void RemoveHandler()
{
	if (g_OldInt0EHandler != NULL)
		SetInterruptHandler(0x0E, g_OldInt0EHandler, 0x8e);
}

void* GetInterruptHandler(DWORD IntNo, DWORD IdtNum)
{
   DWORD IDTBase=0;
   DWORD OldIntr;
   WORD  *IdtEntry;

   IDTBase = mp_GetIDTBase(IdtNum);

   IdtEntry=(WORD*)(IDTBase+IntNo*8);

   OldIntr=IdtEntry[0]+(IdtEntry[3]<<16);

   return (void*)OldIntr;
}

void* SetInterruptHandler(int IntNo, void *HandlerPtr, BYTE flags)
{

   __asm
   {
          pushad

          mov    eax, IntNo
          mov    dl,  flags
          mov    edi, HandlerPtr
          mov    bx,  cs

          cli                       ; TO DO: StopSecondCPUs()
          call   mp_HookInterrupt
          sti

          popad
   }

   return (void*)mp_OldHandler;
}
