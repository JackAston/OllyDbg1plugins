///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                     AttachAnyway Plugin for OllyDbg                       //
//                                                                           //
//                     Copyright (C) 2005 Joe Stewart                        //
//                                                              	     //
// This plugin allows you to attach to a process in OllyDbg even when        //
// it is attempting to prevent the attach by hooking NtContinue as           //
// demonstrated by Piotr Bania.						     //
// 									     //
// See http://pb.specialised.info/all/anti-dattach.asm for more information  //
//                                                                           //
// This code is distributed "as is", without warranty of any kind, expressed //
// or implied, including, but not limited to warranty of fitness for any     //
// particular purpose. In no event will Joe Stewart be liable to you for any //
// special, incidental, indirect, consequential or any other damages caused  //
// by the use, misuse, or the inability to use of this code, including any   //
// lost profits or lost savings, even if Joe Stewart has been advised of the //
// possibility of such damages. Or, translated into English: use at your own //
// risk!                                                                     //
//                                                                           //
// This code is free. You can modify this code, include parts of it in your  //
// own programs and redistribute modified code provided that you remove all  //
// copyright messages or, if changes are significant enough, substitute them //
// with your own copyright.                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "plugin.h"

HINSTANCE        hinst;
HWND             hwmain;
int		 found = 0;
char NtContinue_sb[5] = "\xb8\x20\x00\x00\x00"; // reasonable default?

void AttachList(void);

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;
  return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"AttachAnyway");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  hwmain=hw;
  Addtolist(0,0,"AttachAnyway plugin v0.1");
  Addtolist(0,-1,"  Copyright (C) 2005 Joe Stewart");
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  switch (origin) {
    case PM_MAIN:
      strcpy(data,"0 Attach to NtContinue-Hooked Process|"
                  "1 About");
      return 1;
    default: break;
  };
  return 0;
};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin == PM_MAIN) {
    switch (action) {
      case 0:
	AttachList();
        break;
      case 1:
        MessageBox(hwmain,
          "AttachAnyway plugin v0.1\n"
          "Attach to NtContinue-Hooked Processes\n"
          "Copyright (C) 2005 Joe Stewart",
          "AttachAnyway Plugin",MB_OK|MB_ICONINFORMATION);
        break;
      default: break;
    }; // end of switch
  } 
}

void AttachList(void) {

  typedef BOOL (WINAPI * PFNENUMPROCESSES)(
          DWORD * lpidProcess,
          DWORD   cb,
          DWORD * cbNeeded
          );

  typedef BOOL (WINAPI * PFNENUMPROCESSMODULES)(
          HANDLE hProcess,
          HMODULE *lphModule,
          DWORD cb,
          LPDWORD lpcbNeeded
          );

  typedef DWORD (WINAPI * PFNGETMODULEFILENAMEEXA)(
          HANDLE hProcess,
          HMODULE hModule,
          LPSTR lpFilename,
          DWORD nSize
          );

  DWORD aProcesses[1024];
  DWORD cbNeeded;
  DWORD cProcesses;
  char hookedProc[MAX_PATH*2];
  char szProcessName[MAX_PATH];
  LPCVOID NtContinue;
  DWORD i;
  char NtContinue_b[5];
  HANDLE hProcess = 0;
  HMODULE hModPSAPI = 0;
  HMODULE hModNTDLL = 0;
  PFNENUMPROCESSES EnumProcesses = 0;
  PFNENUMPROCESSMODULES EnumProcessModules = 0;
  PFNGETMODULEFILENAMEEXA GetModuleFileNameExA = 0;

  hModPSAPI = LoadLibrary("psapi.dll");
  if (!hModPSAPI) { 
    MessageBox(hwmain, "Couldn't load psapi.dll",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
    return;
  }
	  
  EnumProcesses = (PFNENUMPROCESSES) 
	  GetProcAddress(hModPSAPI,"EnumProcesses");
  EnumProcessModules = (PFNENUMPROCESSMODULES) 
	  GetProcAddress(hModPSAPI, "EnumProcessModules");
  GetModuleFileNameExA = (PFNGETMODULEFILENAMEEXA) 
	  GetProcAddress(hModPSAPI, "GetModuleFileNameExA");

  if (!EnumProcesses || !EnumProcessModules || !GetModuleFileNameExA) {
    MessageBox(hwmain, "Couldn't find needed API entries in psapi.dll",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
    Addtolist(0,0,"EnumProcesses = %x",EnumProcesses);
    Addtolist(0,0,"EnumProcessModules = %x",EnumProcessModules);
    Addtolist(0,0,"GetModuleFileNameExA = %x",GetModuleFileNameExA);
    return;
  }

  hModNTDLL = GetModuleHandle("ntdll.dll");
  if (!hModNTDLL) {
    MessageBox(hwmain, "Couldn't get handle for ntdll.dll",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
    return;
  }
    
  NtContinue = (LPCVOID) GetProcAddress (hModNTDLL, "NtContinue");

  if (!NtContinue) {
    MessageBox(hwmain, "Couldn't locate NtContinue in ntdll.dll",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
    return;
  }

  if (!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded)) {

    MessageBox(hwmain, "Couldn't enumerate processes!",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
  }
  cProcesses = cbNeeded / sizeof(DWORD);

  for ( i = 0; i < cProcesses; i++ ) {
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, 
		    aProcesses[i]);   
    if ( hProcess ) {
      if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) {
        strcpy(szProcessName, "<unknown>");
      }
      memset(NtContinue_b,0,5);
      ReadProcessMemory(hProcess, NtContinue, NtContinue_b, 5, NULL);

      if (NtContinue_b[0] == 0xb8) {
        memcpy(NtContinue_sb, NtContinue_b, 5);
      } 

      if ((NtContinue_b[0] != 0xb8) && (NtContinue_b[0] != 0)) {
	found = 1;
        sprintf(hookedProc,
			"Hooked process found:\nPID: %08x (%d)\nName:  %s\n\n"
			"Click OK to attach to this process\n",
			aProcesses[i], aProcesses[i], szProcessName);
        if (MessageBox(hwmain, hookedProc, "Hooked NtContinue Found",
		      MB_OKCANCEL|MB_ICONINFORMATION) != IDCANCEL) {
	  CloseHandle ( hProcess );
	  hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE
		,FALSE,aProcesses[i]);
	  if (hProcess) {
	    if (!WriteProcessMemory(hProcess,NtContinue,NtContinue_sb,5,NULL)) 
	    {
              MessageBox(hwmain, "Couldn't write to process virtual memory",
			         "Hook Removal Failed", MB_OK|MB_ICONERROR);
	    }
	    else {
      	      CloseHandle( hProcess );
	      Attachtoactiveprocess(aProcesses[i]);
	      return;
	    }
	  } else {
            MessageBox(hwmain, "Couldn't open process virtual memory for write",		  "Hook Removal Failed", MB_OK|MB_ICONERROR);
	  }
	}
      }
      if (hProcess)
      	CloseHandle( hProcess );
    }  // end if (hProcess)
  } // end process read loop
  if (found) {
    MessageBox(hwmain, "Done reading processes", 
		    "Done", MB_OK|MB_ICONINFORMATION);
    found = 0;
  } else {
    MessageBox(hwmain, "Done reading processes, no hooked NtContinue found", 
		    "Done", MB_OK|MB_ICONINFORMATION);
  }
}

extc void _export cdecl ODBG_Pluginreset(void) {
};

extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};

extc void _export cdecl ODBG_Plugindestroy(void) {
};

