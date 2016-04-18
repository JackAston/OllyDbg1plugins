;@echo off
;goto make
.486
.model flat, stdcall
option casemap:none

include C:\masm32\include\windows.inc
include C:\masm32\include\kernel32.inc
include C:\masm32\include\user32.inc
includelib C:\masm32\lib\user32.lib
includelib C:\masm32\lib\kernel32.lib

include macros.inc
include Strings.mac

include plugin.inc
includelib ollydbg.lib

include GetAddressOfVAForCurrentApiInIAT.asm
include CheckOS.asm

CheckSignature          PROTO  :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
WaitForDebugEventHook   PROTO  :DWORD, :DWORD
ContinueDebugEventHook  PROTO  :DWORD, :DWORD, :DWORD
SetWindowTextHook       PROTO  :DWORD, :DWORD
GetWindowTextHook       PROTO  :DWORD, :DWORD, :DWORD
SetHook                 PROTO  :DWORD, :DWORD, :DWORD, :DWORD
RemoveHook              PROTO  :DWORD, :DWORD, :DWORD, :DWORD
IsDebuggerPresentKill   PROTO  :DWORD, :DWORD
OptionsDlgProc          PROTO  :DWORD, :DWORD, :DWORD, :DWORD
WriteToMemory           PROTO  :DWORD, :DWORD, :DWORD, :DWORD
ErrorToLog              PROTO  :DWORD
ErrorMessage            PROTO  :DWORD
SetRemoteHook           PROTO  :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD

VERSION    TEXTEQU  <"1.2.4">

IDC_BUTTON1       equ  101
IDC_BUTTON2       equ  102
IDC_CHECKBOX1     equ  103
IDC_CHECKBOX2     equ  104
IDC_CHECKBOX3     equ  105
IDC_CHECKBOX4     equ  106
IDC_CHECKBOX5     equ  107
IDC_CHECKBOX6     equ  108
IDC_CHECKBOX7     equ  109

ProcessDebugPort      equ         7
DBG_PRINTEXCEPTION_C  equ  40010006h
STATUS_ACCESS_DENIED  equ 0C0000022h


.DATA

szKernel32               db "kernel32.dll",0
szUser32                 db "user32.dll",0
szIsDebuggerPresent      db "IsDebuggerPresent",0
szWaitForDebugEvent      db "WaitForDebugEvent",0
szContinueDebugEvent     db "ContinueDebugEvent",0
szSetWindowText          db "SetWindowTextA",0
szGetWindowText          db "GetWindowTextA",0
szPluginName             db "Hide Debugger",0
szPluginName_len        equ $ - szPluginName - 1
szPluginNameAndVersion   db "Hide Debugger v",VERSION,0
szCopyright              db "  Copyright (c) 2004-"
                         year
                         db " by Asterix",0

;;                              1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17    18
ALIGN DWORD
IsDebuggerPresentSignature db 064h, 0A1h, 018h, 000h, 000h, 000h, 08Bh, 040h, 030h, 00Fh, 0B6h, 040h, 002h, 0C3h
sz db "Hide Debugger v",VERSION,0Dh,0Ah
   db 0Dh,0Ah
   db "This plugin hides OllyDbg from many",0Dh,0Ah
   db "debugger detection tricks.",0Dh,0Ah
   db 0Dh,0Ah
   db "Coded by Asterix",0Dh,0Ah
   db 0Dh,0Ah
   db 'Built on '
date
db ' at '
time
db 0


ALIGN DWORD                  ; [esp+4]        [esp+4*2]      [esp+4*3]         [esp+4*4]
NtOpenProcessHookProc equ $  ; ProcessHandle, DesiredAccess, ObjectAttributes, ClientId
mov eax, DWORD PTR [esp+4*4]
db  081h, 038h      ; cmp  DWORD PTR [eax], OllyDbgPid
OllyDbgPid  dd  0
mov   eax, STATUS_ACCESS_DENIED
db 074h, 017h  ; je @F
NtOpenProcessVA equ $ + 1
  mov   eax, 090909090h
  push  DWORD PTR [esp+4*4]
  push  DWORD PTR [esp+4*4]
  push  DWORD PTR [esp+4*4]
  push  DWORD PTR [esp+4*4]
  call  eax
;@@:
  retn  4*4
NtOpenProcessHookProc_len equ $ - NtOpenProcessHookProc


