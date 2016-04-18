#include <string.h>
// Temp storage
char buffer[65535] = {0};

// -------------------------------------------------------------------------------
// COMMANDS
// -------------------------------------------------------------------------------
bool OllyLang::DoADD(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2 ;
	string str1, str2;

	if (GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		// "0" to force hexa constant
		args = ops[0] + ",0" + ultoa(dw1 + dw2, buffer, 16); 
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	else if (GetSTROpValue(ops[0], str1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		//concate string with ultoa or buffer with hex value
		var v1=str1, v2=dw2;
		v1+=v2;

		args = ops[0] + ", \"" + v1.str +"\"";
		nIgnoreNextValuesHist++;
	    return DoMOV(args);
	}
	else if (GetSTROpValue(ops[0], str1) 
		     && GetANYOpValue(ops[1], str2))
	{
		//Class var for buffer/str concate support
		var v1=str1, v2=str2;
		v1+=v2;

		args = ops[0] + ", " + "\"" + v1.str + "\"";
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	else if (GetANYOpValue(ops[0], str1) 
		     && GetANYOpValue(ops[1], str2))
	{
		//Class var for buffer/str concate support
		var v1=str1, v2=str2;
		v1+=v2;

		args = ops[0] + ", " + "\"" + v1.str + "\"";
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoAI(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F7); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoALLOC(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	ulong addr, size;
	if(GetDWOpValue(ops[0], size))
	{

		HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);
		addr = (DWORD) VirtualAllocEx(hDbgPrc,NULL,size,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		variables["$RESULT"] = addr;
		//Refresh Memory Window
		Listmemory();
		require_ollyloop=1;

		regBlockToFree((void *)addr, size, false);
		return true;
	}
	return false;
}

bool OllyLang::DoAN(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	ulong dw;

	if(GetDWOpValue(ops[0], dw))
	{
		Analysecode(Findmodule(dw));
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoAND(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if (GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 & dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoAO(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F8); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoASK(string args)
{
	string ops[1];
	string title;

	//Reset $RESULT, (when dialog closed)
	variables["$RESULT"] = 0;
	variables["$RESULT_1"] = 0;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetSTROpValue(ops[0], title))
	{
		if (wndProg.hw!=NULL)
			InvalidateRect(wndProg.hw, NULL, FALSE);

		HWND hw = 0; //not modal but dialog need to be closed on plugin close.
		char* returned_buffer = (char*)DialogBoxParam(hinstModule(), MAKEINTRESOURCE(IDD_INPUT), hw, (DLGPROC) InputDialogProc, (LPARAM)title.c_str());
		if ((BYTE)returned_buffer != NULL)
		{
			string returned = returned_buffer;
			delete[256] returned_buffer;
			
			if(is_hex(returned)) 
			{
				variables["$RESULT"] = strtoul(returned.c_str(), 0, 16);
				variables["$RESULT_1"] = (int) (returned.length() / 2);            //size
			}
			else 
			{
				UnquoteString(returned, '"', '"'); // To Accept input like "FFF" (forces string)
				variables["$RESULT"] = returned.c_str();
				variables["$RESULT_1"] = (int) returned.length();
			}
		}
		else
		{
			Pause();
		}
		return true;
	}
	return false;
} 

bool OllyLang::DoASM(string args)
{
	string ops[3],cmd;
	ulong addr, attempt=0;

	if(!CreateOperands(args, ops, 3))
		if(!CreateOperands(args, ops, 2))
			return false;

	t_asmmodel model={0};
	char error[255] = {0};
	int len = 0;

	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], cmd))
	{
		cmd=FormatAsmDwords(cmd);

		GetDWOpValue(ops[2], attempt);

		if((len = Assemble((char*) cmd.c_str(), addr, &model, attempt, 3, error)) <= 0)
		{
			errorstr = error;
			return false;
		}
		else
		{ 
			Writememory(model.code, addr, len, MM_SILENT|MM_DELANAL);
			Broadcast(WM_USER_CHALL, 0, 0);
			variables["$RESULT"] = len;
			require_ollyloop = 1;
			return true;
		}
	}

	return false;
}

bool OllyLang::DoASMTXT(string args)
{
	string ops[2];

	if (!CreateOp(args, ops, 2))
	   return false;

	FILE *fp = NULL;
    t_asmmodel model={0};
	string asmfile;
    char opcode[4096]={0},error[256]={0};
	long len = 0,line = 0;
	ulong addr, p, lens = 0,attempt = 0;


	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1],asmfile))
	{
		if(fopen(asmfile.c_str(), "rb")){
			
			string asmline,asmdoc;
			std::ifstream fin;
			(asmfile.c_str(), ios::in);
			fin.open(asmfile.c_str());

			p = (ulong) opcode;
			while(getline(fin, asmline))
			{
				line++;
				asmline=FormatAsmDwords(asmline);
				len=Assemble((char*) asmline.c_str(), addr+lens, &model, attempt, 3, error);                     
				memcpy((void*) (p + lens),&model.code,len);
				lens += len;
			} 
			Writememory(opcode, addr, lens, MM_DELANAL);
			Broadcast(WM_USER_CHALL, 0, 0);
			variables["$RESULT"] = lens;
			variables["$RESULT_1"] = line;
			require_ollyloop = 1;
			fin.close();
		}	
		return true;
	}
	return false;
}

bool OllyLang::DoATOI(string args)
{
	string ops[2];

	if (!CreateOperands(args, ops, 2)) 
	{
		ops[1] = "10"; //10h=(16.) : Default Hexa number
		if (!CreateOperands(args, ops, 1))
			return false;
	}

	string str;
	ulong base, dw;

	if(GetSTROpValue(ops[0], str) 
		&& GetDWOpValue(ops[1], base) )
	{
		variables["$RESULT"] = strtoul(str.c_str(),0,base);
		return true;
	}
	return false;
}

bool OllyLang::DoBACKUP(string args)
{
	if (DoOPENDUMP(args)) {

		HWND wnd = (HWND) variables["$RESULT"].dw;
		t_dump * dump = dumpWindows[wnd];

		if (dump) {
			Dumpbackup(dump, BKUP_CREATE);
			//Dumpbackup(dump, BKUP_VIEWCOPY);
			return true;
		}
	}

	return false;
}

bool OllyLang::DoBC(string args)
{
	if (args=="")
		return DoBCA(args);

	string ops[1];

	if (!CreateOperands(args, ops, 1))
		return false;

	ulong dw;
	if (GetDWOpValue(ops[0], dw))
	{
		Deletebreakpoints(dw, dw + 1, true);
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

//clear all loaded breakpoints
bool OllyLang::DoBCA(string args)
{
	t_table *bptable;
	t_bpoint *bpoint;
	bptable=(t_table *)Plugingetvalue(VAL_BREAKPOINTS);
	if (bptable!=NULL) 
	{
		for (int b=bptable->data.n-1; b>=0;b--) {
			bpoint=(t_bpoint *)Getsortedbyselection(&(bptable->data),b);
			if (bpoint!=NULL) {
				Deletesorteddata(&(bptable->data),bpoint->addr);
			}
		}
	}
	Broadcast(WM_USER_CHALL, 0, 0);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoBD(string args)
{
	if (args=="")
		return DoBDA(args);

	string ops[1];
	if (!CreateOperands(args, ops, 1))
		return false;

	ulong dw;
	if(GetDWOpValue(ops[0], dw))
	{
		Setbreakpoint(dw, TY_DISABLED, 0);
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

//disable all loaded breakpoints
bool OllyLang::DoBDA(string args)
{
	t_table *bptable;
	t_bpoint *bpoint;
	bptable=(t_table *)Plugingetvalue(VAL_BREAKPOINTS);
	if (bptable!=NULL) {
		for (int b=bptable->data.n-1; b>=0;b--) {
			bpoint=(t_bpoint *)Getsortedbyselection(&(bptable->data),b);
			if (bpoint!=NULL) {
			  Setbreakpoint(bpoint->addr, TY_DISABLED, bpoint->cmd);
			}
		}
	}
	Broadcast(WM_USER_CHALL, 0, 0);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoBEGINSEARCH(string args)
{
	string ops[1];
	ulong start;

	if (!CreateOperands(args, ops, 1))
		start=0;
	else
		if (!GetDWOpValue(ops[0], start)) 
			return false;

	t_memory* tm;
	t_table* tt;

	tt=(t_table*) Plugingetvalue(VAL_MEMORY);
	if (tt==NULL)
		return false;

	if (start==0) {
		//get first mem block addr.
		tm=(t_memory*) Getsortedbyselection(&tt->data, 0);
		if (tm==NULL)
			return false;
		start=tm->base;
	} else {
		//get block at addr
		tm=(t_memory*) Findmemory(start);
		if (tm==NULL)
			return false;
	}

	//Last Memory Bloc

	tm=(t_memory*) Getsortedbyselection(&tt->data, tt->data.n-1);
	if (tm==NULL)
		return false;
 
	int fullsize = tm->base+tm->size - start;

	search_buffer = new unsigned char[fullsize];
	fullsize = Readmemory(search_buffer,start,fullsize,MM_SILENT);
	Havecopyofmemory(search_buffer,start,fullsize);
	//Havecopyofmemory(search_buffer,tm->base,tm->size);
	return true;
}

bool OllyLang::DoENDSEARCH(string args)
{
	if (search_buffer!=NULL) {
		Havecopyofmemory(NULL,0,0);
		delete [] search_buffer;
		search_buffer=NULL;
	}
	return true;
}

bool OllyLang::DoBP(string args)
{
	string ops[1];

	if (!CreateOperands(args, ops, 1))
		return false;

	ulong dw;
	if(GetDWOpValue(ops[0], dw))
	{
		Setbreakpoint(dw, TY_ACTIVE, 0);
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoBPCND(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr = 0;
	string condition;
	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], condition))
	{
		char* buffer = new char[condition.length() + 1];
		strcpy(buffer, condition.c_str());
		Setbreakpoint(addr, TY_ACTIVE, 0);
		Insertname(addr, NM_BREAK, buffer);
		Deletenamerange(addr, addr + 1, NM_BREAKEXPL);
		Deletenamerange(addr, addr + 1, NM_BREAKEXPR);
		Broadcast(WM_USER_CHALL,0,0);
		delete buffer;
		return true;
	}
	return false;
}

bool OllyLang::DoBPD(string args)
{
  	string ops[1];

	if (!CreateOp(args, ops, 1))
		return false;

	return DoBPX(ops[0] + ", 1");

}

//Set On Breakpoint Goto Script Label (addr, label)
bool OllyLang::DoBPGOTO(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr = 0;
	if(GetDWOpValue(ops[0], addr))
	{
		if(labels.find(ops[1]) != labels.end()) {
			AddBPJump(addr,labels[ops[1]]);
			return true;
		}
	}
	return false;
}

bool OllyLang::DoBPHWCA(string args)
{
	int i = 0;
	while(i < 4)
	{
		if (Deletehardwarebreakpoint(i) == -1)
		{return false;}
		i++;
	}
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoBPHWC(string args)
{
	if (args=="")
		return DoBPHWCA(args);

	string ops[1];

	if (!CreateOperands(args, ops, 1))
		return false;

	ulong dw1;
	if(GetDWOpValue(ops[0], dw1))
	{
		Deletehardwarebreakbyaddr(dw1);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoBPHWS(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2)){
		if(!CreateOperands(args, ops, 1))
			return false;
		else ops[1]="\"x\"";
	}

	ulong dw1;
	string str1;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetSTROpValue(ops[1], str1))
	{
		int type;
		if(str1 == "r")
			type = HB_ACCESS;
		else if(str1 == "w")
			type = HB_WRITE;
		else if(str1 == "x")
			type = HB_CODE;
		else
			return false;

		Sethardwarebreakpoint(dw1, 1, type);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoBPL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr = 0;
	string expression;
	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], expression))
	{
		sprintf(buffer, "%c%s", 0x45/*COND_NOBREAK | COND_LOGALWAYS | COND_ARGALWAYS | COND_FILLING*/, expression.c_str());
		Setbreakpoint(addr, TY_ACTIVE, 0);
		Deletenamerange(addr, addr + 1, NM_BREAK);
		Deletenamerange(addr, addr + 1, NM_BREAKEXPL);
		Insertname(addr, NM_BREAKEXPR, buffer);
		Broadcast(WM_USER_CHALL,0,0);
		return true;
	}
	return false;
}

bool OllyLang::DoBPLCND(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3))
		return false;

	ulong addr = 0;
	string expression, condition;
	if (GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], expression) 
		&& GetSTROpValue(ops[2], condition))
	{
		Setbreakpoint(addr, TY_ACTIVE, 0);
		Deletenamerange(addr, addr + 1, NM_BREAKEXPL);
		sprintf(buffer, "%s", condition.c_str());
		Insertname(addr, NM_BREAK, buffer);
		sprintf(buffer, "%c%s", 0x43, expression.c_str());
		Insertname(addr, NM_BREAKEXPR, buffer);
		Broadcast(WM_USER_CHALL,0,0);
		return true;
	}
	return false;
}

bool OllyLang::DoBPMC(string args)
{
	Setmembreakpoint(0, 0, 0);
	return true;
}

bool OllyLang::DoBPRM(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr, size;
	if(GetDWOpValue(ops[0], addr) 
		&& GetDWOpValue(ops[1], size))
	{
		Setmembreakpoint(MEMBP_READ, addr, size);
		return true;
	}
	return false;
}

bool OllyLang::DoBPWM(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr, size;
	if(GetDWOpValue(ops[0], addr) 
		&& GetDWOpValue(ops[1], size))
	{
		Setmembreakpoint(MEMBP_WRITE, addr, size);
		return true;
	}
	return false;
}

//Breakpoint on DLL Calls in current module
//ex: BPX "GetModuleHandleA"
bool OllyLang::DoBPX(string args)
{

    string ops[2];
	int i;
	int bpnmb=0;
	ulong del=0;
	string callname;
	string module;
	bool closeWindow=false;
 
	if(CreateOp(args, ops, 2))
	{
		GetDWOpValue(ops[1], del);
	}
	else
	if(!CreateOp(args, ops, 1))
		return false;


	if (GetSTROpValue(ops[0],callname))
	{
		t_table *reftable;
		t_ref *pref;
  
		char findname[TEXTLEN]={0};
		
		variables["$RESULT"]=0;

		if(callname.length()==0) 
		{
			errorstr="Function name missed";
			return false;
		}
		
		if (callname.find(".") != string::npos)
		{
			module = callname.substr(0, callname.find("."));
			callname = callname.substr(callname.find(".")+1);
		}

		reftable=(t_table *)Plugingetvalue(VAL_REFERENCES);

		//Hide window after if was closed
		if(reftable->hw == 0)
			closeWindow = true;

		Findalldllcalls((t_dump *)Plugingetvalue(VAL_CPUDASM),0,"Intermodular calls");
		reftable=(t_table *)Plugingetvalue(VAL_REFERENCES);

		if(reftable==NULL || reftable->data.n==0)
		{
			errorstr="No references";
			return false;
		}

		if(reftable->data.itemsize<sizeof(t_ref))
		{
			errorstr="Old version of OllyDbg";
			return false;
		}

		for(i=0; i<reftable->data.n; i++) 
		{
			// The safest way: size of t_ref may change in the future!
			pref=(t_ref *)((char *)reftable->data.data+reftable->data.itemsize*i);

			if(Findlabel(pref->dest,findname)==0) 
			{// Unnamed destination
				continue;
			}
   
			if(stricmp(callname.c_str(),findname)!=0) 
			{      // Different function
				continue;
			} 

			if(!del) 
			{                     // Set breakpoint
				Setbreakpoint(pref->addr,TY_ACTIVE,0);
				Deletenamerange(pref->addr,pref->addr+1,NM_BREAK);
				Deletenamerange(pref->addr,pref->addr+1,NM_BREAKEXPL);
				Deletenamerange(pref->addr,pref->addr+1,NM_BREAKEXPR);
				bpnmb++;
			}
			else 
			{
				 Deletebreakpoints(pref->addr,pref->addr+1,true);
				 bpnmb++;
			}
		}
        variables["$RESULT"]=bpnmb;

		if (closeWindow && reftable->hw != 0) 
			DestroyWindow(reftable->hw);

        Broadcast(WM_USER_CHALL,0,0);
        return true;
	}
   return false;
}

bool OllyLang::DoBUF(string args)
{
	string op[1];

	if(!CreateOp(args, op, 1))
		return false;

	if (is_variable(op[0])) {
		if (variables[op[0]].vt == STR) {

			if (!variables[op[0]].isbuf)
				variables[op[0]] = "#"+variables[op[0]].strbuffhex()+"#";

			return true;

		} else if (variables[op[0]].vt == DW) {

			var v; v="##";
			v+=variables[op[0]].dw;
			variables[op[0]]=v;

			return true;

		}
	}
	return false;
}

bool OllyLang::DoCALL(string args)
{
	if (args=="") 
		return false;

	if (labels.find(args)!=labels.end()) 
	{
		calls.push_back(script_pos);
		return DoJMP(args);
	}
	errorstr="Label not found";
	return false;
}

bool OllyLang::DoCLOSE(string args)
{

	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	HWND hwnd = 0;
	string str;
	bool bOk = false;

	t_table * tbl = NULL;
	t_dump * dmp = NULL;

	if( GetSTROpValue("\""+ops[0]+"\"", str) )
	{
		transform(str.begin(), str.end(), str.begin(), toupper);
		
		if(str == "SCRIPT")
		{ 			
			hwnd = wndProg.hw;
			bOk = true;
		}
		else if(str == "SCRIPTLOG")
		{ 
			hwnd = wndLog.hw;
			bOk = true;
		}
		else if(str == "MODULES")
		{ 
			tbl = (t_table *) Plugingetvalue(VAL_MODULES);
			hwnd = tbl->hw;
		}
		else if(str == "MEMORY")
		{
			tbl = (t_table *) Plugingetvalue(VAL_MEMORY);
			hwnd = tbl->hw;
		}
		else if(str == "THREADS")
		{
			tbl = (t_table *) Plugingetvalue(VAL_THREADS);
			hwnd = tbl->hw;
		}
		else if(str == "BREAKPOINTS")
		{
			tbl = (t_table *) Plugingetvalue(VAL_BREAKPOINTS);
			hwnd = tbl->hw;
		}
		else if(str == "REFERENCES")
		{
			tbl = (t_table *) Plugingetvalue(VAL_REFERENCES);
			hwnd = tbl->hw;
		}
		else if(str == "SOURCELIST")
		{
			tbl = (t_table *) Plugingetvalue(VAL_SOURCELIST);
			hwnd = tbl->hw;
		}
		else if(str == "WATCHES")
		{
			tbl = (t_table *) Plugingetvalue(VAL_WATCHES);
			hwnd = tbl->hw;
		}
		else if(str == "PATCHES")
		{
			tbl = (t_table *) Plugingetvalue(VAL_PATCHES);
			hwnd = tbl->hw;
		}
		else if(str == "CPU")
		{
			dmp = (t_dump *) Plugingetvalue(VAL_CPUDASM);
			hwnd = dmp->table.hw;
			if (hwnd != 0) hwnd = GetParent(hwnd);
		}
		else if(str == "RUNTRACE")
		{						
			hwnd = GetODBGWindow(NULL,"ARTRACE");
			bOk = true;
		}
		else if(str == "WINDOWS")
		{
			hwnd = GetODBGWindow(NULL,"AWINDOWS");
			bOk = true;
		}
		else if(str == "CALLSTACK")
		{
			hwnd = GetODBGWindow(NULL,"ACALLSTK");
			bOk = true;
		}
		else if(str == "LOG")
		{
			hwnd = GetODBGWindow(NULL,"ALIST");
			bOk = true;
		}
		else if(str == "TEXT")
		{
			hwnd = GetODBGWindow(NULL,"ASHOWTEXT");
			bOk = true;
		}
		else if(str == "FILE")
		{
			hwnd = GetODBGWindow(NULL,"ADUMPFILE");
			bOk = true;
		}
		else if(str == "HANDLES")
		{
			hwnd = GetODBGWindow(NULL,"AHANDLES");
			bOk = true;
		}
		else if(str == "SEH")
		{
			hwnd = GetODBGWindow(NULL,"ASEH");
			bOk = true;
		}
		else if(str == "SOURCE")
		{
			hwnd = GetODBGWindow(NULL,"ASOURCE");
			bOk = true;
		}

		if (hwnd != 0) {
			DestroyWindow(hwnd);
		}
		if (tbl || dmp || hwnd || bOk)
			return true;
	} 

	if(hwnd==0 && GetDWOpValue(ops[0], (ulong &) hwnd) ) {
		if (hwnd != 0) {
			DestroyWindow(hwnd);
			return true;
		}
	}

	errorstr = "Bad operand";
	return false;
}

bool OllyLang::DoCMP(string args)
{
	string ops[3];
	ulong dw1=0, dw2=0, size=0;
	string s1, s2;

	if(!CreateOperands(args, ops, 3)) {
		if(!CreateOperands(args, ops, 2))
			return false;
	} else {
		GetDWOpValue(ops[2], size);
	}

	if(GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		if (size==1) {
			dw1&=0xFF;
			dw2&=0xFF;
		}
		else if (size==2) {
			dw1&=0xFFFF;
			dw2&=0xFFFF;
		}

		if(dw1 == dw2)
		{
			zf = 1;
			cf = 0;
		}
		else if(dw1 > dw2)
		{
			zf = 0;
			cf = 0;
		}
		else if(dw1 < dw2)
		{
			zf = 0;
			cf = 1;
		}
		return true;
	}
	else if(GetANYOpValue(ops[0], s1, true) // see also SCMP command, code is not finished here...
		    && GetANYOpValue(ops[1], s2, true))
	{
		var v1=s1,v2=s2;
		if (size>0) {
			v1.resize(size);
			v2.resize(size);
		}
		int res = v1.compare(v2); //Error if -2 (type mismatch)
		if(res == 0)
		{
			zf = 1;
			cf = 0;
		}
		else if(res > 0)
		{
			zf = 0;
			cf = 0;
		}
		else if(res < 0)
		{
			zf = 0;
			cf = 1;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoCMT(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	string cmt;
	ulong addr;

	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], cmt))
	{
		if(cmt != "")
		{
            char cmtt[1024]={0};
			strcpy(cmtt, cmt.c_str());
			Insertname(addr, NM_COMMENT, cmtt);
			Broadcast(WM_USER_CHALL, 0, 0);
		}
		else
		{
			Deletenamerange(addr, addr + 1, NM_COMMENT);
			Broadcast(WM_USER_CHALL, 0, 0);
		}
		return true;
	}
	return false;
}

bool OllyLang::DoCOB(string args)
{
	EOB_row = -1;
	return true;
}

bool OllyLang::DoCOE(string args)
{
	EOE_row = -1;
	return true;
}

bool OllyLang::DoDBH(string args)
{
	t_thread* thr = Findthread(Getcputhreadid());
	BYTE buf[4];
	ulong fs = thr->reg.limit[2]; // BUG IN ODBG!!!
	fs += 0x30;
	Readmemory(buf, fs, 4, MM_RESTORE);
	fs = *((ulong*)buf);
	fs += 2;
	buffer[0] = 0;
	Writememory(buf, fs, 1, MM_RESTORE);
	return true;   
}

bool OllyLang::DoDBS(string args)
{
	t_thread* thr = Findthread(Getcputhreadid());
	BYTE buffer[4];
	ulong fs = thr->reg.limit[2]; // BUG IN ODBG!!!
	fs += 0x30;
	Readmemory(buffer, fs, 4, MM_RESTORE);
	fs = *((ulong*)buffer);
	fs += 2;
	buffer[0] = 1;
	Writememory(buffer, fs, 1, MM_RESTORE);
	return true;   
}

bool OllyLang::DoDEC(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	return DoSUB(ops[0] + ", 1");
}

bool OllyLang::DoDIV(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		if(dw2==0)
		{
			errorstr = "Division by 0";
			return false;
		}
		args = ops[0] + ", 0" + ultoa(dw1 / dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}




bool OllyLang::DoDM(string args)
{
	string ops[3];
	if(!CreateOperands(args, ops, 3))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);
	
	ulong addr, size;
	string filename;
	if (GetDWOpValue(ops[0], addr) 
		&& GetDWOpValue(ops[1], size) 
		&& GetSTROpValue(ops[2], filename))
	{
        if (filename.find(":\\") != string::npos)
			path = filename;
		else
			path += filename;
		char* membuf = new char[size];
		int memlen = Readmemory(membuf, addr, size, MM_RESILENT);

		ofstream fout;
        fout.open(path.c_str(), ios::binary);
		if(!fout.fail())
		{
			for(int i = 0; i < memlen; i++)
				fout << membuf[i];
			fout.close();
			delete [] membuf;
            variables["$RESULT"] = memlen;
			return true;
		}
		else
		{
			errorstr = "Couldn't create file!";
			delete [] membuf;
			return false;
		}
	}
	return false;
}

bool OllyLang::DoDMA(string args)
{
	string ops[3];
	if(!CreateOperands(args, ops, 3))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);

	ulong addr, size;
	string filename;
	if(GetDWOpValue(ops[0], addr) 
		&& GetDWOpValue(ops[1], size) 
		&& GetSTROpValue(ops[2], filename))
	{
		char* membuf = new char[size];
		int memlen = Readmemory(membuf, addr, size, MM_RESILENT);

        if (filename.find(":\\") != string::npos)
			path = filename;
		else
			path += filename;
		ofstream fout;
        fout.open(path.c_str(), ios::app | ios::binary);
		if(!fout.fail())
		{
			for(int i = 0; i < memlen; i++)
				fout << membuf[i];
			fout.close();
			delete [] membuf;
            variables["$RESULT"] = memlen;
			return true;
		}
		else
		{
			errorstr = "Couldn't create file!";
			delete [] membuf;
			return false;
		}
	}
	return false;
}

