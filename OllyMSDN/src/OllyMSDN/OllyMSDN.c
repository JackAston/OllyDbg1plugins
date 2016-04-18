/*
 *	OllyDbg/ImmDbg MSDN Online Help Plugin
 *	Copyright (C) 2012 Mario Vilas <mvilas@gmail.com>
 *
 *	This program is free software; you can redistribute it and/or modify it
 *	under the terms of the GNU General Public License as published by the Free
 *	Software Foundation; either version 2 of the License, or (at your option)
 *	any later version.
 *
 *	This program is distributed in the hope that it will be useful, but WITHOUT
 *	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *	more details.
 *
 *	You should have received a copy of the GNU General Public License along with
 *	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 *	Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <Windows.h>
#include <string.h>
#include "Plugin.h"

//////////////////////////////////////////////////////////////////////////////

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason,
						  LPVOID lpReserved);

int iendswith(const char *str, const char *suffix);
BOOL WINAPI Hooked_WinHelpA(HWND hWndMain, LPCSTR lpszHelp, UINT uCommand,
							ULONG_PTR dwData);
BOOL WINAPI Original_WinHelpA(HWND hWndMain, LPCSTR lpszHelp, UINT uCommand,
							  ULONG_PTR dwData);

int install();
void uninstall();

extc int _export cdecl ODBG_Plugininit(int ollydbgversion, HWND hw,
									   ulong *features);
extc int _export cdecl ODBG_Pluginclose();
extc int _export cdecl ODBG_Plugindata(char shortname[32]);
extc int _export cdecl ODBG_Pluginmenu(int origin, char *data, void *item);
extc void _export cdecl ODBG_Pluginaction(int origin, int action, void *item);

extc int _export cdecl IMMDBG_Plugininit(int ollydbgversion, HWND hw,
										 ulong *features);
extc int _export cdecl IMMDBG_Pluginclose();
extc int _export cdecl IMMDBG_Plugindata(char shortname[32]);
extc int _export cdecl IMMDBG_Pluginmenu(int origin, char *data, void *item);
extc void _export cdecl IMMDBG_Pluginaction(int origin, int action,
											void *item);

//////////////////////////////////////////////////////////////////////////////

HINSTANCE g_hInstance    = NULL;
HWND      g_hWndMain     = NULL;
DWORD     g_dwOldProtect = 0;

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL,
						  DWORD fdwReason,
						  LPVOID lpReserved)
{
	switch (fdwReason)
	{

	case DLL_PROCESS_ATTACH:
		g_hInstance = hinstDLL;
		break;

	case DLL_PROCESS_DETACH:
		uninstall();
		break;

	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////

void *g_pWinHelpA = NULL;

const unsigned char g_OrigCode[]  = { 0x8b, 0xff, 0x55, 0x8b, 0xec };
      unsigned char g_PatchCode[] = { 0xe9, 0x00, 0x00, 0x00, 0x00 };

BOOL __declspec(naked) WINAPI Original_WinHelpA(HWND hWndMain,
												LPCSTR lpszHelp,
												UINT uCommand,
												ULONG_PTR dwData)
{
	__asm
	{
		push ebp
		mov ebp, esp
		mov eax, [g_pWinHelpA]
		lea eax, [eax + 5]
		jmp eax
	}
}

int install()
{
	int retval = -1;
	SIZE_T delta;
	SIZE_T *pDelta;

	g_pWinHelpA = (void *) GetProcAddress(GetModuleHandleA("user32"),
										  "WinHelpA");
	if (g_pWinHelpA != NULL &&
		0 == memcmp((void *) g_OrigCode, g_pWinHelpA, sizeof(g_OrigCode)))
	{
		delta = (char *) Hooked_WinHelpA - (char *) g_pWinHelpA
										 - sizeof(g_PatchCode);
		pDelta = (SIZE_T *) (void *) (&g_PatchCode[1]);
		*pDelta = delta;

		if (0 != VirtualProtect(g_pWinHelpA, sizeof(g_PatchCode),
								PAGE_EXECUTE_WRITECOPY, &g_dwOldProtect))
		{
			retval = 0;
		}
		else
		{
			if (0 != VirtualProtect(g_pWinHelpA, sizeof(g_PatchCode),
									PAGE_EXECUTE_READWRITE, &g_dwOldProtect))
			{
				retval = 0;
			}
		}

		if (retval == 0)
		{
			memcpy(g_pWinHelpA, (void *) g_PatchCode, sizeof(g_PatchCode));
		}
	}

	return retval;
}

void uninstall()
{
	if (g_PatchCode[0] != 0)
	{
		memcpy(g_pWinHelpA, (void *) g_OrigCode, sizeof(g_OrigCode));
		VirtualProtect(g_pWinHelpA, sizeof(g_PatchCode),
			g_dwOldProtect, &g_dwOldProtect);
	}
}

//////////////////////////////////////////////////////////////////////////////

// Based on http://stackoverflow.com/a/7718223/426293
int iendswith(const char *str, const char *suffix)
{
	size_t str_len;
	size_t suffix_len;

	if (str == NULL || suffix == NULL)
	{
		return 0;
	}

	str_len = strlen(str);
	suffix_len = strlen(suffix);

	if (suffix_len > str_len)
	{
		return 0;
	}

	return 0 == _strnicmp( str + str_len - suffix_len, suffix, suffix_len );
}

BOOL WINAPI Hooked_WinHelpA(HWND hWndMain,
							LPCSTR lpszHelp,
							UINT uCommand,
							ULONG_PTR dwData)
{
	if (lpszHelp != NULL && ! iendswith(lpszHelp, "win32.hlp"))
	{
		return Original_WinHelpA(hWndMain, lpszHelp, uCommand, dwData);
	}

	switch (uCommand)
	{

	case HELP_CONTENTS:
		{
			ShellExecuteA(hWndMain, "open", "https://msdn.microsoft.com/",
				NULL, NULL, SW_SHOW);
		}
		break;

	case HELP_PARTIALKEY:
		{
			char buffer[512];

			// TODO use MSDN search instead of Google
			strcpy_s(buffer, sizeof(buffer),
				"https://encrypted.google.com/search?"
				"btnI=1&"
				"as_searchsite=msdn.microsoft.com&"
				"q=inurl%3A%22%2Fen-us%2Flibrary%2Fwindows%2Fdesktop%2F%22+");
			strcat_s(buffer, sizeof(buffer), (const char *) (void *) dwData);
			strcat_s(buffer, sizeof(buffer), "+function");

			ShellExecuteA(hWndMain, "open", buffer, NULL, NULL, SW_SHOW);
		}
		break;

	case HELP_QUIT:
		break;

	default:
		return Original_WinHelpA(hWndMain, lpszHelp, uCommand, dwData);

	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,
									   HWND hw,
									   ulong *features)
{
	int retval = -1;

	if (ollydbgversion >= PLUGIN_VERSION)
	{
		g_hWndMain = hw;
		retval = install();
	}

	return retval;
}

extc int _export cdecl ODBG_Pluginclose()
{
	uninstall();
	return 0;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
	strcpy_s(shortname, 32, "MSDN Online Help");
	return PLUGIN_VERSION;
}

extc int _export cdecl ODBG_Pluginmenu(int origin,
									   char *data,
									   void *item)
{
	int retval = 0;

	if (origin == PM_MAIN)
	{
		strcpy_s(data, 32, "0 &About");
		retval = 1;
	}

	return retval;
}

extc void _export cdecl ODBG_Pluginaction(int origin,
										  int action,
										  void *item)
{
	if (origin == PM_MAIN && action == 0)
	{
		MessageBoxW(g_hWndMain,
			L"MSDN Online Help Plugin v1.0\n\n"
			L"Mario Vilas - mvilas@gmail.com\n\n",
			L"MSDN Online Help",
			MB_ICONINFORMATION);		
	}
	return;
}

//////////////////////////////////////////////////////////////////////////////

extc int _export cdecl IMMDBG_Plugininit(int ollydbgversion,
									     HWND hw,
									     ulong *features)
{
	return ODBG_Plugininit(ollydbgversion, hw, features);
}

extc int _export cdecl IMMDBG_Pluginclose()
{
	return ODBG_Pluginclose();
}

extc int _export cdecl IMMDBG_Plugindata(char shortname[32])
{
	return ODBG_Plugindata(shortname);
}

extc int _export cdecl IMMDBG_Pluginmenu(int origin,
									     char *data,
									     void *item)
{
	return ODBG_Pluginmenu(origin, data, item);
}

extc void _export cdecl IMMDBG_Pluginaction(int origin,
										  int action,
										  void *item)
{
	ODBG_Pluginaction(origin, action, item);
}

//////////////////////////////////////////////////////////////////////////////
