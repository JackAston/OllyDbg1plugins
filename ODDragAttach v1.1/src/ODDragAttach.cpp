// ODDragAttach.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include <stdio.h>
#include "Plugin.h"
#pragma comment(lib, "OLLYDBG.LIB")
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")


extern HWND WINAPI GetWindowFromPointEx(POINT pt);

LRESULT MyMsgProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam);


HINSTANCE hInstance;
WNDPROC OldMsgProc;
HWND MyStatic, g_hWnd;
HWND hWndTarget = NULL; //目标窗口句柄
DWORD dwPid = 0; //目标进程PID
char processName[MAX_PATH] = {0}; //目标进程名
char windowText[256]; //目标窗口名
char msg[1024];
BOOL bCapSet = FALSE;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hInstance = (HINSTANCE)hModule;
	}
    return TRUE;
}


extc int _export cdecl ODBG_Plugindata(char shortname[32])

{
	
    strcpy(shortname, "ODDragAttach");	

    return PLUGIN_VERSION;

}

extc int _export ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features)
{

	g_hWnd = hw;

	MyStatic = CreateWindowEx(0,"static","  拖拽",WS_CHILD|WS_VISIBLE|BS_FLAT|WS_BORDER,35,1,60,20,hw,NULL,hInstance,NULL);  

	OldMsgProc = (WNDPROC)SetWindowLong(hw, GWL_WNDPROC, (LONG)MyMsgProc);

	return 0;	
}


extc int _export cdecl ODBG_Pluginmenu(int origin, char data[4096], void *item)
{
	
	if (origin == PM_MAIN)	
	{		
		strcpy(data, "0 关于");
		return 1;	
	}	
	return 0;
	
}
extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) 
{
	
	switch(action) 
	{
	case 0:
		MessageBox(0, "OD Drag Attach Plugin v1.1 by MentalDease", "(hackerzhuang@qq.com)", 0);
		break;
	default: 
		break;
	};
};



BOOL IsPointInRect(int ptx, int pty, int rectx, int recty, int rectw, int recth)
{

	if (ptx > rectx && ptx < rectx+rectw && pty > recty && pty < recty + rectw)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID GetProcessNameFromPid(DWORD pid)
{

	HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, NULL, pid);

	if (hProcess)
	{
		GetModuleFileNameEx(hProcess, NULL, processName, MAX_PATH);
	}
	else
	{
		MessageBox(0, "Openprocess Error", "Error",0);
	}
}

extern HWND WINAPI GetWindowFromPointEx(POINT pt);

LRESULT MyMsgProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{

	static HCURSOR cursor_me = ::LoadCursor(
		hInstance,
		MYCURSOR);
	static HCURSOR cursor_sys;

	switch(message)
	{	

	case WM_LBUTTONDOWN:

		WORD xPos, yPos;
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);

		if (IsPointInRect (xPos, yPos, 35, 1, 60, 20))
		{
			cursor_sys = SetCursor (cursor_me);
			bCapSet = TRUE;
			SetCapture (g_hWnd);
			ShowWindow (g_hWnd, SW_SHOWMINIMIZED);
		}
		
		break;
		
	case WM_LBUTTONUP:

		if (bCapSet)
		{

			::SetCursor(cursor_sys);
			ReleaseCapture();

			POINT pt;
			GetCursorPos (&pt);
			hWndTarget = GetWindowFromPointEx(pt);
			GetWindowThreadProcessId (hWndTarget, &dwPid);

			if (dwPid)
			{
				GetWindowText(hWndTarget, windowText, 256);
				GetProcessNameFromPid (dwPid);
				sprintf (msg, "目标窗口：%s\nPid：  %d\n进程路径：%s", windowText, dwPid, processName);
				ZeroMemory(processName, MAX_PATH);
				
				ShowWindow (g_hWnd, SW_SHOWMAXIMIZED);
				if(MessageBox(0, msg, "DragAttach提示 是否附加该进程？", MB_YESNO) == IDYES)
				{
					Attachtoactiveprocess(dwPid);
				}

			}

			dwPid = 0;
			hWndTarget = NULL;
		}

		bCapSet = FALSE;

		break;
		
	default:
		break;
	}
	
	return CallWindowProc(OldMsgProc,hwnd,message,wParam,lParam);
}



