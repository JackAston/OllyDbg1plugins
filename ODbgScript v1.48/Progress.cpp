
LRESULT CALLBACK wndprog_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp) {
int i,m,shiftkey,controlkey;
HMENU menu,mLoad,mRun,mVars=NULL;
t_wndprog_data *ppl;

	switch (msg) {
		case WM_LBUTTONDBLCLK:
			i=ollylang->wndProg.data.selected;
			//ppl=(t_wndprog_data *)Getsortedbyselection(&ollylang->wndProg.data,ollylang->wndProg.data.selected);
			if (i > 0) {
				ollylang->jumpToLine(i);
				InvalidateRect(hw, NULL, FALSE);
			}
		break;
		case WM_DESTROY:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_TIMER:
		case WM_SYSKEYDOWN:
			Tablefunction(&ollylang->wndProg,hw,msg,wp,lp);
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
			return Tablefunction(&ollylang->wndProg,hw,msg,wp,lp);
		case WM_USER_MENU:
			menu=CreatePopupMenu();

			mRun=CreatePopupMenu();
			AppendMenu(menu,MF_POPUP,(DWORD) mRun,"Run Script"); 
			AppendMenu(mRun,MF_MENUBREAK,20,"Open...");
			AppendMenu(mRun,MF_SEPARATOR,0,"-");
			mruGetCurrentMenu(mRun,20);

			mLoad=CreatePopupMenu();
			AppendMenu(menu,MF_POPUP,(DWORD) mLoad,"Load Script"); 
			AppendMenu(mLoad,MF_MENUBREAK,10,"Open...");
			AppendMenu(mLoad,MF_SEPARATOR,0,"-");
			mruGetCurrentMenu(mLoad,10);

			ppl=(t_wndprog_data *)Getsortedbyselection(&ollylang->wndProg.data,ollylang->wndProg.data.selected);
			if (menu!=NULL && ppl!=NULL) {
				AppendMenu(menu,MF_STRING, 30,"Edit Script...");
				AppendMenu(menu,MF_SEPARATOR,0,"-"); 
				AppendMenu(menu,MF_DEFAULT,31,"&Follow\tEnter");
				AppendMenu(menu,MF_STRING, 32,"Toggle Script BP\tF2");
				AppendMenu(menu,MF_STRING, 33,"Step\tS");
				
				if (script_state==SS_PAUSED)
					AppendMenu(menu,MF_STRING, 34,"Resume\tSPC");
				else
					AppendMenu(menu,MF_STRING, 34,"Pause\tSPC");
				
				AppendMenu(menu,MF_STRING, 35,"Abort\tESC");

			};
			
			if (ollylang->variables.size() > 0) {
				mVars=CreatePopupMenu();
				AppendMenu(menu,MF_SEPARATOR,0,"-");
				AppendMenu(menu,MF_POPUP,(DWORD) mVars,"Edit Variables"); 
				ollylang->menuListVariables(mVars,40);
			}
			if (Getstatus() == STAT_STOPPED) {
				AppendMenu(menu,MF_SEPARATOR,0,"-");
				AppendMenu(menu,MF_STRING, 39,"Execute Command...\tX");
			}

			// Even when menu is NULL, call to Tablefunction is still meaningful.
			i=Tablefunction(&ollylang->wndProg,hw,WM_USER_MENU,0,(LPARAM)menu);

			if (menu!=NULL) DestroyMenu(menu);
			if (mLoad!=NULL) DestroyMenu(mLoad);
			if (mRun!=NULL) DestroyMenu(mRun);
			if (mVars!=NULL) DestroyMenu(mVars);
			
			if (i>10 && i<=15 || i>20 && i<=25) {

				char key[5]="NRU ";
				key[3]=(i%10)+0x30;
							
				ZeroMemory(&buff, sizeof(buff));
				Pluginreadstringfromini(hinstModule(),key,buff,0);

				// Load script
				ollylang->LoadScript(buff);

				mruAddFile(buff);

				// Save script directory
				char* buf2;
				GetFullPathName(buff,sizeof(buff),buff,&buf2); *buf2=0;			
				Pluginwritestringtoini(hinstModule(), "ScriptDir", buff);

				// Pause script (From Load Script MRU)
				if (i<20) {
					ollylang->Pause();
				}
				return 1;
			} 
			else if (i>=40) {
				if (ollylang->editVariable(i-40))
					InvalidateRect(hw, NULL, FALSE);
				return 1;
			} 
			else

			switch (i) { 
				case 20: // Open Run
					ODBG_Pluginaction(PM_MAIN,0,NULL);
					return 1;
				case 10: // Open Load
					ODBG_Pluginaction(PM_MAIN,0,NULL); 
					ollylang->Pause();
					return 1;
				case 30: // Edit Script
					ShellExecute(hwndOllyDbg(),"open",ollylang->scriptpath.c_str(),NULL,ollylang->currentdir.c_str(),SW_SHOWDEFAULT);
					return 1;
				case 31: // Follow in Disassembler 
					Setcpu(0,ppl->eip,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
					InvalidateRect(hw, NULL, FALSE);
					return 1;
				case 32: // Toggle Script BP
					if (ppl->pause) ppl->pause=0; else ppl->pause=1;
					InvalidateRect(hw, NULL, FALSE);
					return 1;
				case 33: // Step
					ollylang->Pause();
					ollylang->Step(1);
					script_state = ollylang->GetState();
					focusonstop=5;
					return 1;
				case 34: // Pause/Resume
					if (script_state==SS_PAUSED) {
						ollylang->Resume();
					} else {
						ollylang->Pause();
						script_state = ollylang->GetState();
					}
					return 1;
				case 35: // Abort
					ollylang->Reset();
					ollylang->Pause();
					return 1;
				case 39: // Execute Command
					if (Getstatus() == STAT_STOPPED) {
						ollylang->execCommand();
						InvalidateRect(hw, NULL, FALSE);
					}
					return 1;
				default:;
			}
			return 0;

		case WM_USER_DBLCLK:
			ppl=(t_wndprog_data *)Getsortedbyselection(&(ollylang->wndProg.data),ollylang->wndProg.data.selected);
			if (ppl!=NULL) {
				if (ppl->eip) Setcpu(0,ppl->eip,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
				InvalidateRect(hw, NULL, FALSE);
				return 1;
			}

		case WM_KEYDOWN:
			shiftkey=GetKeyState(VK_SHIFT) & 0x8000;
			controlkey=GetKeyState(VK_CONTROL) & 0x8000;
			if (wp==VK_RETURN && shiftkey==0 && controlkey==0) {
				// Return key follows in Disassembler.
				ppl=(t_wndprog_data *)Getsortedbyselection(&(ollylang->wndProg.data),ollylang->wndProg.data.selected);
				if (ppl!=NULL) {
					if (ppl->eip) Setcpu(0,ppl->eip,0,0,CPU_ASMHIST|CPU_ASMCENTER|CPU_ASMFOCUS);
					InvalidateRect(hw, NULL, FALSE);
				}
			} 
			else if (wp==VK_F2) { // && shiftkey==0 && controlkey==0) {
				// Toggle Script BP
				ppl=(t_wndprog_data *)Getsortedbyselection(&(ollylang->wndProg.data),ollylang->wndProg.data.selected);
				if (ppl!=NULL) {
					if (ppl->pause) ppl->pause=0; else ppl->pause=1;
					InvalidateRect(hw, NULL, FALSE);
				}
			}
			else if (wp=='S') {
			
				// Step
				ollylang->Pause();
				ollylang->Step(1);
				script_state = ollylang->GetState();
				focusonstop=4;
				return 1;
			
			}
			else if (wp=='X') {
			
				// Command
				if (Getstatus() == STAT_STOPPED) {
					ollylang->execCommand();
					InvalidateRect(hw, NULL, FALSE);
				}
				return 1;
			
			}
			else if (wp==' ') { // Pause/Resume

				if (script_state==SS_PAUSED) {
					ollylang->Resume();					
				} else {
					ollylang->Pause();
					script_state = ollylang->GetState();
				}
				return 1;
			} 
			else if (wp==VK_ESCAPE) {
				// Resume
				ollylang->Reset();
				ollylang->Pause();
				return 1;
			} 
			Tablefunction(&ollylang->wndProg,hw,msg,wp,lp);
			break;
        case WM_USER_CHALL:
        case WM_USER_CHMEM:
            InvalidateRect(hw, NULL, FALSE);
            return 0;
        case WM_PAINT:
            Painttable(hw, &ollylang->wndProg, wndprog_get_text);
			return 0;
		default:
		break;
	}
	return DefMDIChildProc(hw,msg,wp,lp);
}

void initProgTable() {

  	if (ollylang->wndProg.bar.nbar==0) {

	ollylang->wndProg.bar.name[0]="Line";
    ollylang->wndProg.bar.defdx[0]=5;
    ollylang->wndProg.bar.mode[0]=BAR_NOSORT;

	ollylang->wndProg.bar.name[1]="Command"; 
    ollylang->wndProg.bar.defdx[1]=40;
    ollylang->wndProg.bar.mode[1]=BAR_NOSORT;

	ollylang->wndProg.bar.name[2]="Result";
    ollylang->wndProg.bar.defdx[2]=15;
    ollylang->wndProg.bar.mode[2]=BAR_NOSORT;

    ollylang->wndProg.bar.name[3]="EIP";
    ollylang->wndProg.bar.defdx[3]=9;
    ollylang->wndProg.bar.mode[3]=BAR_NOSORT;

    ollylang->wndProg.bar.name[4]="Values <---";
    ollylang->wndProg.bar.defdx[4]=100;
    ollylang->wndProg.bar.mode[4]=BAR_NOSORT;

    ollylang->wndProg.bar.nbar=5;
    ollylang->wndProg.mode=TABLE_COPYMENU|TABLE_APPMENU|TABLE_SAVEPOS|TABLE_ONTOP|TABLE_HILMENU|TABLE_WIDECOL;
    ollylang->wndProg.drawfunc=wndprog_get_text;
	}
	
	Quicktablewindow(&ollylang->wndProg,15,5,wndprogclass,"Script Execution");

	if (ollylang->wndProg.hw) {
		HICON ico=LoadIcon(hinstModule(),MAKEINTRESOURCE(IDI_ICON_SCRIPT)); 
		SendMessage(ollylang->wndProg.hw,WM_SETICON,false,(long)ico); 
		CloseHandle(ico);
	}

}

int wndprog_sort_function(const t_sortheader *p1,const t_sortheader *p2,const int sort) {
	t_wndprog_data *pline1 = (t_wndprog_data *)p1;
	t_wndprog_data *pline2 = (t_wndprog_data *)p2;

	if (pline1->line > pline2->line) 
		return 1;
	else if (pline1->line < pline2->line) 
		return -1;
	return 0;
}

int wndprog_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column) {

	unsigned int ret;
	t_wndprog_data *pline = (t_wndprog_data *)ph;
	t_wndprog_data *plineBf = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),pline->line-1);
	t_wndprog_data *plineAf = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),pline->line+1);

	t_dump *cpuasm;
	int p;

	cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);

    ret = sprintf(s,"");

	switch (column) {
		case 0:
			if (pline->line>0)
			  ret = sprintf(s, "%d", pline->line);
			else if (pline->line==-1)
			  ret = sprintf(s, "LOAD");
			
			if (pline->pause) {
				//Hilite Breakpoint
				*select=DRAW_MASK;
				memset(mask,DRAW_BREAK,ret);

			} else if (pline->line == ollylang->pgr_scriptpos) {
				//Hilite Current Line	
				*select=DRAW_MASK;
				memset(mask,DRAW_EIP,ret);
				
			} else if (pline->command[strlen(pline->command)-1]==':') {
				*select=DRAW_MASK;
				memset(mask,DRAW_GRAY,ret);
			}
		break;
		case 1:
			if (pline->command[strlen(pline->command)-1] == ':' || pline->line == 0) {
				ret = sprintf(s, "%s", &pline->command[1]);
				memset(&s[ret],'_',PROG_CMD_LEN-ret);
				ret=PROG_CMD_LEN;
				*select=DRAW_MASK;
				memset(mask,DRAW_GRAY,ret);				
			} else {
				ret = sprintf(s, "%s", pline->command);
				memset(mask,DRAW_NORMAL,ret);
			}
			if (pline->line == ollylang->pgr_scriptpos && ret>0) {
				//Hilite Current Line	
				*select=DRAW_MASK;
				memset(mask,DRAW_EIP,ret);			
			}
			else if (pline->type==2 && ret>0) {
				//Hilite executed lines
				*select=DRAW_MASK;
				memset(mask,DRAW_HILITE,ret);
			}

			//DRAW JUMP ARROWS
			if (pline->jumpto > 0) {
				*select=DRAW_MASK;
				
				if (pline->jumpto < pline->line) {
					*s='U'; 
				} else {
					*s='D';
				}
				*mask=DRAW_GRAPH;

			} 
		break;
		case 2:
			if (*pline->result != 0) {
				if (plineBf) {
					if (strcmp(plineBf->result,pline->result)==0) {
						//Same result as previous line
						*select=DRAW_MASK;
						*s=' ';	s[1]=D_GRAYPATH;
						ret=2;
						memset(mask,DRAW_GRAPH,ret);
						break;
					}
				}
				ret = sprintf(s, "%s", pline->result);
			} else if (pline->command[strlen(pline->command)-1] == ':') {
				ret=PROG_RES_LEN;
				memset(s,'_',ret);
				*select=DRAW_MASK;
				memset(mask,DRAW_GRAY,ret);				
				
				//ret=30;strcpy(s,"N .BIJKESTU<Duvdeijrsfgahztbcl");			
				//ret=30;strcpy(s,"Duvdeijrsfgahztbcl");			
				//memset(mask,DRAW_GRAPH,ret);
			}
		break;
		case 3:
			if (pline->eip>0) {
				if (plineBf) {
					if (plineBf->eip == pline->eip) {
						
						if (plineAf)
							if (plineAf->eip != pline->eip) goto draw_normal_eip;
						
						*select=DRAW_MASK;
						*s=' ';
						if (cpuasm->sel0 == pline->eip) 
							s[1]=D_PATH;
						else 
							s[1]=D_GRAYPATH;
					
						ret=2;
						memset(mask,DRAW_GRAPH,ret);
						break;
					}
				}

				draw_normal_eip:
				ret = sprintf(s, "%08X", pline->eip);
				
				if (cpuasm->sel0 == pline->eip) {
					//Hilite current eip
					*select=DRAW_MASK;
					memset(mask,DRAW_HILITE,ret);				
				}
			}
			else if (pline->command[strlen(pline->command)-1] == ':') {
				memset(s,'_',9);
				ret=9;
				*select=DRAW_MASK;
				memset(mask,DRAW_GRAY,ret);				
			}
		break;
		case 4:
			if (*pline->values != 0) {
				if (pline->values[0] != ',') 
					ret = sprintf(s, "%s", pline->values);
				else {
					ret = sprintf(s, "%s", &pline->values[1]);
					p = strchr(&pline->values[1],',')-pline->values;
					if (p>0) {
						*select=DRAW_MASK;
						memset(mask,DRAW_NORMAL,ret);
						memset(mask,DRAW_HILITE,p-1);
					}
				}
			} 
			else if (pline->command[strlen(pline->command)-1] == ':') {
				memset(s,'_',PROG_VAL_LEN);
				ret=PROG_VAL_LEN;
				*select=DRAW_MASK;
				memset(mask,DRAW_GRAY,ret);
			}
			break;

	}

