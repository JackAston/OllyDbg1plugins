; *********************************************************************************
; 
; push offset String
; call Copy2ClipBoard
; 
; *********************************************************************************

Copy2Clipboard PROC pString:DWORD
LOCAL   Handle_ :DWORD;
LOCAL   hMem_   : DWORD;
LOCAL   Size_   : DWORD;

    mov edi, pString
    xor ecx, ecx
    xor eax, eax
@@:
    cmp byte ptr [edi], 0
    je @F
    inc edi
    inc ecx
    jmp @B
@@:
    inc ecx
    mov Size_, ecx                  ; Get the string size 
    push Size_
    push GMEM_MOVEABLE
    call GlobalAlloc                ; allocate global memory
    mov Handle_, eax

@@:
    push Handle_
    call GlobalLock                 ; lock allocated memory
    mov hMem_, eax
    
    mov edi, hMem_
    mov esi, pString
    mov ecx, Size_
    rep movsb                       ; copy string in allocated memory
    
    push Handle_
    call GlobalUnlock               ; unlock allocated memory
    push 0        
    call OpenClipboard              ; open the clipboard
    call EmptyClipboard             ; empty the clipboard
    push Handle_
    push CF_TEXT
    call SetClipboardData           ; fill the clipboard
    call CloseClipboard             ; close the clipboard
    ret
Copy2Clipboard ENDP

; *********************************************************************************
; 
; call Copy2NotePad
; 
; *********************************************************************************
Copy2NotePad PROC 

    push 0
    push offset Classe_
    call FindWindow                             ; look for Notepad 
    cmp eax, 0
    jne @F
    call Open_NotePad
@@:
    ; ====================== use "CTRL+C" Olly routine = Copy to Clipboard
    
    mov eax, 4CDA31h
    mov eax, dword ptr [eax]
    push eax                    ; Address of the dump end 
    mov eax, 4CDA2Dh
    mov eax, dword ptr [eax]    ; Address of the dump start
    push eax
    push 4CD6A8h    
    mov eax, 4CD6A4h
    push dword ptr [eax]        ; Handle of Disasm windows
    mov eax, 44AE5Ch
    call eax                    ; CTRL+C routine
    add esp, 10h
    

   ; ======================== paste in the NotePad
    push 0
    push 0
    push WM_PASTE
    push hEdit
    call SendMessage                            ; paste in the NotePad

    ret
Copy2NotePad ENDP

; *********************************************************************************
; 
; push offset String
; push Color
; call AddtoLogData
; 
; *********************************************************************************

AddtoLogData proc Color:DWORD,pString:DWORD
    mov esi, 4EACB0h
    mov esi, dword ptr [esi]        ; Address of table logs
    mov ecx, 4EADE4h
    mov ecx, dword ptr [ecx]        ; Rva of the end of the table
    add esi, ecx
    
    mov edi, pString   
    cld
    mov ecx, -1
    xor eax, eax
    repne scasb
    not ecx
    dec ecx
    mov edi, pString   
    
    push ecx
    mov byte ptr [esi], cl          ; string size
    mov eax, Color
    mov byte ptr [esi+1], al        ; string color  0 = black ; 1 = red ; -1 = gray
    add esi, 7
    xchg edi, esi
    rep movsb
    pop ecx
    mov byte ptr [edi], cl          ; string size
    inc edi

    mov ecx, 4EACB0h
    mov ecx, dword ptr [ecx]
    sub edi, ecx
    mov esi, 4EADE4h
    mov dword ptr [esi], edi
            
    ret
AddtoLogData endp

; *********************************************************************************
; 
; call Open_NotePad
; 
; *********************************************************************************

Open_NotePad proc

    ; ***************************** Open NotePad

    push 0
    push offset Classe_
    call FindWindow                             ; look for Notepad 
    cmp eax, 0
    jne @1

    push SW_SHOWNORMAL
    push offset Dir_
    push 0
    push offset NotePad_
    push offset open_
    push 0
    call ShellExecuteA                          ; run Notepad 

@@:
    push 0
    push offset Classe_
    call FindWindow                             ; get the handle of Notepad
    cmp eax, 0
    je @B

@1:
    mov hNotePad, eax

    push SWP_SHOWWINDOW
    push 400                                    ; height
    push 400                                    ; width
    push 50                                     ; y
    push 50                                     ; x
    push HWND_TOPMOST
    push hNotePad
    call SetWindowPos                           ; modify size and position of the window

    push WS_EX_LAYERED
    push GWL_EXSTYLE
    push hNotePad
    call SetWindowLong                          ; Set LAYERED WINDOW

    push LWA_ALPHA
    push Opacity                                ; Opacity 
    push 0
    push hNotePad
    call SetLayeredWindowAttributes             ; Set the opacity
    
@@:
    push 0
    push offset Edit_
    push 0
    push hNotePad
    call FindWindowEx                           ; Get the handle of the "edit" control
    cmp eax, 0
    je @B
    
    mov hEdit, eax

    push offset Accueil
    call Copy2Clipboard                         ; copy in the clipboard
    push 0
    push 0
    push WM_PASTE
    push hEdit
    call SendMessage                            ; paste in the NotePad

    push 0        
    call OpenClipboard              ; open the clipboard
    call EmptyClipboard             ; empty the clipboard
    call CloseClipboard
    ret
