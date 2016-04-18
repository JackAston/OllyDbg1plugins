#include "stdafx.h"
#define _CHAR_UNSIGNED
#include "windows.h"
#include "commdlg.h"
#include "plugin.h"
//-------------------------------------Type definitions---------------------------------------------
typedef t_status(__cdecl *FUNC)(void);
//----------------------------Functions Declaration-------------------------------------------------
extern "C"
{
	void __stdcall DebugActiveProcessStop(int);
}
//-------------------------------------My own stoopid function--------------------------------------
unsigned long relativeOffset(unsigned long from,unsigned long to)
{
	unsigned long diff=0;
	if(from>to)
	{
		 //  5-byte jump instruction
         diff=((unsigned long)from+5)-(unsigned long)to;
		 diff=-diff;
	}
	else
	{
          diff=(unsigned long)to-((unsigned long)from+5);
	}
	return diff;
}
//----------------------------OS version stuff--------------------------------------
struct OSVERSIONINFOEx 
{
  DWORD dwOSVersionInfoSize;
  DWORD dwMajorVersion;
  DWORD dwMinorVersion;
  DWORD dwBuildNumber;
  DWORD dwPlatformId;
  TCHAR szCSDVersion[128];
  WORD  wServicePackMajor;
  WORD  wServicePackMinor;
  WORD  wSuiteMask;
  BYTE  wProductType;
  BYTE  wReserved;
};
#define VER_NT_WORKSTATION 0x0000001
//---------------------------------------------------------------------------------
HWND mainWnd;
bool os_supported;
int ver;
HANDLE hProcess;
unsigned long TEB;
bool done;
//---------WINXP-specific----------
bool INT2E;
//---------Win7-specific-----------
unsigned long gate_key;
char gate_code[7];
unsigned char* pShellcode;

//winxp position-independent shellcode
char shellcode_xp[]=
"\x9C\x60\xA1\x3C\x03\x23\x00\x85"
"\xC0\x74\x37\x83\x3D\x38\x03\x23"
"\x00\x01\x74\x25\x8B\x58\x3C\x8B"
"\x5C\x18\x28\x03\xD8\x90\x54\x6A"
"\x04\x6A\x00\xC7\x05\x38\x03\x23"
"\x00\x01\x00\x00\x00\xFF\xD3\xC7"
"\x05\x38\x03\x23\x00\x00\x00\x00"

"\x00\x61\x9D\x8B\xD4\x0F\x34\x90"
"\x90\x90\x83\x3D\x04\x04\x23\x00"
"\x00\x75\xEE\xC7\x05\x04\x04\x23"
"\x00\x01\x00\x00\x00\x68\x00\x05"
"\x23\x00\xFF\x15\x08\x04\x23\x00"
"\x85\xC0\xA3\x3C\x03\x23\x00\x74"
"\xD0\xEB\x97";

char INT_2E[]="\x8D\x54\x24\x08\xCD\x2E\xC3";

//win7 position-independent shellcode
char shellcode_7[]=
"\x9C\x60\xA1\x3C\x03\x22\x00\x85"
"\xC0\x74\x37\x83\x3D\x38\x03\x22"
"\x00\x01\x74\x25\x8B\x58\x3C\x8B"
"\x5C\x18\x28\x03\xD8\x90\x54\x6A"
"\x04\x6A\x00\xC7\x05\x38\x03\x22"
"\x00\x01\x00\x00\x00\xFF\xD3\xC7"
"\x05\x38\x03\x22\x00\x00\x00\x00"
"\x00\x61\x9D\xEA\x1E\x27\x06\x75"
"\x33\x00\x83\x3D\x04\x04\x22\x00"
"\x00\x75\xEE\xC7\x05\x04\x04\x22"
"\x00\x01\x00\x00\x00\x68\x00\x05"
"\x22\x00\xFF\x15\x08\x04\x22\x00"
"\x85\xC0\xA3\x3C\x03\x22\x00\x74"
"\xD0\xEB\x97";

