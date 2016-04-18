/*
	OllyDbg /SafeSEH Module Scanner 

	Copyright (C) 2007 Mario Ballano <mballano@gmail.com> http://www.48bits.com/

	This program is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2 of the License, or (at your option)
	any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA
*/

#define _CRT_SECURE_NO_DEPRECATE

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Plugin.h"

#define ERROR_READING_SEH -1
#define SAFESEH_OFF        0
#define SAFESEH_ON         1       
#define NOSEH              2

HINSTANCE hinst;
HWND      hwmain;
char      safesehwinclass[32];
char      handlerwinclass[32];

typedef struct
{
	ulong  index;
	ulong  size;
	ulong  type;
	DWORD  address;
}t_handler;

typedef struct
{
	ulong     index;
	ulong     size;
	ulong     type;
	DWORD     base;
	DWORD     limit;
	char      modversion[NVERS];
	char      blockname[MAX_PATH];
	DWORD     sseh_enabled;
	DWORD     nHandlers;
	t_table   *handlers;
} t_sseh;

t_table   safeseh;
t_table   *current_htable;

//
void SSEH_Dtor(t_sortheader *pItem);
//

#define IS_CONTAINED(p1,s1,p2,s2)( ( (LPBYTE)(p1) >= (p2) ) && \
								   ( (LPBYTE)(p1) +  (s1) ) <= ( (LPBYTE) (p2) + (s2) ) )

///////////////////////////////////////

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
	if (reason==DLL_PROCESS_ATTACH)
		hinst=hi;                         
	return 1;                            
};

void PluginError(void)
{
	MessageBoxA(hwmain,"Internal plugin error!","Error!",MB_ICONWARNING);
	ExitThread(0);
}

int handlergettext(char *s,char *mask,int *select, t_sortheader *ph,int column) 
{	
	t_handler *handler = (t_handler *) ph;
	int        retval  = 0;
	
	if ( handler && column == 0 )
	{
		retval = sprintf(s,"%#x",handler->address);
	}

	return retval;
	
}

int safesehgettext(char *s,char *mask,int *select, t_sortheader *ph,int column) 
{	
	int retval=0;		
	t_sseh *sseh = (t_sseh *) ph;

	if ( sseh->sseh_enabled == SAFESEH_OFF )
	{
		*select |= DRAW_HILITE;
	}

	switch (column)
	{		
		case 0:
			switch (sseh->sseh_enabled)
			{
				case ERROR_READING_SEH:
					retval = sprintf(s,"Error");
					break;
				case  SAFESEH_OFF:
					retval = sprintf(s,"/SafeSEH OFF");			
					break;
				case  SAFESEH_ON:
					retval = sprintf(s,"/SafeSEH ON");
					break;
				case NOSEH:
					retval = sprintf(s,"No SEH");
			}
			break;
		case 1:
			retval = sprintf(s,"%#x",sseh->base);
			break;
		case 2:
			retval = sprintf(s,"%#x",sseh->limit);
			break;
		case 3:
			retval = sprintf(s,"%s",sseh->modversion);
			break;
		case 4:			
			retval = sprintf(s,"%s",sseh->blockname);
									
	}

	return retval;
}

int handlersortfunc(t_handler *i_1 ,t_handler *i_2, int sort) 
{	
	int retval = 0;

	if (i_1->address > i_2->address)
		retval =  1;
	if (i_1->address < i_2->address)
		retval = -1;

	return retval;
}

int safesehsortfunc(t_sseh *i_1 ,t_sseh *i_2, int sort) 
{	
	int retval = 0;

	switch(sort)
	{
		case 0: // SEH mode

			if ( i_1->sseh_enabled != i_2->sseh_enabled  )
			{
				if (i_1->sseh_enabled == SAFESEH_OFF)
					retval = 1;
				if (i_2->sseh_enabled == SAFESEH_OFF)
					retval = -1;
			}	
			break;

		case 1:  // Address 
			if (i_1->base > i_2->base)
				retval =  1;
			if (i_1->base < i_2->base)
				retval = -1;
			break;

		case 2: // Limit 
			if (i_1->limit > i_2->limit)
				retval =  1;
			if (i_1->limit < i_2->limit)
				retval = -1;
	}

	return retval;
}

