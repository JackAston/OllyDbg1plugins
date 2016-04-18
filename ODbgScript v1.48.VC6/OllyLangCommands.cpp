// Temp storage
char buffer[4096] = {0};

// -------------------------------------------------------------------------------
// COMMANDS
// -------------------------------------------------------------------------------
bool OllyLang::DoADD(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	string str1, str2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 + dw2, buffer, 16);
		return DoMOV(args);
	}
	else if(GetSTROpValue(ops[0], str1) && GetANYOpValue(ops[1], str2, false))
	{
		args = ops[0] + ", " + "\"" + str1 + str2 + "\"";
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

	DWORD addr, size;
	if(GetDWOpValue(ops[0], size))
	{
		HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);
		addr = (DWORD) VirtualAllocEx(hDbgPrc,NULL,size,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE);
		variables["$RESULT"] = addr;
		//Refresh Memory Window
		//Listmemory(); //LNK ERROR, not found in .LIB 
		return true;
	}
	return false;
}

bool OllyLang::DoAN(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	DWORD dw;
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

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 & dw2, buffer, 16);
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

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetSTROpValue(ops[0], title))
	{
		if (wndProg.hw!=NULL)
			InvalidateRect(wndProg.hw, NULL, FALSE);

		HINSTANCE hi = (HINSTANCE)GetModuleHandle("ODbgScript.dll");
		HWND hw = hwndOllyDbg();
		char* returned_buffer = (char*)DialogBoxParam(hi, MAKEINTRESOURCE(IDD_INPUT), hw, (DLGPROC) InputDialogProc, (LPARAM)title.c_str());
		if((byte)returned_buffer != 0)
		{
			string returned = returned_buffer;
			delete[256] returned_buffer;
			if(is_hex(returned)) {
				variables["$RESULT"] = strtoul(returned.c_str(), 0, 16);
				variables["$RESULT_1"] = (int) (returned.length() / 2);
			}
			else {
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
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	t_asmmodel model;
	DWORD addr;
	string cmd;
	char error[255] = {0};
	int len = 0;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], cmd))
	{
		strcpy(buffer, cmd.c_str());
		if((len = Assemble(buffer, addr, &model, 0, 0, error)) < 0)
		{
			errorstr = error;
			return false;
		}
		else
		{
			Writememory(model.code, addr, len, MM_DELANAL | MM_SILENT);
			Broadcast(WM_USER_CHALL, 0, 0);
			variables["$RESULT"] = len;
			require_ollyloop = 1;
			return true;
		}
	}

	return false;
}


bool OllyLang::DoATOI(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2)) {
		ops[1] = "16.";
		if(!CreateOperands(args, ops, 1))
			return false;
	}

	string str;
	DWORD base, dw;

	if(GetSTROpValue(ops[0], str) && GetDWOpValue(ops[1], base) )
	{
		variables["$RESULT"] = strtoul(str.c_str(),0,base);
		return true;
	}
	return false;
}

bool OllyLang::DoBC(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	DWORD dw;
	if(GetDWOpValue(ops[0], dw))
	{
		Deletebreakpoints(dw, dw + 1, 0);
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}
	return false;
}

bool OllyLang::DoBP(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	DWORD dw;
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

	DWORD addr = 0;
	string condition;
	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], condition))
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

bool OllyLang::DoBPHWCALL(string args)
{
	int i = 0;
	while(i < 4){
		if(Deletehardwarebreakpoint(i) == -1)
			return false;
		i++;
	}
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoBPHWC(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	DWORD dw1;
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

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1;
	string str1;
	if(GetDWOpValue(ops[0], dw1) && GetSTROpValue(ops[1], str1))
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

	DWORD addr = 0;
	string expression;
	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], expression))
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

	DWORD addr = 0;
	string expression, condition;
	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], expression) && GetSTROpValue(ops[2], condition))
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

	DWORD addr, size;
	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size))
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

	DWORD addr, size;
	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size))
	{
		Setmembreakpoint(MEMBP_READ | MEMBP_WRITE, addr, size);
		return true;
	}
	return false;
}

