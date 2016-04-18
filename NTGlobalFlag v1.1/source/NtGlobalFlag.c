#include <windows.h>
#include <winbase.h>
#include <shlwapi.h>
#include <winternl.h>
#include <stdlib.h>
#include "plugin.h"


NTSTATUS (WINAPI *ntdll_NtQueryInformationProcess)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG,PULONG);


 
HINSTANCE        		hinst;
HINSTANCE	 		Dll_Handle;


FARPROC				ntdll_DbgBreakPoint;
FARPROC				ntdll_RtlRaiseException;

void *				status;
void *				glob;
void *				data;
void *				debesp;
void *				debespplus;
void *				hProcess;

char *				mySecPointer;

LPVOID 				dlah;

ulong				myPointer;

long				elah;
long 				isSet;
long 				isUnset;

int 				type;
int 				pid;
int 				answer;
int				noofitems;
int 				breakname;
int				lenofbreakname;
int				debstrlen;
int 				breakontlscallback;
int 				breakondllinitrout;

PROCESS_BASIC_INFORMATION 	pbi;
MEMORY_BASIC_INFORMATION	mbi;
DEBUG_EVENT			debev;
t_table 			valref;
t_reg				currentreg;

DWORD				lpOld;
DWORD				Curr_Pid;

HANDLE				Curr_Proc_Pseudo;
HANDLE				debproc;

HWND             		hwmain; 
HWND				seqwindow;
HWND				editwindow;
HWND				buttwindow;
HWND				asswindow;
HWND				asseditwindow;
HWND				assbuttwindow;
HWND				asscombo;
HWND				asscancel;

char 				clah[4] = {0x00,0x00,0x00,0x00};
char             		NtGlobalFlagwinclass[32];	
char 				spattern[] = {"MOV DWORD PTR DS:[RA+CONST],40010006"};
char 				mystring[200]; 
char 				asstext[200];
char 				trim[200];
char				debugstring[400];
char				tlscallback[10];

int myfunc (int flag);
DWORD WINAPI ThreadProcFirst( LPVOID lpParam );
DWORD WINAPI ThreadProcSecond( LPVOID lpParam );


#pragma argsused
BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved)
{
  	if (reason==DLL_PROCESS_ATTACH)
    	hinst=hi;                            // Mark plugin instance
  	return 1;                            // Report success
};





#pragma argsused
extc int _export cdecl ODBG_Plugindata(char shortname[32])

{
	strcpy(shortname,"NtGlobalFlag");       // Name of plugin
	return PLUGIN_VERSION;
};



#pragma argsused
extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features)

{
  	// Check that version of OllyDbg is correct.
  	if (ollydbgversion<PLUGIN_VERSION)
    	return -1;
 	// Keep handle of main OllyDbg window. This handle is necessary, for example,
  	// to display message box.
  	hwmain=hw;
		Addtolist(0,0,"NtGlobalFlag Plugin v1.10");
  		Addtolist(0, 1,"  Dedicated to oleh yuschuk author of this Debugger");
		Addtolist(0,-1,"    authored by stingduk");
		Addtolist(0, 1,"      and thanks to Detten for cing my asm ");
		return 0;
};