LRESULT CALLBACK HandlersWinProc(HWND hw,UINT msg,WPARAM wp,LPARAM lp)
{
	t_handler *handler;

	switch(msg)
	{		
		case WM_DESTROY:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_TIMER:
		case WM_SYSKEYDOWN:
			Tablefunction(current_htable,hw,msg,wp,lp);
			break;		
		case WM_USER_SCR:
		case WM_USER_VABS:
		case WM_USER_VREL:
		case WM_USER_VBYTE:
		case WM_USER_STS:
		case WM_USER_CNTS:
		case WM_USER_CHGS:
		case WM_WINDOWPOSCHANGED:
		return Tablefunction(current_htable,hw,msg,wp,lp);
		case WM_USER_MENU:
		return 0;
		case WM_KEYDOWN:
			if ( !(wp == VK_RETURN) )
			{
				break;
			}
		case WM_USER_DBLCLK:
		
			if ( handler =(t_handler *)Getsortedbyselection(&(current_htable->data),current_htable->data.selected) )
			{
				Setcpu(0,handler->address,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);

			}
			return 1;

		case WM_USER_CHALL:
		case WM_USER_CHMEM:			
			InvalidateRect(hw,NULL,FALSE);
			return 0;
		case WM_PAINT:			
			Painttable(hw,current_htable,handlergettext);
			return 0;
		default:
			break;
	}

	return DefMDIChildProc(hw,msg,wp,lp);
}

void CreateHandlersWindow(t_sseh *sseh)
{
	t_table *handlers;

	handlers = sseh->handlers;	

	if (handlers)
	{
		if (!handlers->bar.nbar)
		{
			handlers->bar.name[0]  = "Handler";
			handlers->bar.defdx[0] = 20;
			handlers->bar.mode[0]  = 0;

			handlers->bar.nbar = 1;
			handlers->mode= TABLE_COPYMENU|TABLE_SORTMENU|TABLE_APPMENU|TABLE_SAVEPOS|TABLE_ONTOP;	
			handlers->drawfunc = handlergettext;		
		}	
		current_htable = sseh->handlers;
		Quicktablewindow(sseh->handlers,15,1,handlerwinclass,"Handlers");
	}
}

LRESULT CALLBACK SafeSEHWinProc(HWND hw,UINT msg,WPARAM wp,LPARAM lp)
{
	HMENU   menu;
	t_sseh *sseh;
	int    i;

	switch(msg)
	{		
		case WM_DESTROY:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_TIMER:
		case WM_SYSKEYDOWN:
			Tablefunction(&safeseh,hw,msg,wp,lp);
			break;		
		case WM_USER_SCR:
		case WM_USER_VABS:
		case WM_USER_VREL:
		case WM_USER_VBYTE:
		case WM_USER_STS:
		case WM_USER_CNTS:
		case WM_USER_CHGS:
		case WM_WINDOWPOSCHANGED:
			return Tablefunction(&safeseh,hw,msg,wp,lp);
		case WM_USER_MENU:			
			menu = CreatePopupMenu();
			sseh = (t_sseh *) Getsortedbyselection(&safeseh.data,safeseh.data.selected);
			if (menu && sseh)
			{			
				AppendMenuA(menu,MF_STRING,1,"&List /SafeSEH registered handlers");
			}

			i = Tablefunction(&safeseh,hw,WM_USER_MENU,0,(LPARAM)menu);

			if (i==1)
			{
				CreateHandlersWindow(sseh);
			}

			return i;

		case WM_USER_CHALL:
		case WM_USER_CHMEM:			
			InvalidateRect(hw,NULL,FALSE);
			return 0;
		case WM_PAINT:			
			Painttable(hw,&safeseh,safesehgettext);
			return 0;
		default:
			break;
	}

	return DefMDIChildProc(hw,msg,wp,lp);
}

void CreateSafeSehWindow(void)
{

	// Build MDI window struct

	if (!safeseh.bar.nbar)
	{
		safeseh.bar.name[0]  = "SEH mode";
		safeseh.bar.defdx[0] = 15;
		safeseh.bar.mode[0]  = 0;

		safeseh.bar.name[1]  = "Base";
		safeseh.bar.defdx[1] = 12;
		safeseh.bar.mode[1]  = 0;

		safeseh.bar.name[2]  = "Limit";
		safeseh.bar.defdx[2] = 12;
		safeseh.bar.mode[2]  = 0;

		safeseh.bar.name[3]  = "Module version";
		safeseh.bar.defdx[3] = NVERS;
		safeseh.bar.mode[3]  = BAR_NOSORT;

		safeseh.bar.name[4]  = "Module Name";
		safeseh.bar.defdx[4] = MAX_PATH;
		safeseh.bar.mode[4]  = BAR_NOSORT;

		safeseh.bar.nbar = 5;
		safeseh.mode= TABLE_COPYMENU|TABLE_SORTMENU|TABLE_APPMENU|TABLE_SAVEPOS|TABLE_ONTOP;	
		safeseh.drawfunc = safesehgettext;		
	}		

	// Show it 

	Quicktablewindow(&safeseh,15,5,safesehwinclass,"/SafeSEH Module Scanner");	
}

