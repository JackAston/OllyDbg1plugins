.386
.model flat,stdcall
option casemap:none

include TransOlly.inc

GetOllydbgIni proc
	pushad
	invoke GetModuleFileName,NULL,offset Buffer,MAX_PATH
	invoke crt_strrchr,offset Buffer,'\'
	mov    edi,eax
	inc    edi
	mov    esi,offset szIni
	mov    ecx,sizeof szIni 
@@:
	rep    movsb
	mov    byte ptr [edi],0
	popad
	ret
GetOllydbgIni endp

DllEntry proc hInst:HINSTANCE,reason:DWORD,reserved1:DWORD
	.if reason == DLL_PROCESS_ATTACH
		m2m    hInstance,hInst
	.endif
	return TRUE
DllEntry endp

MainProc proc hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM
	
	.if uMsg==WM_INITDIALOG
		invoke SetWindowText,hWnd,offset szPluginTitle
		invoke SendDlgItemMessage,hWnd,1001,WM_SETTEXT,0,offset szEnable
		invoke SendDlgItemMessage,hWnd,1002,TBM_SETRANGEMAX,0,0
		invoke SendDlgItemMessage,hWnd,1002,TBM_SETRANGEMAX,0,255
		invoke SendDlgItemMessage,hWnd,1002,TBM_SETLINESIZE,0,255
		invoke SendDlgItemMessage,hWnd,1002,TBM_SETPOS,TRUE,Alpha
		.if bEnable==TRUE
			invoke EnableWindow,rv(GetDlgItem,hWnd,1002),TRUE
			mov    eax,BST_CHECKED
		.else
			invoke EnableWindow,rv(GetDlgItem,hWnd,1002),FALSE
			mov    eax,BST_UNCHECKED
		.endif
		invoke SendDlgItemMessage,hWnd,1001,BM_SETCHECK,eax,0
		
	.elseif uMsg==WM_HSCROLL
		.if bEnable==TRUE
			invoke SendDlgItemMessage,hWnd,1002,TBM_GETPOS,0,0
			mov    Alpha,eax
			invoke SetLayeredWindowAttributes,hWin,0,eax,LWA_ALPHA
		.endif
		
	.elseif uMsg==WM_COMMAND
		.if wParam==1001
			.if bEnable==FALSE
				invoke GetWindowLong,hWin,GWL_EXSTYLE
				test   eax,WS_EX_LAYERED
				jnz    @F
				add    eax,WS_EX_LAYERED
			@@:
				invoke SetWindowLong,hWin,GWL_EXSTYLE,eax
				invoke EnableWindow,rv(GetDlgItem,hWnd,1002),TRUE
				mov    bEnable,TRUE
				invoke SetLayeredWindowAttributes,hWin,0,Alpha,LWA_ALPHA
			.else
				invoke GetWindowLong,hWin,GWL_EXSTYLE
				test   eax,WS_EX_LAYERED
				jz     @F 
				sub    eax,WS_EX_LAYERED
			@@:
				invoke SetWindowLong,hWin,GWL_EXSTYLE,eax
				invoke EnableWindow,rv(GetDlgItem,hWnd,1002),FALSE
				mov    bEnable,FALSE
			.endif
		.endif
		
	.elseif uMsg==WM_CLOSE
		.if bEnable==TRUE
			mov    eax,offset szOne
		.else
			mov    eax,offset szNull
		.endif
		invoke WritePrivateProfileString,offset szPluginTitle,offset szEnable,eax,offset Buffer
		invoke crt__itoa,Alpha,offset Settings,10
		invoke WritePrivateProfileString,offset szPluginTitle,offset szAlpha,offset Settings,offset Buffer
		invoke EndDialog,hWnd,0
	.else
		xor    eax,eax
		ret
	.endif
	return TRUE
MainProc endp 

ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
	mov    eax,action
	.if eax == 0      ;options
		invoke DialogBoxParam,hInstance,1000,hWin,offset MainProc,0
	.elseif eax == 1  ;about
		invoke MessageBox,hWin,offset szAboutInfo,offset szPluginTitle,MB_ICONINFORMATION
	.endif
	ret
ODBG_Pluginaction endp

ODBG_Pluginclose proc C
	xor    eax,eax
	ret
ODBG_Pluginclose endp

ODBG_Plugindata proc C shortname:ptr byte
        invoke lstrcpy,shortname,offset szPluginTitle
        mov    eax,PLUGIN_VERSION
        ret
ODBG_Plugindata endp

ODBG_Plugininit proc C ollydbgversion:dword,hwnd:HWND,features:ptr dword
	.if ollydbgversion < PLUGIN_VERSION
		mov	   eax,TRUE
	.else
		m2m    hWin,hwnd
		invoke Addtolist,0,0,offset szPluginTitle
		invoke Addtolist,0,-1,offset szPluginInfo
		invoke GetOllydbgIni
		invoke GetPrivateProfileString,offset szPluginTitle,offset szEnable,NULL,offset Settings,sizeof Settings,offset Buffer
		invoke StrToInt,offset Settings
		mov    bEnable,eax
		invoke GetPrivateProfileString,offset szPluginTitle,offset szAlpha,NULL,offset Settings,sizeof Settings,offset Buffer
		invoke StrToInt,offset Settings
		cmp    eax,25
		jl     set_255
		cmp    eax,255
		jng    @F
	set_255:
		mov    eax,255
	@@:
		mov    Alpha,eax
		.if bEnable==TRUE
			invoke GetWindowLong,hWin,GWL_EXSTYLE
			test   eax,WS_EX_LAYERED
			jnz    @F
			add    eax,WS_EX_LAYERED
		@@:
			invoke SetWindowLong,hWin,GWL_EXSTYLE,eax
			invoke SetLayeredWindowAttributes,hWin,0,Alpha,LWA_ALPHA
		.endif
		xor    eax,eax
	.endif
	ret
ODBG_Plugininit endp

ODBG_Pluginmenu proc C uses ebx origin:dword,data:ptr byte,item:dword
	mov    eax,origin
	.if eax == PM_MAIN
		invoke lstrcpy,data,offset szMenu
		mov	   eax,TRUE
	.else
		xor    eax,eax
	.endif
	ret
ODBG_Pluginmenu endp

end DllEntry