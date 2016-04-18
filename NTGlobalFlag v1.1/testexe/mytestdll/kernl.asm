.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib

;struct copied from four-f's kmdkits ntddk.inc :)

NTSTATUS	typedef	       SDWORD
PVOID		typedef		 PTR

PROCESS_BASIC_INFORMATION STRUCT	; sizeof = 18h
	ExitStatus				NTSTATUS	?
	PebBaseAddress			PVOID		? ; PPEB
	AffinityMask			DWORD		?
	BasePriority			DWORD		? ; KPRIORITY
	UniqueProcessId			DWORD		?
	InheritedFromUniqueProcessId	DWORD		?
PROCESS_BASIC_INFORMATION ENDS
PPROCESS_BASIC_INFORMATION typedef PTR PROCESS_BASIC_INFORMATION


.data
deb_det             db "debugger was detected process can be terminated here",0
caption             db "error",0
ModuleName          db "ntdll.dll",0
proc_name           db "NtQueryInformationProcess",0
proc_name1          db "ZwOpenProcess",0
proc_name2          db "RtlNtStatusToDosError",0
hardcoded_string    db "Internal OLLYDBG error",0

.data
string              db 100 dup (?)
hwnd_ntdll          dd ?
proc_addr           dd ?
proc_addr1          dd ?
proc_addr2          dd ?
curr_procid         dd ?
proc_handle         dd ?
par_proc_handle     dd ?

align 4
pbi PROCESS_BASIC_INFORMATION <>

.code

DllEntry proc hInstance:HINSTANCE, reason:DWORD, reserved1:DWORD
	.if reason==DLL_PROCESS_ATTACH
        invoke IsDebuggerPresent
        .if EAX != 0
            invoke MessageBox ,NULL,offset deb_det,offset caption, NULL
        .endif
	.elseif reason==DLL_PROCESS_DETACH
	.elseif reason==DLL_THREAD_ATTACH
        invoke GetModuleHandle,offset ModuleName                ;ntdll
        mov hwnd_ntdll ,eax
        invoke GetProcAddress,hwnd_ntdll,offset proc_name       ;NtQueryProcessInformation
        mov proc_addr ,eax
        invoke GetProcAddress,hwnd_ntdll,offset proc_name1      ;ZwOpenProcess
        mov proc_addr1,eax
        invoke GetProcAddress,hwnd_ntdll,offset proc_name2      ;RtlNtStatusToDosError
        mov proc_addr2,eax
        invoke GetCurrentProcessId
        mov curr_procid,eax
        invoke OpenProcess,PROCESS_ALL_ACCESS,FALSE,curr_procid
        mov proc_handle,eax
        push NULL
        PUSH sizeof pbi
        push OFFSET pbi
        push NULL
        push proc_handle
        call proc_addr                                          ;NtQueryProcessInformation

        push pbi.InheritedFromUniqueProcessId
        push FALSE
        push PROCESS_VM_READ or PROCESS_VM_OPERATION or PROCESS_VM_WRITE
        call blah                                                                   ;calling simulated code to ZwOpenProcess
        mov par_proc_handle,eax
        invoke ReadProcessMemory,par_proc_handle,04b0642h,offset string,22,NULL     ;reading a string from ollydbg :)
        invoke lstrcmp,offset string,offset hardcoded_string
        .if eax == 0
        invoke MessageBox,NULL,offset string,offset caption,NULL                    ;if run under debugger this will be called
        .endif
        invoke CloseHandle,proc_handle
        invoke CloseHandle,par_proc_handle
        
      .else        
	.endif
	mov  eax,TRUE
	ret
DllEntry Endp

Messagebox proc
        invoke MessageBox ,NULL,offset deb_det,offset caption, NULL
        ret	
Messagebox endp


;simulated call to ZwOpenProcess from kernel32.dll :)

blah:

  PUSH    EBP
  MOV     EBP, ESP
  SUB     ESP, 20h
  MOV     EAX, DWORD PTR SS:[EBP+10h]
  PUSH    ESI
  MOV     DWORD PTR SS:[EBP-8h], EAX
  MOV     EAX, DWORD PTR SS:[EBP+0Ch]
  XOR     ESI, ESI
  MOV     DWORD PTR SS:[EBP-20h], 18h
  NEG     EAX
  SBB     EAX, EAX
  MOV     DWORD PTR SS:[EBP-4h], ESI
  AND     EAX, 2h
  MOV     DWORD PTR SS:[EBP-1Ch], ESI
  MOV     DWORD PTR SS:[EBP-14h], EAX
  LEA     EAX, DWORD PTR SS:[EBP-8h]
  PUSH    EAX
  LEA     EAX, DWORD PTR SS:[EBP-20h]
  PUSH    EAX
  LEA     EAX, DWORD PTR SS:[EBP+10h]
  PUSH    DWORD PTR SS:[EBP+8h]
  MOV     DWORD PTR SS:[EBP-18h], ESI
  MOV     DWORD PTR SS:[EBP-10h], ESI
  MOV     DWORD PTR SS:[EBP-0Ch], ESI
  PUSH    EAX
  CALL proc_addr1
  CMP     EAX, ESI
  JL      clah
  MOV     EAX, DWORD PTR SS:[EBP+10h]

elah:
  POP     ESI
  LEAVE
  RETN    0Ch

clah:

  PUSH    EAX
  CALL    dlah
  XOR     EAX, EAX
  JMP     elah

dlah:

  PUSH    ESI
  PUSH    DWORD PTR SS:[ESP+8h]
  CALL    proc_addr2
  MOV     ESI, EAX
  PUSH    ESI
  CALL    SetLastError
  MOV     EAX, ESI
  POP     ESI
  RETN    4

End DllEntry
