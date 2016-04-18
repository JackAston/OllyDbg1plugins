// ODbgScript.cpp : Defines the entry point for the DLL application.
//
//#define STRICT

#define _CHAR_UNSIGNED 1 // huhu

#include "StdAfx.h"

//static HINSTANCE hinst;                // DLL instance
//static HWND      hwmain;               // Handle of main OllyDbg window

// Temp storage
char buff[4096] = {0};

// OllyLang object
OllyLang* ollylang;

// Script state
int script_state = SS_NONE;

// Entry point into a plugin DLL. Many system calls require DLL instance
// which is passed to DllEntryPoint() as one of parameters. Remember it.
BOOL APIENTRY DllMain(HINSTANCE hi,DWORD reason,LPVOID reserved)
{
    switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			//hinst = hi;        // Mark plugin instance
			//no more used (bug with global vars)
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }

	#ifdef _DEBUG
	//RedirectIOToConsole();
	#endif

    return TRUE;
}

// Report plugin name and return version of plugin interface.
extc int _export cdecl ODBG_Plugindata(char shortname[32]) 
{
	strcpy(shortname,"ODbgScript");
	return PLUGIN_VERSION;
}

// OllyDbg calls this obligatory function once during startup. I place all
// one-time initializations here. Parameter features is reserved for future
// extentions, do not use it.
extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw, ulong *features) 
{
	HINSTANCE hinst;

	if(ollydbgversion < PLUGIN_VERSION)
		return -1;

	// Report plugin in the log window.
	Addtolist(0, 0, "ODbgScript v%i.%i",VERSIONHI,VERSIONLO);
	Addtolist(0, -1,"  by Epsylon3@gmail.com from OllyScript 0.92 by SHaG");
	ollylang = new OllyLang();

	if (Createsorteddata(&ollylang->wndProg.data,"ODbgScript Data", 
		sizeof(t_wndprog_data),50, (SORTFUNC *)wndprog_sort_function,NULL)!=0)
			return -1; 

	if (Createsorteddata(&ollylang->wndLog.data,"ODbgScript Log", 
	sizeof(t_wndlog_data),20, (SORTFUNC *)wndlog_sort_function,NULL)!=0)
		return -1;

	hinst = hinstModule();

	if (Registerpluginclass(wndprogclass,NULL,hinst,wndprog_winproc)<0) {
		return -1;
	}
	if (Registerpluginclass(wndlogclass,NULL,hinst,wndlog_winproc)<0) {
		return -1;
	}
	if (Plugingetvalue(VAL_RESTOREWINDOWPOS)!=0 && Pluginreadintfromini(hinst,"Restore Script Log",0)!=0)
		initLogWindow();

	if (Plugingetvalue(VAL_RESTOREWINDOWPOS)!=0 && Pluginreadintfromini(hinst,"Restore Script window",0)!=0)
		initProgTable();

	return 0;
}

