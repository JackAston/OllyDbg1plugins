;@goto end

;############################################################################;

.586
.model flat,stdcall
option casemap:none

;############################################################################;

; ofs equ offset
; bpt equ byte ptr
; dpt equ dword ptr
; wpt equ word ptr

; include_ macro arg
;     include arg.inc
;     includelib arg.lib
; endm

    include     windows.inc
    include     my.mac
    include_    gdi32
    include_    user32
    include_    kernel32

ProcessImports PROTO :DWORD,:DWORD

SEH struct
    PrevLink        dd ? ; адрес предыдущего SEH-фрейма
    CurrentHandler  dd ? ; адрес обработчика исключений
    SafeOffset      dd ? ; Смещение безопасного места
    PrevEsp         dd ? ; Старое значение esp
    PrevEbp         dd ? ; Старое значение ebp
SEH ends

;############################################################################;

.data
           DB "DLL Loader (C) 2004 Oleh Yuschuk" ; magic string! don't change

; Link area. Never change the meaning or order of next 32 dwords!
ERRMSG     DD 0                        ; Pointer to error
HINST      DD 0                        ; Process instance
hWND       DD 0                        ; Handle of main window
DLLBASE    DD 0                        ; Base address of loaded DLL or NULL
           DD ofs Firstbp              ; Address of first breakpoint
           DD ofs Prepatch             ; Address of patch area before call
           DD ofs Arg1                 ; Base of 10 arguments x 1024 bytes
           DD ofs Finished             ; Address of breakpoint after call
DUMMY      DD 4 DUP(0)                 ; Reserved for the future
PROCADR    DD 0                        ; Address of procedure, starts execution
REGEAX     DD 0                        ; Register arguments
REGECX     DD 0
REGEDX     DD 0
REGEBX     DD 0
REGESI     DD 0
REGEDI     DD 0
NARG       DD 0                        ; Number of arguments to push on stack
ARGLIST    DD 10 DUP(0)                ; DLL argument list
ESPDIFF    DD 0                        ; Difference in ESP caused by code
           DD 0                        ; Reserved for the future

WCLASS     = THIS DWORD                ; Hand-made WNDCLASS structure
           DD 0000002Bh                ; CS_HREDRAW|VREDRAW|DBLCLKS|OWNDC
           DD WndProc                  ; Window procedure
           DD 0                        ; Class extra bytes
           DD 0                        ; Window extra bytes
WCINST     DD 0                        ; Instance
WCICON     DD 0                        ; Icon
HCURS      DD 0                        ; Cursor
HBGND      DD 0                        ; Background brush
           DD 0                        ; No menu
           DD CLSNAME                  ; Class name

msg        = THIS DWORD                ; Hand-made MSG structure
           DD 0                        ; Handle of window
MSGID      DD 0                        ; Message ID
           DD 0                        ; wParam
           DD 0                        ; lParam
           DD 0                        ; Timestamp
           DD 0                        ; X coordinate
           DD 0                        ; Y coordinate

PSTRUCT    = THIS DWORD                ; Hand-made PAINTSTRUCT structure
           DD 0                        ; HDC
           DD 0                        ; fErase
           DD 0                        ; rcPaint.left
           DD 0                        ; rcPaint.top
           DD 0                        ; rcPaint.right
           DD 0                        ; rcPaint.bottom
           DD 0                        ; fRestore
           DD 0                        ; fIncUpdate
           DB 32 DUP(0)                ; rgbReserved

ORIGESP    DD 0                        ; Original ESP before call
ORIGEBP    DD 0                        ; Original EBP before call
EXPESP     DD 0                        ; Expected ESP after call (C)
WNDNAME    DB "OllyDbg DLL Loader",0
CLSNAME    DB "LoadDLLClass",0
ICONAME    DB "MAINICON",0             ; Green smashed bug - igitt!
E_NONAM    DB "Missing DLL name",0     ; Error notifications to OllyDbg
E_NODLL    DB "Unable to load DLL",0
E_NPARM    DB "Too many parameters",0

           db "DLL Loader '06 Modified by UsAr [http://vir-detective.be/]",0

