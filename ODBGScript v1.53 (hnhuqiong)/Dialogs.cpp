#include "StdAfx.h"

INT_PTR CALLBACK InputDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char* buffer;

	switch(uMsg) 
	{ 
	case WM_INITDIALOG:
		SetDlgItemText(hwndDlg, IDC_TITLE, (LPCSTR) lParam);
		return TRUE;
	case WM_SHOWWINDOW:
		SetFocus(GetDlgItem(hwndDlg, IDC_INPUT));
		return TRUE;
	case WM_COMMAND: 
		switch (LOWORD(wParam)) 
		{ 
		case IDOK:
			buffer=new char[256];
			if(GetDlgItemText(hwndDlg, IDC_INPUT, buffer, 256))
			{
				EndDialog(hwndDlg, (INT_PTR)buffer);
				return TRUE;
			}
		case IDCANCEL: 
			EndDialog(hwndDlg, 0); 
			return TRUE; 
		} 
	} 
	return FALSE; 
}