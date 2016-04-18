.386
.model flat,stdcall
assume fs:nothing
option casemap:none

include Poison.inc
.code
DllEntryPoint proc hi:HINSTANCE, reason:dword, res:dword
        .IF reason == DLL_PROCESS_ATTACH
            m2m     hinst, hi               
        .ENDIF
        return  1                           
DllEntryPoint endp

ODBG_Plugindata proc C shortname:ptr byte
     	invoke lstrcpy, shortname, offset sPluginTitle
        mov eax,PLUGIN_VERSION
        ret
ODBG_Plugindata endp

ODBG_Plugininit proc C ollydbgversion:dword, hwnd:HWND, features:ptr dword
		.if ollydbgversion < PLUGIN_VERSION
			mov	eax,TRUE
		.else
			m2m hWin,hwnd
			invoke Addtolist,0,0,offset sPluginTitle
			invoke Addtolist,0,-1,offset sPluginInfo
			mov	eax,FALSE	
		.endif		
		ret
ODBG_Plugininit endp

ODBG_Pluginreset proc C
		pushad
		invoke GetCurrentProcess								
        mov Handle, eax
		invoke LoadLibrary, offset sIsDebug						
		invoke GetProcAddress, eax, offset sBreakPoints
		mov NewOffset, eax										
		mov ebx, 0042F91Dh									
		sub eax, ebx
		mov NewOffset, eax
		sub ebx, 4h
		invoke VirtualProtectEx, Handle, 0042F91Dh, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
		invoke WriteProcessMemory,Handle, ebx,offset NewOffset, 4h,0
		sub ebx, 1h
		invoke GetCurrentProcess
        mov Handle, eax
		Invoke WriteProcessMemory, Handle, ebx, offset sCall, 1h, 0
		invoke GetCurrentProcess								
        mov Handle, eax
		invoke LoadLibrary, offset sIsDebug						
		invoke GetProcAddress, eax, offset sAggresive
		mov ebx, eax
		mov eax, ebx 											
		mov NewOffset, eax										
		mov ebx, 0041B5B0h										
		sub eax, ebx
		mov NewOffset, eax
		sub ebx, 4h
		invoke VirtualProtectEx, Handle, 0041B5ABh, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
		invoke WriteProcessMemory,Handle, ebx,offset NewOffset, 4h,0
		sub ebx, 1h
		invoke GetCurrentProcess
        mov Handle, eax
		Invoke WriteProcessMemory, Handle, ebx, offset sCall, 1h, 0
		Call Reset
		invoke PluginRead
		.if (FPUBug1!=0)
        	invoke GetCurrentProcess
        	mov Handle, eax
        	mov eax, 004AA2F2h
          	mov GoodOffset, eax
        	invoke VirtualProtect, eax, 10,PAGE_EXECUTE_WRITECOPY, offset Old
        	mov [dw_buffer], 0
        	invoke ReadProcessMemory, Handle, 004AA2F0h, offset dw_buffer, 1h, 0
        	mov eax, [dw_buffer]
        	.if eax == 0DBh
        		invoke WriteProcessMemory, Handle, GoodOffset, offset sFPUBugPatch, 1, 0
        	.endif
        .endif
        .if (GetStartUpInfo1!=0)
        	invoke GetCurrentProcess
        	mov Handle, eax
        	mov eax, 00477855h
          	mov GoodOffset, eax
        	invoke VirtualProtectEx, Handle, eax, 22h,PAGE_EXECUTE_WRITECOPY, offset Old
        	invoke WriteProcessMemory, Handle, GoodOffset, offset sGetStartUpInfoPatch, 22h, 0
        .endif
		.if (CaptionPatch1!=0)
			invoke GetCurrentProcess
			mov Handle, eax
			invoke VirtualProtectEx, Handle, 004779E3h, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
			invoke WriteProcessMemory, Handle, 004779E3h, offset sCaptionPatch1, 6h, 0
			invoke VirtualProtectEx, Handle, 004AF670h, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
			invoke WriteProcessMemory, Handle, 004AF670h, offset sCaptionPatch2, 6h, 0
			invoke VirtualProtectEx, Handle, 0041E3B2h, 10h,PAGE_EXECUTE_WRITECOPY, offset Old 
			invoke WriteProcessMemory, Handle, 0041E3B2h, offset sCaptionPatch3, 5h, 0
			invoke VirtualProtectEx, Handle, 004AF680h, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
			invoke WriteProcessMemory, Handle, 004AF680h, offset sCaptionPatch4, 14h, 0
			invoke VirtualProtectEx, Handle, 00475BC1h, 10h,PAGE_EXECUTE_WRITECOPY, offset Old
			invoke WriteProcessMemory, Handle, 00475BC1h, offset sCaptionPatch5, 5h, 0
		.endif
		popad
		ret	

