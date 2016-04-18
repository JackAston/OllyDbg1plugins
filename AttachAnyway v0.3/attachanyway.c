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

// stuff from winternl.h
DWORD ProcessBasicInformation = 0;

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PVOID PebBaseAddress; // normally it's a PPEB, but we don't care
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

HINSTANCE        hinst;
HWND             hwmain;
int		 found = 0;
char NtContinue_sb[5] = "\xb8\x20\x00\x00\x00"; // reasonable default?
#define TEB	0x7ffde000		// only works for versions < XP SP2

void AttachList(void);
void PEB_Restore_Permissions(void);

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
  Addtolist(0,0,"AttachAnyway plugin v0.3");
  Addtolist(0,-1,"  Copyright (C) 2005 Joe Stewart");
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  switch (origin) {
    case PM_MAIN:
      strcpy(data,"0 Attach to NtContinue-Hooked Process,"
      		  "1 Attach to Process With Altered PEB_LDR_DATA Permissions|"
                  "2 About");
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
        PEB_Restore_Permissions();
	break;
      case 2:
        MessageBox(hwmain,
          "AttachAnyway plugin v0.3\n"
          "Attach to NtContinue-Hooked and\n" 
	  "unreadable PEB_LDR_DATA Processes\n"
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
  HANDLE hProcess;
  HMODULE hModPSAPI;
  HMODULE hModNTDLL;
  PFNENUMPROCESSES EnumProcesses;
  PFNENUMPROCESSMODULES EnumProcessModules;
  PFNGETMODULEFILENAMEEXA GetModuleFileNameExA;

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
	    if (!WriteProcessMemory(hProcess,(void *) NtContinue,NtContinue_sb,5,NULL)) 
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

void PEB_Restore_Permissions(void) {

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
typedef DWORD (WINAPI *PFNNTQUERYINFORMATIONPROCESS)( 
		  HANDLE hProcess,
		  DWORD processInfoClass,
		  PVOID pbi,
		  ULONG pbiSize,
		  PULONG unused 
		  );
  DWORD aProcesses[1024];
  DWORD cbNeeded;
  DWORD cProcesses;
  MEMORY_BASIC_INFORMATION pMem;
  char hookedProc[MAX_PATH*2];
  char szProcessName[MAX_PATH];
  DWORD i;
  DWORD res;
  DWORD oldProt;
  DWORD peb;
  DWORD peb_ldr_data;
  HANDLE hProcess;
  HMODULE hModPSAPI;
  HMODULE hModNTDLL;
  TOKEN_PRIVILEGES tPriv;
  TOKEN_PRIVILEGES tPrivOld;
  HANDLE hToken;
  DWORD cbPriv;
  DWORD red;
  PFNNTQUERYINFORMATIONPROCESS NtQueryInformationProcess;
  PROCESS_BASIC_INFORMATION pbi;
  PFNENUMPROCESSES EnumProcesses;
  PFNENUMPROCESSMODULES EnumProcessModules;
  PFNGETMODULEFILENAMEEXA GetModuleFileNameExA;

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
  if (!hModNTDLL) 
  {
    MessageBox(hwmain, "Couldn't load psapi.dll",
	               "Error Loading DLL",MB_OK|MB_ICONERROR);
    return;
  }

  NtQueryInformationProcess = (PFNNTQUERYINFORMATIONPROCESS)
	  GetProcAddress(hModNTDLL, "NtQueryInformationProcess");
  
  if (!NtQueryInformationProcess) 
  {
    MessageBox(hwmain, "Couldn't find needed API entry in ntdll.dll",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
    return;
  }
  
  if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY |
                        TOKEN_ADJUST_PRIVILEGES, FALSE, &hToken))
  {
    if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY |
                                TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
      hToken=NULL;
    }
  }

  if (hToken)
  {
    tPriv.PrivilegeCount = 1;
    tPriv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tPriv.Privileges[0].Luid);
    if (AdjustTokenPrivileges(hToken, FALSE, &tPriv,
          sizeof(tPriv), &tPrivOld, &cbPriv))
    {
      if(GetLastError()==ERROR_NOT_ALL_ASSIGNED)
      {
        CloseHandle(hToken);
        hToken=NULL;
      }
    }
    else
    {
      CloseHandle(hToken);
      hToken=NULL;
    }
  }

  if (!EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded)) {

    MessageBox(hwmain, "Couldn't enumerate processes!",
	       "Error Loading DLL",MB_OK|MB_ICONERROR);
  }
  cProcesses = cbNeeded / sizeof(DWORD);

  for ( i = 0; i < cProcesses; i++ ) 
  {
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | 
      PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, 
		    aProcesses[i]);   
    if ( hProcess ) 
    {
      if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) 
      {
        strcpy(szProcessName, "<unknown>");
      }
      
      oldProt = 0;
      peb_ldr_data = 0;	
      res = 0;
      Addtolist(0,-1,"---");
      Addtolist(0,0,
        "Checking process %d - %s for altered PEB_LDR_DATA permissions",
        aProcesses[i], szProcessName);
  
      memset(&pMem,0,sizeof(pMem));
      memset(&pbi, 0, sizeof(pbi));
      
      // only works for pre-SP2
      // ReadProcessMemory(hProcess, (void *) (TEB + 0x30), &peb, 4, &read);
    
      // better method to find PEB suggested by anonymouse:
		      
      NtQueryInformationProcess(hProcess, 
		      		ProcessBasicInformation,
		                &pbi, 
				sizeof(pbi), 
				NULL);
      peb = (DWORD) pbi.PebBaseAddress;

      if (peb == 0) 
      {
        Addtolist(0,1,"Failed to locate PEB from NtQueryInformationProcess");
      }
      else 
      {
        SetLastError(ERROR_SUCCESS);
	Addtolist(0,0,"Found PEB at 0x%x", peb);
        red = ReadProcessMemory(hProcess, (void *) (peb + 0x0c), 
      	  &peb_ldr_data, 4, NULL);
        if (red != 0)
        {
          if (peb_ldr_data == 0)
	  {
	    Addtolist(0,0,"peb_ldr_data could not be found at 0x%x",
	     peb + 0x0c);
	  }
          else if (VirtualQueryEx(hProcess, (void *) peb_ldr_data, &pMem, 
		sizeof(pMem)) > 0)
	  {
	    Addtolist(0,0,
	      "Current PEB_LDR_DATA is at 0x%x and has perms: 0x%x", 
	       peb_ldr_data, pMem.Protect);

	    if (pMem.Protect == PAGE_NOACCESS) 
	    {
              res = VirtualProtectEx(hProcess, (void *) peb_ldr_data, 5, 
	  	  PAGE_READWRITE, &oldProt);
              Addtolist(0,0,
	        "VirtualProtectEx returned %d, peb_ldr_data is 0x%x," 
	        "oldProt = 0x%x, last error is %d",
      	        res, peb_ldr_data, oldProt, GetLastError());
            }
          }
	  else
	  {
	    Addtolist(0,1,"VirtualQueryEx failed with error code %d",
	      GetLastError());
	  }
          if ((res != 0) && (oldProt == PAGE_NOACCESS)) {
	    found = 1;
	    // try again to get the process name
            if (!GetModuleFileNameExA(hProcess, NULL, szProcessName, MAX_PATH)) 
	    {
            strcpy(szProcessName, "<unknown>");
            }
            sprintf(hookedProc,
	  		"PEB with altered permissions found in process:\n"
			"PID: %08x (%d)\nName:  %s\n\n"
			"Click OK to attach to this process\n",
			aProcesses[i], aProcesses[i], szProcessName);
            if (MessageBox(hwmain, hookedProc, "Altered PEB Permissions Found",
		      MB_OKCANCEL|MB_ICONINFORMATION) != IDCANCEL) {
      	        CloseHandle( hProcess );
	        Attachtoactiveprocess(aProcesses[i]);
	        return;
	    }
	  } // end of if res !=0 &&
        } // end of if ReadProcessMemory
        else {
         Addtolist(0,1,"ReadProcessMemory failed with error code 0x%x (0x%x)",
       	  red,GetLastError());
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
    MessageBox(hwmain, "Done reading processes, no altered PEB permissions found", 
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