int CheckSafeSEH(t_module *module, t_sseh * ssm)
{
	LPBYTE    lpHead;	
	int       retval = SAFESEH_OFF;
	t_handler handler;
	DWORD	  i;	
	
	// Check bounds .. 

	if ( !(module->headersize > sizeof (IMAGE_DOS_HEADER)) ||
		 !(lpHead = malloc(module->headersize)) )
	{
		return ERROR_READING_SEH;
	}

	// Read Module Headers 

	if ( Readmemory(lpHead,module->base,module->headersize, MM_RESTORE | MM_SILENT ) )
	{
		PIMAGE_DOS_HEADER              lpDOSh;
		PIMAGE_NT_HEADERS              lpNTh;
		PIMAGE_DATA_DIRECTORY          lpDD;
		PIMAGE_LOAD_CONFIG_DIRECTORY32 lpLCD;
		DWORD						   *lpHTable;


		// Get NT header 

		lpDOSh = (PIMAGE_DOS_HEADER) lpHead;
		lpNTh  = (PIMAGE_NT_HEADERS) ( (LPBYTE)(lpDOSh) + lpDOSh->e_lfanew );
		
		// Check bounds ..

		if  (!IS_CONTAINED(lpNTh,sizeof(IMAGE_NT_HEADERS),lpHead,module->headersize) )
		{
			free(lpHead);
			return ERROR_READING_SEH;
		}
		
		// Check DllCharacteristics, is SEH enabled for this image?

		if ( lpNTh->OptionalHeader.DllCharacteristics & IMAGE_DLLCHARACTERISTICS_NO_SEH )
		{
			free(lpHead);
			return NOSEH;
		}
	
		// Get Data directory 

		lpDD   = (PIMAGE_DATA_DIRECTORY) &lpNTh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];

		// Check bounds ..
		
		if (!IS_CONTAINED(lpDD,sizeof(IMAGE_DATA_DIRECTORY),lpHead,module->headersize) )
		{
			free(lpHead);
			return ERROR_READING_SEH;
		}

		// We don't need the header anymore

		free(lpHead);

		// Allocate memory for Load Config Directory

		if (! ( lpLCD = malloc(sizeof(IMAGE_LOAD_CONFIG_DIRECTORY)) ) )
		{
			PluginError();
		}

		// Read Load Config Directory 

		if (lpDD->Size && Readmemory (lpLCD, 
						 module->base + lpDD->VirtualAddress,
						 sizeof(IMAGE_LOAD_CONFIG_DIRECTORY), MM_RESTORE | MM_SILENT )  )
		{			
			
			// Do we have a SEH handler table? ;-)

			if ( lpLCD->SEHandlerTable	)
			{
				// Allocate memory for SEHandler Table

				if (! (lpHTable = (DWORD *) malloc( lpLCD->SEHandlerCount * sizeof(DWORD) ) ) )
				{
					free(lpLCD);
					PluginError();
				}								

				// Read SEHandler Table

				if ( ! Readmemory (lpHTable,
					    		   lpLCD->SEHandlerTable,
								   lpLCD->SEHandlerCount * sizeof(DWORD), MM_RESTORE | MM_SILENT ) )
				{			
					
					free(lpHTable);
					free(lpLCD);
					return ERROR_READING_SEH;
				}

				// Allocate memory for handler's table 

				
				if ( (!ssm->handlers) && (!(ssm->handlers = malloc(sizeof(t_table)) )) )
				{
					free(lpHTable);
					free(lpLCD);
					PluginError();
				}
				
				// Zeroing memory  

				memset(ssm->handlers,0,sizeof(t_table));
	
				// Just create a new sorted data for handler addresses.
				
				if ( Createsorteddata(&ssm->handlers->data,
					"handlers",
					sizeof(t_handler),
					10,
					(SORTFUNC *)handlersortfunc,
					NULL ) != 0 )
				{					
					free(lpHTable);
					free(lpLCD);
					PluginError();
				}

				// Add handlers to sorted data
				
				for ( i = 0; i < lpLCD->SEHandlerCount ; i++  )
				{
					handler.index = i;
					handler.size  = 1;
					handler.type  = 0;					
					handler.address = module->base + lpHTable[i];					
					Addsorteddata(&ssm->handlers->data,&handler);
				}

				// Free memory and return success
				
				free(lpHTable );
				free(lpLCD);
				retval = SAFESEH_ON;
			}
		}

		free(lpDD);
	}

	else
	{
		free(lpHead);
		retval = ERROR_READING_SEH;
	}

	return retval;
}