ALIGN DWORD                              ; [esp+4]              [esp+4*2]                      [esp+4*3]                 [esp+4*4]                       [esp+4*5]
NtQueryInformationProcessHookProc equ $  ; ProcessHandle:DWORD, ProcessInformationClass:DWORD, ProcessInformation:DWORD, ProcessInformationLength:DWORD, ReturnLength:DWORD
  cmp   DWORD PTR [esp+4], 0FFFFFFFFh
  db    75h, 0Dh ; jne @F
  cmp   DWORD PTR [esp+4*2], ProcessDebugPort
  db    75h, 06h ; jne @F
  xor   eax, eax
  mov   DWORD PTR [esp+4], eax
;@@:
NtQueryInformationProcessVA equ $ + 1
  mov   eax, 090909090h
  push  DWORD PTR [esp+4*5]
  push  DWORD PTR [esp+4*5]
  push  DWORD PTR [esp+4*5]
  push  DWORD PTR [esp+4*5]
  push  DWORD PTR [esp+4*5]
  call  eax
  retn  4*5
NtQueryInformationProcessHookProc_len equ $ - NtQueryInformationProcessHookProc


ALIGN DWORD
RtlRaiseExceptionHookProc equ $
  mov   edx, [esp+4]
  assume edx:ptr EXCEPTION_RECORD
  cmp   [edx].ExceptionCode, DBG_PRINTEXCEPTION_C
  db    75h, 27h ; jne @F
  cmp   [edx].ExceptionFlags, 0
  db    75h, 21h ; jne @F
  cmp   [edx].pExceptionRecord, NULL
  db    75h, 1Bh ; jne @F
  cmp   [edx].NumberParameters, 2
  db    75h, 15h ; jne @F
  mov   eax, [edx].ExceptionInformation[4]
  xor   ecx, ecx
;@L1:
  cmp   ecx, [edx].ExceptionInformation
  db    74h, 0Bh ; je @F
  cmp   BYTE PTR [eax + ecx], '%'
  db    74h, 0Dh ; je @L2
  add   ecx, 1
  db    0EBh, 0F0h ; jmp @L1
;@@:
RtlRaiseExceptionVA equ $ + 1
  mov   eax, 090909090h
  push  edx
  assume edx:nothing
  call  eax
;@L2:
  retn  4
RtlRaiseExceptionHookProc_len equ $ - RtlRaiseExceptionHookProc



.DATA?

ALIGN DWORD
hinst                                   dd  ?
hwmain                                  dd  ?   ; Handle of main OllyDbg window
PID                                     dd  ?
fDebugActiveProcessStop                 dd  ?
fNoWarning                              dd  ?
fDoIsDebuggerPresentKill                dd  ?
fDoSetOpenProcessHook                   dd  ?
fDoSetNtQueryInformationProcessHook     dd  ?
fDoSetRtlRaiseExceptionHook             dd  ?
fHeapCheckingTricks                     dd  ?
fEnableDetach                           dd  ?
fDoNotSetOpenProcessHook                dd  ?
fDoNotSetNtQueryInformationProcessHook  dd  ?
fDoNotSetRtlRaiseExceptionHook          dd  ?
fIsTextSet                              dd  ?
fFirstLOAD_DLL_DEBUG_EVENT              dd  ?
fSecondLOAD_DLL_DEBUG_EVENT             dd  ?
fApplyChangesMessage                    dd  ?
fIsDebuggerPresentSignOk                dd  ?
ALIGN DWORD
pDebugEvent                             dd  ?
ALIGN DWORD
sBuffer                                 db  1024 dup (?)
BuffForWindowText                       db  1024 dup (?)
BuffForWindowText2                      db  1024 dup (?)
Buf                                     db    32 dup (?)

.CODE

ALIGN DWORD
DllEntry proc hi:DWORD, reason:DWORD, reserved1:DWORD
option PROLOGUE:NONE
option EPILOGUE:NONE
  .IF (DWORD PTR [esp+8]) == DLL_PROCESS_ATTACH     ; When the dll is loaded
     push DWORD PTR [esp+4]
     pop hinst
  .ENDIF
  xor eax, eax
  inc eax         ; mov eax, TRUE
  ret (sizeof DWORD)*3
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
DllEntry Endp

ALIGN DWORD
ODBG_Plugindata proc C shortname:ptr byte
option PROLOGUE:NONE
option EPILOGUE:NONE
  invoke lstrcpy, [esp+8], OFFSET szPluginName
  mov eax, PLUGIN_VERSION
  ret
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
ODBG_Plugindata endp

