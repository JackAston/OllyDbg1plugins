.386
.model flat, stdcall  ; 32 bit memory model
option scoped         ; local labels are enabled, global labels inside
                      ; PROC should be defined with double colons (LABEL::)
option casemap :none  ; case sensitive

include StollyStruct.inc


.code

; Entry point into a plugin DLL. Many system calls require DLL instance
; which is passed to DllEntryPoint() as one of parameters. Remember it.
; Preferrable way is to place initializations into _ODBG_Plugininit() and
; cleanup in _ODBG_Plugindestroy().
DllEntryPoint proc hi:HINSTANCE, reason:dword, res:dword
    .IF reason == DLL_PROCESS_ATTACH
        push hi
        pop hDllInst
        invoke GetModuleHandle, NULL
        mov hOllyInst, eax              ; Keep a handle to Olly to make the patches
       
    .ENDIF
    mov eax, 1                           ; Report success
    ret
DllEntryPoint endp

;=========================================== OLLY CALLBACK ROUTINES ===========================================
 
 
; ====================================================================================== 
; _ODBG_Plugindata() is a "must" for valid OllyDbg plugin. It must fill in                                                                                                     
; plugin name and return version of plugin interface. If function is absent,                                                                                                  
; or version is not compatible, plugin will be not installed. Short name                                                                                                      
; identifies it in the Plugins menu. This name is max. 31 alphanumerical                                                                                                      
; characters or spaces + terminating '\0' long. To keep life easy for users,                                                                                                  
; this name should be descriptive and correlate with the name of DLL.                                                                                                         
; ======================================================================================  
_ODBG_Plugindata proc C shortname:ptr byte        
    invoke  lstrcpy, shortname, addr szPlugin      ; Name of plugin                                                                                                     
    mov eax, PLUGIN_VERSION
    ret                                                                                                                                               
_ODBG_Plugindata endp 


; ======================================================================================                                                                                                                                                                               
; OllyDbg calls this obligatory function once during startup. Place all                                                                                                       
; one-time initializations here. If all resources are successfully allocated,                                                                                                 
; function must return 0. On error, it must free partially allocated resources                                                                                                
; and return -1, in this case plugin will be removed. Parameter ollydbgversion                                                                                                
; is the version of OllyDbg, use it to assure that it is compatible with your                                                                                                 
; plugin; hw is the handle of main OllyDbg window, keep it if necessary.                                                                                                      
; Parameter features is reserved for future extentions, do not use it.                   
; ======================================================================================                                                                                      
_ODBG_Plugininit proc C ollydbgversion:dword, hw:HWND, features:ptr dword                                                                                                      
    ; Check that version of OllyDbg is correct.                                                                                                                           
    .IF ollydbgversion < PLUGIN_VERSION
        jmp BadExit                                                                                                                                   
    .ENDIF

    ; Keep handle of main OllyDbg window. This handle is necessary, for example,                                                                                          
    ; to display message box. 
    push hw
    pop hwMain
    
    ; Initialize StollyStruct data. Data consists of elements of type t_StollyStruct,
    ; we reserve space for 10 elements.
    invoke  Createsorteddata, offset StollyStructTable.data, offset szStructTableName,
                              sizeof t_StollyStruct, 10, NULL, NULL

    .IF eax
        jmp     BadExit
    .ENDIF
    
    ; Set Path to structure data file
    invoke GetModuleFileName, hOllyInst, offset szDbPath, sizeof szDbPath
    invoke lstrlen, offset szDbPath
    mov ecx, eax
    mov eax, offset szDbPath
    Loop0:
    cmp byte ptr[eax+ecx], 5Ch ;'\'
    je FoundB
    dec ecx
    cmp ecx,0
    je BadExit
    jmp Loop0
    FoundB:
    mov byte ptr[eax+ecx], 0
    invoke lstrcat,offset szDbPath, offset szDbFile

    
    invoke  Registerpluginclass, offset szStollyStructWinClass, NULL, hOllyInst, offset StollyStructWinProc
    .IF eax < 0
        ; Failure! Destroy sorted data and exit.
        invoke  Destroysorteddata, offset StollyStructTable.data    
        jmp     BadExit
    .ENDIF

    
    mov szStructListPtr, 0
    invoke GetProcessHeap
    mov hHeap, eax
    .IF eax != NULL
        invoke HeapAlloc,hHeap, HEAP_ZERO_MEMORY, IniAppSize
        .if eax != NULL
            mov szStructListPtr, eax
            invoke GetPrivateProfileString,NULL, NULL, offset szKeyDefault, szStructListPtr, IniAppSize, offset szDbPath
        .endif    
    .endif
    
    ; Say Hello           
    invoke Addtolist, 0,0,offset szInit
    
    mov eax,0                                                                                                                                                                          
    ret
BadExit:
    invoke Addtolist, 0,0,offset szBadInit
    mov eax,-1
    ret                                                                                                                                                
_ODBG_Plugininit endp                                                                                                                                                          

; ======================================================================================
; OllyDbg calls this optional function once on exit. At this moment, all MDI
; windows created by plugin are already destroyed (and received WM_DESTROY
; messages). Function must free all internally allocated resources, like
; window classes, files, memory and so on.
; ======================================================================================
_ODBG_Plugindestroy proc C
    invoke  Unregisterpluginclass, offset szStollyStructWinClass
    invoke  Destroysorteddata, offset StollyStructTable.data
    ret