.data?
ALIGN 16
PUBLIC Arg1
PUBLIC Arg2
PUBLIC Arg3
PUBLIC Arg4
PUBLIC Arg5
PUBLIC Arg6
PUBLIC Arg7
PUBLIC Arg8
PUBLIC Arg9
PUBLIC Arg10

Arg1       DB 1024 DUP (?)             ; Area for 10 memory arguments, 1 k each
Arg2       DB 1024 DUP (?)
Arg3       DB 1024 DUP (?)
Arg4       DB 1024 DUP (?)
Arg5       DB 1024 DUP (?)
Arg6       DB 1024 DUP (?)
Arg7       DB 1024 DUP (?)
Arg8       DB 1024 DUP (?)
Arg9       DB 1024 DUP (?)
Arg10      DB 1024 DUP (?)

tmpbyte     db ?
tmpdword    dd ?
tmpdword2   dd ?

.code

;############################################################################;

PUBLIC WndProc
WndProc proc uses edx edi esi LP:DWORD,WP:DWORD,MS:DWORD,HW:DWORD

    mov     eax,[MS]
    CMP     eax,WM_CREATE
    JE      RET0
    CMP     eax,WM_DESTROY
    JNE     @@080
    invoke  PostQuitMessage,0
    JMP     RET0

@@080:
    CMP     eax,WM_PAINT
    JNE     @@100
    invoke  BeginPaint,[HW],ofs PSTRUCT
    invoke  EndPaint,[HW],ofs PSTRUCT
    JMP     RET0

@@100:
    CMP     eax,WM_CLOSE
    JNE     @@200
    invoke  DestroyWindow,[HW]
    JMP     RET0

@@200:     ; None of listed above, pass to DefWindowProc().
    invoke  DefWindowProcA,[HW],[MS],[WP],[LP]
    JMP     RETA

RET0:
    XOR     eax,eax
    JMP     RETA

RET1:
    xor     eax,eax
    inc     eax

RETA:
    RET
WndProc ENDP

ProcessImports proc uses esi edi ebx PImports:DWORD,dwSize:DWORD
local lpDllName:DWORD
local hDll:DWORD
local lpFuncAddr:DWORD
local lpFuncNameRef:DWORD
local lpImageImportByName:DWORD

;--------------------------------------------------;

;   int     3

    mov     esi,PImports
    add     dwSize,esi
    assume  esi:ptr IMAGE_IMPORT_DESCRIPTOR

;--------------------------------------------------;

@while1:
;   cmp     [esi].OriginalFirstThunk,0
;   je      @endproc

    mov     eax,[esi].Name1
    test    eax,eax
    je      @endproc
    add     eax,[DLLBASE]
    mov     lpDllName,eax

    invoke  GetModuleHandle,eax
    test    eax,eax
    jne     @F
    invoke  LoadLibrary,lpDllName
@@: mov     hDll,eax

;--------------------------------------------------;

;     mov     ecx,[DLLBASE]
;     .if [esi].TimeDateStamp == 0
;         mov     eax,[esi].FirstThunk
;     .else
;         mov     eax,[esi].OriginalFirstThunk
;     .endif
;     add     eax,ecx
;     mov     lpFuncNameRef,eax
;     mov     eax,[esi].FirstThunk
;     add     eax,ecx
;     mov     lpFuncAddr,eax

;     mov     ebx,lpFuncNameRef
    mov     ecx,[DLLBASE]
    mov     eax,[esi].OriginalFirstThunk
    test    eax,eax
    jne     @F
    mov     eax,[esi].FirstThunk
    test    eax,eax
    je      @endproc
    @@:

    add     eax,ecx
    mov     lpFuncNameRef,eax
    mov     eax,[esi].FirstThunk
    add     eax,ecx
    mov     lpFuncAddr,eax

    mov     ebx,lpFuncNameRef