ALIGN DWORD              ; [esp+4]               [esp+8]
ODBG_Plugininit proc C ollydbgversion:dword, hw:HWND, features:ptr dword
option PROLOGUE:NONE
option EPILOGUE:NONE
 push ebp
 push ebx
 push esi
 push edi
 .IF (DWORD PTR [esp+4+4*4]) >= PLUGIN_VERSION
   .IF $invoke(CheckOS) != 0
     push DWORD PTR [esp+8+4*4]
     pop hwmain
     mov OllyDbgPid, $invoke(GetCurrentProcessId)
     mov fIsDebuggerPresentSignOk, $invoke(CheckSignature, $invoke(GetCurrentProcess), OFFSET szKernel32, OFFSET szIsDebuggerPresent, OFFSET IsDebuggerPresentSignature, 14)
     xor eax, eax
     mov fNoWarning, eax
     mov fDoIsDebuggerPresentKill, eax
     mov fDoSetOpenProcessHook, eax
     mov fDoSetNtQueryInformationProcessHook, eax
     mov fDoSetRtlRaiseExceptionHook, eax
     mov fHeapCheckingTricks, eax
     mov fEnableDetach, eax
     mov ebx, OFFSET sBuffer
;;     int 3
     .IF ($invoke(GetModuleFileName, hinst, ebx, SIZEOF sBuffer))
@loop:
        .IF BYTE PTR [ebx+eax-1] != '.'
           dec eax
           jnz @loop
           jmp @bad_exit
        .ENDIF
        mov DWORD PTR [ebx+eax], 'ini'
                invoke WritePrivateProfileString, NULL, NULL, NULL, ebx
                mov esi, OFFSET Buf
                invoke GetPrivateProfileString, $CTA0("Options"), $CTA0("NoWarning"),\
                                                $CTA0(), esi, SIZEOF Buf, ebx
                .IF eax != 0
                   .IF BYTE PTR [esi] == '1'
                      inc fNoWarning
                   .ENDIF
                .ENDIF
                invoke GetPrivateProfileString, $CTA0("Options"), $CTA0("Parameters"),\
                                                $CTA0(), esi, SIZEOF Buf, ebx
                .IF eax == 0
                   invoke WritePrivateProfileString, $CTA0("Options"), $CTA0("Parameters"),\
                                                     $CTA0("1000001"), ebx
                   invoke GetPrivateProfileString, $CTA0("Options"), $CTA0("Parameters"),\
                                                   $CTA0(), esi, SIZEOF Buf, ebx
                   test eax, eax
                   jz @bad_exit
                .ENDIF
                mov edi, OFFSET fDoIsDebuggerPresentKill
                mov bl, '1'
                xor ebp, ebp
                .IF (BYTE PTR [esi]) == bl
                   .IF (fIsDebuggerPresentSignOk)
                      inc BYTE PTR [edi]
                      inc ebp
                   .ELSE
                      invoke ErrorToLog, $CTA0(" \[Failed to apply protection against I\*sDebuggerPresent\]")
                   .ENDIF
                .ENDIF
                .IF (BYTE PTR [esi+2]) == bl
                   inc BYTE PTR [edi+(OFFSET fDoSetOpenProcessHook - OFFSET fDoIsDebuggerPresentKill)]
                   inc ebp
                .ENDIF
                .IF (BYTE PTR [esi+3]) == bl
                   inc BYTE PTR [edi+(OFFSET fDoSetNtQueryInformationProcessHook - OFFSET fDoIsDebuggerPresentKill)]
                   inc ebp
                .ENDIF
                .IF (BYTE PTR [esi+4]) == bl
                   inc BYTE PTR [edi+(OFFSET fDoSetRtlRaiseExceptionHook - OFFSET fDoIsDebuggerPresentKill)]
                   inc ebp
                .ENDIF
                .IF (BYTE PTR [esi+5]) == bl
                   .IF (fIsDebuggerPresentSignOk)
                      inc BYTE PTR [edi+(OFFSET fHeapCheckingTricks - OFFSET fDoIsDebuggerPresentKill)]
                      inc ebp
                   .ELSE
                      invoke ErrorToLog, $CTA0(" \[Failed to apply protection against Heap-checking tricks\]")
                   .ENDIF
                .ENDIF
                .IF (BYTE PTR [esi+6]) == bl
                   inc BYTE PTR [edi+(OFFSET fEnableDetach - OFFSET fDoIsDebuggerPresentKill)]
                   inc ebp
                .ENDIF
                mov edi, $invoke(GetModuleHandle, NULL)
                .IF (ebp)
                    invoke SetHook, OFFSET WaitForDebugEventHook, edi, OFFSET szKernel32, OFFSET szWaitForDebugEvent
                    .IF eax != 0
                       invoke SetHook, OFFSET ContinueDebugEventHook, edi, OFFSET szKernel32, OFFSET szContinueDebugEvent
                       test eax, eax
                       jnz @F
                       invoke RemoveHook, OFFSET WaitForDebugEventHook, edi, OFFSET szKernel32, OFFSET szWaitForDebugEvent
                       .IF eax == 0
                          invoke ErrorMessage, $CTA0("Critical error.")
                       .ENDIF
                    .ENDIF
                    invoke ErrorToLog, $CTA0(" \[An error occurred\]")
                .ENDIF
