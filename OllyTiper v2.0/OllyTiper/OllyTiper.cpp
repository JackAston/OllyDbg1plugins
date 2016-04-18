#include "OllyTiper.h"
#include "Plugin.h"
HINSTANCE hInst;
#include "Settings.h"
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
			push esi
			push edi
			push ecx
			push eax
			push ebx
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
			pop ebx
			pop eax
			pop ecx
			pop edi
			pop esi
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
	for (int i = 0; i < 5; ++i){
		getCtrl(i);
		getShift(i);
		getAlt(i);
		getKey(i);
	}

	Addtolist(NULL,1,"OllyTiper");
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
	switch (origin)
	{
    case PM_MAIN:                      // Plugin menu in main window
		strcpy_s(data,4096,"1 &Settings|0 &About");
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
					MessageBoxA(0,"__Be Your Personnal Best__\n\n"
						"First release: Ryokou@inbox.com(Chs).\n"
						"Special Thx:KanXue,HeXer\n\n"
									"Update: 09.12.13 DhtFish[CCG]\n\n"
									"Totally remade: vptrlx, Jan 2010",
									"About OllyTiper",
						MB_OK);
					break;
				case 1:
					DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SETTINGS), 
						g_hwMain, SettingsDlgProc, NULL);
					break;
				default: 
					break;
			}
	}
}

void ToCpbrd(unsigned int Val){
	if (OpenClipboard(NULL))
	{
		if (!EmptyClipboard())
		{
			Infoline("Cannot empty the Clipboard");
		}
		HANDLE hcopy = GlobalAlloc(GMEM_MOVEABLE,9);
		if (!hcopy)
		{
			CloseClipboard();
		}
		else
		{
			char* hmen=(char*)GlobalLock(hcopy);
			sprintf(hmen,"%08X",Val);
			hmen[9]=(TCHAR) 0;
			GlobalUnlock(hcopy); 
			SetClipboardData(CF_TEXT,hcopy);
			Infoline("%08X Copied to Clipboard",Val);
			CloseClipboard();
		}
		
	}
	else
		Infoline("Cannot open the Clipboard");
}

BOOL OnlyShift = 0, OnlyCtrl = 0, OnlyAlt = 0;
BOOL IsCopy(UINT_PTR key){
	/*if (CopyKey == 0)
		return (OnlyShift == CopyShift &&
			OnlyAlt == CopyAlt &&
			OnlyCtrl == CopyCtrl);
	else
	*/
	if (CopyKey==0 && CopyShift==0 && CopyAlt==0 && CopyCtrl==0)
		return 0;
	return (CopyKey==key &&
				OnlyShift == CopyShift &&
				OnlyAlt == CopyAlt &&
				OnlyCtrl == CopyCtrl);
}
BOOL IsCode(UINT_PTR key){
	/*if (CodeKey == 0)
		return (OnlyShift == CodeShift &&
			OnlyAlt == CodeAlt &&
			OnlyCtrl == CodeCtrl);
	else
	*/
	if (CodeKey==0 && CodeShift==0 && CodeAlt==0 && CodeCtrl==0)
		return 0;
	return (CodeKey==key &&
				OnlyShift == CodeShift &&
				OnlyAlt == CodeAlt &&
				OnlyCtrl == CodeCtrl);
}
BOOL IsData(UINT_PTR key){
	/*if (DataKey == 0)
		return (OnlyShift == DataShift &&
			OnlyAlt == DataAlt &&
			OnlyCtrl == DataCtrl);
	else
	*/	
	if (DataKey==0 && DataShift==0 && DataAlt==0 && DataCtrl==0)
		return 0;
	return (DataKey==key &&
				OnlyShift == DataShift &&
				OnlyAlt == DataAlt &&
				OnlyCtrl == DataCtrl);
}
BOOL IsStack(UINT_PTR key){
	/*if (StackKey == 0)
		return (OnlyShift == StackShift &&
			OnlyAlt == StackAlt &&
			OnlyCtrl == StackCtrl);
	else
	*/	
	if (StackKey==0 && StackShift==0 && StackAlt==0 && StackCtrl==0)
		return 0;
	return (StackKey==key &&
				OnlyShift == StackShift &&
				OnlyAlt == StackAlt &&
				OnlyCtrl == StackCtrl);
}
BOOL IsCI(UINT_PTR key){
	/*if (CIKey == 0)
		return (OnlyShift == CIShift &&
			OnlyAlt == CIAlt &&
			OnlyCtrl == CICtrl);
	else
	*/	
	if (CIKey==0 && CIShift==0 && CIAlt==0 && CICtrl==0)
		return 0;
	return (CIKey==key &&
				OnlyShift == CIShift &&
				OnlyAlt == CIAlt &&
				OnlyCtrl == CICtrl);
}
//Our custom Tablefunction

