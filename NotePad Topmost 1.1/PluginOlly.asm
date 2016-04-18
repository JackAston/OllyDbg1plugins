; ==========================================================================
;
;                       NotePad Topmost 1.1 - FRET 2006
;
; coded by BeatriX 
; Plugin for OllyDebugger 1.10
; it is a plugin example coded in asm only !
; 
; Documentations : ICZELION - MSDN - KIT de développement Olly 1.10
; 
; Thanks to Gabri3l for suggestion
; 
; ==========================================================================

.386
.model flat,stdcall
option casemap:none
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\kernel32.lib
include \masm32\include\user32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\shell32.lib
include \masm32\include\shell32.inc
include \masm32\include\comctl32.inc
includelib \masm32\lib\comctl32.lib



.const

    WS_EX_LAYERED = 80000h
    LWA_ALPHA = 2
    PLUGIN_VERSION = 110
    PM_MAIN = 0
    PM_DISASM = 31

    m2m MACRO M1, M2
        push M2
        pop  M1
    ENDM
    
.data

    ; ******************************************** for Log data

    menu                BYTE    "NotePad Topmost",0
    sub_menus           BYTE    "0 Run NotePad,1 Parameters",0
    sub_menus_disasm    BYTE    "3 Copy to NotePad",0
    InfoLog             BYTE    "NotePad Topmost - FRET 2006",0
    InfoLog2            BYTE    "      coded in asm by BeatriX - copyright 2006",0

    ; ******************************************** For NotePad
    
    Accueil             BYTE    0Dh, 0Ah, "*** NotePad Topmost - FRET ***",0Dh, 0Ah, " ", 0
    ClipEnd             BYTE    "topmost..topmost...",0
    Dir_                BYTE    "%system%\",0
    NotePad_            BYTE    "NOTEPAD",0
    Classe_             BYTE    "Notepad",0
    Edit_               BYTE    "edit",0
    open_               BYTE    "open",0
    hLog                DWORD   0
    hInstance           DWORD   0
    hEdit               DWORD   0
    hNotePad            DWORD   0
    Opacity             DWORD   200


    ; ************************************************** For "Parameters" window
    ClassName           BYTE	"Opacity_NotePad",0
    TrackClass          BYTE    "msctls_trackbar32",0

    wc                  WNDCLASSEX <0>
    msg                 MSG     <0>
    hIcon               dword   0
    hInstance_          dword   0
    hWnd 		      dword   0
    hMem                dword   0
    hTrackBar           DWORD   0
    WxDim               equ 400		 
    WyDim               equ 80		 
    AppName             BYTE  "NotePad Topmost - Opacity value",0
   
.code
; **************************************************************************
DllEntry proc hInst:HINSTANCE, reason:DWORD, reserved1:DWORD
     push hInst
     pop hInstance
     mov  eax,TRUE
     ret
DllEntry Endp

; **************************************************************************
_ODBG_Plugindata proc 
    push ebp
    mov ebp, esp
    pushad
    mov edi, offset menu                
    cld
    mov ecx, -1
    xor eax, eax
    repne scasb
    not ecx

    mov edi, dword ptr [ebp+8]
    mov esi, offset menu
    rep movsb                           ; <------ Register menu
    
    popad
    mov eax, PLUGIN_VERSION
    pop ebp
    ret
_ODBG_Plugindata endp

; *************************************************************************
_ODBG_Plugininit proc 

    push offset InfoLog
    push 0
    call AddtoLogData               ; <--- Add InfoLog in Log data
    
    push offset InfoLog2            ; <--- Add InfoLog2 in Log data
    push -1
    call AddtoLogData

    mov eax, 0                      ; success
    ret 
_ODBG_Plugininit endp


; ************************************************************************
_ODBG_Pluginaction proc
    push ebp
    mov ebp, esp
    mov eax, dword ptr [ebp+0Ch]
    pushad
    .if eax==0
        call Open_NotePad
    .elseif eax==1
        call Param_Opacity
    .elseif eax==3
        call Copy2NotePad
    .endif

    popad
    pop ebp
    ret
_ODBG_Pluginaction endp

; ************************************************************************

_ODBG_Pluginmenu proc 
    push ebp
    mov ebp, esp
    pushad
    mov eax, dword ptr [ebp+8]
    cmp eax, PM_MAIN
    jne @F

    mov edi, offset sub_menus               
    cld
    mov ecx, -1
    xor eax, eax
    repne scasb
    not ecx
    mov edi, dword ptr [ebp+0Ch]
    mov esi, offset sub_menus
    rep movsb                               ; <--- Register sub_menus
@@:
    cmp eax, PM_DISASM
    jne @F
    mov edi, offset sub_menus_disasm               
    cld
    mov ecx, -1
    xor eax, eax
    repne scasb
    not ecx
    mov edi, dword ptr [ebp+0Ch]
    mov esi, offset sub_menus_disasm
    rep movsb                               ; <--- Register sub_menus for disasm windows

@@:

    popad
    mov eax, 1                              ; <-- eax = 1 (success)
    pop ebp
    ret

_ODBG_Pluginmenu endp 

include Includes\functions.inc

End DllEntry