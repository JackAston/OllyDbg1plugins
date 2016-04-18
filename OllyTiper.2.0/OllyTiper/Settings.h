#include "resource1.h"
#include "OllyTiper.h"
HHOOK hhk;
HWND hWnd;
BOOL DataShift, DataAlt, DataCtrl,
	 StackShift, StackAlt, StackCtrl,
	 CodeShift, CodeAlt, CodeCtrl,
	 CopyCtrl, CopyAlt, CopyShift,
	 CIShift, CIAlt, CICtrl;
UINT_PTR DataKey, CodeKey, StackKey, CIKey, CopyKey;

enum wds {DIS, DATA, STACK, COPY, CI};
enum KeyState {CTRL, ALT, SHIFT};
char def[3][5][2]={
	{"1","0","1","1","1"},
	{"0","0","0","0","0"},
	{"0","1","0","0","0"}
};
char* defKey[5]=
{
	"0", "0", "44", "51", "0"
};

void setCtrl(int idx, BOOL b){
	switch (idx){
		case DIS:
			CodeCtrl = b;
			break;
		case DATA:
			DataCtrl = b;
			break;
		case STACK:
			StackCtrl = b;
			break;
		case COPY:
			CopyCtrl = b;
			break;
		case CI:
			CICtrl = b;
			break;
	}
}
void setAlt(int idx, BOOL b){
	switch (idx){
		case DIS:
			CodeAlt = b;
			break;
		case DATA:
			DataAlt = b;
			break;
		case STACK:
			StackAlt = b;
			break;
		case COPY:
			CopyAlt = b;
			break;
		case CI:
			CIAlt = b;
			break;
	}
}
void setShift(int idx, BOOL b){
	switch (idx){
		case DIS:
			CodeShift = b;
			break;
		case DATA:
			DataShift = b;
			break;
		case STACK:
			StackShift = b;
			break;
		case COPY:
			CopyShift = b;
			break;
		case CI:
			CIShift = b;
			break;
	}
}
void setKey(int idx, int b){
	switch (idx){
		case DIS:
			CodeKey = b;
			break;
		case DATA:
			DataKey = b;
			break;
		case STACK:
			StackKey = b;
			break;
		case COPY:
			CopyKey = b;
			break;
		case CI:
			CIKey = b;
			break;
	}
}
UINT getCtrl(int idx){
	char s[20], ans[2];
	strcpy(s, "Ctrl");
	char bf[10];
	strcat(s, _itoa(idx, bf, 10)); 
	int l = Pluginreadstringfromini(hInst,  s,  ans,  "");
	if (l == 0){
		Pluginwritestringtoini(hInst,  s,  def[CTRL][idx]);
		ans[0] = def[CTRL][idx][0];
	}
	setCtrl(idx, ans[0]=='1');
	return ans[0]=='1'?BST_CHECKED:BST_UNCHECKED;
}
UINT getAlt(int idx){
	char s[20], ans[2];
	strcpy(s, "Alt");
	char bf[10];strcat(s, _itoa(idx, bf, 10)); 
	int l = Pluginreadstringfromini(hInst,  s,  ans,  "");
	if (l == 0){
		Pluginwritestringtoini(hInst,  s,  def[ALT][idx]);
		ans[0] = def[ALT][idx][0];
	}
	setAlt(idx, ans[0]=='1');
	return ans[0]=='1'?BST_CHECKED:BST_UNCHECKED;
}
UINT getShift(int idx){
	char s[20], ans[2];
	strcpy(s, "Shift");
	char bf[10];strcat(s, _itoa(idx, bf, 10)); 
	int l = Pluginreadstringfromini(hInst,  s,  ans,  "");
	if (l == 0){
		Pluginwritestringtoini(hInst,  s,  def[SHIFT][idx]);
		ans[0] = def[SHIFT][idx][0];
	}
	setShift(idx, ans[0]=='1');
	return ans[0]=='1'?BST_CHECKED:BST_UNCHECKED;
}

