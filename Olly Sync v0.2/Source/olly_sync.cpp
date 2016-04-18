//  olly_sync.cpp
//  Copyright (C) 2005 Andrew Hintz < drew@overt.org, http://guh.nu >
#define VERSION "0.2"

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

// we must define _WINSOCKAPI_ prior to including windows.h, otherwise compilation
// will fail on a number of redefinition errors in winsock structures.
#define _WINSOCKAPI_

#define PLUGIN_NAME   "olly_sync"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "plugin.h"
#include "connector.hpp"

struct CON_INFO {
	char server_addr [256];
	char portString	 [256];
	char username    [256];
	char project     [256];
};

void connect(void);
void insert_comment(t_dump *pd);
void insert_name(t_dump *pd);

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window
char             bookmarkwinclass[32]; // Name of bookmark window class
CON_INFO*		 coninfo; //global info for making a connection


BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
	if (reason==DLL_PROCESS_ATTACH)
		hinst=hi;                          // Mark plugin instance
	return 1;                            // Report success
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
	strcpy(shortname,""PLUGIN_NAME"");       // Name of plugin
	return PLUGIN_VERSION;
};

// OllyDbg calls this obligatory function once during startup. Place all
// one-time initializations here. If all resources are successfully allocated,
// function must return 0. On error, it must free partially allocated resources
// and return -1, in this case plugin will be removed. 
extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {

	// This plugin uses all the newest features, check that version of OllyDbg is
	// correct. I will try to keep backward compatibility at least to v1.99.
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;

	// Keep handle of main OllyDbg window. This handle is necessary, for example,
	// to display message box.
	hwmain=hw;

	coninfo = new CON_INFO; //initialize connection info struct
	strcpy(coninfo->server_addr, "127.0.0.1");
	strcpy(coninfo->portString, "5041");
	strcpy(coninfo->username, "user");

	coninfo->project[0] = NULL;

	Addtolist(0,0,""PLUGIN_NAME" plugin v"VERSION" initialized. Compiled on " __DATE__ "");
	Addtolist(0,-1,"  Copyright (C) 2005 Andrew Hintz, http://guh.nu");

	return 0;
};

#define TAG_OLLY_SYNC   0x272fe20AL     // Bookmark record type in .udd file

// Time to save data to .udd file! This is done by calling Pluginsaverecord()
// for each data item that must be saved. Global, process-oriented data must
// be saved in main .udd file (named by .exe); module-relevant data must be
// saved in module files. Don't forget to save all addresses relative to
// module's base, so that data will be restored correctly even when module is
// relocated.
extc void _export cdecl ODBG_Pluginsaveudd(t_module *pmod,int ismainmodule) {
  if (ismainmodule==0)
    return;                            // Save to main file only

  Message(0, "[*] "PLUGIN_NAME"> saving .udd values");

  Pluginsaverecord(TAG_OLLY_SYNC,sizeof(CON_INFO),(void *)coninfo);

};

// OllyDbg restores data from .udd file. If record belongs to plugin, it must
// process record and return 1, otherwise it must return 0 to pass record to
// other plugins. Note that module descriptor pointed to by pmod can be
// incomplete, i.e. does not necessarily contain all informations, especially
// that from .udd file.
extc int _export cdecl ODBG_Pluginuddrecord(t_module *pmod,int ismainmodule,
  ulong tag,ulong size,void *data) {
  if (ismainmodule==0)
    return 0;                          // saved in main file only
  if (tag!=TAG_OLLY_SYNC)
    return 0;                          // Tag is not recognized
  
  Message(0, "[*] "PLUGIN_NAME"> reading .udd values");
 
  memcpy(coninfo, data, sizeof(CON_INFO)); //coninfo was alloc'ed by initialize plugin

  return 1;                            // Record processed
};

// Function adds items either to main OllyDbg menu (origin=PM_MAIN) or to popup
// menu in one of standard OllyDbg windows. 
extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
	t_dump *pd;
	switch (origin) {

	case PM_MAIN:                      // Plugin menu in main window
		//TODO: only have connect option if disassembly is open
		strcpy(data,"1 &Connect \tAlt+1| ");

		strcat(data,"9 &About");
		return 1;
	case PM_DISASM:                    // Popup menu in Disassembler
		// First check that menu applies.
		pd=(t_dump *)item;
		if (pd==NULL || pd->size==0)
			return 0;                      // Window empty, don't add

		if (connector->is_connected())
		{
			sprintf(data,""PLUGIN_NAME"{2 Set &Comment \tAlt+;, 3 Set &Name \tAlt+:}");
			return 1;
		}

		break;
	default: break;                    // Any other window
	};
	return 0;                            // Window not supported by plugin
};