_ODBG_Plugindestroy endp


; ======================================================================================
; Function is called when user opens new or restarts current application.
; Plugin should reset internal variables and data structures to initial state.
; ======================================================================================
_ODBG_Pluginreset proc C
    invoke  Deletesorteddatarange, offset StollyStructTable.data, 0, 0FFFFFFFFh
    ret
_ODBG_Pluginreset endp                                                                                                                                                         
                                                                                                                                                                              

; ======================================================================================                                                                                                                                                                             
; Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup                                                                                                
; menu in one of standard OllyDbg windows. When plugin wants to add own menu                                                                                                  
; items, it gathers menu pattern in data and returns 1, otherwise it must                                                                                                     
; return 0. Except for static main menu, plugin must not add inactive items.                                                                                                  
; Item indices must range in 0..63. Duplicated indices are explicitly allowed.                                                                                                
; ======================================================================================
_ODBG_Pluginmenu proc C uses ebx origin:dword, data:ptr byte, item:dword                                                                                                       
    mov     eax, origin                                                                                                           
    .IF eax == PM_MAIN ; Plugin menu in main window                                                                                                                       
        invoke  lstrcpy, data, addr szMainMenu                                                                                                             
        mov eax,1                                                                                        
        ret
        
    .ELSEIF eax == PM_CPUDUMP
        invoke lstrcpy, data, addr szStructMenu
        mov eax, 1  
        ret
        
    .ELSEIF eax == PM_CPUSTACK
        invoke lstrcpy, data, addr szStructMenu
        mov eax, 1  
        ret         
                 
    .ENDIF                                                                                                                                                                
    xor eax,eax
    ret                                                                                                                                                            
_ODBG_Pluginmenu endp                                                                                                                                                          
                                                 
                                                 
; ======================================================================================                                                                                                                                                                              
; This optional function receives commands from plugin menu in window of type                                                                                                 
; origin. Argument action is menu identifier from _ODBG_Pluginmenu(). If user                                                                                                  
; activates automatically created entry in main menu, action is 0.                                                                                                            
; ======================================================================================
_ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
    mov     eax, origin                                                                                                                                                   
    .IF eax == PM_MAIN                                                                                                                                                    
        mov     eax, action
        .IF eax == 0 ; StollyStruct
            invoke DialogBoxParam, hDllInst, IDD_DLG_STRUCTSELECT, hwMain, ADDR StructSelectDlgProc, NULL 
        .ELSEIF eax == 1    ; Show me your PEB
            ; First reset soted data    
            invoke Deletesorteddatarange, offset StollyStructTable.data, 0, 0FFFFFFFFh
            invoke wsprintf, offset szStructName, offset szFormatString, offset szPEB
            invoke GetPEBAddress
            .IF EAX
                invoke LoadStructure, offset szPEB, eax, NULL    
            .endif
        .ELSEIF eax == 2    ; Show me your TEB
            invoke Deletesorteddatarange, offset StollyStructTable.data, 0, 0FFFFFFFFh
            invoke wsprintf, offset szStructName, offset szFormatString, offset szTEB
            invoke GetTEBAddress
            .IF EAX
                invoke LoadStructure, offset szTEB, eax, NULL    
            .endif
        .ELSEIF eax == 3    ; Menu item "About", displays plugin info.                                                                                                                    
            invoke DialogBoxParam, hDllInst, IDD_DLG_ABOUT, hwMain, ADDR StructAboutDlgProc, NULL
        .ENDIF
           
    .ELSEIF eax == PM_CPUDUMP
        mov eax, action
        .IF eax == 0 ; Fire up StollyStruct
            invoke Plugingetvalue, VAL_CPUDDUMP
            push (t_dump ptr[eax]).sel0
            pop dwStructAddress 
            invoke DialogBoxParam, hDllInst, IDD_DLG_STRUCTSELECT, hwMain, ADDR StructSelectDlgProc, NULL          
        .endif                                                                                                                                                         
                                                                                                                                   
    .ELSEIF eax == PM_CPUSTACK
        mov eax, action
        .IF eax == 0 ; Fire up StollyStruct
            invoke Plugingetvalue, VAL_CPUDSTACK
            push (t_dump ptr[eax]).sel0
            pop dwStructAddress 
            invoke DialogBoxParam, hDllInst, IDD_DLG_STRUCTSELECT, hwMain, ADDR StructSelectDlgProc, NULL          
        .endif
                                                                                                                                                                 
    .ENDIF                                                                                                                                                                
    ret                                                                                                                                                                   
    _ODBG_Pluginaction endp                                                                                                                                                        


; Test if a given strucure is available in the database file
IsStructureAvailable proc szName:PTR BYTE
    pushad
    
	mov esi, szStructListPtr
	
	Loop0:
	invoke lstrlen, esi
	cmp eax, 0
	je StructNotFound
	
    mov edi, eax                ; Keep length 
    invoke lstrcmp,szName, esi  ; Compare
    cmp eax, 0                  ; Found
    je StructFound
    
    add esi, edi                ; Point to end of string
    inc esi                     ; Point to next string
    jmp Loop0
       	
    StructNotFound:
    popad
    xor eax,eax
    ret

    StructFound:
    popad
    mov eax, TRUE
    ret
