/*
Olly Virtual2Physical plugin
Copyright (C) 2006 Joseph  Moti  <xbxice@yahoo.com>
thankes  to  Mark Russinovich Systems Internals http://www.sysinternals.com

*/
#include  <windows.h>
#include  <stdarg.h>
#include  <shlobj.h>
#include  <stdio.h>
#include  "plugin.h"
#include  <ctype.h>

#include <detours.h>
#include "V2PMemory.h"
#pragma comment(lib,"ollydbg.lib")
#pragma comment(lib,"Ole32.lib")

HANDLE    g_hInst;                    // DLL instance
HWND      g_hMainOllyWnd;             // handle of main OllyDbg window


HANDLE		g_hSection;

DWORD       g_PageDirectory;

char      g_PluginModulePath[MAX_PATH];
void      DbgBufferPrint(unsigned char*,	unsigned int);
VOID      DbgMsg(LPCTSTR,...);
ULONG       WINAPI GetPhysicalThreadProc(LPVOID lpParameter);
ULONG     g_GetPhysicalThreadProcRVA;
#pragma comment(linker, "/base:0x2cd0000") 






VOID DbgMsg(LPCTSTR szError,...)
{
    char                        szBuff[0x100];
    va_list                     vl;
	
    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    OutputDebugString(szBuff);
    va_end(vl);
}

void DbgBufferPrint(unsigned char* pBuffer ,	unsigned int uLength)
{
	
	
	unsigned int  i;
	char _t[1024],*pTmp;
	pTmp=_t;
	for ( i=0; i<uLength; i++){
		
		
		_snprintf(pTmp,1000,"%02X-",(unsigned char)pBuffer[i]);
		while(*pTmp!='-') pTmp++;
		pTmp++;
		if ((i%15)==0 && i ) {
			OutputDebugString(_t);
            pTmp=_t;
			
		}
		
	}
	OutputDebugString(_t);
	
	
	
}

BOOL EnablePriv (LPCTSTR thePriv)
{//ripped off
	BOOL bRet = FALSE;
	
    HANDLE hToken = 0;
    DWORD dwErr = 0;
    TOKEN_PRIVILEGES newPrivs;
	
    if (!OpenProcessToken (GetCurrentProcess (),
		TOKEN_ADJUST_PRIVILEGES,
		&hToken))
    {
        dwErr = GetLastError ();
    }
	else if (!LookupPrivilegeValue (	NULL, thePriv,
		&newPrivs.Privileges[0].Luid))
    {
        dwErr = GetLastError ();
    }
	else
	{
		newPrivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		newPrivs.PrivilegeCount = 1;
		
		if (!AdjustTokenPrivileges (hToken, FALSE, &newPrivs, 0, NULL, NULL))
		{
			dwErr = GetLastError ();
		}
		else bRet = TRUE;
	}
	
    if (hToken)
        CloseHandle (hToken);
	
    return bRet;
}



int CDECL inject_dll(DWORD nProcessId, char *szDllPath)
{
	
	HANDLE hProc;
	
	
	if(!EnablePriv(SE_DEBUG_NAME)){
		DbgMsg(
			"[Virtual2Physical]: EnablePriv(%d) failed: %d\n",
			
			nProcessId, GetLastError());
		return (-1);
	}
	
	
	hProc = OpenProcess(	
		PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION  |
		PROCESS_QUERY_INFORMATION |
		PROCESS_VM_WRITE,
		FALSE,
		nProcessId);
	
	
	if (hProc == NULL){
        DbgMsg(
			
			"[Virtual2Physical]: OpenProcess(%d) failed: %d\n",
			
			nProcessId, GetLastError());
		return(-1);
	}
	
	DbgMsg( "[Virtual2Physical]: Loading %s into %d.Handle %08X\n", szDllPath, nProcessId,hProc);
	
    
	if (!DetourContinueProcessWithDllA(hProc, szDllPath))
	{
		DbgMsg(
			
			"Virtual2Physical:DetourContinueProcessWithDll(%s) failed: %d",
			
			szDllPath, GetLastError());
		
		return(-1);
	}
	
	return(0);
}
BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
	
	
	
	
	
	
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {

		
		if (!g_GetPhysicalThreadProcRVA){
			g_GetPhysicalThreadProcRVA=(DWORD)GetPhysicalThreadProc-(DWORD)hModule;
		}

		
		
		if( !LocateNtdllEntryPoints() ) {
			
			DbgMsg("Unable to locate NTDLL entry points.\n\n");
			return -1;
		}
		
		
		if( !(g_hSection = OpenPhysicalMemory())) {
			DbgMsg("OpenPhysicalMemory failed\n\n");
			return -1;
		}
		
		g_PageDirectory=Scan4CR3_Register(g_hSection);
		
        g_hInst = hModule;
		
		
		
	}
	
	if (ul_reason_for_call==DLL_PROCESS_DETACH){
		
		CloseHandle(g_hSection);
		
	}
	
    return TRUE;
}


int __declspec(dllexport) __cdecl ODBG_Plugindata (char shortname[32])
{
	strcpy(shortname, "Olly Virtual2Physical ");
    return PLUGIN_VERSION;
}