#pragma argsused
int ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent)
{
	
	if (reg != 0)
	{
	if(debugevent !=0)
	{
		
		debev = *debugevent;
		currentreg = *reg;
		
		if(debev.u.Exception.ExceptionRecord.ExceptionCode != EXCEPTION_BREAKPOINT)
		{
		return 0;
		}
		Dll_Handle = GetModuleHandle("ntdll.dll");
		if(Dll_Handle ==0)
		{
		MessageBox(hwmain,"error","LoadLibFailed",MB_OK);
		return 0;
		}
		ntdll_DbgBreakPoint = GetProcAddress(Dll_Handle,"DbgBreakPoint");
		if(ntdll_DbgBreakPoint == 0)
		{
		return 0;
		}
		ntdll_RtlRaiseException = GetProcAddress(Dll_Handle,"RtlRaiseException");
		if(ntdll_RtlRaiseException == 0)
		{
		return 0;
		}


		if (debev.u.Exception.ExceptionRecord.ExceptionAddress == ntdll_DbgBreakPoint)
		{ 		

			*(FARPROC *)&ntdll_NtQueryInformationProcess = GetProcAddress(Dll_Handle,"NtQueryInformationProcess");
			if(ntdll_NtQueryInformationProcess == 0)
			{
			MessageBox(hwmain,"error","GetProcFailed",MB_OK);
			return 0;
			}

			pid = Plugingetvalue(VAL_PROCESSID);
			debproc = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
			ntdll_NtQueryInformationProcess(debproc,ProcessBasicInformation,&pbi,sizeof(pbi),NULL);
			status = pbi.PebBaseAddress;
			(byte *)status += 0x68;
			VirtualQueryEx(debproc,status,&mbi,sizeof(mbi));
			VirtualProtectEx(debproc,mbi.BaseAddress,mbi.RegionSize,PAGE_EXECUTE_READWRITE,&lpOld);
			ReadProcessMemory(debproc,status,&dlah,4,NULL);
			elah = (long)dlah;
			elah |= 0x02;
			WriteProcessMemory(debproc,status,&elah,4,NULL);
			noofitems = 0;
			while (noofitems < 2)
			{
			CreateThread(NULL,NULL,ThreadProcFirst,NULL,NULL,NULL);
			Sendshortcut(PM_DISASM,0,WM_KEYDOWN,1,0,'S');
			Sendshortcut(PM_DISASM,0,WM_KEYDOWN,1,0,'R');
			valref = *(t_table *)Plugingetvalue(VAL_REFERENCES);
			noofitems = valref.data.n;
			}
			valref = *(t_table *)Plugingetvalue(VAL_REFERENCES);
			data = valref.data.data;
			Setdisasm(*(ulong *)data,1,CPU_ASMCENTER|CPU_ASMFOCUS);
			memset(mystring,0,200);
			wsprintf(mystring,"Assemble at %x",*(ulong *)data);
			CreateThread(NULL,NULL,ThreadProcSecond,NULL,NULL,NULL);
			Sendshortcut(PM_DISASM,*(ulong *)data,WM_CHAR,0,0,' ');
			Insertname((ulong)ntdll_RtlRaiseException,NM_BREAK,"STRING [[[esp+4]+18h]] == \"LDR: Calling Tls Callback Imagebase\" | STRING [[[esp+4]+18h]] == \" - Calling\"");
			Setbreakpointext((ulong)ntdll_RtlRaiseException,TY_ACTIVE|TY_KEEPCOND,0,0);
			Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F9);
			VirtualProtectEx(debproc,mbi.BaseAddress,mbi.RegionSize,lpOld,&lpOld);
			CloseHandle(debproc);
			return 0;
		}
		else if (debev.u.Exception.ExceptionRecord.ExceptionAddress == ntdll_RtlRaiseException)
		{
			
			myPointer = currentreg.r[4];
			myPointer += 0x4;
			hProcess = (void *)Plugingetvalue(VAL_HPROCESS);
			ReadProcessMemory(hProcess,(const void *)myPointer,&debesp,4,NULL);
			(byte *)debesp += 0x18;
			ReadProcessMemory(hProcess,(const void *)debesp,&debespplus,4,NULL);
			ReadProcessMemory(hProcess,(const void *)debespplus,&debugstring,0x400,NULL);
			debstrlen = lstrlen(debugstring);
			mySecPointer = strrchr(debugstring,0x20);
			if(StrCmpN(" - Calling",debugstring,10) == 0)
			{
			if(breakondllinitrout == 1)
			{
			Setbreakpointext(strtol((unsigned char *)mySecPointer,NULL,16),TY_ONESHOT,0,0);
			Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F9);
			return 0;
			}
			Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F9);
			return 0;
			}
			if(breakontlscallback == 1)
			{
			Setbreakpointext(strtol((unsigned char *)mySecPointer,NULL,16),TY_ONESHOT,0,0);
			Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F9);
			return 0;
			}
			Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F9);
			return 0;
		}
		else 
		{
			return 0;
		}	
	}

	}
return 0;
};



#pragma argsused
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)

{
switch (origin)
	{
	case PM_MAIN:                      // Plugin menu in main window
      	strcpy(data,"0 &SET LDR_SHOW_SNAPS|1 &UNSET LDR_SHOW_SNAPS|3 &SetTempBreak on Tls cllabacks |4 &SetTempbreak on all DllInit Routines|2 &About");
	return 1;
	default: break;                    // Any other window
	};
  	return 0;                           // Window not supported by plugin
};




#pragma argsused
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item)