// This function is called each time OllyDbg passes main Windows loop. When
// debugged application stops, bring command line window in foreground.
extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) 
{	
	t_status status; 
	status = Getstatus();
	script_state = ollylang->GetState();
	
	// Check for breakpoint jumps
	if(script_state == SS_RUNNING && debugevent && debugevent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
	{
		EXCEPTION_DEBUG_INFO edi = debugevent->u.Exception;
		if(edi.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT)
			ollylang->OnBreakpoint();
		else if(edi.ExceptionRecord.ExceptionCode != EXCEPTION_SINGLE_STEP)
			ollylang->OnException(edi.ExceptionRecord.ExceptionCode);
		else
			
			if(script_state == SS_RUNNING)
			{
				t_thread* t;
				t = Findthread(Getcputhreadid());
				CONTEXT context;
				context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
				GetThreadContext(t->thread, &context);

				if(t->reg.ip == context.Dr0 || t->reg.ip == context.Dr1 || t->reg.ip == context.Dr2 || t->reg.ip == context.Dr3) {
					ollylang->OnBreakpoint();
				}

			}
	}

	if(status == STAT_STOPPED && (script_state == SS_RUNNING || script_state == SS_LOADED))
	{

		try
		{
			ollylang->Step(0);
			script_state = ollylang->GetState();
		}
		catch( ... )
		{
			delete ollylang;
			MessageBox(hwndOllyDbg(), "An error occured in the plugin!\nPlease contact Epsylon3.", "ODbgScript", MB_OK | MB_ICONERROR | MB_TOPMOST);
		}

	}
	//Refocus script windows (ex: when using "Step")
	if (ollylang->wndProg.hw && status == STAT_STOPPED && script_state != SS_RUNNING) {
		if (focusonstop>0) { 
			SetForegroundWindow(ollylang->wndProg.hw);
			SetFocus(ollylang->wndProg.hw);
			focusonstop--;
		}
	}	

}

// Function adds items to main OllyDbg menu (origin=PM_MAIN).
extc int _export cdecl ODBG_Pluginmenu(int origin, char data[4096], void *item) 
{
	switch (origin) {
		
	case PM_MAIN:
	//MRU Menu content, actions are 21..25
		ZeroMemory(buff, sizeof(buff));
		strcpy(buff, "Run &Script{0 Open...|");
		mruGetMenu(&buff[strlen(buff)]);

		strcpy(&buff[strlen(buff)],

		"}|"
		"1 Abort"
		",2 Pause"
		",3 Resume"
		",4 Step"
		"|"
		"30 Script &Window..."
		",31 Script &Log..."
		"|"
		"10 &About"
//		",11 TEST"
//		",12 TEST2"
		);

		strcpy(data,buff);
		return 1;

	case PM_DISASM:
		ZeroMemory(buff, sizeof(buff));
		strcpy(buff, "Run Scri&pt{0 Open...|");
		mruGetCurrentMenu(&buff[strlen(buff)]);

		strcpy(&buff[strlen(buff)],"}");
		strcpy(data,buff);
		return 1;

	}
	return 0; // No pop-up menus in OllyDbg's windows
}

extc int _export cdecl ExecuteScript(const char* const filename)
{
	ollylang->LoadScript((LPSTR)filename);
	ollylang->Step(0);
	return 0;
}

extc int _export cdecl ODBG_Pausedex(int reasonex, int dummy, t_reg* reg, DEBUG_EVENT* debugevent)
{
	EXCEPTION_DEBUG_INFO edi;
	if(debugevent)
		edi = debugevent->u.Exception;
	script_state = ollylang->GetState();

	// cout << hex << reasonex << endl;

	// Handle events
	if(script_state == SS_RUNNING)
	{
		switch(reasonex) 
		{
		case PP_INT3BREAK:
		case PP_HWBREAK:
		case PP_MEMBREAK:
			ollylang->OnBreakpoint();
			break;
		case PP_EXCEPTION:
		case PP_ACCESS:
		case PP_GUARDED:
		case PP_SINGLESTEP | PP_BYPROGRAM:
			ollylang->OnException(edi.ExceptionRecord.ExceptionCode);
			break;
		}
	}
	// Step script
//	if(script_state == SS_RUNNING || script_state == SS_LOADED)
//	{
//		try
//		{
//			Broadcast(WM_USER_CHALL, 0, 0);
//			ollylang->Step(0);
//			script_state = ollylang->GetState();
//		}
//		catch( ... )
//		{
//			delete ollylang;
//			MessageBox(hwmain, "An error occured in the plugin!\nPlease contact SHaG.", "OllyScript", MB_OK | MB_ICONERROR | MB_TOPMOST);
//		}
//	}
	return 0;
}

// Receives commands from main menu.
extc void _export cdecl ODBG_Pluginaction(int origin, int action, void *item) 
{
  if (origin != PM_MAIN && origin != PM_DISASM)
    return;

  char s[256];
  HINSTANCE hinst  = hinstModule();
  HWND      hwmain = hwndOllyDbg();
  switch (action) 
  {
	case 0: // Run script
		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwmain;
		ofn.lpstrFile = szFile; 
		//
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		//
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Olly Scripts\0*.osc;*.txt\0All\0*.*\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		Pluginreadstringfromini(hinst, "ScriptDir", buff, 0);
		ofn.lpstrInitialDir = buff;
		ofn.lpstrTitle = "Select Script";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 
		if (GetOpenFileName(&ofn)==TRUE)
		{
			// Load script
			ollylang->LoadScript(ofn.lpstrFile);
			// Save script directory
			
			// Start script
			ollylang->Resume();
		}
		break;

	case 1: // Abort
		ollylang->Reset(); 
		MessageBox(hwmain,"Script aborted!","ODbgScript",MB_OK|MB_ICONEXCLAMATION);
		ollylang->Pause();
		break;

	case 2: // Pause
		ollylang->Pause();
		break;

	case 3: // Resume
		ollylang->Resume();
		break;

	case 4: // Step
		ollylang->Step(1);
		script_state = ollylang->GetState();
		break;

    case 10:
		sprintf(s,"ODbgScript plugin v%i.%i\n"
			      "by Epsylon3@gmail.com\n\n"
				  "From OllyScript 0.92 written by SHaG\n"
				  "PE dumper by R@dier\n"
				  "Byte replacement algo by Hex\n\n"
				  "http://ollydbg.win32asmcommunity.net \n\n"
				  "Compiled %s %s",
			VERSIONHI,VERSIONLO, __DATE__, __TIME__);
		MessageBox(hwmain,s,"ODbgScript",MB_OK|MB_ICONINFORMATION);
		break;
	case 21: // MRU List
	case 22:
	case 23:
	case 24:
	case 25:
		{
			action-=20; 
			char key[5]="MRU ";
			key[3]=action+0x30;
						
			ZeroMemory(&buff, sizeof(buff));
			Pluginreadstringfromini(hinst,key,buff,0);

			// Load script
			ollylang->LoadScript(buff);

			mruAddFile(buff);
 
			// Save script directory
			char* buf2;
			GetFullPathName(buff,sizeof(buff),buff,&buf2); *buf2=0;			
			Pluginwritestringtoini(hinst, "ScriptDir", buff);

			ollylang->Resume();
			break;
		}
	case 30:
		{
			initProgTable();
			break;
		}
	case 31:
		{
			initLogWindow();
			break;
		}
	case 11:
		{
//			string x = "Hej";
//			string y = ToLower(x);
//			__asm nop;
		}
	case 12:
		{
//			Broadcast(WM_USER_CHALL, 0, 0);
		}
//			t_thread* thr = Findthread(Getcputhreadid());
//			byte buffer[4];
//			ulong fs = thr->reg.limit[2]; // BUG IN ODBG!!!
//			fs += 0x30;
//			Readmemory(buffer, fs, 4, MM_RESTORE);
//			fs = *((ulong*)buffer);
//			fs += 2;
//			buffer[0] = 0;
//			Writememory(buffer, fs, 1, MM_RESTORE);
//			cout << endl;
		
//			ulong addr = t->reg.s[SEG_FS];
//			Readmemory(buffer, addr, 4, MM_RESTORE);
//			cout << hex << &buffer;

			/*
			HMODULE hMod = GetModuleHandle("OllyScript.dll");
			if(hMod) // Check that the other plugin is present and loaded
			{
				// Get address of exported function
				int (*pFunc)(char*) = (int (*)(char*)) GetProcAddress(hMod, "ExecuteScript");
				if(pFunc) // Check that the other plugin exports the correct function
					pFunc("xxx"); // Execute exported function
			}

			cout << hex << hMod << endl;*/
			//403008 401035
			/*DWORD pid = Plugingetvalue(VAL_PROCESSID);
			DebugSetProcessKillOnExit(FALSE);
			DebugActiveProcessStop(pid);
			break;*/
			//t_module* mod = Findmodule(0x401000);
			//cout << hex << mod->codebase;
			
			//cout << hex << mod->codebase;



			/*char cmd[MAXCMDSIZE] = {0};
		//Readmemory(cmd, 0x401030, 1, 0);

		t_memory* tmem = Findmemory(0x412fc0);
		ulong addr = 0x412fc0;

		do 
		{
			addr = Disassembleforward(0, tmem->base, tmem->size, addr, 1, 0); 
			Readcommand(addr, cmd);
		} while(cmd[0] != 0x61);

		Manualbreakpoint(addr, VK_F2, 0, 0, FIXEDFONT);
		Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F9);

		//Disasm((uchar*)cmd, sizeof(cmd), 0x412fc0, 0, &d, DISASM_CODE, 0);
		//ulong x = 
		t_disasm d;
		DWORD r = cmd[0] << (8 * 3);// + cmd[1] << (8 * 2);// + cmd[2] << 8 + cmd[3];
		r += cmd[1] << (8 * 2);
		r += cmd[2] << 8;
		r += cmd[3];
		cerr << cmd << endl;
		*/
		break;

    default: 
		break;
  }
}

extc int ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item) {

	switch (origin) {
		case PM_MAIN:

			break; //This function is usually called twice

/*
PM_MAIN	item is always NULL	Main window
PM_DUMP	(t_dump *)	Any Dump window
PM_MODULES	(t_module *)	Modules window
PM_MEMORY	(t_memory *)	Memory window
PM_THREADS	(t_thread *)	Threads window
PM_BREAKPOINTS	(t_bpoint *)	Breakpoints window
PM_REFERENCES	(t_ref *)	References window
PM_RTRACE	(int *)	Run trace window
PM_WATCHES	(1-based index)	Watches window
PM_WINDOWS	(t_window *)	Windows window
PM_DISASM	(t_dump *)	CPU Disassembler
PM_CPUDUMP	(t_dump *)	CPU Dump
PM_CPUSTACK	(t_dump *)	CPU Stack
PM_CPUREGS	(t_reg *)	CPU Registers
*/
		default:
			//if (key==VK_F8 && shift==0 && ctrl==0) {
#ifdef _DEBUG
			char* data = new char[256];
			sprintf(data,"ODBG_Pluginshortcut %d %d",origin,key);
			Addtolist(0, -1, data );
			delete[] data;
			return 0;
#endif
;
	}
	return 0;

}

// OllyDbg calls this optional function when user restarts debugged app
extc void _export cdecl ODBG_Pluginreset()
{	
	//we keep the script state on restart (paused or not)
	if (ollylang->GetState() == SS_PAUSED) {
		ollylang->Reset();
		ollylang->Pause();
	} 
	else 
		ollylang->Reset();
}

// OllyDbg calls this optional function when user wants to terminate OllyDbg.
extc int _export cdecl ODBG_Pluginclose() 
{
	Pluginwriteinttoini(hinstModule(),"Restore Script window",(ollylang->wndProg.hw!=NULL));
	Pluginwriteinttoini(hinstModule(),"Restore Script Log",(ollylang->wndLog.hw!=NULL));
	return 0;
}

// OllyDbg calls this optional function once on exit. At this moment, all
// windows created by plugin are already destroyed (and received WM_DESTROY
// messages). Function must free all internally allocated resources, like
// window classes, files, memory and so on.
extc void _export cdecl ODBG_Plugindestroy()
{
	delete ollylang;
	Unregisterpluginclass(wndprogclass);
	Unregisterpluginclass(wndlogclass);
}


#include "Progress.cpp"
#include "LogWindows.cpp"