Reset:
	mov ZwSetInformationThread2,0	
	mov ZwQueryInformationProcess2,0			
	mov OpenProcess2,0
	mov InvalidHandle2, 0
	mov ZwQueryObject2, 0
	mov GetTickCount2, 0
	mov Process32NextW2, 0
	ret
			
ODBG_Pluginreset endp

ODBG_Pluginclose proc C
	mov	eax,FALSE
	ret
ODBG_Pluginclose endp

ODBG_Pluginmenu proc C uses ebx origin:dword, data:ptr byte, item:dword
	mov eax,origin
	.if eax==PM_MAIN
		invoke lstrcpy, data, offset sMenu
		mov	eax,TRUE
	.else
		mov	eax,FALSE
	.endif
	ret
	
ODBG_Pluginmenu endp

ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
         mov eax, origin    
        .IF eax == PM_MAIN
        	mov  eax, action
            .IF eax == 0 
            	Invoke Hide
            .ELSEIF eax == 1
                invoke DialogBoxParam, hinst, IDD_OPTION, hwmain, addr DlgOptionProc, NULL
            .elseif eax == 2
            	invoke MessageBox, NULL, offset AboutText, offset AboutCaption,MB_OK
            .ENDIF
		.endif
        ret
ODBG_Pluginaction endp

GetAPIAddress proc sdll:dword, pdll:dword, sapi:dword, papi:dword
	invoke GetModuleHandle,sdll
	mov ebx,[pdll]
	mov [ebx],eax
	.if eax!=0
		invoke GetProcAddress,eax,sapi
		.if eax!=0
			mov ebx,[papi]
			mov [ebx],eax
		.endif
	.endif
	ret
GetAPIAddress endp

