/* modfied command line plugin 
command added Loaddll
usage 
replace the default cmdline plugin with new one
alt+f1
type Loaddll "yourdll"
if successfully loaded you can see a log entry "loaded "your dll"" in ollydbg log window
and you can see your dll in memory window
modified by anonymouse
*/





#define MEM_SHARED 		0x8000000 // thanks ElicZ for this 9x hack
#define STRICT              	        // Avoids some type mismatches
#include <windows.h>
#include <stdio.h>
#include "plugin.h"


int magic(void);
extern char 			string[TEXTLEN];// defined in cmdexec.c
char 				copy[TEXTLEN];	
ulong 				eip;
ulong				esp;
ulong				status;
HANDLE 				hDebugee;
char 				*hMem;

int magic()
{
	t_thread 		*thread;
	t_asmmodel 		model;
	char 			buffer[TEXTLEN] = {0};
	char 			error[TEXTLEN] = {0};
	int 			len,totallen;
	strncpy(copy,string,(TEXTLEN-1));
	hDebugee  = (HANDLE)Plugingetvalue(VAL_HPROCESS);
	thread	  = Findthread(Getcputhreadid());
	eip	  = thread->reg.ip;
	esp	  = thread->reg.r[REG_ESP];
	totallen = 0;

	if(GetVersion() > 0x80000000)
	{
		hMem = (CHAR *)VirtualAlloc(NULL,1000,MEM_COMMIT|MEM_RESERVE|MEM_SHARED,PAGE_EXECUTE_READWRITE);
	}
	else
	{
		hMem = (CHAR *)VirtualAllocEx(hDebugee,NULL,1000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);
	}

	WriteProcessMemory(hDebugee, (LPVOID)(hMem), copy, strlen(copy), 0);

	snprintf(buffer,(TEXTLEN-1),"%s","pushad");
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;

	snprintf(buffer,(TEXTLEN-1),"%s","pushfd");
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;			
		
	snprintf(buffer,(TEXTLEN-1),"push 0x%lx",*(ulong *)&hMem);
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;

	snprintf(buffer,(TEXTLEN-1),"call %s","LoadLibraryA");
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;
		
		
	snprintf(buffer,(TEXTLEN-1),"%s","popfd");
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;

	snprintf(buffer,(TEXTLEN-1),"%s","popad");
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);
	totallen += len;

	snprintf(buffer,(TEXTLEN-1),"jmp 0x%lx",eip);
	len = Assemble(buffer, (ULONG_PTR)(hMem+500 + totallen), &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)(hMem+500 + totallen), model.code, len, 0);

	thread->reg.ip = (ULONG_PTR)hMem+500;
	thread->reg.modified = 1;
	thread->regvalid = 1;
	Broadcast(WM_USER_CHREG, 0, 0);
	Go(Getcputhreadid(), eip, STEP_RUN, 0, 1);
	return 0;
}


int Loaddll(char *answer,ulong parm)
{

	
	if(Getstatus() == STAT_NONE)
	{
		MessageBox(NULL,"I Cant Load Dlls On Thin Air Start Some Process First","LoadDll Function",NULL);
		return 0;
	}
	else if(Getstatus() == STAT_RUNNING)
	{
		status = 1;
		Suspendprocess(1);
		magic();
		return 0;
	}
	else if(Getstatus() == STAT_STOPPED)
	{	
		status = -1;
		magic();
		return 0;

	}
	else
	{
		return 0;
	}
	
}



