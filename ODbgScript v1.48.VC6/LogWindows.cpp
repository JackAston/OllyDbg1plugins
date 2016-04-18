
LRESULT CALLBACK wndlog_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
t_logwnd_data *pll;
HMENU menu;
int i,shiftkey,controlkey;

	switch (msg) {
		case WM_DESTROY:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_TIMER:
		case WM_SYSKEYDOWN:
			Tablefunction(&ollylang->wndLog,hw,msg,wp,lp);
			break;                           // Pass message to DefMDIChildProc()
			// Custom messages responsible for scrolling and selection. User-drawn
			// windows must process them, standard OllyDbg windows without extra
			// functionality pass them to Tablefunction()
		case WM_USER_SCR:
		case WM_USER_VABS:
		case WM_USER_VREL:
		case WM_USER_VBYTE:
		case WM_USER_STS:
		case WM_USER_CNTS:
		case WM_USER_CHGS:
		case WM_WINDOWPOSCHANGED:
			return Tablefunction(&ollylang->wndLog,hw,msg,wp,lp);
		case WM_USER_MENU:
			menu=CreatePopupMenu();

//			AppendMenu(menu,MF_SEPARATOR,0,"-"); 

			pll=(t_logwnd_data *)Getsortedbyselection(&(ollylang->wndLog.data),ollylang->wndLog.data.selected);
			if (menu!=NULL && pll!=NULL) {
				AppendMenu(menu,MF_DEFAULT, 10,"Clear");
//				AppendMenu(menu,MF_STRING, 32,"Toggle Script BP\tF2");
			};

			// Even when menu is NULL, call to Tablefunction is still meaningful.
			i=Tablefunction(&ollylang->wndLog,hw,WM_USER_MENU,0,(LPARAM)menu);

			if (menu!=NULL) DestroyMenu(menu);
			switch (i) { 
				case 10:
					clearLogLines();
					InvalidateRect(hw, NULL, FALSE);
					return 1;
				default:;
			}
			return 0;

		case WM_USER_DBLCLK:
			pll=(t_wndlog_data *)Getsortedbyselection(&(ollylang->wndLog.data),ollylang->wndLog.data.selected);
			if (pll!=NULL) {
				if (pll->line) Setcpu(0,pll->line,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
				InvalidateRect(hw, NULL, FALSE);
				return 1;
			}

		case WM_KEYDOWN:
			shiftkey=GetKeyState(VK_SHIFT) & 0x8000;
			controlkey=GetKeyState(VK_CONTROL) & 0x8000;
			if (wp==VK_RETURN && shiftkey==0 && controlkey==0) {
				// Return key follows in Disassembler.
				pll=(t_wndlog_data *)Getsortedbyselection(&(ollylang->wndLog.data),ollylang->wndLog.data.selected);
				if (pll!=NULL) {
					if (pll->line) Setcpu(0,pll->line,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
					InvalidateRect(hw, NULL, FALSE);
				}
			} 
//			else if (wp==VK_F2) { // && shiftkey==0 && controlkey==0) {

			Tablefunction(&ollylang->wndLog,hw,msg,wp,lp);
			break;
        case WM_USER_CHALL:
        case WM_USER_CHMEM:
            InvalidateRect(hw, NULL, FALSE);
            return 0;
        case WM_PAINT:
            Painttable(hw, &ollylang->wndLog, wndlog_get_text);
			return 0;
		default:
		break;
	}
	return DefMDIChildProc(hw,msg,wp,lp);
}

void initLogWindow() {

  	if (ollylang->wndLog.bar.nbar==0) {

	ollylang->wndLog.bar.name[0]="Address";
    ollylang->wndLog.bar.defdx[0]=9;
    ollylang->wndLog.bar.mode[0]=BAR_NOSORT;

	ollylang->wndLog.bar.name[1]="Message"; 
    ollylang->wndLog.bar.defdx[1]=100;
    ollylang->wndLog.bar.mode[1]=BAR_NOSORT;

	ollylang->wndLog.bar.nbar=2;
    ollylang->wndLog.mode=TABLE_COPYMENU|TABLE_APPMENU|TABLE_SAVEPOS|TABLE_ONTOP|TABLE_HILMENU;
    ollylang->wndLog.drawfunc=wndlog_get_text;

	}
	
	Quicktablewindow(&ollylang->wndLog,15,2,wndlogclass,"Script Log Window");
	
	if (ollylang->wndLog.hw) {
		HICON ico=LoadIcon(hinstModule(),MAKEINTRESOURCE(IDI_ICON_LOG)); 
		SendMessage(ollylang->wndLog.hw,WM_SETICON,false,(long)ico);
		CloseHandle(ico);
	}
}

int wndlog_sort_function(const t_sortheader *p1,const t_sortheader *p2,const int sort) {
	t_wndlog_data *lline1 = (t_wndlog_data *)p1;
	t_wndlog_data *lline2 = (t_wndlog_data *)p2;

	if (lline1->line > lline2->line) 
		return 1;
	else if (lline1->line < lline2->line) 
		return -1;
	return 0;
}

int wndlog_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column) {

	unsigned int ret;
	t_wndlog_data *lline = (t_wndlog_data *)ph;

	t_dump *cpuasm;
	int p;

	cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);

    ret = sprintf(s,"");

	switch (column) {
		case 0:
			ret = sprintf(s, "%X", lline->eip);
		break;
		case 1:
			ret = sprintf(s, "%s", lline->message);
		break;

	}

if (!ret) ret=strlen(s);

return ret;
}

void clearLogLines() {

	if (!ollylang->tLogLines.empty()) {
		Deletesorteddatarange(&(ollylang->wndLog.data),0,0xffffffff);
		ollylang->tLogLines.clear();
		if (ollylang->wndLog.hw!=NULL)	InvalidateRect(ollylang->wndLog.hw, NULL, FALSE);
	}
}

int add2log(char* message) {

	t_dump *cpuasm;
	t_wndlog_data lline={0};
	
	cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);

	lline.line = ollylang->tLogLines.size()+1;
	lline.eip = cpuasm->sel0;
	lline.size = 1;
	strncpy(lline.message,message,LOG_MSG_LEN-1);

	ollylang->tLogLines.push_back(lline);

	Addsorteddata(&(ollylang->wndLog.data),&(ollylang->tLogLines.back()));

	if (ollylang->wndLog.hw!=NULL) {
		Selectandscroll(&ollylang->wndLog,lline.line-1,2);
		InvalidateRect(ollylang->wndLog.hw, NULL, FALSE);
	}
	return 1;
}

int add2log(string & message) {

	return add2log((char*)message.c_str());

}
