/*
    Olly De-Attach Helper
    Copyright (C) 2006 Pedram Amini <pedram.amini@gmail.com>

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
#include <Tlhelp32.h>

#include "olly_redefines.h"
#include "plugin.h"
#include "olly_de-attach_helper.h"
#include "olly_callbacks.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// attach to last created process.
void attach_to_last (void)
{
    int next;
    HANDLE h_proc;
    PROCESSENTRY32 last_pe, pe;

    h_proc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    next = Process32First(h_proc, &pe);

    while (next)
    {
        // get the next process entry.
        next = Process32Next(h_proc, &pe);

        // ensure we don't attach to ourself. if the last process was ollydbg, then we want the second to last.
        if (stricmp(pe.szExeFile, "ollydbg.exe") != 0)
            memcpy(&last_pe, &pe, sizeof(PROCESSENTRY32));

        // if we've found the last process, attach to the process.
        if (!next && (GetLastError() == ERROR_NO_MORE_FILES))
        {
            olly_add_to_list(0, 0, "Attaching to last created process %s (%d).", last_pe.szExeFile, last_pe.th32ProcessID);
            olly_attach_to_active_process(last_pe.th32ProcessID);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// detach from the debuggee.

void detach_from_debuggee (void)
{
    HMODULE h_kernel32;

    if ((h_kernel32 = LoadLibrary("kernel32.dll")) == NULL)
        return;

    pDebugSetProcessKillOnExit = (lpfDebugSetProcessKillOnExit) GetProcAddress(h_kernel32, "DebugSetProcessKillOnExit");

    if (pDebugSetProcessKillOnExit)
    {
        pDebugSetProcessKillOnExit(FALSE);
        exit(1);
    }
    else
    {
        olly_add_to_list(0, 1, "DebugSetProcessKillOnExit() unsupported on current platform.");
    }
}