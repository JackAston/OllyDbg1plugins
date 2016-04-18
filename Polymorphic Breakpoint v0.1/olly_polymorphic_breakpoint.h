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

#define __MAX_PMBP__ 999

// Color for the list
#define __ERROR__ 	1

// This is the table for the PolyMorphic Breakpoint's Manager
typedef struct t_polymorphicbreakpoint {

	unsigned long	addr;			// Address of the polymorphic breakpoint
	unsigned int	type;			// See the after
	unsigned int	able;			// Enable or Unable ?
	unsigned long	index;			// Index

} t_polymorphicbreakpoint;

// When a polymorphic breakpoint is setting
typedef struct t_patching {

	unsigned long	addr;			// Address of the polymorphic breakpoint
	unsigned int	size;			// Size of code to restore
	unsigned char	original[16];	// Maximum size of a polymorphic breakpoint is 16

} t_patching;

typedef struct t_reg32 {

	unsigned long eax;
	unsigned long ecx;
	unsigned long edx;
	unsigned long ebx;
	unsigned long esp;
	unsigned long ebp;
	unsigned long esi;
	unsigned long edi;

} t_reg32;

// ...
unsigned char *type[] = {
		"PolyMorphic Int 3",
		"PolyMorphic Jmp Eip(EB FE)",
		"Classical Jmp Eip(EB FE)",
		"Stack PolyMorphic EB FE Breakpoint"
};

unsigned char *polymorph[] = {
	// Polymorphic Int 3
	"\xE8\x01\x00\x00\x00\xCB\xFE\x04\x24\xFF\x14\x24",
	// Polymorphic EB FE
	"\xE8\x02\x00\x00\x00\xDC\xED\x81\x34\x24\x37\x13\x00\x00\xFF\x24\x24",
	// Classical EB FE
	"\xEB\xFE",
	// (Stack) PolyMorphic Jmp Eip (EB FE)
	"\xE8\x00\x00\x00\x00\x66\x68\x21\xFE\x80\x34\x24\xCA\xFF\xE4"
};

const int size[4] = {
			12,
			17,
			2,
			15
};

unsigned char *nable[] = {
		"Disable",
		"Enable"
};

// Globals Variable
HINSTANCE			hInstance;
HWND				hWMain;
unsigned char		dllclass[32];

// Ollydbg Variable
t_table				breakpoint;
t_patching			tpatch[__MAX_PMBP__];
unsigned int		numberofpatch = 0;
unsigned int		ispaused = 1;
CONTEXT				context;

// Functions
LRESULT CALLBACK polybp_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp);

int polybp_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column);
int polybp_sort_function(void);

void BreakpointManager(void);

int AddBreakpoint(unsigned long addr, unsigned int type);
int DisableBreakpoint(t_polymorphicbreakpoint *crt_breakpoint);

