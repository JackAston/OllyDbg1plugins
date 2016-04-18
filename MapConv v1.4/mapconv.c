// MapConv 1.04
// Purpose: converts IDA and Dede map files to OllyDbg
// by godfather+
// modified by TBD and SHaG

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR!

#include <windows.h>
#include <stdio.h>
#include <string.h>
// #include <dir.h>

#include "plugin.h"
DWORD GetCurrentEIP(void);

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window
byte usecomment, uselabel;
t_module *pmodule;

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;                          // Mark plugin instance
  return 1;                            // Report success
};

int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"MapConv");       // Name of plugin
  return PLUGIN_VERSION;
};

int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION)
    return -1;
  hwmain=hw;
  Addtolist(0,0,"MapConv ver 1.4 by godfather+, TBD and SHaG");
  return 0;
};

int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)  {
  int i,n;
  t_dump *pd;
  switch (origin) {
    case PM_MAIN:                      // Plugin menu in main window
      strcpy(data,"0 Replace label, 1 Replace comment |3 &About");
      return 1;
    default: break;                    // Any other window
  };
  return 0;                            // Window not supported by plugin
};

void information(char *message)
{
  MessageBox(hwmain,message, "MapConv v1.4",MB_OK|MB_ICONINFORMATION);
}

DWORD GetCurrentEIP(void)
{
  t_thread* t2;// t_thread

  t2=Findthread(Getcputhreadid());
  return t2->reg.ip;
}

void LoadMap(int what)
{
  char path[255];
  FILE *in;

  char mapseg[8];
  char buff[20];
  char *mapoff = "0x12345678", *endptr;
  long lnumber;

  char maplabel[50];
  char mapline[TEXTLEN];

  char *str2;
  int i,j, addmenuid, mnuitems;

  str2 = Plugingetvalue(VAL_PROCESSNAME);    //some checks
  if (strlen(str2) == 0)
    {
     Addtolist (0,1,"MapConv ERROR: No process to add map info");
     information("Well - if you don't debug anything - your don't need .map file ;-)");
     return;
    }
  strcpy(path,".\\");
  Browsefilename("Select map file",path,".map",0);

  if (stricmp(path, ".\\") == 0)        //just a litle check
    return;

  if ((in = fopen(path, "rt"))== NULL)
    Addtolist (0,1,"MapConv ERROR: Cannot open %s", path);
  else
      {
      while (!feof(in))
        {
        strcpy(mapseg, " 0001:");
        if ((strstr(mapline, mapseg) != NULL) && (strstr(mapline, "CODE") == NULL) && (strstr(mapline, "Program entry") == NULL))
          {
            str2 = mapoff;
            str2++;
            str2++;
            for (i=6;i<14;i++)
             *str2++ = mapline[i];

            *str2++ = '\0';

            str2 = maplabel;
            j = strlen(mapline)-1;
            for (i=0x15;i<j;i++)
             *str2++ = mapline[i];

            *str2++ = '\0';
            lnumber = strtol(mapoff, &endptr, 16);

			pmodule = Findmodule((ulong)GetCurrentEIP());

            if (what == 0)
             Insertname(pmodule->codebase + lnumber , NM_LABEL, maplabel);
            else
             Insertname(pmodule->codebase + lnumber , NM_COMMENT, maplabel);

          };
        fgets(mapline, 256, in);
        };
      fclose(in);
      if (what == 0) {
        Addtolist(0,0,"MapConv: OK: Map file successfuly imported - labels updated");
        information("Map file successfuly imported - labels updated");
        }
      else  {
        Addtolist(0,0,"MapConv: OK: Map file successfuly imported - comments updated");
        information("Map file successfuly imported - comments updated");
        }
      };

  Setcpu(0,0,0,0,CPU_ASMFOCUS);
}

void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin==PM_MAIN) {
    switch (action) {
      case 0:
        LoadMap(0);
        break;
      case 1:
        LoadMap(1);
        break;
      case 2:
        break;
      case 3:
        // Menu item "About", displays plugin info.
        information("MapConv v1.4 \nby godfather+\nWith help from TBD and SHaG");
        break;
      default: break;
    }; }
  };
;


