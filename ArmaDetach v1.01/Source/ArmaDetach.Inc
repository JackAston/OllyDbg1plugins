include windows.inc
include kernel32.inc
include user32.inc
include Libs\Plugin.inc

includelib kernel32.lib
includelib user32.lib
includelib Libs\OllyDbg.lib

GetAPIAddress PROTO :DWORD,:DWORD,:DWORD,:DWORD

m2m macro M1, M2
	push M2
	pop  M1
endm

.data
sPluginTitle TCHAR "Armadillo Process Detach",0
sPluginInfo TCHAR "  Copyright (C) 2005 AvAtAr",0
sAboutInfo TCHAR "Armadillo Process Detach Plugin v1.0",10,13,"Author: AvAtAr {RES}",10,13,"eMail: avatar@appznet.org",0
sMenu TCHAR "0 Detach!|1 About",0
sErrorApp TCHAR "Process not loaded!",0
sErrorDll TCHAR "%s not found!",0
sDetachReady TCHAR "Process successfully detached!",10,13,"Child Process ID: %.8lX",10,13,"Entry Point: %.8lX",10,13,"Original Bytes: %.4lX",0
sKernel32 TCHAR "kernel32.dll",0
sDebugActiveProcessStop TCHAR "DebugActiveProcessStop",0
sWriteProcessMemory TCHAR "WriteProcessMemory",0
sWaitForDebugEvent TCHAR "WaitForDebugEvent",0
sLoopPatch db 0EBh,0FEh,0
sDebugActiveProcessPatch db 68h,00,00,00,00,0FFh,15h,00,00,00,00,90h,00,00,00,00,0

.data?
hInstance dd ?
hWin dd ?
ActionCheck dd ?
Counter dd ?
pBuffer dd ?
pChildPID dd ?
pOEP dd ?
OriginalBytes dd ?
pKernel32 dd ?
pDebugActiveProcessStop dd ?
pWriteProcessMemory dd ?
pWaitForDebugEvent dd ?
sBuffer CHAR 100h dup (?)