////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                      COMMAND LINE PLUGIN FOR OLLYDBG                       //
//                                                                            //
//                  (Plugin window and interface to OllyDbg)                  //
//                                                                            //
//                                                                            //
//                     Originally written by Oleh Yuschuk                     //
//                                                                            //
//                               Contributors:                                //
//                                    TBD                                     //
//                                                                            //
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
// own programs and redistribute modified code.                               //
//                                                                            //
// Any contributions are welcome. Please send them to Ollydbg@t-online.de     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR! THIS IS OLLYDBG'S DEFAULT!

#define STRICT                         // Avoids some type mismatches
#include <windows.h>
#include <stdio.h>
#include "plugin.h"

#define VERSIONHI      1               // High plugin version
#define VERSIONLO      10              // Low plugin version

#define DX             370             // Width of command line window
#define DY             130             // Height of command line window

#define TAG_CMDLINE    0x6C6D430AL     // Command line record type in .udd file

#define ID_HWBOX       1001            // Identifier of hwbox
#define ID_HWERR       1002            // Identifier of hwerr

#define NHIST          32              // Number of commands in the history

static HINSTANCE hinst;                // DLL instance
HWND      hwmain;               // Handle of main OllyDbg window
static HWND      hwcmd;                // Command line window
static HWND      hwbox;                // Combobox with edit and history list
static HWND      hwedit;               // Edit control inside the combobox
static WNDPROC   oldhweditproc;        // Original window procedure of hwedit
static HWND      hwerr;                // Error message

static char      cmdlinewinclass[32];  // Name of command line window class

static int       posx;                 // Screen X position of window
static int       posy;                 // Screen Y position of window
static char      hist[NHIST][TEXTLEN]; // Previously entered commands
static int       nhist;                // Number of commands in history
static int       poponstop;            // Foreground on pause

int Execute(char *text,char *answer);

// Inserts line into the first position in history and update combobox. If
// exactly the same line is already in the list, old copy will be deleted. If
// resulting number of lines exceeds NHIST, last line will be deleted. If
// text is NULL, function clears history, if "" - redraws combobox.
static void Addline(char *text) {
  int i;
  if (text==NULL) {
    nhist=0;                           // Request to clear history
    SetWindowText(hwerr,""); }         // Clear error message
  else if (text[0]!='\0') {
    // Remove identical strings from the history.
    for (i=nhist-1; i>=0; i--) {
      if (strcmp(hist[i],text)==0) {
        nhist--;
        if (i<nhist) memmove(hist[i],hist[i+1],(nhist-i)*TEXTLEN);
      };
    };
    // Insert new string.
    if (nhist>=NHIST) nhist=NHIST-1;
    memmove(hist[1],hist[0],nhist*TEXTLEN);
    strcpy(hist[0],text);
    nhist++;
  };
  // Copy history to combobox.
  if (hwcmd!=NULL) {
    SendMessage(hwbox,CB_RESETCONTENT,0,0);
    for (i=0; i<nhist; i++)
      SendMessage(hwbox,CB_ADDSTRING,0,(LPARAM)hist[i]);
    if (text!=NULL && nhist>0) {
      SetWindowText(hwbox,text);
      SendMessage(hwbox,CB_SETEDITSEL,0,TEXTLEN);
    };
  };
};

// Subclassed window procedure of edit control in combobox. Processes return
// (execute) and escape (clear line).
LRESULT CALLBACK Editsubclass(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
  char s[TEXTLEN],answer[TEXTLEN];
  if (msg==WM_KEYDOWN) {
    switch (wp) {
      case VK_RETURN:
        GetWindowText(hwbox,s,TEXTLEN);
        if (Execute(s,answer)==0)
          Addline(s);                  // Add line only if command valid
        SetWindowText(hwerr,answer);   // Display result or error message
        SetForegroundWindow(hwcmd);    // Return focus to command window
        SetFocus(hwbox);
        return 0;
      case VK_ESCAPE:
        SetWindowText(hwbox,"");       // Clear edit text and message
        SetWindowText(hwerr,"");
        return 0;
      default: break;
    };
  };
  return CallWindowProc(oldhweditproc,hw,msg,wp,lp);
};

// Each window class needs its own window procedure. Command line window is
// neither a table nor MDI, we don't need to call Tablefunction().
LRESULT CALLBACK Cmdlinewinproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
  RECT rc;
  PAINTSTRUCT ps;
  HBRUSH hbr;
  HDC dc;
  switch (msg) {
    case WM_DESTROY:
      hwcmd=NULL;
      break;
    case WM_SETFOCUS:
      SetFocus(hwbox);
      break;
    case WM_CLOSE:
      // Get current position of command line window to restore it later in
      // the session, and save it to .ini file dependless whether it must be
      // restored later or not.
      GetWindowRect(hw,&rc);
      posx=rc.left;
      posy=rc.top;
      Pluginwriteinttoini(hinst,"Command line window X",rc.left);
      Pluginwriteinttoini(hinst,"Command line window Y",rc.top);
      return DefWindowProc(hw,msg,wp,lp);
    case WM_PAINT:
      // Fill background of command line window with default button colour.
      // Necessary because Registerpluginclass() sets hbrBackground to NULL.
      dc=BeginPaint(hw,&ps);
      GetClientRect(hw,&rc);
      hbr=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
      FillRect(dc,&rc,hbr);
      DeleteObject(hbr);
      EndPaint(hw,&ps);
      break;
    default: return DefWindowProc(hw,msg,wp,lp);
  };
  return 0;
};