#pragma argsused
extc int _export cdecl ODBG_Pausedex(int reason, int extdata, t_reg *reg, DEBUG_EVENT *debugevent)
{
	DWORD 				Exceptcode;
	DEBUG_EVENT			debev;
	ulong				newesp;
	t_thread 			*newthread;

	
	
	if (reg != 0)
	{
	if(debugevent !=0)
	{
		
		debev = *debugevent;
		Exceptcode = debev.u.Exception.ExceptionRecord.ExceptionCode;
		
		if((Exceptcode != EXCEPTION_BREAKPOINT) && (Exceptcode != EXCEPTION_SINGLE_STEP))
		{
		return 0;
		}

		
		if (debev.u.Exception.ExceptionRecord.ExceptionAddress == (PVOID)(eip))
		{ 	
			if(GetVersion() > 0x80000000)
			{	
				Addtolist(0,1,"Loaded %s ",copy);			
				VirtualFree(hMem,0,MEM_RELEASE);
				if(status == 1)
					Go(Getcputhreadid(), 0, STEP_RUN, 0, 1);
				eip = esp = status = *copy = 0;
				return 0;
			}
			else
			{
				
				newthread  = Findthread(Getcputhreadid());
				newesp	  = newthread->reg.r[REG_ESP];
				if(esp == newesp)
				{
					Addtolist(0,1,"Loaded %s ",copy);
					VirtualFreeEx(hDebugee,hMem,0,MEM_RELEASE);
					if(status == 1)
						Go(Getcputhreadid(), 0, STEP_RUN, 0, 1);
					eip = esp = status = *copy = 0;
					return 0;
				}
				Go(Getcputhreadid(), eip, STEP_RUN, 0, 1);
				return 0;
			}
		}
	}

	}
return 0;
};


/*
updated with one more command
loadplugin 
usage alt+f1 type loadplugin "yourplugin"
this will load the plugin and register 
should be use full under circumstances where you dont have 
the requisite plugin and you dont want to copy 
it to ollydbg plugin path and also you dont want to 
restart the existing session
this will also bypass the 32 bit plugin limit 
if you already have 32 plugins loaded it will ask you 
if you want to free the last loaded plugin and 
insert the plugin in its place 
on confirmation it will load the plugin as the last one

and a BIG THANKS to ALEX_IONESCU for helping me with 
wrting this plugin without his patience and help 
i would have ended up with some horrible looking code
like ((*(char **)&(DWORD *)0x40f55bc + *(word *)ox401234 ) * 45 ) = (ulong)my_newcrap;
all the slick looking typedefs declarations etc belongs to Alex

*/


#define COUNT_OF_PLUGINS	0x4f55b4  // ollydbg global 
#define	MENU_DATA		0x4f55bc  // ollydbg global
#define PLUGIN_ARRAY		0x4f0ab4  // ollydbg global
#define MENU_ID_BASE		0xE000    // ollydbg global
#define MENUS_PER_PLUGIN	0x40      // ollydbg global

extern HWND			hwmain; // defined in command.c


typedef INT	(__cdecl *PODBG_PLUGIN_DATA)		(IN PCHAR ShortName);
typedef INT	(__cdecl *PODBG_PLUGIN_INIT)		(int ollydbgversion,HWND hw,ulong *features);
typedef VOID	(__cdecl *PODBG_PLUGIN_MAINLOOP)	(DEBUG_EVENT *debugevent);
typedef VOID	(__cdecl *PODBG_PLUGIN_SAVEUDD)		(t_module *pmod,int ismainmodule);
typedef INT	(__cdecl *PODBG_PLUGIN_UDDRECORD)	(t_module *pmod,int ismainmodule,ulong tag,ulong size,void *data);
typedef INT	(__cdecl *PODBG_PLUGIN_MENU)		(int origin,char data[4096],void *item);
typedef VOID	(__cdecl *PODBG_PLUGIN_ACTION)		(int origin,int action,void *item);
typedef INT	(__cdecl *PODBG_PLUGIN_SHORTCUT)	(int origin,int ctrl,int alt,int shift,int key,void *item);
typedef VOID	(__cdecl *PODBG_PLUGIN_RESET)		(void);
typedef VOID	(__cdecl *PODBG_PLUGIN_CLOSE)		(void);
typedef VOID	(__cdecl *PODBG_PLUGIN_DESTROY)		(void);
typedef INT	(__cdecl *PODBG_PLUGIN_PAUSED)		(int reason, t_reg *reg);
typedef INT	(__cdecl *PODBG_PLUGIN_PAUSEDEX)	(int reason,int extdata,t_reg *reg,DEBUG_EVENT *debugevent);
typedef INT	(__cdecl *PODBG_PLUGIN_PLUGINCMD)	(int reason,t_reg *reg,char *cmd);
typedef VOID	(__cdecl *PODBG_PLUGIN_SOMEROUTINE)	(HMENU nm,char *menubuf,UINT_PTR id,int unkn);
typedef void	(__cdecl *PODBG_PLUGIN_CLOSE)		(void);
typedef void	(__cdecl *PODBG_PLUGIN_DESTROY)		(void);