@@:
                .IF BYTE PTR [esi+1] == bl
                   invoke SetHook, OFFSET SetWindowTextHook, edi, OFFSET szUser32, OFFSET szSetWindowText
                   .IF eax != 0
                      invoke SetHook, OFFSET GetWindowTextHook, edi, OFFSET szUser32, OFFSET szGetWindowText
                      .IF eax != 0
                         .IF ($invoke(GetWindowText, hwmain, OFFSET BuffForWindowText2, SIZEOF BuffForWindowText2))
                            invoke SetWindowTextHook, hwmain, OFFSET BuffForWindowText2
                         .ENDIF
                         jmp @F
                      .ENDIF
                      invoke RemoveHook, OFFSET SetWindowTextHook, edi, OFFSET szUser32, OFFSET szSetWindowText
                      .IF eax == 0
                         invoke ErrorMessage, $CTA0("Critical error.")
                      .ENDIF
                   .ENDIF
                   invoke ErrorToLog, $CTA0(" \[Failed to apply protection against F\*indWindow/E\*numWindows\]")
                .ENDIF
@@:
                invoke Addtolist, 0, 0, OFFSET szPluginNameAndVersion
                invoke Addtolist, 0, -1, OFFSET szCopyright
                xor eax, eax
                jmp @return
     .ENDIF
   .ENDIF
 .ENDIF
@bad_exit:
 xor eax, eax
 dec eax
@return:
 pop edi
 pop esi
 pop ebx
 pop ebp
 ret
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
ODBG_Plugininit endp

ALIGN DWORD
ErrorToLog proc uses esi edi lpString:DWORD
LOCAL sBuff[128]:CHAR
  mov esi, OFFSET szPluginName
  lea edi, sBuff
  mov ecx, szPluginName_len
  cld
  rep movsb
  mov esi, lpString
@@:
  inc ecx
  cmp BYTE PTR [esi+ecx-1], 0
  jne @B
  rep movsb
  invoke Addtolist, 0, 1, ADDR sBuff
  ret
ErrorToLog endp

ALIGN DWORD
ErrorMessage proc lpszMessage:DWORD
option PROLOGUE:NONE
option EPILOGUE:NONE
  invoke MessageBox, hwmain, [esp+0Ch], OFFSET szPluginName, MB_OK or MB_ICONERROR
  ret (sizeof DWORD)
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
ErrorMessage endp

ALIGN DWORD
CheckSignature proc uses ebx esi edi hProcess:DWORD, lpszNameOfDll:DWORD, lpszNameOfApi:DWORD, lpSignature:DWORD, SignatureLength:DWORD
LOCAL sBuff[32]:CHAR
  invoke GetModuleHandle, lpszNameOfDll
  .IF eax != NULL
      invoke GetProcAddress, eax, lpszNameOfApi
      .IF eax != NULL
          mov ebx, eax
          mov edi, SignatureLength
          invoke ReadProcessMemory, hProcess, ebx, ADDR sBuff, edi, NULL
          .IF eax != 0
              mov ecx, edi
              lea esi, sBuff
              mov edi, lpSignature
              cld
              repe cmpsb
              je @Ok
              xor ebx, ebx
@Ok:
              mov eax, ebx
          .ENDIF
      .ENDIF
  .ENDIF
  ret
CheckSignature endp

