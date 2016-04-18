////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                          OllyStepNSearch plugin                            //
//                                                                            //
//                   2006 Didier Stevens https://DidierStevens.com            //
//                                                                            //
// This plugin allows you to search for a given text when automatically       //
// stepping through the debugged program.                                     //
//                                                                            //
// This code is in the public domain, there is no Copyright.                  //
// Use at your own risk.                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

// History:
//  19/08/2006 no version number	Development started
//	26/08/2006 version 0.5.0			Development resumed
//	22/09/2006 version 0.6.0			Added options dialog
//	23/09/2006 version 0.6.0			Continued options dialog, added Disable After Break option
//	24/09/2006 version 0.6.0			Added Search in Information Pane functionality, replaced help function
//	12/11/2006 version 0.6.1			Corrected bugs in HlpDlgProc, thanks to Ngan Truong for reporting them

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <commctrl.h>
#include <malloc.h>

#include "plugin.h"

#include "resource.h"

// Fields used by the plugin in OllyDbg.ini
#define INI_VERSION "Version"
#define INI_SEARCH "Search"
#define INI_STEPINTOLIMIT "StepIntoLimit"
#define INI_DISABLEAFTERBREAK "DisableAfterBreak"
#define INI_SEARCHINREGISTERS "SearchInRegisters"
#define INI_SEARCHININFORMATION "SearchInInformation"

#define URL "https://DidierStevens.com"
#define VERSION "0.6.1"

HINSTANCE	hiDLL;												// DLL instance
HWND			hwMain;												// Handle of main OllyDbg window
int				iFlagEnableStepSearch;				// Flag, true when the user wants to enable the OllyStepNSearch plugin
ulong			ulStepInLimit;								// Minimum address, we don't want to StepSearch from this address on and step in
char			achSearchText[TEXTLEN];				// Text the user wants to search for
int				iFlagSearchForSearchText;			// Flag, true when we want to search for text and break when found
ulong			ulRegPrev[8];									// Array to store the previous value of the registers
int				iFlagRegPrevNotInitialized;		// Flag, true when ulRegPrev contains no data
int				iStep;												// Indicates the step we take (step in / step over)
char			*aszRegName[] = 							// Names of the registers
					{"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
int				iFlagDisableAfterBreak;				// Flag, when true, OllyStepNSearch is disabled after a break on the search string
int				iFlagSearchInRegisters;				// Flag, true when we want to search the search string in strings pointed to by registers
int				iFlagSearchInInformation;			// Flag, true when we want to search the search string in the information pane

LRESULT CALLBACK OptDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK HlpDlgProc(HWND, UINT, WPARAM, LPARAM);
ulong ParseStepInLimit(char *);
void CenterDialog(HWND);

// Load settings from OllyDbg.ini and define default values when settings not set in INI file
void LoadFromOllyDbgIni(void)
{
	char achBuf[TEXTLEN];

	Pluginreadstringfromini(hiDLL, INI_SEARCH, achSearchText, "");
	iFlagSearchForSearchText = strcmp(achSearchText, "") == 1;
	Pluginreadstringfromini(hiDLL, INI_STEPINTOLIMIT, achBuf, "0x10000000");
	ulStepInLimit = ParseStepInLimit(achBuf);
	iFlagDisableAfterBreak = Pluginreadintfromini(hiDLL, INI_DISABLEAFTERBREAK, 0);
	iFlagSearchInRegisters = Pluginreadintfromini(hiDLL, INI_SEARCHINREGISTERS, 1);
	iFlagSearchInInformation = Pluginreadintfromini(hiDLL, INI_SEARCHININFORMATION, 0);

  Addtolist(0, 0, "OllyStepNSearch achSearchText = %s", achSearchText);
  Addtolist(0, 0, "OllyStepNSearch ulStepInLimit = 0x%08lX", ulStepInLimit);
  Addtolist(0, 0, "OllyStepNSearch iFlagDisableAfterBreak = %d", iFlagDisableAfterBreak);
  Addtolist(0, 0, "OllyStepNSearch iFlagSearchInRegisters = %d", iFlagSearchInRegisters);
  Addtolist(0, 0, "OllyStepNSearch iFlagSearchInInformation = %d", iFlagSearchInInformation);
}

void SaveToOllyDbgIni(void)
{
  char achBuf[TEXTLEN];

	Pluginwritestringtoini(hiDLL, INI_VERSION, VERSION);
	Pluginwritestringtoini(hiDLL, INI_SEARCH, achSearchText);
	sprintf(achBuf, "0x%X", ulStepInLimit);
	Pluginwritestringtoini(hiDLL, INI_STEPINTOLIMIT, achBuf);
	Pluginwriteinttoini(hiDLL, INI_DISABLEAFTERBREAK, iFlagDisableAfterBreak);
	Pluginwriteinttoini(hiDLL, INI_SEARCHINREGISTERS, iFlagSearchInRegisters);
	Pluginwriteinttoini(hiDLL, INI_SEARCHININFORMATION, iFlagSearchInInformation);
}

// Entry point into a plugin DLL. Many system calls require DLL instance
// which is passed to DllEntryPoint() as one of parameters. Remember it.
// Preferrable way is to place initializations into ODBG_Plugininit() and
// cleanup in ODBG_Plugindestroy().
BOOL WINAPI DllEntryPoint(HINSTANCE hi, DWORD reason, LPVOID reserved) {
  if (reason == DLL_PROCESS_ATTACH)
    hiDLL=hi;                          // Mark plugin instance
  return 1;                            // Report success
};

// ODBG_Plugindata() is a "must" for valid OllyDbg plugin. It must fill in
// plugin name and return version of plugin interface. If function is absent,
// or version is not compatible, plugin will be not installed. Short name
// identifies it in the Plugins menu. This name is max. 31 alphanumerical
// characters or spaces + terminating '\0' long. To keep life easy for users,
// this name should be descriptive and correlate with the name of DLL.
extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname, "OllyStepNSearch");       // Name of plugin
  return PLUGIN_VERSION;
};