if (!ret) ret=strlen(s);

return ret;
}

int addProgLine(int line, string & command) {

	t_wndprog_data pline={0};

	pline.line = line;
	pline.size = 1;
	strcpy(pline.command," ");
	strncat(pline.command,command.c_str(),PROG_CMD_LEN-2);
	strcpy(pline.result,"");	
	strcpy(pline.values,"");
	pline.type = 1;
	pline.eip = 0;
		
	ollylang->tProgLines.push_back(pline);

	Addsorteddata(&(ollylang->wndProg.data),&(ollylang->tProgLines.back()));
	
	if (ollylang->wndProg.hw!=NULL)	InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);

	return 1;
}

//Change type to 2 to hilite executed commands
int setProgLineEIP(int line, int eip) {

	if (line>ollylang->script.size()) return 0;

	t_wndprog_data *ppl;

	ppl = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),line);

	ppl->type = 2;

	if (ppl->command[strlen(ppl->command)-1] == ':')
		return 1;

    ppl->eip = eip;
	
	if (ollylang->wndProg.hw!=NULL)	{
		Selectandscroll(&ollylang->wndProg,ollylang->pgr_scriptpos,2);
		InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);
	}

	return 1;
}

int setProgLineValue(int line, string& value) {

	if (line>ollylang->script.size()) return 0;

	t_wndprog_data *ppl;
	ppl = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),line);

	if (ppl->command[strlen(ppl->command)-1] == ':')
		return 1;

	string values;

	values = value;

	if (strcmp(ppl->values,"")) {
		if ( values.length() < PROG_VAL_LEN-2) {
			if (ppl->values[0] != ',' && ppl->values[0] != '®' && value.compare("®") != 0)
				values += " ";
			values.append(ppl->values);
		}
	}

	strncpy(ppl->values,values.c_str(),PROG_VAL_LEN-1);

	ppl->values[PROG_VAL_LEN-1] = 0;
 
	if (ollylang->wndProg.hw!=NULL)	InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);

	return 1;
}