ALIGN DWORD
WriteToMemory proc hProcess:DWORD, AddressToWrite:DWORD, lpBytesToWrite:DWORD, nSize:DWORD
LOCAL OldProtect:DWORD
  invoke WriteProcessMemory, hProcess, AddressToWrite, lpBytesToWrite,\
                             nSize, NULL
  .IF eax == 0
     invoke VirtualProtectEx, hProcess, AddressToWrite, nSize,\
                              PAGE_READWRITE, ADDR OldProtect
     .IF eax != 0
        invoke WriteProcessMemory, hProcess, AddressToWrite, lpBytesToWrite,\
                                   nSize, NULL
        push eax
        invoke VirtualProtectEx, hProcess, AddressToWrite, nSize,\
                                 OldProtect, ADDR OldProtect
        pop eax
     .ENDIF
  .ENDIF
  ret
WriteToMemory endp

ALIGN DWORD
ODBG_Pluginmenu proc C origin:dword, data:ptr byte, item:dword
option PROLOGUE:NONE
option EPILOGUE:NONE
  mov eax, [esp+4]
  .IF eax == PM_MAIN
     .IF (fEnableDetach)
        invoke lstrcpy, [esp+8+4], $CTA0("0 &Detach|1 &Options|2 &About")
     .ELSE
        invoke lstrcpy, [esp+8+4], $CTA0("1 &Options|2 &About")
     .ENDIF
     xor eax, eax
     inc eax
     jmp @F
  .ENDIF
  xor eax, eax
@@:
  ret
option PROLOGUE:PROLOGUEDEF
option EPILOGUE:EPILOGUEDEF
ODBG_Pluginmenu endp

ALIGN DWORD
ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
   mov eax, origin
   .IF eax == PM_MAIN
      mov eax, action
       .IF eax == 0
         .IF ($invoke(GetProcAddress, $invoke(GetModuleHandle, OFFSET szKernel32), $CTA0("D\*ebugActiveProcessStop")))
           .IF (PID) && (!(fDebugActiveProcessStop))
              push PID
              call eax
              .IF (eax)
                 inc fDebugActiveProcessStop
                 invoke MessageBox, hwmain, $CTA0("Debugger has been successfully detached."), OFFSET szPluginName, MB_OK or MB_ICONINFORMATION
              .ELSE
                 invoke ErrorMessage, $CTA0("Unable to detach debugger.")
              .ENDIF
           .ENDIF
         .ELSE
           invoke ErrorMessage, $CTA0("Your system not supports D\*ebugActiveProcessStop function.")
         .ENDIF
       .ELSEIF eax == 1
          invoke DialogBoxParam, hinst, 300, hwmain, OFFSET OptionsDlgProc, 0
       .ELSEIF eax == 2
          invoke MessageBox, hwmain, OFFSET sz, OFFSET szPluginName, MB_OK or MB_ICONINFORMATION
       .ENDIF
   .ENDIF
   ret
ODBG_Pluginaction endp

; Function is called when user opens new or restarts current application.
; Plugin should reset internal variables and data structures to initial state.
ALIGN DWORD
ODBG_Pluginreset proc C
  xor eax, eax
  mov PID, eax
  mov fDebugActiveProcessStop, eax
  mov fIsTextSet, eax
  mov fDoNotSetOpenProcessHook, eax
  mov fDoNotSetNtQueryInformationProcessHook, eax
  mov fDoNotSetRtlRaiseExceptionHook, eax
  mov fFirstLOAD_DLL_DEBUG_EVENT, eax
  mov fSecondLOAD_DLL_DEBUG_EVENT, eax
  ret
ODBG_Pluginreset endp

ALIGN DWORD
OptionsDlgProc proc uses ebx esi hDlg, uMsg, wParam, lParam
  mov eax, uMsg
  WM_CASE eax, <INITDIALOG, COMMAND, CLOSE>
@r0:
  xor eax, eax
  jmp @r
@INITDIALOG:
  and fApplyChangesMessage, FALSE
  mov ebx, OFFSET Buf
  mov esi, IDC_CHECKBOX1
@@:
  .IF BYTE PTR [ebx] == '1'
     invoke CheckDlgButton, hDlg, esi, BST_CHECKED
  .ENDIF
  inc ebx
  inc esi
  cmp esi, (IDC_CHECKBOX7+1)
  jne @B
  jmp @r1
@COMMAND:
  mov eax, wParam
  cmp ax, IDCANCEL
  je @button2
  cmp ax, IDC_BUTTON2
  je @button2
  cmp ax, IDC_BUTTON1
  jne @r0
     mov ebx, OFFSET Buf
     mov esi, IDC_CHECKBOX1