IsStructureAvailable endp

; WinProc Structure selection form.
StructSelectDlgProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    LOCAL hCombo:HANDLE
    LOCAL hEdit:HANDLE
    .if uMsg == WM_INITDIALOG
    	invoke GetDlgItem,hWnd,IDC_CBO_STRUCTS
    	mov hCombo, eax
    	
    	; Fill the ListBox
    	push esi
    	push edi 
    	mov esi, szStructListPtr
    	Loop0:
    	invoke lstrlen, esi
    	cmp eax, 0
    	je EndOfBuffer
        mov edi, eax
        invoke SendMessage, hCombo, CB_ADDSTRING, 0, esi
        add esi, edi
        inc esi
        jmp Loop0   	
    	
    	invoke lstrlen, offset szStructType
    	cmp eax, 0
    	je EndOfBuffer
    	invoke SetDlgItemText, hWnd, IDC_CBO_STRUCTS, offset szStructType
    	
    	EndOfBuffer:
    	invoke wsprintf,offset szStructAddressBuffer, offset szFormatDWORD, dwStructAddress
    	invoke SetDlgItemText, hWnd, IDC_EDT_STRUCTSTART, offset szStructAddressBuffer
    	    	
    	pop edi
    	pop esi
    	
    .elseif uMsg == WM_COMMAND
    	mov	eax,wParam
    	.IF eax==IDC_BTN_OK
    	    ; Get Address
    	    invoke GetDlgItemText,hWnd, IDC_EDT_STRUCTSTART, offset szStructAddressBuffer, 9
    	    cmp eax, 0
    	    je BadInput
    	    ; Parse Adress
    	    
    	    invoke ParseHex, addr szStructAddressBuffer
    	    cmp eax, 0
    	    je BadInput
    	    mov dwStructAddress, eax
    	    
    	    ; Get structure type
    	    invoke GetDlgItemText, hWnd, IDC_CBO_STRUCTS, offset szStructType, 63
    	    cmp eax, 0
    	    je BadInput
    	    
    	    invoke GetDlgItemText, hWnd, IDC_EDT_INSTANCE, offset szStructName, 15
    	    
    		invoke Deletesorteddatarange, offset StollyStructTable.data, 0, 0FFFFFFFFh
    		Invoke LoadStructure, offset szStructType, dwStructAddress, NULL
    		invoke	SendMessage,hWnd,WM_CLOSE,0,0
    		
    		BadInput:
    	.endif
    .elseif	uMsg == WM_CLOSE
    	invoke	EndDialog,hWnd,0
    .endif
            
    xor	eax,eax
    ret 
StructSelectDlgProc endp 

; WinProc About form
StructAboutDlgProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
    LOCAL bm:BITMAP 
    LOCAL ps:PAINTSTRUCT
    LOCAL hdc:HDC
    LOCAL hdcMem:HDC
    LOCAL hbmOld:HBITMAP
    
    .if uMsg == WM_INITDIALOG
		invoke LoadBitmap,hDllInst, 6000
        mov hLogo, eax	
    .elseif uMsg == WM_PAINT
    
        invoke BeginPaint, hWnd, ADDR ps
        mov hdc,eax          
        invoke CreateCompatibleDC, hdc
        mov hdcMem, eax
        invoke SelectObject, hdcMem, hLogo
        mov hbmOld, eax
        invoke GetObject, hLogo, sizeof bm, addr bm
        invoke BitBlt,hdc,0,0,bm.bmWidth, bm.bmHeight, hdcMem,0,0,SRCCOPY
        invoke SelectObject,hdcMem, hbmOld
        invoke DeleteDC, hdcMem
        invoke EndPaint,hWnd, addr ps
        
    .elseif uMsg == WM_COMMAND
    	mov	eax,wParam
    	.IF eax==IDC_BTN_OK
    		invoke	SendMessage,hWnd,WM_CLOSE,0,0
    	.endif
    .elseif	uMsg == WM_CLOSE
    	invoke	EndDialog,hWnd,0
    .endif
            
    xor	eax,eax
    ret 
StructAboutDlgProc endp 