bool OllyLang::DoDPE(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);

	string filename;
	ulong ep;
	if(GetSTROpValue(ops[0], filename) 
		&& GetDWOpValue(ops[1], ep))
	{
        if (filename.find(":\\") != string::npos)
			path = filename;
		else
			path += filename;
		bool result = false;
		result = GetPEInfo(ep);
		if (!result)
			return result;
		result = SaveDump(path, ep);
		return result;
	}
	return false;
}

bool OllyLang::DoENDE(string args)
{
	// Free memory
	if (pmemforexec!=NULL)
		DelProcessMemoryBloc((DWORD) pmemforexec);
	pmemforexec=NULL;
	return true;
}

bool OllyLang::DoERUN(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F9); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoESTI(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F7); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoESTEP(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F8); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoEOB(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(ops[0] == "") // Go interactive
		EOB_row = -1;
	else if(labels.find(ops[0]) != labels.end()) // Set label to go to
		EOB_row = labels[ops[0]];
	else
		return false;
	return true;
}

bool OllyLang::DoEOE(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(ops[0] == "") // Go interactive
		EOE_row = -1;
	else if(labels.find(ops[0]) != labels.end()) // Set label to go to
		EOE_row = labels[ops[0]];
	else
		return false;
	return true;
}

bool OllyLang::DoEVAL(string args)
{
	string ops[1];

	if (!CreateOperands(args, ops, 1))
		return false;

	string to_eval;

	if (GetSTROpValue(ops[0], to_eval))
	{
		string res = ResolveVarsForExec(to_eval,false);
		variables["$RESULT"] = res;
		return true;
	}
	return false;
}

bool OllyLang::DoEXEC(string args)
{
	// Old eip
	ulong eip;
	// For Assemble API
	t_asmmodel model;
	char error[255] = {0};
	char buffer[255] = {0};
	// Bytes assembled
	int len = 0, totallen = 0;
	bool res = true;
	// Temp storage
	string tmp;

	// Get process handle and save eip
	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);
	t_thread* thr = Findthread(Getcputhreadid());
	eip = thr->reg.ip;

	// Allocate memory for code
	pmemforexec = VirtualAllocEx(hDebugee, NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	// Pass EXECs
	script_pos++;
	setProgLineResult(script_pos,variables["$RESULT"]);

	// Assemble and write code to memory (everything between EXEC and ENDE)
	while(ToLower(script[script_pos]) != "ende")
	{
		tmp = ResolveVarsForExec(script[script_pos],true);
		sprintf(buffer, tmp.c_str());
		len = Assemble(buffer, (ulong)pmemforexec + totallen, &model, 0, 0, error);
		if (len < 0) {
			errorstr = tmp+"\n"+error;
			return false;
		} else {
			WriteProcessMemory(hDebugee, (LPVOID)((ulong)pmemforexec + totallen), model.code, len, 0);
			totallen += len;
		}
		script_pos++;
		setProgLineEIP(script_pos,eip);
		setProgLineResult(script_pos,variables["$RESULT"]);
		if (script_pos>script.size()) {
			DoENDE("");
			errorstr = "EXEC needs ENDE command !";
			return false;
		}
	}

	//return before ENDE command
	script_pos--;

	// Assemble and write jump to original EIP
	sprintf(buffer, "JMP %lX", eip);
	len = Assemble(buffer, (ulong)pmemforexec + totallen, &model, 0, 0, error);
	if (len < 0) {
		errorstr = error;
		return false;
	} else {
		WriteProcessMemory(hDebugee, (LPVOID)((ulong)pmemforexec + totallen), model.code, len, 0);
	}

	// Set new eip and run to the original one
	thr->reg.ip = (ulong)pmemforexec;
	thr->reg.modified = 1;
	thr->regvalid = 1;

	// tell to odbgscript to ignore next breakpoint
	bInternalBP=true;

	Broadcast(WM_USER_CHREG, 0, 0);
	Go(Getcputhreadid(), eip, STEP_RUN, 0, 1);
	
	t_dbgmemblock block={0};
	block.hmem = pmemforexec;
	block.size = 0x1000;
	block.script_pos = script_pos;
	block.free_at_eip = eip;
	block.autoclean = true;

	regBlockToFree(block);
	require_addonaction = 1;
	require_ollyloop = 1;

	return true;
}