;--------------------------------------------------;

@while2:
    cmp     dpt [ebx],0
    je      @endw

    mov     eax,[ebx]
    add     eax,[DLLBASE]
    mov     lpImageImportByName,eax

    test    dpt [ebx],IMAGE_ORDINAL_FLAG32
    je      @noneordinal

;--------------------------------------------------;

@ordinal:
    mov     eax,[ebx]
    and     eax,0FFFFh
    invoke  GetProcAddress,hDll,eax
    mov     ecx,lpFuncAddr

    pusha
    push    0
    invoke  VirtualProtect,ecx,4,PAGE_READWRITE,esp
    pop     eax
    popa

    mov     [ecx],eax
    jmp     @endif1


;--------------------------------------------------;

@noneordinal:
    mov     eax,lpImageImportByName
    assume  eax:ptr IMAGE_IMPORT_BY_NAME
    lea     eax,[eax].Name1

    invoke  GetProcAddress,hDll,eax
    mov     ecx,lpFuncAddr

    pusha
    push    0
    invoke  VirtualProtect,ecx,4,PAGE_READWRITE,esp
    pop     eax
    popa

    mov     [ecx],eax

;--------------------------------------------------;

@endif1:
    mov     eax,lpFuncAddr
    cmp     dpt [eax],0
    je      @endproc

    add     lpFuncAddr,4
    add     ebx,4
    jmp     @while2

;--------------------------------------------------;

@endw:
    add     esi,sizeof IMAGE_IMPORT_DESCRIPTOR
    cmp     dwSize,esi
    jb      @while1

;--------------------------------------------------;

@endproc:
    ret
ProcessImports endp

START:
    mov     ebp,esp                             ; Here execution begins
    invoke  GetModuleHandleA,0
    mov     WCINST,eax
    mov     HINST,eax
    invoke  GetCommandLineA                     ; Path to LOADDLL is taken into quotes
    mov     esi,eax
    inc     esi                                 ; skip first quote

@@10:
    mov     al,bpt [esi]                        ; Skip path to LOADDLL.EXE
    inc     esi
    or      al,al
    jne     @@12
    mov     dpt [ERRMSG],ofs E_NONAM
    jmp     ERROR

@@12:
    cmp     al,'"'
    jne     @@10

@@20:
    mov     al,bpt [esi]                        ; Skip spaces
    cmp     al,' '
    jne     @@30
    inc     esi
    jmp     @@20

@@30:
    invoke  LoadLibraryEx,esi,0,DONT_RESOLVE_DLL_REFERENCES
    or      eax,eax
    jne     @@32
    mov     dpt [ERRMSG],ofs E_NODLL
    jmp     ERROR

@@32:
    mov     dpt [DLLBASE],eax
    add     eax,dpt [eax+03Ch]
    assume  eax:ptr IMAGE_NT_HEADERS
    mov     esi,[eax].OptionalHeader.AddressOfEntryPoint
    add     esi,dpt [DLLBASE]
    mov     ecx,[eax].OptionalHeader.DataDirectory[(SIZEOF IMAGE_DATA_DIRECTORY * IMAGE_DIRECTORY_ENTRY_IMPORT)].isize
    mov     eax,[eax].OptionalHeader.DataDirectory[(SIZEOF IMAGE_DATA_DIRECTORY * IMAGE_DIRECTORY_ENTRY_IMPORT)].VirtualAddress
    add     eax,dpt [DLLBASE]
    invoke  ProcessImports,eax,ecx

    invoke  LoadIconA,[HINST],ofs ICONAME
    mov     dpt [WCICON],eax
    invoke  LoadCursorA,0,IDC_NO
    mov     dpt [HCURS],eax
    invoke  GetStockObject,WHITE_BRUSH
    mov     dpt [HBGND],eax
    invoke  RegisterClassA,ofs WCLASS
    invoke  CreateWindowExA,0,ofs CLSNAME,ofs WNDNAME,WS_OVERLAPPEDWINDOW or WS_VISIBLE,CW_USEDEFAULT,CW_USEDEFAULT,200,100,0,0,[HINST],0
    mov     dpt [hWND],eax                      ; Save handle
    invoke  ShowWindow,eax,SW_RESTORE

    mov     eax,[esp]
    push    1
    push    esi
    push    0
    push    1
    push    dpt [DLLBASE]
    push    eax
    jmp     esi