; Each window class needs its own window procedure. Both standard and custom
; OllyDbg windows must pass some system and OllyDbg-defined messages to
; Tablefunction(). See description of Tablefunction() for more details.
StollyStructWinProc proc hw:HWND, msg:UINT, wp:WPARAM, lp:LPARAM
    LOCAL menu:HMENU
    LOCAL mnuFollow:HMENU
    LOCAL dwBuffer:DWORD
    LOCAL bValueAvail:DWORD
    
    mov     eax, msg
    ; Standard messages. You can process them, but - unless absolutely sure -

    ; always pass them to Tablefunction().
    .IF eax == WM_MOUSEMOVE || eax == WM_LBUTTONDOWN || \
        eax == WM_LBUTTONDBLCLK || eax == WM_LBUTTONUP || eax == WM_RBUTTONDOWN || \
        eax == WM_RBUTTONDBLCLK || eax == WM_HSCROLL || eax == WM_VSCROLL || eax == WM_TIMER || eax == WM_SYSKEYDOWN
        invoke  Tablefunction, offset StollyStructTable, hw, msg, wp, lp

    ; Custom messages responsible for scrolling and selection. User-drawn
    ; windows must process them, standard OllyDbg windows without extra
    ; functionality pass them to Tablefunction().
    .ELSEIF eax == WM_USER_SCR || eax == WM_USER_VABS || eax == WM_USER_VREL || \
            eax == WM_USER_VBYTE || eax == WM_USER_STS || eax == WM_USER_CNTS || eax == WM_USER_CHGS
        invoke  Tablefunction, offset StollyStructTable, hw, msg, wp, lp
        ret
        
    .ELSEIF eax == WM_WINDOWPOSCHANGED
        invoke  Tablefunction, offset StollyStructTable, hw, msg, wp, lp
        ret
        
    .ELSEIF eax == WM_USER_MENU
        invoke  CreatePopupMenu
        mov     menu, eax
        invoke  CreatePopupMenu
        mov     mnuFollow, eax

        ; Find selected structure entry. Any operations with structures make sense only
        ; if at least one structure entry exists and is selected. Note that sorted data
        ; has special sort index table which is updated only when necessary.
        ; Getsortedbyselection() does this; some other sorted data functions
        ; don't and you must call Sortsorteddata(). Read documentation!
        invoke  Getsortedbyselection, offset StollyStructTable.data, StollyStructTable.data.selected
        mov     esi, eax
        .IF menu && eax
            invoke AppendMenu, menu, MF_STRING, 1, offset szMenuEdit
            invoke AppendMenu, menu, MF_STRING, 2, offset szMenuLabel
            invoke AppendMenu, menu, MF_STRING, 3, offset szMenuLabelTmp
            invoke AppendMenu, menu, MF_POPUP, mnuFollow, offset szMenuFollow 
            invoke AppendMenu, mnuFollow, MF_STRING, 4, offset szMenuFollowInDump
            invoke AppendMenu, mnuFollow, MF_STRING, 5, offset szMenuFollowInStack
            invoke AppendMenu, mnuFollow,MF_SEPARATOR, 0, NULL
            mov bValueAvail, 0
            mov eax, (t_StollyStruct ptr[esi]).struct_size
            .IF eax == 4
                invoke Readmemory, addr dwBuffer, (t_StollyStruct ptr [esi]).struct_addr, (t_StollyStruct ptr [esi]).struct_size, MM_SILENT
                .IF eax != 0
                    mov bValueAvail, 1
                .endif        
            .endif
            .IF bValueAvail == 1
                invoke AppendMenu, mnuFollow, MF_STRING, 6, offset szMenuFolValInDump
                invoke AppendMenu, mnuFollow, MF_STRING, 7, offset szMenuFolValInStack
                invoke AppendMenu, menu, MF_STRING, 8, offset szMenuNewStruct
            .else
                invoke AppendMenu, mnuFollow,MF_GRAYED or MF_DISABLED or MF_STRING, 6, offset szMenuFolValInDump
                invoke AppendMenu, mnuFollow,MF_GRAYED or MF_DISABLED or MF_STRING, 7, offset szMenuFolValInStack
                invoke AppendMenu, menu,MF_GRAYED or MF_DISABLED or MF_STRING, 8, offset szMenuNewStruct
            .endif

        .ENDIF
        
        ; Even when menu is NULL, call to Tablefunction is still meaningful.
        invoke  Tablefunction, offset StollyStructTable, hw, WM_USER_MENU, 0, menu
        push    eax
        mov     eax, menu
        .IF eax
            invoke  DestroyMenu, eax
        .ENDIF
        pop     eax

        .IF eax == 1        ; Edit
            ;invoke  Getsortedbyselection, offset StollyStructTable.data, StollyStructTable.data.selected
            .IF eax
                invoke  Createdumpwindow, addr (t_StollyStruct ptr[esi]).struct_name , (t_StollyStruct ptr[esi]).struct_addr, \
                    (t_StollyStruct ptr[esi]).struct_size,(t_StollyStruct ptr[esi]).struct_addr, DU_ESCAPABLE or 1101h, NULL
            .ENDIF
            
        .ELSEIF eax == 2
            mov bTempLabels, 0    
            invoke  LabelSortedStructure            ; Label all the structure entries
            invoke  InvalidateRect, hw, NULL, FALSE 
            
        .ELSEIF eax == 3    
            mov bTempLabels, 1
            invoke  LabelSortedStructure            ; Label all the structure entries
            invoke  InvalidateRect, hw, NULL, FALSE 
            
        .ELSEIF eax == 4                            ;Follow address in dump
            ;void Setcpu(ulong threadid,ulong asmaddr,ulong dumpaddr,ulong stackaddr,int mode);
            invoke Setcpu, 0, 0, (t_StollyStruct ptr[esi]).struct_addr, 0, CPU_DUMPFIRST or CPU_DUMPFOCUS
            invoke  InvalidateRect, hw, NULL, FALSE 
            
        .ELSEIF eax == 5                            ; Follow address in stack
            invoke Setcpu, 0, 0, 0, (t_StollyStruct ptr[esi]).struct_addr, CPU_STACKFOCUS
            invoke  InvalidateRect, hw, NULL, FALSE 
            
        .ELSEIF eax == 6                            ;Follow value in dump
            invoke Setcpu, 0, 0, dwBuffer,0 , CPU_DUMPFIRST or CPU_DUMPFOCUS
            invoke  InvalidateRect, hw, NULL, FALSE
            
        .ELSEIF eax == 7                            ;Follow address in stack
            invoke Setcpu, 0, 0, 0, dwBuffer, CPU_STACKFOCUS
            invoke  InvalidateRect, hw, NULL, FALSE 
            
        .ELSEIF eax == 8                            ; Start new structure
            push dwBuffer
            pop dwStructAddress
            invoke DialogBoxParam, hDllInst, IDD_DLG_STRUCTSELECT, hwMain, ADDR StructSelectDlgProc, NULL          
            invoke  InvalidateRect, hw, NULL, FALSE
       .ENDIF
        xor eax,eax
        ret
        
    .ELSEIF eax == WM_KEYDOWN
        invoke  Tablefunction, offset StollyStructTable, hw, msg, wp, lp
                    
    .ELSEIF eax == WM_USER_DBLCLK
        ; Doubleclicking row opens structure entry in dump.
        invoke  Getsortedbyselection, offset StollyStructTable.data, StollyStructTable.data.selected
        .IF eax

            invoke  Createdumpwindow, addr (t_StollyStruct ptr[eax]).struct_name , (t_StollyStruct ptr[eax]).struct_addr, \
                (t_StollyStruct ptr[eax]).struct_size,(t_StollyStruct ptr[eax]).struct_addr, DU_ESCAPABLE or 1101h, NULL
        .ENDIF
        mov eax,1   ; Doubleclick processed
        ret

    .ELSEIF eax == WM_USER_CHALL || eax == WM_USER_CHMEM
        ; Something is changed, redraw window.
        invoke  InvalidateRect, hw, NULL, FALSE
        xor eax,eax
        ret
    .ELSEIF eax == WM_DESTROY
        cmp bTempLabels,1
        jnz DoNotDelete
        invoke ClearLabelSortedStructure
        DoNotDelete:
        invoke  Tablefunction, offset StollyStructTable, hw, msg, wp, lp       
    .ELSEIF eax == WM_PAINT
        ; Painting of all OllyDbg windows is done by Painttable(). Make custom
        ; drawing only if you have important reasons to do this.
        invoke  Painttable, hw, offset StollyStructTable, offset StollyStructGetText
        xor eax,eax
        ret
    .ENDIF
    
    invoke  DefMDIChildProc, hw, msg, wp, lp
    ret