typedef struct _ODBG_PLUGIN
{
HMODULE 		hMod;
char 			dll[MAX_PATH];
char			name[32];
char			unknown[MAX_PATH];
DWORD			unknown1;
PODBG_PLUGIN_MAINLOOP	my_ODBG_Pluginmainloop;
PODBG_PLUGIN_MENU	my_ODBG_Pluginmenu;
PODBG_PLUGIN_ACTION	my_ODBG_Pluginaction;
PODBG_PLUGIN_SHORTCUT	my_ODBG_Pluginshortcut;
PODBG_PLUGIN_SAVEUDD	my_ODBG_Pluginsaveudd;
PODBG_PLUGIN_UDDRECORD	my_ODBG_Pluginuddrecord;
PODBG_PLUGIN_RESET	my_ODBG_Pluginreset;
PODBG_PLUGIN_PAUSED	my_ODBG_Paused;
PODBG_PLUGIN_PAUSEDEX	my_ODBG_Pausedex;
PODBG_PLUGIN_PLUGINCMD	my_ODBG_Plugincmd;
} ODBG_PLUGIN, *PODBG_PLUGIN;




int Loadplugin(char *answer,ulong parm)
{
	HMODULE			newmod;
	char			shortname[4096];
	int			r,s,t,u;
	char			pdata[] 	= {"_ODBG_Plugindata"};
	char			pinit[] 	= {"_ODBG_Plugininit"};
	char			paction[] 	= {"_ODBG_Pluginaction"};
	char			pmainloop[]	= {"_ODBG_Pluginmainloop"};
	char			pmenu[] 	= {"_ODBG_Pluginmenu"};
	char			pshortcut[]	= {"_ODBG_Pluginshortcut"};
	char			psaveudd[]	= {"_ODBG_Pluginsaveudd"};
	char			puddrecord[]	= {"_ODBG_Pluginuddrecord"};
	char			preset[]	= {"_ODBG_Pluginreset"};
	char			ppaused[]	= {"_ODBG_Paused"};
	char			ppausedex[]	= {"_ODBG_Pausedex"};
	char			pplugincmd[]	= {"_ODBG_plugincmd"};
	char			pload[MAX_PATH]; 
	char			perror[] 	= {"Plugin '%s' has invalid version (%i.%02i)"};
	char			perrinit[] 	= {"Plugin '%s' failed to initialize (code %i)"};
	char 			maxplugin[]	= {"max 32 plugin allowed want to free the last plugin and continue"};
	PODBG_PLUGIN_SOMEROUTINE  someroutine 	= (PODBG_PLUGIN_SOMEROUTINE)0x496260;
	PODBG_PLUGIN_DATA	my_ODBG_Plugindata;
	PODBG_PLUGIN_INIT	my_ODBG_Plugininit;
	PODBG_PLUGIN_CLOSE	my_pluginclose;
	PODBG_PLUGIN_DESTROY	my_plugindestroy;
	PODBG_PLUGIN		my_plugin;
	ulong 			count;
	ulong			features;
	UINT_PTR		uIDNewItem;
	HMENU			newmenu;
	HMENU			newmainmenu;
	PCHAR 			*pblah 		= (PCHAR *)MENU_DATA; 


	newmainmenu = GetSubMenu(GetMenu(hwmain),4);
	count = *(PULONG)COUNT_OF_PLUGINS;

	if(count>=32)
	{
		t = MessageBox(hwmain,maxplugin,"OLLYDBG Plugin Loader",MB_YESNO);
		if(t == IDYES)
		{
			my_plugin = (PODBG_PLUGIN)PLUGIN_ARRAY;
			my_plugin = &my_plugin[count-1];
			my_pluginclose = (PODBG_PLUGIN_CLOSE)GetProcAddress(my_plugin->hMod,"_ODBG_Pluginclose");
			if(my_pluginclose)
				my_pluginclose();
			my_plugindestroy = (PODBG_PLUGIN_CLOSE)GetProcAddress(my_plugin->hMod,"_ODBG_Plugindestroy");
			if(my_plugindestroy)
				my_plugindestroy();
			FreeLibrary(my_plugin->hMod);
			memset(my_plugin,0,sizeof(ODBG_PLUGIN));
			RemoveMenu(newmainmenu,count,MF_BYPOSITION);
			DrawMenuBar(hwmain);
			*(PULONG)COUNT_OF_PLUGINS -= 1;
			goto delload;
		}
	}
	else
	{
delload:
		strncpy(pload,string,TEXTLEN-1);
		newmod = LoadLibrary(pload);
		if(newmod)
		{
			my_ODBG_Plugindata = (PODBG_PLUGIN_DATA)GetProcAddress(newmod,pdata);
			my_ODBG_Plugininit = (PODBG_PLUGIN_INIT)GetProcAddress(newmod,pinit);
			if((my_ODBG_Plugininit) && (my_ODBG_Plugindata))
			{
				*shortname = 0;
				r = my_ODBG_Plugindata((shortname));
			if(r < 0x6a || r  > 0x6e || *shortname == 0)
			{
				Addtolist(0,0,perror,pload,(r / 0x64),(r % 0x64));
			}
			else
			{
				count = *(PULONG)COUNT_OF_PLUGINS;
				my_plugin = (PODBG_PLUGIN)PLUGIN_ARRAY;
				my_plugin = &my_plugin[count];
				my_plugin->hMod= newmod; 
				strcpy(my_plugin->dll,pload);
				strncpy(my_plugin->name,shortname,0x1f);
				my_plugin->name[31] = 0;
				my_plugin->my_ODBG_Pluginaction   = (PODBG_PLUGIN_ACTION)GetProcAddress(newmod,paction); 
				my_plugin->my_ODBG_Pluginmainloop = (PODBG_PLUGIN_MAINLOOP)GetProcAddress(newmod,pmainloop);
				my_plugin->my_ODBG_Pluginmenu	  = (PODBG_PLUGIN_MENU)GetProcAddress(newmod,pmenu);
				my_plugin->my_ODBG_Pluginshortcut = (PODBG_PLUGIN_SHORTCUT)GetProcAddress(newmod,pshortcut);
				my_plugin->my_ODBG_Pluginsaveudd  = (PODBG_PLUGIN_SAVEUDD)GetProcAddress(newmod,psaveudd);
				my_plugin->my_ODBG_Pluginuddrecord= (PODBG_PLUGIN_UDDRECORD)GetProcAddress(newmod,puddrecord);
				my_plugin->my_ODBG_Pluginreset	  = (PODBG_PLUGIN_RESET)GetProcAddress(newmod,preset);
				my_plugin->my_ODBG_Paused  	  = (PODBG_PLUGIN_PAUSED)GetProcAddress(newmod,ppaused);
				my_plugin->my_ODBG_Pausedex 	  = (PODBG_PLUGIN_PAUSEDEX)GetProcAddress(newmod,ppausedex);
				my_plugin->my_ODBG_Plugincmd	  = (PODBG_PLUGIN_PLUGINCMD)GetProcAddress(newmod,pplugincmd);
				features = 0;
				s = my_ODBG_Plugininit(PLUGIN_VERSION,hwmain,&features);
				if(s)
				{
					Addtolist(0,0,perrinit,pload,s);
				}
				else
				{
					uIDNewItem = ((*(PULONG)COUNT_OF_PLUGINS) * MENUS_PER_PLUGIN) + MENU_ID_BASE;
					*shortname =0;
					if( 	(my_plugin->my_ODBG_Pluginmenu == 0) ||
	   					!(my_plugin->my_ODBG_Pluginmenu(0,shortname,0)) ||
	    					(*shortname == 0))
					{
						newmenu =0;
					}
					else
						newmenu = CreateMenu();
						if(newmenu)
						{
							*pblah = shortname;
							someroutine(newmenu,(PCHAR)PLUGIN_ARRAY,uIDNewItem,1);
						}
						u = *(PULONG)COUNT_OF_PLUGINS;
						if(!(u >= 10))	
							sprintf(shortname,"&%i %s",((u+1)%10),my_plugin->name);
						else
							sprintf(shortname,"%s",my_plugin->name);
						if(!newmenu)
							AppendMenu(newmainmenu,MF_BYCOMMAND|MF_ENABLED|MF_STRING,uIDNewItem,shortname);
						else
							AppendMenu(newmainmenu,MF_BYCOMMAND|MF_ENABLED|MF_STRING|MF_POPUP,(UINT_PTR)newmenu,shortname);
							*(PULONG)COUNT_OF_PLUGINS += 1;
							newmod = 0;
				}
			}
		}
	}

if(newmod)
FreeLibrary(newmod);
DrawMenuBar(hwmain);
Addtolist(0,1,"New Plugin %s loaded succesfully",pload); 
}
return 0;
}





