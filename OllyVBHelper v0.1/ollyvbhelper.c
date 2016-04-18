///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                      OllyVBHelper Plugin for OllyDbg                      //
//                                                                           //
//                     Copyright (C) 2005 Joe Stewart                        //
//                                                              	     //
// This plugin finds and labels external DLL function calls in VB programs.  //
// It also finds MSVBVM imports, often labeled by ordinal number and renames //
// them based on export name, for better understandability of the code.      //
// 									     //
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

void LabelDllFunctionCalls(void);

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;
  return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"OllyVBHelper");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  hwmain=hw;
  Addtolist(0,0,"OllyVBHelper plugin v0.1");
  Addtolist(0,-1,"  Copyright (C) 2005 Joe Stewart");
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  switch (origin) {
    case PM_MAIN:
      strcpy(data,"0 &Find and Label DllFunctionCall Stubs and MSVBVM Imports|"
                  "1 &About");
      return 1;
    default: break;
  };
  return 0;
};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin==PM_MAIN) {
    switch (action) {
      case 0:
	LabelDllFunctionCalls();
        break;
      case 1:
        MessageBox(hwmain,
          "OllyVBHelper plugin v0.1\n"
          "Find and label DllFunctionCall stubs and MSVBVM imports\n"
          "Copyright (C) 2005 Joe Stewart",
          "OllyVBHelper Plugin",MB_OK|MB_ICONINFORMATION);
        break;
      default: break;
    };
  };
};


void LabelDllFunctionCalls(void) {
	char buf[16];
	ulong labelptr;
	char label[256];
	ulong addr = 0;
	ulong codebase = 0;
	ulong codesize = 0;
	ulong baseaddr = 0;
	ulong labeled = 0;
	ulong ilabeled = 0;
	t_module *mainmodule = NULL;
	
	baseaddr = Plugingetvalue(VAL_MAINBASE);
	mainmodule = Findmodule(baseaddr);
	codebase = mainmodule->codebase;
	codesize = mainmodule->codesize;

	for(addr = codebase; addr < codebase+codesize; addr++)
	{
		Readmemory(buf,addr, 16, MM_SILENT|MM_RESTORE);

		// find DllFunctionCall code stubs and label
		if ((*buf == 0xa1) && 
		   (*(buf+5) == 0x0b) &&
		   (*(buf+6) == 0xc0) &&
		   (*(buf+7) == 0x74) &&
		   (*(buf+8) == 0x02) &&
		   (*(buf+9) == 0xff) &&
		   (*(buf+10) == 0xe0))
		{
			memset(label,0,256);
			labelptr = 0;
			Readmemory((void *)&labelptr, addr-0x14, 4, MM_SILENT|MM_RESTORE);
			if (labelptr !=0) {
				Readmemory(label, labelptr, 255, MM_SILENT|MM_RESTORE);
			} else {
				sprintf(label, "UnknownStub%x", labeled);
			}
			Insertname(addr, NM_LABEL, label);
			Insertname(addr, NM_COMMENT, label);
			labeled++;
		}
		
		// find MSVBVM import thunks and relabel based on export name
		if ((*buf == 0xff) &&
		    (*(buf+1) == 0x25))
		{
			memset(label,0,256);
			labelptr = 0;
			Readmemory((void *)&labelptr, addr+2, 4, MM_SILENT|MM_RESTORE);
			if ((labelptr > codebase) && (labelptr < codebase+codesize))
			{
				Readmemory((void *)&labelptr, labelptr, 4, MM_SILENT|MM_RESTORE);
				if (Findname(labelptr, NM_EXPORT, label) != 0)
				{
					Insertname(addr, NM_LABEL, label);
					Insertname(addr, NM_COMMENT, label);
					ilabeled++;			
				}
			}
		}
	}		
	Redrawdisassembler();
	Addtolist(0,0,"Labeled %d DllFunctionCall stubs and %d MSVBVM imports", labeled, ilabeled);
	Infoline("Labeled %d DllFunctionCall stubs stubs and %d MSVBVM imports", labeled, ilabeled);
}

extc void _export cdecl ODBG_Pluginreset(void) {
};

extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};

extc void _export cdecl ODBG_Plugindestroy(void) {
};