HHOOK hk;
LRESULT CALLBACK MainHook(int code, WPARAM wPar, LPARAM lParam){
	if (((MSG*)lParam)->message != WM_KEYUP && ((MSG*)lParam)->message != WM_KEYDOWN)
		return CallNextHookEx(hk, code, wPar, lParam);
	char* addr[4];
	t_dump* t_diasm;
	t_dump* t_stack;
	t_dump* cpudump;
	WPARAM wParam =((MSG*)lParam)->wParam;
	if (((MSG*)lParam)->message == WM_KEYDOWN){
		OnlyCtrl |= wParam == VK_CONTROL;
		OnlyAlt |= wParam == VK_MENU;
		OnlyShift |= wParam == VK_SHIFT;
		if (wParam == VK_CONTROL || wParam == VK_MENU || wParam == VK_SHIFT)
			return CallNextHookEx(hk, code, wPar, lParam);
		if (wParam != CodeKey && wParam != DataKey && wParam != StackKey && wParam != CopyKey){
			OnlyShift = OnlyCtrl = OnlyAlt = 0;
			UnhookWindowsHookEx(hk);
			goto unhk;
		}
	}
	UnhookWindowsHookEx(hk);
		
	//__asm __emit 0xCC;
	if (((MSG*)lParam)->message == WM_KEYUP)
		wParam = 0;
	BOOL isCopy = IsCopy(wParam);
	BOOL isDispStack = IsStack(wParam);
	BOOL isDispData = IsData(wParam);
	BOOL isDispCode = IsCode(wParam);
	BOOL isCodeInfo = IsCI(wParam);
	OnlyShift = OnlyCtrl = OnlyAlt = 0;
	//Infoline("%i %i %i %i %i", isCopy, isDispStack, isDispData, isDispCode, isCodeInfo);
	if (!lstrcmpi(tf_sorted.name,"CPU dump"))
	{
		{
			if (isDispData || isDispCode || isDispStack || isCopy)
			{
				cpudump = (t_dump *)Plugingetvalue(VAL_CPUDDUMP);
				Readmemory(addr,cpudump->sel0,4,MM_SILENT);
				if (Findmemory((unsigned long)*addr))
				{
					if (isDispData)
						Setcpu(NULL,NULL,(unsigned long)*addr,NULL,CPU_NOFOCUS);						
					else if (isDispCode)
						Setcpu(NULL,(unsigned long)*addr,NULL,NULL,CPU_NOFOCUS);
					else if (isDispStack)
						Setcpu(NULL,NULL,NULL,(unsigned long)*addr,CPU_NOFOCUS);
				}
				if (isCopy)
					ToCpbrd((unsigned long)*addr);

			}
		}
	}
	else if (!lstrcmpi(tf_sorted.name,"CPU stack"))
	{
		
		{	
			if (isDispData || isDispStack || isCopy || isDispCode)
			{
				t_stack=(t_dump *)Plugingetvalue(VAL_CPUDSTACK);
				Readmemory(addr,t_stack->sel0,4,MM_SILENT);
				if (Findmemory((unsigned long)*addr)!=NULL)
				{
					if (isDispData)
						Setcpu(NULL,NULL,(unsigned long)*addr,NULL,CPU_NOFOCUS);
					else if (isDispStack)
						Setcpu(NULL,NULL,NULL,(unsigned long)*addr,CPU_NOFOCUS);
					else if (isDispCode)
						Setcpu(NULL,(unsigned long)*addr,NULL,NULL,CPU_NOFOCUS);
				}
				if (isCopy)
					ToCpbrd((unsigned long)*addr);
				
			}
		}
	}
 	else if (!lstrcmpi(tf_sorted.name,"CPU disasm"))
 	{
		
		t_disasm t_dis;
		int cmdlen,n;
		char cmd[MAXCMDSIZE+2];
 		{
			if (isDispData || isDispStack)
			{
				t_diasm=(t_dump *)Plugingetvalue(VAL_CPUDASM);
				cmdlen=t_diasm->sel1-t_diasm->sel0;
				if (cmdlen>MAXCMDSIZE)
					cmdlen = MAXCMDSIZE;
				memset(cmd,0,MAXCMDSIZE);
				Readmemory(&cmd,t_diasm->sel0,cmdlen,MM_SILENT);
				cmd[cmdlen]='\0';
				n=Disasm(cmd,cmdlen,t_diasm->sel0,NULL,&t_dis,DISASM_ALL,Plugingetvalue(VAL_MAINTHREADID));
				if (n!=0)
				{
					//jmp/call/return
					if (Findmemory(t_dis.jmpaddr))
					{
						if (isDispData)
							Setcpu(NULL,NULL,t_dis.jmpaddr,NULL,CPU_NOFOCUS);
						else if(isDispStack)
							Setcpu(NULL,NULL,NULL,t_dis.jmpaddr,CPU_NOFOCUS);
					}
					//push xxxxx,d xxxxxx
 					else if(Findmemory(t_dis.immconst))
					{
						if (isDispData)
							Setcpu(NULL,NULL,t_dis.immconst,NULL,CPU_NOFOCUS);
						else if(isDispStack)
							Setcpu(NULL,NULL,NULL,t_dis.immconst,CPU_NOFOCUS);
					}
					//mov eax,[xxxxx],d xxxxxx
					else if(Findmemory(t_dis.adrconst))
					{
						if (isDispData)
							Setcpu(NULL,NULL,t_dis.adrconst,NULL,CPU_NOFOCUS);
						else if(isDispStack)
							Setcpu(NULL,NULL,NULL,t_dis.adrconst,CPU_NOFOCUS);
					}
					//mov eax, [esp+10],d [esp+10]
					else if (Findmemory(t_dis.opaddr[0]))
					{
						if (isDispData)
							Setcpu(NULL,NULL,t_dis.opaddr[0],NULL,CPU_NOFOCUS);
						else if(isDispStack)
							Setcpu(NULL,NULL,NULL,t_dis.opaddr[0],CPU_NOFOCUS);
					}
				}
			}
			else if (isCopy){
				t_diasm=(t_dump *)Plugingetvalue(VAL_CPUDASM);
				ToCpbrd(t_diasm->sel0);
			}
			else if (isCodeInfo)
			{
				t_diasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
				if (t_diasm->sel1-t_diasm->sel0!=0)
				{
					Infoline("Start:%08X,End:%08X,Size:0x%0X",t_diasm->sel0,t_diasm->sel1-1,t_diasm->sel1-t_diasm->sel0);				}
			}
		}
 	}
	else if (!lstrcmpi(tf_sorted.name,"CPU Registers"))
 	{
		{
			if (isDispData || isDispCode || isDispStack || isCopy)
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
				if (Findmemory((unsigned long)pcputhread->reg.r[JI])!=NULL)
				{
					if (isDispData)
						Setcpu(NULL,NULL,(unsigned long)pcputhread->reg.r[JI],NULL,CPU_NOFOCUS);
					else if (isDispCode)
						Setcpu(NULL,(unsigned long)pcputhread->reg.r[JI],NULL,NULL,CPU_NOFOCUS);
					else if (isDispStack)
						Setcpu(NULL,NULL,NULL,(unsigned long)pcputhread->reg.r[JI],CPU_NOFOCUS);
				}
				if(isCopy)
					ToCpbrd((unsigned long)pcputhread->reg.r[JI]);
			}
					
		}
	}
