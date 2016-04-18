.386
.model flat,stdcall
option casemap:none

include ArmaDetach.inc

.code

DllEntry proc hInst:HINSTANCE, reason:DWORD, reserved1:DWORD
	.if reason == DLL_PROCESS_ATTACH
	m2m hInstance,hInst
	.endif
	mov eax,TRUE
	ret
DllEntry endp

ODBG_Pluginaction proc C origin:dword, action:dword, item:dword
	mov eax,origin
	.if eax==PM_MAIN
		mov eax, action
		.if eax==0 ;Process Detach
			mov ActionCheck,1
			invoke Plugingetvalue,VAL_PROCESSID
			.if eax!=0
				invoke GetAPIAddress,addr sKernel32,offset pKernel32,addr sWaitForDebugEvent,offset pWaitForDebugEvent
				invoke GetAPIAddress,addr sKernel32,offset pKernel32,addr sWriteProcessMemory,offset pWriteProcessMemory
				invoke GetAPIAddress,addr sKernel32,offset pKernel32,addr sDebugActiveProcessStop,offset pDebugActiveProcessStop
				.if eax!=0
					invoke Setmembreakpoint,MEMBP_READ+MEMBP_WRITE,pWriteProcessMemory,1
					invoke Go,NULL,NULL,STEP_RUN,TRUE,NULL
				.endif
			.else
				invoke Error,offset sErrorApp
			.endif
		.elseif eax==1 ;About Message
			invoke  MessageBox, hWin, offset sAboutInfo, offset sPluginTitle, MB_ICONINFORMATION
		.endif
	.endif
	ret
ODBG_Pluginaction endp

ODBG_Pluginclose proc C
	mov	eax,FALSE
	ret
ODBG_Pluginclose endp

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

ODBG_Pausedex proc C reason:dword, extdata:dword, preg:ptr t_reg, debugevent:ptr DEBUG_EVENT
	mov eax,reason
	.if ActionCheck!=0
		.if eax==PP_MEMBREAK ;Memory Breakpoint
			.if Counter==2 ;If 3rd memory breakpoint in WriteProcessMemory
				inc Counter
				;Get entry point
				mov esi,preg
				mov eax,[esi].t_reg.r[REG_ESP]
				add eax,08h
				mov pBuffer,eax
				invoke Readmemory,offset pOEP,eax,4,MM_RESTORE+MM_SILENT
				;Get original bytes
				add pBuffer,4
				invoke Readmemory,offset pBuffer,pBuffer,4,MM_RESTORE+MM_SILENT
				invoke Readmemory,offset OriginalBytes,pBuffer,2,MM_RESTORE+MM_SILENT
				;Write infinite loop
				invoke Writememory,offset sLoopPatch,pBuffer,2,MM_RESTORE+MM_SILENT
				;Set memory breakpoint in WaitForDebugEvent
				invoke Setmembreakpoint,MEMBP_READ+MEMBP_WRITE,pWaitForDebugEvent,1
				;Run!
				invoke Go,NULL,NULL,STEP_RUN,TRUE,NULL
			.elseif Counter==4 ;If 2nd memory breakpoint in WaitForDebugEvent
				inc Counter
				;Get child process ID
				mov esi,preg
				mov eax,[esi].t_reg.r[REG_ESP]
				add eax,04h
				invoke Readmemory,offset pChildPID,eax,4,MM_RESTORE+MM_SILENT
				add pChildPID,4
				invoke Readmemory,offset pChildPID,pChildPID,4,MM_RESTORE+MM_SILENT
				;Set data and write DebugActiveProcessStop patch
				mov eax, pChildPID
				mov dword ptr ds:[sDebugActiveProcessPatch+1],eax
				mov eax,[esi].t_reg.r[RS_EIP]
				add eax,0Ch
				mov dword ptr ds:[sDebugActiveProcessPatch+7],eax
				mov eax, pDebugActiveProcessStop
				mov dword ptr ds:[sDebugActiveProcessPatch+0Ch],eax
				invoke Writememory,offset sDebugActiveProcessPatch,[esi].t_reg.r[RS_EIP],21h,MM_RESTORE+MM_SILENT
				;Remove memory breakpoint
				invoke Setmembreakpoint,NULL,NULL,NULL
				;Set temporary breakpoint
				mov eax,[esi].t_reg.r[RS_EIP]
				add eax,0Bh
				invoke Tempbreakpoint,eax,TY_ONESHOT
				;Run!
				invoke Go,NULL,NULL,STEP_RUN,TRUE,NULL
			.else
				;Increment breakpoint counter
				inc Counter
				;Run!
				invoke Go,NULL,NULL,STEP_RUN,TRUE,NULL
			.endif
		.elseif eax==PP_INT3BREAK ;INT3 Breakpoint
			.if Counter==5 ;If temporary breakpoint
				mov ActionCheck,0
				mov Counter,0
				;Process detached successfully!
				mov eax,OriginalBytes
				xchg al,ah
				invoke wsprintf,addr sBuffer,offset sDetachReady, pChildPID, pOEP, eax
				invoke MessageBox,hWin,addr sBuffer, offset sPluginTitle,MB_ICONINFORMATION
			.endif
		.endif
	.endif
	ret
ODBG_Pausedex endp

ODBG_Pluginreset proc C
	mov ActionCheck,0
	mov Counter,0
	ret
ODBG_Pluginreset endp

GetAPIAddress proc sdll:dword, pdll:dword, sapi:dword, papi:dword
	invoke GetModuleHandle,sdll
	mov ebx,[pdll]
	mov [ebx],eax
	.if eax!=0
		invoke GetProcAddress,eax,sapi
		.if eax!=0
			mov ebx,[papi]
			mov [ebx],eax
		.else
			invoke Error,offset sErrorDll, sapi
			mov eax,FALSE
		.endif
	.else
		invoke Error,offset sErrorDll, sdll
		mov eax,FALSE
	.endif
	ret
GetAPIAddress endp

End DllEntry