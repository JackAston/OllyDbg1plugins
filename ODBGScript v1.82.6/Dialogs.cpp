
INT_PTR CALLBACK InputDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char* buffer;
	
	switch(uMsg) 
	{ 
	case WM_INITDIALOG:
		SetDlgItemText(hwndDlg, IDC_TITLE, (LPCSTR) lParam);
		SetWindowPos(hwndDlg,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		ollylang->hwndinput=hwndDlg;
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
				ollylang->hwndinput=0;
				EndDialog(hwndDlg, (INT_PTR)buffer);
				return TRUE;
			}
		case IDCANCEL: 
			ollylang->hwndinput=0;
			EndDialog(hwndDlg, 0); 
			return TRUE; 
		} 
	} 
	return FALSE; 
}