bool OllyLang::DoFILL(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3))
		return false;
	ulong start, len, val;

	if(GetDWOpValue(ops[0], start) 
		&& GetDWOpValue(ops[1], len) 
		&& GetDWOpValue(ops[2], val))
	{
		BYTE* buffer = new BYTE[len];
		FillMemory(buffer,len,val);
/*		BYTE b = 0 + val;
		for(ulong i = 0; i < len; i++)
			buffer[i] = b;*/
		Writememory(buffer, start, len, MM_SILENT);
		delete []buffer;
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoFIND(string args)
{
	string ops[3];
	ulong maxsize=0;
	if(CreateOp(args, ops, 3)){
		GetDWOpValue(ops[2], maxsize);
	}
	else if(!CreateOp(args, ops, 2))
		return false;

	ulong addr,dw;
	string data=ops[1];
	string finddata=ops[1];

	if(UnquoteString(ops[1], '#', '#')) 
	{
		if(ops[1].length() % 2 != 0)
		{
			errorstr = "Hex string must have an even number of characters!";
			return false;
		}
		finddata=ops[1];
	}
	else if (UnquoteString(ops[1], '"', '"'))
	{
	   string wilddata;
	   wilddata=ops[1];
       Str2Hex(ops[1],finddata);
	   ReplaceString(finddata,"3f","??");
	}
	else if(GetDWOpValue(ops[1], dw) && !is_variable(ops[1])) 
	{
		itoa(rev(dw),buffer,16);
		string data1=buffer;
		transform( data1.begin(),data1.end(), data1.begin(),(int(*)(int)) toupper );
		while(data1.length() < data.length())
			data1.insert(0,"0");
		while (data1.length() > data.length())
			data1.erase(data1.length()-1,1);
		
		finddata=data1;		
	}
	else if(is_variable(ops[1]))
	{   
        GetANYOpValue(ops[1], data);
		if (UnquoteString(data, '#', '#'))
		{
		  finddata=data;	
		}
   	    else if(GetSTROpValue(ops[1],data))
		{
		  Str2Hex(data,finddata);
		}
        else if(GetDWOpValue(ops[1], dw))
		{
			itoa(rev(dw),buffer,16);
			string data1=buffer;
			while(data1.length() < data.length())
				data1.insert(0,"0");
			while (data1.length() > data.length())
				data1.erase(data1.length()-1,1);

			finddata=data1;
		}
	}

	if(GetDWOpValue(ops[0], addr) && is_hexwild(finddata))
	{

		t_memory* tmem = Findmemory(addr);

		if (tmem==NULL) {
			// search in current mem block
			variables["$RESULT"] = 0;
			return true;
		}

		if(finddata.find('?') != -1)
		{
			// Wildcard search
			char *membuf = 0;
			int memlen = tmem->size - (addr - tmem->base);
			membuf = new char[memlen];

			memlen = Readmemory(membuf, addr, memlen, MM_RESILENT);
            int pos;

			if (maxsize && maxsize <= memlen)
				pos = FindWithWildcards(membuf, finddata.c_str(), maxsize);
			else
				pos = FindWithWildcards(membuf, finddata.c_str(), memlen);

			delete [] membuf;

			if(pos > -1)
				variables["$RESULT"] = addr + pos;
			else
				variables["$RESULT"] = 0;
		}
		else
		{
			// Regular search
			char *membuf = 0;
			int memlen = tmem->size - (addr - tmem->base);

			int len = Str2Rgch(finddata, buffer, sizeof(buffer));
            if (maxsize && maxsize <= memlen)
			{
				memlen = maxsize;
			}
			membuf = new char[memlen];
			memlen = Readmemory(membuf, addr, memlen, MM_RESILENT);
			char *p = search(membuf, membuf + memlen, buffer, buffer + len);

			delete[] membuf;

			if(p < membuf + memlen)
				variables["$RESULT"] = addr + p - membuf;
			else
				variables["$RESULT"] = 0;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoFINDCALLS(string args)
{
	string ops[2];
	if(!CreateOp(args, ops, 2))
		if (args=="")
			return false;

	bool bRefVisible=false,bResetDisam=false;
	ulong addr,base,size,disamsel=0;

	Getdisassemblerrange(&base,&size);

	//Get initial Ref Window State
	t_table* tt;
	tt=(t_table*) Plugingetvalue(VAL_REFERENCES);
	if (tt!=NULL)
		bRefVisible=(tt->hw!=0);

	t_dump* td;
	td=(t_dump*) Plugingetvalue(VAL_CPUDASM);
	if (td==NULL)
		return false;

	if (GetDWOpValue(ops[0], addr))
	{
		if (addr<base || addr>=(base+size)) {
			//outside debugger window range
			disamsel=td->sel0;
			Setdisasm(addr,0,0);
			bResetDisam=true;
		}

		variables["$RESULT"]=0;
		if (Findalldllcalls(td,addr,NULL)>0) {
			
			if (tt==NULL)
				tt=(t_table*) Plugingetvalue(VAL_REFERENCES);

			if (tt!=NULL) 
			{
				t_ref* tr;

				if (tt->data.n > 1) 
				{ 
					//Filter results
					string filter;
					if (GetSTROpValue(ops[1], filter) && filter!="") {
						//filter=ToLower(filter);
						(char*) buffer[TEXTLEN+1];
						for (int nref=tt->data.n-1;nref>0;nref--) {
							tr=(t_ref*) Getsortedbyselection(&tt->data, nref);
							if (tr!=NULL && tr->dest!=0) {
								//ZeroMemory(buffer,TEXTLEN+1);
								//Decodename(tr->dest,NM_LABEL,buffer);
								Findlabel(tr->dest,buffer);
								if (stricmp(buffer,filter.c_str())!=0)
									Deletesorteddata(&tt->data,tr->addr);
							}
						}
					}

					tr=(t_ref*) Getsortedbyselection(&tt->data, 1); //0 is CPU initial
					if (tr!=NULL)
						variables["$RESULT"]=tr->addr;
				}

				if (tt->hw && !bRefVisible) {
					DestroyWindow(tt->hw);
					tt->hw=0;
				}
			}
		}
		if (bResetDisam)
			Setdisasm(disamsel,0,0);
		return true;
	}
	return false;
}

//search for asm command in disasm window
bool OllyLang::DoFINDCMD(string args)
{
	string ops[2];
	if(!CreateOp(args, ops, 2))
		return false;

	bool bRefVisible=false,bResetDisam=false;
	string cmd, cmds;
	int len,pos;
	ulong addr,base,size,attempt,opsize=3,disamsel=0;
	int startadr=0,endadr=0,lps=0,length,ncmd=0,cmdpos=0;
	char error[256]={0};

	Getdisassemblerrange(&base,&size);

	//Get initial Ref Window State
	t_table* tt;
	tt=(t_table*) Plugingetvalue(VAL_REFERENCES);
	if (tt!=NULL)
		bRefVisible=(tt->hw!=0);

	t_dump* td;
	td=(t_dump*) Plugingetvalue(VAL_CPUDASM);
	if (td==NULL)
		return false;

	ulong tmpaddr=AddProcessMemoryBloc(0x100,PAGE_EXECUTE_READWRITE);

	if (GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], cmds))
	{
		if (addr<base || addr>=(base+size)) {
			//outside debugger window range
			disamsel=td->sel0;
			Setdisasm(addr,0,0);
			bResetDisam=true;
		}

		t_asmmodel model={0};
		t_extmodel models[NSEQ][NMODELS]={0};

		length = cmds.length();
		while (cmdpos<length && ncmd<NSEQ)
		{

			endadr= cmds.find(";",startadr);
			if (endadr==-1)
			{
				endadr=length;
			}
			lps=endadr-startadr;

			cmd=cmds.substr(startadr,lps);
			
			attempt=0;
			strcpy(buffer, cmd.c_str());

			do {

				if((len = Assemble(buffer, tmpaddr, &model, attempt, opsize, error)) <= 0)
				{
					if (attempt!=0) {
						break;
					}

					pos=(cmd.length()+len);
					if (pos>=0 && pos<cmd.length())
						errorstr = "\nFINDCMD error at \""+cmd.substr(pos,cmd.length()-pos)+"\"!\n\n";
					else
						errorstr = "\nFINDCMD error !\n\n";
					errorstr.append(error);
					goto return_false;
				}
				memcpy(&models[ncmd][attempt],&model,sizeof(model));
				attempt++;

			} while (len>0 && attempt<NMODELS);

			startadr=endadr+1;
			cmdpos+=lps+1;

			ncmd++;
		}

		variables["$RESULT"]=0;
		if (Findallsequences(td,models,addr,NULL)>0) {
			
			if (tt==NULL)
				tt=(t_table*) Plugingetvalue(VAL_REFERENCES);

			if (tt!=NULL) 
			{
				t_ref* tr;
				if (tt->data.n > 1)
				{
					tr=(t_ref*) Getsortedbyselection(&tt->data, 1); //0 is CPU initial
					if (tr!=NULL)
						variables["$RESULT"]=tr->addr;
				}

				if (tt->hw && !bRefVisible) {
					DestroyWindow(tt->hw);
					tt->hw=0;
				}
			}
		}
		DelProcessMemoryBloc(tmpaddr);
		if (bResetDisam)
			Setdisasm(disamsel,0,0);
		return true;

	}
return_false:
	if (bResetDisam)
		Setdisasm(disamsel,0,0);
	DelProcessMemoryBloc(tmpaddr);
	return false;
}
/*
//Buggy, could assemble different command code bytes, (from chinese code)
bool OllyLang::DoFINDCMDS(string args)
{

	string ops[2];
	t_asmmodel model;
	ulong addr;
	string cmds,args1,cmd;
	char opcode[256]={0},buff[32]={0},tmp[64]={0},error[64]={0};
	int i,pos,len=0,length=0,startadr=0,endadr=0,lps=0,codelen=0;
	int attempt=0,opsize=3;

	if(!CreateOp(args, ops, 2))
		return false;

	if (GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], cmds))
	{

	  if (cmds.find(";")==-1)
	  {
		nIgnoreNextValuesHist++;
		return DoFINDoneCMD(args);
	  }

	  length = cmds.length();
 
	  ulong tmpaddr=AddProcessMemoryBloc(0x100,PAGE_EXECUTE_READWRITE);

	  while (len<length)
	  {
		endadr= cmds.find(";",startadr);
		if (endadr==-1)
		{
			endadr=length;
		}
		lps=endadr-startadr;
		cmd=cmds.substr(startadr,lps);
       
		strcpy(buffer, cmd.c_str());
		if((codelen = Assemble(buffer, tmpaddr, &model, attempt, opsize, error)) <= 0)
		{
			pos=(cmd.length()+codelen);
			if (pos>=0 && pos<cmd.length())
				errorstr = "\nFINDCMDS error on \""+cmd.substr(pos,cmd.length()-pos)+"\"!\n\n";
			else
				errorstr = "\nFINDCMDS error !\n\n";
			errorstr.append(error);
			DelProcessMemoryBloc(tmpaddr);
			return false;
		}
		else
		{
			sprintf(buff, "%s", (model.code));
		}

		i=0;
		while(i<codelen)
		{
			itoa(buff[i],tmp,16);
			i++;
			strcat(opcode,tmp);
		}

		startadr=endadr+1;
		len=len+lps+1;
	  }
	  DelProcessMemoryBloc(tmpaddr);

	  args1 = ops[0] + ", " + "#" + opcode + "#";
	  nIgnoreNextValuesHist++;
	  return DoFIND(args1);
	}
	return false;
}
*/

bool OllyLang::DoFINDMEM(string args)
{

	if (args=="")
		return false;

	string ops[2];

	if (args.find(',') == -1)
		args+=",0";

	if(!CreateOperands(args, ops, 2))
		return false;

	t_table* tt;
	tt=(t_table*) Plugingetvalue(VAL_MEMORY);
	if (tt==NULL)
		return false;

	t_memory* tm;
	char szBase[9];
	ulong start=0; 
	string sArgs;
	bool bSkip;

	if (!GetDWOpValue(ops[1], start))
		return false;

	for (int m=0; m < tt->data.n; m++) {
		tm=(t_memory*) Getsortedbyselection(&tt->data, m);
		if (tm==NULL) {
			return false;
		}

		itoa(tm->base,szBase,16);

		bSkip=false;

		if (start > tm->base + tm->size)
			bSkip=true;
		else if (start >= tm->base) 
			itoa(start,szBase,16);

		if (!bSkip) {
			sArgs="";
			sArgs.append(szBase);
			sArgs+=","+ops[0];
			if (DoFIND(sArgs)) {
				if (variables["$RESULT"].dw!=0 || variables["$RESULT"].str!="") {
					var_logging=true;
					return true;
				}
			}
			//Display value in ollyscript wdw only once
			var_logging=false;
		}
	}

	var_logging=true;
	variables["$RESULT"] = 0;
	return true;
}

bool OllyLang::DoFINDOP(string args)
{
	string ops[3];
	ulong maxsize=0;
	if(CreateOp(args, ops, 3)){
		GetDWOpValue(ops[2], maxsize);
	}
	else if(!CreateOp(args, ops, 2))
		return false;

	ulong addr, dw, endaddr;
	string data=ops[1];
	string finddata=ops[1];

	if (UnquoteString(ops[1], '#', '#')) 
	{
		if(ops[1].length() % 2 != 0)
		{
			errorstr = "Hex string must have an even number of characters!";
			return false;
		}
	}
	else if (GetDWOpValue(ops[1], dw) && !is_variable(ops[1]))
	{
//		DoREV(ops[1]);
		itoa(dw,buffer,16);
        string data1=buffer;
		while(data1.length() < data.length())
			data1.insert(0,"0");
		while (data1.length() > data.length())
			data1.erase(data1.length()-1,1);

		ops[1]=data1;
	}
	else if(is_variable(ops[1]))
	{   
        GetANYOpValue(ops[1], data);
		if (UnquoteString(data, '#', '#'))
		{
		  ops[1]=data;	
		 }
   	    else if(GetSTROpValue(ops[1],data))
		{
			errorstr = "findop: incorrect data !";
			return false;
		}
        else if(GetDWOpValue(ops[1], dw))
		{
	//		DoREV(ops[1]);
			itoa(dw,buffer,16);
			string data1=buffer;
			while(data1.length() < data.length())
				data1.insert(0,"0");
			while (data1.length() > data.length())
				data1.erase(data1.length()-1,1);
			
			ops[1]=data1;
		}
	}

	int result = -1;
	int ok = 0;
	ulong addrsize;
	if(GetDWOpValue(ops[0], addr) && is_hexwild(ops[1]))
	{
		t_memory* tmem = Findmemory(addr);
		char cmd[MAXCMDSIZE] = {0};

		if(maxsize && maxsize <= tmem->size)
		{
			tmem->base = addr;
			tmem->size = maxsize;
		}

		do 
		{
			ulong nextaddr = Disassembleforward(0, tmem->base, tmem->size, addr, 1, 0); 
//			endaddr = Disassembleforward(0, tmem->base, tmem->size, addr, 1, 0); 
			ok = Readcommand(addr, cmd);

			addrsize = nextaddr -addr;
            addr=nextaddr;

			if(addr >= tmem->base + tmem->size)
				ok = 0;

			if(ok)
				result = FindWithWildcards(cmd, ops[1].c_str(),addrsize);

		} while(result != 0 && ok != 0);
	}
	if(ok != 0){
		variables["$RESULT"] = addr - addrsize;
	    variables["$RESULT_1"] = addrsize;
	}


	else
		variables["$RESULT"] = 0;
	return true;
}

bool OllyLang::DoFINDOPREV(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	if(ops[1].length() % 2 != 0)
	{
		errorstr = "Hex string must have an even number of characters!";
		return false;
	}

	DWORD addr, endaddr;
	int result = -1;
	int ok = 0;

	if(GetDWOpValue(ops[0], addr) && UnquoteString(ops[1], '#', '#'))
	{
		t_memory* tmem = Findmemory(addr);
		char cmd[MAXCMDSIZE] = {0};
		do 
		{
			addr = Disassembleback(0, tmem->base, tmem->size, addr, 1, 0); 
			endaddr = Disassembleback(0, tmem->base, tmem->size, addr, 1, 0); 
			ok = Readcommand(addr, cmd);

			if(addr == tmem->base + tmem->size)
				ok = 0;

			if(ok)
				result = FindWithWildcards(cmd, ops[1].c_str(), endaddr - addr);
		} while(result != 0 && ok != 0);
	}
	if(ok != 0)
		variables["$RESULT"] = addr;
	else
		variables["$RESULT"] = 0;
	return true;

}


bool OllyLang::DoFREE(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2)) {
		ops[1]="0";
		if(!CreateOperands(args, ops, 1))
		return false;
	}

	void * hMem;
	ulong addr, size;

	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size))
	{
		hMem = (void*)addr;
		//To Refresh Memory Window
		require_ollyloop=1;
		if (size==0) {
			if (DelProcessMemoryBloc(addr)) {
				unregMemBlock(hMem);
				Listmemory();
				return true;
			}
		}
		else {
			
			HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);
			variables["$RESULT"] = (DWORD) VirtualFreeEx(hDbgPrc,hMem,size,MEM_DECOMMIT);
			unregMemBlock(hMem);
			Listmemory();
			return true;
		}
	}
	return false;
}