int setProgLineValue(int line, DWORD value) {
	char buf[17];
	sprintf(buf, "%X", value);
	string str(buf);
	return setProgLineValue(line, str);
}

int setProgLineValueFloat(int line, long double value) {
	char buf[128];
	sprintf(buf, "%lf", value);
	string str(buf);
	while (true) {
		if (str.length() <= 3) break; 
		if (str[str.length()-1] == '0')
			str.erase(str.length()-1,1);
		else 
			break;
	}
	return setProgLineValue(line, str);
}

int setProgLineResult(int line, string& result) {

	if (line>ollylang->script.size()) return 0;
	
	char values[PROG_VAL_LEN];
	t_wndprog_data *ppl;
	
	ppl = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),line);
	
	if (ppl->command[strlen(ppl->command)-1] == ':')
		return 1;

	strncpy(ppl->result,result.c_str(),PROG_RES_LEN);
	if (strcmp(ppl->values,"")) {
		if (ppl->values[0] != ',') {
			strcpy(values,ppl->values);
			strncpy(&ppl->values[1],values,PROG_VAL_LEN-2);
			ppl->values[0]=',';
		}
	}

	if (ollylang->wndProg.hw!=NULL)	{
		//Selectandscroll(&ollylang->wndProg,line,1);
		InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);
	}
	return 1;
}