void ScanModules(void)
{
	t_sseh    ssm;
	t_table  *table;
	t_sorted *sorted;
	t_module *module;
	int i;

	// Get modules from Ollydebug internal list

	if ( ( table    = (t_table*) Plugingetvalue(VAL_MODULES)) &&		   
		 ( sorted   = (t_sorted *) &table->data ) &&
		 ( module   = (t_module *) sorted->data ) )
	{		
		// Scan each module, saving data into a sorted list

		for ( i=0 ; i < sorted->n ; i++, module++ )
		{
			ssm.index = i;
			ssm.size  = 1;
			ssm.type  = 0;
			ssm.base  = module->base;
			ssm.limit = module->base + module->size;	
			ssm.handlers = NULL;
			ssm.sseh_enabled = CheckSafeSEH(module,&ssm);
			strcpy (ssm.modversion,module->version);
			strcpy (ssm.blockname, module->path);
			Addsorteddata(&safeseh.data,&ssm);
		}		
	}		

	// Display information 

	CreateSafeSehWindow();
}



extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
	strcpy(shortname,"SafeSEH");
	return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit(int OdbgVersion,HWND hw,ulong *features) 
{
	int retval = -1;

	if ( OdbgVersion >= PLUGIN_VERSION )
	{
		hwmain = hw;

		// Create sorted data for modules information 

		memset(&safeseh,0,sizeof(safeseh));

		if (!Createsorteddata(&(safeseh.data),
			"safeseh",
			sizeof(t_sseh),
			10,
			(SORTFUNC *)safesehsortfunc,
			 SSEH_Dtor ) )
		{
			// Register plugin classes

			if (!Registerpluginclass(safesehwinclass,NULL,hinst,SafeSEHWinProc)) 
			{			 
				if (!Registerpluginclass(handlerwinclass,NULL,hinst,HandlersWinProc))
				{
					retval = 0;
				}
				else
				{
					Unregisterpluginclass(safesehwinclass);
				}
			}

			if (retval)
			{
				Destroysorteddata(&safeseh.data);
			}
		}		
	}	
	return retval;
}

extc int _export cdecl ODBG_Pluginmenu(int origin,char *data,void *item) 
{
	int retval = 0;

	if (origin==PM_MAIN)
	{
		strcpy(data,"0 &Scan /SafeSEH Modules|1 &About");
		retval = 1;
	}
	
	return retval;
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
	if (origin == PM_MAIN)	
	{
		switch (action) 
		{
		case 0:
			// Go go go ! 
			ScanModules();
			break;
		case 1:
			MessageBoxW(hwmain,
				L"/SafeSEH Module Inspector Plugin v0.1\n"
				L" Mario Ballano - mballano@gmail.com\n"
				L" http://www.48bits.com/",
				L"/SafeSEH Module Inspector",
				MB_ICONINFORMATION);		
		};
	}
};

void SSEH_Dtor(t_sortheader *pItem)
{
	t_sseh *sseh = (t_sseh *)pItem;
	
	// Have we allocated memory for handler's table?

	if (sseh->handlers)
	{
		// Destroy sorted data and free pointer 

		Destroysorteddata(&sseh->handlers->data);
		free(sseh->handlers);
	}
}

extc int _export cdecl ODBG_Pluginclose(void) 
{	
	// Unregister both plugin classes and destroy all data (sorted and allocated) 

	Unregisterpluginclass(safesehwinclass);
	Unregisterpluginclass(handlerwinclass);
	Destroysorteddata(&safeseh.data);
	return 0;
}; 