int __declspec(dllexport) __cdecl ODBG_Plugininit (int OllyDbgVer, HWND hWnd, ulong *pFeatures)
{
    if (OllyDbgVer < PLUGIN_VERSION) {
		Addtolist(0, 0,  "OllySnake not compitable with the current olly version ");
		return -1;
		
	}
	
	
	
    g_hMainOllyWnd = hWnd;
	
	
	
    Addtolist(0, 0,  "  Olly Virtual2Physical  Plug-in compiled on " __DATE__);
    Addtolist(0, -1, "  Copyright (C) 2006 Jospeh Moti ");
	
	
	
    return 0;
}


void __declspec(dllexport) __cdecl ODBG_Plugindestroy (void)
{
	
	
	return;
	
}


int __declspec(dllexport) __cdecl ODBG_Pluginmenu (int Orig, char Data[4096], void *pItem)
{
    switch (Orig)
    {
		
		
	case PM_DISASM:
	case  PM_CPUDUMP:
	case PM_CPUSTACK:
		
		strcpy(Data ,"Olly Virtual2Physical {1 GetPhysicalAddress } ");
		
		return 1;
	case PM_MAIN:
		strcpy(Data,"0 About ");
		return 1;
		
	default:
		break;
    }
	
    return 0;
}



DWORD WINAPI GetPhysicalThreadProc(LPVOID lpParameter)
{
	
	
	
	
	char cBuffer[MAX_PATH];
	GetModuleFileName(NULL,cBuffer,MAX_PATH);
	char  x[111];
	wsprintf(x,"Virtual  Address  [0x%08X]\nMapping to \nPhysical Address [0x%08X]",
		lpParameter,
		Virtual2Physical(g_hSection,g_PageDirectory,(DWORD)lpParameter));
	
	MessageBox(GetTopWindow(GetDesktopWindow()),x,"Virtual2Physical",0);
	
	
	return 0;
	
}


DWORD WINAPI InjectDLLThreadProc(LPVOID lpParameter)

{
	DWORD TragetModuleBaseVA=0;
	DWORD ProcessID;
	ProcessID=Plugingetvalue(VAL_PROCESSID);
	GetModuleFileName(GetModuleHandle("Virtual2Physical.dll"),g_PluginModulePath,MAX_PATH);
	
	if (inject_dll(ProcessID,g_PluginModulePath)==-1){
		
		DbgMsg("Inject of %s Failed",g_PluginModulePath);
		return -1;
	}
	
	Sleep(333);
	t_table* table = (t_table*) Plugingetvalue( VAL_MODULES); 
	if( table != 0) 
	{ 
		t_sorted& sorted = table->data; 
		for( int index=0; index<sorted.n; index++) 
		{ 
			t_module* module = (t_module*)( (int)sorted.data+(index*sorted.itemsize)); 
				
			if (strstr(module->name,"Virtual2Physical")) {
				TragetModuleBaseVA=module->base;


			}
		} 

	} 
	if (!TragetModuleBaseVA) { 
		DbgMsg("Searching for module Virtual2Physical  base address   failed");
		return -1;
	}

	
	HANDLE ProcessHandle=(HANDLE)Plugingetvalue(VAL_HPROCESS);
	DbgMsg("Debugged  Process  Handle %08X",ProcessHandle);
	
	DWORD  ThreadID;
	if (!CreateRemoteThread(ProcessHandle,NULL,0,
		(LPTHREAD_START_ROUTINE )(g_GetPhysicalThreadProcRVA+(DWORD)TragetModuleBaseVA),lpParameter,0,&ThreadID)){
		
		
		DbgMsg("CreateRemoteThread Failed Error %d",GetLastError());
		return 1;
		
	}
	
	
	return 0;
}


void __declspec(dllexport) __cdecl ODBG_Pluginaction (int Orig, int Action, void *pItem)
{
	
	
	t_dump                *pX86Dasm=NULL;
	
	DWORD ThreadID;
	
    pX86Dasm=(	t_dump *)pItem;
	
	
	
	
    switch (Orig)
    {
	case PM_MAIN:
		
		
		switch (Action)
		{
			
			
			
		case 0:
			
			MessageBox(g_hMainOllyWnd,
				"Virtual2Physical Plug-in\n\n"
				"Copyright (C) 2006 Jospeh Moti xbxice@yahoo.com",
				"About Virtual2Physical Plug-in",
				MB_OK | MB_ICONINFORMATION);
			
		default:
			break;
		}
		
		break;
		
		case PM_DISASM:
		case  PM_CPUDUMP:
		case PM_CPUSTACK:
			
			
			switch (Action)
			{
				
				
			case 1:
				
				{
					if (Getstatus()!=STAT_RUNNING){
						Flash("Process  must  be in running mode...");
						
						
					}else{
						
						
						
						::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE )InjectDLLThreadProc,
							(void*)pX86Dasm->sel0,0,&ThreadID);
					}
					
				}
				
				
				break;
			case 2:
				
				break;
			case 3:
				
				break;
				
				
			}
			break;
			
			
			
			default:
				break;
    }
}