@@:
     invoke IsDlgButtonChecked, hDlg, esi
     .IF eax == BST_CHECKED
        mov BYTE PTR [ebx], '1'
     .ELSE
        mov BYTE PTR [ebx], '0'
     .ENDIF
     inc ebx
     inc esi
     cmp esi, (IDC_CHECKBOX7+1)
     jne @B
     mov BYTE PTR [ebx], 0
     invoke WritePrivateProfileString, $CTA0("Options"), $CTA0("Parameters"),\
                                       OFFSET Buf, OFFSET sBuffer
     test eax, eax
     jz @r1
       mov fApplyChangesMessage, TRUE
     jmp @r1
@button2:
  invoke SendMessage, hDlg, WM_CLOSE, 0, 0
  jmp @r1
@CLOSE:
  .IF fNoWarning != TRUE
     .IF fApplyChangesMessage == TRUE
        invoke MessageBox, hDlg,\
                           $CTA0("You must restart OllyDbg for the changes to take effect."),\
                           OFFSET szPluginName, MB_OK or MB_ICONINFORMATION
     .ENDIF
  .ENDIF
  invoke EndDialog, hDlg, 0
@r1:
  xor eax, eax
  inc eax
@r:
  ret
OptionsDlgProc endp

ALIGN DWORD
SetHook proc lpHookProc:DWORD, BaseOfImage:DWORD, lpszNameOfDll:DWORD, lpszNameOfApi:DWORD
   invoke GetModuleHandle, lpszNameOfDll
   invoke GetProcAddress, eax, lpszNameOfApi
   test eax, eax
   jz @F
   invoke GetAddressOfVAForCurrentApiInIAT, BaseOfImage, lpszNameOfDll, eax
   test eax, eax
   jz @F
   push SIZEOF DWORD
   lea edx, lpHookProc
   push edx
   push eax
   invoke GetCurrentProcess
   push eax
   call WriteToMemory
@@:
   ret
SetHook endp


ALIGN DWORD
WaitForDebugEventHook proc uses ebx lpDebugEvent:DWORD, dwMilliseconds:DWORD
  mov ebx, lpDebugEvent
  mov pDebugEvent, ebx
  invoke WaitForDebugEvent, ebx, dwMilliseconds
  .IF (fDebugActiveProcessStop)
     assume ebx:ptr DEBUG_EVENT
     mov [ebx].dwDebugEventCode, EXIT_PROCESS_DEBUG_EVENT
     mov eax, PID
     mov [ebx].dwProcessId, eax
     .IF ($invoke(Plugingetvalue, VAL_MAINTHREADID))
        mov [ebx].dwThreadId, eax
     .ENDIF
     mov [ebx].u._ExitProcess.dwExitCode, STILL_ACTIVE
     assume ebx:nothing
     xor eax, eax
     inc eax
  .ENDIF
  ret
WaitForDebugEventHook endp


