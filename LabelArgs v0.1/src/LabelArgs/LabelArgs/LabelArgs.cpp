#include <Windows.h>
#include "Plugin.h"
#include <string>
#include "detours/Detours.h"
#include <map>
typedef std::string string;

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window

static std::map<string, string*> ArgList;

typedef char* (__cdecl*GetArgsPtr)(int addr, int num1, int num2, char* outname);
GetArgsPtr OldGetArgs;

string& Trim(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());

	return str;
}

char* __cdecl MyGetArgs(int addr, int num1, int num2, char * outname)
{
	char name[TEXTLEN];
	int len;

	if (!(len = Findname(addr, NM_LABEL, name)))
		return OldGetArgs(addr, num1, num2, outname);
	
	string label = string(name, len);

	string id;

	int st;
	if ((st = label.find('(')) == -1)
		return OldGetArgs(addr, num1, num2, outname);

	int en;
	if ((en = label.find(')', st)) == -1)
		return OldGetArgs(addr, num1, num2, outname);

	if (outname)
		strncpy(outname, label.substr(0, st).c_str(), TEXTLEN);

	std::map<string, string*>::iterator it = ArgList.find(label);
	if (it != ArgList.end())
		return (char*)(it->second->c_str());

	string* ret = new string("@");

	for(int i = st + 1; i < en; i++)
	{
		if (label[i] == ',')
		{
			if (Trim(id).length() > 0)
				*ret += "I=" + Trim(id) + ",";
			id = "";
		}
		else
		{
			id += label[i];
		}
	}
	if (Trim(id).length() > 0)
		*ret += "I=" + Trim(id);

	ArgList.insert(std::make_pair(*(new string(label.c_str())), ret));

	return (char*)ret->c_str();
}

extc int _export cdecl ODBG_Plugindata(char shortname[32]) 
{
	strcpy(shortname,"LabelArgs");       // Name of plugin
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion < PLUGIN_VERSION)
		return -1;
	hwmain=hw;

	Addtolist(0,0,"LabelArgs plugin by high6");

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		OldGetArgs = (GetArgsPtr)DetourFunction((PBYTE)0x0040B1B4, (PBYTE)&MyGetArgs);
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		DetourRemove((PBYTE)OldGetArgs, (PBYTE)&MyGetArgs);
	}
	return TRUE;
}