bool OllyLang::DoGAPI(string args)
{
    string ops[1];
	ulong addr,size,test,addr2;

	if(!CreateOp(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr)){
		BYTE buffer[MAXCMDSIZE];
		//size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
		size=Readcommand(addr,(char *) buffer);

		
		if (size>0) {
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_CODE,NULL);
			test=disasm.jmpaddr;

			if (size>0) {
//				variables["$RESULT"] = disasm.result; //asm command text
//				variables["$RESULT_1"] = disasm.dump;     //command bytes
				variables["$RESULT_3"] = disasm.addrdata;
				variables["$RESULT_4"] = disasm.jmpaddr; 

			}
		}
		if (test!=NULL)   {
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_CODE,NULL);
		 	char sym[4096] = {0};
		    char buf[TEXTLEN] = {0};
			addr2 = disasm.addrdata;
            int res = Decodeaddress(addr2, 0, ADC_JUMP | ADC_STRING | ADC_ENTRY | ADC_OFFSET | ADC_SYMBOL, sym, 4096, buf);
		    if(res)
			{
			variables["$RESULT"] = sym;
			char *tmp = strstr(sym, ".");
			if(tmp)
			{
				strtok(sym, ">");                          //buxfix
				*tmp = '\0';
				variables["$RESULT_1"] = sym + 2;          //bugfix
				variables["$RESULT_2"] = tmp + 1;
			}
		}
		    return true;
		}
		variables["$RESULT"] = NULL;
		return true;
	}
    return false;
}

bool OllyLang::DoGBPM(string args)
{
	variables["$RESULT"]=break_memaddr;
	return true;
}

//Get Breakpoint Reason
bool OllyLang::DoGBPR(string args)
{
	string ops[1];

	if (break_reason == PP_MEMBREAK) {
		//Mem Breakpoint Reason

		break_memaddr=0;
		t_thread* t;
		t = Findthread(Getcputhreadid());
		CONTEXT context;
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		GetThreadContext(t->thread, &context);

		ulong tHwBpAddr[4];
		tHwBpAddr[0]=context.Dr0;
		tHwBpAddr[1]=context.Dr1;
		tHwBpAddr[2]=context.Dr2;
		tHwBpAddr[3]=context.Dr3;

		BYTE buffer[MAXCMDSIZE];
		ulong size=Readmemory(buffer, t->reg.ip, MAXCMDSIZE, MM_SILENT);
		if (size>0) {
			t_disasm disasm;
			size=Disasm(buffer,size,t->reg.ip,NULL,&disasm,DISASM_DATA,NULL);
			if (size>0) {
				char i,j;
				for (j=0;j<3;j++)
					if (disasm.opaddr[j]!=0 && disasm.opgood[j]) 
					{
						break_memaddr=disasm.opaddr[j];
						for (i=0;i<4;i++) 
						if (tHwBpAddr[i]==disasm.opaddr[j])
							break;
					}
			}
		}
	}

	if(CreateOp(args, ops, 1))
	{
		if (is_variable(ops[0])) {
			variables[ops[0]] = break_reason;
			return true;
		}
	} 
	variables["$RESULT"] = break_reason;
	return true;
}

//Get Code Information
bool OllyLang::DoGCI(string args)
{
    string ops[2], param;
	ulong addr,size;

	if(!CreateOp(args, ops, 2))
		return false;

	if ( GetDWOpValue(ops[0], addr) 
	   && GetSTROpValue("\""+ops[1]+"\"", param) )
	{

		transform(param.begin(), param.end(), param.begin(), toupper);

		BYTE buffer[MAXCMDSIZE];
//		size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
		size=Readcommand(addr,(char *) buffer);

		if (size>0) 
		{
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_CODE,NULL);

			if (size<=0)
				return false;
			else if (param == "COMMAND")
			{
				string s;
				s.assign(disasm.result);
				while (s.find("  ") != string::npos) ReplaceString(s,"  "," ");
				variables["$RESULT"] = s; 
				return true;
			}
			else if (param == "CONDITION") 
			{
				variables["$RESULT"] = disasm.condition; 
				return true;
			}
			else if (param == "DESTINATION") 
			{
				variables["$RESULT"] = disasm.jmpaddr; 
				return true;
			}
			else if (param == "SIZE") 
			{
				variables["$RESULT"] = size; 
				return true;
			}
			else if (param == "TYPE") 
			{
				variables["$RESULT"] = disasm.cmdtype; 
				return true;
			}
		}
	}
	return false;
}

bool OllyLang::DoGCMT(string args)
{
	string comment;
	ulong addr,size;

	if(GetDWOpValue(args, addr))
	{
		if(addr != 0)
		{
			size = Findname(addr, NM_COMMENT, buffer);
			if (size==0)
				size = Findname(addr, NM_LIBCOMM, buffer);
			if (size==0)
				size = Findname(addr, NM_ANALYSE, buffer);		
			comment = buffer;
			if(comment==""){
			   variables["$RESULT"] = " ";
               return true;
			}  
			variables["$RESULT"] = comment;
			return true;
		}
	}
	return false;
}

bool OllyLang::DoGFO(string args)
{
	string comment;
	ulong addr;

	if(GetDWOpValue(args, addr))
	{
		if(addr != 0)
		{
			t_module* mod = (t_module*) Findmodule(addr);
			if (mod!=NULL) {
				variables["$RESULT"] = Findfileoffset(mod, addr);
			} else {
				variables["$RESULT"] = 0;
			}
			return true;
		}
	}
	return false;
}

// Get Label
// returns label from address
// glbl 
bool OllyLang::DoGLBL ( string args )
{
	string comment;
	ulong addr, type;
	char buffer[TEXTLEN]={0};

	if (!GetDWOpValue(args, addr))
		return false;
	{
		variables["$RESULT"] = 0;

		if (addr != 0)
		{
			if (Findlabel ( addr, buffer ) != NM_LABEL)
				variables["$RESULT"] = 0;

			comment.assign(buffer);
			if (comment == "")
				variables["$RESULT"] = 0;

			variables["$RESULT"] = comment;
		}
	}
	return true;
}

bool OllyLang::DoGMA(string args)
{
	string str,ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	if(GetSTROpValue(ops[0], str)) {

		if (str.length() > 8)
			str = str.substr(0,8);

		//Module names : Spaces to underscores
		while(str.find(" ")!=string::npos)
			str.replace(str.find(" "),1,"_");

		Listmemory();

		int t=Plugingetvalue(VAL_MEMORY);
		if (t<=0)
			return false;

		t_table* ttab=(t_table*) t;
		t_memory* mem;
		t_module* mod;
		string sMod;

		for (int n=0;n<ttab->data.n;n++) {
	
			mem = (t_memory*) Getsortedbyselection(&ttab->data,n);
			mod = (t_module*) Findmodule(mem->base);
			if (mod!=NULL) {
				sMod.assign(mod->name,SHORTLEN);
				if (stricmp(sMod.c_str(),str.c_str())==0) {
					Int2Hex(mem->base, str);
					nIgnoreNextValuesHist++;
					return DoGMI(str+","+ops[1]);		
				}
			}
		}
	}

	variables["$RESULT"] = 0;

	return true;

}

bool OllyLang::DoGMEMI(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr = 0;
	string str;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) )
	{
		transform(str.begin(), str.end(), str.begin(), toupper);

		t_memory* mem = Findmemory(addr);
		if(str == "MEMORYBASE" && mem != NULL)
		{
			variables["$RESULT"] = (DWORD)mem->base;
			return true;
		}
		else if(str == "MEMORYSIZE" && mem != NULL)
		{
			variables["$RESULT"] = (DWORD)mem->size;
			return true;
		}
		else if(str == "MEMORYOWNER" && mem != NULL)
		{
			variables["$RESULT"] = (DWORD)mem->owner;
			return true;
		}
		else if(mem == NULL)
		{
			variables["$RESULT"] = 0;
			return true;
		}
		else
		{			
			//DoGMI(Int2Str(mem->base)+" ops[1]);
			variables["$RESULT"] = 0;
			errorstr = "Second operand bad";
			return false;
		}
	}

	errorstr = "Bad operand";
	return false;
}

bool OllyLang::DoNAMES(string args)
{
	string ops[1];
	ulong addr;
	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr)) {
		t_module* mod = Findmodule(addr);
		Setdisasm(mod->codebase,1,0);
		Sendshortcut(PM_DISASM, 0, WM_KEYDOWN, 1, 0, 'N');  //"Ctrl + N"
		require_ollyloop = 1;
		return true;
	}

	return false;
}

bool OllyLang::DoGMEXP(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="0";
		if(!CreateOperands(args, ops, 2))
			return false;
	}

	ulong i, num, addr, count=0;
	string str;
	bool cache=false, cached=false;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) && GetDWOpValue(ops[2], num) )
	{
		transform(str.begin(), str.end(), str.begin(), toupper);

		t_module * mod = Findmodule(addr);
		if (!mod) {
			variables["$RESULT"] = 0;
			return true;
		}

		t_export exp={0};

		if (str == "COUNT") {
			cache = true;
			tExportsCache.clear();
			exportsCacheAddr = addr;
		} else {
			if (exportsCacheAddr == addr && num < tExportsCache.size()) {
				exp = tExportsCache[num];
				count = tExportsCache.size();
				cached = true;
			}
		}

		if (!cached)
		for(i = 0; i < mod->codesize ; i++) {
			if (Findname(mod->codebase + i, NM_EXPORT, exp.label))
			{
				count++;
				exp.addr=mod->codebase + i;
				if (count==num && !cache) break;
				if (cache) {
					tExportsCache.push_back(exp);
				}
			}
		}

		if (num > count) //no more
		{
			variables["$RESULT"] = 0;
			return true;
		}

		if(str == "COUNT")
		{
			variables["$RESULT"] = count;
			return true;
		}
		else if(str == "ADDRESS")
		{
			variables["$RESULT"] = exp.addr;
			return true;
		}
		else if(str == "LABEL")
		{
			variables["$RESULT"] = exp.label;
			return true;
		}
		else
		{
			variables["$RESULT"] = 0;
			errorstr = "Second operand bad";
			return false;
		}
	}

	errorstr = "Bad operand";
	return false;
}

bool OllyLang::DoGMI(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong addr = 0;
	string str;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) )
	{
		t_module* mod = Findmodule(addr);

		transform(str.begin(), str.end(), str.begin(), toupper);
		
		if(mod == NULL)
		{
			variables["$RESULT"] = 0;
			return true;
		}
		else if(str == "MODULEBASE")
		{ 
			variables["$RESULT"] = (DWORD)mod->base;
			return true;
		}
		else if(str == "MODULESIZE")
		{
			variables["$RESULT"] = (DWORD)mod->size;
			return true;
		}
		else if(str == "CODEBASE")
		{
			variables["$RESULT"] = (DWORD)mod->codebase;
			return true;
		}
		else if(str == "CODESIZE")
		{
			variables["$RESULT"] = (DWORD)mod->origcodesize;
			return true;
		}
		else if(str == "ENTRY")
		{
			variables["$RESULT"] = (DWORD)mod->entry;
			return true;
		}
		else if(str == "NSECT")
		{
			variables["$RESULT"] = (DWORD)mod->nsect;
			return true;
		}
		else if(str == "DATABASE")
		{
			variables["$RESULT"] = (DWORD)mod->database;
			return true;
		}
		else if(str == "EDATATABLE")
		{
			variables["$RESULT"] = (DWORD)mod->edatatable;
			return true;
		}
		else if(str == "EDATASIZE")
		{
			variables["$RESULT"] = (DWORD)mod->edatasize;
			return true;
		}
		else if(str == "IDATABASE")
		{
			variables["$RESULT"] = (DWORD)mod->idatabase;
			return true;
		}
		else if(str == "IDATATABLE")
		{
			variables["$RESULT"] = (DWORD)mod->idatatable;
			return true;
		}
		else if(str == "RESBASE")
		{
			variables["$RESULT"] = (DWORD)mod->resbase;
			return true;
		}
		else if(str == "RESSIZE")
		{
			variables["$RESULT"] = (DWORD)mod->ressize;
			return true;
		}
		else if(str == "RELOCTABLE")
		{
			variables["$RESULT"] = (DWORD)mod->reloctable;
			return true;
		}
		else if(str == "RELOCSIZE")
		{
			variables["$RESULT"] = (DWORD)mod->relocsize;
			return true;
		}
		else if(str == "NAME")
		{
			variables["$RESULT"] = (char*)mod->name;
			if (variables["$RESULT"].str.length() >	SHORTLEN)
				variables["$RESULT"].str = variables["$RESULT"].str.substr(0,SHORTLEN);
			return true;
		}
		else if(str == "PATH")
		{
			variables["$RESULT"] = (char[MAX_PATH])mod->path;
			return true;
		}
		else if(str == "VERSION")
		{
			variables["$RESULT"] = (char*)mod->version;
			return true;
		}
		else
		{
			variables["$RESULT"] = 0;
			errorstr = "Second operand bad";
			return false;
		}
	}
	errorstr = "Bad operand";
	return false;
}