UINT getKey(int idx){
	char s[20], ans[5];
	strcpy(s, "Key");
	char bf[10];strcat(s, _itoa(idx, bf, 10)); 
	int l = Pluginreadstringfromini(hInst,  s,  ans,  "");
	if (l == 0){
		Pluginwritestringtoini(hInst,  s,  defKey[idx]);
		strcpy(ans, defKey[idx]);
	}
	int key;
	sscanf(ans, "%x", &key);
	setKey(idx, key);
	return key;
}
void writeCtrl(int idx, BOOL b){
	char s[20], ans[2];
	strcpy(s, "Ctrl");
	char bf[10];strcat(s, _itoa(idx, bf, 10));
	ans[0]='0'+b;
	ans[1]=0;
	Pluginwritestringtoini(hInst,  s,  ans);
	setCtrl(idx, ans[0]=='1');
}
void writeAlt(int idx, BOOL b){
	char s[20], ans[2];
	strcpy(s, "Alt");
	char bf[10];strcat(s, _itoa(idx, bf, 10));
	ans[0]='0'+b;
	ans[1]=0;
	Pluginwritestringtoini(hInst,  s,  ans);
	setAlt(idx, ans[0]=='1');
}
void writeShift(int idx, BOOL b){
	char s[20], ans[2];
	strcpy(s, "Shift");
	char bf[10];strcat(s, _itoa(idx, bf, 10));
	ans[0]='0'+b;
	ans[1]=0;
	Pluginwritestringtoini(hInst,  s,  ans);
	setShift(idx, ans[0]=='1');
}
void writeKey(int idx, char *sKey){
	char s[20];
	strcpy(s, "Key");
	char bf[10];strcat(s, _itoa(idx, bf, 10)); 
	Pluginwritestringtoini(hInst,  s,  sKey);
	int key;
	sscanf(sKey, "%x", &key);
	setKey(idx, key);
}
LRESULT CALLBACK Hook(int code, WPARAM wPar, LPARAM lParam){
	HWND hFocus;
	HWND hwnd = hWnd;
	WPARAM wParam;
	hFocus = GetFocus();
	char text[10];
	wParam = wPar;
	if (wParam == VK_DELETE || wParam == VK_BACK)
		wParam = 0;
	if (hFocus==GetDlgItem(hwnd, ID_B_DIS))
		SetDlgItemText(hwnd, ID_B_DIS, _itoa(wParam, text, 16));
	else if (hFocus==GetDlgItem(hwnd, ID_B_DATA))
		SetDlgItemText(hwnd, ID_B_DATA, _itoa(wParam, text, 16));
	else if (hFocus==GetDlgItem(hwnd, ID_B_STACK))
		SetDlgItemText(hwnd, ID_B_STACK, _itoa(wParam, text, 16));
	else if (hFocus==GetDlgItem(hwnd, ID_B_COPY))
		SetDlgItemText(hwnd, ID_B_COPY, _itoa(wParam, text, 16));
	else if (hFocus==GetDlgItem(hwnd, ID_B_CI))
		SetDlgItemText(hwnd, ID_B_CI, _itoa(wParam, text, 16));
	//if (code < 0)
		return CallNextHookEx(hhk, code, wPar, lParam);
	
}
BOOL CALLBACK SettingsDlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//__asm __emit 0xCC;
	switch(Message){
		case WM_INITDIALOG:
			hWnd = hwnd;
			CheckDlgButton(hwnd, ID_CTRL_DIS, getCtrl(DIS));
			CheckDlgButton(hwnd, ID_CTRL_DATA, getCtrl(DATA));
			CheckDlgButton(hwnd, ID_CTRL_STACK, getCtrl(STACK));
			CheckDlgButton(hwnd, ID_CTRL_COPY, getCtrl(COPY));
			CheckDlgButton(hwnd, ID_CTRL_CI, getCtrl(CI));
			
			CheckDlgButton(hwnd, ID_ALT_DIS, getAlt(DIS));
			CheckDlgButton(hwnd, ID_ALT_DATA, getAlt(DATA));
			CheckDlgButton(hwnd, ID_ALT_STACK, getAlt(STACK));
			CheckDlgButton(hwnd, ID_ALT_COPY, getAlt(COPY));
			CheckDlgButton(hwnd, ID_ALT_CI, getAlt(CI));

			CheckDlgButton(hwnd, ID_SHIFT_DIS, getShift(DIS));
			CheckDlgButton(hwnd, ID_SHIFT_DATA, getShift(DATA));
			CheckDlgButton(hwnd, ID_SHIFT_STACK, getShift(STACK));
			CheckDlgButton(hwnd, ID_SHIFT_COPY, getShift(COPY));
			CheckDlgButton(hwnd, ID_SHIFT_CI, getShift(CI));

			char bf[10];
			SetDlgItemText(hwnd, ID_B_DIS, _itoa(getKey(DIS), bf, 16));
			SetDlgItemText(hwnd, ID_B_DATA, _itoa(getKey(DATA), bf, 16));
			SetDlgItemText(hwnd, ID_B_STACK, _itoa(getKey(STACK), bf, 16));
			SetDlgItemText(hwnd, ID_B_COPY, _itoa(getKey(COPY), bf, 16));
			SetDlgItemText(hwnd, ID_B_CI, _itoa(getKey(CI), bf, 16));

			
			hhk=SetWindowsHookEx(WH_KEYBOARD, Hook, hInst,GetCurrentThreadId());
			
			return TRUE;
			break;
		case WM_CLOSE:
			UnhookWindowsHookEx(hhk);
			EndDialog(hwnd, 0);
			return 0;
			//return (INT_PTR)FALSE;
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_OK:
					UnhookWindowsHookEx(hhk);

					writeCtrl(DIS, IsDlgButtonChecked(hwnd, ID_CTRL_DIS));
					writeCtrl(DATA, IsDlgButtonChecked(hwnd, ID_CTRL_DATA));
					writeCtrl(STACK, IsDlgButtonChecked(hwnd, ID_CTRL_STACK));
					writeCtrl(COPY, IsDlgButtonChecked(hwnd, ID_CTRL_COPY));
					writeCtrl(CI, IsDlgButtonChecked(hwnd, ID_CTRL_CI));

					writeAlt(DIS, IsDlgButtonChecked(hwnd, ID_ALT_DIS));
					writeAlt(DATA, IsDlgButtonChecked(hwnd, ID_ALT_DATA));
					writeAlt(STACK, IsDlgButtonChecked(hwnd, ID_ALT_STACK));
					writeAlt(COPY, IsDlgButtonChecked(hwnd, ID_ALT_COPY));
					writeAlt(CI, IsDlgButtonChecked(hwnd, ID_ALT_CI));

					writeShift(DIS, IsDlgButtonChecked(hwnd, ID_SHIFT_DIS));
					writeShift(DATA, IsDlgButtonChecked(hwnd, ID_SHIFT_DATA));
					writeShift(STACK, IsDlgButtonChecked(hwnd, ID_SHIFT_STACK));
					writeShift(COPY, IsDlgButtonChecked(hwnd, ID_SHIFT_COPY));
					writeShift(CI, IsDlgButtonChecked(hwnd, ID_SHIFT_CI));

					char str[10];
					GetDlgItemText(hwnd, ID_B_DIS, str, sizeof str);
					writeKey(DIS, str); 
					GetDlgItemText(hwnd, ID_B_DATA, str, sizeof str);
					writeKey(DATA, str); 
					GetDlgItemText(hwnd, ID_B_STACK, str, sizeof str);
					writeKey(STACK, str); 
					GetDlgItemText(hwnd, ID_B_COPY, str, sizeof str);
					writeKey(COPY, str); 
					GetDlgItemText(hwnd, ID_B_CI, str, sizeof str);
					writeKey(CI, str); 
					
					
					EndDialog(hwnd, 0);
					return 0;
					//return (INT_PTR)FALSE;
					break;
				case IDC_CANCEL:
					UnhookWindowsHookEx(hhk);
					EndDialog(hwnd, 1);
					return 0;
					//return (INT_PTR)FALSE;
					break;
			}
			return FALSE;
			break;
		default:
	//		return DefWindowProc(hwnd, Message, wParam, lParam);
			break;
	//		return FALSE;
	}
	return FALSE;
}