ALIGN DWORD
ContinueDebugEventHook proc dwProcessId:DWORD, dwThreadId:DWORD, dwContinueStatus:DWORD
  push dwProcessId
  pop PID
   .IF dwContinueStatus == DBG_CONTINUE
   mov eax, pDebugEvent
   assume eax:ptr DEBUG_EVENT
     .IF [eax].dwDebugEventCode == LOAD_DLL_DEBUG_EVENT
        .IF (fHeapCheckingTricks)
           .IF (!(fFirstLOAD_DLL_DEBUG_EVENT))
              invoke IsDebuggerPresentKill, dwProcessId, 0
              mov fFirstLOAD_DLL_DEBUG_EVENT, 1
           .ELSEIF (fFirstLOAD_DLL_DEBUG_EVENT) && (!(fSecondLOAD_DLL_DEBUG_EVENT))
              invoke IsDebuggerPresentKill, dwProcessId, 1
              mov fSecondLOAD_DLL_DEBUG_EVENT, 1
           .ENDIF
        .ENDIF
     .ELSEIF [eax].dwDebugEventCode == EXCEPTION_DEBUG_EVENT
         assume eax:nothing
        .IF fDoIsDebuggerPresentKill == TRUE
           invoke IsDebuggerPresentKill, dwProcessId, 0
        .ENDIF
        .IF (fDoSetOpenProcessHook) && (!(fDoNotSetOpenProcessHook))
            invoke SetRemoteHook, $CTA0("ntdll.dll"),\
                                  $CTA0("NtOpen\*Process"),\
                                  OFFSET szKernel32,\
                                  OFFSET NtOpenProcessHookProc,\
                                  NtOpenProcessHookProc_len,\
                                  OFFSET NtOpenProcessVA
            .IF eax == 0
                invoke ErrorToLog, $CTA0(" \[Failed to apply protection against T\*erminateProcess\]")
            .ENDIF
            inc fDoNotSetOpenProcessHook
        .ENDIF
        .IF (fDoSetNtQueryInformationProcessHook) && (!(fDoNotSetNtQueryInformationProcessHook))
            invoke SetRemoteHook, $CTA0("ntdll.dll"),\
                                  $CTA0("Nt\*QueryInformationProcess"),\
                                  OFFSET szKernel32,\
                                  OFFSET NtQueryInformationProcessHookProc,\
                                  NtQueryInformationProcessHookProc_len,\
                                  OFFSET NtQueryInformationProcessVA
            .IF eax == 0
               invoke ErrorToLog, $CTA0(" \[Failed to apply protection against Unhandled exception tricks\]")
            .ENDIF
            inc fDoNotSetNtQueryInformationProcessHook
        .ENDIF
        .IF (fDoSetRtlRaiseExceptionHook) && (!(fDoNotSetRtlRaiseExceptionHook))
            invoke SetRemoteHook, $CTA0("ntdll.dll"),\
                                  $CTA0("R\*tlRaiseException"),\
                                  OFFSET szKernel32,\
                                  OFFSET RtlRaiseExceptionHookProc,\
                                  RtlRaiseExceptionHookProc_len,\
                                  OFFSET RtlRaiseExceptionVA
            .IF eax == 0
               invoke ErrorToLog, $CTA0(" \[Failed to apply protection against O\*utputDebugString exploit\]")
            .ENDIF
            inc fDoNotSetRtlRaiseExceptionHook
        .ENDIF
     .ENDIF
   .ENDIF
   invoke ContinueDebugEvent, dwProcessId, dwThreadId, dwContinueStatus
   ret
ContinueDebugEventHook endp


ALIGN DWORD
IsDebuggerPresentKill proc uses ebx dwProcessId:DWORD, bSetByte:DWORD
LOCAL hProcess:DWORD
LOCAL Buff:DWORD
 invoke OpenProcess, PROCESS_VM_OPERATION or \
                     PROCESS_VM_READ or PROCESS_VM_WRITE, FALSE, dwProcessId
 .IF eax != NULL
   mov hProcess, eax
   .IF $invoke(Plugingetvalue, VAL_MAINTHREADID) != 0
     .IF $invoke(Findthread, eax) != NULL
       assume eax:ptr t_thread
       mov ebx, [eax].reg.base[4*4]
       assume eax:nothing
       add ebx, 30h
       invoke ReadProcessMemory, hProcess, ebx, ADDR Buff, SIZEOF DWORD, NULL
       .IF eax != 0
         mov ebx, Buff
         add ebx, 2
         invoke ReadProcessMemory, hProcess, ebx, ADDR Buff, SIZEOF BYTE, NULL
         .IF eax != 0
           .IF (bSetByte)
              mov BYTE PTR [Buff], 1
           .ELSE
              mov BYTE PTR [Buff], 0
           .ENDIF
             invoke WriteToMemory, hProcess, ebx, ADDR Buff, SIZEOF BYTE
         .ENDIF
       .ENDIF
     .ENDIF
   .ENDIF
   invoke CloseHandle, hProcess
 .ENDIF
 ret
IsDebuggerPresentKill endp

;;-----------------------------------------------------------------
;; код API функции IsDebuggerPresent из kernel32.dll (windows 2000)
;;-----------------------------------------------------------------
;;.text:77E81828 IsDebuggerPresent proc near             ; CODE XREF: .text:77EC80FEp
;;.text:77E81828                 mov     eax, large fs:18h
;;.text:77E8182E                 mov     eax, [eax+30h]
;;.text:77E81831                 movzx   eax, byte ptr [eax+2]
;;.text:77E81835                 retn
;;.text:77E81835 IsDebuggerPresent endp

