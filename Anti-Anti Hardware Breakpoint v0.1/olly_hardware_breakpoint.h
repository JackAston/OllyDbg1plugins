/*
    Ollydbg Plugin  - Anti Anti Hardware Breakpoint v0.1
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

#define __PATH__ "ravioli.dll"
#define __DLLNAME__ "ravioli.dll"

// Color for the list
#define __ERROR__ 	1



// Globals Variable
HINSTANCE		hInstance;
HWND			hWMain;
unsigned char	dllclass[32];
unsigned int	ishooked = 0;
unsigned int	failed = 1;
unsigned int	injected = 0;
unsigned char	buffer[12];

unsigned long find_dllbase_by_process32(unsigned char *process,unsigned char *dll);
unsigned int RemoteLoadLibrary(HANDLE hProcess, unsigned char *szDll);
unsigned int SetUpHook();
unsigned int SetDownHook();