////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        SAMPLE PLUGIN FOR OLLYDBG                           //
//                                                                            //
//                   Copyright (C) 2001-2004 Oleh Yuschuk                     //
//                                                                            //
// This plugin allows to set up to 10 code bookmarks using keyboard shortcuts //
// or popup menus in Disassembler and then quickly return to one of the       //
// bookmarks using shortcuts, popup menu or Bookmark window. Bookmarks        //
// are kept between sessions in .udd file.                                    //
//                                                                            //
// This code is distributed "as is", without warranty of any kind, expressed  //
// or implied, including, but not limited to warranty of fitness for any      //
// particular purpose. In no event will Oleh Yuschuk be liable to you for any //
// special, incidental, indirect, consequential or any other damages caused   //
// by the use, misuse, or the inability to use of this code, including any    //
// lost profits or lost savings, even if Oleh Yuschuk has been advised of the //
// possibility of such damages. Or, translated into English: use at your own  //
// risk!                                                                      //
//                                                                            //
// This code is free. You can modify this code, include parts of it into your //
// own programs and redistribute modified code provided that you remove all   //
// copyright messages or, if changes are significant enough, substitute them  //
// with your own copyright.                                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

#include <windows.h>

#include "plugin.h"
#include "resource.h"

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window
char  Name[]="DebugPrivilege";

// Entry point into a plugin DLL. Many system calls require DLL instance
// which is passed to DllEntryPoint() as one of parameters. Remember it.
// Preferrable way is to place initializations into ODBG_Plugininit() and
// cleanup in ODBG_Plugindestroy().

BOOL WINAPI DllMain(HINSTANCE hi,DWORD reason,LPVOID reserved) 
{
  if (reason==DLL_PROCESS_ATTACH)
		hinst=hi;                      // Mark plugin instance
  
  return 1;                            // Report success
}

BOOL AdjustPrivilege(int pid, BOOL bEnable)
{
    HANDLE hProcess;
	HANDLE hToken=0;
    TOKEN_PRIVILEGES tkp;
   
	tkp.PrivilegeCount = 1;  
	tkp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
   
	if( ! LookupPrivilegeValue(NULL, "SeDebugPrivilege", &tkp.Privileges[0].Luid) )
	{
		MessageBox(hwmain, "LookupPrivilegeValue error", Name, MB_OK|MB_ICONERROR);
		return 0;
	}
	
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (! hProcess) 
	{
		MessageBox(hwmain, "OpenProcess error", Name, MB_OK|MB_ICONERROR);
		return 0;
	}
  

  	if( ! OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken) )
	{
		CloseHandle(hProcess);
		MessageBox(hwmain, "OpenProcessToken error", Name, MB_OK|MB_ICONERROR);
		return 0;
	}

    if( ! AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, NULL))
	{
		CloseHandle(hToken);
		CloseHandle(hProcess);
		MessageBox(hwmain, "AdjustTokenPrivileges error", Name, MB_OK|MB_ICONERROR);
		return 0;
	}
    
	
	CloseHandle(hToken);
	CloseHandle(hProcess);
    return 1;
}



BOOL CALLBACK DlgOptionProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int AutoDisable;
	int SleepTime;
	char textbuffer[8];

	if (uMsg == WM_INITDIALOG)
	{
		//read params from ini & update windows
        AutoDisable= Pluginreadintfromini(hinst,"AutoDisable", 0);
        SleepTime  = Pluginreadintfromini(hinst,"Sleeptime",500);
		
        CheckDlgButton(hWnd, IDC_AUTO, AutoDisable);
		
		wsprintf(textbuffer, "%d", SleepTime);
        SetDlgItemText(hWnd,IDC_SLEEPTIME,textbuffer);
       
	}
    
	else if( uMsg == WM_CLOSE)
        EndDialog(hWnd, 0);
    
	else if( uMsg == WM_COMMAND)
	{
  
			if (wParam==(UINT)IDC_QUIT)
                EndDialog(hWnd, 0);
            
			else if( wParam==(UINT)IDC_SAVE)     
			{
                AutoDisable=IsDlgButtonChecked(hWnd, IDC_AUTO);     
                Pluginwriteinttoini(hinst, "AutoDisable", AutoDisable);
               
                GetDlgItemText(hWnd, IDC_SLEEPTIME, textbuffer, 10);
                SleepTime=atoi(textbuffer);
                Pluginwriteinttoini(hinst, "Sleeptime", SleepTime);
			}    
    }
    else 
         return FALSE;
    
    return TRUE;
}




// ODBG_Plugindata() is a "must" for valid OllyDbg plugin. It must fill in
// plugin name and return version of plugin interface. If function is absent,
// or version is not compatible, plugin will be not installed. Short name
// identifies it in the Plugins menu. This name is max. 31 alphanumerical
// characters or spaces + terminating '\0' long. To keep life easy for users,
// this name should be descriptive and correlate with the name of DLL.
extc int _export cdecl ODBG_Plugindata(char shortname[32]) 
{
  strcpy(shortname, Name);       // Name of plugin
  return PLUGIN_VERSION;
}