//---------exports-------------------------
extern "C"
{
_declspec(dllexport)  int __cdecl ODBG_Plugindata(char shortname[32]) 
{
    strcpy(shortname,"InjectHookLib");       // Name of plugin
    return 110;   //version 1.10
}
_declspec(dllexport) int __cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
  mainWnd=hw;
  Addtolist(0,0,"InjectHookLib");
  Addtolist(0,-1,"started successfully");
  return 0;
}
_declspec(dllexport) int __cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
   if(origin==PM_MAIN)
   {
		strcpy(data,"0 Choose DLL");
		return 1;
   }
   return 0;
}
_declspec(dllexport) void __cdecl ODBG_Pluginmainloop(DEBUG_EVENT *pde)
{
	if(!pde) return;
	if(pde->dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT)
	{
		 TEB=(unsigned long)(pde->u.CreateProcessInfo.lpThreadLocalBase); //Main thread's TEB.
		 hProcess=pde->u.CreateProcessInfo.hProcess;
	}
}
_declspec(dllexport) void __cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
  if(origin==PM_MAIN) 
  {
	  if(action==0) 
	  {
		  if(os_supported==false || ver==0)
		  {
			  MessageBox(mainWnd,"OS not supported.\r\n Only Windows 7 SP0/SP1 and XP SP2/SP3 are supported.","InjectHookLib",0);
			  return;
		  }
		  else
		  {
			  FUNC Get_Status=(FUNC)GetProcAddress(GetModuleHandle(0),"_Getstatus");
		      if(Get_Status()!=STAT_STOPPED)
			  {
			        MessageBox(mainWnd,"Target process must be in paused mode.","InjectHookLib",0);
			        return;
			  }
			  if(done)        //we had already patched the target process
			  {
				  MessageBox(mainWnd,"Already hooked","InjectHookLib",0);
				  return;
			  }
			  char pathhxx[MAX_PATH]={0};
			  OPENFILENAME OFN={0};
              OFN.lStructSize=sizeof(OPENFILENAME);
              OFN.hwndOwner=mainWnd;
              OFN.lpstrFilter="Dynamic link library(*.dll)\0*.dll\0\0";
              OFN.lpstrFile=pathhxx;
              OFN.nMaxFile=MAX_PATH;
              OFN.lpstrTitle="Select DLL to inject";			  
			  if((ver==1) || (ver==2))
			  {	         
				         if(!GetOpenFileName(&OFN))
						 {
							  MessageBox(mainWnd,"Can't inject dll file.","InjectHookLib",0);
							  return;
						 }
			  }

			  if(ver==1) //Windows XP SP2/SP3
			  {
				     unsigned long kiFast=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"KiFastSystemCall");
					 unsigned long kiInt2e=(unsigned long)GetProcAddress(GetModuleHandle("ntdll.dll"),"KiIntSystemCall");

					 unsigned long readd_;
					 unsigned long SystemCall=0;  //_KUSER_SHARED_DATA::SystemCall At offset 0x300
					 ReadProcessMemory(hProcess,(void*)0x7ffe0300,&SystemCall,0x4,&readd_);
					 if(SystemCall==kiInt2e)  //Perhaps, it uses KiIntSystemCall instead.
					 {
						 INT2E=true;
					 }
					 else if(SystemCall==kiFast)
					 {
						 INT2E=false;
					 }
					 else
					 {
						 MessageBox(mainWnd,"Architecture not supported!","InjectHookLib",0);
						 return;
					 }

					 
					 bool properAddr_found=false;
					 unsigned long rel=0;
					 unsigned char* po=0;
					 if(INT2E==true)
					 {
						    po=(unsigned char*)VirtualAllocEx(hProcess,0,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
							if(po==0)
							{
								MessageBox(mainWnd,"Can't allocate memory into target process","InjectHookLib",0);
								return;
							}
							else 
							{
								rel=relativeOffset(kiInt2e,(unsigned long)po);
								properAddr_found=true;
							}
					 }
					 else
					 {
						    unsigned long least=0x20000;
							unsigned long max=0x80000000;
							for(unsigned long i=least;i<max;i+=0x1000)   //Try to find the proper address
							{
		                                  rel=relativeOffset(kiFast,i);
			                              if(  (rel>>0x18) == 0xC3)  //This is a must
										  {
				                                po=(unsigned char*)VirtualAllocEx(hProcess,(void*)i,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
								                if(po) /*Okay, lets copy shellcode_xp*/
												{
									                  if((unsigned long)po!=i)
													  {
										                       Addtolist(0,0,"Retrying...");
										                       VirtualFreeEx(hProcess,po,0x1000,MEM_DECOMMIT|MEM_RELEASE);
										                       continue;
													  }
													  else
													  {
														  properAddr_found=true;
														  break;
													  }
												}
										  }
							}
					 }
					 if(properAddr_found)
					 {
                                    unsigned long read__written;
									 //--------------------------Formulate shellcode---------------------------
                                    *(unsigned long*)(&shellcode_xp[0x3])=(unsigned long)(po+0x33C); //ImageBase of new DLL.
									*(unsigned long*)(&shellcode_xp[0x63])=(unsigned long)(po+0x33C); //ImageBase of new DLL.
									*(unsigned long*)(&shellcode_xp[0xD])=(unsigned long)(po+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_xp[0x25])=(unsigned long)(po+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_xp[0x31])=(unsigned long)(po+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_xp[0x56])=(unsigned long)(po+0x500); //
									WriteProcessMemory(hProcess,po+0x500,OFN.lpstrFile,strlen(OFN.lpstrFile)+1,&read__written);
									unsigned long _pLdLib_=(unsigned long)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
									*(unsigned long*)(&shellcode_xp[0x5C])=(unsigned long)(po+0x408); 
									WriteProcessMemory(hProcess,(void*)(po+0x408),(void*)(&_pLdLib_),0x4,&read__written);
									*(unsigned long*)(&shellcode_xp[0x44])=(unsigned long)(po+0x404);
									*(unsigned long*)(&shellcode_xp[0x4D])=(unsigned long)(po+0x404);
									if(INT2E)  memcpy(&shellcode_xp[0x3B],INT_2E,7);
									 //--------------------------Write shellcode-------------------------------
									 unsigned long read;
									 WriteProcessMemory(hProcess,po,shellcode_xp,sizeof(shellcode_xp)-1,&read);
									 //--------------------------Patch ntdll.KiFastSystemCall------------------
									 unsigned long old;
									 unsigned char jump=0xE9;
									 unsigned long Ki_=0;
									 if(INT2E==false) Ki_=kiFast;
									 else Ki_=kiInt2e;

									 VirtualProtectEx(hProcess,(void*)Ki_,0x5,PAGE_EXECUTE_READWRITE,&old);
							         WriteProcessMemory(hProcess,(void*)Ki_,&jump,0x1,&read);
									 WriteProcessMemory(hProcess,(void*)(Ki_+1),&rel,0x4,&read);
									 VirtualProtectEx(hProcess,(void*)Ki_,0x5,old,&old);
									 Addtolist((unsigned long)po,0,"Hook code inserted.");
									 done=true;
					 }
				     if(!done)
					 {
					       MessageBox(mainWnd,"Sorry, proper memory address could not be found","InjectHookLib",0);
					       return;
					 }
			  }
			  else if(ver==2) /*windows 7*/
			  {
				  if(TEB)
				  {
					     pShellcode=(unsigned char*)VirtualAllocEx(hProcess,0,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
						 if(!pShellcode)
						 {
							  MessageBox(mainWnd,"Can't allocate memory in debuggee","InjectHookLib",0);
							  return;
						 }
					     unsigned long read_written;
						 if(ReadProcessMemory(hProcess,(void*)(TEB+0xC0),&gate_key,0x4,&read_written))
						 {
							 if(ReadProcessMemory(GetCurrentProcess(),(void*)gate_key,&gate_code[0],0x7,&read_written)) //read from self
							 {
							        unsigned long oldxx;
						            if(!VirtualProtectEx(hProcess,(void*)gate_key,0x6,PAGE_EXECUTE_READWRITE,&oldxx))
									{
							               MessageBox(mainWnd,"Can't set proper memory protection.","InjectHookLib",0);
							               return;
									}
									WriteProcessMemory(hProcess,(void*)gate_key,"\x68" /*push*/,0x1,&read_written);
							        WriteProcessMemory(hProcess,(void*)(gate_key+1),&pShellcode,0x4,&read_written);
									WriteProcessMemory(hProcess,(void*)(gate_key+5),"\xC3",0x1,&read_written);
									//--------Formulate shellcode here-------------------------------------------
									*(unsigned long*)(&shellcode_7[0x3])=(unsigned long)(pShellcode+0x33C); //ImageBase of new DLL.
									*(unsigned long*)(&shellcode_7[0x63])=(unsigned long)(pShellcode+0x33C); //ImageBase of new DLL.
									*(unsigned long*)(&shellcode_7[0xD])=(unsigned long)(pShellcode+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_7[0x25])=(unsigned long)(pShellcode+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_7[0x31])=(unsigned long)(pShellcode+0x338); //call from inside DllMain
									*(unsigned long*)(&shellcode_7[0x56])=(unsigned long)(pShellcode+0x500); //
									WriteProcessMemory(hProcess,pShellcode+0x500,OFN.lpstrFile,strlen(OFN.lpstrFile)+1,&read_written);
									unsigned long pLdLib=(unsigned long)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
									*(unsigned long*)(&shellcode_7[0x5C])=(unsigned long)(pShellcode+0x408); 
									WriteProcessMemory(hProcess,(void*)(pShellcode+0x408),(void*)(&pLdLib),0x4,&read_written);
									*(unsigned long*)(&shellcode_7[0x44])=(unsigned long)(pShellcode+0x404);
									*(unsigned long*)(&shellcode_7[0x4D])=(unsigned long)(pShellcode+0x404);
									memcpy(&shellcode_7[0x3B],gate_code,0x7);
									//memcpy(&shellcode_7[sizeof(shellcode_7)-8],gate_code,0x7);
									//--------------------------------------------------------------------------
									WriteProcessMemory(hProcess,pShellcode,shellcode_7,sizeof(shellcode_7)-1,&read_written);  //write shellcode  			
									VirtualProtectEx(hProcess,(void*)gate_key,0x6,oldxx,&oldxx);
									Addtolist((unsigned long)pShellcode,0,"Hook code inserted.");
									done=true;
							 }
						 }
				  }
			  }
		  }
	  }
  }
}

__declspec(dllexport) void __cdecl ODBG_Pluginreset(void)
{
		TEB=0;
		hProcess=0;
		INT2E=false;
		gate_key=0;
	    memset(gate_code,0,7);
		pShellcode=0;
		done=0;
}

__declspec(dllexport) int __cdecl ODBG_Pluginclose(void)
{
	TEB=0;
	hProcess=0;
	INT2E=false;
	gate_key=0;
	memset(gate_code,0,7);
	pShellcode=0;
	done=0;
	return 0;
}

}

//------------------------------
BOOL __stdcall Dllmain(HMODULE hModule,int reason,LPVOID lpReserved)
{
	if(reason==DLL_PROCESS_ATTACH)
	{
		OSVERSIONINFOEx OSEX={sizeof(OSVERSIONINFOEX)};
		if(GetVersionEx((OSVERSIONINFO*)&OSEX))
		{

			if( OSEX.dwMajorVersion==6 && OSEX.dwMinorVersion==1 && OSEX.wProductType==VER_NT_WORKSTATION)
			{
				if( (OSEX.wServicePackMajor==0) || (OSEX.wServicePackMajor==1) )
				{
					os_supported=true;
				    ver=2; //win7 SP0 or win7 SP1.
				}
			}
			else if(   OSEX.dwMajorVersion==5 && OSEX.dwMinorVersion==1  )
			{
				if( (OSEX.wServicePackMajor==2) || (OSEX.wServicePackMajor==3) )
				{
				    os_supported=true;
				    ver=1;  //XP SP2/SP3
				}
			}
		}
	}
    return TRUE;
}