int setProgLineResult(int line, DWORD result) {
	char buf[17];
	sprintf(buf, "%X", result);
	string str(buf);
	return setProgLineResult(line, str);
}

int setProgLineResultFloat(int line, long double result) {
	char buf[128];
	sprintf(buf, "%lf", result);
	string str(buf);
	while (true) {
		if (str.length() <= 3) break; 
		if (str[str.length()-1] == '0')
			str.erase(str.length()-1,1);
		else 
			break;
	}
	return setProgLineResult(line, str);
}

void clearProgLines() {
	ollylang->pgr_scriptpos=0;
	if (!ollylang->tProgLines.empty()) {
		Deletesorteddatarange(&(ollylang->wndProg.data),0,0xffffffff);
		ollylang->tProgLines.clear();
		if (ollylang->wndProg.hw!=NULL)	InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);
	}
}

void resetProgLines() {
	vector<t_wndprog_data>::iterator iter=ollylang->tProgLines.begin();
	int line=0;
	t_wndprog_data *ppl;

	while (iter!=ollylang->tProgLines.end()) {	
		ppl = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),line);
		
		ppl->type = 1;
		
		strcpy(ppl->result,"");	
		strcpy(ppl->values,"");
		
		//ppl->jumpto = 0;
		ppl->eip = 0;

		line++;
		iter++;
	}
	if (ollylang->wndProg.hw!=NULL)	InvalidateRect(ollylang->wndProg.hw, NULL, FALSE);
}

int isProgLineBP(int line) {

	t_wndprog_data *ppl;
	ppl = (t_wndprog_data *) Getsortedbyselection(&(ollylang->wndProg.data),line);
	
	if (ppl->pause && ollylang->wndProg.hw) {
		focusonstop=4;
	}

	return ppl->pause;

}

