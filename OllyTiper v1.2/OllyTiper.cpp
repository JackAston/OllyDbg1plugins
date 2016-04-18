// OllyTiper.cpp : Defines the initialization routines for the DLL.
//  [6/2/2006 Ryokou]

#include "stdafx.h"
#include "OllyTiper.h"
#include "Plugin.h"
//#include "AggressiveOptimize.h"
#pragma comment(linker,"/SECTION:.text,ERW")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define stAddrCount 20    //storageAddr Count
void patchcode();
//////////////////////////////////////////////////////////////////////////
//Cunstom MyTablefunction
void MyTablefunction(t_table *pt,HWND hw,UINT msg,WPARAM wParam,LPARAM lParam);
t_table*	tf_table;
t_sorted	tf_sorted;
HWND		tf_hw;
UINT		tf_msg;
WPARAM		tf_wParam;
LPARAM		tf_lParam;
//....................................................................
HWND			g_hwMain;
DWORD			_Tablefunction,_Setcpu,SetcpuAddr,forwardAddr;
unsigned char	szJmp[]={0xE9};
DWORD			oldProtect1,oldProtect;

//////////////////////////////////////////////////////////////////////////
//Patch code in ollydbg
void patchcode()
{	
	//hook_Tablefunction	
		_Tablefunction= (DWORD)GetProcAddress((HINSTANCE)Plugingetvalue(VAL_HINST),"_Tablefunction");
		Addtolist(NULL,1,"_Tablefunction Addr %08X",_Tablefunction);
		VirtualProtect((void*)_Tablefunction,10,PAGE_EXECUTE_READWRITE,&oldProtect);
		__asm
		{
			mov esi,[_Tablefunction]
			mov edi,offset to_Tablefunction
			mov ecx,9
			rep movsb 
			mov eax,[_Tablefunction]
			mov ebx,offset New_Tablefunction
			sub ebx,eax
			sub ebx,5
			mov byte ptr[eax],0xE9
			mov [eax+1],ebx
		}
		return;
//////////////////////////////////////////////////////////////////////////
//our new _Tablefunction
New_Tablefunction:
		__asm
		{
			mov eax,[esp+4]
			mov [tf_table],eax
			mov eax,[esp+8]
			mov [tf_hw],eax
			mov eax,[esp+0xc]
			mov [tf_msg],eax
			mov eax,[esp+0x10]
			mov [tf_wParam],eax
			mov eax,[esp+0x14]
			mov [tf_lParam],eax
		}
		__asm   pushad
		MyTablefunction(tf_table,tf_hw,tf_msg,tf_wParam,tf_lParam);
		__asm   popad
to_Tablefunction:
		//org code from the function
 		__asm __emit 0x90
 		__asm __emit 0x90
 		__asm __emit 0x90 
 		__asm __emit 0x90
 		__asm __emit 0x90
 		__asm __emit 0x90
 		__asm __emit 0x90
 		__asm __emit 0x90
		__asm __emit 0x90
		__asm	mov eax,[_Tablefunction]
		__asm	add eax,9
		__asm	jmp eax
}
//////////////////////////////////////////////////////////////////////////
extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,unsigned long *features)
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	Addtolist(NULL,1,"OllyTiper V1.0 by Ryokou");
	g_hwMain = hw;
	//apply patch....
	patchcode();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
extc int _export cdecl ODBG_Plugindata(char *shortname)
{
	strcpy(shortname,"OllyTiper");       // Name of plugin
	return PLUGIN_VERSION;
}