bool OllyLang::DoGMIMP(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="0";
		if(!CreateOperands(args, ops, 2))
			return false;
	}

	ulong i, num, addr, count=0;
	string str;
	bool cache=false, cached=false;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) && GetDWOpValue(ops[2], num) )
	{
		transform(str.begin(), str.end(), str.begin(), toupper);

		t_module * mod = Findmodule(addr);
		if (!mod) {
			variables["$RESULT"] = 0;
			return true;
		}

		t_export exp={0};

		if (str == "COUNT") {
			cache = true;
			tImportsCache.clear();
			importsCacheAddr = addr;
		} else {
			if (importsCacheAddr == addr && num < tImportsCache.size()) {
				exp = tImportsCache[num];
				count = tImportsCache.size();
				cached = true;
			}
		}

		if (!cached)
		for(i = 0; i < mod->codesize ; i++) {
			if (Findname(mod->codebase + i, NM_IMPORT, exp.label))
			{
				count++;
				exp.addr=mod->codebase + i;
				if (count==num && !cache) break;
				if (cache) {
					tImportsCache.push_back(exp);
				}
			}
		}

		if (num > count) //no more
		{
			variables["$RESULT"] = 0;
			return true;
		}

		if(str == "COUNT")
		{
			variables["$RESULT"] = count;
			return true;
		}
		else if(str == "ADDRESS")
		{
			variables["$RESULT"] = exp.addr;
			return true;
		}
		else if(str == "LABEL")
		{
			variables["$RESULT"] = exp.label;
			return true;
		}
		else if(str == "NAME")
		{
			string s = exp.label;
			if (s.find(".") != string::npos) {
				variables["$RESULT"] = s.substr(s.find(".")+1);
			}
			else 
				variables["$RESULT"] = exp.label;

			return true;
		}
		else if(str == "MODULE")
		{
			string s = exp.label;
			if (s.find(".") != string::npos) {
				variables["$RESULT"] = s.substr(0,s.find("."));
			}
			else 
				variables["$RESULT"] = "";
			return true;
		}
		else
		{
			variables["$RESULT"] = 0;
			errorstr = "Second operand bad";
			return false;
		}
	}

	errorstr = "Bad operand";
	return false;
}

bool OllyLang::DoGN(string args)
{
	string ops[1];
	ulong addr;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr))
	{
		char sym[4096] = {0};
		char buf[TEXTLEN] = {0};
//		int res = Decodeaddress(addr, 0, ADC_STRING, sym, 4096, buf);
		int res = Decodeaddress(addr, 0, ADC_JUMP | ADC_STRING | ADC_ENTRY | ADC_OFFSET | ADC_SYMBOL, sym, 4096, buf);
		if(res)
		{
			variables["$RESULT"] = sym;
			char* tmp = strstr(sym, ".");
			if(tmp)
			{
				strtok(sym, ">");                      
				*tmp = '\0';
				variables["$RESULT_1"] = sym; //+ 2 ... not a bug Charset pb ?
				variables["$RESULT_2"] = tmp + 1;
			}
		}
		else
		{
			variables["$RESULT"] = 0;
			variables["$RESULT_1"] = 0;
			variables["$RESULT_2"] = 0;
		}

		return true;
	}

	return false;
}

bool OllyLang::DoGO(string args)
{
	string ops[1];
	ulong addr;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr))
	{
		Go(Getcputhreadid(), addr, STEP_RUN, 1, 1);
		require_ollyloop = 1;
      return true;
	}

	return false;
}

//Get Code Information
bool OllyLang::DoGOPI(string args)
{
    string ops[3], param;
	ulong addr,size,index;

	if(!CreateOp(args, ops, 3))
		return false;

	if ( GetDWOpValue(ops[0], addr) 
	   && GetDWOpValue(ops[1], index) 
	   && GetSTROpValue("\""+ops[2]+"\"", param) )
	{

		index--;
		if (index < 0 || index > 2) {
			errorstr = "Bad operand index (1-3) !";
			return false;
		}

		transform(param.begin(), param.end(), param.begin(), toupper);

		BYTE buffer[MAXCMDSIZE]={0};
//		size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
		size=Readcommand(addr,(char *) buffer);

		if (size>0) 
		{
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_ALL,Getcputhreadid());

			if (size<=0)
				return false;
			else if (param == "TYPE")
			{
				variables["$RESULT"] = disasm.optype[index]; // Type of operand (extended set DEC_xxx)
				return true;
			}
			else if (param == "SIZE") 
			{
				variables["$RESULT"] = disasm.opsize[index]; // Size of operand, bytes
				return true;
			}
			else if (param == "GOOD") 
			{
				variables["$RESULT"] = disasm.opgood[index]; // Whether address and data valid
				return true;
			}
			else if (param == "ADDR") 
			{
				variables["$RESULT"] = disasm.opaddr[index]; // Address if memory, index if register
				return true;
			}
			else if (param == "DATA") 
			{
				variables["$RESULT"] = disasm.opdata[index]; // Actual value (only integer operands)
				return true;
			}
		}
	}
	return false;
}


//gpa "LoadLibraryA","kernel32.dll"
bool OllyLang::DoGPA(string args)
{
	string ops[3];
	ulong dontfree=0;

	if(CreateOperands(args, ops, 3))
		GetDWOpValue(ops[2], dontfree);
	else
		if(!CreateOperands(args, ops, 2))
			return false;

	variables["$RESULT"] = 0;
	DropVariable("$RESULT_1");
	DropVariable("$RESULT_2");

	string proc, lib;
	FARPROC p;

	if(GetSTROpValue(ops[0], proc) && GetSTROpValue(ops[1], lib))
	{
		HMODULE hMod=LoadLibraryEx(lib.c_str(),NULL,LOAD_WITH_ALTERED_SEARCH_PATH);

		if(hMod==0)	{
			variables["$RESULT"] = 0;
			errorstr = "GPA: "+StrLastError();

			if (dontfree) {

				nIgnoreNextValuesHist++;
				DoGPA("\"LoadLibraryA\",\"kernel32.dll\"");

				HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);

				ulong pmem = AddProcessMemoryBloc(lib);

				string tmp;
				sprintf(buffer,"%09X",pmem);
				tmp.assign(buffer);

				if (DoPUSH(tmp)) {

					sprintf(buffer,"%09X",variables["$RESULT"].dw);

					ExecuteASM("call "+tmp);
				
				}

				regBlockToFree((void*)pmem,0x1000,true);
				//VirtualFreeEx(hDebugee, pmem, 0x1000, MEM_DECOMMIT);

			}
			return true;
		}

		variables["$RESULT_1"] = lib;

		//if (proc != "")
		p = GetProcAddress(hMod, proc.c_str());
		//else
		//:	GetModu

		if (!dontfree) {
			FreeLibrary(hMod);
			require_ollyloop=1;
		}

		if(p == 0) {
			errorstr = "GPA: No such procedure: " + proc;
			return true;
		}

		variables["$RESULT"] = (DWORD) p;
		variables["$RESULT_2"] = proc;
		return true;
	}
	return false;
}

bool OllyLang::DoGPI(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	string str;

	if(GetSTROpValue("\""+ops[0]+"\"", str))
	{
 
		if(str == "HPROCESS") //(HANDLE) Handle of debugged process 
		{
			variables["$RESULT"] = (DWORD)Plugingetvalue(VAL_HPROCESS);
			return true;
		}
		else if(str == "PROCESSID") //Process ID of debugged process 
		{
			variables["$RESULT"] = (DWORD)Plugingetvalue(VAL_PROCESSID);
			return true;
		}
		else if(str == "HMAINTHREAD") //(HANDLE) Handle of main thread of debugged process 
		{
			variables["$RESULT"] = (DWORD)Plugingetvalue(VAL_HMAINTHREAD);
			return true;
		}
		else if(str == "MAINTHREADID") //Thread ID of main thread of debugged process 
		{
			variables["$RESULT"] = (DWORD)Plugingetvalue(VAL_MAINTHREADID);
			return true;
		}
		else if(str == "MAINBASE") //Base of main module in the debugged process 
		{
			variables["$RESULT"] = (DWORD)Plugingetvalue(VAL_MAINBASE);
			return true;
		}
		else if(str == "PROCESSNAME") //Name of the debugged process 
		{
			variables["$RESULT"] = (char *)Plugingetvalue(VAL_PROCESSNAME);
			return true;
		}
		else if(str == "EXEFILENAME") //Name of the main debugged file 
		{
			variables["$RESULT"] = (char *)Plugingetvalue(VAL_EXEFILENAME);
			return true;
		}
		else if(str == "CURRENTDIR") //Current directory for debugged process 
		{
			variables["$RESULT"] = (char *)Plugingetvalue(VAL_CURRENTDIR);
			if (variables["$RESULT"].str.length()==0) {
				char spath[MAX_PATH];
				strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
				string path = spath;
				variables["$RESULT"] = path.substr(0, path.rfind('\\') + 1);				
			}
			return true;
		}
		else if(str == "SYSTEMDIR") //Windows system directory 
		{
			variables["$RESULT"] = (char *)Plugingetvalue(VAL_SYSTEMDIR);
			return true;
		}
	}
	errorstr = "Bad operand";
	return false;
}

//in dev... i try to find API parameters count and types
bool OllyLang::DoGPP(string args)
{
	if (!DoGPA(args))
		return false;

	ulong addr = variables["$RESULT"].dw;
	if (addr==0)
		return false;

	string sAddr = itoa(addr,buffer,16);
	if (!DoREF(sAddr))
		return false;

	int t=Plugingetvalue(VAL_REFERENCES);
	if (t<=0)
		return false;

	int size;
	t_table* tref=(t_table*) t;
	for (int n=0;n<tref->data.n;n++) {
	
		t_ref* ref= (t_ref*) Getsortedbyselection(&tref->data,n);
			
		if (ref->addr == addr)
			continue; 

		//Disasm origin to get comments
		BYTE buffer[MAXCMDSIZE];
		size=Readmemory(buffer, ref->addr, MAXCMDSIZE, MM_SILENT);					
		if (size>0) {

			t_disasm disasm;
			t_module* mod = Findmodule(ref->addr);
			Analysecode(mod);

			size=Disasm(buffer,size,ref->addr,NULL,&disasm,DISASM_ALL,NULL);
            DbgMsg(disasm.nregstack,disasm.comment);

			if (size>0) {
				variables["$RESULT"] = ref->addr;
				variables["$RESULT_1"] = disasm.result; //command text
				variables["$RESULT_2"] = disasm.comment;
				return true; 
			}
		}
	}
	return true;
}

//Get Reference Window Address at Offset
bool OllyLang::DoGREF(string args)
{
	string ops[1];
	ulong line;

	CreateOperands(args, ops, 1);

	t_table* tt;
	tt=(t_table*) Plugingetvalue(VAL_REFERENCES);

	variables["$RESULT"] = 0;

	//Get Ref. Addr
	if (tt!=NULL) 
	{
		if(ops[0]!="" && GetDWOpValue(ops[0], line))
		{
			if (line < tt->data.n) 
			{
				t_ref* tr;
				tr=(t_ref*) Getsortedbyselection(&tt->data, line); //0 is CPU initial sel.
				if (tr!=NULL)
					variables["$RESULT"]=tr->addr;
			}
		} else {
			//Get Ref. Count
			variables["$RESULT"] = tt->data.n-1;
		}
		return true;
	}
	return false;
}

//Get Relative Offset
bool OllyLang::DoGRO(string args)
{
	string ops[1];
	ulong addr;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr))
	{
		char sym[4096] = {0};

		int size = Decoderelativeoffset(addr, ADC_NONTRIVIAL, sym, 4096);
		if (size > 0)
		{
			variables["$RESULT"] = sym;
		}
		else
		{
			variables["$RESULT"] = 0;
		}

		return true;
	}

	return false;
}

// Get Selection Limits
// returns START/END address from currently selected line in CPUASM | DUMP | STACK window in $RESULT & $RESULT_1
// arg can be either : CPUDASM, CPUDUMP, CPUSTACK 
// ex		:	gsl CPUDUMP
bool OllyLang::DoGSL ( string args )
{
	string str;
	t_dump* td;

	variables["$RESULT"] = 0;
	variables["$RESULT_1"] = 0;
	variables["$RESULT_2"] = 0;
	
	GetSTROpValue("\""+args+"\"", str);
	if (str == "") str = "CPUDASM";

	transform(str.begin(), str.end(), str.begin(), toupper);

	if(str == "CPUDASM")
		td = (t_dump*) Plugingetvalue (VAL_CPUDASM);
	else if (str == "CPUDUMP")
		td = (t_dump*) Plugingetvalue(VAL_CPUDDUMP);
	else if (str == "CPUSTACK")
		td = (t_dump*) Plugingetvalue(VAL_CPUDSTACK);
	else
		return false;
	
	if (td)
	{
		variables["$RESULT"] = (DWORD)td->sel0;
		variables["$RESULT_1"] =  (DWORD)td->sel1-1;
		variables["$RESULT_2"] =  (DWORD)td->sel1-(DWORD)td->sel0;
	}
	else
	{
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0;
		variables["$RESULT_2"] = 0;
	}

	return true;
}

// Get String
// returns a null terminated string from addr, the string is at least arg1 charachters
// gstr addr, [arg1]
// returns in   :
// - $RESULT    : the string
// - $RESULT_1  : len of string
//
// ex       : gstr 401000     ; arg1 in this case is set to default (5 chars)
//          : gstr 401000, 20 ; must be at least 20 chars
bool OllyLang::DoGSTR(string args)
{
	string str;
	char buf[TEXTLEN]={0};
	ulong addr, size, tmpSize;
	char c;
	bool bUseDef = false;
	uint iDashNum;

	string ops[2];


	if (!CreateOperands(args, ops, 2))
	{
		if (!CreateOperands(args, ops, 1))
			return false;
		else
			bUseDef = true;
	}

	if (GetDWOpValue (ops[0], addr))
	{
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0;
		if (addr != 0)
		{
			buffer[0] = '\0';
			tmpSize = Decodeascii(addr, buf, TEXTLEN, DASC_ASCII);
			if (tmpSize > 2 && buf[0]=='"') {
				tmpSize -= 2;
				lstrcpyn(buffer, (char *)(buf+1), tmpSize+1);
			}
			else {
				return true;
			}

			//tmpSize = Readmemory(buf, addr, TEXTLEN, MM_RESILENT);
			//if (!tmpSize)
			//	return true;

			//lstrcpy(buffer, buf);
			tmpSize = lstrlen(buffer);

			if (!bUseDef)
				GetDWOpValue(ops[1], size);
			else
				size = 2;

			if (tmpSize < size)
				return true;
/*
			int i;
			for (iDashNum=0,i=0; i < tmpSize; i++)
			{
				c = buffer[i];
				if (isalnum(c))
					continue;
				else
				{
					buffer[i] = '_';
					iDashNum++;
				}
			}

			if (iDashNum >= tmpSize / 2)
				return true;
*/
			str.assign(buffer);
			variables["$RESULT"] = str;
			variables["$RESULT_1"] = tmpSize;			
		}
		return true;
	}
	return false;
}

