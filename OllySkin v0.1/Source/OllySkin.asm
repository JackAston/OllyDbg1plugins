.686
.model flat,stdcall
option casemap:none
	
include OllySkin.inc

_strlen proc uses esi lpString:DWORD
	mov    eax,lpString
	mov    esi,eax
@@:
	cmp    byte ptr [esi],0
	jz     @F
	inc    esi
	jmp    @B
@@:
	sub    esi,eax
	xchg   eax,esi
	ret
_strlen endp

_strcpy proc uses ecx esi edi lpDest:DWORD,lpSource:DWORD
	invoke _strlen,lpSource
	mov    ecx,eax
	mov    esi,lpSource
	mov    edi,lpDest
	cld
	rep    movsb
	mov    byte ptr [edi],0
	ret
_strcpy endp

_strcat proc uses esi edi string1:DWORD,string2:DWORD
	mov    esi,string2
	mov    edi,string1
	invoke _strlen,edi
	add    edi,eax
	invoke _strcpy,edi,esi
	ret
_strcat endp

_strrchr proc uses ebx esi string:DWORD,char:BYTE
	mov    bl,char
	mov    esi,string
	invoke _strlen,esi
	mov    ecx,eax
	add    esi,ecx
@@:
	cmp    byte ptr [esi],bl
	je     @char_found
	dec    esi
	dec    ecx
	jnz    @B
	xor    eax,eax
	jmp    @F                     ; eax 0 == char not found
@char_found:
	mov    eax,esi
@@:
	ret
_strrchr endp

SetSkin proc
	pushad
	.if bEnable==TRUE
		.if bInit==FALSE
			call   _InitSkin
			inc    bInit
		.endif
		push   offset SkinFile
		call   _LoadSkin
		call   _ApplySkin
	.endif
	popad
	ret
SetSkin endp

GetOllydbgIni proc
	pushad
	invoke GetModuleFileName,NULL,addr Ollydbg_Ini,sizeof Ollydbg_Ini
	invoke _strrchr,addr Ollydbg_Ini,'\'
	mov    edi,eax
	inc    edi
	mov    esi,offset szIni
	mov    ecx,sizeof szIni 
@@:
	cld
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
		invoke SendDlgItemMessage,hWnd,1001,WM_SETTEXT,0,addr szEnable
		invoke SendDlgItemMessage,hWnd,1002,WM_SETTEXT,0,addr SkinFile
		invoke SendDlgItemMessage,hWnd,1003,WM_SETTEXT,0,addr szSelect
		invoke SendDlgItemMessage,hWnd,1004,WM_SETTEXT,0,addr szPluginTitle
		invoke SendDlgItemMessage,hWnd,1005,WM_SETTEXT,0,addr szClose
		
		.if bEnable==TRUE
			mov    eax,BST_CHECKED
		.else
			mov    eax,BST_UNCHECKED
		.endif
		invoke SendDlgItemMessage,hWnd,1001,BM_SETCHECK,eax,0
		
		comment %
		invoke GetClientRect,hWnd,addr rc          ; Groupbox anpassen (*passt auch so alles)
		invoke GetDlgItem,hWnd,1004
		push   eax
		mov    eax,rc.RECT.bottom
		sub    eax,14
		mov    ecx,rc.RECT.right
		sub    ecx,14
		invoke MoveWindow,[esp+20],14/2,14/2,ecx,eax,TRUE
		pop    eax
		invoke SendMessage,eax,WM_SETTEXT,0,addr szPluginTitle
		;%
		
	.elseif uMsg==WM_COMMAND
		mov    eax,wParam
		.if ax==1001
			.if bEnable==FALSE	
				invoke _strlen,addr SkinFile
				.if (!eax)
					invoke MessageBox,hWnd,addr szNoFile,0,MB_OK
					invoke SendDlgItemMessage,hWnd,1001,BM_SETCHECK,BST_UNCHECKED,0
					ret
				.endif
				mov    bEnable,TRUE
				invoke SetSkin
			.else
				mov    bEnable,FALSE
				call   _DisableSkin
			.endif
			
		.elseif ax==1003
			invoke _strcpy,addr TempBuffer,addr PluginPath
			invoke _strcat,addr TempBuffer,addr szSkins
			mov    byte ptr [Buffer],0
			
			mov    ofn.OPENFILENAME.lStructSize,sizeof OPENFILENAME
			m2m    ofn.OPENFILENAME.hwndOwner,hWnd
			m2m    ofn.OPENFILENAME.hInstance,hInstance
			mov    ofn.OPENFILENAME.lpstrFilter,offset szFilter
			mov    ofn.OPENFILENAME.lpstrCustomFilter,NULL
			mov    ofn.OPENFILENAME.Flags,OFN_FILEMUSTEXIST+OFN_PATHMUSTEXIST+OFN_LONGNAMES+OFN_NODEREFERENCELINKS+OFN_NONETWORKBUTTON+OFN_HIDEREADONLY+OFN_EXPLORER
			mov    ofn.OPENFILENAME.lpstrFile,offset Buffer
			mov    ofn.OPENFILENAME.nMaxFile,sizeof Buffer
			mov    ofn.OPENFILENAME.lpstrInitialDir,offset TempBuffer
			invoke GetOpenFileName,addr ofn
			.if (eax)
				invoke SendDlgItemMessage,hWnd,1002,WM_SETTEXT,0,addr Buffer
				
				; file checks
				
				invoke _strcpy,addr SkinFile,addr Buffer
				invoke SetSkin
			.else
				ret
			.endif
			
		.elseif ax==1005
			invoke SendMessage,hWnd,WM_SYSCOMMAND,SC_CLOSE,0
			
		.endif
		
	.elseif uMsg==WM_LBUTTONDOWN
		invoke SendMessage,hWnd,WM_NCLBUTTONDOWN,HTCAPTION,0
		
	.elseif uMsg==WM_CLOSE
		.if bEnable==TRUE
			mov    eax,offset szOne
		.else
			mov    eax,offset szZero
		.endif
		invoke WritePrivateProfileString,offset szPluginTitle,offset szEnable,eax,offset Ollydbg_Ini
		invoke WritePrivateProfileString,offset szPluginTitle,offset szSkinFile,offset SkinFile,offset Ollydbg_Ini
		invoke EndDialog,hWnd,NULL
	.else
		xor    eax,eax
		ret
	.endif
	return TRUE