// OllyDbg calls this obligatory function once during startup. Place all
// one-time initializations here. If all resources are successfully allocated,
// function must return 0. On error, it must free partially allocated resources
// and return -1, in this case plugin will be removed. Parameter ollydbgversion
// is the version of OllyDbg, use it to assure that it is compatible with your
// plugin; hw is the handle of main OllyDbg window, keep it if necessary.
// Parameter features is reserved for future extentions, do not use it.
extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {
  // This plugin uses all the newest features, check that version of OllyDbg is
  // correct. I will try to keep backward compatibility at least to v1.99.
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  // Keep handle of main OllyDbg window. This handle is necessary, for example,
  // to display message box.
  hwMain=hw;
  // Plugin successfully initialized. Now is the best time to report this fact
  // to the log window. To conform OllyDbg look and feel, please use two lines.
  // The first, in black, should describe plugin, the second, gray and indented
  // by two characters, bears copyright notice.
  Addtolist(0, 0, "OllyStepNSearch v%s", VERSION);
  Addtolist(0, -1, "  Public domain, no Copyright 2006 Didier Stevens %s", URL);
	iFlagRegPrevNotInitialized = 1;
	iFlagEnableStepSearch = 0;

	LoadFromOllyDbgIni();

  return 0;
};

// Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup
// menu in one of standard OllyDbg windows. When plugin wants to add own menu
// items, it gathers menu pattern in data and returns 1, otherwise it must
// return 0. Except for static main menu, plugin must not add inactive items.
// Item indices must range in 0..63. Duplicated indices are explicitly allowed.
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  switch (origin) {
    // Menu creation is very simple. You just fill in data with menu pattern.
    // Some examples:
    // 0 Aaa,2 Bbb|3 Ccc|,,  - linear menu with 3items, relative IDs 0, 2 and 3,
    //                         separator between second and third item, last
    //                         separator and commas are ignored;
    // #A{0Aaa,B{1Bbb|2Ccc}} - unconditional separator, followed by popup menu
    //                         A with two elements, second is popup with two
    //                         elements and separator inbetween.
    case PM_MAIN:                      // Plugin menu in main window
      strcpy(data, "0 &Options");
      strcat(data, ",|1 &Help");
      strcat(data, ",2 &About");
      // If your plugin is more than trivial, I also recommend to include Help.
      return 1;
    default: break;                    // Any other window
  };
  return 0;                            // Window not supported by plugin
};