// Creates command line window or, if window already exists, brings it to top.
// Returns handle of the window or NULL on error. Corresponding class is
// already registered.
static HWND Createcmdlinewindow(void) {
  HFONT hf;
  RECT rc;
  POINT pt;
  if (hwcmd==NULL) {                   // Window is not yet here, create new.
    // Create main command line window.
    hwcmd=CreateWindow(
      cmdlinewinclass,"Command line",
      WS_POPUPWINDOW|WS_CAPTION|WS_VISIBLE|DS_3DLOOK,
      posx,posy,DX,DY,
      hwmain,NULL,(HINSTANCE)Plugingetvalue(VAL_HINST),NULL);
    if (hwcmd==NULL) return NULL;      // Oops!..
    GetClientRect(hwcmd,&rc);
    // Create edit control with history list, assign default OllyDbg font and
    // limit length of entered commands to OllyDbg-wide default.
    hwbox=CreateWindow(
      "COMBOBOX","",
      WS_CHILD|WS_TABSTOP|WS_BORDER|WS_VISIBLE|WS_VSCROLL|CBS_SIMPLE|
      CBS_HASSTRINGS|CBS_NOINTEGRALHEIGHT|CBS_AUTOHSCROLL|CBS_DISABLENOSCROLL,
      5,5,rc.right-10,rc.bottom-32,
      hwcmd,(HMENU)ID_HWBOX,(HINSTANCE)Plugingetvalue(VAL_HINST),NULL);
    hf=((HFONT *)Plugingetvalue(VAL_FONTS))[Plugingetvalue(VAL_DEFFONT)];
    SendMessage(hwbox,WM_SETFONT,(WPARAM)hf,1);
    SendMessage(hwbox,CB_LIMITTEXT,TEXTLEN-1,1);
    // To intercept return key, subclass the edit control inside the combobox.
    // Don't ask me whether this code always works - the trick is from the MS
    // documentation. I _really_ hate Windows!
    pt.x=pt.y=1;
    hwedit=ChildWindowFromPoint(hwbox,pt);
    oldhweditproc=(WNDPROC)SetWindowLong(hwedit,GWL_WNDPROC,(long)Editsubclass);
    // Create text control displaying error messages.
    hwerr=CreateWindow(
      "STATIC","",
      WS_CHILD|WS_VISIBLE|SS_LEFT|SS_SUNKEN,
      5,rc.bottom-22,rc.right-10,17,
      hwcmd,(HMENU)ID_HWERR,(HINSTANCE)Plugingetvalue(VAL_HINST),NULL);
    hf=((HFONT *)Plugingetvalue(VAL_FONTS))[SYSFONT];
    SendMessage(hwerr,WM_SETFONT,(WPARAM)hf,1);
    Addline("");                       // Refill combobox
  };
  // Bring window to top
  SetForegroundWindow(hwcmd);
  SetFocus(hwbox);
  return hwcmd;
};

// Entry point into a plugin DLL. Many system calls require DLL instance
// which is passed to DllEntryPoint() as one of parameters. Remember it.
BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;                          // Mark plugin instance
  return 1;                            // Report success
};

// Report plugin name and return version of plugin interface.
extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"Command line");    // Name of command line plugin
  return PLUGIN_VERSION;
};

// OllyDbg calls this obligatory function once during startup. I place all
// one-time initializations here. Parameter features is reserved for future
// extentions, do not use it.
extc int _export cdecl ODBG_Plugininit(
  int ollydbgversion,HWND hw,ulong *features) {
  int maxx,maxy;
  // This plugin uses all the newest features, check that version of OllyDbg is
  // correct. I will try to keep backward compatibility at least to v1.99.
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  // Keep handle of main OllyDbg window. This handle is necessary, for example,
  // to display message box.
  hwmain=hw;
  // Register window class for command line window - correct although this
  // window is not a table. Note that formally this class belongs to instance
  // of main OllyDbg program and uses standard plugin icon.
  if (Registerpluginclass(cmdlinewinclass,NULL,hinst,Cmdlinewinproc)<0)
    return -1;
  // Report plugin in the log window.
  Addtolist(0,0,"Command line plugin v1.10");
  Addtolist(0,-1,"  Written by Oleh Yuschuk");
  // Get last position of command line window from .ini file and assure that
  // window is completely visible!
  posx=Pluginreadintfromini(hinst,"Command line window X",CW_USEDEFAULT);
  posy=Pluginreadintfromini(hinst,"Command line window Y",CW_USEDEFAULT);
  maxx=GetSystemMetrics(SM_CXSCREEN)-DX;
  if (posx>maxx) posx=maxx; if (posx<0) posx=0;
  maxy=GetSystemMetrics(SM_CYSCREEN)-DY;
  if (posy>maxy) posy=maxy; if (posy<0) posy=0;
  // If restoring of windows on startup is requested and Command line window
  // was open, restore it now.
  if (Plugingetvalue(VAL_RESTOREWINDOWPOS)!=0 &&
    Pluginreadintfromini(hinst,"Restore command line window",0)!=0)
    Createcmdlinewindow();
  // Confirm success.
  return 0;
};

