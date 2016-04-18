#define  _CRT_SECURE_NO_DEPRECATE
/****************************************************************************** 
/*                   Libraries used by this module
/*****************************************************************************/ 

#include "stdafx.h"
#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>


/****************************************************************************** 
/*                          Function prototypes
/*****************************************************************************/ 

void ImportComment(void);
void ErrorHandler(DWORD);
void LoadMap(int what);
DWORD GetCurrentEIP(void);


/****************************************************************************** 
/*                          Global variables
/*****************************************************************************/ 

HINSTANCE hinst; // DLL instance
HWND hwmain; // Handle of main OllyDbg window

unsigned char usecomment, uselabel;
t_module *pmodule;

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
	strcpy(shortname,"DLCI plugin"); // Name of plugin
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
	Addtolist(0,0,"Delphi Library Call Importer plugin v1.0");
	Addtolist(0,-1,"  Copyright (C) 2007 Advant");
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
			strcpy(data,"0 &Import DeDe Events File,"
				"1 &Import DeDe Map File|"
				"2 &About");
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
				ImportComment();
				break;
			case 1:
				LoadMap(1);
				break;
			case 2:
				MessageBox(hwmain,
					"Delphi Library Call Importer v1.0\n"
					"Import library calls into comments from DeDe analyzer\n"
				    "Thanks to DaFixer for his great app!\n"
				    "Thanks to scherzo for his plugin code!\n"
					"Thanks to godfather+ for his MapConv code!\n\n"
					"Copyright (C) 2007 Advant",
					"DLCI Plugin",MB_OK|MB_ICONINFORMATION);
				break;
			default:
				break;
		};
	};
};




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
	char filename[2048] = {'\0'}, result[1024] = {'\0'}, name[1024] = {'\0'}, line[1024] = {'\0'}, funcname[1024] = {'\0'},address[1024] = {'\0'};
	char modulename[9] = {'\0'}, modulename_temp[9] = {'\0'};
	char *filecontent, *str2;
	int count = 0, k, j, i,z, filesize;
	unsigned long maddr;
	struct t_module *moduleinfo;
	struct t_table *modules;
	HANDLE h_file;
	DWORD lpNumberOfBytesWritten;
    char * ptr1, *ptr2, *space_start, *lineptr;   
    t_table *tTable;
	void * ptr;
	ulong codesize = 0;

	lineptr = &line[0];

	  str2 = (char *)Plugingetvalue(VAL_PROCESSNAME);    //some checks
	  if (strlen(str2) == 0)
		{
		 MessageBox(hwmain,"No executable is loaded!","Error",MB_ICONERROR);
		 return;
		}
	pmodule = Findmodule((ulong)GetCurrentEIP());
	//codesize = (Findmemory(0x401000))->size;
	codesize = (Findmemory(pmodule->codebase))->size;

	if(!Browsefilename("Select DeDe Events file to import labels",filename,"*.txt",0)) 
		return;
	h_file = CreateFile(filename,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

    filesize = GetFileSize(h_file,NULL);
	filecontent = (char *)VirtualAlloc(NULL,filesize,MEM_COMMIT,PAGE_READWRITE);
	if(filecontent == NULL) {
		return;
	}
	if(!ReadFile(h_file,filecontent,filesize,&lpNumberOfBytesWritten,NULL) || lpNumberOfBytesWritten!=filesize) {
		return;
	}

	MessageBox(0,"Warning:  This could take a couple of minutes\ndepending on the size your your events.txt file.","Warning", MB_ICONINFORMATION);
	
	for(j=0; j < filesize; j++) {
		for(i=0;filecontent[j] != '\n';j++,i++)
			line[i] = filecontent[j];
		if(strncmp(line,"This file is generated",strlen("This file is generated")) == 0)
			{ZeroMemory(line,1024); continue; }//ignore the first line
		if(strlen(line) == 0)
			{ZeroMemory(line,1024); continue; } //ignore blank lines
		if(strcmp(line,"\r") == 0 || strcmp(line,"\n") == 0)
			{ZeroMemory(line,1024); continue; } //ignore blank lines
        
		if(strncmp(line,"Unit ",5) == 0)
			lineptr += 5;
		ptr1 = strchr(lineptr,' ');
		ptr2 = strchr(lineptr,';');

		if(ptr1 == 0 && ptr2 == 0)
			{ZeroMemory(line,1024); continue; }
		if(ptr1 != 0)
		{
		   space_start = ptr1;
           while(ptr1[0] == ' ')
			   ptr1++;
		   strncpy(funcname,lineptr,space_start - lineptr);
		   ptr1[strlen(ptr1) - 1] = 0;
		   strcpy(address, ptr1);
		   sscanf(ptr1,"%x",&maddr);

		}
		else
		{
		   ptr1 = ptr2; 
		   ptr2 = strchr(ptr2+1,';'); 
		   while(ptr2 > 0)
		   {
			   ptr1 = ptr2;
			   ptr2 = strchr(ptr2+1,';'); 
		   }
		   ptr2 = ptr1;
		   space_start = ptr2;
           while(ptr2[0] == ' ' || ptr2[0] == ';' )
			   ptr2++;

		   strncpy(funcname,lineptr,space_start - lineptr);
		   ptr2[strlen(ptr2) - 1] = 0;
		   strcpy(address, ptr2);
		   sscanf(ptr1,"%x",&maddr);


		}
		//printf("Name: >%s<  Address: >%x<\n", funcname, maddr);
		
		//Findreferences(0x401000, codesize, maddr,maddr+1,maddr,0,"Delphi Library Calls");
		Findreferences(pmodule->codebase, codesize, maddr,maddr+1,maddr,0,"Delphi Library Calls");

		tTable = (t_table *)Plugingetvalue(VAL_REFERENCES);
        if(tTable > 0)
		{
			for(z = 0; z < tTable->data.n; z++)
			{
			  ptr = Getsortedbyselection((t_sorted*)&tTable->data,z);	  
			  Quickinsertname(((t_sortheader*)ptr)->addr,NM_COMMENT,funcname);
			}

			ZeroMemory(funcname,1024);
			ZeroMemory(address,1024);
			ZeroMemory(line,1024);
			lineptr = &line[0];
			}
		}

	Mergequicknames();

	if(!VirtualFree(filecontent,0,MEM_RELEASE))
		ErrorHandler(GetLastError());
		
	return;
}
/*
	Findreferences(0x401000, 0xDB35A, 0x00402C14,0x00402C15,0x00402C14,0,"Test references");
	
	tTable = (t_table *)Plugingetvalue(VAL_REFERENCES);
	//Message(NULL, "Address: %X",((t_sortheader*)tTable->data.data)->addr);
	
	for(i = 0; i < tTable->data.n; i++)
	{
	  ptr = Getsortedbyselection((t_sorted*)&tTable->data,i);
	  Message(NULL, "Address: %X",((t_sortheader*)ptr)->addr);
	  Quickinsertname(((t_sortheader*)ptr)->addr,NM_COMMENT,"test comment");
	}
	Mergequicknames();
	
	//MessageBox(hwmain, result,"LCB Import Result",MB_OK|MB_ICONINFORMATION);

*/


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

	MessageBox(hwmain,"An error occured. Please, contact Advant reporting the problem.","Error",MB_OK | MB_ICONERROR);
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


