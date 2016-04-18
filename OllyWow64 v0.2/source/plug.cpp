// plug.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#define _CHAR_UNSIGNED
#include "windows.h"
#include "plugin.h"
#include "pe.h"
#include "str.h"
//---------------------------------------------------------------------------------
HWND mainWnd;





//---------exports-------------------------


extern "C"
{
_declspec(dllexport)  int __cdecl ODBG_Plugindata(char shortname[32]) 
{
    strcpy(shortname,"OllyWow64");       // Name of plugin
    return 110;   //version 1.10
}
_declspec(dllexport) int __cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
	//real fix goes here
	unsigned long old;
	unsigned long addr=0x4311C2;
	VirtualProtect((void*)0x4311C2,0x1,PAGE_EXECUTE_READWRITE,&old);
			  *(unsigned char*)(addr)=0xEB;
	VirtualProtect((void*)0x4311C2,0x1,old,&old);
	//-----------------------------------------------------
  mainWnd=hw;
  Addtolist(0,0,"OllyWow64");
  Addtolist(0,-1,"started successfully");
  return 0;
}
_declspec(dllexport) int __cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
   if(origin==PM_MAIN)
   {
		strcpy(data,"0 &OllyWow64");
		return 1;
   }
   return 0;
}


_declspec(dllexport) void __cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
  //unsigned long value=0;
  if(origin==PM_MAIN) 
  {
	  if(action==0)
	  {
		  MessageBox(mainWnd,"OllyWow64 by waliedassar\r\nwaliedassar@gmail.com","OllyWow64",0);
	  }
  }
}

}
//------------------------------
BOOL APIENTRY Dllmain(HMODULE hModule,int reason,LPVOID lpReserved)
{
    return TRUE;
}