bool OllyLang::DoCMP(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1 = 0, dw2 = 0;
	string s1, s2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
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
		else // if(dw1 < dw2)
		{
			zf = 0;
			cf = 1;
		}
		return true;
	}
	else if(GetANYOpValue(ops[0], s1, false) && GetANYOpValue(ops[1], s2, false))
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
bool OllyLang::DoCMT(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	string cmt;
	DWORD addr;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], cmt))
	{
		if(cmt != "")
		{
			strcpy(buffer, cmt.c_str());
			Insertname(addr, NM_COMMENT, buffer);
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
	byte buffer[4];
	ulong fs = thr->reg.limit[2]; // BUG IN ODBG!!!
	fs += 0x30;
	Readmemory(buffer, fs, 4, MM_RESTORE);
	fs = *((ulong*)buffer);
	fs += 2;
	buffer[0] = 0;
	Writememory(buffer, fs, 1, MM_RESTORE);
	return true;   
}

bool OllyLang::DoDBS(string args)
{
	t_thread* thr = Findthread(Getcputhreadid());
	byte buffer[4];
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

bool OllyLang::DoDM(string args)
{
	string ops[3];
	if(!CreateOperands(args, ops, 3))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);
	
	DWORD addr, size;
	string filename;
	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size) && GetSTROpValue(ops[2], filename))
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

	DWORD addr, size;
	string filename;
	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size) && GetSTROpValue(ops[2], filename))
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
	DWORD ep;
	if(GetSTROpValue(ops[0], filename) && GetDWOpValue(ops[1], ep))
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
	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);
	VirtualFreeEx(hDebugee, pmemforexec, 0, MEM_RELEASE);
	return true;
}

bool OllyLang::DoESTI(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F7); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoESTO(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F9); 
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

	if(!CreateOperands(args, ops, 1))
		return false;

	string to_eval;

	if(GetSTROpValue(ops[0], to_eval))
	{
		string res = ResolveVarsForExec(to_eval);
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

	// Assemble and write code to memory (everything between EXEC and ENDE)
	while(ToLower(script[script_pos]) != "ende")
	{
		tmp = ResolveVarsForExec(script[script_pos]);
		sprintf(buffer, tmp.c_str());
		len = Assemble(buffer, (ulong)pmemforexec + totallen, &model, 0, 0, error);
		WriteProcessMemory(hDebugee, (LPVOID)((ulong)pmemforexec + totallen), model.code, len, 0);
		totallen += len;
		script_pos++;
	}

	// Assemble and write jump to original EIP
	sprintf(buffer, "JMP %lX", eip);
	len = Assemble(buffer, (ulong)pmemforexec + totallen, &model, 0, 0, error);
	WriteProcessMemory(hDebugee, (LPVOID)((ulong)pmemforexec + totallen), model.code, len, 0);

	// Set new eip and run to the original one
	thr->reg.ip = (ulong)pmemforexec;
	thr->reg.modified = 1;
	thr->regvalid = 1;
	Broadcast(WM_USER_CHREG, 0, 0);
	Go(Getcputhreadid(), eip, STEP_RUN, 0, 1);
	return true;
}

bool OllyLang::DoFILL(string args)
{
	string ops[3];

	if(!CreateOperands(args, ops, 3))
		return false;
	DWORD start, len, val;

	if(GetDWOpValue(ops[0], start) && GetDWOpValue(ops[1], len) && GetDWOpValue(ops[2], val))
	{
		byte* buffer = new byte[len];
		byte b = 0 + val;
		for(ulong i = 0; i < len; i++)
			buffer[i] = b;
		Writememory(buffer, start, len, MM_DELANAL | MM_SILENT);
		delete [] buffer;
		Broadcast(WM_USER_CHALL, 0, 0);
		require_ollyloop = 1;
		return true;
	}

	return false;
}

