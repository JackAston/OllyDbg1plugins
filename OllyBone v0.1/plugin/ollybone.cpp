///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                       OllyBonE Plugin for OllyDbg                         //
//                                                                           //
//                     Copyright (C) 2006 Joe Stewart                        //
//                                                     	                     //
// This plugin allows you to set a breakpoint on execution of a memory block //
// instead of having to break on any access (read/write/execute). This makes //
// things like unpacking certain packed executables simple, just set an      //
// break-on-execution breakpoint point on the original code section and wait //
// for the code to stop at the original entry point (OEP), ready to be       //
// dumped using something like OllyDump.   		                     //
//						                             //
// Since Windows doesn't have software support for a NX page, this is        //
// accomplished through use of a kernel driver which hooks the page fault    //
// handler and allows reads/writes of our target section, but faults on      //
// execution attempts, similar to how Grsecurity's PaX software works.	     //
//						                             //
// Thanks go to Chris Carr for looking at some of the kernel-level bugs      //
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
#include <winioctl.h>
#include "plugin.h"
#include "Instdrv.h"

HINSTANCE        hinst;
HWND             hwmain;
BOOL	driverInitialized = FALSE;
HANDLE gh_Device = INVALID_HANDLE_VALUE;
char ac_driverName[] = "ollybone.sys";
char ac_driverLabel[] = "ollybone";
DWORD nxSections[256];
int lastNxSection = -1;


#define FILE_DEVICE_OLLYBONE      0x0000584e
#define IOCTL_OLLYBONE_SET		(ULONG) CTL_CODE(FILE_DEVICE_OLLYBONE, 0x01, METHOD_BUFFERED, FILE_WRITE_ACCESS)
#define IOCTL_OLLYBONE_RESTORE	(ULONG) CTL_CODE(FILE_DEVICE_OLLYBONE, 0x02, METHOD_BUFFERED, FILE_WRITE_ACCESS)
void SetBreakOnExecute();
void RemoveBreakOnExecute();
DWORD InitDriver();
void Fatal(char *msg);
BOOL nxAllreadySetHere();
void AddSectionToList(DWORD base);
void RemoveSectionFromList(DWORD base);

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;
  return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"OllyBonE");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  hwmain=hw;
  Addtolist(0,0,"OllyBonE plugin v0.1");
  Addtolist(0,-1,"  Copyright (C) 2006 Joe Stewart");
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
	switch (origin) {
    case PM_MAIN:
		strcpy(data, "0 About");
		return 1;
    case PM_MEMORY:
		if (nxAllreadySetHere())
		{
			strcpy(data,"#1 Remove break-on-execute|");
		}
		else 
		{
			strcpy(data,"#0 Set break-on-execute|");
		}
		return 1;
    default: break;
	};
	return 0;
};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin == PM_MAIN) {
    switch (action) {
      case 0:
        MessageBox(hwmain,
          "OllyBonE plugin v0.1\n"
          "Set break-on-execute on virtual memory sections\n"
          "Copyright (C) 2006 Joe Stewart",
          "OllyBonE Plugin",MB_OK|MB_ICONINFORMATION);
        break;
      default: break;
    }; // end of switch
  } 
  if (origin == PM_MEMORY){
    switch (action) {
      case 0:
	SetBreakOnExecute();
	break;
      case 1:
	RemoveBreakOnExecute();
	break;
      default: break;
    };
  }
}