StollyStructWinProc endp


; Reads the data from the sorted data and
; labels each specific address
LabelSortedStructure proc
    LOCAL lBuffer[512]:BYTE
    LOCAL lAddName:DWORD
    
    pushad
    
    invoke lstrlen, offset szStructName
    mov lAddName, eax
    
    mov esi, StollyStructTable.data.n ; Number of elements
    
    Loop0:
    test esi, esi
    jz EndOfStructure
   
    mov edi, esi
    dec edi ; Zero based
    invoke Getsortedbyselection, offset StollyStructTable.data, edi
    test eax,eax
    jz EndOfStructure
    mov ebx, eax
    
    lea eax, lBuffer        ; Set starting pointer
    mov byte ptr [eax],0
    
    cmp lAddName,0
    je NoName
    invoke lstrcat, addr lBuffer, offset szStructName
    invoke lstrcat, addr lBuffer, offset szDelimiter

    NoName:
    invoke lstrcat, addr lBuffer, addr (t_StollyStruct ptr[ebx]).struct_name
    invoke Quickinsertname, (t_StollyStruct ptr[ebx]).struct_addr, NM_LABEL, addr lBuffer
    dec esi	
    jmp Loop0
    
    EndOfStructure:
    invoke Mergequicknames
    popad
    ret

LabelSortedStructure endp

; Reads the data from the sorted data and
; removes labels
ClearLabelSortedStructure proc
    pushad
    mov esi, StollyStructTable.data.n ; Number of elements
    
    Loop0:
    test esi, esi
    jz EndOfStructure
   
    mov edi, esi
    dec edi ; Zero based
    invoke Getsortedbyselection, offset StollyStructTable.data, edi
    test eax,eax
    jz EndOfStructure
    mov ecx, eax
    mov ebx, (t_StollyStruct ptr[ecx]).struct_addr
    add ebx, 1
    invoke Deletenamerange, (t_StollyStruct ptr[ecx]).struct_addr, ebx, NM_ANYNAME
    dec esi	
    jmp Loop0
    
    EndOfStructure:
    
    popad
    ret

ClearLabelSortedStructure endp