Open_NotePad endp



; *********************************************************************************
; 
; call Param_Opacity
; 
; *********************************************************************************
Param_Opacity proc

   push 0
   call GetModuleHandle
   mov  hInstance_,eax
   call InitCommonControls    ; needed - not necessary executed
   
; =============================================== Create class "NotePad" ===========================================
   xor    esi,esi
   mov    wc.cbSize,SIZEOF WNDCLASSEX
   mov    wc.style,CS_HREDRAW or CS_VREDRAW
   mov    wc.lpfnWndProc,OFFSET WndProc
   mov    wc.cbClsExtra,NULL
   mov    wc.cbWndExtra,NULL
   m2m    wc.hInstance,hInstance_
   mov    wc.hbrBackground,COLOR_BACKGROUND+1
   mov    wc.lpszMenuName,0
   mov    wc.lpszClassName,OFFSET ClassName
   invoke LoadIcon,hInstance_,200
   mov    hIcon,eax
   m2m    wc.hIcon,hIcon
   m2m    wc.hIconSm,hIcon
   invoke LoadCursor,esi,IDC_ARROW
   mov    wc.hCursor,eax
   invoke RegisterClassEx,addr wc
 
; ===========================================================================

   invoke GetSystemMetrics,SM_CXSCREEN
   sub    eax,WxDim
   shr    eax,1
   m2m    ebx,eax

   invoke GetSystemMetrics,SM_CYSCREEN
   sub    eax,WyDim
   shr    eax,1


; ============================================ Create instance window ==========================================

    push NULL
    push hInstance_
    push NULL
    push NULL
    push WyDim
    push WxDim
    push eax
    push ebx
    push WS_OVERLAPPEDWINDOW
    push offset AppName
    push offset ClassName
    push WS_EX_TOPMOST
    call CreateWindowExA    
    mov  hWnd,eax

; =========================================== Show window ===================================================================
   invoke ShowWindow,hWnd,SW_SHOWNORMAL
   invoke UpdateWindow,hWnd

   .while 1
      xor    eax,eax
      invoke GetMessage,addr msg,eax,eax,eax
      .break .if (!eax)
      invoke TranslateMessage,addr msg
      invoke DispatchMessage,addr msg
   .endw
   mov    eax,msg.wParam
   
 Fin:

    ret
Param_Opacity endp




; ========================================================================================================


WndProc PROC uses ebx ecx edx esi edi hwnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

    LOCAL ps     :PAINTSTRUCT
    LOCAL hDC    :HDC
    LOCAL hMemDC :HDC
    LOCAL rect   :RECT
    LOCAL hOld   :DWORD

    push 0
    push 0
    push TBM_GETPOS
    push hTrackBar
    call SendMessage
    .if eax >= 100
        mov Opacity, eax

        .if hNotePad != 0
            push WS_EX_LAYERED
            push GWL_EXSTYLE
            push hNotePad
            call SetWindowLong                          
        
            push LWA_ALPHA
            push Opacity                                
            push 0
            push hNotePad
            call SetLayeredWindowAttributes            
        .endif
        
    .endif
    
    .IF uMsg==WM_CREATE 

        push hwnd
        call CreationTrackBar
        
    .ELSEIF uMsg==WM_DESTROY
 
        xor eax,eax
     	  invoke GlobalFree,hMem
        invoke PostQuitMessage,eax
        ret
    
    .ELSEIF uMsg==WM_KEYDOWN
        
        cmp wParam, VK_ESCAPE
        jz EndProgram

    .ELSEIF uMsg==WM_LBUTTONDBLCLK

        EndProgram:
            invoke DestroyWindow,hwnd
            invoke ExitProcess,0

   .ELSE

       invoke DefWindowProc,hwnd,uMsg,wParam,lParam
       ret
   .ENDIF
   xor eax, eax
   ret
        

WndProc ENDP

; *************************************************************************************

CreationTrackBar proc hwnd:DWORD

        xor edi, edi
        push edi
        push hInstance_
        push 0
        push hwnd
        push 30
        push 380
        push 10
        push 10
        push WS_VISIBLE + WS_CHILDWINDOW + TBS_AUTOTICKS+TBS_ENABLESELRANGE
        push edi
        push offset TrackClass
        push edi
        call CreateWindowExA
        
        mov hTrackBar,eax

        mov eax,255
        shl eax,16
        mov ax,100
        push eax
        push TRUE
        push TBM_SETRANGE
        push hTrackBar
        call SendMessage
        
        push 1
        push TRUE
        push TBM_SETPAGESIZE
        push hTrackBar
        call SendMessage        


        mov eax,255
        shl eax,16
        mov ax,100
        push eax
        push FALSE
        push TBM_SETSEL
        push hTrackBar
        call SendMessage

        push Opacity
        push TRUE
        push TBM_SETPOS
        push hTrackBar
        call SendMessage
        

    ret
CreationTrackBar endp