typedef enum {
    SymNone = 0,
    SymCoff,
    SymCv,
    SymPdb,
    SymExport,
    SymDeferred,
    SymSym,       // .sym file
    SymDia,
    SymVirtual,
    NumSymTypes
} SYM_TYPE;

typedef struct _IMAGEHLP_MODULE {
    DWORD    SizeOfStruct;           // set to sizeof(IMAGEHLP_MODULE)
    DWORD    BaseOfImage;            // base load address of module
    DWORD    ImageSize;              // virtual size of the loaded module
    DWORD    TimeDateStamp;          // date/time stamp from pe header
    DWORD    CheckSum;               // checksum from the pe header
    DWORD    NumSyms;                // number of symbols in the symbol table
    SYM_TYPE SymType;                // type of symbols loaded
    CHAR     ModuleName[32];         // module name
    CHAR     ImageName[256];         // image name
    CHAR     LoadedImageName[256];   // symbol file name
} IMAGEHLP_MODULE, *PIMAGEHLP_MODULE;


/*
typedef struct _SYMBOL_INFO {
    ULONG       SizeOfStruct;
    ULONG       TypeIndex;        // Type Index of symbol
    ULONG64     Reserved[2];
    ULONG       Index;
    ULONG       Size;
    ULONG64     ModBase;          // Base Address of module comtaining this symbol
    ULONG       Flags;
    ULONG64     Value;            // Value of symbol, ValuePresent should be 1
    ULONG64     Address;          // Address of symbol including base address of module
    ULONG       Register;         // register holding value or pointer to value
    ULONG       Scope;            // scope of the symbol
    ULONG       Tag;              // pdb classification
    ULONG       NameLen;          // Actual length of name
    ULONG       MaxNameLen;
    CHAR        Name[1];          // Name of symbol
} SYMBOL_INFO, *PSYMBOL_INFO;

*/