; Standard function Painttable() makes most of OllyDbg windows redrawing. You                                                                                                 
; only need to supply another function that prepares text strings and                                                                                                         
; optionally colours them. Case of custom windows is a bit more complicated,                                                                                                  
; please read documentation.                                                                                                                                                  
StollyStructGetText proc C USES EBX ESI EDI s:ptr byte, mask_:ptr byte, select:ptr dword, ph:ptr t_sortheader, column:dword
    LOCAL dwBufferPtr:DWORD
    LOCAL dwBuffer:DWORD
                                                                             
    mov     eax, column                                                                                                                                                  
   .IF eax == 0            ; Address of structure entry                                                                                                                                   
        mov     eax, ph                                                                                                                                                  
        invoke  wsprintf, s, offset szFormatDWORD, (t_StollyStruct ptr [eax]).struct_addr                                                                                                     
        invoke  lstrlen, s
        
    .ELSEIF eax == 1        ; Name of structure entry
        ; We know that the Name in the structure can not be more than TEXTLEN
        ; The buffer s has 2*TEXTLEN but we can only use half!!!
        mov eax, s
        mov byte ptr[eax], 0
        
        invoke lstrlen, offset szStructName
        cmp eax, 0
        je NoName
        invoke wsprintf, s, offset szFormatString, offset szStructName     
        invoke  lstrcat, s, offset szDelimiter          ; Add a '.'  
        
        NoName:                                                                                                                            
        mov     eax, ph
        invoke  lstrcat, s, addr (t_StollyStruct ptr [eax]).struct_name                                                                                                                                                   
        invoke  lstrlen, s
        cmp eax, TEXTLEN
        jna BufferOk
        mov eax, s
        add eax, TEXTLEN-4
        mov ecx, offset szKeyDefault
        mov ecx, [ecx]
        mov [eax], ecx 
        invoke lstrlen, s
            
        BufferOk:                                                                                                                                                
    .ELSEIF eax == 2        ; Type of entry                                                                                                                                                     
        mov     eax, ph                                                                                                                                                  
        invoke  wsprintf, s, offset szFormatString, addr (t_StollyStruct ptr [eax]).struct_type                                                                                                       
        invoke  lstrlen, s

    .ELSEIF eax == 3        ; Value (only BYTE, WORD or DWORD)
        mov dwBuffer,0
        mov dwBufferPtr, 0                                                                                                                                                     
        mov     esi, ph                                                                                                                                                  
        cmp (t_StollyStruct ptr [esi]).struct_size, 4
        je PrintDword
        cmp (t_StollyStruct ptr [esi]).struct_size, 2
        je PrintWord
        cmp (t_StollyStruct ptr [esi]).struct_size, 1
        je PrintByte
        jmp InValidValueSize
        
        PrintDword:
        mov dwBufferPtr, offset szFormatDWORD
        jmp PrintValue
        
        PrintWord:
        mov dwBufferPtr, offset szFormatWORD
        jmp PrintValue
        
        PrintByte:
        mov dwBufferPtr, offset szFormatByte
        
        PrintValue:
        invoke Readmemory, addr dwBuffer, (t_StollyStruct ptr [esi]).struct_addr, (t_StollyStruct ptr [esi]).struct_size, MM_SILENT
        cmp eax, 0
        je InValidValueSize
        mov eax, dwBuffer
        invoke wsprintf, s, dwBufferPtr, eax
        invoke lstrlen,s
        jmp EndOfValue
        
                 
        InValidValueSize:
        xor eax, eax
        
        EndOfValue:
        
    .ELSEIF eax == 4        ; Hex Dump
    

        mov eax,s
        mov byte ptr [eax],0
                                                                                                                                                     
        mov esi, ph
        cmp (t_StollyStruct ptr [esi]).struct_size, 4
        je SkipDump
        cmp (t_StollyStruct ptr [esi]).struct_size, 2
        je SkipDump
        cmp (t_StollyStruct ptr [esi]).struct_size, 1
        je SkipDump
                
        invoke HeapAlloc,hHeap, HEAP_ZERO_MEMORY, (t_StollyStruct ptr [esi]).struct_size
      
        .IF eax != NULL
            mov dwBufferPtr, eax
            invoke Readmemory, dwBufferPtr, (t_StollyStruct ptr [esi]).struct_addr, (t_StollyStruct ptr [esi]).struct_size, MM_SILENT
            
            mov ebx, dwBufferPtr        ;   Init ebx with pointer to internal buffer
            mov edi , eax               ;   Bytes retieved from memory of debuggee
      
            Loop0:
            cmp edi, 0
            je EndOfBuffer
            
            xor eax,eax
            mov al, BYTE PTR[ebx]
            invoke wsprintf, offset szByteFormatBuf, offset szFormatByte, eax
            invoke lstrcat, s, offset szByteFormatBuf
            dec edi                     ;   Decrease the number of bytes to process
            inc ebx                     ;   Increase the pointer
            
            invoke lstrlen, s           ; Test if we are not surpassing the buffer capasity.
            cmp eax, (TEXTLEN - 4) 
            jne Loop0
            invoke lstrcat,s, offset szKeyDefault   ; add '...' to inform the user that not everything is shown
           
            EndOfBuffer:
            invoke HeapFree,hHeap,HEAP_NO_SERIALIZE	, dwBufferPtr
        .endif
        
        SkipDump:
        invoke  lstrlen, s
    .ELSE
       xor     eax, eax                                                                                                                                                 
    .ENDIF                                                                                                                                                               
    ret                                                                                                                                                                  
StollyStructGetText endp                                                                                                                                                         