bool OllyLang::DoFIND(string args)
{
	string ops[2];
	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD addr,dw;
	string data=ops[1];
	if (UnquoteString(ops[1], '#', '#')) {
		if(ops[1].length() % 2 != 0)
		{
			errorstr = "Hex string must have an even number of characters!";
			return false;
		}
	}
	else if (GetSTROpValue(data,ops[1])) {
		Str2Hex(ops[1],data);
		ops[1]=data;
	}
	else if (GetDWOpValue(ops[1], dw)) {
		DoREV(ops[1]);
		itoa(variables["$RESULT"].dw,buffer,16);
		data=buffer;
		while(data.length() < ops[1].length())
			data.insert(0,"0");
		while (data.length() > ops[1].length())
			data.erase(data.length()-1,1);
		ops[1]=data;
	}

	if(GetDWOpValue(ops[0], addr) && is_hexwild(ops[1]))
	{
		if(ops[1].find('?') != -1)
		{
			// Wildcard search
			char *membuf = 0;
			t_memory* tmem = Findmemory(addr);
			int memlen = tmem->size - (addr - tmem->base);
			membuf = new char[memlen];

			memlen = Readmemory(membuf, addr, memlen, MM_RESILENT);
			int pos = FindWithWildcards(membuf, ops[1].c_str(), memlen);

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
			t_memory* tmem = Findmemory(addr);
			int memlen = tmem->size - (addr - tmem->base);
			membuf = new char[memlen];

			int len = Str2Rgch(ops[1], buffer, sizeof(buffer));

			memlen = Readmemory(membuf, addr, memlen, MM_RESILENT);
			char* p = search(membuf, membuf + memlen, buffer, buffer + len);

			delete [] membuf;

			if(p < membuf + memlen)
				variables["$RESULT"] = addr + p - membuf;
			else
				variables["$RESULT"] = 0;
		}
		return true;
	}
	return false;
}