// This optional function receives commands from plugin menu in window of type
// origin. Argument action is menu identifier from ODBG_Pluginmenu(). If user
// activates automatically created entry in main menu, action is 0.
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  char achBuf[1024];
//  char achCmd[1024];
  int id;
  
  if (origin==PM_MAIN) {
    switch (action) {
      case 0:
      	id = DialogBox(hiDLL, MAKEINTRESOURCE(IDD_OPTIONS), hwMain, (DLGPROC)OptDlgProc);
      	if(id == IDOK)
      	{
	      	Flash("OllyStepNSearch is %s", iFlagEnableStepSearch ? "enabled" : "disabled");
	      	SaveToOllyDbgIni();
      	}
				break;

      case 1:
      	DialogBox(hiDLL, MAKEINTRESOURCE(IDD_HELP), hwMain, (DLGPROC)HlpDlgProc);
//			  system(achCmd);	
				break;

      case 2:
      	sprintf(achBuf, "OllyStepNSearch v%s plugin v1.10\nPublic domain, no Copyright 2006 Didier Stevens\n%s", VERSION, URL);
        MessageBox(hwMain, achBuf, "OllyStepNSearch plugin", MB_OK|MB_ICONINFORMATION);
        break;

      default: 
      	break;
    }; 
	}
};

// Function is called when user opens new or restarts current application.
// Plugin should reset internal variables and data structures to initial state.
extc void _export cdecl ODBG_Pluginreset(void) {
	iFlagRegPrevNotInitialized = 1;
	iFlagEnableStepSearch = 0;
	LoadFromOllyDbgIni();
};

// OllyDbg calls this optional function when user wants to terminate OllyDbg.
// All MDI windows created by plugins still exist. Function must return 0 if
// it is safe to terminate. Any non-zero return will stop closing sequence. Do
// not misuse this possibility! Always inform user about the reasons why
// termination is not good and ask for his decision!
extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};

// OllyDbg calls this optional function once on exit. At this moment, all MDI
// windows created by plugin are already destroyed (and received WM_DESTROY
// messages). Function must free all internally allocated resources, like
// window classes, files, memory and so on.
extc void _export cdecl ODBG_Plugindestroy(void) {
};

// Called after a step
int ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent)
{
	int iLen;
	int iIter;
	t_disasm da;
	int iFound;
	char achText[TEXTLEN];
	unsigned char achInstStr[MAXCMDSIZE];

	// Do nothing if the plugin is not enabled
  if (iFlagEnableStepSearch == 0)
  	return 0;

	if (reg == NULL)
		return 0;

	// Initialize
	if (iFlagRegPrevNotInitialized == 1)
	{
		iFlagRegPrevNotInitialized = 0;
		for (iIter = 0; iIter < 8; iIter++)
			ulRegPrev[iIter] = reg->r[iIter];
	}

	// Step Over if we are above the limit address,
	// normally we should not break here, but it happens
	// Just continue
	if (reg->ip >= ulStepInLimit)
	{
		iStep = STEP_OVER;
		Go(0, 0, iStep, 1, 1);
		return 0;
	}

	// Check if registers have changed, if yes if they point to a string, and if the string contains the search text
	iFound = 0;
	if (iStep == STEP_IN)
		for (iIter = 0; iIter < 8; iIter++)
		{
			if (ulRegPrev[iIter] != reg->r[iIter])
			{
				if (iIter != 4 && Decodeascii(reg->r[iIter], achText, TEXTLEN, DASC_NOHEX) != 0)
				{
		   		Addtolist(reg->ip, 0, "OllyStepNSearch: %s %08lX -> %08lX string %s", aszRegName[iIter], ulRegPrev[iIter], reg->r[iIter], achText);
		   		if (iFlagSearchForSearchText && iFlagSearchInRegisters && strstr(achText, achSearchText) != NULL)
		   			iFound = 1;
		   	}
				ulRegPrev[iIter] = reg->r[iIter];
			}
		}
	// Break if we find the search text
	if (iFound == 1)
	{
		Infoline ("Found %s in registers", achSearchText);
		if (iFlagDisableAfterBreak) // disable OllyStepNSearch after finding the text if the user required this
			iFlagEnableStepSearch = 0;
		return 0;
	}

	// Analyze the next command to decide if we Step In or Step Over
	// Analysis is needed because I didn't find out how to read OllyDbgs t_disasm data
	iStep = STEP_IN;
	iLen = Readcommand(reg->ip, (char *) achInstStr);
	if (iLen == 0)
		return 0;

	Disasm(achInstStr, MAXCMDSIZE, reg->ip, NULL, &da, DISASM_ALL, Getcputhreadid()); 
	if (iFlagSearchForSearchText && iFlagSearchInInformation)
		for (iIter = 0; iIter < 3; iIter++)
			if (strstr(da.opinfo[iIter], achSearchText) != NULL)
			{
				Infoline ("Found %s in information", achSearchText);
				if (iFlagDisableAfterBreak) // disable OllyStepNSearch after finding the text if the user required this
					iFlagEnableStepSearch = 0;
				return 0;
			}
	switch (da.cmdtype)
	{
		case C_CAL:
		case C_JMP:
		case C_JMC:
			if (da.jmpaddr >= ulStepInLimit)
				iStep = STEP_OVER;
			else if (Followcall(da.jmpaddr) >= ulStepInLimit)
				iStep = STEP_OVER;
			break;
			
		case C_REP:
			iStep = STEP_OVER;
	}
	
	Go(0, 0, iStep, 1, 1);
	
	return 0;
}

