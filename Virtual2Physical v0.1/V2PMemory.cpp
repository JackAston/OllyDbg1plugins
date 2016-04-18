#include  <windows.h>
#include  <stdarg.h>
#include  <shlobj.h>
#include  <stdio.h>
#include  "plugin.h"
#include  <ctype.h>
#include "NATIVE.h"


/*

	  [--------------32 bit------------------]
	  [  10 bit      10 bit       12 bit     ]
	  [XXXXXXXXXX] [XXXXXXXXXX] [XXXXXXXXXXXX]
	  offset in    offset in     offset in 
	  page         the page      page
	  directory    table
	  
		
		  SIZE 4K  CR3 REGISTER HOLD THE ADDRESS OF THE PAGE DIRECTORY
		  
			
			  
                [PAGE DIRECTORY 10 bit CR3]               [PAGES TABLES 10 bit]        [PAGES 12 bit]
				-----------------------------------------------------------------------------------------------
				0   [DWORD ENTRY]   ------------------------> PAGE TABLE #0  [4K]   ----->PHSY PAGE  
				0   [DWORD]                  0-4096 bytes
				1   [DWORD]                 
				1023[DWORD]
				
				  1   [DWORD ENTRY]   ------------------------> PAGE TABLE #1  [4K]    ----->PHSY PAGE 
				  0   [DWORD]                  0-4096 bytes
				  1   [DWORD]
				  1023[DWORD]
				  
					2   [DWORD ENTRY]   ------------------------> PAGE TABLE 2   [4K]     ----->PHSY PAGE
					0   [DWORD]                  0-4096 bytes
					1   [DWORD]
					1023[DWORD]
					
					  1023 [DWORD ENTRY]   ------------------------> PAGE TABLE 3   [4K]     ----->PHSY PAGE 
					  0   [DWORD]                  0-4096 bytes
					  1   [DWORD]
					  1023[DWORD]
					  
						
						  
							
							  
								
								  
									
									  
										
										  
											
*/
struct PageEntry
{
    DWORD Present:1;
    DWORD ReadWrite:1;
    DWORD UserSupervisor:1;
    DWORD WriteThrough:1;
    DWORD CacheDisabled:1;
    DWORD Accessed:1;
    DWORD Reserved:1;
    DWORD Size:1;
    DWORD Global:1;
    DWORD Unused:3;
    DWORD PhysicalAddress:20;
};

//
// Functions in NTDLL that we dynamically locate
//


LPFN_NtUnmapViewOfSection  pfn_NtUnmapViewOfSection;
LPFN_NtOpenSection         pfn_NtOpenSection;
LPFN_NtMapViewOfSection    pfn_NtMapViewOfSection;
LPFN_RtlInitUnicodeString  pfn_RtlInitUnicodeString;
LPFN_RtlNtStatusToDosError pfn_RtlNtStatusToDosError;


BOOLEAN LocateNtdllEntryPoints()
{
	if( !(pfn_RtlInitUnicodeString = (LPFN_RtlInitUnicodeString) GetProcAddress( GetModuleHandle("ntdll.dll"),
		"RtlInitUnicodeString" )) ) {
		
		return FALSE;
	}
	if( !(pfn_NtUnmapViewOfSection = (LPFN_NtUnmapViewOfSection) GetProcAddress( GetModuleHandle("ntdll.dll"),
		"NtUnmapViewOfSection" )) ) {
		
		return FALSE;
	}
	if( !(pfn_NtOpenSection = (LPFN_NtOpenSection) GetProcAddress( GetModuleHandle("ntdll.dll"),
		"NtOpenSection" )) ) {
		
		return FALSE;
	}
	if( !(pfn_NtMapViewOfSection = (LPFN_NtMapViewOfSection) GetProcAddress( GetModuleHandle("ntdll.dll"),
		"NtMapViewOfSection" )) ) {
		
		return FALSE;
	}
	if( !(pfn_RtlNtStatusToDosError = (LPFN_RtlNtStatusToDosError) GetProcAddress( GetModuleHandle("ntdll.dll"),
		"RtlNtStatusToDosError" )) ) {
		
		return FALSE;
	}
	return TRUE;
}


//----------------------------------------------------------------------
//
// PrintError
//
// Formats an error message for the last error
//
//----------------------------------------------------------------------
void PrintError( char *message, NTSTATUS status )
{
	char *errMsg;
	
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, (*pfn_RtlNtStatusToDosError)( status ), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR) &errMsg, 0, NULL );
	printf("%s: %s\n", message, errMsg );
	LocalFree( errMsg );
}
//--------------------------------------------------------
//
// UnmapPhysicalMemory
//
// Maps a view of a section.
//
//--------------------------------------------------------
VOID UnmapPhysicalMemory( DWORD Address )
{
	NTSTATUS		status;
	
	status = (*pfn_NtUnmapViewOfSection)( (HANDLE) -1, (PVOID) Address );
	if( !NT_SUCCESS(status)) {
		
		PrintError("Unable to unmap view", status );
	}
}


//--------------------------------------------------------
//
// MapPhysicalMemory
//
// Maps a view of a section.
//
//--------------------------------------------------------
BOOLEAN MapPhysicalMemory( HANDLE PhysicalMemory,
						  PDWORD Address, PDWORD Length,
						  PDWORD VirtualAddress )
{
	NTSTATUS			ntStatus;
	PHYSICAL_ADDRESS	viewBase;
	char				error[256];
	
	*VirtualAddress = 0;
	viewBase.QuadPart = (ULONGLONG) (*Address);
	
	ntStatus = (*pfn_NtMapViewOfSection) (PhysicalMemory,
		(HANDLE) -1,
		(PVOID*) VirtualAddress,
		0L,
		*Length,
		&viewBase,
		Length,
		ViewShare,
		0,
		PAGE_READONLY );
	
	if( !NT_SUCCESS( ntStatus )) {
		
		sprintf( error, "Could not map view of %X length %X",
			*Address, *Length );
		PrintError( error, ntStatus );
		return FALSE;					
	}
	
	*Address = viewBase.LowPart;
	return TRUE;
}

