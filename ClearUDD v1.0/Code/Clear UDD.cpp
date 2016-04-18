// Clear UDD.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "Plugin.h"
#include <windows.h>
#include <iostream.h>
#include   "Shlwapi.h" 
#include <shellapi.h>



HWND HOD;
char  szPath[MAX_PATH];
char  szDirectory[MAX_PATH];
char  szudd[MAX_PATH];
char  szbak[MAX_PATH];
char  szTemp[MAX_PATH];
char *lpStr;
char  szDel[]="del *.udd\r\n\del *.bak\r\n";
WIN32_FIND_DATA szFiledata;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
    strcpy(shortname,"Clear UDD");
	return PLUGIN_VERSION;
}

extc int ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features)
{
	HOD=hw;
	GetModuleFileName(NULL,szPath,MAX_PATH);
	lpStr=szPath;
    PathRemoveFileSpec(lpStr);
	strcat(szPath,"\\UDD");
	strcpy(szDirectory,szPath);
    strcpy(szudd,szPath);
	strcpy(szbak,szPath);
	strcat(szPath,"\\del.bat");
	strcat(szudd,"\\*.udd");
	strcat(szbak,"\\*.bak");
	return 0;
}

extc int ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
	if (origin==PM_MAIN	)
	{
        strcpy(data,"0 Del udd,1 Goto directory|2 About");
		return 1;
    }
 
    return 0;
}

extc void ODBG_Pluginaction(int origin,int action,void *item)
{
	
	HANDLE hFile;
	HANDLE hFindFile;
	int hFindNext;
	int  i;
	DWORD dwBytesToWrite;


	RtlZeroMemory(&szTemp,MAX_PATH);
	if (origin ==PM_MAIN)	
   {
	  
		
	  switch (action)
	  {
	  case 0:
		  
          hFile =CreateFile(szPath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		  if (hFile!= INVALID_HANDLE_VALUE)
		  {
              
			  i=WriteFile(hFile,szDel,sizeof(szDel),&dwBytesToWrite,NULL);
			  if (i!=0)
			  {
                  
				 
				  hFindFile=FindFirstFile(szudd,&szFiledata);
				  if (hFindFile!=INVALID_HANDLE_VALUE)
				  {
                        do 
                        {
                            strcat(szTemp,szDirectory);
							strcat(szTemp,"\\");
							strcat(szTemp,szFiledata.cFileName);
                            DeleteFile(szTemp);
							RtlZeroMemory(&szTemp,MAX_PATH);
							hFindNext=FindNextFile(hFindFile,&szFiledata);
                        } while (hFindNext!=0);
				  }
				  FindClose(hFindFile);
				  hFindFile=FindFirstFile(szbak,&szFiledata);
				  if (hFindFile!=INVALID_HANDLE_VALUE)
				  {
					  do 
					  {
						  strcat(szTemp,szDirectory);
						  strcat(szTemp,"\\");
						  strcat(szTemp,szFiledata.cFileName);
                          DeleteFile(szTemp);
						  RtlZeroMemory(&szTemp,MAX_PATH);
						  hFindNext=FindNextFile(hFindFile,&szFiledata);
					  } while (hFindNext!=0);
				  }
				  FindClose(hFindFile);
				  MessageBox(HOD,"succeed","Clear UDD",MB_OK+MB_ICONASTERISK);
			  } 
			  else
			  {
				  CloseHandle(hFile);
				  goto Error;
			  }
			  CloseHandle(hFile);
		  } 
		  else
		  {
Error:	  
			  MessageBox(HOD,"Failure","Clear UDD",MB_OK+MB_ICONSTOP);
		  }
		  break;
	  case 1:
		     ShellExecute(HOD,NULL,szDirectory,0,NULL,SW_SHOW);
             break;
	  case 2:
          MessageBox(HOD,"Clear UDD v1.0\nby cektop","Abuot Clear UDD",MB_OK+MB_ICONINFORMATION);
		  break;
	  }
   }

}