LRESULT CALLBACK OptDlgProc(HWND hwDialog, UINT msg, WPARAM wp, LPARAM lp)
{
  char achBuf[TEXTLEN];

  switch (msg) 
  {
	  case WM_INITDIALOG:
	  	CenterDialog(hwDialog);

			// Populate window	
	    SendMessage(GetDlgItem(hwDialog, IDC_CHK_ENABLE), BM_SETCHECK, (WPARAM)iFlagEnableStepSearch, 0L);
			sprintf(achBuf, "0x%X", ulStepInLimit);
	    SetDlgItemText(hwDialog, IDC_EDT_SEARCHSTRING, achSearchText);
	    SetDlgItemText(hwDialog, IDC_EDT_STEPINLIMIT, achBuf);
	    SendMessage(GetDlgItem(hwDialog, IDC_CHK_DISABLEAFTERBREAK), BM_SETCHECK, (WPARAM)iFlagDisableAfterBreak, 0L);
	    SendMessage(GetDlgItem(hwDialog, IDC_CHK_REGISTERS), BM_SETCHECK, (WPARAM)iFlagSearchInRegisters, 0L);
	    SendMessage(GetDlgItem(hwDialog, IDC_CHK_INFORMATION), BM_SETCHECK, (WPARAM)iFlagSearchInInformation, 0L);
	    break;

	  case WM_COMMAND:
	    switch (LOWORD(wp)) 
	    {
		    case IDOK:
		      iFlagEnableStepSearch = ((IsDlgButtonChecked(hwDialog, IDC_CHK_ENABLE) == BST_CHECKED) ? 1 : 0);
		      GetDlgItemText(hwDialog, IDC_EDT_SEARCHSTRING, achSearchText, TEXTLEN-1);
					iFlagSearchForSearchText = strcmp(achSearchText, "") == 1;
		      GetDlgItemText(hwDialog, IDC_EDT_STEPINLIMIT, achBuf, TEXTLEN-1);
					ulStepInLimit = ParseStepInLimit(achBuf);
		      iFlagDisableAfterBreak = ((IsDlgButtonChecked(hwDialog, IDC_CHK_DISABLEAFTERBREAK) == BST_CHECKED) ? 1 : 0);
		      iFlagSearchInRegisters = ((IsDlgButtonChecked(hwDialog, IDC_CHK_REGISTERS) == BST_CHECKED) ? 1 : 0);
		      iFlagSearchInInformation = ((IsDlgButtonChecked(hwDialog, IDC_CHK_INFORMATION) == BST_CHECKED) ? 1 : 0);
		      EndDialog(hwDialog, IDOK);
		      break;

		    case IDCANCEL:
		      EndDialog(hwDialog, IDCANCEL);
		      break;
	    }
	    break;

	  default:
	    return FALSE;
  }
  return TRUE;
}

ulong ParseStepInLimit(char *szValue)
{
	ulong ulResult;	
	char *szError;

	ulResult = strtoul(szValue, &szError, 0);
	if (*szError != '\0' || ulResult == ULONG_MAX)
		return 0x10000000;
	else
		return ulResult;
}