//this original above commented out structure definition was butchered by me 
//fubared for making it work with ollydbg see below


// fubared symbol info structure 
typedef struct _SYMBOL_INFO {
    ULONG       SizeOfStruct;
    CHAR	butchered[0x34];
    ULONG       Address;
    CHAR	butcheredagain[0x18];
    CHAR	Name[1];
}SYMBOL_INFO, *PSYMBOL_INFO;




typedef BOOL 	(CALLBACK	*PSYM_ENUMERATESYMBOLS_CALLBACK)(
	PSYMBOL_INFO 	pSymInfo,
	ULONG		SymbolSize,
	PVOID 		UserContext
);

typedef BOOL 	(WINAPI		*PSymInitialize)(
	HANDLE 		hProcess,
	PCSTR 		UserSearchPath,
 	BOOL 		fInvadeProcess
);

typedef DWORD	(WINAPI		*PSymLoadModule)(
	HANDLE 		hProcess,
	HANDLE 		hFile,
	PCSTR 		ImageName,
	PCSTR 		ModuleName,
	DWORD 		BaseOfDll,
	DWORD 		SizeOfDll
);

typedef BOOL 	(WINAPI		*PSymGetModuleInfo)(
	HANDLE 		hProcess,
	DWORD 		dwAddr,
	PIMAGEHLP_MODULE ModuleInfo
);