// This function is called each time OllyDbg passes main Windows loop. When
// debugged application stops, bring command line window in foreground.
extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent) {
  t_status status;
  if (hwcmd!=NULL) {
    status=Getstatus();
    if (status==STAT_NONE || status==STAT_RUNNING)
      poponstop=1;
    else if (poponstop!=0 && (status==STAT_STOPPED || status==STAT_FINISHED)) {
      SetForegroundWindow(hwcmd);
      SetFocus(hwbox);
      poponstop=0;
    };
  };
};

// Save command history to .udd file. History data is project-related.
extc void _export cdecl ODBG_Pluginsaveudd(t_module *pmod,int ismainmodule) {
  int i;
  if (ismainmodule==0)
    return;                            // Save history to main file only
  // When restoring from the .udd, last saved history line will become first
  // in the history list.
  for (i=nhist-1; i>=0; i--) {
    Pluginsaverecord(TAG_CMDLINE,strlen(hist[i])+1,hist[i]);
  };
};

// OllyDbg restores data from .udd file.
extc int _export cdecl ODBG_Pluginuddrecord(t_module *pmod,int ismainmodule,
  ulong tag,ulong size,void *data) {
  if (tag!=TAG_CMDLINE)
    return 0;                          // Not a command line history record
  if (ismainmodule==0)
    return 0;                          // History is saved in main file only
  Addline((char *)data);
  return 1;
};

// Function adds items to main OllyDbg menu (origin=PM_MAIN).
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  if (origin!=PM_MAIN)
    return 0;                          // No pop-up menus in OllyDbg's windows
  strcpy(data,"0 &Command line\tAlt+F1|1 &Help,2 &About");
  return 1;
};

// Receives commands from main menu.
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin!=PM_MAIN)
    return;
  switch (action) {
    case 0:                            // "Command line", creates window
      Createcmdlinewindow();
      break;
    case 1:                            // "Help", opens help file
      WinHelp(hwmain,"cmdline.hlp",HELP_CONTENTS,0);
      break;
    case 2:                            // "About", displays plugin info
      MessageBox(hwmain,
        "Command line plugin v1.10\nWritten by Oleh Yuschuk",
        "Command line",MB_OK|MB_ICONINFORMATION);
      break;
    default: break;
  };
};

// Command line window recognizes global shortcut Alt+F1.
extc int _export cdecl ODBG_Pluginshortcut(
  int origin,int ctrl,int alt,int shift,int key,void *item) {
  if (ctrl==0 && alt==1 && shift==0 && key==VK_F1) {
    Createcmdlinewindow();
    return 1; };                       // Shortcut recognized
  return 0;                            // Shortcut not recognized
};

// User opens new or restarts current application, clear command line history.
extc void _export cdecl ODBG_Pluginreset(void) {
  poponstop=1;
  Addline(NULL);
};

// OllyDbg calls this optional function when user wants to terminate OllyDbg.
extc int _export cdecl ODBG_Pluginclose(void) {
  RECT rc;
  // For automatical restoring, mark in .ini file whether command line window
  // is still open and save coordinates. (WM_CLOSE is not sent in this case).
  Pluginwriteinttoini(hinst,"Restore command line window",hwcmd!=NULL);
  if (hwcmd!=NULL) {
    GetWindowRect(hwcmd,&rc);
    Pluginwriteinttoini(hinst,"Command line window X",rc.left);
    Pluginwriteinttoini(hinst,"Command line window Y",rc.top); };
  return 0;
};

// OllyDbg calls this optional function once on exit. At this moment, all
// windows created by plugin are already destroyed (and received WM_DESTROY
// messages). Function must free all internally allocated resources, like
// window classes, files, memory and so on.
extc void _export cdecl ODBG_Plugindestroy(void) {
  Unregisterpluginclass(cmdlinewinclass);
};

// OllyDbg calls this optional function each time the execution is paused on
// breakpoint with attached list of commands, separately for each command.
// Function must return 1 if command is processed (in this case it will not
// be passed to other plugins) and 0 otherwise.
extc int _export cdecl ODBG_Plugincmd(int reason,t_reg *reg,char *cmd) {
  char answer[TEXTLEN];
  // Command-line plugin accepts only commands that start with point (.)
  if (cmd==NULL || cmd[0]!='.' || cmd[1]=='\0')
    return 0;
  if (Execute(cmd+1,answer)==0)
    Addline(cmd+1);                    // Add line only if command valid
  if (hwmain!=NULL && hwerr!=NULL)
    SetWindowText(hwerr,answer);       // Display result or error message
  return 1;
};