void SetBreakOnExecute()
{
	DWORD success;
	DWORD d_bytesRead;
	int idx;
	char buf[256];
	int err;
	DWORD i;
	DWORD invalid;
	DWORD base = 0;
	DWORD size = 0;
	DWORD pid = 0;
	DWORD itemsize = 0;
	t_table *memTable = NULL;
	void *allrows = NULL;
	t_memory *memRow = NULL;
	t_table *modTable = NULL;
	void *modrows = NULL;
	t_module *module = NULL;
	int *params;
	
	pid = Plugingetvalue(VAL_PROCESSID);		
	if (pid == 0)
		return;

	memTable = (t_table *) Plugingetvalue(VAL_MEMORY);
	if (memTable == NULL)
		return;

	allrows =  memTable->data.data;
	if (allrows == NULL)
		return;
	idx = memTable->data.selected;
	itemsize = memTable->data.itemsize;
	if (idx == -1)
		return;

	memRow = (t_memory *)((char *)allrows + (itemsize * idx));

	base = memRow->base;
	size = memRow->size;

	if (base == 0)
		return;
	if (size == 0)
		return;
	
	modTable = (t_table *) Plugingetvalue(VAL_MODULES);
	if (modTable == NULL)
		return;
	modrows =  modTable->data.data;
	if (modrows == NULL)
		return;

	module = (t_module *) ((char *)modrows);

	if (base + size > module->base + module->size)
	{
		MessageBox(hwmain, 
			"Setting break-on-execute for address greater\n"
			"  than main module range is not supported", 
			"OllyBonE Error",
				MB_OK|MB_ICONERROR);
		return;
	}

	if (!driverInitialized)
	{
		err = InitDriver();
		if (err != ERROR_SUCCESS)
		{
			sprintf(buf, "Kernel driver load returned status %x", err);
        	MessageBox(hwmain, buf, 
				"OllyBonE abort",
				MB_OK|MB_ICONERROR);
			return;
		}
	}
	
	params = (int *)calloc(1, sizeof(int)*3);
	if (!params)
	{
		Fatal("Failed to allocate memory for IOCTL params");
		return;
	}

	params[0] = pid;
	params[1] = base;
	params[2] = size;

	/*sprintf(buf, "Params:\n0:%x\n1:%x\n2:%x\n", params[0], params[1], params[2]);
    MessageBox(hwmain, buf, 
				"OllyBonE",
				MB_OK|MB_ICONINFORMATION);
	*/
	// before IOCTL, read from each page to ensure it is not paged out
	for (i = base; i < base+size; i+=4096)
	{
		Readmemory(&invalid, i, sizeof(DWORD), MM_SILENT);
	}
	success = DeviceIoControl(gh_Device,IOCTL_OLLYBONE_SET,
						params,
						sizeof(int)*3,
						NULL,
						0,
						&d_bytesRead,
						NULL);
				
	if (success)
	{
		AddSectionToList(base);
		Addtolist(base,0,"Sent virtual address to ollybone module for NX set");
	}
	else
	{
		Addtolist(base,0,"IOCTL failed to send virtual address to ollybone module for NX set");
	}

	if (params)
		free(params);
	return;
}

void RemoveBreakOnExecute()
{
	DWORD success;
	char buf[256];
	DWORD d_bytesRead;
	int err;
	int idx;
	DWORD base = 0;
	DWORD size = 0;
	DWORD pid = 0;
	DWORD itemsize = 0;
	t_table *memTable = NULL;
	void *allrows = NULL;
	t_memory *memRow = NULL;
	int *params;

	pid = Plugingetvalue(VAL_PROCESSID);		
	if (pid == 0)
		return;

	memTable = (t_table *) Plugingetvalue(VAL_MEMORY);
	if (memTable == NULL)
		return;
	allrows =  memTable->data.data;
	if (allrows == NULL)
		return;
	idx = memTable->data.selected;
	itemsize = memTable->data.itemsize;
	if (idx == -1)
		return;

	memRow = (t_memory *)((char *)allrows + (itemsize * idx));

	base = memRow->base;
	size = memRow->size;

	if (base == 0)
		return;
	if (size == 0)
		return;

	if (!driverInitialized)
	{
		err = InitDriver();
		if (err != ERROR_SUCCESS)
		{
			sprintf(buf, "Kernel driver load returned status %x", err);
        	MessageBox(hwmain, buf, 
				"OllyBonE driver load failed",
				MB_OK|MB_ICONERROR);
			return;
		}
	}
	params = (int *)calloc(1, sizeof(int)*3);
	if (!params)
	{
		Fatal("Failed to allocate memory for IOCTL params");
		return;
	}

	params[0] = pid;
	params[1] = base;
	params[2] = size;

	success = DeviceIoControl(gh_Device,IOCTL_OLLYBONE_RESTORE,
						params,
						sizeof(int)*3,
						NULL,
						0,
						&d_bytesRead,
						NULL);
						
	if (success)
	{
		RemoveSectionFromList(base);
		Addtolist(base,0,"Sent virtual address to ollybone module for NX remove");
	}
	else
	{
		Addtolist(base,0,"IOCTL failed to send virtual address to ollybone module for NX remove");
	}
	if (params)
		free(params);
	return;
}

extc void _export cdecl ODBG_Pluginreset(void) {
};

extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};

extc void _export cdecl ODBG_Plugindestroy(void) {
};

