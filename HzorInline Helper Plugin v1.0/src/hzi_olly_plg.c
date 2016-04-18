/*
    Ollydbg Plugin  - HzorInline Helper Plugin v1.0
    Copyright (C) 2007 arnix <arnix.at@gmail.com>

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

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "plugin.h"
#include "hzi_olly_plg.h"

HINSTANCE hInstance;
HWND hWMain;
unsigned char hziclass[32];
t_table hzpatches;
unsigned int index = 0;

BOOL WINAPI DllEntryPoint(HINSTANCE hi, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        hInstance = hi;
    }
    return 1;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
    strcpy(shortname,"HzorInline Helper");
    return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw, ulong *features)
{
    if (ollydbgversion < PLUGIN_VERSION)
    {
        return -1;
    }

    if (Createsorteddata(&(hzpatches.data), "HzorInline Patches List", sizeof(t_hzpatch),10, (SORTFUNC*)hzpatches_sortfunc, (DESTFUNC*)hzpatches_destr) != 0)
    {
        return -1;
    }

    if (Registerpluginclass(hziclass, NULL, hInstance, hzpatches_winproc) < 0)
    {
        Destroysorteddata(&(hzpatches.data));
        return -1;
    }

    Addtolist(0, 0,  "HzorInline Helper Plugin " HZH_VERSION " Loaded (Compiled on " DT ")");
    Addtolist(0, -1, "  Copyright (c) 2007 - arnix [arnix.at@gmail.com]");

    return 0;
}

int hzpatches_sortfunc(t_hzpatch *hzp1, t_hzpatch *hzp2, int sort)
{
    switch (sort)
    {
        case 0:
            return ((hzp1->addr < hzp2->addr) ? -1 : ((hzp1->addr > hzp2->addr) ? 1 : 0));
            break;
        case 1:
            return ((hzp1->va < hzp2->va) ? -1 : ((hzp1->va > hzp2->va) ? 1 : 0));
            break;
        case 2:
            return ((hzp1->count < hzp2->count) ? -1 : ((hzp1->count > hzp2->count) ? 1 : 0));
            break;
        case 3:
            return ((hzp1->enabled < hzp2->enabled) ? -1 : ((hzp1->enabled > hzp2->enabled) ? 1 : 0));
            break;
        default:
            break;
    }
}

void hzpatches_destr(t_sortheader *pe)
{
    t_hzpatch *pb = (t_hzpatch*)pe;
      
    free(pb->data);
    pb->count = 0;
    pb->va = 0;
}

LRESULT CALLBACK hzpatches_winproc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
    HMENU menu;
    t_hzpatch *pb;
    int menuop, i, j, n;
    unsigned int mem_size;
    HGLOBAL hClipData;
    PTSTR pClipData;
    
    switch (msg)
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
            Tablefunction(&hzpatches, hw, msg, wp, lp);
            break;

        case WM_USER_SCR:
        case WM_USER_VABS:
        case WM_USER_VREL:
        case WM_USER_VBYTE:
        case WM_USER_STS:
        case WM_USER_CNTS:
        case WM_USER_CHGS:
            return Tablefunction(&hzpatches, hw, msg, wp, lp);
            
        // If window should support TABLE_ONTOP ("Always on top" mode), it must pass
        // WM_WINDOWPOSCHANGED to Tablefunction().
        case WM_WINDOWPOSCHANGED:
            //return Tablefunction(&bookmark, hw, msg, wp, lp);
            break;
        case WM_USER_MENU:
            menu = CreatePopupMenu();
            pb = (t_hzpatch*)Getsortedbyselection(&(hzpatches.data), hzpatches.data.selected);
            if (menu != NULL)
            {
                if (pb != NULL)
                {
                    AppendMenu(menu, MF_STRING, 1, (pb->enabled == 1) ? "&Disable" : "&Enable");
                    AppendMenu(menu, MF_STRING, 2, "&Copy All");
                }
            }
            menuop = Tablefunction(&hzpatches, hw, WM_USER_MENU, 0, (LPARAM)menu);
            if (menu != NULL)
            {
                DestroyMenu(menu);
            }
            switch (menuop)
            {
                case 1:
                    pb->enabled = ~pb->enabled;
                    InvalidateRect(hw, NULL, FALSE); 
                    break;
                case 2:
                    OpenClipboard(NULL); 
                    EmptyClipboard();
                    pb = (t_hzpatch*)hzpatches.data.data;
                    mem_size = 0;
                    for (i = 0; i < hzpatches.data.n; i++, pb++)
                    {
                        mem_size += 11 + (pb->count * 3);
                    }
                    mem_size++;
                    hClipData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, mem_size);
                    pClipData = (PTSTR)GlobalLock(hClipData);
                    pb = (t_hzpatch*)hzpatches.data.data;
                    n = 0;
                    for (i = 0; i < hzpatches.data.n; i++, pb++)
                    {
                        if ((pb->count > 0) && (pb->enabled == 1))
                        {
                            n += sprintf(pClipData + n, "P,%08X,", pb->va);
                            for (j = 0; j < pb->count; j++)
                            {
                                n += sprintf(pClipData + n, "%02X,", pb->data[j]);
                            }
                            n--;
                            n += sprintf(pClipData + n, "\n");
                        }
                    }
                    SetClipboardData(CF_TEXT, hClipData);
                    CloseClipboard();
                    GlobalFree(hClipData);
                    break;
                default:
                    break;
            }
            return 0;
        case WM_KEYDOWN:
        case WM_USER_DBLCLK:
            Tablefunction(&hzpatches, hw, msg, wp, lp);
            break;


        case WM_USER_CHALL:
        case WM_USER_CHMEM:
            InvalidateRect(hw, NULL, FALSE);
            return 0;
      
        case WM_PAINT:
          Painttable(hw, &hzpatches, hzpatches_gettext);
          return 0;
        default:
            break;
    }
    return DefMDIChildProc(hw, msg, wp, lp);
}

int hzpatches_gettext(char *s, char *mask, int *select, t_sortheader *ph, int column)
{
    int i, n;
    t_hzpatch *pb = (t_hzpatch*)ph;

    *select = DRAW_NORMAL;
    if (column == 0)
    {
        n = sprintf(s, "%d", pb->addr);
    }
    else if (column == 1)
    {
        n = sprintf(s, "%08X", pb->va);
    }
    else if (column == 2)
    {
        n = 0;
        if (pb->count > 0)
        {
            for (i = 0; i < pb->count; i++)
            {
                n += sprintf(s + n, "%02X,", pb->data[i]);
            }
            n--;
        }
    }
    else if (column == 3)
    {
        n = sprintf(s, (pb->enabled == 1) ? "Enabled" : "Disabled");
    }
    else
    {
        n = 0;
    }
    return n;
}

void create_hzpatches_window(void)
{
    if (hzpatches.bar.nbar == 0)
    {
        hzpatches.bar.name[0] = "N";
        hzpatches.bar.defdx[0] = 5;
        hzpatches.bar.mode[0] = 0;
        
        hzpatches.bar.name[1] = "Patch Address";
        hzpatches.bar.defdx[1] = 16;
        hzpatches.bar.mode[1] = 0;
        
        hzpatches.bar.name[2] = "Patched bytes";
        hzpatches.bar.defdx[2] = 80;
        hzpatches.bar.mode[2] = 0;

        hzpatches.bar.name[3] = "Status";
        hzpatches.bar.defdx[3] = 10;
        hzpatches.bar.mode[3] = 0;

        hzpatches.bar.nbar = 4;
        hzpatches.mode = TABLE_SORTMENU;

        hzpatches.drawfunc = hzpatches_gettext;
    }
    Quicktablewindow(&hzpatches, 10, hzpatches.bar.nbar, hziclass, "HzorInline Patches List");
}

extc int _export cdecl ODBG_Pluginmenu(int origin, char data[4096], void *item)
{
    t_dump *pd;

    switch (origin)
    {
        case PM_MAIN:
            sprintf(data, "0 About...");
            return 1;
        case PM_DUMP:
        case PM_DISASM:
            pd = (t_dump*)item;
            if ((pd != NULL) && (pd->size > 0) && ((pd->sel1 - pd->sel0) > 0))
            {
                sprintf(data, "HzorInline Helper{0 Add to list}");
                return 1;
            }
            else
            {
                return 0;
            }
        default: 
            break;
    }
    return 0;
}

extc void _export cdecl ODBG_Pluginaction(int origin, int action, void *item)
{
    t_dump *pd;
    HWND pw;
    unsigned int region_size;
    t_hzpatch new_hzpatch;

    switch (origin)
    {
        case PM_MAIN:
            switch (action)
            {
                case 0:
                    MessageBox(hWMain, "HzorInline Helper Plugin " HZH_VERSION "\n"
                                       "\n"
                                       "Build date and time: " DT "\n"
                                       "Programmed by arnix [arnix.at@gmail.com]\n"
                                       "Compiled with Open Watcom C/C++ v1.6\n"
                                       ,
                                       "HzorInline Helper Plugin - About",
                                       MB_OK|MB_ICONINFORMATION);
                    break;
                default: 
                    break;
            }
            break;

        case PM_DUMP:
        case PM_DISASM:
            switch (action)
            {
                case 0:
                    pd = (t_dump*)item;
                    region_size = pd->sel1 - pd->sel0;
                    if (region_size < 1)
                    {
                        MessageBox(hWMain, "You should select some bytes before...", "Error", MB_OK + MB_ICONERROR);
                    }
                    else
                    {
                        new_hzpatch.addr = index++;
                        new_hzpatch.size = 1;
                        new_hzpatch.type = 0;
                        
                        new_hzpatch.count = region_size;
                        new_hzpatch.va = pd->sel0;
                        new_hzpatch.data = malloc(region_size);
                        new_hzpatch.enabled = 1;
                        Readmemory(new_hzpatch.data, pd->sel0, region_size, MM_RESILENT);
                        Addsorteddata(&(hzpatches.data), &new_hzpatch);
                        create_hzpatches_window();
                        if (hzpatches.hw != NULL)
                        {
                            InvalidateRect(hzpatches.hw, NULL, FALSE);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
    }
}

extc void _export cdecl ODBG_Pluginreset(void)
{
    Deletesorteddatarange(&(hzpatches.data), 0, 0xFFFFFFFF);
}

extc int _export cdecl ODBG_Pluginclose(void)
{
    return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void)
{
    Unregisterpluginclass(hziclass);
    Destroysorteddata(&(hzpatches.data));
}