MainProc endp 

ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
	mov    eax,action
	.if eax == 0      ;config
		invoke DialogBoxParam,hInstance,1000,hWin,offset MainProc,0
	.elseif eax == 1  ;about
		invoke MessageBox,hWin,offset szAboutInfo,offset szPluginTitle,MB_ICONINFORMATION
	.endif
	ret
ODBG_Pluginaction endp

ODBG_Pluginclose proc C
;	call   _FreeSkin     ; fehler: olly beendet sich nicht
	xor    eax,eax
	ret
ODBG_Pluginclose endp

ODBG_Plugindata proc C shortname:ptr byte
        invoke _strcpy,shortname,offset szPluginTitle
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
		invoke GetPrivateProfileString,offset szHistory,offset szPluginPath,NULL,offset PluginPath,sizeof PluginPath,offset Ollydbg_Ini
		invoke GetPrivateProfileString,offset szPluginTitle,offset szEnable,NULL,offset Buffer,sizeof Buffer,offset Ollydbg_Ini
		.if byte ptr [Buffer]=='1'
			mov   bEnable,TRUE
		.endif
		invoke GetPrivateProfileString,offset szPluginTitle,offset szSkinFile,NULL,offset SkinFile,sizeof SkinFile,offset Ollydbg_Ini
		
;		invoke GetMenu,hWin
;		invoke GetSubMenu,eax,0
;		mov    hFileMenu,eax
;		invoke AppendMenu,eax,MF_SEPARATOR,0,0
;		invoke AppendMenu,hFileMenu,MF_STRING,2004,offset sz_Menu
		;invoke DestroyMenu,eax
		
		invoke _strcpy,addr Buffer,addr PluginPath
		invoke _strcat,addr Buffer,addr szDll
		
		mov    hDll,rv(LoadLibrary,addr Buffer)
		mov    _InitSkin,rv(GetProcOffset,eax,addr szInit)
		mov    _LoadSkin,rv(GetProcOffset,hDll,addr szLoadSkin)
		mov    _ApplySkin,rv(GetProcOffset,hDll,addr szApplySkin)
		mov    _DisableSkin,rv(GetProcOffset,hDll,addr szDisableSkin)
;		mov    _FreeSkin,rv(GetProcOffset,hDll,addr szFreeSkin)
		
		; *** Nag fix ***
		invoke GetProcOffset,hDll,addr szAboutSkin
;		mov    _AboutSkin,eax
		sub    eax,0eb4ch
		mov    byte ptr [eax],0c3h  ; ret
		invoke SetSkin
		
		
		
		xor    eax,eax
	.endif
	ret
ODBG_Plugininit endp

ODBG_Pluginmenu proc C origin:dword,data:ptr byte,item:dword
	mov    eax,origin
	.if eax == PM_MAIN
		invoke _strcpy,data,offset szMenu
		mov	   eax,TRUE
	.else
		xor    eax,eax
	.endif
	ret
ODBG_Pluginmenu endp

end DllEntry