DWORD InitDriver()
{
	char errbuf[512];
	HANDLE h_Device = INVALID_HANDLE_VALUE;
	DWORD  d_error;
	CHAR ac_driverPath[MAX_PATH];
	
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
	BOOL Found = FALSE;
	
	if (driverInitialized)
	{
		return ERROR_SUCCESS;
	}
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) )
		{
			Fatal("Could not determine Windows version");
			return -1;
		}
	}
	
	switch (osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		if ( osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0)
		{
			//sprintf(errbuf, "Microsoft Windows NT 4.0 ");
			Found = TRUE;
		}
		else if ( osvi.dwMajorVersion == 5 && \
			osvi.dwMinorVersion == 0 )
		{
			//sprintf(errbuf, "Microsoft Windows 2000 ");
			Found = TRUE;
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			//sprintf(errbuf, "Microsoft Windows XP ");
			Found = TRUE;
		}
		else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			//sprintf(errbuf, "Microsoft Windows Server 2003 ");
			Found = TRUE;
		}
		break;
		
	default:
		sprintf(errbuf, "Operating System Version %d.%d Not Supported!", osvi.dwMajorVersion, osvi.dwMinorVersion);
		Fatal(errbuf);
		return -1;
		break;
	}
	if (!Found)
	{
		sprintf(errbuf, "Operating System Version %d.%d Not Supported!",
			osvi.dwMajorVersion,
			osvi.dwMinorVersion);
		Fatal(errbuf);
		return -1;
	}
	else
	{
		if (GetCurrentDirectory(MAX_PATH, ac_driverPath))
		{
			strncat(ac_driverPath, "\\", MAX_PATH-strlen(ac_driverPath));
			strncat(ac_driverPath, ac_driverName, MAX_PATH-strlen(ac_driverPath));
		}
		LoadDeviceDriver(ac_driverLabel, ac_driverPath, &h_Device, &d_error);
		if (h_Device == INVALID_HANDLE_VALUE)
		{
			Sleep(500);
			LoadDeviceDriver(ac_driverLabel, ac_driverPath, &h_Device, &d_error);
			if (h_Device == INVALID_HANDLE_VALUE)
			{
				return d_error;
			}
		}
		
		gh_Device = h_Device;
	}		
	driverInitialized = TRUE;
	Addtolist(0,0,"Loaded OllyBonE kernel driver");
	return ERROR_SUCCESS;
}

void Fatal(char *msg)
{
	if (msg != NULL)
	{
		MessageBox(hwmain, msg, "Fatal",
				MB_OK|MB_ICONERROR);
	}
}

BOOL nxAllreadySetHere()
{
	int i;
	int idx;
	DWORD base;
	DWORD itemsize;
	t_table *memTable = NULL;
	void *allrows = NULL;
	t_memory *memRow = NULL;
	
	memTable = (t_table *) Plugingetvalue(VAL_MEMORY);
	if (memTable == NULL)
		return FALSE;
	allrows =  memTable->data.data;
	if (allrows == NULL)
		return FALSE;
	idx = memTable->data.selected;
	itemsize = memTable->data.itemsize;
	if (idx == -1)
		return FALSE;

	memRow = (t_memory *)((char *)allrows + (itemsize * idx));

	base = memRow->base;

	for (i = 0; i <= lastNxSection; i++)
	{
		if (nxSections[i] == base)
			return TRUE;
	}
	return FALSE;
}

void AddSectionToList(DWORD base)
{
	int i;

	if (lastNxSection >= 255)
		return;

	for (i = 0; i <= lastNxSection; i++)
	{
		if (nxSections[i] == base)
			return;
	}
	nxSections[++lastNxSection] = base;
	return;
}

void RemoveSectionFromList(DWORD base)
{
	int i, j;
	DWORD tmp[256];

	if (lastNxSection <= -1)
		return;
	for (i = 0; i <= lastNxSection; i++)
	{
		if (nxSections[i] == base)
		{
			for (j = 0; j < i; j++)
				tmp[j] = nxSections[j];
			for (j = i+1; j <= lastNxSection; j++)
				tmp[j-1] = nxSections[j];
			lastNxSection--;
			for (j = 0; j <= lastNxSection; j++)
				nxSections[j] = tmp[j];
			return;
		}
	}
}

extc int _export cdecl ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent) 
{
	DWORD base;
	int i;
	
	if ((reason & PP_TERMINATED) == PP_TERMINATED)
	{
		// reset list of hooked sections
		lastNxSection = -1;
	} 
	else if ( ((reason & PP_EVENT) == PP_EVENT) && ((reason & PP_SINGLESTEP) == PP_SINGLESTEP) )
	{
		// display break-on-execute message
		base = reg->ip;
		base &= 0xFFFFF000;
		for (i = 0; i <= lastNxSection; i++)
		{
			if (nxSections[i] == base)
				Infoline("Break-on-execute at [%08x]", reg->ip);
			return 0;
		}
	}
	return 0;
};