typedef BOOL	(WINAPI		*PSymEnumSymbols)(
	HANDLE 		hProcess,
	ULONG64 	BaseOfDll,
	PCSTR 		Mask,
	PSYM_ENUMERATESYMBOLS_CALLBACK EnumSymbolsCallback,
	PVOID UserContext
);

typedef BOOL 	(WINAPI		*PSymUnloadModule)(
	HANDLE 		hProcess,
	DWORD 		BaseOfDll
);

typedef BOOL 	(WINAPI		*PSymCleanup)(
	HANDLE 		hProcess
);

typedef DWORD 	(WINAPI		*PSymSetOptions)(
	DWORD 		SymOptions
);

BOOL CALLBACK SYM_ENUMERATESYMBOLS_CALLBACK(
	PSYMBOL_INFO	pSymInfo,
	ULONG 		SymbolSize,
	PVOID 		UserContext
);



int Loadpdb(char *answer,ulong parm)

{
	PSYM_ENUMERATESYMBOLS_CALLBACK	psymcallback = SYM_ENUMERATESYMBOLS_CALLBACK;
	
	PSymInitialize			psyminit;
	PSymLoadModule			psymloadmodule;
	PSymGetModuleInfo		psymgetmoduleinfo;
	PSymEnumSymbols			psymenumsymbols;
	PSymUnloadModule		psymunloadmodule;
	PSymSetOptions			psymsetoptions;
	PSymCleanup			psymcleanup;
	
	HINSTANCE			dbghlp_dll;
	HANDLE				hProcess; 
	DWORD				dbgpid;
	DWORD 				dwModuleBase;
	IMAGEHLP_MODULE			im;
	CHAR 				filename[TEXTLEN];
	CHAR				lpname[] = {"_NT_SYMBOL_PATH"};
	CHAR				lpbuffer[4];
	DWORD				nsize = (sizeof(lpbuffer)-1);
	DWORD				dwretfake;
	DWORD				dwret;
	char 				*hMem;
		


	dwretfake	= GetEnvironmentVariable(lpname,lpbuffer,nsize);
	if((dwretfake ==0))
	{
		MessageBox(NULL,"YOU have to set _NT_SYMBOL_PATH to load pdbs from this plugin","ERROR",NULL);
		return 1;
	}
	else if((dwretfake > nsize))
	{
		
		hMem		= (CHAR *)VirtualAlloc(NULL,(dwretfake+100),MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE); 
		dwret		= GetEnvironmentVariable(lpname,hMem,dwretfake+10);
	}
	Addtolist(0,1,"%x %s",dwret,hMem);

	memset(&im,0,sizeof(im));	// see comment below this sets only 0 for 239 bytes 

	im.SizeOfStruct = 0x23c;	// hack coz of packed struct i think  enum SYM_TYPE Is proabbly treated a 1 byte
					// whereas default is probably (dword align) 4 bytes so sizeof for me return 0x239 while dbghlp checks for 0x23c

	strncpy(filename,string,(TEXTLEN-1)); 
	dbgpid		= Plugingetvalue(VAL_PROCESSID);
	hProcess	= OpenProcess(PROCESS_ALL_ACCESS,FALSE,dbgpid);
	
	dbghlp_dll = LoadLibrary("dbghelp.dll");
	
	if((dbghlp_dll))
	{
		psyminit 		= (PSymInitialize) 	GetProcAddress(dbghlp_dll,"SymInitialize");
		psymloadmodule		= (PSymLoadModule) 	GetProcAddress(dbghlp_dll,"SymLoadModule");
		psymsetoptions		= (PSymSetOptions)	GetProcAddress(dbghlp_dll,"SymSetOptions");
		psymgetmoduleinfo	= (PSymGetModuleInfo)	GetProcAddress(dbghlp_dll,"SymGetModuleInfo");
		psymenumsymbols		= (PSymEnumSymbols)	GetProcAddress(dbghlp_dll,"SymEnumSymbols");
		psymunloadmodule	= (PSymUnloadModule)	GetProcAddress(dbghlp_dll,"SymUnloadModule");
		psymcleanup		= (PSymCleanup)		GetProcAddress(dbghlp_dll,"SymCleanup");
		if((psyminit) && (psymloadmodule) && (psymsetoptions) && (psymgetmoduleinfo) && (psymenumsymbols) && (psymunloadmodule) &&(psymcleanup) )
		{
			Addtolist(0,1,"dbghelp dll loaded and address retrieved");
		}
		else
		{
			Addtolist(0,1,"dbghelp loaded but get proc failed");
			VirtualFree(hMem,0,MEM_RELEASE);
			CloseHandle(hProcess);
			return 1;
		}
	}
	else
	{
		Addtolist(0,1,"load lib failed");
		VirtualFree(hMem,0,MEM_RELEASE);
		CloseHandle(hProcess);
		return 1;
	}
		

    if ( !psyminit( hProcess, hMem, FALSE ) )
    {
       	Addtolist(0,1, "SymInitialize failed" );
	VirtualFree(hMem,0,MEM_RELEASE);
	CloseHandle(hProcess);
        return 1;
    }
	
	psymsetoptions(0x80000003);
	dwModuleBase = psymloadmodule( hProcess, 0, filename, 0,0,0);

    if ( !dwModuleBase )
    {
       	Addtolist(0,1, "SymLoadModuleFailed" );
	VirtualFree(hMem,0,MEM_RELEASE);
	CloseHandle(hProcess);
        return 1;
    }

	
	psymgetmoduleinfo( hProcess, dwModuleBase, &im );

	if ( im.SymType == SymExport )
	{
		Addtolist(0,1, "Only Export symbols - skipping");
		VirtualFree(hMem,0,MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}

	if ( im.SymType == SymNone )
	{
		Addtolist(0,1, "No Symbols - skipping");
		VirtualFree(hMem,0,MEM_RELEASE);
		CloseHandle(hProcess);
		return FALSE;
	}



    psymenumsymbols( hProcess, dwModuleBase, 0, psymcallback, 0 );
    psymunloadmodule( hProcess, dwModuleBase );
    psymcleanup( hProcess );   
    VirtualFree(hMem,0,MEM_RELEASE);
    CloseHandle(hProcess);
    return 0;
}



#pragma argsused
BOOL CALLBACK SYM_ENUMERATESYMBOLS_CALLBACK( PSYMBOL_INFO  pSymInfo, ULONG SymbolSize, PVOID UserContext )
{

	
	
	Insertname(pSymInfo->Address,NM_LIBRARY,pSymInfo->Name);
	return TRUE;
}