//////////////////////////////////////////////////////////////////////////
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
	CString s;
	
	switch (origin)
	{
    case PM_MAIN:                      // Plugin menu in main window
		strcpy(data,"0 &About");
		break;
	default:
		break;
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////////
extc void _export cdecl  ODBG_Pluginaction(int origin,int action,void *item)
{
	switch(origin)
	{
		case PM_MAIN:
			switch(action)
			{
				case 0:
						MessageBoxA(0,"__Be Your Personnal Best__\nSpecial Thx:KanXue,HeXer\nUpdate:09.12.13 DhtFish[CCG]  ","Ryokou@inbox.com(Chs)",MB_OK);
						break;
				default: break;
			}
	}
}
//////////////////////////////////////////////////////////////////////////
extc int _export cdecl  _ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item)
{
	char* addr[4];
	if(origin == PM_DISASM)
	{
		switch(key)
		{
		case 0x58: //X key，copy the address to clipboard
			if (ctrl==TRUE)
			{
				t_dump* tdism;
				tdism=(t_dump *)Plugingetvalue(VAL_CPUDASM);
				//Infoline("%08X",tdism->sel0);
				if (OpenClipboard(NULL))
				{
					if (!EmptyClipboard())
					{
						AfxMessageBox("Cannot empty the Clipboard");
					}
					HANDLE hcopy = GlobalAlloc(GMEM_MOVEABLE,9);
					if (!hcopy)
					{
						CloseClipboard();
					}
					else
					{
						char* hmen=(char*)GlobalLock(hcopy);
						sprintf(hmen,"%08X",tdism->sel0);
						hmen[9]=(TCHAR) 0;
						GlobalUnlock(hcopy); 
						SetClipboardData(CF_TEXT,hcopy);
						Infoline("Address:%08X Copy to Clipboard",tdism->sel0);
						CloseClipboard();
					}
					
				}
				else
					Infoline("Cannot open the Clipboard");
			}
			
		default:
			break;
		}
		return 1;
	}
	else 
		return 0;
}
//////////////////////////////////////////////////////////////////////////
//Our custom Tablefunction
void MyTablefunction(t_table *pt,HWND hw,UINT msg,WPARAM wParam,LPARAM lParam)
{
	char* addr[4];
	tf_sorted=pt->data;
	t_dump* t_diasm;
	t_dump* t_stack;
	t_dump* cpudump;
	//t_reg*  t_regs;
	//判断是否在数据窗口
	//汉化版一起处理
	if (!lstrcmpi(tf_sorted.name,"CPU dump")||!lstrcmp(tf_sorted.name,"CPU 信息"))
	{
//		if(msg==WM_LBUTTONDBLCLK)
//		{
//			cpudump = (t_dump *)Plugingetvalue(VAL_CPUDDUMP);
//			Setcpu(NULL,cpudump->sel0,NULL,NULL,CPU_NOFOCUS);
//		}
//		else if (msg==WM_MOUSEMOVE)
//		{
//			cpudump = (t_dump *)Plugingetvalue(VAL_CPUDDUMP);
//			if (cpudump->sel1-cpudump->sel0!=0)
//			{
//				Infoline("Start:%08X,End:%08X,Size:0x%0X",cpudump->sel0,cpudump->sel1-1,cpudump->sel1-cpudump->sel0);	
//			}
//		}
//		else 
		if (msg==WM_KEYDOWN)
		{
			if (wParam==VK_SHIFT)
			{
				cpudump = (t_dump *)Plugingetvalue(VAL_CPUDDUMP);
				Readmemory(addr,cpudump->sel0,4,MM_SILENT);
				if (Findmemory((unsigned long)*addr))
				{
					Setcpu(NULL,(unsigned long)*addr,NULL,NULL,CPU_NOFOCUS);
				}
			}
		}
	}
	//判断是否在堆栈窗口
	else if (!lstrcmpi(tf_sorted.name,"CPU stack")||!lstrcmp(tf_sorted.name,"CPU 堆栈"))
	{
		//双击
		if (msg==WM_LBUTTONDBLCLK)
		{
			t_stack=(t_dump *)Plugingetvalue(VAL_CPUDSTACK);
			Readmemory(addr,t_stack->sel0,4,MM_SILENT);
			if (Findmemory((unsigned long)*addr)!=NULL)
			{
				Setcpu(NULL,(unsigned long)*addr,NULL,NULL,CPU_NOFOCUS);
			}
		}
		//SHIFT键
		if (wParam==VK_SHIFT)
		{	
			t_stack=(t_dump *)Plugingetvalue(VAL_CPUDSTACK);
			Readmemory(addr,t_stack->sel0,4,MM_SILENT);
			if (Findmemory((unsigned long)*addr)!=NULL)
			{
				Setcpu(NULL,NULL,(unsigned long)*addr,NULL,CPU_NOFOCUS);
			}
		}

	}

 	else if (!lstrcmpi(tf_sorted.name,"CPU disasm")||!lstrcmp(tf_sorted.name,"CPU 反汇编"))
 	{
		
		t_disasm t_dis;
		int cmdlen,n;
		char cmd[MAXCMDSIZE];
 		if (msg==WM_KEYDOWN)
		{
			if (wParam==VK_SHIFT)
			{
				t_diasm=(t_dump *)Plugingetvalue(VAL_CPUDASM);
				cmdlen=t_diasm->sel1-t_diasm->sel0;
				memset(cmd,0,MAXCMDSIZE);
				Readmemory(&cmd,t_diasm->sel0,cmdlen,MM_SILENT);
				cmd[cmdlen]='\0';
				n=Disasm(cmd,cmdlen,t_diasm->sel0,NULL,&t_dis,DISASM_ALL,Plugingetvalue(VAL_MAINTHREADID));
				//Infoline("%0x,%0x,%0x,%0x,%0x,%0x",t_dis.opaddr[0],t_dis.opaddr[1],t_dis.opaddr[2],t_dis.opdata[0],t_dis.opdata[1],t_dis.opdata[2]);
				if (n!=0)
				{
					//jmp/call/return
					if (Findmemory(t_dis.jmpaddr))
					{
						Setcpu(NULL,NULL,t_dis.jmpaddr,NULL,CPU_NOFOCUS);
					}
					//push xxxxx,d xxxxxx
 					else if(Findmemory(t_dis.immconst))
					{
						Setcpu(NULL,NULL,t_dis.immconst,NULL,CPU_NOFOCUS);
					}
					//mov eax,[xxxxx],d xxxxxx
					else if(Findmemory(t_dis.adrconst))
					{
						Setcpu(NULL,NULL,t_dis.adrconst,NULL,CPU_NOFOCUS);
					}
					//mov eax, [esp+10],d [esp+10]
					else if (Findmemory(t_dis.opaddr[0]))
					{
						Setcpu(NULL,NULL,t_dis.opaddr[0],NULL,CPU_NOFOCUS);
					}
				}
			}
			if (wParam==VK_CONTROL)
			{
				t_diasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
				if (t_diasm->sel1-t_diasm->sel0!=0)
				{
					Infoline("Start:%08X,End:%08X,Size:0x%0X",t_diasm->sel0,t_diasm->sel1-1,t_diasm->sel1-t_diasm->sel0);	
				}
			}
		}
 	}
	//CPU窗口
	else if (!lstrcmpi(tf_sorted.name,"CPU 寄存器"))
 	{
		if (msg==WM_KEYDOWN)
		{
			if (wParam==VK_SHIFT)
			{
				DWORD JI;
				__asm{
						push	eax
						mov     eax, dword ptr ds:[0x4D3384]
						and		eax, 0xF
						mov		JI,eax
						pop		eax
				}			
				t_thread* pcputhread = Findthread(Getcputhreadid()); 
				Setcpu(NULL,NULL,(unsigned long)pcputhread->reg.r[JI],NULL,CPU_NOFOCUS);
				Infoline("当前寄存器值:%08X",pcputhread->reg.r[JI]);
			}
		}
		else if (msg==WM_RBUTTONDOWN)
		{
			if (wParam==MK_RBUTTON)
			{
				//TODO........
				//AfxMessageBox(tf_sorted.name);
			}		
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// COllyTiperApp

BEGIN_MESSAGE_MAP(COllyTiperApp, CWinApp)
	//{{AFX_MSG_MAP(COllyTiperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COllyTiperApp construction

COllyTiperApp::COllyTiperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COllyTiperApp object

COllyTiperApp theApp;