// Get String
// returns a null terminated string from addr, the string is at least arg1 charachters
// gstr addr, [arg1]
// returns in   :
// - $RESULT    : the string
// - $RESULT_1  : len of string
//
// ex       : gstr 401000     ; arg1 in this case is set to default (5 chars)
//          : gstr 401000, 20 ; must be at least 20 chars
bool OllyLang::DoGSTRW(string args)
{
	string str,wstr;
	char buf[TEXTLEN*2]={0};
	ulong addr, size, tmpSize;
	char c;
	bool bUseDef = false;
	uint iDashNum;

	string ops[2];


	if (!CreateOperands(args, ops, 2))
	{
		if (!CreateOperands(args, ops, 1))
			return false;
		else
			bUseDef = true;
	}

	if (GetDWOpValue (ops[0], addr))
	{
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0;
		if (addr != 0)
		{
			buffer[0] = '\0';
			tmpSize = Decodeunicode(addr, buf, TEXTLEN*2);
			if (tmpSize > 2 && buf[0]=='"') {
				tmpSize -= 2;
				lstrcpyn(buffer, (char *)(buf+1), tmpSize+1);				
			}
			else {
				return true;
			}

			str.assign(buffer);
			tmpSize = str.length();//lstrlen(buffer);

			if (tmpSize < Readmemory(buf, addr, TEXTLEN*2, MM_RESILENT)) {
				wcsncpy((wchar_t *)buffer, (wchar_t *) buf, tmpSize);
				wstr.assign(buffer,tmpSize*2);
			}

			if (!bUseDef)
				GetDWOpValue(ops[1], size);
			else
				size = 2;

			if (tmpSize < size)
				return true;
/*
			int i;
			for (iDashNum=0,i=0; i < tmpSize; i++)
			{
				c = buffer[i];
				if (isalnum(c))
					continue;
				else
				{
					buffer[i] = '_';
					iDashNum++;
				}
			}

			if (iDashNum >= tmpSize / 2)
				return true;
*/
			
			variables["$RESULT"] = str;
			variables["$RESULT_1"] = tmpSize;
			variables["$RESULT_2"] = wstr;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoHANDLE(string args)
{
	string ops[3];
	bool useCaption=true;

	if(!CreateOperands(args, ops, 3)) {
			return false;
	}

	string sClassName;
	ulong x,y;
	ulong thid = Plugingetvalue(VAL_MAINTHREADID);

	if(GetDWOpValue(ops[0], x) 
		&& GetDWOpValue(ops[1], y) 
		&& GetSTROpValue(ops[2], sClassName)) 
	{
		variables["$RESULT"] = (DWORD) FindHandle(thid, sClassName, x, y);
		return true;
	}

	return false;

}

bool OllyLang::DoINC(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	return DoADD(ops[0] + ", 1");
}

bool OllyLang::DoHISTORY(string args)
{
	string ops[1];
	ulong dw;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], dw)) {
		showVarHistory=(dw!=0);
		return true;
	}

	return false;
}

bool OllyLang::DoITOA(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2)) {
		ops[1] = "16.";
		if(!CreateOperands(args, ops, 1))
			return false;
	}

	ulong base, dw;

	if(GetDWOpValue(ops[0], dw) 
		&& GetDWOpValue(ops[1], base) )
	{
		char buffer [20]={0};
		itoa(dw,buffer,base);
		variables["$RESULT"] = buffer;
		return true;
	}
	return false;
}

bool OllyLang::DoJA(string args)
{
	if(zf == 0 && cf == 0)
		return DoJMP(args);
	return true;
}

bool OllyLang::DoJAE(string args)
{
	if(cf == 0)
		return DoJMP(args);
	return true;
}

bool OllyLang::DoJB(string args)
{
	if(cf == 1)
		return DoJMP(args);
	return true;
}

bool OllyLang::DoJBE(string args)
{
	if(zf == 1 || cf == 1)
		return DoJMP(args);
	return true;
}

bool OllyLang::DoJE(string args)
{
	if(zf == 1)
		return DoJMP(args);
	return true;
}

bool OllyLang::DoJMP(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(labels.find(ops[0]) == labels.end())
		return false;

	//Store jump destination to display it
	t_wndprog_data *pline;
	pline = (t_wndprog_data *) Getsortedbyselection(&wndProg.data,pgr_scriptpos);
	if (pline != NULL) {
		pline->jumpto = labels[ops[0]];
	}

	script_pos = labels[ops[0]];
	return true;
}

bool OllyLang::DoJNE(string args)
{
	if(zf == 0)
		return DoJMP(args);
	return true;
}


bool OllyLang::DoKEY(string args)
{
	string ops[3];
	ulong key,shift,ctrl;

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="0";
		if(!CreateOperands(args, ops, 2)) {
			ops[1]="0";
			if(!CreateOperands(args, ops, 1))
				return false;
		}
	}
	
	if(GetDWOpValue(ops[0], key) 
		&& GetDWOpValue(ops[1], shift) 
		&& GetDWOpValue(ops[2], ctrl)) 
	{
		Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, ctrl, shift, key); 
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoLBL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	string lbl;
	ulong addr;

	if(GetDWOpValue(ops[0], addr) 
		&& GetSTROpValue(ops[1], lbl))
	{
		if(lbl != "")
		{
			strcpy(buffer, lbl.c_str());
			Insertname(addr, NM_LABEL, buffer);
			Broadcast(WM_USER_CHALL, 0, 0);
		}
		else
		{
			Deletenamerange(addr, addr + 1, NM_COMMENT);
			Broadcast(WM_USER_CHALL, 0, 0);
		}
		return true;
	}
	return false;
}

bool OllyLang::DoLM(string args)
{
    string ops[3];
    if(!CreateOperands(args, ops, 3))
        return false;

    streamsize sum = 0;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);

    ulong addr, size;
    string filename;
    if(GetDWOpValue(ops[0], addr) 
		&& GetDWOpValue(ops[1], size) 
		&& GetSTROpValue(ops[2], filename))
    {

        if (filename.find(":\\") != string::npos)
			path = filename;
		else
			path += filename;

		std::ifstream fin(path.c_str(),ios::in | ios::binary);

		if(fin.fail()) {
	        variables["$RESULT"] = 0;
            errorstr = "Couldn't open file!";
			fin.close();
            return false;
		}

        char buf[4096];
		int gotten;
		while (!fin.eof() && (sum<size || size==0) )
        {
			if (size==0) 
				fin.read(buf, sizeof buf);			
			else
				fin.read(buf, min(size-sum, sizeof buf));
			gotten=fin.gcount();
			sum += Writememory(buf, addr+sum, gotten, MM_RESILENT);
        }
        fin.close();

		if (sum) {
	        variables["$RESULT"] = sum;
            return true;
        }
        else
        {
            errorstr = "Couldn't load file!";
            return false;
        }
    }
    return false;
}

bool OllyLang::DoLC(string args)
{
	//Clear Main Log Window

	HWND hwndLog=Createlistwindow();
	if (hwndLog==0)
		return false;

	//open context menu to clear main log wdw
	PostMessage(hwndLog,WM_USER+101,17,0);
	PostMessage(hwndLog,WM_KEYDOWN,'C',0);

	return true;
}

bool OllyLang::DoLCLR(string args)
{
	clearLogLines();
	return true;
}

bool OllyLang::DoLEN(string args)
{
	string ops[1],str;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(!GetSTROpValue(ops[0], str))
		return false;

	variables["$RESULT"] = (int) str.length();
	return true;
}

bool OllyLang::DoLOADLIB(string args)
{
	string ops[1],str;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(!GetSTROpValue(ops[0], str))
		return false;

	ulong fnload;

	SaveRegisters(true);
	variables["$RESULT"] = 0;

	DoGPA("\"LoadLibraryA\",\"kernel32.dll\"");
	fnload = variables["$RESULT"].dw;

	//alloc memory bloc to store DLL name
	HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);
	void * hMem = VirtualAllocEx(hDbgPrc,NULL,0x1000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);

	char bfdlladdr[10]={0};
	sprintf(bfdlladdr, "%09lX", hMem);
	
	Writememory((void*)str.c_str(), (ulong) hMem, str.length(), MM_DELANAL|MM_SILENT);

	if (DoPUSH(bfdlladdr)) {

		char bffnloadlib[10]={0};
		sprintf(bffnloadlib,"%09X",fnload);
		string libPtrToLoad = bffnloadlib;

		ExecuteASM("call "+libPtrToLoad);	

		variables["$RESULT"] = 0;

		// result returned after process
		// variables["$RESULT"] = pt->reg.r[REG_EAX];
		t_dbgmemblock block={0};
		block.hmem = hMem;
		block.size = 0x1000;
		block.script_pos = script_pos;
		block.free_at_eip = reg_backup.eip;
		block.result_register = true;
		block.reg_to_return = REG_EAX;
		block.restore_registers = true;
		block.listmemory = true;
		block.autoclean = true;

		// Free memory block after next ollyloop
		regBlockToFree(block);
		require_addonaction = 1;
		require_ollyloop = 1;
		
		return true;
	}
	

	return false;
}

bool OllyLang::DoLOG(string args)
{
	string prefix, ops[2];

	if(CreateOp(args, ops, 2))
	{
		if (!GetSTROpValue(ops[1], prefix))
			return false;
	} 
	else 
	{
		prefix="DEFAULT";
		if(!CreateOperands(args, ops, 1))
			return false;
	}

	ulong dw=0;
	string str;
	long double flt=0;

	if(UnquoteString(ops[0], '"', '"'))
	{
		//string constant
		if (prefix.compare("DEFAULT") == 0) 
		{
			strncpy(buffer, ops[0].c_str(),TEXTLEN-1);
		} 
		else 
		{
			prefix = prefix + ops[0];
			strncpy(buffer, prefix.c_str(),TEXTLEN-1);
		}
		buffer[TEXTLEN-1]=0;
		Infoline(buffer);
		Addtolist(0, 1, buffer);
		add2log(buffer);
		return true;
	}
	else 
	{
		vtype vt=EMP;
		if (is_variable(ops[0]))
			vt = variables[ops[0]].vt;

		if (vt==DW || vt==EMP) {
			if(GetDWOpValue(ops[0], dw))
			{
				//ulong constant
				if (prefix.compare("DEFAULT") == 0) {

					char sym[4096] = {0};
					char buf[TEXTLEN-1] = {0};
					int res = Decodeaddress(dw, 0, ADC_JUMP | ADC_STRING | ADC_ENTRY | ADC_OFFSET | ADC_SYMBOL, sym, 4096, buf);
					sprintf(buffer, "%s: %08lX", ops[0].c_str(), dw);

					if(strcmp(buf, ""))
						sprintf(buffer, "%s | %s", buffer, buf);

					if(strcmp(sym, "") && dw !=  strtoul(sym, 0, 16))
						sprintf(buffer, "%s | %s", buffer, sym);
				
				} 
				else 
				{
					sprintf(buffer, "%s%08lX", prefix.c_str(), dw);
				}

				buffer[TEXTLEN-1]=0;
				str.assign(buffer);
				//Infoline and Addtolist need parameters if %s %d... is present in string
				if (str.find("%") == string::npos) {
					Infoline(buffer);
					Addtolist(0, 1, buffer);
				}
				add2log(buffer);
				return true;
			}
		}

		if (vt==FLT || vt==EMP) {
			if(GetFLTOpValue(ops[0], flt))
			{
				//ulong constant
				if (prefix.compare("DEFAULT") == 0) {
					sprintf(buffer, "%s: %2lf", ops[0].c_str(), flt);				
				} else {
					sprintf(buffer, "%s%2lf", prefix.c_str(), flt);
				}

				buffer[TEXTLEN-1]=0;
				Infoline(buffer);
				Addtolist(0, 1, buffer);
				add2log(buffer);
				return true;
			}
		}

		if (vt==STR || vt==EMP) {		
			if(GetSTROpValue(ops[0], str))
			{
				var v=str;

				if(v.isbuf) {

					//log a buffer
					if (prefix.compare("DEFAULT") == 0) 
					{
						if (str.length()+ops[0].length() < 4094)
							sprintf(buffer, "%s: %s", ops[0].c_str(), str.c_str());
						else
							strncpy(buffer, variables[ops[0]].str.c_str(), TEXTLEN-1);
					} 
					else 
					{
						prefix = prefix + str;
						strncpy(buffer, prefix.c_str(), TEXTLEN-1);
					}
				} 
				else 
				{

					//log a string
					if (prefix.compare("DEFAULT") == 0) 
					{
						if (variables[ops[0]].str.length()+ops[0].length() < 4094)
							sprintf(buffer, "%s: %s", ops[0].c_str(), variables[ops[0]].strclean().c_str());
						else
							strncpy(buffer, variables[ops[0]].strclean().c_str(), TEXTLEN-1);
					} 
					else 
					{
						prefix = prefix + CleanString(str);
						strncpy(buffer, prefix.c_str(), TEXTLEN-1);
					}
				}
				buffer[TEXTLEN-1]=0;
				str.assign(buffer);
				//Infoline and Addtolist need parameters if %s %d... is present in string
				if (str.find("%") == string::npos) {
					Addtolist(0, 1, buffer);
				}
				add2log(str);
				return true;
			}
		}
	}
	return false;
}

bool OllyLang::DoLOGBUF(string args)
{
	string sSep,ops[3];

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="\" \"";
		if(!CreateOperands(args, ops, 2)) {
			ops[0]=args;
			ops[1]="0";
		}
	}

	GetSTROpValue(ops[2], sSep);

	if (!is_variable(ops[0]))
		return false;
	
	ulong dw;
	if(GetDWOpValue(ops[1], dw))
	{
		if (dw==0) dw=16;

		string sLine="";
		string data = variables[ops[0]].strbuffhex();
		for (int n=0; n < variables[ops[0]].size; n++)
		{
			sLine=sLine+data.substr(n*2,2)+sSep;
			if (n>0 && !((n+1) % dw)) { 
				DoLOG("\""+sLine+"\",\"\"");
				sLine="";
			}
		}
		if (sLine!="") DoLOG("\""+sLine+"\",\"\"");
		return true;
	}
	return false;
}

bool OllyLang::DoMEMCOPY(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3))
		return false;

	args="["+ops[0]+"],["+ops[1]+"],"+ops[2];

	return DoMOV(args);
}