PUBLIC Firstbp
Firstbp:
    nop                                         ; First breakpoint is set here

WINLOOP:
    cmp     dpt [PROCADR],0                     ; Request to call some function?
    je      NOCALL
    mov     dpt [ORIGESP],esp
    mov     dpt [ORIGEBP],esp

    push    0                                   ; Security buffer (16 doublewords)
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0
    push    0

    mov     ecx,dpt [NARG]
    jecxz   @@44
    cmp     ECX,10
    jbe     @@40
    mov     dpt [ERRMSG],ofs E_NPARM
    jmp     ERROR

@@40:
    mov     eax,ofs ARGLIST

@@42:
    push    dpt [eax]                           ; Push requested number of arguments
    add     eax,4
    loop    @@42

@@44:
    mov     dpt [EXPESP],esp                    ; Expected ESP after return (C)
    mov     eax,dpt [REGEAX]                    ; Preset registers
    mov     ecx,dpt [REGECX]
    mov     edx,dpt [REGEDX]
    mov     ebx,dpt [REGEBX]
    mov     esi,dpt [REGESI]
    mov     edi,dpt [REGEDI]

PUBLIC Prepatch
Prepatch:
    db      16 dup (90h)                        ; Patch area before call

PUBLIC CallDLL
CallDLL:
    CALL    dpt [PROCADR]                       ; Call DLL function
    db      16 dup (90h)                        ; Patch area after call
    mov     dpt [REGEAX],eax                    ; Get modified registers

    mov     dpt [REGECX],ECX
    mov     dpt [REGEDX],EDX
    mov     dpt [REGEBX],EBX
    mov     dpt [REGESI],ESI
    mov     dpt [REGEDI],EDI
    mov     eax,esp
    sub     eax,dpt [EXPESP]
    mov     dpt [ESPDIFF],eax
    mov     ebp,dpt [ORIGEBP]
    mov     esp,dpt [ORIGESP]
    mov     dpt [PROCADR],0                     ; Confirm execution
    nop

PUBLIC Finished
Finished:
    int     3                                   ; Pause after execution
    nop

NOCALL:
    invoke  Sleep,0                             ; Be fair to other applications
    invoke  PeekMessageA,ofs msg,0,0,0,PM_REMOVE
    or      eax,eax
    jz      WINLOOP
    invoke  TranslateMessage,ofs msg
    invoke  DispatchMessageA,ofs msg
    mov     eax,dpt [MSGID]
    cmp     eax,WM_QUIT
    jne     WINLOOP
    invoke  ExitProcess,0                       ; Hasta la vista!

ERROR:
    invoke  ExitProcess,00001001h               ; Special return code, means error
    ALIGN   4

public Patcharea
Patcharea   DB 2047 dup(90h)                    ; Big patch area (2 K of NOPs)
Endpatch:
    nop

;############################################################################;

end	START

;############################################################################;

;:end
;@echo off
;set cmpl=\usasm
;set proj=%~n0
;%cmpl%\bin\ml /nologo /c /coff /Cp /I%cmpl%\include\ %proj%.bat
;%cmpl%\bin\link /align:0x10000 /SUBSYSTEM:WINDOWS /LIBPATH:%cmpl%\lib /MERGE:.rdata=.text /DEF:%proj%.def %proj%.obj loaddll.res
;del *.obj
