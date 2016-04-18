
#include "stdafx.h"
#include "windows.h"
#define _CHAR_UNSIGNED
#include "plugin.h"

extern "C"
{
	int __stdcall ZwQueryInformationThread(HANDLE,int,unsigned char*,unsigned long,unsigned long*);
	int __stdcall ZwSetInformationThread(HANDLE,int,unsigned long,int);
}
//-----------------------
HWND mainWnd;
HMODULE hinstt;
//---------exports-----------------------

extern "C"
{
_declspec(dllexport)  int __cdecl ODBG_Plugindata(char shortname[32]) 
{
    strcpy(shortname,"HiddenThreads");       // Name of plugin
    return 110;   //version 1.10
}
_declspec(dllexport) int __cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
  mainWnd=hw;
  Addtolist(0,0,"HiddenThreads");
  Addtolist(0,-1,"started successfully");
  return 0;
}
_declspec(dllexport) int __cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
   if(origin==PM_MAIN)
   {
		strcpy(data,"0 About");
		return 1;
   }
   return 0;
}
_declspec(dllexport) void __cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
  if(origin==PM_MAIN) 
  {
	  if(action==0)
	  {
		   MessageBox(mainWnd,"HiddenThreads by waliedassar\r\nwaliedassar@gmail.com","HiddenThreads",0);
	  }
  }
}

__declspec(dllexport) void _cdecl ODBG_Pluginmainloop(DEBUG_EVENT* px)
{
	if(!px) return;	
	if(px->dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT)
	{
		       HANDLE hThread=px->u.CreateProcessInfo.hThread;
		       unsigned char old=0;
			   ZwQueryInformationThread(hThread,0x11,&old,0x1,0);
			   if(old)
			   {
		            MessageBox(mainWnd,"A hidden thread has been detected.","HiddenThreads",0);
			   }
		       return;
	}
	else if(px->dwDebugEventCode==CREATE_THREAD_DEBUG_EVENT)
	{
		       HANDLE hThread=px->u.CreateThread.hThread;
		       unsigned char oldd=0;
			   ZwQueryInformationThread(hThread,0x11,&oldd,0x1,0);
			   if(oldd)
			   {
		            MessageBox(mainWnd,"A hidden thread has been detected.","HiddenThreads",0);
			   }
		       return;
	}
	else return;
	return;
}

__declspec(dllexport) void __cdecl ODBG_Pluginreset(void)
{
}

__declspec(dllexport) int __cdecl ODBG_Pluginclose(void)
{
	return 0;
}

}
//------------------------------
BOOL __stdcall DllMain(HMODULE hModule,int reason,LPVOID lpReserved)
{
	if(reason==DLL_PROCESS_ATTACH) 
	{
		hinstt=hModule;
	}

    return TRUE;
}
