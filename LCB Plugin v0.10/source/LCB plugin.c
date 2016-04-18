/****************************************************************************** 
/* Module: LCB plugin
/* Description: Ollydbg plugin for export and import labels, comments and
/*              breakpoints.             
/*
/*              This code is distributed "as is", without warranty of any 
/*              kind, expressed or implied, including, but not limited to
/*              warranty of fitness for any particular purpose. In no event
/*              will scherzo be liable to you for any special, incidental, 
/*              indirect, consequential or any other damages caused by the
/*              use, misuse, or the inability to use of this code, including 
/*              any lost profits or lost savings, even if scherzo has been 
/*              advised of the possibility of such damages. Or, translated 
/*              into English: use at your own risk!                                                                     
/*                                                                           
/*              This code is free. You can modify this code, include parts
/*              of it in your own programs and redistribute modified code 
/*              provided that you remove all copyright messages or, if 
/*              changes are significant enough, substitute them with your own
/*              copyright.                                                  
/*                                                                           
/*              VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT
/*              OF STRUCTURES AND UNSIGNED CHAR!
/*
/* Authors: scherzo  
/* (c) 2006
/*****************************************************************************/ 


/****************************************************************************** 
/*                   Libraries used by this module
/*****************************************************************************/ 

#include "stdafx.h"
#include "plugin.h"


/****************************************************************************** 
/*                          Function prototypes
/*****************************************************************************/ 

void ImportLabel(void);
void ImportComment(void);
void ImportBreakpoint(void);
void ExportLabel(void);
void ExportComment(void);
void ExportBreakpoint(void);
void ErrorHandler(DWORD);


/****************************************************************************** 
/*                          Global variables
/*****************************************************************************/ 

HINSTANCE hinst; // DLL instance
HWND hwmain; // Handle of main OllyDbg window


/*****************************************************************************
* DllEntryPoint
*
*  Entry point into a plugin DLL.
*
* Inputs
*  Standard DllEntryPoint parameters
* 
* Outputs
*  None
*
* Returns
*  Exit Code
******************************************************************************/

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
	if (reason==DLL_PROCESS_ATTACH)
		hinst=hi; // Mark plugin instance
	return 1; // Report success
};


/*****************************************************************************
* ODBG_Plugindata
*
*  ODBG_Plugindata() is a "must" for valid OllyDbg plugin. It must fill in
*  plugin name and return version of plugin interface.
*
* Inputs
*  char shortname[32]: Name of plugin
* 
* Outputs
*  None
*
* Returns
*  Version of plugin interface
******************************************************************************/

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
	strcpy(shortname,"LCB plugin"); // Name of plugin
	return PLUGIN_VERSION;
};


/*****************************************************************************
* ODBG_Plugininit
*
*  OllyDbg calls this obligatory function once during startup. Place all
*  one-time initializations here. If all resources are successfully allocated,
*  function must return 0. On error, it must free partially allocated resources
// and return -1, in this case plugin will be removed.
*
* Inputs
*  int ollydbgversion: ollydbg version
*  HWND hw: Handle of main OllyDbg window
*  ulong *features: reserved for future extentions
* 
* Outputs
*  None
*
* Returns
*  return 0  - successfully execution
*  return -1 - not successfully execution
******************************************************************************/

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {

	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;
	Addtolist(0,0,"LCB - Labels, comments and breakpoints plugin v0.10");
	Addtolist(0,-1,"  Copyright (C) 2006 scherzo");
	return 0;
};


/*****************************************************************************
* ODBG_Pluginmenu
*
*  add menu items either to main OllyDbg menu (origin=PM_MAIN) or to popup 
*  menu in one of standard OllyDbg windows.
*
* Inputs
*  int origin: code of window that calls ODBG_Pluginmenu. 
*  char data[4096]: pointer to buffer 4K bytes long that receives description
*                   of menu structure.
*  void *item: pointer either to selected element of sorted data displayed in 
*              window or, in case of dump windows, pointer to dump descriptor.
* 
* Outputs
*  None
*
* Returns
*  return 1 - prepare to add menu
*  return 0 - nothing was done
******************************************************************************/

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
	switch (origin) {
		case PM_MAIN:
			strcpy(data,"0 &Import Labels from file,"
				"1 &Import Comments from file,"
				"2 &Import Breakpoints from file|"
				"3 &Export Labels to file,"
				"4 &Export Comments to file,"
				"5 &Export Breakpoints to file|"
				"6 &About");
			return 1;
		default:
			break;
	};
	return 0;
};


