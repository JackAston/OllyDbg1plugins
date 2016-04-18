/*
    Olly PUSH 0x86 Tracer
    Copyright (C) 2006 Joseph  Moti  <xbxice@yahoo.com>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include  <windows.h>
#include  <stdarg.h>
#include  <shlobj.h>
#include  <stdio.h>
#include  "plugin.h"
#include  <ctype.h>

#include <detours.h>
#pragma comment(lib,"ollydbg.lib")
#pragma comment(lib,"Ole32.lib")

HANDLE    g_hInst;                    // DLL instance
HWND      g_hMainOllyWnd;             // handle of main OllyDbg window

char      g_PluginModulePath[MAX_PATH];
void      DbgBufferPrint(unsigned char*,	unsigned int);
VOID      DbgMsg(LPCTSTR,...);

#define   MAXNUMOF_0x68PUSH 1024*10
BOOL      g_bIsTracerStart=FALSE;

#pragma comment(linker, "/base:0x2cd0000") 

#pragma pack(1)

struct PUSH0x68Inst 
{
	
	unsigned long   EIP;
	unsigned long   Address;
	
};
#pragma pack()
	
DWORD  PUSH0x68_Trampoline_FuncPtrRVA=0;
DWORD  PUSH0x68_Trampoline_FuncPtr=0;

#pragma data_seg("0x68PUSH")  
BOOL               g_bWait=TRUE;
ULONG              g_NumOfPush0x86Found=0;
PUSH0x68Inst       gs_Push0x86Data[MAXNUMOF_0x68PUSH]={0};
#pragma data_seg()          
                            
#pragma comment(linker, "/section:0x68PUSH,RWS")




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



BOOL IsString(char *pBuffer,int Size)
{


	for(int  i=0; i<Size;i++){
		if (!isprint (pBuffer[i])) 
			return FALSE; 


	}
	
	return TRUE;

}


VOID
__stdcall DumpBuffer(DWORD dwEIP,LPSTR pBuffer)
{


	if (IsBadReadPtr(pBuffer,50))  return;
//	DbgMsg("PUSH AT %08X BUFFER %08X",dwEIP,pBuffer);

	static char cDbg[0x1024];
	
	if (pBuffer){
		if (IsString(pBuffer,3)){
			wsprintf(cDbg,"PUSH     [%50s]        AT      [0x%08X]",pBuffer,dwEIP);
			OutputDebugString(cDbg);
		}

	}

}
static DWORD  __stdcall GetOrginalPushInstrAddress(DWORD dwEIP)
{
	
	
	for (unsigned long i=0; i<g_NumOfPush0x86Found; i++){
		if(gs_Push0x86Data[i].EIP==(dwEIP-5))
			return gs_Push0x86Data[i].Address;
		
		
	}
	DbgMsg("CIRTICAL ERROR");
	
	
	return -1;
	
	
	
}
extern "C" __declspec(dllexport) void __declspec(naked) PUSH0x68_Trampoline()
{
	


	while(g_bWait);  ///hmm  you wonder why?
	__asm{
      
     //   _emit 0xCC
		sub esp,8
		pushad      //sub esp,0x20 
		pushfd      //sub esp,0x04


		lea esi,[esp+0x2C]
        mov eax ,dword ptr[esp+0x2C]
		mov dword ptr[esp+0x24],eax //ret from PUSH0x68_Trampoline
		push eax
		call GetOrginalPushInstrAddress 
		mov dword ptr[esp+0x28],eax //push [Address]
		mov [esi],eax
	
		push eax
		push dword ptr[esp+0x28]
		call DumpBuffer
		
		popfd
		popad
		add esp,0x08  //ALL CLEAN

		jmp dword ptr[esp-0x8]
		
	}
	
}

int CDECL inject_dll(DWORD nProcessId, char *szDllPath)
{
     
     HANDLE hProc;
     

	    if(!EnablePriv(SE_DEBUG_NAME)){
			DbgMsg(
				"[_PushTracerLoader]: EnablePriv(%d) failed: %d\n",
				
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

               "[_PushTracerLoader]: OpenProcess(%d) failed: %d\n",
               
          nProcessId, GetLastError());
          return(-1);
     }

     DbgMsg( "[_PushTracerLoader]: Loading %s into %d.Handle %08X\n", szDllPath, nProcessId,hProc);
     
    
     if (!DetourContinueProcessWithDllA(hProc, szDllPath))
     {
          DbgMsg(
          
               "_PushTracerLoader:DetourContinueProcessWithDll(%s) failed: %d",
          
          szDllPath, GetLastError());
          
          return(-1);
     }
	
     return(0);
}




BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{


	if (PUSH0x68_Trampoline_FuncPtrRVA==0){
		PUSH0x68_Trampoline_FuncPtrRVA=(DWORD)PUSH0x68_Trampoline-(DWORD)hModule;
		PUSH0x68_Trampoline_FuncPtr=0x2cd0000+PUSH0x68_Trampoline_FuncPtrRVA;
		DbgMsg("PUSH0x68_Trampoline_FuncPtr==%08X",PUSH0x68_Trampoline_FuncPtr);
		
	}
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		
        g_hInst = hModule;
	
		
		
	}
	
    return TRUE;
}


int __declspec(dllexport) __cdecl ODBG_Plugindata (char shortname[32])
{
     strcpy(shortname, "Olly Push 0x68[ADDRESS] Tracer");
    return PLUGIN_VERSION;
}


int __declspec(dllexport) __cdecl ODBG_Plugininit (int OllyDbgVer, HWND hWnd, ulong *pFeatures)
{
    if (OllyDbgVer < PLUGIN_VERSION) {
		Addtolist(0, 0,  "OllySnake not compitable with the current olly version ");
		return -1;
		
	}


   
    g_hMainOllyWnd = hWnd;



    Addtolist(0, 0,  "  OllyTrcaer  Plug-in compiled on " __DATE__);
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
		strcpy(Data ,"Olly PUSH0x68[ADDRESS] Tracer {1 Start Tracer } ");
		
		return 1;
	case PM_MAIN:
		strcpy(Data,"0 About ");
		return 1;
		
	default:
		break;
    }
	
    return 0;
}


BOOL  DoCallPatch(t_dump*  pX86Dasm)
{

	t_module               *pModule=NULL;
	ulong                  Address,SOffest,EOffset,i,InstLength;
	t_disasm               da;
    unsigned char          *pdecode=NULL; 
	unsigned char          InstStr[MAXCMDSIZE];
    if (!pX86Dasm)         return FALSE;
	unsigned long          TotalPush;
    unsigned long lDisplacement;
	pModule=Findmodule(pX86Dasm->addr);
	if (Analysecode(pModule)==-1){
		
		DbgMsg("OllyDbg Analysecode Failed");
		return FALSE;
	}
	Address=pX86Dasm->base;
	TotalPush=0;
	g_NumOfPush0x86Found=0;
	for(i=0; i<pModule->nsect; i++){
		
		
			DbgMsg("[%s] [%08X]",(char*)(pModule->sect[i].Name),pModule->sect[i].Characteristics);
		
	}
	while(Address=Findnextproc(Address)){
		
	
		Getproclimits(Address,&SOffest,&EOffset);
		
		for (i=SOffest; i<EOffset; ){
			
			if (!Readcommand(i,(char*)InstStr)) break;
			
			
		
			InstLength=Disasm(InstStr,MAXCMDSIZE,i,pdecode,&da,DISASM_CODE,0); 
			

			if (InstStr[0]==0x68 && InstLength==5 ){
				
				
				
				char cSymbolic[0x100];
				char cComment[0x100];
				DWORD  pAddr=*(DWORD*)&InstStr[1];
				if (Decodeaddress(pAddr,0,ADC_VALID,cSymbolic,sizeof(cSymbolic),cComment)){
					
					if (pAddr>pModule->base &&pAddr<(pModule->base+pModule->size)){
						gs_Push0x86Data[g_NumOfPush0x86Found].Address=pAddr;
						gs_Push0x86Data[g_NumOfPush0x86Found].EIP=i;
						lDisplacement = ((DWORD)PUSH0x68_Trampoline_FuncPtr-(gs_Push0x86Data[g_NumOfPush0x86Found].EIP+5));
						unsigned char cJmpCode[5];
						cJmpCode[0]=0xE8;
						memcpy(&cJmpCode[1],&lDisplacement,sizeof(long));
						
						
						DbgMsg("Patching %08X DATA=0x%08X ",gs_Push0x86Data[g_NumOfPush0x86Found].EIP,
							pAddr);
						Flash("Patching %08X DATA=0x%08X ",gs_Push0x86Data[g_NumOfPush0x86Found].EIP,pAddr);
						if (!Writememory(cJmpCode,gs_Push0x86Data[g_NumOfPush0x86Found].EIP,5,MM_RESTORE)){
							
							
							DbgMsg("Patch Failed");
							//???hmmmm
						}
						TotalPush++;
						if (TotalPush>=MAXNUMOF_0x68PUSH){ 
							
							
							DbgMsg("Total PUSH ==%d",MAXNUMOF_0x68PUSH);
							g_bWait=FALSE;
							return TRUE;
							
						}
						g_NumOfPush0x86Found++;
					}
				}
			}
			
			i+=InstLength;
			
		}
		
		

		
	}
	DbgMsg("Total PUSH ==%d",TotalPush);
	g_bWait=FALSE;
	return TRUE;
}

DWORD WINAPI InjectDLLThreadProc(LPVOID lpParameter)
								 
{
	t_dump                *pX86Dasm=NULL;
	pX86Dasm=(	t_dump *)lpParameter;
	DWORD ProcessID;
	ProcessID=Plugingetvalue(VAL_PROCESSID);
	DbgMsg("Process ID=%08X",ProcessID);
	GetModuleFileName(GetModuleHandle("Push0x86Trace.dll"),g_PluginModulePath,MAX_PATH);
	
	if (inject_dll(ProcessID,g_PluginModulePath)==-1){
		
		DbgMsg("Inject of %s Failed",g_PluginModulePath);
		return -1;
	}
	
    g_bIsTracerStart=TRUE;
	//Suspendprocess(0);  //?
	DoCallPatch(pX86Dasm);
	g_bIsTracerStart=FALSE;  
	//Go(0,0,STEP_RUN,0,0);//?
	return 0;
}


void __declspec(dllexport) __cdecl ODBG_Pluginaction (int Orig, int Action, void *pItem)
{
	
	
	t_dump                *pX86Dasm=NULL;
	t_module              *pModule=NULL;
	
    pX86Dasm=(	t_dump *)pItem;
	
    switch (Orig)
    {
	case PM_MAIN:
		
		
		switch (Action)
		{
			
			
			
		case 0:

			MessageBox(g_hMainOllyWnd,
				"Olly Push 0x86 Plug-in\n\n"
				"Copyright (C) 2006 Jospeh Moti  xbxice@yahoo.com",
				"About Olly PUSH[0x86]  Tracer Plug-in",
				MB_OK | MB_ICONINFORMATION);
			
		default:
			break;
		}
		
		break;
		
		case PM_DISASM:
			
			
			switch (Action)
			{
				
				
			case 1:


				if (g_bIsTracerStart==TRUE) {

					Flash("Tracer in progress please wait....");
					Addtolist(0, -1, "Tracer in progress please wait....");

				}else
				if (Getstatus()==STAT_RUNNING ){
					
					
					DWORD ThreadID;
					::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE )InjectDLLThreadProc,(void*)pX86Dasm,0,&ThreadID);
				}else{
					
					::MessageBox(g_hMainOllyWnd,
						
						"Debugged Process must be in running mode",
						"?",
						MB_OK | MB_ICONINFORMATION);   //you propbly ask why  not to ask the user if he want to start it ;)
					
					
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
