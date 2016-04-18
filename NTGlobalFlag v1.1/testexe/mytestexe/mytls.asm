.386
.model flat, stdcall
option casemap:none
include       \masm32\include\windows.inc
include       \masm32\include\kernel32.inc
includelib    \masm32\lib\kernel32.lib
includelib    kernl.lib
include       \masm32\include\user32.inc
includelib    \masm32\lib\user32.lib

; the code below is  copied and modified  a little from elicz's tlsinasm :) 
; you can get the original from his site 
; www.anticracking.sk/EliCZ/ which shows 
; lot more including a dll and thread 
; the original exe taken for example is iczelions tut02 aka MessageBox tutorial 

.CONST
_tls_array EQU    2CH      ;FS:[2CH] - pointer to array of pointers to copied tls blocks

.DATA?
_tls_index DWORD  ?

;pro forma create section with name .tls
;(but tls can be placed anywhere)
   OPTION         DOTNAME
.tls   SEGMENT
_tls_start LABEL  DWORD
 DWORD     80H    DUP ("slt.") 
_tls_end   LABEL  DWORD
.tls   ENDS
   OPTION         NODOTNAME

.DATA
__xl_a DWORD TlsCallBack0 , TlsCallBack1
__xl_z DWORD 0    ;null terminated list of pointers to callback procedures

MsgCaption      db "Iczelion's tutorial no.2",0
MsgBoxText      db "Win32 Assembly is Great!",0



TLS_DIRECTORY    STRUCT
 lpTlsDataStart LPDWORD ? ;copy block starting here
 lpTlsDataEnd   LPDWORD ? ;and ending here + block (size=ZeroFillSize) filled with 0 to
 lpTlsIndex     LPDWORD ? ;DS:[FS:[2CH]]+TlsIndex*4
 lpTlsCallbacks LPDWORD ? ;pointer to 0 terminated array of pointers to callbacks
 ZeroFillSize     DWORD ? ;overall size=lpTlsDataEnd-lpTlsDataStart+ZeroFillSize
 Characteristic   DWORD ? ;reserved
TLS_DIRECTORY      ENDS

PUBLIC _tls_used ;this name is required and must be PUBLIC!!!!
_tls_used TLS_DIRECTORY <_tls_start, _tls_end, _tls_index, __xl_a, 0, ?>

Messagebox PROTO
Thread PROTO 

.code
start:
    INVOKE  CreateThread, NULL, NULL, OFFSET Thread, ESP, NULL, ESP
    push eax
   invoke WaitForSingleObject ,eax,INFINITE
    POP     EAX
   INVOKE  CloseHandle, EAX


   
      invoke MessageBox, NULL,addr MsgBoxText, addr MsgCaption, MB_OK
	invoke ExitProcess,NULL
   


Thread    PROC 
   RET
 Thread    ENDP


.code mysection

TlsCallBack0   PROC    hinstImg, fdwReason, lpvReserved
   invoke IsDebuggerPresent
   .if eax == 1
   invoke Messagebox
   .endif
    

   MOV     EAX, TRUE
   RET
 TlsCallBack0   ENDP

TlsCallBack1   PROC    hinstImg, fdwReason, lpvReserved
   invoke IsDebuggerPresent
   .if eax == 1
   invoke Messagebox
   .endif
    

   MOV     EAX, TRUE
   RET
 TlsCallBack1   ENDP

end start
