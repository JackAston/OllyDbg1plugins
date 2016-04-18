#include <Windows.h>
#include "Plugin.h"
#include <sstream>
#include <DbgHelp.h>
#include <map>

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window

/*
class DMOption
{
public:
	std::string Name;
	bool Val;

	DMOption(std::string name, bool val)
	{
		Name = name;
		Val = val;
	}

	std::string ToString()
	{
		std::string ret = std::string();
		ret = "[";
		ret += (Val) ? "X" : " ";
		ret += "] ";
		ret += Name;
	}
};*/

/*
static DMOption DMOptions[] = 
{
	DMOption("COMPLETE", true),
	DMOption("NAME_ONLY", false),
};*/


//static std::map<std::string, bool> DMOptions = std::map<std::string, bool>();

typedef std::pair<std::string, bool> DMOptionPair;
typedef std::map<std::string, bool>::iterator DMOptionIterator;

class CDMOptions : public std::map<std::string, bool>
{
public:
	CDMOptions()
	{
		this->insert(DMOptionPair("REMOVE_EMPTY", true));
		this->insert(DMOptionPair("COMPLETE", false));
		this->insert(DMOptionPair("NAME_ONLY", true));
	}
};
CDMOptions DMOptions;


BOOL APIENTRY DllMain( HINSTANCE hi,
                       DWORD  reason,
                       LPVOID lpReserved
					 )
{	
	if (reason == DLL_PROCESS_ATTACH)
		hinst = hi;                          // Mark plugin instance
    return TRUE;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32]) 
{
	strcpy(shortname,"Demangler");       // Name of plugin
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) 
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	hwmain=hw;

	Addtolist(0,0,"OllyDemangler plugin by high6");
	Addtolist(0,-1,"  Copyright (C) 2009 high6");

	return 0;
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
	if (origin == PM_MAIN)
	{
		std::string str = std::string();

		str = "2Options{";

		int num = 3;	
		for(DMOptionIterator it = DMOptions.begin(); it != DMOptions.end(); it++, num++)
		{
			std::ostringstream numstr;
			numstr << num;
			str += numstr.str();
			str += it->first;
			str += ",";
		}

		str += "}|1Demangle Labels";

		strcpy(data, str.c_str());
		return 1;
	}

	return 0;
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item)
{
	ulong base;
	ulong size;
	char name[TEXTLEN];
	char dename[TEXTLEN];

	if(action == 1)
	{
		Getdisassemblerrange(&base, &size);

		if (!base)
			return;
		
		for(int i = 0; i < size; i++)
		{
			if (Findname(base + i, NM_LABEL, name))
			{
				std::string str = std::string(name);

				if (str == "00:?" && DMOptions["REMOVE_EMPTY"])
				{
					Insertname(base + i, NM_LABEL, "");
					continue;
				}

				if(str.length() > 3 && str.find("00:") == 0)
					str = str.substr(3);

				int flags = 0;

				if (DMOptions["NAME_ONLY"])
					flags |= UNDNAME_NAME_ONLY;

				if (UnDecorateSymbolName(str.c_str(), dename, TEXTLEN, flags))
				{
					Insertname(base + i, NM_LABEL, dename);
				}
			}
		}
	}
	else if (action >= 3)
	{
		DMOptionIterator it = DMOptions.begin();
		for(int i = 3; i < action; i++, it++);

		it->second = it->second ? false : true;

		std::string str = std::string();

		str += it->first;
		str += " = ";
		str += it->second ? "True" : "False";

		Infoline((char*)str.c_str());
	}
}