HANDLE OpenPhysicalMemory()
{
	NTSTATUS		status;
	HANDLE			physmem;
	UNICODE_STRING	physmemString;
	OBJECT_ATTRIBUTES attributes;
	WCHAR			physmemName[] = L"\\device\\physicalmemory";
	
	(*pfn_RtlInitUnicodeString)( &physmemString, physmemName );	
	
	InitializeObjectAttributes( &attributes, &physmemString,
								OBJ_CASE_INSENSITIVE, NULL, NULL );			
	status = (*pfn_NtOpenSection)( &physmem, SECTION_MAP_READ, &attributes );
	
	if( !NT_SUCCESS( status )) {
		
		PrintError( "Could not open \\device\\physicalmemory", status );
		return NULL;
	}
	
	return physmem;
}



DWORD Virtual2Physical(HANDLE Section,DWORD DirectoryMappedAddress,DWORD VirtualAddress)
{
	
	DWORD            PhysicalAddress;
	DWORD            DirectoryOffset;
	DWORD            TableOffset;
	DWORD            *pPageEntry;
	DWORD 	         MappedSize=4096; 
	LARGE_INTEGER    phys;
	PVOID            TableMappedAddress=0;
	DWORD            NtStatus;
	
	
	
	DirectoryOffset=VirtualAddress>>22;
	TableOffset=(VirtualAddress>>12)&0x3FF;
	pPageEntry=(DWORD*)DirectoryMappedAddress;
	
	phys.QuadPart=(pPageEntry[DirectoryOffset]&0xFFFFF000); 
	NtStatus = (*pfn_NtMapViewOfSection)(Section, (HANDLE) -1, 
		&TableMappedAddress, 0L,MappedSize,
		&phys, &MappedSize, ViewShare,0, 
		PAGE_READONLY);
	
	
	if (NtStatus) return -1;
	
	
	
	pPageEntry=(DWORD*)TableMappedAddress;
	PhysicalAddress=(pPageEntry[TableOffset]&0xFFFFF000);
	
	(*pfn_NtUnmapViewOfSection)((HANDLE) -1, TableMappedAddress);
	
	return (PhysicalAddress+(VirtualAddress&0xFFF));
	
	
	
	
}

DWORD Scan4CR3_Register(HANDLE Section)
{
	MEMORYSTATUS meminfo;
    GlobalMemoryStatus(&meminfo);
	
	DWORD           dwMemIndexOffest;
	DWORD           dwMappedSize=4096; //4K
	LARGE_INTEGER   physMemoryAddress;
	PVOID           pPageDirectoryVA=0,TableMappedAddress;
	DWORD           dwNtStatus;
	
	DWORD           PageDirectoryOffset;
	DWORD           PageOffset;  
	DWORD           *pPageEntry;
    BOOL bFound=FALSE;
	
	for (dwMemIndexOffest=0; dwMemIndexOffest<meminfo.dwTotalPhys; 
	dwMemIndexOffest+=0x1000)
	{
		
		physMemoryAddress.QuadPart=dwMemIndexOffest;
		pPageDirectoryVA=0;
		dwMappedSize=4096;
		//Take 4K of Bytes
		dwNtStatus = (*pfn_NtMapViewOfSection)(Section, 
			(HANDLE) -1, &pPageDirectoryVA, 0L,
			dwMappedSize, &physMemoryAddress, &dwMappedSize, ViewShare,0, PAGE_READONLY);
		
		if (dwNtStatus) continue;
		
		
		pPageEntry=(DWORD*)pPageDirectoryVA;  
		
		PageDirectoryOffset=((DWORD)pPageDirectoryVA)>>22;
		PageOffset=(((DWORD)pPageDirectoryVA)>>12)&0x3FF;
		
		
		
		if((pPageEntry[0x300]&0xFFFFF000)!=dwMemIndexOffest ||(pPageEntry[0x300]&1)!=1 || (pPageEntry[PageDirectoryOffset]&1)!=1)
		{
			(*pfn_NtUnmapViewOfSection)((HANDLE) -1, pPageDirectoryVA);
			continue;
			
		}
		
		
		
		physMemoryAddress.QuadPart=(pPageEntry[PageDirectoryOffset]&0xFFFFF000); 
		
		
		TableMappedAddress=0;
		dwMappedSize=4096;
		dwNtStatus = (*pfn_NtMapViewOfSection)(Section, (HANDLE) -1, &TableMappedAddress, 
			
	           0L,dwMappedSize, &physMemoryAddress, &dwMappedSize, ViewShare,0, PAGE_READONLY);
		
		
		if(dwNtStatus){
			(*pfn_NtUnmapViewOfSection)((HANDLE) -1, pPageDirectoryVA);
			continue;
		}
		
		
		pPageEntry=(DWORD*)TableMappedAddress;
		if((pPageEntry[PageOffset]&1)==1 && (pPageEntry[PageOffset]&0xFFFFF000)==dwMemIndexOffest)
			
			bFound=TRUE;
		
		
		(*pfn_NtUnmapViewOfSection)((HANDLE) -1, TableMappedAddress);
		
		
		if(bFound) break;
		(*pfn_NtUnmapViewOfSection)((HANDLE) -1, pPageDirectoryVA);
		
		
		
		
		
	}
	return (DWORD)pPageDirectoryVA;
	
}