{
if (origin==PM_MAIN)
 {
    switch (action)
   	{
  	case 2:
 		MessageBox(hwmain,
          	"NtGlobalFlag v1.10\n"
	  	"thanks to oleh yuschuk for making this wonderful Debugger\n"
	  	"thanks to Detten for converting my crazy asm code and ideas\n"
	  	" into c and compiling this plugin",
	  	"NtGLobalFlag",
          	 MB_OK|MB_ICONINFORMATION);
        	break;
	
	case 0:
	isSet = myfunc (2);
		if(isSet == 1)
		MessageBox(hwmain,"LDR_SHOW_SNAPS was successfully SET\n"
				  "RESTART DEBUGEE \n\n"
				  "YES DEBUGGEE NOT DEBUGGER :)\n",
				  "         Success",
				   MB_OK|MB_ICONINFORMATION);
		else
		MessageBox(hwmain,"         ERROR","LDR_SHOW_SNAPS could not be SET",MB_OK|MB_ICONEXCLAMATION);
		break;
	case 1:
	isUnset = myfunc (0);
	if(isUnset == 1)
		MessageBox(hwmain,"         Success","LDR_SHOW_SNAPS was successfully UNSET",MB_OK|MB_ICONINFORMATION);
		else
		MessageBox(hwmain,"         ERROR","LDR_SHOW_SNAPS could not be UNSET",MB_OK|MB_ICONEXCLAMATION);
		break;
	case 3:
		breakontlscallback = 1;
		break;
	case 4:
		breakondllinitrout = 1;
		break;
	
      default: break;
	};
 }
};






int myfunc(int flag)
{
		Curr_Pid  = GetCurrentProcessId();
		Curr_Proc_Pseudo = OpenProcess(PROCESS_ALL_ACCESS,FALSE,Curr_Pid); 
		type = Plugingetvalue(VAL_FONTHEIGHTS); 
		(int)glob = type+0x2a2C;
		if (flag == 2)
			{
			clah[0] = 0;
			}
			else
			{
			clah[0] = 2;
			}
		if(WriteProcessMemory(Curr_Proc_Pseudo,glob,&clah,1,NULL))
			{
			CloseHandle(Curr_Proc_Pseudo);
                        return 1;
			}
		CloseHandle(Curr_Proc_Pseudo);
return -1;
}







#pragma argsused
DWORD WINAPI ThreadProcFirst( LPVOID lpParam )

{
	seqwindow =0;
	editwindow = 0;
	buttwindow =0;

		while (seqwindow == 0)
		{
		seqwindow = FindWindow("#32770","Find sequence of commands");
		}

		while (editwindow == 0)
		{
		editwindow = FindWindowEx(seqwindow,NULL,"EDIT",NULL);
		}
	SendMessage(editwindow,WM_SETTEXT,NULL,(long)&spattern);

		while (buttwindow == 0)
		{
		buttwindow = FindWindowEx(seqwindow,NULL,"BUTTON","Find");
		}
	SendMessage(buttwindow,BM_CLICK,NULL,NULL);
	return 0;
}


#pragma argsused
DWORD WINAPI ThreadProcSecond( LPVOID lpParam )

{
	memset(asstext,0,200);
	memset(trim,0,200);
	memcpy(trim,"JMP ",4);
	asswindow = 0;
	asseditwindow =0;
	assbuttwindow =0;
	asscombo = 0;
	asscancel = 0;
		while (asswindow == 0)
		{
		asswindow = FindWindow("#32770",mystring);
		}

		while (asscombo ==0)
		{
		asscombo = FindWindowEx(asswindow,NULL,"ComboBox",NULL);
		}

		while (asseditwindow == 0)
		{
		asseditwindow = FindWindowEx(asscombo,NULL,"EDIT",NULL);
		}
	SendMessage(asseditwindow,WM_GETTEXT,100,(long)&asstext);
	if(StrCmpN("JNZ ",asstext,4) != 0)
		{
		MessageBox(hwmain,"strings are not equal","NtGlobalFlag",MB_OK);
		return 0;
		}
	StrCat(trim,asstext+4);
	SendMessage(asseditwindow,WM_SETTEXT,NULL,(long)&trim);

		while (assbuttwindow == 0)
		{
		assbuttwindow =FindWindowEx(asswindow,NULL,"BUTTON","Assemble");
		}
	SendMessage(assbuttwindow,BM_CLICK,NULL,NULL);

		while (asscancel ==0)
		{
		asscancel = FindWindowEx(asswindow,NULL,"BUTTON","Cancel");
		}
	SendMessage(asscancel,BM_CLICK,NULL,NULL);
return 0;
}