bool OllyLang::DoFINDOP(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD addr, dw, endaddr;
	string data=ops[1];
	if (UnquoteString(ops[1], '#', '#')) {
		if(ops[1].length() % 2 != 0)
		{
			errorstr = "Hex string must have an even number of characters!";
			return false;
		}
	}
	else if (GetSTROpValue(data,ops[1])) {
		Str2Hex(ops[1],data);
		ops[1]=data;
	}
	else if (GetDWOpValue(ops[1], dw)) {
		DoREV(ops[1]);
		itoa(variables["$RESULT"].dw,buffer,16);
		ops[1]=buffer;
		while(data.length() < ops[1].length())
			data.insert(0,"0");
		while (data.length() > ops[1].length())
			data.erase(data.length()-1,1);
		ops[1]=data;
	}

	int result = -1;
	int ok = 0;
	if(GetDWOpValue(ops[0], addr) && is_hexwild(ops[1]))
	{
		t_memory* tmem = Findmemory(addr);
		char cmd[MAXCMDSIZE] = {0};
		do 
		{
			addr = Disassembleforward(0, tmem->base, tmem->size, addr, 1, 0); 
			endaddr = Disassembleforward(0, tmem->base, tmem->size, addr, 1, 0); 
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
	DWORD start=0; 
	string sArgs;
	bool bSkip;

	if (!GetDWOpValue(ops[1], start))
		return false;


	for (int m=0; m < tt->data.n; m++) {
		tm=(t_memory*) Getsortedbyselection(&tt->data, m);
		if (tm==NULL)
			return false;

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

bool OllyLang::DoFREE(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD addr, size;
	if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size))
	{
		HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);
		variables["$RESULT"] = (DWORD) VirtualFreeEx(hDbgPrc,(void*)addr,size,MEM_DECOMMIT);
		//Refresh Memory Window
		//Listmemory(); //LNK ERROR, not found in .LIB 
		return true;
	}
	return false;
}

bool OllyLang::DoGCMT(string args)
{
	string cmt;
	DWORD addr,size;

	if(GetDWOpValue(args, addr))
	{
		if(addr != 0)
		{
			size = Findname(addr, NM_COMMENT, buffer);
			if (size==0)
				size = Findname(addr, NM_LIBCOMM, buffer);
			if (size==0)
				size = Findname(addr, NM_ANALYSE, buffer);
					
			cmt = buffer;
			variables["$RESULT"] = cmt;
			return true;
		}
	}
	return false;
}

bool OllyLang::DoGMEMI(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD addr = 0;
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

bool OllyLang::DoGMI(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD addr = 0;
	string str;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue("\""+ops[1]+"\"", str) )
	{
		t_module* mod = Findmodule(addr);

		transform(str.begin(), str.end(), str.begin(), toupper);
		
		if(str == "MODULEBASE" && mod != NULL)
		{ 
			variables["$RESULT"] = (DWORD)mod->base;
			return true;
		}
		else if(str == "MODULESIZE" && mod != NULL)
		{
			variables["$RESULT"] = (DWORD)mod->size;
			return true;
		}
		else if(str == "CODEBASE" && mod != NULL)
		{
			variables["$RESULT"] = (DWORD)mod->codebase;
			return true;
		}
		else if(str == "CODESIZE" && mod != NULL)
		{
			variables["$RESULT"] = (DWORD)mod->origcodesize;
			return true;
		}
		else if(mod == NULL)
		{
			variables["$RESULT"] = 0;
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
	DWORD addr;

	if(!CreateOperands(args, ops, 1))
		return false;

	if(GetDWOpValue(ops[0], addr))
	{
		char sym[4096] = {0};
		char buf[TEXTLEN] = {0};
		int res = Decodeaddress(addr, 0, ADC_JUMP | ADC_STRING | ADC_ENTRY | ADC_OFFSET | ADC_SYMBOL, sym, 4096, buf);
		if(res)
		{
			variables["$RESULT"] = sym;
			char* tmp = strstr(sym, ".");
			if(tmp)
			{
				*tmp = '\0';
				variables["$RESULT_1"] = sym;
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
	DWORD addr;

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

bool OllyLang::DoGPA(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	string proc, lib;

	if(GetSTROpValue(ops[0], proc) && GetSTROpValue(ops[1], lib))
	{
		HMODULE hMod = LoadLibraryEx(lib.c_str(),NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
		if(hMod == 0)
		{
			//errorstr = "No such library: " + lib;
			variables["$RESULT"] = 0;
			variables["$RESULT_1"] = "";
			variables["$RESULT_2"] = "";
			return true;
		}
		variables["$RESULT_1"] = lib;

		FARPROC p = GetProcAddress(hMod, proc.c_str());
		FreeLibrary(hMod);

		if(p == 0)
		{
			//errorstr = "No such procedure: " + proc;
			variables["$RESULT"] = 0;
			return true;
		}
		variables["$RESULT"] = (DWORD) p;
		variables["$RESULT_2"] = proc;
		return true;
	}

	variables["$RESULT"] = 0;
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

//in dev... i try to find API parameters number and types
bool OllyLang::DoGPP(string args)
{
	if (!DoGPA(args))
		return false;

	DWORD addr = variables["$RESULT"].dw;
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
		byte buffer[MAXCMDSIZE];
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

bool OllyLang::DoHANDLE(string args)
{
	string ops[2];
	bool useCaption=true;

	if(!CreateOperands(args, ops, 3)) {
			return false;
	}

	string sClassName;
	DWORD x,y;
	DWORD thid = Plugingetvalue(VAL_MAINTHREADID);

	if(GetDWOpValue(ops[0], x) && GetDWOpValue(ops[1], y) && GetSTROpValue(ops[2], sClassName)) {
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

bool OllyLang::DoITOA(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2)) {
		ops[1] = "16.";
		if(!CreateOperands(args, ops, 1))
			return false;
	}

	DWORD base, dw;

	if(GetDWOpValue(ops[0], dw) && GetDWOpValue(ops[1], base) )
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
	DWORD key,shift,ctrl;

	if(!CreateOperands(args, ops, 3)) {
		ops[2]="0";
		if(!CreateOperands(args, ops, 2)) {
			ops[1]="0";
			if(!CreateOperands(args, ops, 1))
				return false;
		}
	}
	
	if(GetDWOpValue(ops[0], key) && GetDWOpValue(ops[1], shift) && GetDWOpValue(ops[2], ctrl)) {
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
	DWORD addr;

	if(GetDWOpValue(ops[0], addr) && GetSTROpValue(ops[1], lbl))
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

    DWORD addr, size;
    string filename;
    if(GetDWOpValue(ops[0], addr) && GetDWOpValue(ops[1], size) && GetSTROpValue(ops[2], filename))
    {

        if (filename.find(":\\") != string::npos)
			path = filename;
		else
			path += filename;

		std::ifstream fin(path.c_str(),ios::in | ios::binary);
    
        char buf[4096];
		while (!fin.eof())
        {
			fin.read(buf, sizeof buf);			
			int gotten=fin.gcount();
			sum += Writememory(buf, addr+sum, gotten, MM_RESILENT);
        }
        fin.close();

		if (sum) {
	        variables["$RESULT"] = sum;
            return true;
        }
        else
        {
            errorstr = "Couldn't create file!";
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

	if(!GetSTROpValue(ops[1], str))
		return false;

	variables["$RESULT"] = (int) str.length();
	return true;
}

bool OllyLang::DoLOG(string args)
{
	string prefix, ops[2];

	if(CreateOperands(args, ops, 2)) {
		if (!GetSTROpValue(ops[1], prefix))
			return false;
	} else {
		prefix="DEFAULT";
		if(!CreateOperands(args, ops, 1))
			return false;
	}

	DWORD dw=0;
	string str;
	long double flt=0;

	if(UnquoteString(ops[0], '"', '"'))
	{
		//string constant
		if (prefix.compare("DEFAULT") == 0) {
			strncpy(buffer, ops[0].c_str(),TEXTLEN-1);
		} else {
			prefix = prefix + ops[0];
			strncpy(buffer, prefix.c_str(),TEXTLEN-1);
		}
		buffer[TEXTLEN-1]=0;
		Infoline(buffer);
		Addtolist(0, 1, buffer);
		add2log(buffer);
		return true;
	}
	else {
		vtype vt=EMP;
		if (is_variable(ops[0]))
			vt = variables[ops[0]].vt;

		if (vt==DW || vt==EMP) {
			if(GetDWOpValue(ops[0], dw))
			{
				//dword constant
				if (prefix.compare("DEFAULT") == 0) {

					char sym[4096] = {0};
					char buf[TEXTLEN-1] = {0};
					int res = Decodeaddress(dw, 0, ADC_JUMP | ADC_STRING | ADC_ENTRY | ADC_OFFSET | ADC_SYMBOL, sym, 4096, buf);
					sprintf(buffer, "%s: %08lX", ops[0].c_str(), dw);

					if(strcmp(buf, ""))
						sprintf(buffer, "%s | %s", buffer, buf);

					if(strcmp(sym, "") && dw !=  strtoul(sym, 0, 16))
						sprintf(buffer, "%s | %s", buffer, sym);
				
				} else {
					sprintf(buffer, "%s%08lX", prefix.c_str(), dw);
				}

				buffer[TEXTLEN-1]=0;
				Infoline(buffer);
				Addtolist(0, 1, buffer);
				add2log(buffer);
				return true;
			}
		}

		if (vt==FLT || vt==EMP) {
			if(GetFLTOpValue(ops[0], flt))
			{
				//dword constant
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
				
				if(UnquoteString(str, '#', '#')) {

					//log a buffer
					if (prefix.compare("DEFAULT") == 0) {
						if (str.length()+ops[0].length() < 4094)
							sprintf(buffer, "%s: %s", ops[0].c_str(), str.c_str());
						else
							strncpy(buffer, variables[ops[0]].str.c_str(), TEXTLEN-1);
					} else {
						prefix = prefix + str;
						strncpy(buffer, prefix.c_str(), TEXTLEN-1);
					}
				} else {

					//log a string
					if (prefix.compare("DEFAULT") == 0) {
						if (variables[ops[0]].str.length()+ops[0].length() < 4094)
							sprintf(buffer, "%s: %s", ops[0].c_str(), variables[ops[0]].str.c_str());
						else
							strncpy(buffer, variables[ops[0]].str.c_str(), TEXTLEN-1);
					} else {
						prefix = prefix + str;
						strncpy(buffer, prefix.c_str(), TEXTLEN-1);
					}
				}
				buffer[TEXTLEN-1]=0;
				Addtolist(0, 1, buffer);
				add2log(buffer);
				return true;
			}
		}
	}
	return false;
}

bool OllyLang::DoMOV(string args)
{
	string ops[3];
	bool bDeclared=false;
	DWORD maxsize=0;
	if(CreateOperands(args, ops, 3))
		GetDWOpValue(ops[2], maxsize);
	else
		if(!CreateOperands(args, ops, 2))
			return false;

	// Check source
	DWORD dw = 0, addr = 0;
	string str = "";
	long double flt;

	// Used to retry after automatic variable declaration 
	retry_DoMOV:

	// Check destination
	if(variables.find(ops[0]) != variables.end())
	{
		// Dest is variable
		if(GetDWOpValue(ops[1], dw) && maxsize <= 4)
		{
			dw = resizeDW(dw,maxsize);
			variables[ops[0]].vt = DW;
			variables[ops[0]] = dw;
		}
		else if(GetSTROpValue(ops[1], str, maxsize))
		{
			variables[ops[0]].vt = STR;
			variables[ops[0]] = str;
		}
		else if(GetFLTOpValue(ops[1], flt))
		{
			variables[ops[0]].vt = FLT;
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

				if (ops[0].length()==2) {
					if (ops[0][1] == 'l') {
						//R8
						dw &= 0xFF;
						pt->reg.r[regnr] &= 0xFFFFFF00;
						pt->reg.r[regnr] |= dw;
					} else if ( ops[0][1] == 'h') {
						//R8
						dw &= 0xFF;
						pt->reg.r[regnr] &= 0xFFFF00FF;
						pt->reg.r[regnr] |= dw * 0x100;
					} else {
						//R16
						dw &= 0xFFFF;
						pt->reg.r[regnr] &= 0xFFFF0000;
						pt->reg.r[regnr] |= dw;
					}
				} 
				else
					//R32
					pt->reg.r[regnr] = dw;
			}
			else
				if(ops[0] == "eip")
					pt->reg.ip = dw;
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
	else if(is_floatreg(ops[0])) {
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
		// Dest is memory address
		int len;
		if(GetDWOpValue(ops[0], addr))
		{
			if(ops[1].length() % 2 == 0 && UnquoteString(ops[1], '#', '#'))
			{
				len = Str2Rgch(ops[1], buffer, sizeof(buffer));				
				Writememory(buffer, addr, len, MM_DELANAL | MM_SILENT);
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if(GetDWOpValue(ops[1], dw) && maxsize <= 4)
			{
				if (maxsize==0) maxsize=4;
				dw = resizeDW(dw,maxsize);
				Writememory(&dw + (4-maxsize), addr, maxsize, MM_DELANAL | MM_SILENT);				
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if(GetSTROpValue(ops[1], str, maxsize))
			{
				if (UnquoteString(str, '#', '#')) {
					len = Str2Rgch(str, buffer, sizeof(buffer));				
					Writememory(buffer, addr, len, MM_DELANAL | MM_SILENT);			
				} else {
					strcpy(buffer, str.c_str());
					Writememory(buffer, addr, str.length(), MM_DELANAL | MM_SILENT);
				}
				Broadcast(WM_USER_CHALL, 0, 0);
			}
			else if(GetFLTOpValue(ops[1], flt))
			{
				Writememory(&flt, addr, 8, MM_DELANAL | MM_SILENT);				
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

	} else if (!bDeclared && ops[0][0] >= 'A' ) {

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

	if(!CreateOperands(args, ops, 1))
		return false;

	string msg;
	if(GetANYOpValue(ops[0], msg, false))
	{
		if (wndProg.hw!=NULL)
			InvalidateRect(wndProg.hw, NULL, FALSE);

		int ret = MessageBox(hwndOllyDbg(), msg.c_str(), "MSG ODbgScript", MB_ICONINFORMATION | MB_OKCANCEL | MB_TOPMOST);
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

		int ret = MessageBox(hwndOllyDbg(), msg.c_str(), "MSG ODbgScript", MB_ICONQUESTION | MB_YESNOCANCEL | MB_TOPMOST);
		if(ret == IDCANCEL) {
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

bool OllyLang::DoOR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 | dw2, buffer, 16);
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoOPCODE(string args)
{
	string ops[1];
	if(!CreateOperands(args, ops, 1))
		return false;

	DWORD addr,size;
	if(GetDWOpValue(ops[0], addr))
	{
		byte buffer[MAXCMDSIZE];
		size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
		
		if (size>0) {
			t_disasm disasm;
			size=Disasm(buffer,size,addr,NULL,&disasm,DISASM_CODE,NULL);

			if (size>0) {
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

bool OllyLang::DoPAUSE(string args)
{
	return Pause();
}

bool OllyLang::DoPREOP(string args)
{
	string ops[1];

	CreateOperands(args, ops, 1);
	if (ops[0]=="")
		ops[0]="eip";

	DWORD addr,base;
	if(!GetDWOpValue(ops[0], addr))
		return false;

	t_memory* mem = Findmemory(addr);
	if(mem != NULL)	{
		variables["$RESULT"] = Disassembleback(NULL,mem->base,mem->size,addr,1,true);
		return true;
	} else {
		variables["$RESULT"] = 0;
		return true;
	}
	return false;
}

bool OllyLang::DoREF(string args)
{
	string ops[1];
	if(!CreateOperands(args, ops, 1))
		return false;

	char title[256]="Reference to Command - ODbgScript REF";	
	DWORD addr,size;
	if(GetDWOpValue(ops[0], addr))
	{
		variables["$RESULT"] = 0;
		variables["$RESULT_1"] = 0; //command bytes
		variables["$RESULT_2"] = 0;

		if (adrREF!=addr) {
			curREF=-1;
			
			//Info to get destination address
			byte buffer[MAXCMDSIZE];
			size=Readmemory(buffer, addr, MAXCMDSIZE, MM_SILENT);
			t_disasm disasm;
			size=Disasm(buffer, size, addr, NULL, &disasm, DISASM_SIZE, NULL);
			if (size==0)
				return true;

			// Get process handle and save eip
			HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);
			t_thread* thr = Findthread(Getcputhreadid());
			DWORD eip = thr->reg.ip;

			//Search for references
			t_memory* mem = Findmemory(addr);

			if (Findreferences(mem->base,mem->size,addr,addr+size,addr,16,title) > 0)
				adrREF=addr;
			else
				return true;
		}
		t_table* tref=(t_table*) Plugingetvalue(VAL_REFERENCES);
		
		if (tref != NULL) {
			ignore_origin:
			curREF++;
			if (curREF > tref->data.n)
				adrREF=0;
			else 
			{
				t_ref* ref= (t_ref*) Getsortedbyselection(&tref->data,curREF);
				if (ref != NULL) {

					if (ref->addr == addr)
						goto ignore_origin;

					//Disasm origin to get comments
					byte buffer[MAXCMDSIZE];
					size=Readmemory(buffer, ref->addr, MAXCMDSIZE, MM_SILENT);					
					if (size>0) {
						t_disasm disasm;
						size=Disasm(buffer,size,ref->addr,NULL,&disasm,DISASM_ALL,NULL);

						if (size>0) {
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

	DWORD addr, len;
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
	Reset();
	MessageBox(hwmain, "Script finished", "ODbgScript", MB_ICONINFORMATION | MB_OK | MB_TOPMOST);
	return true;
}

bool OllyLang::DoREV(string args)
{
	string ops[1];
	DWORD dw;
	byte b, tb[4];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(!GetDWOpValue(ops[0], dw))
		return false;

	memcpy(&tb[0],&dw,4);
	dw=tb[3]; tb[3]=tb[0]; tb[0]=dw;
	dw=tb[2]; tb[2]=tb[1]; tb[1]=dw;
	memcpy(&dw,&tb[0],4);

	variables["$RESULT"] = dw;
	return true;
}

bool OllyLang::DoRTR(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F9); 
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoRTU(string args)
{
	PostMessage(hwmain, WM_SYSKEYDOWN, VK_F9, 0);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoRUN(string args)
{
	Go(Getcputhreadid(), 0, STEP_RUN, 0, 1);
	require_ollyloop = 1;
	return true;
}

bool OllyLang::DoSCMP(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2, true))
		return false;

	string s1, s2;
	if(GetSTROpValue(ops[0], s1) && GetSTROpValue(ops[1], s2))
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
	string ops[2];

	if(!CreateOperands(args, ops, 2, true))
		return false;
	
	string s1, s2;
	if(GetSTROpValue(ops[0], s1) && GetSTROpValue(ops[1], s2))
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

bool OllyLang::DoSHL(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 << dw2, buffer, 16);
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoSHR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 >> dw2, buffer, 16);
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

bool OllyLang::DoSUB(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 - dw2, buffer, 16);
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoTC(string args)
{
	Deleteruntrace();
	return true;
}

bool OllyLang::DoTI(string args)
{
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 1, 0, VK_F11); 
	require_ollyloop = 1;
	return true;
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

bool OllyLang::DoVAR(string args)
{
	string ops[1];

	if(!CreateOperands(args, ops, 1))
		return false;

	if(reg_names.find(ops[0]) == reg_names.end())
	{
		variables.insert(pair<string, var> (ops[0], 0));
		return true;
	}
	errorstr = "Bad variable name: " + ops[0];
	return false;
}

bool OllyLang::DoXOR(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

	DWORD dw1, dw2;
	if(GetDWOpValue(ops[0], dw1) && GetDWOpValue(ops[1], dw2))
	{
		args = ops[0] + ", " + ultoa(dw1 ^ dw2, buffer, 16);
		return DoMOV(args);
	}
	return false;
}

bool OllyLang::DoWRT(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);
 
	string filename,data;

	if(GetSTROpValue(ops[0], filename) && GetANYOpValue(ops[1], data, false))
	{
        if (filename.find(":\\") != string::npos)
		  path = filename;
		else
		  path += filename;
		
		HANDLE hFile;
		DWORD dwAccBytes=0;

		hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) 
		{
			//SetFilePointer(hFile, 0, 0, FILE_BEGIN);
			WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);
			CloseHandle(hFile);
		}
		return dwAccBytes;
	}
	return false;
}

bool OllyLang::DoWRTA(string args)
{
	string ops[2];

	if(!CreateOperands(args, ops, 2))
		return false;

    char spath[MAX_PATH];
    strcpy(spath, (char*)Plugingetvalue(VAL_EXEFILENAME));
    string path = spath;

    path = path.substr(0, path.rfind('\\') + 1);

	string filename,data;

	if(GetSTROpValue(ops[0], filename) && GetANYOpValue(ops[1], data, false))
	{
        if (filename.find(":\\") != string::npos)
		  path = filename;
		else
		  path += filename;
		
		HANDLE hFile;
		DWORD dwAccBytes=0;

		hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE) 
		{
			SetFilePointer(hFile, 0, 0, FILE_END);
			WriteFile(hFile, data.c_str(), data.length(), &dwAccBytes, NULL);
			CloseHandle(hFile);
		}
		return dwAccBytes;
	}
	return false;
}