; OllyDbg makes most of work when creating standard MDI window. Plugin must
; only describe number of columns, their properties and properties of window
; as a whole.
CreateStollyStructWindow proc
    ; Describe table columns. Note that column names are pointers, so strings
    ; must exist as long as table itself.
    .IF !StollyStructTable.bar.nbar
    
        ; Bar still uninitialized.
        mov     StollyStructTable.bar.name_, offset szBarAddress                     ; Address
        mov     StollyStructTable.bar.defdx, 9                                       ; Default number of chars
        mov     StollyStructTable.bar.mode, 0                             
        
        mov     StollyStructTable.bar.name_[1 * sizeof dword], offset szBarName      ; Name structure member
        mov     StollyStructTable.bar.defdx[1 * sizeof dword], 32                    
        mov     StollyStructTable.bar.mode[1 * sizeof byte], BAR_NOSORT              ; Sort Y/N
      
        mov     StollyStructTable.bar.name_[2 * sizeof dword], offset szBarType      ; Type
        mov     StollyStructTable.bar.defdx[2 * sizeof dword], 8
        mov     StollyStructTable.bar.mode[2 * sizeof byte], BAR_NOSORT

        mov     StollyStructTable.bar.name_[3 * sizeof dword], offset szBarValue      ; Value
        mov     StollyStructTable.bar.defdx[3 * sizeof dword], 8
        mov     StollyStructTable.bar.mode[3 * sizeof byte], BAR_NOSORT

        mov     StollyStructTable.bar.name_[4 * sizeof dword], offset szBarDump     ; dump
        mov     StollyStructTable.bar.defdx[4 * sizeof dword], 32
        mov     StollyStructTable.bar.mode[4 * sizeof byte], BAR_NOSORT
        
        mov     StollyStructTable.bar.nbar, 5
        
        mov     StollyStructTable.mode, TABLE_COPYMENU  or TABLE_APPMENU or TABLE_SAVEPOS;
        mov     StollyStructTable.drawfunc, offset StollyStructGetText
    .ENDIF
    ; If window already exists, Quicktablewindow() does not create new window,
    ; but restores and brings to top existing. This is the simplest way,
    ; Newtablewindow() is more flexible but more complicated. I do not recommend
    ; custom (plugin-drawn) windows without very important reasons to do this.
    invoke  Quicktablewindow, offset StollyStructTable, 32, 4, offset szStollyStructWinClass, offset szStructTableName
    .IF StollyStructTable.hw                                                                                                                                              
        invoke  InvalidateRect, StollyStructTable.hw, NULL, FALSE                                                                                                         
    .ENDIF                                                                                                                                                       

    ret
CreateStollyStructWindow endp


; Loads a structure from a database and places it at a given address in a sorted data table
LoadStructure proc szType:ptr byte, dwAddress:DWORD, szPrefix: ptr byte
    LOCAL lStartAddress:DWORD
    LOCAL lStructBuffer:t_StollyStruct            ; Buffer used to add a new structure entry
    LOCAL lBigBuffer[1024]:BYTE
    LOCAL bThisLineIsStruct:DWORD
    
    pushad
    mov ebx,0                                                           ; ebx is the key counter
    
    LoadLine:
    invoke RtlZeroMemory, addr lStructBuffer, sizeof lStructBuffer      ; Clear out struct buffer 
    mov lStructBuffer.size_, 1                                          ; Size is always 1
    
    invoke wsprintf,offset szCountBuffer, offset szFormatNumber, ebx    ; Read from ini-file
    invoke GetPrivateProfileString, szType, offset szCountBuffer, offset szKeyDefault, addr lBigBuffer, 1024, offset szDbPath
    cmp eax, 3                                                          ; Length equal to default '...' ?
    jz EndOfStructure
    
    push dwAddress                                                      ; Place Address in structure
    pop lStructBuffer.struct_addr 
    
    ; ==================================================================
    ; uses esi as string pointer, edi as counter for the ',' charachter
    ; First we are going to check if the buffer has some valid input
    ; ==================================================================
    lea esi, lBigBuffer                                                 ; Init ESI with the address the line
    xor edi, edi                                                        ; clear edi
    
    ; =======================
    ; Sting Testing Loop
    ; =======================
    StringLoop0:                                                        ; Start of first stringloop
    cmp byte ptr[esi], 0                                                ; Found delimerter 0?
    je EndOfString0                                                     ; Yes, we are at the end of the string.
    cmp byte ptr[esi], 44                                               ; Found delimeter ','?
    jne Next0                                                           ; If not found skip the counter
    inc edi                                                             ; Increase the ',' counter
    
    Next0:
    inc esi                                                             ; Shift pointer to right
    jmp StringLoop0                                                     ; Back to stringloop0
    EndOfString0:

    cmp edi, 2                                                          ; Valid Format "Name,size,type" ? With only 2 comma's?
    jne EndOfStructure                                                  ; Damn, incorrect string. Lets quit this shit.
    
    ; =======================
    ; String Processing Loop
    ; =======================
    lea eax, lBigBuffer                                                 ; Load pointer to start of string in eax
    mov lStartAddress, eax                                              ; Save it in lStartAddress
    lea esi, lBigBuffer                                                 ; Init ESI again to the start of the string
    xor edi, edi                                                        ; Clear our word counter

    StringLoop1:
    cmp byte ptr[esi], 0                                                ; Found delimeter '\0'?
    je FoundDelimiter                                                   
    cmp byte ptr[esi], 44                                               ; Found delimiter ',' ?    
    je FoundDelimiter                                               
    inc esi                                                             ; Increase pointer to next char
    jmp StringLoop1                                                     ; Try again
          
    FoundDelimiter:                                                      
    inc edi                                                             ; Increase our counter
    mov byte ptr[esi], 0                                                ; Change the ',' with a '\0' so we have a new string
    cmp edi,1                                                           ; Where are we at?
    je @@ProcessName
    cmp edi,2
    je @@ProcessType
    jmp @@ProcessSize


    ; =======================
    @@ProcessName:                                                      ; Found the Element name
    ; =======================
    cmp szPrefix, NULL
    je NoPrefix
    invoke wsprintf, addr lStructBuffer.struct_name, offset szFormatString, szPrefix
    invoke lstrcat, addr lStructBuffer.struct_name, offset szDelimiter
    NoPrefix:
    invoke lstrcat, addr lStructBuffer.struct_name, lStartAddress
    
    inc esi                                                             ; Shift pointer to point to the next string in the buffer                                     
    mov lStartAddress, esi                                              ; and save it.
    jmp StringLoop1                                                     ; Back to the stringloop
    

    ; =======================
    @@ProcessType:                                                      ; Found the 'Type' of structure element
    ; =======================
    invoke IsStructureAvailable, lStartAddress                         ; Is this type another structure?
    mov bThisLineIsStruct, eax
    cmp eax, 1                                                          
    jne ContinueThisLine

    invoke LoadStructure, lStartAddress, dwAddress, addr lStructBuffer.struct_name
    inc esi                                                             ; Shift pointer to point to the next string in the buffer
    mov lStartAddress, esi                                              ; and save it.
    jmp StringLoop1                                                     ; Back to the stringloop             
    
    ContinueThisLine:
    invoke wsprintf, addr lStructBuffer.struct_type, offset szFormatString, lStartAddress
    inc esi                                                             ; Shift pointer to point to the next string in the buffer
    mov lStartAddress, esi                                              ; and save it.
    jmp StringLoop1                                                     ; Back to the stringloop             


    ; =======================
    @@ProcessSize:                                                      ; Found the Element Size
    ; =======================
    invoke ParseSizeString, lStartAddress                               ; Whats the size
    mov lStructBuffer.struct_size, eax                                  ; Place it in the Olly table
    
    cmp edx, 1                                                          ; Is it a union
    je FinishElement                                                    ; If its a union then we will not change the address. 
    mov eax, dwAddress                                                  ; Increase the address
    add eax, lStructBuffer.struct_size                                  ; with the size already saved in the olly structure element
    mov dwAddress, eax                                                  ; and save it 
    
    FinishElement:
    cmp bThisLineIsStruct, 1
    je NextLine
     
    push StollyStructTable.data.n
    pop lStructBuffer.index                                             ; Increase our index and add to sorted data
    invoke  Addsorteddata, addr StollyStructTable.data, addr lStructBuffer
    cmp eax, NULL                                                       ; Successfully added?
    je EndOfStructure


    NextLine:
    inc ebx                                                             ; Increase our line counter           
    jmp LoadLine                                                        ; Start again

  
    EndOfStructure:
    invoke CreateStollyStructWindow
    
    popad
    ret
