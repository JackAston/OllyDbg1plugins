// tt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "tt.h"
#include "Plugin.h"

/*
BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
	if (reason==DLL_PROCESS_ATTACH)
		hinst=hi;                          // Mark plugin instance
	return 1;                            // Report success
};
*/
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
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


// This is an example of an exported variable
TT_API int nTt=0;

// This is an example of an exported function.
TT_API int fnTt(void)
{
	return 42;
}
TT_API int _ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features)
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	return 0;
}
TT_API int ODBG_Plugindata(char *shortname)
{
	strcpy(shortname,"test");       // Name of plugin
	return PLUGIN_VERSION;
}

TT_API int _ODBG_Pluginmenu(int origin,char data[4096],void *item)
 {
	switch (origin)
	{
    case PM_MAIN:                      // Plugin menu in main window
		strcpy(data,"0 &Bookmarks|1 &About");
		// If your plugin is more than trivial, I also recommend to include Help.
		return 1;
    case PM_DISASM:                    // Popup menu in Disassembler
	default:
		break;
	}
	return 1;
}
TT_API void _ODBG_Pluginaction(int origin,int action,void *item)
{
 	if (origin==PM_MAIN && origin==0) 
	{
		t_asmmodel tm;
		char buf[256];
	//	for(int i=0; i<3; i++)
			_Assemble("mov eax,0x6789", 0x40100c, &tm, 1, 4, buf);
		ulong l = _Writememory(tm.code, 0x40100c, tm.length, MM_SILENT);
		 _Broadcast(WM_USER_CHALL,0,0);

	}
}