/*****************************************************************************
* ODBG_Pluginaction
*
*  OllyDbg calls it each time the user selected menu item added to menu by 
*  ODBG_Pluginmenu. 
*
* Inputs
*  int origin: code of window that calls ODBG_Pluginmenu.
*  int action: identifier of menu item (0..63), as set by ODBG_Pluginmenu.
*  void *item: pointer either to selected element of sorted data displayed in 
*              window or, in case of dump windows, pointer to dump descriptor.
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
	if (origin==PM_MAIN) {
		switch (action) {
			case 0:
				ImportLabel();
				break;
			case 1:
				ImportComment();
				break;
			case 2:
				ImportBreakpoint();
				break;
			case 3:
				ExportLabel();
				break;
			case 4:
				ExportComment();
				break;
			case 5:
				ExportBreakpoint();
				break;
			case 6:
				MessageBox(hwmain,
					"LCB plugin v1.0\n"
					"Import and Export Labels, Comments and Breakpoints\n"
					"Copyright (C) 2006 scherzo",
					"LCB Plugin",MB_OK|MB_ICONINFORMATION);
				break;
			default:
				break;
		};
	};
};


/*****************************************************************************
* ImportLabel
*
*  Import labels from file to module
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ImportLabel(void) {
	char filename[2048] = {'\0'}, result[1024] = {'\0'}, name[1024] = {'\0'}, line[1024] = {'\0'};
	char modulename[9] = {'\0'}, modulename_temp[9] = {'\0'};
	char *filecontent;
	int count = 0, k, j, i, filesize;
	ulong addr;
	struct t_module *moduleinfo;
	struct t_table *modules;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to import labels",filename,".txt",0)) 
		return;
    h_file = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h_file == INVALID_HANDLE_VALUE) {
        ErrorHandler(GetLastError());
        return;
    }
	modules = (t_table *)Plugingetvalue(VAL_MODULES);
	if(modules==NULL || modules->data.n==0) {
		MessageBox(hwmain,"Error retrieving module information. Please, send an e-mail to scherzocrk@gmail.com reporting the problem.","Error",MB_OK | MB_ICONERROR);
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
	}
	filesize = GetFileSize(h_file,NULL);
	filecontent = (char *)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	if(filecontent == NULL) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
		return;
	}
	if(!ReadFile(h_file,filecontent,filesize,&lpNumberOfBytesWritten,NULL) || lpNumberOfBytesWritten!=filesize) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file))
			ErrorHandler(GetLastError());
		if(!VirtualFree(filecontent,0,MEM_RELEASE))
			ErrorHandler(GetLastError());
		return;
	}
	for(j=0; j < filesize; j++) {
		for(i=0;filecontent[j] != '\n';j++,i++)
			line[i] = filecontent[j];
		sscanf(line,"%[\x20-\x7e] %x %[\x20-\x7e]",modulename,&addr,name);
		for(i=0; i < modules->data.n; i++) {
			moduleinfo = (t_module *)((char *)modules->data.data + modules->data.itemsize*i);
			for(k=0;k<8;k++)
				modulename_temp[k] = moduleinfo->name[k];
			if(!strcmp(modulename_temp,modulename)) {
				addr += moduleinfo->base;
				break;
			}
		}
		ZeroMemory(line,1024);
		if(Quickinsertname(addr,NM_LABEL,name) != -1)
			count++;
	}
	if(!CloseHandle(h_file)) {
        ErrorHandler(GetLastError());
        return;
    }
	Mergequicknames();
	Redrawdisassembler();
	wsprintf(result, "Copied %d user-defined labels from file\n", count);
	MessageBox(hwmain, result,"LCB Import Result",MB_OK|MB_ICONINFORMATION);
	if(!VirtualFree(filecontent,0,MEM_RELEASE))
		ErrorHandler(GetLastError());
	return;
}


/*****************************************************************************
* ImportComment
*
*  Import comments from file to module
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ImportComment(void) {
	char filename[2048] = {'\0'}, result[1024] = {'\0'}, name[1024] = {'\0'}, line[1024] = {'\0'};
	char modulename[9] = {'\0'}, modulename_temp[9] = {'\0'};
	char *filecontent;
	int count = 0, k, j, i, filesize;
	ulong addr;
	struct t_module *moduleinfo;
	struct t_table *modules;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to import comments",filename,".txt",0)) 
		return;
    h_file = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h_file == INVALID_HANDLE_VALUE) {
        ErrorHandler(GetLastError());
        return;
    }
	modules = (t_table *)Plugingetvalue(VAL_MODULES);
	if(modules==NULL || modules->data.n==0) {
		MessageBox(hwmain,"Error retrieving module information. Please, send an e-mail to scherzocrk@gmail.com reporting the problem.","Error",MB_OK | MB_ICONERROR);
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
	}
	filesize = GetFileSize(h_file,NULL);
	filecontent = (char *)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	if(filecontent == NULL) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
		return;
	}
	if(!ReadFile(h_file,filecontent,filesize,&lpNumberOfBytesWritten,NULL) || lpNumberOfBytesWritten!=filesize) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file))
			ErrorHandler(GetLastError());
		if(!VirtualFree(filecontent,0,MEM_RELEASE))
			ErrorHandler(GetLastError());
		return;
	}
	for(j=0; j < filesize; j++) {
		for(i=0;filecontent[j] != '\n';j++,i++)
			line[i] = filecontent[j];
		sscanf(line,"%[\x20-\x7e] %x %[\x20-\x7e]",modulename,&addr,name);
		for(i=0; i < modules->data.n; i++) {
			moduleinfo = (t_module *)((char *)modules->data.data + modules->data.itemsize*i);
			for(k=0;k<8;k++)
				modulename_temp[k] = moduleinfo->name[k];
			if(!strcmp(modulename_temp,modulename)) {
				addr += moduleinfo->base;
				break;
			}
		}
		ZeroMemory(line,1024);
		if(Quickinsertname(addr,NM_COMMENT,name) != -1)
			count++;
	}
	if(!CloseHandle(h_file)) {
        ErrorHandler(GetLastError());
        return;
    }
	Mergequicknames();
	Redrawdisassembler();
	wsprintf(result, "Copied %d user-defined comments from file\n", count);
	MessageBox(hwmain, result,"LCB Import Result",MB_OK|MB_ICONINFORMATION);
	if(!VirtualFree(filecontent,0,MEM_RELEASE))
		ErrorHandler(GetLastError());
	return;
}


/*****************************************************************************
* ImportBreakpoint
*
*  Import breakpoints from module to file
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ImportBreakpoint(void) {
	char filename[2048] = {'\0'}, result[1024] = {'\0'}, name[1024] = {'\0'}, line[1024] = {'\0'};
	char modulename[9] = {'\0'}, modulename_temp[9] = {'\0'};
	char *filecontent;
	int count = 0, k, j, i, filesize;
	ulong addr, type, passcount;
	struct t_module *moduleinfo;
	struct t_table *modules;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to import breakpoints",filename,".txt",0)) 
		return;
    h_file = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if(h_file == INVALID_HANDLE_VALUE) {
        ErrorHandler(GetLastError());
        return;
    }
	modules = (t_table *)Plugingetvalue(VAL_MODULES);
	if(modules==NULL || modules->data.n==0) {
		MessageBox(hwmain,"Error retrieving module information. Please, send an e-mail to scherzocrk@gmail.com reporting the problem.","Error",MB_OK | MB_ICONERROR);
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
	}
	filesize = GetFileSize(h_file,NULL);
	filecontent = (char *)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	if(filecontent == NULL) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file)) {
			ErrorHandler(GetLastError());
			return;
		}
		return;
	}
	if(!ReadFile(h_file,filecontent,filesize,&lpNumberOfBytesWritten,NULL) || lpNumberOfBytesWritten!=filesize) {
		ErrorHandler(GetLastError());
		if(!CloseHandle(h_file))
			ErrorHandler(GetLastError());
		if(!VirtualFree(filecontent,0,MEM_RELEASE))
			ErrorHandler(GetLastError());
		return;
	}
	for(j=0; j < filesize; j++) {
		for(i=0;filecontent[j] != '\n';j++,i++)
			line[i] = filecontent[j];
		sscanf(line,"%[\x20-\x7e] %x %x %x",modulename,&addr,&type,&passcount);
		for(i=0; i < modules->data.n; i++) {
			moduleinfo = (t_module *)((char *)modules->data.data + modules->data.itemsize*i);
			for(k=0;k<8;k++)
				modulename_temp[k] = moduleinfo->name[k];
			if(!strcmp(modulename_temp,modulename)) {
				addr += moduleinfo->base;
				break;
			}
		}
		if(i == modules->data.n) {
			wsprintf(result, "Module %s wasn't found!\n", modulename);
			MessageBox(hwmain, result,"LCB Import Info",MB_OK|MB_ICONINFORMATION);
		}
		else {
			if(Setbreakpointext(addr,type,0,passcount) != -1)
				count++;
		}
		ZeroMemory(line,1024);
	}
	if(!CloseHandle(h_file)) {
        ErrorHandler(GetLastError());
        return;
    }
	Mergequicknames();
	Redrawdisassembler();
	wsprintf(result, "Copied %d breakpoint from file\n", count);
	MessageBox(hwmain, result,"LCB Import Result",MB_OK|MB_ICONINFORMATION);
	if(!VirtualFree(filecontent,0,MEM_RELEASE))
		ErrorHandler(GetLastError());

	return;
}


/*****************************************************************************
* ExportLabel
*
*  Export labels from module to file
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ExportLabel(void) {
	char filename[2048] = {'\0'}, name[256] = {'\0'}, result[1024] = {'\0'}, modulename[9] = {'\0'};
	int count = 0, length;
	struct t_dump *currentmodule;
	struct t_module *moduleinfo;
	ulong start, stop, i;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to export labels",filename,".txt",0))
		return;
	h_file = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h_file == INVALID_HANDLE_VALUE) {
		ErrorHandler(GetLastError());
		return;
	}
	currentmodule = (t_dump *)Plugingetvalue(VAL_CPUDASM);
	moduleinfo = (t_module *)Findmodule(currentmodule->base);
	start = moduleinfo->base;
	stop = start + moduleinfo->size;
	for(i=0;i<8;i++)
		modulename[i] = moduleinfo->name[i];
	for(i=start; i < stop; i++) {
		if(Findname(i,NM_LABEL,name) != 0) {
			length = wsprintf(result,"%s\t%x\t%s\n",modulename,(i-start),name);
				if(!WriteFile(h_file,result,length,&lpNumberOfBytesWritten,NULL) || length != lpNumberOfBytesWritten) {
					ErrorHandler(GetLastError());
					CloseHandle(h_file);
					DeleteFile(filename);
					return;
				}
				count++;
		}
	}
	if(!CloseHandle(h_file)) {
		ErrorHandler(GetLastError());
		return;
	}
	wsprintf(result, "Exported %d user-defined labels from 0x%x-0x%x to file\n",count, start, stop-1);
	MessageBox(hwmain,result,"LCB plugin Export Result",MB_OK|MB_ICONINFORMATION);
	return;
}


/*****************************************************************************
* ExportComment
*
*  Export comments from module to file
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ExportComment(void) {
	char filename[2048] = {'\0'}, name[256] = {'\0'}, result[1024] = {'\0'}, modulename[9] = {'\0'};
	int count = 0, length;
	struct t_dump *currentmodule;
	struct t_module *moduleinfo;
	ulong start, stop, i;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to export comments",filename,".txt",0))
		return;
	h_file = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h_file == INVALID_HANDLE_VALUE) {
		ErrorHandler(GetLastError());
		return;
	}
	currentmodule = (t_dump *)Plugingetvalue(VAL_CPUDASM);
	moduleinfo = (t_module *)Findmodule(currentmodule->base);
	start = moduleinfo->base;
	stop = start + moduleinfo->size;
	for(i=0;i<8;i++)
		modulename[i] = moduleinfo->name[i];
	for(i=start; i < stop; i++) {
		if(Findname(i,NM_COMMENT,name) != 0) {
			length = wsprintf(result,"%s\t%x\t%s\n",modulename,(i-start),name);
				if(!WriteFile(h_file,result,length,&lpNumberOfBytesWritten,NULL) || length != lpNumberOfBytesWritten) {
					ErrorHandler(GetLastError());
					CloseHandle(h_file);
					DeleteFile(filename);
					return;
				}
				count++;
		}
	}
	if(!CloseHandle(h_file)) {
		ErrorHandler(GetLastError());
		return;
	}
	wsprintf(result, "Exported %d user-defined comments from 0x%x-0x%x to file\n",count, start, stop-1);
	MessageBox(hwmain,result,"LCB plugin Export Result",MB_OK|MB_ICONINFORMATION);
	return;
}


/*****************************************************************************
* ExportBreakpoint
*
*  Export breakpoints from module to file
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ExportBreakpoint(void) {
	char filename[2048] = {'\0'}, name[256] = {'\0'}, result[1024] = {'\0'}, modulename[9] = {'\0'};
	int count = 0, length;
	struct t_module *moduleinfo;
	struct t_table *bptable;
	struct t_bpoint *bpoint;
	ulong i, j;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;

	if(!Browsefilename("Select file to export breakpoints",filename,".txt",0))
		return;
	h_file = CreateFile(filename,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(h_file == INVALID_HANDLE_VALUE) {
		ErrorHandler(GetLastError());
		return;
	}
	bptable=(t_table *)Plugingetvalue(VAL_BREAKPOINTS);
	if(bptable==NULL) {
		MessageBox(hwmain,"Error getting breakpoint table","Error",MB_OK | MB_ICONERROR);
		if(!CloseHandle(h_file))
			ErrorHandler(GetLastError());
		return;
	}
	for(i=0; i < bptable->data.n; i++) {
		bpoint = (t_bpoint *)((char *)bptable->data.data + bptable->data.itemsize*i);
		moduleinfo = (t_module *)Findmodule(bpoint->addr);
		for(j=0;j<8;j++)
			modulename[j] = moduleinfo->name[j];
		length = wsprintf(result,"%s\t%x\t%x\t%x\n",modulename,(bpoint->addr - moduleinfo->base),bpoint->type,bpoint->passcount);
		if(!WriteFile(h_file,result,length,&lpNumberOfBytesWritten,NULL) || length != lpNumberOfBytesWritten) {
			ErrorHandler(GetLastError());
			CloseHandle(h_file);
			DeleteFile(filename);
			return;
		}
		count++;
	}
	if(!CloseHandle(h_file)) {
		ErrorHandler(GetLastError());
		return;
	}
	wsprintf(result,"Exported %d breakpoints\n",count);
	MessageBox(hwmain,result,"LCB plugin Export Result",MB_OK|MB_ICONINFORMATION);
	return;
}


/*****************************************************************************
* ErrorHandler
*
*  Display message box with error information
*
* Inputs
*  DWORD error_code: error code from GetLastError().
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

void ErrorHandler(DWORD error_code) {
	LPVOID lpMsgBuf;
	char errormsg[1024] = {'\0'};

	MessageBox(hwmain,"An error occured. Please, send an e-mail to scherzocrk@gmail.com reporting the problem.","Error",MB_OK | MB_ICONERROR);
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,NULL,error_code,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
	wsprintf(errormsg,"Error %d: %s",error_code,lpMsgBuf);
	MessageBox(hwmain,errormsg,"Error",MB_OK | MB_ICONERROR);
	return;	
}


/*****************************************************************************
* ODBG_Pluginreset
*
*  Function is called when user opens new or restarts current application. 
*  Plugin should reset internal variables and data structures to initial state.
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

extc void _export cdecl ODBG_Pluginreset(void) {
};


/*****************************************************************************
* ODBG_Pluginclose
*
*  OllyDbg calls this optional function when user wants to terminate OllyDbg.
*  Function must return 0 if it is safe to terminate. Any non-zero return 
*  will stop closing sequence. 
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  return 0 - safe termination
******************************************************************************/

extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};


/*****************************************************************************
* ODBG_Plugindestroy
*
*  OllyDbg calls this optional function once on exit. Function must free all 
*  internally allocated resources, likewindow classes, files, memory and so on.
*
* Inputs
*  None
* 
* Outputs
*  None
*
* Returns
*  None
******************************************************************************/

extc void _export cdecl ODBG_Plugindestroy(void) {
};