DWORD GetCurrentEIP(void)
{
  t_thread* t2;// t_thread

  t2=Findthread(Getcputhreadid());
  return t2->reg.ip;
}

void LoadMap(int what)
{
  char path[255];
    char faddress[200]= {0};
	char address[200] = {0};
  char fname[500] = {0};
  FILE *in;

  char mapseg[8];
  char buff[500];
  char *mapoff = "0x12345678", *endptr;
  long lnumber;

  char maplabel[500];
  char mapline[TEXTLEN];

  char *str2;
  int i,j, addmenuid, mnuitems;

  str2 = (char *)Plugingetvalue(VAL_PROCESSNAME);    //some checks
  if (strlen(str2) == 0)
    {
     MessageBox(0,"Error: No process to add map info to.","Error", MB_ICONERROR);
     return;
    }
  strcpy(path,".\\");
  Browsefilename("Select map file",path,".map",0);

  if (stricmp(path, ".\\") == 0)        //just a litle check
    return;

  if ((in = fopen(path, "rt"))== NULL)
    in = NULL;
  else
      {
		   fgets(mapline, 256, in);
		  while (!feof(in))
			{
			
			strcpy(mapseg, " 0001:"); 
	        
					if ((strstr(mapline, mapseg) != NULL) && (strstr(mapline, "CODE") == NULL) && (strstr(mapline, "Program entry") == NULL))
					{
						  
						sscanf(mapline,"%s%s",faddress,fname);
						strncpy(address,faddress+5,8);
						sscanf(address,"%x",&lnumber);

						pmodule = Findmodule((ulong)GetCurrentEIP());
						Quickinsertname(pmodule->codebase + lnumber , NM_COMMENT, fname);

					  }
			  ZeroMemory(faddress,200);
			  ZeroMemory(fname,200);
			  fgets(mapline,256,in);	


		  }
		fclose(in);
     

      }

   Mergequicknames();
  //Setcpu(0,0,0,0,CPU_ASMFOCUS);
}