DlgOptionProc proc hWnd:HWND, uMsg:UINT, wParam:WPARAM, lParam:LPARAM

    .IF uMsg == WM_INITDIALOG
    	invoke PluginRead
        .if (BlockInput1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK1
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (IsDebuggerPresent1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK2
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (NtGlobalFlag1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK3
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (CheckRemoteDebuggerPresent1!=0)
        	invoke GetDlgItem,hWnd,IDC_CHK4
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (FindWindow1!=0)
        	invoke GetDlgItem,hWnd,IDC_CHK5
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (QueryPerformanceFrequency1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK6
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (OutputDebugStringA1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK7
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (HeapFlag1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK8
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (FPUBug1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK9
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (ZwSetInformationThread1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK10
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (GetStartUpInfo1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK11
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (Aggressive1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK12
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (ZwQueryInformationProcess1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK13
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (DebugBreak1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK14
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (OpenProcess1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK15
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (CreateThread1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK16
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (EPBreak1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK17
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (TLSCallBack1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK18
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (GetTickCount1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK19
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (InvalidHandle1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK20
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (QueryPerformanceCounter1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK21
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (CaptionPatch1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK23
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (ZwYieldExecution1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK24
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (Module32Next1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK26
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (Process32Next1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK27
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (Process32NextW1!=0)
            invoke GetDlgItem,hWnd,CHK_1
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (GetTickCountv2!=0)
            invoke GetDlgItem,hWnd,IDC_CHK28
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (EnumWindows1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK29
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
        .if (ZwQueryObject1!=0)
            invoke GetDlgItem,hWnd,IDC_CHK31
            invoke SendMessage,eax,BM_SETCHECK,BST_CHECKED,0
        .endif
    .ELSEIF uMsg == WM_CLOSE
		invoke EndDialog, hWnd, NULL
    .ELSEIF uMsg==WM_COMMAND
        mov eax,wParam
        mov edx,wParam
        shr edx,16
        .IF dx==BN_CLICKED
            .IF ax==IDC_BTN2        
                invoke EndDialog, hWnd, NULL
            .ELSEIF ax==IDC_BTN1  
                invoke GetDlgItem,hWnd,IDC_CHK1
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr BlockInput_,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr BlockInput_,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK2
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr IsDebuggerPresent_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr IsDebuggerPresent_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK3
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr NtGlobalFlag_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr NtGlobalFlag_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK4
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr CheckRemoteDebuggerPresent_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr CheckRemoteDebuggerPresent_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK5
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr FindWindow_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr FindWindow_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK6
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr QueryPerformanceFrequency_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr QueryPerformanceFrequency_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK7
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr OutputDebugStringA_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr OutputDebugStringA_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK8
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr HeapFlag_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr HeapFlag_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK9
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr FPUBug_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr FPUBug_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK10
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr ZwSetInformationThread_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr ZwSetInformationThread_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK11
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr GetStartUpInfo_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr GetStartUpInfo_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK12
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr Aggressive_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr Aggressive_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK13
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr ZwQueryInformationProcess_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr ZwQueryInformationProcess_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK14
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr DebugBreak_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr DebugBreak_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK15
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr OpenProcess_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr OpenProcess_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK16
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr CreateThread_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr CreateThread_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK17
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr EPBreak_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr EPBreak_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK18
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr TLSCallBack_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr TLSCallBack_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK19
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr GetTickCount_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr GetTickCount_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK20
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr InvalidHandle_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr InvalidHandle_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK21
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr QueryPerformanceCounter_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr QueryPerformanceCounter_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK23
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr CaptionPatch_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr CaptionPatch_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK24
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr ZwYieldExecution_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr ZwYieldExecution_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK26
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr Module32Next_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr Module32Next_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK27
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr Process32Next_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr Process32Next_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK28
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr GetTickCountv2_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr GetTickCountv2_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK29
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr EnumWindows_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr EnumWindows_ ,0
                .endif
                invoke GetDlgItem,hWnd,IDC_CHK31
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr ZwQueryObject_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr ZwQueryObject_ ,0
                .endif
                invoke GetDlgItem,hWnd,CHK_1
                invoke SendMessage,eax,BM_GETSTATE,0,0
                .if eax==BST_CHECKED
                    invoke Pluginwriteinttoini,hinst,addr Process32NextW_ ,1
                .elseif
                    invoke Pluginwriteinttoini,hinst,addr Process32NextW_ ,0
                .endif
               invoke Flash,CTEXT("Saved")
               invoke EndDialog, hWnd, NULL
			.endif
        .ENDIF
    .ELSE
        mov eax, FALSE
        ret
    .ENDIF
    mov eax, TRUE
    ret
DlgOptionProc endp

ODBG_Plugindestroy proc C
        invoke Unregisterpluginclass,addr SVWinClass
        ret
ODBG_Plugindestroy endp

HeapFlag proc 	
    
	invoke TEBAddress
	add eax, 30h
	MOV edi,DWORD PTR SS:[004D5A60h]
	invoke ReadProcessMemory, edi, eax, offset dw_buffer, 4h, 0h
	mov eax, [dw_buffer]
    add eax, 18h
    mov Saved, eax
    invoke Plugingetvalue, VAL_HPROCESS
    invoke ReadProcessMemory, eax, Saved, offset dw_buffer, 4h, 0h
    mov eax, dw_buffer
    ret
    
HeapFlag endp

EPBreakPoint proc
	invoke PEBAddress
    add eax, 0Ch
    mov Saved, eax
    invoke Plugingetvalue, VAL_HPROCESS
    invoke ReadProcessMemory, eax, Saved, offset dw_buffer, 4h, 0h
    mov eax, [dw_buffer]
    add eax, 4Ch
    mov Saved, eax
    invoke Plugingetvalue, VAL_HPROCESS
    invoke ReadProcessMemory, eax, Saved, offset dw_buffer, 4h, 0h
    mov eax, [dw_buffer]
    ret

EPBreakPoint endp

TEBAddress Proc
	Pushad
	MOV esi,DWORD PTR SS:[004D5A64h]
	MOV edi,DWORD PTR SS:[004D5A60h]
	Invoke LoadLibrary, offset sntdll
	.if (eax)
		invoke GetProcAddress, eax, offset sZwQueryInformationThread
		.if (eax)
			Push 0h
			Push 1Ch
			Push offset ThreadInfo
			Push 0h
			Push esi
			Call eax
			mov eax, offset ThreadInfo
			add eax, 4h
			mov eax, dword ptr [eax]
			mov TEBOffset, eax
		.endif
	.endif
	popad
	mov eax, [TEBOffset]
	ret

TEBAddress endp

PEBAddress Proc
	
	Pushad
	MOV esi,DWORD PTR SS:[004D5A64h]
	MOV edi,DWORD PTR SS:[004D5A60h]
	Invoke LoadLibrary, offset sntdll
	.if (eax)
		invoke GetProcAddress, eax, offset sZwQueryInformationThread
		.if (eax)
			Push 0h
			Push 1Ch
			Push offset ThreadInfo
			Push 0h
			Push esi
			Call eax
			mov eax, offset ThreadInfo
			add eax, 4h
			mov eax, dword ptr [eax]
			mov esi, eax
			add esi, 30h
			invoke Plugingetvalue, VAL_HPROCESS
			mov Handle, eax
			invoke ReadProcessMemory, Handle, esi, offset dw_buffer, 4h, 0h
			mov eax, [dw_buffer]
			mov TEBOffset, eax
		.endif
	.endif
	popad
	mov eax, [TEBOffset]
	ret

PEBAddress endp

Aggresive proc 

	Pushad
	invoke Pluginreadintfromini,hinst,addr Aggressive_,0
    mov Aggressive1,eax
    .if eax == 1h
		invoke GetModuleHandle, offset sntdll
		mov ebx, 004D5714h
		mov ebx, dword ptr [ebx+10h]
		.if [Completed] == 1h
			Call ReturnIsDebugger
			dec [Completed]
		.endif
		.if ebx == eax
			Call PatchIsDebugger
		.endif
	.endif
	mov eax, 004D5714h
	mov eax, dword ptr [eax+0ch]
	.if eax == 80000003h
		Invoke Hide
	.endif
	popad
	MOV EDI,004CD624h
	ret

ReturnIsDebugger:
	invoke TEBAddress
	add eax, 30h
	MOV edi,DWORD PTR SS:[004D5A60h]
	invoke ReadProcessMemory, edi, eax, offset dw_buffer, 4h, 0h
	mov eax, [dw_buffer]
	add eax, 2h
	invoke WriteProcessMemory, edi, eax, offset sRetByteFix, 1h, 0h	
	ret

PatchIsDebugger:
	invoke TEBAddress
	add eax, 30h
	MOV edi,DWORD PTR SS:[004D5A60h]
	invoke ReadProcessMemory, edi, eax, offset dw_buffer, 4h, 0h
	mov eax, [dw_buffer]
	add eax, 2h
	invoke WriteProcessMemory, edi, eax, offset sIsDebuggerPresentPatch, 1h, 0h
	mov Completed, 1h
	ret
	
Aggresive endp

Hide proc 
	
       	invoke PluginRead
        invoke LoadLibrary, offset sUser32
        invoke LoadLibrary, offset sKernel32
        invoke LoadLibrary, offset sntdll
        .if (BlockInput1!=0)
        	invoke GetAPIAddress, addr sUser32, offset pUser32, addr sBlockInput, offset pBlockInput
        	invoke Writememory, offset sBlockPatch, pBlockInput, 4h, MM_RESTORE+MM_SILENT
		.endif
        .if (IsDebuggerPresent1!=0)
			invoke PEBAddress
        	add eax, 2h
			mov Location, eax
			invoke Writememory, offset sIsDebuggerPresentPatch, Location,1,MM_RESTORE+MM_SILENT
		.endif
        .if (NtGlobalFlag1!=0)
        	invoke PEBAddress
        	add eax,68h
			mov Location, eax
			invoke Writememory, offset sIsDebuggerPresentPatch, Location,1,MM_RESTORE+MM_SILENT
        .endif
        .if (CheckRemoteDebuggerPresent1!=0)
        	invoke GetAPIAddress,addr sKernel32, offset pKernel32, addr sCheckRemoteDebuggerPresent, offset pCheckRemoteDebuggerPresent
			invoke Writememory, offset sCheckRemoteDebuggerPresentPatch, pCheckRemoteDebuggerPresent,9,MM_RESTORE+MM_SILENT
        .endif
        .if (FindWindow1!=0)
        	invoke GetAPIAddress,addr sUser32, offset pUser32, addr sFindWindow, offset pFindWindow
			invoke Writememory, offset sFindWindowPatch, pFindWindow,9,MM_RESTORE+MM_SILENT
			invoke GetAPIAddress,addr sUser32, offset pUser32, addr sFindWindowExA, offset pFindWindow
			invoke Writememory, offset sFindWindowExAPatch, pFindWindow,9,MM_RESTORE+MM_SILENT
		.endif
		.if (OutputDebugStringA1!=0)
        	invoke GetAPIAddress,addr sKernel32, offset pKernel32, addr sOutputDebugStringA, offset pOutputDebugStringA
			invoke Writememory, offset sOutputDebugStringAPatch, pOutputDebugStringA,8,MM_RESTORE+MM_SILENT
        .endif
        .if (HeapFlag1!=0)
				invoke HeapFlag
				mov Location, eax
				add eax, 10h
				invoke Writememory, offset sHeapFlagPatch, eax,4,MM_RESTORE+MM_SILENT 
				mov eax, dword ptr [Location]
				add eax, 0Ch
				invoke Writememory, offset sHeapFlagPatch2, eax,4,MM_RESTORE+MM_SILENT
				invoke Plugingetvalue, VAL_HPROCESS
				mov Handle, eax
				Invoke PEBAddress
				mov Location, eax
				add eax, 88h
				Invoke ReadProcessMemory, Handle, eax, offset NumberHeaps, 4h, 0
				mov eax, dword ptr [Location]
				add eax, 90h
				Invoke ReadProcessMemory, Handle, eax, offset LocationHeaps, 4h, 0
				Mov eax, [LocationHeaps]
				mov esi, [NumberHeaps]
				mov edi, 0h
			LengthLoop:
				add edi, 4h
				dec esi
				jne LengthLoop
			HeapsLoop:
				Mov eax, [LocationHeaps]
				add eax, edi
				Invoke ReadProcessMemory, Handle, eax, offset dw_buffer, 4h, 0
				mov eax, [dw_buffer]
				mov Location, eax
				add eax, 10h
				invoke Writememory, offset sHeapFlagPatch, eax,4,MM_RESTORE+MM_SILENT 
				mov eax, dword ptr [Location]
				add eax, 0Ch
				invoke Writememory, offset sHeapFlagPatch2, eax,4,MM_RESTORE+MM_SILENT
				mov eax, dword ptr [Location]
				add eax, 1000h
				sub eax, 4h
				invoke Writememory, offset sHeapFlagPatch, eax,4,MM_RESTORE+MM_SILENT 
				sub edi, 4h
				jne HeapsLoop				
        .endif
        .if (ZwSetInformationThread1!=0)
       		.if [ZwSetInformationThread2] == 0h
        		invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
        		invoke GetAPIAddress,addr sntdll, offset pntdll, addr sZwSetInformationThread, offset pZwSetInformationThread
        		invoke ReadProcessMemory, Handle,[pZwSetInformationThread], dw_buffer, 1h, 0 
        		.if byte ptr [dw_buffer] == 0E9h
        			Jmp Skipped
        		.endif        		
        		mov eax, dword ptr [pZwSetInformationThread]
        		add eax, 5h
        		mov ebx, Dword ptr [Virtual]
        		sub ebx, eax
        		mov NewOffset, ebx
        		sub eax, 5h
        		mov JumpLocation, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
        		mov eax, dword ptr [JumpLocation]
        		add eax, 1h
        		invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		mov eax, Dword ptr [Virtual]
        		invoke Writememory, offset sZwSetInformationThreadPatch, eax,16,MM_RESTORE+MM_SILENT
        		mov eax, Dword ptr [Virtual]
        		add eax, 14h
        		mov ebx, dword ptr [pZwSetInformationThread]
        		add ebx, 5
        		sub ebx, eax
        		mov MoveBack, ebx
        		mov eax, Dword ptr [Virtual]
        		add eax, 10h
        		invoke Writememory, offset MoveBack, eax,4,MM_RESTORE+MM_SILENT
        		mov ZwSetInformationThread2, 1
        	Skipped:
        	.endif
        .endif
        .if (ZwQueryInformationProcess1!=0)
        	.if [ZwQueryInformationProcess2] == 0h
        		invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
        		invoke GetAPIAddress,addr sntdll, offset pntdll, addr sZwQueryInformationProcess, offset pZwQueryInformationProcess
        		mov eax, dword ptr [pZwQueryInformationProcess]
        		loop1:
        		inc eax
        		cmp word ptr [eax], 12FFh
        		jne loop1
        		add eax, 2h
        		mov JumpLocation, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
        		mov eax, JumpLocation
        		add eax, 5h
        		mov ebx, dword ptr [Virtual]
        		sub ebx, eax
        		mov NewOffset, ebx
        		sub eax, 4h
        		invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		mov eax, Dword ptr [Virtual]
        		invoke Writememory, offset sZwQueryInformationProcessPatch, eax,2Fh,MM_RESTORE+MM_SILENT
        		invoke Plugingetvalue, VAL_PROCESSID
				mov esi, eax
				invoke CreateToolhelp32Snapshot,TH32CS_SNAPALL, 0
				mov edi, eax
				Mov Process32entry.dwSize, 128h
				.while TRUE
					invoke Process32Next, edi, offset Process32entry
					invoke lstrcmpi, offset Process32entry.szExeFile, offset explorer
				.break .if eax == 0h
				.endw
				invoke CloseHandle, edi
				Push Process32entry.th32ProcessID
				pop ProcID
        		mov eax, [Virtual]
        		add eax, 20h
        		invoke Writememory, offset ProcID, eax,4,MM_RESTORE+MM_SILENT
        		mov ZwQueryInformationProcess2, 1h
        	.endif
        .endif
        .if (DebugBreak1!=0)
        	invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sDebugBreak, offset pDebugBreak
			Invoke Writememory, offset sDebugBreakPatch, pDebugBreak,1,MM_RESTORE+MM_SILENT
		.endif
		.if (OpenProcess1!=0)
			.if [OpenProcess2] == 0h
				invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
				invoke GetAPIAddress, addr sntdll, offset pntdll, addr sOpenProcess, offset pOpenProcess
				mov ebx, [Virtual]
				mov eax, dword ptr [pOpenProcess]
				add eax, 5
				sub ebx, eax
				mov NewOffset, ebx
				sub eax, 5
				mov JumpLocation, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
				mov eax, [JumpLocation]
				add eax, 1h
				invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		Invoke Writememory, offset sOpenProcessPatch, Virtual,26,MM_RESTORE+MM_SILENT
        		mov ebx, [Virtual]
        		add ebx, 19h
				invoke Writememory, offset sJump, ebx,1,MM_RESTORE+MM_SILENT
				mov ebx, [Virtual]
        		add ebx, 1Eh
				mov eax, dword ptr [pOpenProcess]
				add eax, 5h
				sub eax, ebx
				mov NewOffset, eax
				sub ebx, 4h
        		invoke Writememory, offset NewOffset, ebx,4,MM_RESTORE+MM_SILENT        		
        		invoke Plugingetvalue,VAL_PROCESSID
        		mov Handle, eax
        		mov eax, [Virtual]
        		add eax, 4h
        		invoke Writememory, offset Handle, eax,4,MM_RESTORE+MM_SILENT
        		mov OpenProcess2, 1h
        	.endif
		.endif
		.if (CreateThread1!=0)
			invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sCreateThread, offset pCreateThread
			Invoke Writememory, offset sCreateThreadPatch, pCreateThread,3,MM_RESTORE+MM_SILENT
		.endif
		.if (GetTickCount1!=0)
			.if (GetTickCountv2!=0)
				.if GetTickCount2 == 0h
					invoke Plugingetvalue,VAL_HPROCESS
        			mov Handle, eax
        			invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        			mov Virtual, eax
					invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sGetTickCount, offset pGetTickCount
					mov ebx, [Virtual]
					mov eax, dword ptr [pGetTickCount]
					add eax, 5
					sub ebx, eax
					mov NewOffset, ebx
					sub eax, 5
					mov JumpLocation, eax
        			invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
					mov eax, [JumpLocation]
					add eax, 1h
					invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        			Invoke Writememory, offset sGetTickCountv2Patch, Virtual,26,MM_RESTORE+MM_SILENT
        			mov [GetTickCount2], 1h
        		.endif
			.elseif
				invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sGetTickCount, offset pGetTickCount
				Invoke Writememory, offset sGetTickCountPatch, pGetTickCount,0Ch,MM_RESTORE+MM_SILENT
			.endif
		.endif
		.if (InvalidHandle1!=0)
			.if [InvalidHandle2] == 0h
				invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
				invoke GetAPIAddress, addr sntdll, offset pntdll, addr sKiRaiseUserExceptionDispatcher, offset pKiRaiseUserExceptionDispatcher
				mov ebx, [Virtual]
				mov eax, dword ptr [pKiRaiseUserExceptionDispatcher]
				add eax, 5
				sub ebx, eax
				mov NewOffset, ebx
				sub eax, 5
				mov JumpLocation, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
				mov eax, [JumpLocation]
				add eax, 1h
				invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		Invoke Writememory, offset sKiRaiseUserExceptionDispatcherPatch, Virtual,26,MM_RESTORE+MM_SILENT
        		mov ebx, [Virtual]
        		add ebx, 10h
				invoke Writememory, offset sJump, ebx,1,MM_RESTORE+MM_SILENT
				mov ebx, [Virtual]
        		add ebx, 15h
				mov eax, dword ptr [pKiRaiseUserExceptionDispatcher]
				add eax, 6h
				sub eax, ebx
				mov NewOffset, eax
				sub ebx, 4h
        		invoke Writememory, offset NewOffset, ebx,4,MM_RESTORE+MM_SILENT
        		mov InvalidHandle2, 1h
        	.endif       		
		.endif
		.if (ZwYieldExecution1!=0)
			invoke GetAPIAddress, addr sntdll, offset pntdll, addr sZwYieldExecution, offset pZwYieldExecution
			Invoke Writememory, offset sZwYieldExecutionPatch, pZwYieldExecution,07h,MM_RESTORE+MM_SILENT
		.endif
		.if (Process32Next1!=0)
			.if (Process32NextW1!=0)
				.if (Process32NextW2!=0)
					jmp Process32NextWEnd
				.endif
				invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
        		invoke GetAPIAddress,addr sKernel32, offset pKernel32, addr sProcess32Next, offset pProcess32Next
				mov eax, pProcess32Next
			@@:
				inc eax
				cmp dword ptr [eax], 900008C2h
				jne @B
				mov JumpLocation, eax
				mov edi, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
        		mov eax, JumpLocation
        		add eax, 5h
        		mov ebx, dword ptr [Virtual]
        		sub ebx, eax
        		mov NewOffset, ebx
        		sub eax, 4h
        		invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		mov eax, Dword ptr [Virtual]
        		invoke Writememory, offset sProcess32NextWHook, eax,26h,MM_RESTORE+MM_SILENT
        		invoke Plugingetvalue,VAL_PROCESSID
        		mov dw_buffer, eax
        		mov eax, [Virtual]
        		add eax, 9h
        		invoke Writememory, offset dw_buffer, eax,4,MM_RESTORE+MM_SILENT
        		invoke GetCurrentProcessId
        		mov dw_buffer, eax
        		mov eax, [Virtual]
        		add eax, 18h
        		invoke Writememory, offset dw_buffer, eax,4,MM_RESTORE+MM_SILENT
        		invoke Plugingetvalue, VAL_PROCESSID
				mov esi, eax
				invoke CreateToolhelp32Snapshot,TH32CS_SNAPALL, 0
				mov edi, eax
				Mov Process32entry.dwSize, 128h
				.while TRUE
					invoke Process32Next, edi, offset Process32entry
					invoke lstrcmpi, offset Process32entry.szExeFile, offset explorer
				.break .if eax == 0h
				.endw
				invoke CloseHandle, edi
				Push Process32entry.th32ProcessID
				pop ProcID
				mov eax, [Virtual]
        		add eax, 12h
				invoke Writememory, offset ProcID, eax,4,MM_RESTORE+MM_SILENT
				mov Process32NextW2, 1h
			Process32NextWEnd:
			.else
				invoke GetAPIAddress,addr sKernel32, offset pKernel32, addr sProcess32Next, offset pProcess32Next
				invoke Writememory, offset sProcess32NextPatch, pProcess32Next,5h,MM_RESTORE+MM_SILENT
			.endif
		.endif
		.if (Module32Next1!=0)
			invoke GetAPIAddress,addr sKernel32, offset pKernel32, addr sModule32Next, offset pModule32Next
			invoke Writememory, offset sModule32NextPatch, pModule32Next,5h,MM_RESTORE+MM_SILENT
		.endif
		.if (EnumWindows1!=0)
			invoke GetAPIAddress,addr sUser32, offset pUser32, addr sEnumWindows, offset pEnumWindows
			invoke Writememory, offset sEnumWindowsPatch, pEnumWindows,5h,MM_RESTORE+MM_SILENT
		.endif
		.if (ZwQueryObject1!=0)
			.if [ZwQueryObject2] == 0h
        		invoke Plugingetvalue,VAL_HPROCESS
        		mov Handle, eax
        		invoke VirtualAllocEx, Handle, NULL, 1000h,1000h,40h
        		mov Virtual, eax
        		invoke GetAPIAddress,addr sntdll, offset pntdll, addr sZwQueryObject, offset pZwQueryObject
        		mov eax, dword ptr [pZwQueryObject]
        		loop4:
        		inc eax
        		cmp word ptr [eax], 12FFh
        		jne loop4
        		add eax, 2h
        		mov JumpLocation, eax
        		invoke Writememory, offset sJump, eax,1,MM_RESTORE+MM_SILENT
        		mov eax, JumpLocation
        		add eax, 5h
        		mov ebx, dword ptr [Virtual]
        		sub ebx, eax
        		mov NewOffset, ebx
        		sub eax, 4h
        		invoke Writememory, offset NewOffset, eax,4,MM_RESTORE+MM_SILENT
        		mov eax, Dword ptr [Virtual]
        		invoke Writememory, offset sZwQueryObjectPatch, eax,18h,MM_RESTORE+MM_SILENT
        		mov ZwQueryObject2, 1h
        	.endif
        .endif
        .if (QueryPerformanceFrequency1!=0)
        	invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sQueryPerformanceFrequency, offset pQueryPerformanceFrequency
        	invoke Writememory, offset sQueryPerformancePatch, pQueryPerformanceFrequency, 5h, MM_RESTORE+MM_SILENT
        	invoke GetAPIAddress, addr sntdll, offset pKernel32, addr sZwQueryPerformanceFrequency, offset pZwQueryPerformanceFrequency
        	invoke Writememory, offset sZwQueryPerformancePatch, pZwQueryPerformanceFrequency, 5h, MM_RESTORE+MM_SILENT
        .endif
        .if (QueryPerformanceCounter1!=0)
        	invoke GetAPIAddress, addr sKernel32, offset pKernel32, addr sQueryPerformanceCounter, offset pQueryPerformanceCounter
        	invoke Writememory, offset sQueryPerformancePatch, pQueryPerformanceCounter, 5h, MM_RESTORE+MM_SILENT
        	invoke GetAPIAddress, addr sntdll, offset pKernel32, addr sZwQueryPerformanceCounter, offset pZwQueryPerformanceCounter
        	invoke Writememory, offset sZwQueryPerformancePatch, pZwQueryPerformanceCounter, 5h, MM_RESTORE+MM_SILENT
        .endif
	ret





Hide endp

BreakPoints Proc
	Pushad
	invoke PluginRead
	.if (EPBreak1!=0)
		lea eax, EPBreakPointCode
		invoke CreateThread,NULL,NULL,eax,NULL,NULL,LetsGo
	.endif
	.if (TLSCallBack1!=0)
		invoke Plugingetvalue, VAL_HPROCESS
		mov Handle, eax
		invoke TEBAddress              
   		add eax,2ch
       	invoke ReadProcessMemory, Handle, eax, offset dw_buffer, 4h, 0 
        mov eax,[dw_buffer]
        sub eax, 14h
        invoke ReadProcessMemory, Handle, eax, offset dw_buffer, 4h, 0 
        mov eax,[dw_buffer]
        invoke ReadProcessMemory, Handle, eax, offset dw_buffer, 4h, 0
        mov eax,[dw_buffer]
        .if eax == 0
        	JMP NOTLS2
        .endif
    	Invoke Tempbreakpoint, eax, TY_ONESHOT
    	NOTLS2:
	.endif
		popad
		CMP DWORD PTR DS:[004D734Ch],0
		mov dword ptr [esp], 0042F91Fh
	ret
	
EPBreakPointCode:
		invoke Sleep, 200h
		invoke Plugingetvalue, VAL_MAINTHREADID
		invoke Findthread, eax
		add eax, 14h
		mov ebx, dword ptr [eax]
    	dec ebx
		mov Range1, ebx
		add ebx, 2h
		mov Range2, ebx
		Invoke Deletebreakpoints, [Range1], [Range2], ATR_REMOVE
		invoke ExitThread, TRUE
		ret


BreakPoints endp

PluginRead Proc
		Pushad
	    invoke Pluginreadintfromini,hinst,addr FPUBug_,0
        mov FPUBug1,eax
        invoke Pluginreadintfromini,hinst,addr OutputDebugStringA_,0
        mov OutputDebugStringA1,eax
    	invoke Pluginreadintfromini,hinst,addr AutoHide_,0
        mov AutoHide1,eax
        invoke Pluginreadintfromini,hinst,addr BlockInput_,0
        mov BlockInput1,eax
        invoke Pluginreadintfromini,hinst,addr IsDebuggerPresent_, 0 
        mov IsDebuggerPresent1, eax
        invoke Pluginreadintfromini,hinst,addr NtGlobalFlag_, 0 
        mov NtGlobalFlag1, eax
        invoke Pluginreadintfromini,hinst,addr CheckRemoteDebuggerPresent_, 0 
        mov CheckRemoteDebuggerPresent1, eax
        invoke Pluginreadintfromini,hinst,addr FindWindow_, 0 
        mov FindWindow1, eax
        invoke Pluginreadintfromini,hinst,addr HeapFlag_,0
        mov HeapFlag1,eax
        invoke Pluginreadintfromini,hinst,addr ZwSetInformationThread_,0
        mov ZwSetInformationThread1,eax
        invoke Pluginreadintfromini,hinst,addr GetStartUpInfo_,0
        mov GetStartUpInfo1,eax
        invoke Pluginreadintfromini,hinst,addr ZwQueryInformationProcess_,0
        mov ZwQueryInformationProcess1,eax
        invoke Pluginreadintfromini,hinst,addr DebugBreak_,0
        mov DebugBreak1,eax
        invoke Pluginreadintfromini,hinst,addr OpenProcess_,0
        mov OpenProcess1,eax
        invoke Pluginreadintfromini,hinst,addr CreateThread_,0
        mov CreateThread1,eax
        invoke Pluginreadintfromini,hinst,addr EPBreak_,0
        mov EPBreak1,eax
        invoke Pluginreadintfromini,hinst,addr TLSCallBack_,0
        mov TLSCallBack1,eax
        invoke Pluginreadintfromini,hinst,addr GetTickCount_,0
        mov GetTickCount1,eax
        invoke Pluginreadintfromini,hinst,addr InvalidHandle_,0
        mov InvalidHandle1,eax
       	invoke Pluginreadintfromini,hinst,addr CaptionPatch_,0
        mov CaptionPatch1,eax
        invoke Pluginreadintfromini,hinst,addr ZwYieldExecution_,0
        mov ZwYieldExecution1,eax
        invoke Pluginreadintfromini,hinst,addr Process32Next_,0
        mov Process32Next1,eax
        invoke Pluginreadintfromini,hinst,addr Module32Next_,0
        mov Module32Next1,eax
       	invoke Pluginreadintfromini,hinst,addr GetTickCountv2_,0
        mov GetTickCountv2,eax
        invoke Pluginreadintfromini,hinst,addr EnumWindows_,0
        mov EnumWindows1,eax
	    invoke Pluginreadintfromini,hinst,addr ZwQueryObject_,0
        mov ZwQueryObject1,eax
        invoke Pluginreadintfromini,hinst,addr Aggressive_,0
        mov Aggressive1,eax
        invoke Pluginreadintfromini,hinst,addr QueryPerformanceFrequency_,0
        mov QueryPerformanceFrequency1,eax
        invoke Pluginreadintfromini,hinst,addr QueryPerformanceCounter_,0
        mov QueryPerformanceCounter1,eax
        invoke Pluginreadintfromini,hinst,addr Process32NextW_,0
        mov Process32NextW1, eax
   		popad
		ret

PluginRead endp


end DllEntryPoint
