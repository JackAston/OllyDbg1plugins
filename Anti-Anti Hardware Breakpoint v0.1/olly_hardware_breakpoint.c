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

#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

#define _CHAR_UNSIGNED 1 // huhu

#include "plugin.h"
#include "olly_redefines.h"
#include "olly_hardware_breakpoint.h"
#include "olly_functions.c"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

BOOL WINAPI DllEntryPoint(HINSTANCE hi,unsigned long reason,LPVOID reserved) {

	if (reason==DLL_PROCESS_ATTACH) {
		hInstance=hi;						// Mark plugin instance
	}

	return 1;								// Report success

}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// The main part of this plugin :) Try to read it ;)
// Here we patch preExecuteHandler(w2k) or  ExecuteHandler(Wxp)
unsigned int SetUpHook() {

unsigned int i, j;
unsigned long pKiUserExceptionDispatcher, pRtlDispatchException, pRtlpExecuteHandlerForException, pExecuteHandler;
unsigned long pNewExecute2;

	pKiUserExceptionDispatcher = (unsigned long )GetProcAddress(GetModuleHandle("ntdll.dll"), "KiUserExceptionDispatcher");

	for(i=0; ((unsigned char *)(pKiUserExceptionDispatcher))[i] != 0xE8; i++)
		continue;

	pRtlDispatchException = pKiUserExceptionDispatcher+i;

	pRtlDispatchException += ( (unsigned long *)(pRtlDispatchException+1) )[0] + 5;

	j=0;
	i=-1;

	while(!j) {
		i++;
		for(; ((unsigned char *)(pRtlDispatchException))[i] != 0xE8; i++)
			continue;

		if(((unsigned char *)(pRtlDispatchException))[i+5] == 0xF6 &&  ((unsigned char *)(pRtlDispatchException))[i+6] == 0x05)
			j++;
	}

	pRtlpExecuteHandlerForException = pRtlDispatchException+i;
	pRtlpExecuteHandlerForException += ( (unsigned long *)(pRtlpExecuteHandlerForException+1) )[0] + 5;

	injected = find_dllbase_by_process32((unsigned char *)olly_plugin_get_value(VAL_EXEFILENAME),__DLLNAME__);

if((unsigned short)GetVersion() == 0x0005) {
	// --- Win 2k Begin ---
	pExecuteHandler = pRtlpExecuteHandlerForException+5;

	if(!injected) {
		if(!RemoteLoadLibrary((HANDLE)olly_plugin_get_value(VAL_HPROCESS),__DLLNAME__))
			return -3;
		else injected = 1;
	}

	Sleep(200);

	pNewExecute2 = find_dllbase_by_process32((unsigned char *)olly_plugin_get_value(VAL_EXEFILENAME),__DLLNAME__);

	if(!pNewExecute2) // Hummm you the dll isn't loaded
		return -1;

	pNewExecute2 += (unsigned long )GetProcAddress(LoadLibrary(__PATH__), "_NewExecute2@20");
	pNewExecute2 -= (unsigned long )LoadLibrary(__PATH__);

	pNewExecute2 -= pExecuteHandler+5;

	if(olly_read_memory(buffer,(unsigned long)(pExecuteHandler),5,MM_RESILENT) == 0)
		return -2;

	if(olly_write_memory("\xE9",(unsigned long)(pExecuteHandler),1,MM_RESILENT) == 0)
		return 0;

	if(olly_write_memory(&pNewExecute2,(unsigned long)(pExecuteHandler+1),4,MM_RESILENT) == 0)
		return 0;

	// --- Win 2k END ---
}

else if((unsigned short)GetVersion() == 0x0105) {
	// --- Win XP BEGIN ---

	pExecuteHandler = (pRtlpExecuteHandlerForException) + (((unsigned char *)(pRtlpExecuteHandlerForException))[6]) + 7;

	for(i=0; ((unsigned char *)(pExecuteHandler))[i] != 0xE8; i++)
		continue;
	pExecuteHandler = pExecuteHandler+i;

	if(!injected) {
		if(!RemoteLoadLibrary((HANDLE)olly_plugin_get_value(VAL_HPROCESS),__DLLNAME__))
			return -3;
		else injected = 1;
	}

	//Sleep(200);

	pNewExecute2 = find_dllbase_by_process32((unsigned char *)olly_plugin_get_value(VAL_EXEFILENAME),__DLLNAME__);

	if(!pNewExecute2) // Hummm you the dll isn't loaded
		return -1;

	pNewExecute2 += (unsigned long )GetProcAddress(LoadLibrary(__PATH__), "_NewExecute2@20");
	pNewExecute2 -= (unsigned long )LoadLibrary(__PATH__);

	pNewExecute2 -= pExecuteHandler+5;

	if(olly_read_memory(buffer,(unsigned long)(pExecuteHandler),5,MM_RESILENT) == 0)
		return -2;

	if(olly_write_memory("\xE8",(unsigned long)(pExecuteHandler),1,MM_RESILENT) == 0)
		return 0;

	if(olly_write_memory(&pNewExecute2,(unsigned long)(pExecuteHandler+1),4,MM_RESILENT) == 0)
		return 0;
	// --- Win XP END ---
	}

	return 1;

}