// This optional function receives commands from plugin menu in window of type
// origin. Argument action is menu identifier from ODBG_Pluginmenu(). If user
// activates automatically created entry in main menu, action is 0.
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
	t_dump *pd;
	if (origin==PM_MAIN) {
		switch (action) {
		case 1:
			connect();
			break;
		case 9: //About
			MessageBox(hwmain,
				""PLUGIN_NAME" plugin v"VERSION"\n"
				"Compiled on " __DATE__ "\n"
				"Copyright (C) 2005 Andrew Hintz\n"
				"http://guh.nu",
				""PLUGIN_NAME"",MB_OK|MB_ICONINFORMATION);
			break;
		default: break;
		}; }
	else if (origin==PM_DISASM) {
		pd=(t_dump *)item;
		switch (action)
		{
		case 2:
			insert_comment(pd);
			break;
		case 3:
			insert_name(pd);
			break;
		default:
			break;
		}//switch
	};//else if
};


// This function receives possible keyboard shortcuts from standard OllyDbg
// windows. If it recognizes shortcut, it must process it and return 1,
// otherwise it returns 0.
extc int _export cdecl ODBG_Pluginshortcut(
	int origin,int ctrl,int alt,int shift,int key,void *item) {

		if (ctrl==0 && alt!=0 && key=='1') //Alt+1 to connect
		{
			connect();
			return 1;
		}

		//key==186==VK_OEM_1 is when the semicolon/colon is pressed down on US keyboards
		if (key==VK_OEM_1 && ctrl==0 && alt!=0 && item!=NULL) //item is required for setting a name/comment
		{
			t_dump *pd;
			pd=(t_dump *)item;

			if(shift!=0)
				insert_name(pd); //Alt+; to insert name
			else //shift==0
				insert_comment(pd); //Alt+: to insert comment

			return 1;
		}

		return 0; // Shortcut not recognized
	};

// Function is called when user opens new or restarts current application.
extc void _export cdecl ODBG_Pluginreset(void) {
	connector->cleanup();
};

// OllyDbg calls this optional function when user wants to terminate OllyDbg.
// All MDI windows created by plugins still exist. Function must return 0 if
// it is safe to terminate. Any non-zero return will stop closing sequence. Do
// not misuse this possibility! 
extc int _export cdecl ODBG_Pluginclose(void) {
	connector->cleanup();
	return 0;
};

//Connect to the olly/ida_sync server; prompts for connection info
void connect(void)
{
	//ensure a file is open for debugging
	if ( 0 == ((char *)Plugingetvalue(VAL_PROCESSNAME))[0])
	{
		MessageBox(hwmain,
				"Please open a file for debugging before connecting to an ida_sync server.",
				""PLUGIN_NAME"",MB_OK|MB_ICONSTOP);
		return;
	}

	// if we are already connected then to do nothing.
	if (connector->is_connected())
		return;

	int port  = 5041;
	char password    [256];

	if (-1 == Gettext("Server address:", coninfo->server_addr, 0x00, NM_NONAME, FIXEDFONT))
		return; //cancelled by user

	if (-1 == Gettext("Server port:", coninfo->portString, 0x00, NM_NONAME, FIXEDFONT))
		return; //cancelled by user
	port = atoi(coninfo->portString);

	if (-1 == Gettext("Username:", coninfo->username, 0x00, NM_NONAME, FIXEDFONT))
		return; //cancelled by user

	strcpy(password, "password");
	if (-1 == Gettext("Password:", password, 0x00, NM_NONAME, FIXEDFONT))
		return; //cancelled by user

	if (coninfo->project[0] == NULL) //if "", then set default project name
		strcpy(coninfo->project, (char *)Plugingetvalue(VAL_PROCESSNAME));
	if (-1 == Gettext("Project name:", coninfo->project, 0x00, NM_NONAME, FIXEDFONT))
		return; //cancelled by user

	connector->server_connect(coninfo->server_addr, port, 
		coninfo->username, password, coninfo->project);
}