void CenterDialog(HWND hwDialog)
{
  RECT rMain, rDialog;
  UINT  iW, iH;

  GetWindowRect(hwDialog, &rDialog);
  GetWindowRect(hwMain, &rMain);
  iH = rDialog.bottom - rDialog.top;
  iW = rDialog.right  - rDialog.left;
  MoveWindow(hwDialog, rMain.left + (rMain.right  - rMain.left) / 2 - iW / 2, rMain.top + (rMain.bottom - rMain.top) / 2 - iH / 2, iW, iH, TRUE);
}

LRESULT CALLBACK HlpDlgProc(HWND hwDialog, UINT msg, WPARAM wp, LPARAM lp)
{
	void *pHelp;
  char *szHelp = 
		"OllyStepNSearch v%s plugin\r\n"
		"2006 Didier Stevens %s\r\n"
		"\r\n"
		"This plugin allows you to search for a given text when automatically\r\n"
		"stepping through the debugged program.\r\n"
		"\r\n"
		"When the plugin is enabled, it will step automatically through the debugged\r\n"
		"program once a step command (like Step Into) is issued.\r\n"
		"Enabling the plugin is done with the \"Options\" menu command.\r\n"
		"After enabling, press F7 to start.\r\n"
		"\r\n"
		"After each step, the plugin will check which registers have changed.\r\n"
		"If a changed register points to an ASCII string, it is logged.\r\n"
		"If a search string has been defined and it is contained in the ASCII string\r\n"
		"pointed to by the register or the Information pane, the stepping is paused.\r\n"
		"Comparison is case sensitive.\r\n"
		"\r\n"
		"A search string is defined by entering it with the \"Options\" menu command."
		"\r\nIt is remembered in the OllyDbg INI file.\r\n"
		"Entering an empty string disables the break on string command.\r\n"
		"OllyStepNSearch can search in strings pointed to by registers (search in registers toggle)\r\n"
		"and it can search in the Information pane of the CPU window (search in information toggle).\r\n"
		"Read the \"Information window\" help section of the OllyDbg v1.10 help file if you're not familiar\r\n"
		"with the Information pane.\r\n"
		"\r\n"
		"If the search string is not found, debugging is resumed. If the current address\r\n"
		"is lower than the limit address (by default 0x10000000) a step into command is \r\n"
		"issued. A step over command is issued if the current address is higher than the\r\n"
		"limit address, or if the current command is a call/jump to an address higher \r\n"
		"than the limit address.\r\n"
		"The limit address can be changed in the Options dialog.\r\n"
		"\r\n"
		"The plugin can be disabled automatically when the search string is found (Disable after break toggle).\r\n"
		"I added this option because I usually want to single step after finding the search string,\r\n"
		"but often forgot to disable the plugin before single stepping.\r\n"
		"\r\n"
		"Restarting the debugged program disables the plugin.\r\n"
		"\r\n"
		"Debugging example:\r\n"
		"- Start OllyDbg and load the ftp.exe program (in system32 directory)\r\n"
		"- Start the OllyStepNSearch plugin \"Options\" menu command\r\n"
		"- Enter \"google\" as Search string (without the double quotes, of course)\r\n"
		"- Enable StepNSearch\r\n"
		"- Click OK\r\n"
		"- Press F7 to start debugging\r\n"
		"- Go to the FTP window and type \"open google.com\" and press enter\r\n"
		"- The StepNSearch plugin will stop debugging when a register points to \r\n"
		"  a string containing google.\r\n"
		"  On my Windows XP SP2, this happens when EAX points to \"open google.com\".\r\n"
		"- You can continue with F7 and see how ftp.exe parses the \"open google.com\" command.\r\n"
  ;

  switch (msg) 
  {
	  case WM_INITDIALOG:
	  	CenterDialog(hwDialog);
			if ((pHelp = malloc(strlen(szHelp) + 1024)) == NULL)
		    SetDlgItemText(hwDialog, IDC_EDT_HELP, "Error!");
		  else
			{
				sprintf(pHelp, szHelp, VERSION, URL);
		    SetDlgItemText(hwDialog, IDC_EDT_HELP, pHelp);
	    	free(pHelp);
			}
	    break;

	  case WM_COMMAND:
	    if (LOWORD(wp) == IDOK || LOWORD(wp) == IDCANCEL)
		  	EndDialog(hwDialog, IDOK);
	    break;

	  default:
	    return FALSE;
  }
  return TRUE;
}