// OllyDbg calls this obligatory function once during startup. Place all
// one-time initializations here. If all resources are successfully allocated,
// function must return 0. On error, it must free partially allocated resources
// and return -1, in this case plugin will be removed. Parameter ollydbgversion
// is the version of OllyDbg, use it to assure that it is compatible with your
// plugin; hw is the handle of main OllyDbg window, keep it if necessary.
// Parameter features is reserved for future extentions, do not use it.
extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
  // This plugin uses all the newest features, check that version of OllyDbg is
  // correct. I will try to keep backward compatibility at least to v1.99.
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;

  // Keep handle of main OllyDbg window. This handle is necessary, for example,
  // to display message box.
  hwmain=hw;
 
  // Register window class for MDI window that will display plugins. Please
  // note that formally this class belongs to instance of main OllyDbg program,
  // not a plugin DLL. String winclass gets unique name of new class.
  // Keep it to create window and unregister on shutdown.
 

  // Plugin successfully initialized. Now is the best time to report this fact
  // to the log window. To conform OllyDbg look and feel, please use two lines.
  // The first, in black, should describe plugin, the second, gray and indented
  // by two characters, bears copyright notice.
  Addtolist(0,0, "DebugPrivilege plugin v1.0");
  Addtolist(0,-1,"  Copyright (C) 2000-2006 Simonzh2000" );
   
  return 0;
}



// Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup
// menu in one of standard OllyDbg windows. When plugin wants to add own menu
// items, it gathers menu pattern in data and returns 1, otherwise it must
// return 0. Except for static main menu, plugin must not add inactive items.
// Item indices must range in 0..63. Duplicated indices are explicitly allowed.
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) 
{
  
  switch (origin) 
  {
    // Menu creation is very simple. You just fill in data with menu pattern.
    // Some examples:
    // 0 Aaa,2 Bbb|3 Ccc|,,  - linear menu with 3items, relative IDs 0, 2 and 3,
    //                         separator between second and third item, last
    //                         separator and commas are ignored;
    // #A{0Aaa,B{1Bbb|2Ccc}} - unconditional separator, followed by popup menu
    //                         A with two elements, second is popup with two
    //                         elements and separator inbetween.
    case PM_MAIN:                      // Plugin menu in main window
      strcpy(data,"0 Disable,1 Enable,2 Option|3 About");
      // If your plugin is more than trivial, I also recommend to include Help.
      return 1;
    
	case PM_DISASM:                    // Popup menu in Disassembler  
      return 1;
    
	default: break;                    // Any other window
  }
  return 0;                            // Window not supported by plugin
}

// This optional function receives commands from plugin menu in window of type
// origin. Argument action is menu identifier from ODBG_Pluginmenu(). If user
// activates automatically created entry in main menu, action is 0.
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
  if (origin==PM_MAIN) 
  {
    switch (action) 
	{
      case 0:
	      if (AdjustPrivilege(Plugingetvalue(VAL_PROCESSID), FALSE)) 
			  Addtolist(0,-1, "  DebugPrivilege disabled");
		  
		  break;

      case 1:
	      if (AdjustPrivilege(Plugingetvalue(VAL_PROCESSID), TRUE)) 
			  Addtolist(0,-1, "  DebugPrivilege enabled");
          
		  break;

	  case 2:
		  DialogBox(hinst, MAKEINTRESOURCE(IDD_OPTION), hwmain, (DLGPROC)DlgOptionProc);
		  break;
      
	  case 3:
        // Menu item "About", displays plugin info. If you write your own code,
        // please replace with own copyright!
        MessageBox(hwmain,
          "DebugPrivilege plugin v1.0\nCopyright (C) 2000-2006 Simonzh2000",
          Name, MB_OK|MB_ICONINFORMATION);
        break;

      default: break;
    } 
  }
  else if (origin==PM_DISASM) 
  {
 
  }
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	int SleepTime  = Pluginreadintfromini(hinst,"Sleeptime",500);
	int pid=0;

	while(!pid)
	{
		Sleep(SleepTime);
        pid=Plugingetvalue(VAL_PROCESSID);
	}

    if (AdjustPrivilege(pid, FALSE))
	{
		Addtolist(0,-1, "  DebugPrivilege disabled");
		return 0;
	}
	
	return 1;
}
 

// Function is called when user opens new or restarts current application.
// Plugin should reset internal variables and data structures to initial state.
extc void _export cdecl ODBG_Pluginreset(void) 
{
    int AutoDisable= Pluginreadintfromini(hinst,"AutoDisable", 0);  

	if (AutoDisable)
	{
		CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
	}
}


// This function receives possible keyboard shortcuts from standard OllyDbg
// windows. If it recognizes shortcut, it must process it and return 1,
// otherwise it returns 0.
extc int _export cdecl ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item) 
{
  return 0;                            // Shortcut not recognized
}



// OllyDbg calls this optional function when user wants to terminate OllyDbg.
// All MDI windows created by plugins still exist. Function must return 0 if
// it is safe to terminate. Any non-zero return will stop closing sequence. Do
// not misuse this possibility! Always inform user about the reasons why
// termination is not good and ask for his decision!
extc int _export cdecl ODBG_Pluginclose(void) 
{
   return 0;
}

// OllyDbg calls this optional function once on exit. At this moment, all MDI
// windows created by plugin are already destroyed (and received WM_DESTROY
// messages). Function must free all internally allocated resources, like
// window classes, files, memory and so on.
extc void _export cdecl ODBG_Plugindestroy(void) 
{
}


// If you define ODBG_Pluginmainloop, this function will be called each time
// from the main Windows loop in OllyDbg. If there is some debug event from
// the debugged application, debugevent points to it, otherwise it is NULL. Do
// not declare this function unnecessarily, as this may negatively influence
// the overall speed!

/*
extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) 
{
}
*/