//prompt user for comment and push it out to the server
void insert_comment(t_dump* pd)
{
	char buf [MAXSTR+128];
	char text [MAXSTR+128];

	if (connector->is_connected()) // && origin==PM_DISASM) --> do we need this?
	{
		Findname(pd->sel0,NM_COMMENT,text); //get existing comment
		if (-1 == Gettext("Enter "PLUGIN_NAME" comment:", text, 0x00, NM_COMMENT, FIXEDFONT))
			return; //cancelled by user
		Insertname(pd->sel0, NM_COMMENT, text);
		Redrawdisassembler();
		sprintf(buf, "%d:::%08x:::%s", 
			IDA_SYNC_COMMAND_REG_COMMENT, 
			pd->sel0, // our current selected address
			text);
		if (connector_push(buf))
			Message(pd->sel0, "[*] "PLUGIN_NAME"> Successfully pushed comment at address 0x%08x to server.", pd->sel0);
	}//if connected
}

//prompt user for name/label and push it out to the server
void insert_name(t_dump *pd)
{
	char buf [MAXSTR+128];
	char text [MAXSTR+128];

	if (connector->is_connected()) // && origin==PM_DISASM) --> do we need this?
	{
		Findname(pd->sel0,NM_LABEL,text); //get existing name
		if (-1 == Gettext("Enter "PLUGIN_NAME" label(name):", text, 0x00, NM_LABEL, FIXEDFONT))
			return; //cancelled by user
		Insertname(pd->sel0, NM_LABEL, text);
		Redrawdisassembler();
		sprintf(buf, "%d:::%08x:::%s", 
			IDA_SYNC_COMMAND_NAME, 
			pd->sel0, // our current selected address
			text);
		if (connector_push(buf))
			Message(pd->sel0, "[*] "PLUGIN_NAME"> Successfully pushed name/label at address 0x%08x to server.", pd->sel0);
	}//if connected
}


/////////////////////////////////////////////////////////////////////////////////////////
// connector_pull()
//
// this routine is called by the socket_proc notification hook to handle inbound data
// from the server.
//
// arguments: none.
// returns:   false if socket connection was severed, true otherwise.
//

bool connector_pull (void)
{
	int   len;
	char  buf  [1024];
	char  data [1024];
	int   command;

	//ea_t   address;
	ulong address;
	SOCKET connection;

	memset(buf,     0, sizeof(buf));
	memset(data,    0, sizeof(data));

	// grab the socket we wil be reading from.
	connection = connector->get_connection();

	len = recv(connection, buf, sizeof(buf), 0);

	// connection closed.
	if (len == 0 || len == SOCKET_ERROR)
	{
		connector->cleanup();
		//TODO: perhaps change to a MessageBox():
		Message(0, "[!] "PLUGIN_NAME"> Socket read failed. Connection closed.");
		return false;
	}

	// null terminate the string.
	buf[len] = 0;

	// parse the inbound request. if we can't extract the correct fields, return.
	if (sscanf(buf, "%d:::%08x:::%1023[^\0]", &command, &address, data) != 3)
		return true;

	//
	// handle the received command appropriately
	//

	switch(command)
	{
	case IDA_SYNC_COMMAND_JUMPTO:
		Setdisasm(address, 0, 0);
		Redrawdisassembler();
		break;

	case IDA_SYNC_COMMAND_NAME:
		Message(address, "[*] "PLUGIN_NAME"> Received new name (label) @%08x: %s", address, data);
		Insertname(address, NM_LABEL, data);
		Redrawdisassembler();
		break;

	case IDA_SYNC_COMMAND_REG_COMMENT:
	case IDA_SYNC_COMMAND_REP_COMMENT:
		Message(address, "[*] "PLUGIN_NAME"> Received comment @%08x: %s", address, data);
		Insertname(address, NM_COMMENT, data);
		Redrawdisassembler();
		break;

	default:
		Message(0, "[*] "PLUGIN_NAME"> Received unknown command code: %d, ignoring.", command);
	}

	// ping pong. let the server know we're alive?
	send(connection, "1", 1, 0);

	return true;
}