ALIGN DWORD
RemoveHook proc VAOfHookProc:DWORD, BaseOfImage:DWORD, lpszNameOfDll:DWORD, lpszNameOfApi:DWORD
LOCAL AddressOfUnHookProc:DWORD
   invoke GetModuleHandle, lpszNameOfDll
   invoke GetProcAddress, eax, lpszNameOfApi
   test eax, eax
   jz @F
   mov AddressOfUnHookProc, eax
   invoke GetAddressOfVAForCurrentApiInIAT, BaseOfImage, lpszNameOfDll, VAOfHookProc
   test eax, eax
   jz @F
   push SIZEOF DWORD
   lea edx, AddressOfUnHookProc
   push edx
   push eax
   invoke GetCurrentProcess
   push eax
   call WriteToMemory
@@:
   ret
RemoveHook endp

ALIGN DWORD
SetWindowTextHook proc hWnd:DWORD, lpString:DWORD
  mov eax, hWnd
  .IF eax == hwmain
      invoke lstrcpy, OFFSET BuffForWindowText, lpString
      .IF eax != NULL
          mov fIsTextSet, TRUE
          mov eax, $CTA0()
          jmp @F
      .ENDIF
  .ENDIF
  mov eax, lpString
@@:
  invoke SetWindowText, hWnd, eax
  ret
SetWindowTextHook endp

ALIGN DWORD
GetWindowTextHook proc hWnd:DWORD, lpString:DWORD, nMaxCount:DWORD
  mov eax, hWnd
  .IF eax == hwmain
      .IF fIsTextSet == TRUE
          invoke lstrcpy, lpString, OFFSET BuffForWindowText
          jmp @F
      .ENDIF
  .ENDIF
  invoke GetWindowText, hWnd, lpString, nMaxCount
@@:
  ret
GetWindowTextHook endp


ALIGN DWORD
SetRemoteHook proc uses ebx esi lpHookedModuleName:DWORD, lpHookedProcName:DWORD, lpRemoteModuleName:DWORD, lpHookProc:DWORD, HookProcLength:DWORD, lpHookedProcVA:DWORD
LOCAL hProcess:DWORD
LOCAL Buff:DWORD
 .IF ($invoke(Plugingetvalue, VAL_HPROCESS))
    mov hProcess, eax
    .IF ($invoke(GetProcAddress, $invoke(GetModuleHandle, lpHookedModuleName), lpHookedProcName))
       mov ebx, eax
       mov ecx, lpHookedProcVA
       mov [ecx], eax
       .IF ($invoke(GetAddressOfVAForCurrentApiInIAT, $invoke(GetModuleHandle, lpRemoteModuleName), lpHookedModuleName, ebx))
          mov esi, eax
          .IF ($invoke(ReadProcessMemory, hProcess, esi, ADDR Buff, SIZEOF DWORD, NULL))
             .IF Buff == ebx
                .IF ($invoke(VirtualAllocEx, hProcess, NULL, HookProcLength, MEM_COMMIT, PAGE_EXECUTE_READWRITE))
                   mov Buff, eax
                   .IF ($invoke(WriteProcessMemory, hProcess, eax, lpHookProc, HookProcLength, NULL))
                      invoke WriteToMemory, hProcess, esi, ADDR Buff, SIZEOF DWORD
                      test eax, eax
                      jnz @F
                   .ENDIF
                   invoke VirtualFreeEx, hProcess, Buff, 0, MEM_RELEASE
                   xor eax, eax
                .ENDIF
             .ENDIF
          .ENDIF
       .ENDIF
    .ENDIF
 .ENDIF
@@:
 ret
SetRemoteHook endp


End DllEntry

:make
SET PROJECTNAME=HideDebugger
if exist %PROJECTNAME%.res del %PROJECTNAME%.res
if exist %PROJECTNAME%.obj del %PROJECTNAME%.obj
if exist %PROJECTNAME%.dll del %PROJECTNAME%.dll
if exist %PROJECTNAME%.exp del %PROJECTNAME%.exp
if exist %PROJECTNAME%.lib del %PROJECTNAME%.lib
%masm32%\bin\ml /nologo /c /coff /Cp %PROJECTNAME%.bat
%masm32%\bin\rc %PROJECTNAME%.rc
%masm32%\bin\link /nologo /opt:nowin98 /DLL /DEF:%PROJECTNAME%.def /MERGE:.rdata=.text /SECTION:.text,ER /SUBSYSTEM:WINDOWS /LIBPATH:%masm32%\lib %PROJECTNAME%.obj %PROJECTNAME%.res
del %PROJECTNAME%.res
del %PROJECTNAME%.obj
del %PROJECTNAME%.exp
del %PROJECTNAME%.lib
echo.
pause
cls