unhk:
	if (code < 0)
		return CallNextHookEx(hk, code, wPar, lParam);
	else
		return 0;
}


void MyTablefunction(t_table *pt,HWND hw,UINT msg,WPARAM wParam,LPARAM lParam)
{
	//t_reg*  t_regs;
	//Infoline("%x", msg);
	char* addr[4];
	t_dump* t_stack;
	tf_sorted=pt->data;
	if (msg != WM_KEYDOWN && msg != WM_LBUTTONDBLCLK)
		return;
	//__asm __emit 0xCC;
	if (msg==WM_LBUTTONDBLCLK){
		if(!lstrcmpi(tf_sorted.name,"CPU stack"))
		{
			t_stack=(t_dump *)Plugingetvalue(VAL_CPUDSTACK);
			Readmemory(addr,t_stack->sel0,4,MM_SILENT);
			if (Findmemory((unsigned long)*addr)!=NULL)
			{
				Setcpu(NULL,(unsigned long)*addr,NULL,NULL,CPU_NOFOCUS);
			}
			
		}
		return;
	}
	if (msg == WM_KEYDOWN){
		switch (wParam){
			case VK_SHIFT:
				OnlyShift = 1;
				break;
			case VK_CONTROL:
				OnlyCtrl = 1;
				break;
			case VK_MENU:
				OnlyAlt = 1;
				break;
			default:
				break;
		}
	}
	hk = SetWindowsHookEx(WH_GETMESSAGE, MainHook, hInst,GetCurrentThreadId());
	
	
	
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInst = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}