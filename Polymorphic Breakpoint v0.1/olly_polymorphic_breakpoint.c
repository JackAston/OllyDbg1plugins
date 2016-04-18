/*
    Ollydbg Plugin  - Polymorphic Breakpoint Manager
    Copyright (C) 2005 Mattwood^FRET <mattwood9@gmail.com>

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

#include <windows.h>
#include <stdio.h>
#include <string.h>

#define _CHAR_UNSIGNED 1 // huhu

#include "plugin.h"
#include "olly_redefines.h"
#include "olly_polymorphic_breakpoint.h"
#include "olly_functions.c"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {

	if (reason==DLL_PROCESS_ATTACH)
		hInstance=hi;						// Mark plugin instance

	return 1;								// Report success

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int polybp_sort_function(void) {

	return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

LRESULT CALLBACK polybp_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {

HMENU menu;
unsigned int i;
unsigned long retaddr;
t_polymorphicbreakpoint	*slt_breakpoint;
t_thread *pthread;

	switch (msg) {
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
		case WM_USER_SCR:
		case WM_USER_VABS:
		case WM_USER_VREL:
		case WM_USER_VBYTE:
		case WM_USER_STS:
		case WM_USER_CNTS:
		case WM_USER_CHGS:
		case WM_WINDOWPOSCHANGED:
			return olly_table_function(&breakpoint,hw,msg,wp,lp);

		case WM_USER_MENU:

			slt_breakpoint=(t_polymorphicbreakpoint *)olly_get_sorted_by_selection(&(breakpoint.data),breakpoint.data.selected);
			menu=olly_create_popup_menu();

			if (menu != 0 && slt_breakpoint != 0) {

				olly_append_menu(menu,MF_STRING,1,"Follow\tEnter");
				olly_append_menu(menu,MF_STRING,2,"Disable\tDel");

				i = olly_table_function(&breakpoint,hw,WM_USER_MENU,0,(LPARAM)menu);

					if (menu != 0) DestroyMenu(menu);

					if(i==1) olly_set_cpu(0,slt_breakpoint->addr,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);

					if(i==2) {
						/* Remove a PolyMorphic Breakpoint */

						if((slt_breakpoint->able == 1) && (ispaused == 1)) {
								
							switch(slt_breakpoint->type) {

							case 0:
								pthread=olly_find_thread(olly_get_cpu_thread_id());
								context = pthread->context;
								
								if((context.Eip >= slt_breakpoint->addr+4) && (context.Eip < slt_breakpoint->addr + 12)) {
									
									// Ajust esp
									context.Esp += 4;
									pthread->reg.r[4] += 4;
									// Restore eip
									context.Eip = slt_breakpoint->addr;
									pthread->reg.ip = slt_breakpoint->addr;
									

								}
									// Delete
									DisableBreakpoint(slt_breakpoint);
							break;
							
							case 1:
								pthread=olly_find_thread(olly_get_cpu_thread_id());
								context = pthread->context;
								
								if((context.Eip >= slt_breakpoint->addr+4) && (context.Eip < slt_breakpoint->addr + 17)) {

									// Ajust esp
									context.Esp += 4;
									pthread->reg.r[4] += 4;
									// Restore eip
									context.Eip = slt_breakpoint->addr;
									pthread->reg.ip = slt_breakpoint->addr;

								}
									// Delete
									DisableBreakpoint(slt_breakpoint);

							break;

							case 2:
								
									// Delete
									DisableBreakpoint(slt_breakpoint);

							break;

							case 3:
								pthread=olly_find_thread(olly_get_cpu_thread_id());
								context = pthread->context;

								if((context.Eip >= slt_breakpoint->addr+4) && (context.Eip < slt_breakpoint->addr + 15)) {

									// Ajust esp
									context.Esp += 4;
									pthread->reg.r[4] += 4;
									// Restore eip
									pthread->reg.ip = slt_breakpoint->addr;
									context.Eip = slt_breakpoint->addr;


								}
								else if(context.Eip == context.Esp) {

									if(Readmemory(&retaddr,pthread->reg.r[4]+2,4,MM_RESILENT) == 0) {
										olly_add_to_list(0, __ERROR__, "[Error at %x08X] Can't read the memory.", pthread->reg.r[4]+2);
										break;
									}
									if(retaddr == slt_breakpoint->addr+5) {

									// Ajust esp
									context.Esp += 6;
									pthread->reg.r[4] += 6;
									// Restore eip
									pthread->reg.ip = slt_breakpoint->addr;
									context.Eip = slt_breakpoint->addr;

									}

								}
									// Delete
									DisableBreakpoint(slt_breakpoint);
							break;

							} // switch(slt_breakpoint->type)
						} // pause

					} // ifremove

				InvalidateRect(hw,NULL,FALSE);

		}
		return 0;

		case WM_USER_DBLCLK:
			slt_breakpoint=(t_polymorphicbreakpoint *)olly_get_sorted_by_selection(
				&(breakpoint.data),breakpoint.data.selected);
				if(slt_breakpoint != 0)
					olly_set_cpu(0,slt_breakpoint->addr,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
		return 0;

		case WM_KEYDOWN:
			if (wp==VK_RETURN && (GetKeyState(VK_SHIFT) & 0x8000)==0 && (GetKeyState(VK_CONTROL) & 0x8000)==0) {
				slt_breakpoint=(t_polymorphicbreakpoint *)Getsortedbyselection(
						&(breakpoint.data),breakpoint.data.selected);
						if(slt_breakpoint!=NULL)
							olly_set_cpu(0,slt_breakpoint->addr,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
			}
		return 0;

        case WM_USER_CHALL:
        case WM_USER_CHMEM:
            InvalidateRect(hw, NULL, FALSE);
            return 0;
        case WM_PAINT:
            olly_paint_table(hw, &breakpoint, polybp_get_text);
		return 0;

		default:
		break;
	}

	return DefMDIChildProc(hw,msg,wp,lp);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void BreakpointManager(void) {

	if (breakpoint.bar.nbar==0) {

    breakpoint.bar.name[0]="Address";
    breakpoint.bar.defdx[0]=14;
    breakpoint.bar.mode[0]=0;

	breakpoint.bar.name[1]="Size"; 
    breakpoint.bar.defdx[1]=5;
    breakpoint.bar.mode[1]=0;

    breakpoint.bar.name[2]="Type"; 
    breakpoint.bar.defdx[2]=40;
    breakpoint.bar.mode[2]=0;

    breakpoint.bar.name[3]="Active";
    breakpoint.bar.defdx[3]=20;
    breakpoint.bar.mode[3]=0;

    breakpoint.bar.nbar=4;
    breakpoint.mode=TABLE_COPYMENU|TABLE_SORTMENU|TABLE_APPMENU|TABLE_SAVEPOS|TABLE_ONTOP;
    breakpoint.drawfunc=polybp_get_text;
	}

  olly_quick_table_window(&breakpoint,15,4,dllclass,"PolyMorphic BreakPoint Manager");

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int polybp_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column) {
unsigned int ret;
t_polymorphicbreakpoint *pvis = (t_polymorphicbreakpoint *)ph;

	switch (column) {
		case 0:
			ret = sprintf(s, "0x%08X", pvis->addr);
		break;
		case 1:
			ret = sprintf(s, "%d", size[pvis->type]);
		break;
		case 2:
			ret = sprintf(s, "%s", type[pvis->type]);
		break;
		case 3:
			ret = sprintf(s, "%s", nable[pvis->able]);
		break;
	}

return ret;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int AddBreakpoint(unsigned long addr, unsigned int type) {
t_polymorphicbreakpoint crt_breakpoint;

	if(numberofpatch>=__MAX_PMBP__) {
		olly_add_to_list(0, __ERROR__, "[Error at %x08X] Too many breakpoint are already set.", addr);
		return 0;
	}

	crt_breakpoint.index=numberofpatch;
	crt_breakpoint.addr=addr;
	crt_breakpoint.type=type;
	crt_breakpoint.able=1;	

	if(Readmemory(tpatch[numberofpatch].original, addr, (unsigned long)size[crt_breakpoint.type], MM_RESILENT) == 0) {
		olly_add_to_list(0, __ERROR__, "[Error at %x08X] Can't read the memory.", addr);
		return 0;
	}
	
	if(Writememory(polymorph[crt_breakpoint.type], addr, (unsigned long)size[crt_breakpoint.type], MM_RESILENT) == 0) {
		olly_add_to_list(0, __ERROR__, "[Error at %x08X] Can't write the memory / Set the breakpoint.", addr);
		return 0;
	}

	tpatch[numberofpatch].addr = addr;
	tpatch[numberofpatch].size = size[crt_breakpoint.type];

	olly_add_sorted_data(&(breakpoint.data),&crt_breakpoint);
	olly_add_to_list(0,0,"New breakpoint at 0x%08X", addr);

	numberofpatch++;

	return 1;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


int DisableBreakpoint(t_polymorphicbreakpoint *crt_breakpoint) {

	if(Writememory(tpatch[crt_breakpoint->index].original, tpatch[crt_breakpoint->index].addr,
		tpatch[crt_breakpoint->index].size, MM_RESILENT) == 0) {
		olly_add_to_list(0, __ERROR__, "[Error at %x08X] Can't write the memory / Restore the breakpoint.", crt_breakpoint->addr);
		return 0;
	}

	crt_breakpoint->able = 0;

	olly_add_to_list(0,0,"Breakpoint at %08X(%d) disabled", crt_breakpoint->addr, crt_breakpoint->index);

	return 1;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