LoadStructure endp


; Parse the size structure element
; returns the size in eax, union in edx
ParseSizeString proc uses ecx s:ptr byte
    mov edx, 0
    mov ecx, 0
    mov eax, s
    
    
    Loop0:
    cmp byte ptr[eax+ecx], 0
    je EndOfSize
    cmp byte ptr[eax+ecx], 55h ; U
    je IsUnion
    cmp byte ptr[eax+ecx], 75h ; u
    je IsUnion
    inc ecx
    jmp Loop0
    
    IsUnion:
    mov edx,1
    mov byte ptr[eax+ecx],0
    EndOfSize:    
    push edx
    invoke StrToInt, s
    pop edx
    ret
ParseSizeString endp 


GetPEBAddress proc
    push ebx
    invoke Getcputhreadid
    .if eax
        invoke Findthread, eax                  ;retreive thread info
        
        push (t_thread ptr[eax]).reg.base[4*4]               ;base of FS
        pop ebx
        
        add ebx,30h
        invoke Readmemory,addr dw_buffer,ebx,4,MM_RESTORE
        mov eax,dw_buffer
        ;add eax,2h
    .endif
    pop ebx
    ret
GetPEBAddress endp  
GetTEBAddress proc
    push ebx
    invoke Getcputhreadid
    .if eax
        invoke Findthread, eax                  ;retreive thread info
        mov ebx, (t_thread ptr[eax]).reg.base[4*4]               ;base of FS
        mov eax, ebx
      .endif
    pop ebx
    ret
GetTEBAddress endp
       
ParseHex PROC uses esi edi ebx ecx s:ptr byte
    LOCAL dwValue:DWORD
    
    mov dwValue, 0
    mov esi, s                      ; Load pointer
    
    L1:
    mov edi, offset szValidHEX      ; Load pointer to valid HEX chars
    mov ecx, 16
    mov AL, byte ptr [esi]          ; Load test-char in AL
    repne scasb                     ; Scan 4 test-char. If found cx = hexvalue 
    jnz Invalid
    
    mov ebx, dwValue                    ; ebx = eax 
    mov eax, 16                     ; eax = 16
    mul ebx                         ; eax = ebx * eax 
    add eax, ecx                    ; add the found value
    mov dwValue, eax
        
    inc esi
    cmp byte ptr[esi], 0            ; \0 charachter?
    je EndOfTestString              ; If all when fine then eax contains the correct hex representation of string
    jmp L1
    
    Invalid:
    xor eax, eax
    ret
    
    EndOfTestString: 
    mov eax, dwValue  
    ret
ParseHex endp                                                                                                                                       
end DllEntryPoint
