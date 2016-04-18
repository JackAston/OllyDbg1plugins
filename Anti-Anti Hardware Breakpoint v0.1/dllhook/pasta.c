/*
    Ollydbg Plugin  - Anti Anti Hardware Breakpoint v0.1 Dll !
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

typedef struct _DebugRegister {
DWORD Dr0;
DWORD Dr1;
DWORD Dr2;
DWORD Dr3;
DWORD Dr6;
DWORD Dr7;
} DebugRegister;

DebugRegister 		DRx;
DebugRegister 		_DRx;

unsigned long if_changed		= 0;


typedef int (__cdecl *SEHandler)(DWORD ExceptionRecord, DWORD EstablisherFrame, LPCONTEXT ContextRecord, DWORD DispatcherContext);
//int __declspec(dllexport) __stdcall NewExecute2(DWORD ExceptionRecord, DWORD EstablisherFrame, LPCONTEXT ContextRecord, DWORD DispatcherContext,SEHandler Handler);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {

	return 1;

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int __declspec(dllexport) __stdcall NewExecute2(DWORD ExceptionRecord, DWORD EstablisherFrame, LPCONTEXT ContextRecord, DWORD DispatcherContext,SEHandler Handler) {
//DWORD FckHandler;

	//_asm mov FckHandler, edx

	DRx.Dr0 = ContextRecord->Dr0;
	DRx.Dr1 = ContextRecord->Dr1;
	DRx.Dr2 = ContextRecord->Dr2;
	DRx.Dr3 = ContextRecord->Dr3;
	DRx.Dr6 = ContextRecord->Dr6;
	DRx.Dr7 = ContextRecord->Dr7;

	if(if_changed) {
		ContextRecord->Dr0 = _DRx.Dr0;
		ContextRecord->Dr1 = _DRx.Dr1;
		ContextRecord->Dr2 = _DRx.Dr2;
		ContextRecord->Dr3 = _DRx.Dr3;
		ContextRecord->Dr6 = _DRx.Dr6;
		ContextRecord->Dr7 = _DRx.Dr7;
	}
	
	//printf("Eip: 0x%08X // SEH Handler : 0x%08X\n",ContextRecord->Eip, Handler);

	/*
	_asm {
		push    dword ptr ss:[ebp+0xC]
		push    FckHandler
		push    dword ptr fs:[0]
		mov     dword ptr fs:[0],esp
	}
	*/
	Handler(ExceptionRecord, EstablisherFrame, ContextRecord, DispatcherContext);
	/*
	_asm {
		mov     esp,dword ptr fs:[0]
		pop     dword ptr fs:[0]
	}
	*/
	if((ContextRecord->Dr7 == 0x155) && (if_changed == 0)) {

	_DRx.Dr0 = ContextRecord->Dr0;
	_DRx.Dr1 = ContextRecord->Dr1;
	_DRx.Dr2 = ContextRecord->Dr2;
	_DRx.Dr3 = ContextRecord->Dr3;
	_DRx.Dr6 = ContextRecord->Dr6;
	_DRx.Dr7 = ContextRecord->Dr7;

	ContextRecord->Dr0 = DRx.Dr0;
	ContextRecord->Dr1 = DRx.Dr1;
	ContextRecord->Dr2 = DRx.Dr2;
	ContextRecord->Dr3 = DRx.Dr3;
	ContextRecord->Dr6 = DRx.Dr6;
	ContextRecord->Dr7 = DRx.Dr7;

	if_changed = 1;

	} else if (if_changed) {

		ContextRecord->Dr0 = DRx.Dr0;
		ContextRecord->Dr1 = DRx.Dr1;
		ContextRecord->Dr2 = DRx.Dr2;
		ContextRecord->Dr3 = DRx.Dr3;
		ContextRecord->Dr6 = DRx.Dr6;
		ContextRecord->Dr7 = DRx.Dr7;

	} /* else {
	
		if(ContextRecord->Dr0 != DRx.Dr0)
			//olly_add_to_list(0,__ERROR__,"==> The Dr0 have been changed !");

		if (ContextRecord->Dr1 != DRx.Dr1)
			//olly_add_to_list(0,__ERROR__,"==> The Dr1 have been changed !");

		if (ContextRecord->Dr2 != DRx.Dr2)
			//olly_add_to_list(0,__ERROR__,"==> The Dr2 have been changed !");

		if (ContextRecord->Dr3 != DRx.Dr3)
			//olly_add_to_list(0,__ERROR__,"==> The Dr3 have been changed !");

		if (ContextRecord->Dr6 != DRx.Dr6)
			//olly_add_to_list(0,__ERROR__,"==> The Dr6 have been changed !");

		if (ContextRecord->Dr7 != DRx.Dr7)
			//olly_add_to_list(0,__ERROR__,"==> The Dr7 have been changed !");

	} */

	return 0;

}