bool OllyLang::DoMOV(string args)
{
	string ops[3];
	bool bDeclared=false;
	ulong addr=0,maxsize=0;

	if(CreateOperands(args, ops, 3)){
		GetDWOpValue(ops[2], maxsize);
	}
	else
		if(!CreateOperands(args, ops, 2))
			return false;

	//resolve address with operands
	if(UnquoteString(ops[1], '[', ']')) {
		if (!CreateOperands(ops[1],&ops[1],1))
			return false;
		ops[1]="["+ops[1]+"]";
	}

	// Check source
	ulong dw = 0; addr=0;
	string str = "";
	string tmpops=ops[0];
	long double flt;

	// Used to retry after automatic variable declaration 
	retry_DoMOV:

	// Check destination
	if(is_variable(ops[0]))
	{
		// Dest is variable
		if(maxsize <= 4 && variables[ops[0]].vt != STR && GetDWOpValue(ops[1], dw) )
		{
			// DW to DW/FLT var
			if (maxsize==0) maxsize=4;
			dw = resizeDW(dw,maxsize);
			variables[ops[0]] = dw;
			variables[ops[0]].size = maxsize;
		}
		else if(GetSTROpValue(ops[1], str, maxsize))
		{
			// STR to any var
			variables[ops[0]] = str;
		}
		else if(maxsize <= 4 && GetDWOpValue(ops[1], dw) )
		{
			// DW to STR var
			if (maxsize==0) maxsize=4;
			dw = resizeDW(dw,maxsize);
			variables[ops[0]] = dw;
			variables[ops[0]].size = maxsize;
		}
		else if(GetFLTOpValue(ops[1], flt))
		{
			variables[ops[0]] = flt;
		}
		else 
			return false;

		return true;
	}
	else if(is_register(ops[0]))
	{
		// Dest is register
		if(GetDWOpValue(ops[1], dw))
		{
			t_thread* pt = Findthread(Getcputhreadid());
			int regnr = GetRegNr(ops[0]);			
			if(regnr != -1) {
				if (ops[0].length()==2) 
				{
					if (ops[0][1] == 'l') {
						//R8
						dw &= 0xFF;
						pt->reg.r[regnr] &= 0xFFFFFF00;
						pt->reg.r[regnr] |= dw;
					} 
					else if ( ops[0][1] == 'h') 
					{
						//R8
						dw &= 0xFF;
						pt->reg.r[regnr] &= 0xFFFF00FF;
						pt->reg.r[regnr] |= dw * 0x100;
					} 
					else
					{
						//R16
						dw &= 0xFFFF;
						pt->reg.r[regnr] &= 0xFFFF0000;
						pt->reg.r[regnr] |= dw;
					}
				} 
				else
				{
						//R32
						pt->reg.r[regnr] = dw;
				}
			}
			else
				if(ops[0] == "eip") {
					pt->reg.ip = dw;
					//synch disasm window position
					Setdisasm(dw,1,CPU_ASMHIST);
				}

			pt->reg.modified = 1;
			pt->regvalid = 1;
			Broadcast(WM_USER_CHREG, 0, 0);
			require_ollyloop = 1;
			dw = resizeDW(dw,maxsize);
			return true;
		}
		return false;
	}
	else if(is_flag(ops[0]))
	{
		// Dest is flag
		if(GetDWOpValue(ops[1], dw))
		{
			if(dw != 0 && dw != 1)
			{
				errorstr = "Invalid flag value";
				return false;
			}

			eflags flags;
			ZeroMemory(&flags, sizeof DWORD);
			t_thread* pt = Findthread(Getcputhreadid());
			flags.dwFlags = pt->reg.flags;

			if(stricmp(ops[0].c_str(), "!af") == 0)
				flags.bitFlags.AF = dw;
			else if(stricmp(ops[0].c_str(), "!cf") == 0)
				flags.bitFlags.CF = dw;
			else if(stricmp(ops[0].c_str(), "!df") == 0)
				flags.bitFlags.DF = dw;
			else if(stricmp(ops[0].c_str(), "!of") == 0)
				flags.bitFlags.OF = dw;
			else if(stricmp(ops[0].c_str(), "!pf") == 0)
				flags.bitFlags.PF = dw;
			else if(stricmp(ops[0].c_str(), "!sf") == 0)
				flags.bitFlags.SF = dw;
			else if(stricmp(ops[0].c_str(), "!zf") == 0)
				flags.bitFlags.ZF = dw;

			pt->reg.flags = flags.dwFlags;
			pt->reg.modified = 1;
			pt->regvalid = 1;
			Broadcast(WM_USER_CHREG, 0, 0);
			require_ollyloop = 1;
			return true;
		}
		return false;
	}
	else if(is_floatreg(ops[0]))
	{
		// Dest is float register
		if(GetFLTOpValue(ops[1], flt))
		{
			t_thread* pt = Findthread(Getcputhreadid());
			pt->reg.f[(ops[0][3]-0x30)] = flt;
			pt->reg.modified = 1;
			pt->regvalid = 1;
			Broadcast(WM_USER_CHREG, 0, 0);
			require_ollyloop = 1;
			return true;
		}
		return false;
	}
	else if(UnquoteString(ops[0], '[', ']'))
	{
		// Destination is memory address
		// Get Address from Operators (+_*...)
		nIgnoreNextValuesHist++;
		CreateOperands(ops[0], &ops[0], 1);

		if(GetDWOpValue(ops[0], addr))
		{ 
			if (addr==0)
			{
				DoLOG("\"WARNING: writing to address 0 !\"");
				return true;
			}

			tmpops=ops[1];
			if (maxsize > 8 && UnquoteString(ops[1], '[', ']'))
			{
				//Get Addr from Operators
				CreateOperands(ops[1], &ops[1], 1);

				//Optimized Mem Copy
				ulong src;
				if (!GetDWOpValue(ops[1], src) || src==0) {
					DoLOG("\"WARNING: copy from address 0 !\"");
					return true;
				}
				char* copybuffer= new char[maxsize];
				if (maxsize != Readmemory((void*) copybuffer, src, maxsize, MM_RESTORE)) {
					delete[] copybuffer;
					return false;
				}
				Writememory((void*) copybuffer, addr, maxsize, MM_DELANAL);
				delete[] copybuffer;
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if (GetDWOpValue(ops[1], dw) && maxsize <= 4)
			{
				if (maxsize==0) maxsize=4;
				dw = resizeDW(dw,maxsize);
				Writememory(&dw, addr, maxsize, MM_DELANAL|MM_SILENT);
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if (GetSTROpValue(ops[1], str, maxsize))
			{
				var v=str;
				if (maxsize==0) maxsize=v.size;
				maxsize=min(maxsize,v.size);
				Writememory((void*)v.strbuff().c_str(), addr, maxsize, MM_DELANAL|MM_SILENT);			
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if (GetFLTOpValue(ops[1], flt))
			{
				Writememory(&flt, addr, 8, MM_DELANAL|MM_SILENT);				
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else
			{
				errorstr = "Bad operator \"" + ops[1] + "\"";
				return false;
			}
			Broadcast(WM_USER_CHMEM, 0, 0);
			return true;
		}
		return false;

	} 
	else if (!bDeclared && ops[0][0] >= 'A' ) 
	{

		bDeclared=true;
		DoVAR(ops[0]);
		//DoLOG("\"automatic declaration of variable "+ops[0]+"\"");
		goto retry_DoMOV;

	}

	//errorstr = "Variable '" + ops[0] + "' is not declared";
	return false;
}

bool OllyLang::DoMSG(string args)
{
	string ops[1];

	if(!CreateOp(args, ops, 1))
		return false;

	string msg;
	if(GetANYOpValue(ops[0], msg))
	{
		if (wndProg.hw!=NULL)
			InvalidateRect(wndProg.hw, NULL, FALSE);
		
		//hwndOllyDbg() or 0: modal or not
		int ret = MessageBox(0, msg.c_str(), "MSG ODbgScript", MB_ICONINFORMATION | MB_OKCANCEL | MB_TOPMOST | MB_SETFOREGROUND);
		FocusProgWindow();
		if(ret == IDCANCEL) {
			return Pause();
		} 
		return true;
	}
	return false;
}

bool OllyLang::DoMSGYN(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	string msg;
	if(GetSTROpValue(ops[0], msg))
	{
		if (wndProg.hw!=NULL)
			InvalidateRect(wndProg.hw, NULL, FALSE);

		int ret = MessageBox(0, msg.c_str(), "MSG ODbgScript", MB_ICONQUESTION | MB_YESNOCANCEL | MB_TOPMOST | MB_SETFOREGROUND);
		FocusProgWindow();
		if(ret == IDCANCEL)
		{
			variables["$RESULT"] = 2;
			return Pause();
		}
		else if(ret == IDYES)
			variables["$RESULT"] = 1;
		else
			variables["$RESULT"] = 0;

		return true;
	}
	return false;
}

bool OllyLang::DoOLLY(string args)
{
    string ops[1], param;
	ulong value;

	if(!CreateOp(args, ops, 1))
		return false;

	if ( GetSTROpValue("\""+ops[0]+"\"", param) )
	{

		transform(param.begin(), param.end(), param.begin(), toupper);

		if (param == "PID")
		{
			value = GetCurrentProcessId();
			variables["$RESULT"] = value; 
			return true;
		}
		else if (param == "HWND")
		{
			value = (ulong) hwndOllyDbg();
			variables["$RESULT"] = value;
			return true;
		}
		

	}
	return false;
}


bool OllyLang::DoOR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 | dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoMUL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 * dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoNEG(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	ulong dw1;
	if(GetDWOpValue(ops[0], dw1))
	{
		__asm
		{
			push eax
			mov eax,dw1
			neg eax
			mov dw1,eax
			pop eax
		}
		args = ops[0] + ", " +ultoa(dw1, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}


bool OllyLang::DoNOT(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	ulong dw1;
	if(GetDWOpValue(ops[0], dw1))
	{
		__asm
		{
			push eax
			mov eax,dw1
			not eax
			mov dw1,eax
			pop eax
		}
		args = ops[0] + ", " +ultoa(dw1, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

//see also GCI
bool OllyLang::DoOPCODE(string args)
{
	string ops[1];
	if(!CreateOperands(args, ops, 1))
		return false;

	ulong addr,size;
	if(GetDWOpValue(ops[0], addr))
	{
		BYTE buffer[MAXCMDSIZE];
//		size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
		size=Readcommand(addr,(char *) buffer);
		
		if (size>0) 
		{
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_CODE,NULL);

			if (size>0) 
			{
				variables["$RESULT"] = disasm.dump;     //command bytes
				variables["$RESULT_1"] = disasm.result; //asm command text
				variables["$RESULT_2"] = size;
				return true;
			}
		}
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0;
		variables["$RESULT_2"] = 0;
		return true;

	}
	return false;
}

bool OllyLang::DoOPENDUMP(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3))
	if(!CreateOperands(args, ops, 2))
	if(!CreateOperands(args, ops, 1))
		return false;

	ulong addr, base, size;
	if(!GetDWOpValue(ops[0], addr))
		return false;
	if(!GetDWOpValue(ops[1], base))
		return false;
	if(!GetDWOpValue(ops[2], size))
		return false;

	variables["$RESULT"] = 0;

	if (addr==0)
		return true; //do nothing

	t_memory* mem = Findmemory(addr);

	if (base==0) base=mem->base;
	if (size==0) size=mem->size;
	
	HWND wndDump = Createdumpwindow(NULL,base,size,addr,0x01081,NULL);

//dont work: need to ask Olleh to have a function 
//	t_dump * dump = (t_dump *) &wndDump;

	//So i created a special key message to grab it
	if (wndDump!=NULL) {
		ulong res = SendMessage(wndDump,WM_KEYDOWN,VK_F5,0);
		//can now grab (t_dump *) dumpWindows[wndDump];
		variables["$RESULT"] = (ulong) wndDump;		
	}
	return true;

	return false;
}

bool OllyLang::DoOPENTRACE(string args)
{
	ulong threadid = Getcputhreadid();
	if(threadid == 0)
		threadid = Plugingetvalue(VAL_MAINTHREADID);
	t_thread* pthr = Findthread(threadid);
	if(pthr != NULL)
		Startruntrace(&(pthr->reg)); 

	//Show Trace Window
	HWND wndTrace = Creatertracewindow();
	//SetWindowPos(wndTrace,HWND_TOP,0,0,0,0,SWP_NOSIZE+SWP_NOMOVE);

	return true;
}

bool OllyLang::DoPAUSE(string args)
{
	Pause();
	FocusProgWindow();
	return true;
}

bool OllyLang::DoPOP(string args)
{
	string ops[1];

	nIgnoreNextValuesHist++;
	var_logging=false;
	if(!CreateOperands(args, ops, 1))
		return false;

	args = ops[0] + ", [esp], 4";
	nIgnoreNextValuesHist+=2;
	DoMOV(args);

	args = "esp, esp+4";
	nIgnoreNextValuesHist+=1;
	return DoMOV(args);
}

bool OllyLang::DoPOPA(string args)
{
	bool result = RestoreRegisters(true);
	if (result)
		require_ollyloop=1;
	return result;
}

bool OllyLang::DoPREOP(string args)
{
	string ops[1];

	CreateOperands(args, ops, 1);
	if (ops[0]=="")
		ops[0]="eip";

	ulong addr,base;
	if(!GetDWOpValue(ops[0], addr))
		return false;

	t_memory* mem = Findmemory(addr);
	if(mem != NULL)	
	{
		variables["$RESULT"] = Disassembleback(NULL,mem->base,mem->size,addr,1,true);
		return true;
	} 
	else 
	{
		variables["$RESULT"] = 0;
		return true;
	}
	return false;
}

bool OllyLang::DoPUSH(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	ulong dw,dw1;

	//var_logging=false;
	if(GetDWOpValue(ops[0], dw1))
	{
		args = "esp, esp-4";
		nIgnoreNextValuesHist+=2;
		DoMOV(args);

		args = "[esp], " + ops[0];
		nIgnoreNextValuesHist++;
		DoMOV(args);

		t_thread* pt = Findthread(Getcputhreadid());	
     	dw1 = pt->reg.r[4];
		Readmemory(&dw, dw1, 4, MM_DELANAL|MM_SILENT);
		if (nIgnoreNextValuesHist==0)
			setProgLineValue(script_pos+1,dw);
		return true;
	}
	return false;
}

bool OllyLang::DoPUSHA(string args)
{
	return SaveRegisters(true);
}

bool OllyLang::DoREADSTR(string args)
{
	string ops[2],s;
    ulong maxsize=0;

	ulong dw = 0, addr = 0;
	string str = "";
	if(CreateOperands(args, ops, 2)) 
	{
		GetDWOpValue(ops[1], maxsize);
        GetSTROpValue(ops[0], str, maxsize);

        variables["$RESULT"] = str;
		return true;
	}
    return false;
}

// Restore Break Points
// restores all hardware and software breakpoints
// (if arg1 == 'STRICT', all soft bp set by script will be deleted and only those have been set before it runs
// will be restored
// if no argument set, previous soft bp will be appended to those set by script)

// rbp [arg1]
// arg1 = may be STRICT or nothing

// return in:
// - $RESULT number of restored swbp
// - $RESULT_1 number of restored hwbp

// ex     : rbp
//        : rbp STRICT
bool OllyLang::DoRBP ( string args )
{
	t_table* bpt = 0;
	t_bpoint* bpoint = 0;
	uint n,i=0;
	string ops[1];

	CreateOperands ( args, ops, 1 );
	
	variables["$RESULT"] = 0;

	if ( saved_bp )
	{
		bpt = ( t_table * ) Plugingetvalue ( VAL_BREAKPOINTS );
		if ( bpt != NULL )
		{
			bpoint = ( t_bpoint * ) bpt->data.data;

			if ( ops[0] == "STRICT" )
			{
				int dummy;
				dummy = bpt->data.n;
				for ( n = 0; n < dummy; n++ )
				{
					Deletebreakpoints ( bpoint->addr, ( bpoint->addr ) + 1, 1 );
				}
			}

			for ( n=0; n < sortedsoftbp_t.n; n++ )
				Setbreakpoint ( softbp_t[n].addr, ( softbp_t[n].type | TY_KEEPCODE ) ^ TY_KEEPCODE, 0 );

			variables["$RESULT"] = ( DWORD ) sortedsoftbp_t.n;

			Broadcast ( WM_USER_CHALL, 0, 0 );
		}

	}

	//Hardware Bps
	for ( n=0; n < 4; n++ ) {
		if (hwbp_t[n].addr) {
			Sethardwarebreakpoint ( hwbp_t[n].addr, hwbp_t[n].size, hwbp_t[n].type );
			i++;
		}
	}
	variables["$RESULT_1"] = ( DWORD ) i;

	return true;
}

bool OllyLang::DoREF(string args)
{
	string ops[2];
	if (!CreateOperands(args, ops, 2)){
		ops[1]="MEMORY"; 
		if(!CreateOperands(args, ops, 1))
		return false;
	}

	char title[256]="Reference to Command - ODbgScript REF";	
	ulong addr,size;
	string str;
	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) )
	{
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0; //command bytes
		variables["$RESULT_2"] = 0;

		if (adrREF!=addr) 
		{
			curREF=-1;

			//Reset REF
			if (addr == 0) {
				adrREF=0;
				return true;
			}

			//Info to get destination address
			BYTE buffer[MAXCMDSIZE];
//			size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
			size=Readcommand(addr,(char *) buffer);

			t_disasm disasm;
			size=Disasm(buffer, size, addr, NULL, &disasm, DISASM_SIZE, NULL);
			if (size==0)
				return true;

			transform(str.begin(), str.end(), str.begin(), toupper);

			//Search for references
			if(str == "MEMORY") // Compatibility (before v1.71) Search in the memory bloc
			{
				t_memory* mem = Findmemory(addr);

				if (Findreferences(mem->base,mem->size,addr,addr+size,addr,16,title) > 0)
					adrREF=addr;
				else
					return true;
			}
			else if(str == "CODE") // Search in the code part of module
			{
				t_module* mod = Findmodule(addr);

				if (Findreferences(mod->codebase,mod->codesize,addr,addr+size,addr,16,title) > 0)
					adrREF=addr;
				else
					return true;
			}
			else if(str == "MODULE") // Search in the whole module
			{
				t_module* mod = Findmodule(addr);

				if (Findreferences(mod->base,mod->size,addr,addr+size,addr,16,title) > 0)
					adrREF=addr;
				else
					return true;
			}
			else
				return false;

		}
		t_table* tref=(t_table*) Plugingetvalue(VAL_REFERENCES);
		
		if (tref != NULL) 
		{
			ignore_origin:
			curREF++;
			if (curREF > tref->data.n)
				adrREF=0;
			else 
			{
				t_ref* ref= (t_ref*) Getsortedbyselection(&tref->data,curREF);
				if (ref != NULL) 
				{

					if (ref->addr == addr)
						goto ignore_origin;

					//Disasm origin to get comments
					BYTE buffer[MAXCMDSIZE];
					size=Readmemory(buffer, ref->addr, MAXCMDSIZE, MM_SILENT);					
					if (size>0) 
					{
						t_disasm disasm;
						size=Disasm(buffer,size,ref->addr,NULL,&disasm,DISASM_ALL,NULL);

						if (size>0) 
						{
							variables["$RESULT"] = ref->addr;
							variables["$RESULT_1"] = disasm.result; //command text
							variables["$RESULT_2"] = disasm.comment;
							return true;
						}
					}
				}
			}			
		}
		return true;

	}
	return false;
}

bool OllyLang::DoREFRESH(string args)
{
	//Refresh Memory Map
	Listmemory();

	//Refresh Executable Modules (no other way found to do this)
	t_table* tt = (t_table*)Plugingetvalue(VAL_MODULES);
	if (tt->hw!=0) {
		//was visible
		Sendshortcut(PM_MAIN,0,WM_SYSKEYDOWN,0,0,'E');
	} else {
		//was hidden, hide it after
		Sendshortcut(PM_MAIN,0,WM_SYSKEYDOWN,0,0,'E');
		tt = (t_table*)Plugingetvalue(VAL_MODULES);
		if (tt->hw!=0)
			DestroyWindow(tt->hw);
	}

	//Refresh DISASM
	Redrawdisassembler();

	require_ollyloop=1;
	Broadcast(WM_USER_CHALL, 0, 0);
	return true;
}

bool OllyLang::DoREPL(string args)
{
	string ops[4];
	if(!CreateOperands(args, ops, 4))
		return false;

	if(ops[1].length() % 2 != 0 || ops[2].length() % 2 != 0)
	{
		errorstr = "Hex string must have an even number of characters!";
		return false;
	}

	ulong addr, len;
	if(GetDWOpValue(ops[0], addr) && UnquoteString(ops[1], '#', '#') && UnquoteString(ops[2], '#', '#') && GetDWOpValue(ops[3], len))
	{
		// Replace
		char *membuf = 0;
		t_memory* tmem = Findmemory(addr);
		membuf = new char[len];
		int memlen = Readmemory(membuf, addr, len, MM_RESILENT);
		bool replaced = false;

		try
		{
			int i = 0;
			while(i < len)
			{
				replaced = Replace(membuf + i, ops[1].c_str(), ops[2].c_str(), ops[2].length()) || replaced;
				i++;
			}
		}
		catch(char* str)
		{
			errorstr = errorstr.append(str);
			delete [] membuf;
			return false;
		}
		
		if(replaced)
		{
			Writememory(membuf, addr, len, MM_DELANAL | MM_SILENT);
			Broadcast(WM_USER_CHALL, 0, 0);
		}

		delete [] membuf;

		return true;
	}

	return false;
}

bool OllyLang::DoRESET(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F2); 
	return true;
}

bool OllyLang::DoRET(string args)
{
	if (calls.size() > 0) {
		script_pos = calls[calls.size()-1];
		calls.pop_back(); 
	} else {
		MessageBox(0, "Script finished", "ODbgScript", MB_ICONINFORMATION | MB_OK | MB_TOPMOST | MB_SETFOREGROUND);
		Reset();
		FocusProgWindow();
	}
	return true;
}

bool OllyLang::DoREV(string args)
{
	string ops[1];
	ulong dw;
//	BYTE b, tb[4];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(!GetDWOpValue(ops[0], dw))
		return false;

	dw=rev(dw);

/*	memcpy(&tb[0],&dw,4);
	dw=tb[3]; tb[3]=tb[0]; tb[0]=dw;
	dw=tb[2]; tb[2]=tb[1]; tb[1]=dw;
	memcpy(&dw,&tb[0],4);
*/
	variables["$RESULT"] = dw;
	return true;
}

bool OllyLang::DoROL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1;
	BYTE dw2;
	if(GetDWOpValue(ops[0], dw1) && GetBYTEOpValue(ops[1], dw2))
	{
        
		__asm
		{
			push eax
			push ecx
			mov eax,dw1
			mov cl,dw2
			rol eax,cl
			mov dw1,eax
			pop ecx
			pop eax
		}
		args = ops[0] + ", 0" + ultoa(dw1, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoROR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1;
	BYTE dw2;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetBYTEOpValue(ops[1], dw2))
	{
        
		__asm
		{
			push eax
			push ecx
			mov eax,dw1
			mov cl,dw2
			ror eax,cl
			mov dw1,eax
			pop ecx
			pop eax
		}
		args = ops[0] + ", 0" + ultoa(dw1, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}


bool OllyLang::DoRTR(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F9); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoRTU(string args)
{
	Sendshortcut(PM_MAIN,0,WM_SYSKEYDOWN,0,0,VK_F9);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoRUN(string args)
{
	Go(Getcputhreadid(), 0, STEP_RUN, 0, 1);
	require_ollyloop = 1;
	return true;
}

// Store Break Points
// stores all hardware and software breakpoints

// return in:
// - $RESULT number of restored swbp
// - $RESULT_1 number of restored hwbp

// ex 	: sbp
// 		: no argument
bool OllyLang::DoSBP ( string args )
{
	uint n = 0, i;
	bool success;
	t_table *bpt;
	t_bpoint *bpoint;

	variables["$RESULT"] = 0;
	variables["$RESULT_1"] = 0;

	bpt = ( t_table * ) Plugingetvalue ( VAL_BREAKPOINTS );
	if ( bpt != NULL )
	{
		bpoint = ( t_bpoint * ) ( bpt->data.data );
		if ( bpoint != NULL )
		{
			n = bpt->data.n;

			if ( n > alloc_bp )
			{
				//FreeBpMem();
				success = AllocSwbpMem ( n );
			}

			if ( n > alloc_bp && !success ) {
				errorstr = "Can't allocate enough memory to copy all breakpoints";
				return false;
			}
			else if (n > 0)
			{
				memcpy ( ( void* ) softbp_t, bpt->data.data, n*sizeof ( t_bpoint ) );
				memcpy ( ( void* ) &sortedsoftbp_t, ( void* ) &bpt->data, sizeof ( t_sorted ) );
				
			} 

			saved_bp = n;
			variables["$RESULT"] =  ( DWORD ) n;
		}
	}

	memcpy ( ( void* ) &hwbp_t, ( void* ) ( Plugingetvalue ( VAL_HINST ) +0xD8D70 ), 4 * sizeof ( t_hardbpoint ) );

	n = i = 0;
	while ( n < 4 )
	{
		if ( hwbp_t[n].addr )
			i++;
		n++;
	}
	variables["$RESULT_1"] =  ( DWORD ) i;

	return true;
}

bool OllyLang::DoSCMP(string args)
{
	string ops[3];
	ulong size=0;
	string s1, s2;

	if(!CreateOperands(args, ops, 3)) {
		if(!CreateOperands(args, ops, 2))
			return false;
	} else {
		GetDWOpValue(ops[2], size);
	}

	if(GetSTROpValue(ops[0], s1, size) 
		&& GetSTROpValue(ops[1], s2, size))
	{
		int res = s1.compare(s2);
		if(res == 0)	
		{
			zf = 1;
			cf = 0;
		}
		else if(res > 0)
		{
			zf = 0;
			cf = 0;
		}
		else //if(res < 0)
		{
			zf = 0;
			cf = 1;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoSCMPI(string args)
{
	string ops[3];
	ulong size=0;
	string s1, s2;

	if(!CreateOperands(args, ops, 3)) {
		if(!CreateOperands(args, ops, 2))
			return false;
	} else {
		GetDWOpValue(ops[2], size);
	}

	if(GetSTROpValue(ops[0], s1, size) 
		&& GetSTROpValue(ops[1], s2, size) )
	{
		int res = stricmp (s1.c_str(), s2.c_str());//s1.compare(s2,false);
		if(res == 0)		
		{
			zf = 1;
			cf = 0;
		}
		else if(res > 0)
		{
			zf = 0;
			cf = 0;
		}
		else //if(res < 0)
		{
			zf = 0;
			cf = 1;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoSETOPTION(string args)
{
    Sendshortcut(PM_MAIN,0,WM_SYSKEYDOWN,0,0,'O');
	require_ollyloop = 1;
    return true;
}

bool OllyLang::DoSHL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 << dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoSHR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 >> dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoSTI(string args)
{
	Go(Getcputhreadid(), 0, STEP_IN, 0, 1);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoSTO(string args)
{
	Go(Getcputhreadid(), 0, STEP_OVER, 0, 1);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoSTR(string args)
{
	string op[1];

	if(!CreateOp(args, op, 1))
		return false;

	if (is_variable(op[0])) {
		if (variables[op[0]].vt == STR) {

			if (variables[op[0]].isbuf)
				variables[op[0]] = variables[op[0]].strbuff();

			return true;

		} else if (variables[op[0]].vt == DW) {

			var v; v="##";
			v+=variables[op[0]].dw;
			variables[op[0]]=v.strbuff();

			return true;
		}
	}
	return false;
}

bool OllyLang::DoSUB(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 - dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoTC(string args)
{
	Deleteruntrace();
	return true;
}


bool OllyLang::DoTEST(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;

	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		zf = 0;

		if (!(dw1 & dw2)) {
			zf = 1;
		}
		setProgLineValue(script_pos+1,zf);
	}
	return true;

}


bool OllyLang::DoTI(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F11); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoTICK(string args)
{
	string ops[2];
	if(!CreateOperands(args, ops, 2))
		if(args=="" || !CreateOperands(args, ops, 1)) {
			char tmpbuf[256]={0};
			sprintf(tmpbuf,"%u ms",this->tickcount/1000);
			string s; s.assign(tmpbuf);
			variables["$RESULT"]=s;
			return true;
		}

	if (!is_variable(ops[0])) {
		DoVAR(ops[0]);
	}

	ulong timeref;
	GetDWOpValue(ops[1], timeref);

	if (is_variable(ops[0])) {
		variables[ops[0]] = tickcount;
		if (timeref)
			variables["$RESULT"]=tickcount-timeref;
		return true;
	}
	return false;
}

bool OllyLang::DoTICND(string args)
{
	string ops[1];
	string condition;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetSTROpValue(ops[0], condition))
	{
		char* buffer = new char[condition.length() + 1];
		strcpy(buffer, condition.c_str());
		if(Runtracesize() == 0) 
		{
			ulong threadid = Getcputhreadid();
			if(threadid == 0)
				threadid = Plugingetvalue(VAL_MAINTHREADID);
			t_thread* pthr = Findthread(threadid);
			if(pthr != NULL)
				Startruntrace(&(pthr->reg)); 
		}
		Settracecondition(buffer, 0, 0, 0, 0, 0);
		Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F11); 
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoTO(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F12); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoTOCND(string args)
{
	string ops[1];
	string condition;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetSTROpValue(ops[0], condition))
	{
		char* buffer = new char[condition.length() + 1];
		strcpy(buffer, condition.c_str());
		if(Runtracesize() == 0) 
		{
			ulong threadid = Getcputhreadid();
			if(threadid == 0)
				threadid = Plugingetvalue(VAL_MAINTHREADID);
			t_thread* pthr = Findthread(threadid);
			if(pthr != NULL)
				Startruntrace(&(pthr->reg)); 
		}
		Settracecondition(buffer, 0, 0, 0, 0, 0);
		Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F12); 
		require_ollyloop = 1;
		return true;
	}
	return false;
}


bool OllyLang::DoUNICODE(string args)
{
	string ops[1];
	ulong dw;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], dw)) {
		bUnicode=(dw!=0);
		return true;
	}

	return false;
}

bool OllyLang::DoVAR(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(reg_names.find(ops[0]) == reg_names.end()){
		variables.insert(pair<string, var> (ops[0], 0));
		return true;
	}
	errorstr = "Bad variable name: " + ops[0];
	return false;
}

bool OllyLang::DoXCHG(string args)
{
	string ops[2];

	if (!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;

	if (GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2)) {
		args = ops[0] + ", 0" + ultoa(dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		DoMOV(args);
		args = ops[1] + ", 0" + ultoa(dw1, buffer, 16);
		nIgnoreNextValuesHist++;
		DoMOV(args);
		return true;
	}
	return false;

}


bool OllyLang::DoXOR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	ulong dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) 
		&& GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", 0" + ultoa(dw1 ^ dw2, buffer, 16);
		nIgnoreNextValuesHist++;
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoWRT(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

    string path;
	path.assign((char*)Plugingetvalue(VAL_EXEFILENAME));

    path = path.substr(0, path.rfind('\\') + 1);
 
	string filename,data;

	if(GetSTROpValue(ops[0], filename) 
		&& GetANYOpValue(ops[1], data))
	{
        if ((filename.find(":\\") != string::npos) || (filename.find("\\\\") != string::npos)) //hard disk or network
		  path = filename;
		else
		  path += filename;
		
		HANDLE hFile;
		ulong dwAccBytes=0;

		hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) 
		{
			//SetFilePointer(hFile, 0, 0, FILE_BEGIN);
//			if (bUnicode) {
//				if (data.length()>1 && data[1]!=0)
//					data=Str2Unicode(data);
//				WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);
//			} else
				WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);
			CloseHandle(hFile);
		}
		return dwAccBytes;
	}
	return false;
}

bool OllyLang::DoWRTA(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="\"\r\n\"";
		if(!CreateOperands(args, ops, 2))
			return false;
	}
	
    string path;
	path.assign((char*)Plugingetvalue(VAL_EXEFILENAME));

    path = path.substr(0, path.rfind('\\') + 1);

	string filename,data,sSep;

	if(GetSTROpValue(ops[0], filename) 
		&& GetANYOpValue(ops[1], data))
	{
        if ((filename.find(":\\") != string::npos) || (filename.find("\\\\") != string::npos))
		  path = filename;
		else
		  path += filename;
		
		HANDLE hFile;
		ulong dwAccBytes=0;

		if (!GetSTROpValue(ops[2],sSep))
			return false;	

		data=sSep+data;	

		hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) 
		{
			SetFilePointer(hFile, 0, 0, FILE_END);
//			if (bUnicode) {
//				if (data.length()>1 && data[1]!=0)
//					data=Str2Unicode(data);
//				WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);
//			} else
				WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);

			CloseHandle(hFile);
		}
		return dwAccBytes;
	}
	return false;
}
