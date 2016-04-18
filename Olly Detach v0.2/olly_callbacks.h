/*
    Olly Detach - Call Back Functions
    Copyright (C) 2005 Pedram Amini <pedram.amini@gmail.com>

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

//
// ollydbg call-back functions.
//

BOOL WINAPI DllEntryPoint (HINSTANCE hi, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
        hinst = hi;

    return TRUE;
}


int _export cdecl ODBG_Plugindata (char shortname[32])
{
    strcpy(shortname, "Detach");
    return PLUGIN_VERSION;
}


int _export cdecl ODBG_Plugininit (int ollydbgversion, HWND hw, ulong *features)
{
    if (ollydbgversion < PLUGIN_VERSION)
        return -1;

    // keep a handle to the main OllyDbg window.
    hwmain = hw;

    olly_add_to_list(0, 0,  "Detach From Debuggee Plug-in compiled on " __DATE__);
    olly_add_to_list(0, -1, "  Copyright (c) 2005 Pedram Amini <pedram.amini@gmail.com>");

    return 0;
}


void _export cdecl ODBG_Plugindestroy (void)
{
}


int _export cdecl ODBG_Pluginmenu (int origin, char data[4096], void *item)
{
	t_dump *pd;

    switch (origin)
    {
        case PM_MAIN:
            strcpy(data,
                   "0 Detach From Debuggee|"
                   "9 About"
                  );
            return 1;
		case PM_DISASM: // Popup menu in Disassembler
			// First check that menu applies.
			pd = (t_dump *)item;
			if(!pd || pd->size == 0)
				return 0; // Window empty, don't add

			// Menu item
			lstrcpy(data, "0 &Detach From Debuggee");
			return 1;
        default:
            break;
    }

    return 0;
}


void _export cdecl ODBG_Pluginaction (int origin, int action, void *item)
{
    switch (origin)
    {
        case PM_MAIN:
		case PM_DISASM:
            switch (action)
            {
                // Detach
                case 0:
                    if (olly_get_status() != STAT_RUNNING)
                        Error("Process must be in the running state for detach to work.");
                    else
                        detach_from_debuggee();
                    break;

                // About
                case 9:
                    MessageBox(hwmain,
                        "Detach From Debuggee Plug-in\n\n"
                        "Copyright (c) 2005 Pedram Amini <pedram.amini@gmail.com>\n",
                        "About Detach Plug-in",
                        MB_OK | MB_ICONINFORMATION);
                    break;
            }

        default:
            break;
    }
}