unsigned int SetDownHook() {
unsigned int i, j;
unsigned long pKiUserExceptionDispatcher, pRtlDispatchException, pRtlpExecuteHandlerForException, pExecuteHandler;

	pKiUserExceptionDispatcher = (unsigned long )GetProcAddress(GetModuleHandle("ntdll.dll"), "KiUserExceptionDispatcher");

	for(i=0; ((unsigned char *)(pKiUserExceptionDispatcher))[i] != 0xE8; i++)
		continue;

	pRtlDispatchException = pKiUserExceptionDispatcher+i;

	pRtlDispatchException += ( (unsigned long *)(pRtlDispatchException+1) )[0] + 5;

	j=0;
	i=-1;

	while(!j) {
		i++;
		for(; ((unsigned char *)(pRtlDispatchException))[i] != 0xE8; i++)
			continue;

		if(((unsigned char *)(pRtlDispatchException))[i+5] == 0xF6 &&  ((unsigned char *)(pRtlDispatchException))[i+6] == 0x05)
			j++;
	}

	pRtlpExecuteHandlerForException = pRtlDispatchException+i;
	pRtlpExecuteHandlerForException += ( (unsigned long *)(pRtlpExecuteHandlerForException+1) )[0] + 5;

	if((unsigned short)GetVersion() == 0x0005) {
	pExecuteHandler = pRtlpExecuteHandlerForException+5;
	} else if((unsigned short)GetVersion() == 0x0105) {
	pExecuteHandler = (pRtlpExecuteHandlerForException) + (((unsigned char *)(pRtlpExecuteHandlerForException))[6]) + 7;

	for(i=0; ((unsigned char *)(pExecuteHandler))[i] != 0xE8; i++)
		continue;
	pExecuteHandler = pExecuteHandler+i;

	}
	olly_add_to_list(0,1,"%08X",pExecuteHandler);

	if(olly_write_memory(buffer,(unsigned long)(pExecuteHandler),5,MM_RESILENT) == 0)
		return 0;

	return 1;

}

unsigned long find_dllbase_by_process32(unsigned char *process,unsigned char *dll) {
HANDLE  hSnapshot;
unsigned long dwPID;

PROCESSENTRY32 proc;
MODULEENTRY32 me32;

unsigned int i, j;

	j = strlen(process);
	for(i=0;i<j;i++) {
		if((char *)process[i] == (char *)'\\' ) {
			process += i+1;
			j -= i;
			i = 0;
		}
	}

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	proc.dwSize = sizeof(proc);

	if(!Process32First(hSnapshot, &proc))
		return 0;

	while(Process32Next(hSnapshot,&proc)) {

		if(strcmp(process,proc.szExeFile) == 0) {
			dwPID = (unsigned long)proc.th32ProcessID;
			break;
		}

	}

	CloseHandle(hSnapshot);
	if(dwPID == 0)
		return 0;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID );
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	me32.dwSize = sizeof(MODULEENTRY32); 

	if(!Module32First(hSnapshot, &me32)) 
		return 0;

	while(Module32Next(hSnapshot, &me32)) {

		if(strcmp(dll,me32.szModule) == 0) {
			CloseHandle(hSnapshot);
			return (unsigned long)me32.modBaseAddr;
		}
	}

	CloseHandle(hSnapshot);
	return 0;

}

unsigned int RemoteLoadLibrary(HANDLE hProcess, unsigned char *szDll) {
unsigned long pMap,Tmp, dwSize;
unsigned char buffer[64];

	if((strlen(szDll)+15) > 64)
		return 0;

	buffer[0] = 0xE8;					// call ..... ; push the dll string
	buffer[1] = strlen(szDll)+1;
	buffer[2] = buffer[3] = buffer[4] = 0x00;

	memcpy(buffer+5,szDll,strlen(szDll)+1);

	buffer[strlen(szDll)+6] = 0xB8;	// mov eax, XXXXXXXXh
	*(unsigned long *)&buffer[strlen(szDll)+7] = (unsigned long)GetProcAddress(LoadLibrary("kernel32"),"LoadLibraryA");

	buffer[strlen(szDll)+11] = 0xFF; // call eax
	buffer[strlen(szDll)+12] = 0xD0;
	buffer[strlen(szDll)+13] = 0xC3; // retn

	dwSize = strlen(szDll)+14;

	pMap = (unsigned long)VirtualAllocEx(hProcess,0,dwSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	if(!pMap)
		return 0;

	if(!WriteProcessMemory(hProcess,(LPVOID)pMap,&buffer,dwSize,0))
		return 0;

	if(!CreateRemoteThread(hProcess,0,0,(LPVOID)pMap,0,0,&Tmp))
		return 0;

	return 1;
}