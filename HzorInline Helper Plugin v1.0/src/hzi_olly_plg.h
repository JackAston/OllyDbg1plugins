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

#define HZH_VERSION "v1.0"
#define DT __DATE__ " " __TIME__

typedef struct t_hzpatch
{
    unsigned int addr, size, type, count, va, enabled;
    char *data;
} t_hzpatch;

int hzpatches_sortfunc(t_hzpatch *hzp1, t_hzpatch *hzp2, int sort);
void hzpatches_destr(t_sortheader *pe);
LRESULT CALLBACK hzpatches_winproc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
int hzpatches_gettext(char *s, char *mask, int *select, t_sortheader *ph, int column);;
void create_hzpatches_window(void);
