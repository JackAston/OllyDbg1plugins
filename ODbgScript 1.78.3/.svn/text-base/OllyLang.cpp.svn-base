// OllyLang.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

using namespace std;

OllyLang::OllyLang()
{
	memset(&wndProg,0,sizeof(wndProg));
	memset(&wndLog,0,sizeof(wndLog));

	script_state = SS_NONE;

	// Init array with register names
	reg_names.clear();
	reg_names.insert("eax");
	reg_names.insert("ebx");
	reg_names.insert("ecx");
	reg_names.insert("edx");
	reg_names.insert("esi");
	reg_names.insert("edi");
	reg_names.insert("esp");
	reg_names.insert("ebp");
	reg_names.insert("eip");
//  new reg

	reg_names.insert("ax");
    reg_names.insert("bx");
    reg_names.insert("cx");
	reg_names.insert("dx");
	reg_names.insert("bp");
	reg_names.insert("si");
    reg_names.insert("di");

	reg_names.insert("al");
	reg_names.insert("ah");
	reg_names.insert("bl");
	reg_names.insert("bh");
	reg_names.insert("cl");
	reg_names.insert("ch");
	reg_names.insert("dl");
	reg_names.insert("dh");

	// Init array of flag names
	flag_names.clear();
	flag_names.insert("!CF");
	flag_names.insert("!PF");
	flag_names.insert("!AF");
	flag_names.insert("!ZF");
	flag_names.insert("!SF");
	flag_names.insert("!DF");
	flag_names.insert("!OF");

	// Init command array
	commands["add"] = &OllyLang::DoADD;
	commands["ai"] = &OllyLang::DoAI;
	commands["alloc"] = &OllyLang::DoALLOC;
	commands["an"] = &OllyLang::DoAN;
	commands["and"] = &OllyLang::DoAND;
	commands["ao"] = &OllyLang::DoAO;
	commands["ask"] = &OllyLang::DoASK;
	commands["asm"] = &OllyLang::DoASM;	
    commands["asmtxt"] = &OllyLang::DoASMTXT;	
	commands["atoi"] = &OllyLang::DoATOI;
	commands["backup"] = &OllyLang::DoBACKUP;
	commands["bc"] = &OllyLang::DoBC;
	commands["bd"] = &OllyLang::DoBD;
	commands["beginsearch"] = &OllyLang::DoBEGINSEARCH;
	commands["bp"] = &OllyLang::DoBP;
	commands["bpcnd"] = &OllyLang::DoBPCND;
	commands["bpd"] = &OllyLang::DoBPD;
	commands["bpgoto"] = &OllyLang::DoBPGOTO;
	 commands["bphwcall"] = &OllyLang::DoBPHWCA;
	commands["bphwc"] = &OllyLang::DoBPHWC;
	commands["bphws"] = &OllyLang::DoBPHWS;
	commands["bpl"] = &OllyLang::DoBPL;
	commands["bplcnd"] = &OllyLang::DoBPLCND;
	commands["bpmc"] = &OllyLang::DoBPMC;
	commands["bprm"] = &OllyLang::DoBPRM;
	commands["bpwm"] = &OllyLang::DoBPWM;
	commands["bpx"] = &OllyLang::DoBPX;
	commands["buf"] = &OllyLang::DoBUF;
	commands["call"] = &OllyLang::DoCALL;
	commands["close"] = &OllyLang::DoCLOSE;
	commands["cmp"] = &OllyLang::DoCMP;
	commands["cmt"] = &OllyLang::DoCMT;
	commands["cob"] = &OllyLang::DoCOB;
	commands["coe"] = &OllyLang::DoCOE;
	commands["dbh"] = &OllyLang::DoDBH;
	commands["dbs"] = &OllyLang::DoDBS;
	commands["dec"] = &OllyLang::DoDEC;
	commands["div"] = &OllyLang::DoDIV;
	commands["dm"] = &OllyLang::DoDM;
	commands["dma"] = &OllyLang::DoDMA;
	commands["dpe"] = &OllyLang::DoDPE;
	commands["ende"] = &OllyLang::DoENDE;
	commands["endsearch"] = &OllyLang::DoENDSEARCH;
	commands["erun"] = &OllyLang::DoERUN;
	commands["esti"] = &OllyLang::DoESTI;
	 commands["esto"] = &OllyLang::DoERUN;
	commands["estep"] = &OllyLang::DoESTEP;
	commands["eob"] = &OllyLang::DoEOB;
	commands["eoe"] = &OllyLang::DoEOE;
	commands["eval"] = &OllyLang::DoEVAL;
	commands["exec"] = &OllyLang::DoEXEC;
	commands["fill"] = &OllyLang::DoFILL;
	commands["find"] = &OllyLang::DoFIND;
	commands["findcalls"] = &OllyLang::DoFINDCALLS;
	commands["findcmd"] = &OllyLang::DoFINDCMD;
	 commands["findcmds"] = &OllyLang::DoFINDCMD;
	commands["findop"] = &OllyLang::DoFINDOP;
	commands["findoprev"] = &OllyLang::DoFINDOPREV;	
	commands["findmem"] = &OllyLang::DoFINDMEM;
	commands["free"] = &OllyLang::DoFREE;
	commands["gapi"] = &OllyLang::DoGAPI;
	commands["gbpm"] = &OllyLang::DoGBPM;
	commands["gbpr"] = &OllyLang::DoGBPR;
	commands["gci"] = &OllyLang::DoGCI;
	commands["gcmt"] = &OllyLang::DoGCMT;
	commands["gfo"] = &OllyLang::DoGFO;
	commands["glbl"] = &OllyLang::DoGLBL;
	commands["gmemi"] = &OllyLang::DoGMEMI;
	commands["gmexp"] = &OllyLang::DoGMEXP;
	commands["gma"] = &OllyLang::DoGMA;
	commands["gmi"] = &OllyLang::DoGMI;
	commands["gmimp"] = &OllyLang::DoGMIMP;
	commands["gn"] = &OllyLang::DoGN;
	commands["go"] = &OllyLang::DoGO;
	commands["gopi"] = &OllyLang::DoGOPI;	
	commands["gpa"] = &OllyLang::DoGPA;
	commands["gpi"] = &OllyLang::DoGPI;
	commands["gpp"] = &OllyLang::DoGPP;
	commands["gro"] = &OllyLang::DoGRO;
	commands["gref"] = &OllyLang::DoGREF;
	commands["gsl"] = &OllyLang::DoGSL;
	commands["gstr"] = &OllyLang::DoGSTR;
	commands["gstrw"] = &OllyLang::DoGSTRW;
	commands["handle"] = &OllyLang::DoHANDLE;
	commands["history"] = &OllyLang::DoHISTORY;
	commands["inc"] = &OllyLang::DoINC;
	commands["itoa"] = &OllyLang::DoITOA;
	commands["ja"] = &OllyLang::DoJA;
	commands["jae"] = &OllyLang::DoJAE;
	commands["jb"] = &OllyLang::DoJB;
	commands["jbe"] = &OllyLang::DoJBE;
	commands["je"] = &OllyLang::DoJE;
	 commands["jg"] = &OllyLang::DoJA;
	 commands["jge"] = &OllyLang::DoJAE;
	commands["jmp"] = &OllyLang::DoJMP;
	commands["jne"] = &OllyLang::DoJNE;
	 commands["jnz"] = &OllyLang::DoJNE;
	 commands["jz"] = &OllyLang::DoJE;
	commands["key"] = &OllyLang::DoKEY;
	commands["lbl"] = &OllyLang::DoLBL;
    commands["lc"] = &OllyLang::DoLC;
    commands["lclr"] = &OllyLang::DoLCLR;
    commands["len"] = &OllyLang::DoLEN;
	commands["loadlib"] = &OllyLang::DoLOADLIB;
    commands["lm"] = &OllyLang::DoLM;
	commands["log"] = &OllyLang::DoLOG;
	commands["logbuf"] = &OllyLang::DoLOGBUF;
	commands["memcpy"] = &OllyLang::DoMEMCOPY;
	commands["mov"] = &OllyLang::DoMOV;
	commands["msg"] = &OllyLang::DoMSG;
	commands["msgyn"] = &OllyLang::DoMSGYN;
	commands["mul"] = &OllyLang::DoMUL;
	commands["names"] = &OllyLang::DoNAMES;	
    commands["neg"] = &OllyLang::DoNEG;
	commands["not"] = &OllyLang::DoNOT;
	commands["or"] = &OllyLang::DoOR;
	commands["olly"] = &OllyLang::DoOLLY;
	commands["opcode"] = &OllyLang::DoOPCODE;
	commands["opendump"] = &OllyLang::DoOPENDUMP;
	commands["opentrace"] = &OllyLang::DoOPENTRACE;
	commands["pause"] = &OllyLang::DoPAUSE;
	commands["pop"] = &OllyLang::DoPOP;
	commands["popa"] = &OllyLang::DoPOPA;
	commands["preop"] = &OllyLang::DoPREOP;
	commands["push"] = &OllyLang::DoPUSH;
	commands["pusha"] = &OllyLang::DoPUSHA;
	commands["rbp"] = &OllyLang::DoRBP;
	commands["readstr"] = &OllyLang::DoREADSTR;
	commands["refresh"] = &OllyLang::DoREFRESH;
	commands["ref"] = &OllyLang::DoREF;
	commands["repl"] = &OllyLang::DoREPL;
	commands["reset"] = &OllyLang::DoRESET;
	commands["ret"] = &OllyLang::DoRET;
	commands["rev"] = &OllyLang::DoREV;
    commands["rol"] = &OllyLang::DoROL;
    commands["ror"] = &OllyLang::DoROR;
	commands["rtr"] = &OllyLang::DoRTR;
	commands["rtu"] = &OllyLang::DoRTU;
	commands["run"] = &OllyLang::DoRUN;
	commands["sbp"] = &OllyLang::DoSBP;
	commands["scmp"] = &OllyLang::DoSCMP;
	commands["scmpi"] = &OllyLang::DoSCMPI;
    commands["setoption"] = &OllyLang::DoSETOPTION;
	commands["shl"] = &OllyLang::DoSHL;
	commands["shr"] = &OllyLang::DoSHR;
	 commands["step"] = &OllyLang::DoSTO;
	commands["sti"] = &OllyLang::DoSTI;
	commands["sto"] = &OllyLang::DoSTO;
	commands["str"] = &OllyLang::DoSTR;
	commands["sub"] = &OllyLang::DoSUB;
	commands["tc"] = &OllyLang::DoTC;
	commands["test"] = &OllyLang::DoTEST;
	commands["ti"] = &OllyLang::DoTI;
	commands["tick"] = &OllyLang::DoTICK;
	commands["ticnd"] = &OllyLang::DoTICND;
	commands["to"] = &OllyLang::DoTO;
	commands["tocnd"] = &OllyLang::DoTOCND;
	 commands["ubp"] = &OllyLang::DoBP;
	commands["unicode"] = &OllyLang::DoUNICODE;
	commands["var"] = &OllyLang::DoVAR;
	commands["xor"] = &OllyLang::DoXOR;
	commands["xchg"] = &OllyLang::DoXCHG;
	commands["wrt"] = &OllyLang::DoWRT;
	commands["wrta"] = &OllyLang::DoWRTA;

	script_state = SS_INITIALIZED;
	script_pos = 0;
	EOB_row = -1;
	EOE_row = -1;
	zf = 0;
	cf = 0;
	enable_logging = true;
	var_logging = true;

	search_buffer=NULL;

	hwmain = hwndOllyDbg();
	hwndinput = 0;

	require_ollyloop = 0;
	require_addonaction = 0;
	nIgnoreNextValuesHist = 0;

	showVarHistory=true;
	bUnicode=false;

	saved_bp = 0;
	alloc_bp = 0;
	softbp_t = NULL;

	for (int n=0; n < 4; n++ ) hwbp_t[n].addr = 0;

}

OllyLang::~OllyLang()
{
	if (search_buffer!=NULL)
		DoENDSEARCH("");

	FreeBpMem();
	labels.clear();
	script.clear();
	clearProgLines();
	clearLogLines();
	Reset();

}

bool OllyLang::Reset()
{
	freeMemBlocks();
	variables.clear();
	bpjumps.clear();
	calls.clear();
	char s[10] = {0};
	sprintf(s,"%i.%i", VERSIONHI, VERSIONLO);
	string str(s);
	variables["$VERSION"] = str;
	EOB_row = -1;
	EOE_row = -1;
	script_state = SS_LOADED;
	enable_logging = false;
	script_pos = GetFirstCodeLine();
	pgr_scriptpos = script_pos+1; 
	resetProgLines();
	tickcount_startup=0;
	tickcount=0;
	tickcounthi=0;
	break_memaddr=0;
	break_reason=0;
	require_ollyloop = 0;
	require_addonaction = 0;
	nIgnoreNextValuesHist = 0;

	tExportsCache.clear();
	exportsCacheAddr = 0;
	tImportsCache.clear();
	importsCacheAddr = 0;

	dumpWindows.clear();

	if (wndProg.hw!=NULL)
		Selectandscroll(&wndProg,pgr_scriptpos,2);
	return true;
}

vector<string> OllyLang::GetScriptFromFile(LPSTR file)
{
    currentdir = file;
    currentdir = currentdir.substr(0, currentdir.rfind('\\') + 1);
	
	vector<string> script;
	string scriptline;
	ifstream fin(file, ios::in);

	while(getline(fin, scriptline))
		script.push_back(scriptline);

	fin.close();

	return script;
}

int OllyLang::InsertScript(vector<string> toInsert, int posInScript)
{
	vector<string>::iterator iter = toInsert.begin();

	string scriptline, cleanline;
	int inc_pos = -1, linesAdded = 0;
	int p;
	bool comment_todisplay, in_asm=false, in_comment=false;

	while(iter != toInsert.end())
	{
		comment_todisplay=0;
		scriptline = trim(*iter);
		// Handle comments
		if(scriptline == "/*")
		{
			in_comment = true;
			comment_todisplay=1;
			//iter++;
			//continue;
		}
		else if(scriptline == "*/")
		{
			in_comment = false;
			comment_todisplay=1;
			//iter++;
			//continue;
		}
		else if (scriptline.find("/*")==0 && scriptline.find("*/")==string::npos) {
			in_comment = true;
			comment_todisplay=1;
		}
		else if (scriptline.find("*/")==(scriptline.length()-2) ) {
			in_comment = false;
			comment_todisplay=1;
		}
		else if (scriptline.find("/*")!=string::npos && scriptline.find("*/")==string::npos) 
		{
			p=scriptline.find("/*");
			scriptline.erase(p,scriptline.length()-p);
			in_comment = true;
		}
		else if (scriptline.find("*/")!=string::npos && scriptline.find("/*")==string::npos) 
		{
			p=scriptline.find("*/");
			scriptline.erase(0,p+2);
			in_comment = false;
		}
		else if (scriptline.find("/*")!=string::npos && scriptline.find("*/")!=string::npos) 
		{
			p=scriptline.find("/*");
			scriptline.erase(p,scriptline.find("*/")-p+2);
		}
		// Check for comments at the end of rows and erase those
		else if(!in_comment && scriptline.find("//")!=string::npos) 
		{
			cleanline=scriptline;
			p=scriptline.find("//");
			if (scriptline.find("\"")!=string::npos) {

				if (p > scriptline.rfind("\"") || p < scriptline.find("\""))
					scriptline.erase(p,scriptline.length()-p);

			} else
				scriptline.erase(p,scriptline.length()-p);

			if (trim(scriptline) == "") {
				scriptline = cleanline;
				comment_todisplay=1;
			}
		}
		else if(!in_comment && scriptline.find(";")!=string::npos) 
		{
			p=scriptline.find(";");
			if (p==0) {
				comment_todisplay=1;
				scriptline.erase(p,1);
				scriptline = ";" + trim(scriptline);
				while(scriptline.find("\t")!=string::npos)
					scriptline.replace(scriptline.find("\t"),1," ");

			}
			else if (scriptline.find("\"")!=string::npos) {

				if (p > scriptline.rfind("\"") || p < scriptline.find("\""))
					scriptline.erase(p,scriptline.length()-p);

			} else
				scriptline.erase(p,scriptline.length()-p);
		}
		else if(in_comment) //Comment Block, ignored
		{
			comment_todisplay=1;
//			iter++;
//			continue;
		} 
		// ASM Blocks

		scriptline = trim(scriptline);
		if (scriptline=="") {
			comment_todisplay=1;
//			iter++;
//			continue;
		}

		string lcline = scriptline;
		int (*pf)(int)=tolower;
		transform(lcline.begin(), lcline.end(), lcline.begin(), pf);

		// Check for #inc and include file if it exists, else add line
		if((inc_pos = lcline.find("#inc")) != string::npos)
		{
			cout << inc_pos;
			string args = trim(scriptline.substr(inc_pos + 4, scriptline.length()));
			string filename;
			if(GetSTROpValue(args, filename))
			{
				if (filename.find('\\')==string::npos)
					filename = currentdir+filename;
				vector<string> newScript = GetScriptFromFile((LPSTR)filename.c_str());				
				posInScript += InsertScript(newScript, posInScript);

				iter++;
				continue;
			}
			else
			{
				MessageBox(hwmain, "Bad #inc directive!", "OllyScript", MB_OK | MB_ICONERROR | MB_TOPMOST);
			}
		}
		// Logging
		else if((inc_pos = lcline.find("#log")) > -1 && !in_comment)
		{
			enable_logging = true;
		}
		else
		{
			script.insert(script.begin() + posInScript, scriptline);
			linesAdded++;
			posInScript++;

			if(in_asm && lcline == "ende" && !in_comment) {
				in_asm = false;
			}

			addProgLine(posInScript,scriptline,comment_todisplay*PROG_TYPE_COMMENT + in_asm*PROG_TYPE_ASM);

			if(lcline == "exec"  && !in_comment) {
				in_asm = true;
			}

		}
	
		iter++;
	}
	return linesAdded;
}

/*
int OllyLang::GetState()
{
	return script_state;
}
*/

ulong OllyLang::GetFirstCodeLine(ulong from) //=0
{
	ulong nline=from;

	// Check for label after (skip it/them)
	if(nline < script.size()) {
		
		// Check if its a label or comment
		while(nline < script.size() && (!(getProgLineType(nline+1) & PROG_TYPE_COMMAND)))
		{
			nline++;
		} 

	}
	//if (nline>=script.size())
	//	return false;

	return nline;
}

bool OllyLang::SaveBreakPoints(LPSTR fileName) {

	if (strcmp(fileName,"")==0)
		return false;

	Pluginwritestringtoini(hinstModule(), "BP_FILE", fileName);	

	int i,bpcnt=1;
	char* buffer[5]={0};
	string s,sline;

	t_wndprog_data *ppl;
	for (i=1;i<script.size();i++) {
		ppl = (t_wndprog_data *) Getsortedbyselection(&wndProg.data,i);
		if (ppl==NULL)
			return false; 
		if (ppl->pause) {
			s="BP_";
			sprintf((char*)buffer,"%04u",bpcnt);
			s.append((char*)buffer);
			sprintf((char*)buffer,"%u",i);
			sline.assign((char*)buffer);
			sline.append(",");
			sline.append(ppl->command);
			Pluginwritestringtoini(hinstModule(), (char*)s.c_str(), (char*)sline.c_str());			
			bpcnt++;
		}
	}
	//clean last one
	s="BP_";
	sprintf((char*)buffer,"%04u",bpcnt);
	s.append((char*)buffer);
	Pluginwritestringtoini(hinstModule(), (char*)s.c_str(), "");			
	return true;
}

bool OllyLang::LoadBreakPoints(LPSTR fileName) {

	string s;
	char* sbuffer[MAX_PATH]={0};
	Pluginreadstringfromini(hinstModule(), "BP_FILE", (char*)sbuffer, "");
	s.assign((char*)sbuffer);

	if (stricmp(fileName,s.c_str())!=0)
		return false;

	int i,bpcnt=0,p;
	char* ibuffer[5]={0};

	t_wndprog_data *ppl;
	do {
		bpcnt++;
		s="BP_";
		sprintf((char*)ibuffer,"%04u",bpcnt);
		s.append((char*)ibuffer);
		Pluginreadstringfromini(hinstModule(), (char*)s.c_str(), (char*)sbuffer, "");		
		s.assign((char*)sbuffer);
		if (s!="") {

			if ((p=s.find(","))!=string::npos) {
				sscanf(s.substr(0,p).c_str(),"%u",&i);
				s.erase(0,p+1);
				ppl = (t_wndprog_data *) Getsortedbyselection(&wndProg.data,i);
				if (ppl!=NULL) {
					if (s.compare(ppl->command)==0)
						ppl->pause=1; 
				}
			}

		}

	} while (s!="");

	return true;
}

bool OllyLang::LoadScript(LPSTR file)
{

	SaveBreakPoints((char*)scriptpath.c_str());

	script.clear();
	labels.clear();
 	clearProgLines(); 
	Reset();

	scriptpath = file;	
	
	addProgLine(0,scriptpath);

	vector<string> unparsedScript = GetScriptFromFile(file);
	InsertScript(unparsedScript, 0);

	// Find all labels
	ParseLabels();

	Pluginwritestringtoini(hinstModule(), "ScriptDir", file);
	mruAddFile(file);

	pgr_scriptpos=GetFirstCodeLine()+1;
	setProgLineEIP(pgr_scriptpos,0);

	LoadBreakPoints(file);

	return true;
}

bool OllyLang::Pause()
{
	script_state = SS_PAUSED;
	return true;
}

bool OllyLang::Resume()
{
	Pause();
	Step(1);
	script_state = SS_RUNNING;
	return true;
}

bool OllyLang::Step(int forceStep)
{
	require_ollyloop = 0;
	t_dump *cpuasm;
	PFCOMMAND func;
	LARGE_INTEGER i64;
	int refresh=1;
	char lastchar;
	bool jumped;
	int old_pos;

	while(!require_ollyloop && Getstatus() == STAT_STOPPED && (script_state == SS_RUNNING || script_state == SS_LOADED || (forceStep && script_state == SS_PAUSED)))
	{		
		jumped=false;

		if (tickcount_startup==0) {
			i64.QuadPart= MyGetTickCount(0).QuadPart;
			tickcount_startup=i64.QuadPart;
		}

		// Set state to RUNNING
		if(forceStep == 0)
			script_state = SS_RUNNING;

		script_pos=GetFirstCodeLine(script_pos);

		// Check if script out of bounds
		DumpScript();
		if(script.size() <= script_pos)
			return false;

		// Get code line
		string codeLine = trim(script[script_pos]);

		pgr_scriptpos=script_pos+1;
		cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
		setProgLineEIP(pgr_scriptpos,cpuasm->sel0);

		//HANDLE SCRIPT BREAKPOINTS
		if (isProgLineBP(pgr_scriptpos) && script_state!=SS_PAUSED) {
			script_state=SS_PAUSED;
			break;
		}

		// Log line of code if that is enabled
		if(enable_logging)
		{
			char buffer[4096] = {0};
			strcpy(buffer, "--> ");
			strcat(buffer, codeLine.c_str());
			Addtolist(0, -1, buffer);
		}

		// Create command and arguments
		string command = codeLine;
		string args = "";
		int (*pf)(int)=tolower; 
		size_t pos = codeLine.find_first_of(" \t\r\n");
		if(pos != string::npos)
		{
			command = trim(codeLine.substr(0, pos));
			transform(command.begin(), command.end(), command.begin(), pf);
			args = trim(codeLine.substr(pos));

		} else {

			transform(command.begin(), command.end(), command.begin(), pf);
		}

		bool result = false;

		// Find command and execute it
		if(commands.find(command) != commands.end())
		{
		
			// Command found, execute it
			func = commands[command];
			old_pos = script_pos;

			if (tickcount==0 && command=="tick") {
				//for first tick.
				i64 = MyGetTickCount(tickcount_startup);
				tickcount = i64.LowPart;
				tickcounthi = i64.HighPart;
			}

			result = (this->*func)(args);
			i64 = MyGetTickCount(tickcount_startup);
			this->tickcount = i64.LowPart;
			this->tickcounthi = i64.HighPart;

			if (old_pos>script_pos || old_pos+1<script_pos) {
				jumped=true;
			}
 
		}
		else 
		{
			// No such command
			errorstr = "No such command: " + codeLine;
			Pause();
			InvalidateProgWindow();
		}
		if (Getstatus() != STAT_RUNNING)
			pgr_scriptpos=script_pos+1;
		
		// Error in processing, show error and die
		if(!result)
		{
			string message = "Error on line ";
			char buffer[32] = {0};
			message.append(itoa(script_pos + 1, buffer, 10));
			message.append("\n");
			message.append("Text: ");
			message.append(script.at(script_pos));
			message.append("\n");
			setProgLineAttr(script_pos+1,PROG_ATTR_ERROR);
			if(errorstr != "")
			{
				message += errorstr;
			}
			MessageBox(0, message.c_str(), "OllyScript error!", MB_ICONERROR | MB_OK | MB_TOPMOST | MB_SETFOREGROUND);
			errorstr = "";
			script_pos++;
			pgr_scriptpos=script_pos+1;
			Pause();
			InvalidateProgWindow();
			return false;
		} else {
			setProgLineResult(script_pos+1,variables["$RESULT"]);
		}

		if (script_state == SS_LOADED) {
			//After a reset (ret)
			script_state = SS_PAUSED;
			//display start of script
			Selectandscroll(&wndProg,0,2);
			break;
		}
		
		script_pos++;

		//Next code Line
		script_pos=GetFirstCodeLine(script_pos);

		if (jumped) { 
			if(++refresh % 128 == 0) {
				//refresh every xxxx jumps (loop anti freeze)
				if (wndProg.hw) {
					Selectandscroll(&wndProg,pgr_scriptpos,2);
					//refresh only on proc exit
					InvalidateRect(wndProg.hw, NULL, FALSE);
				}
				return true; 
				//will continue after main loop...redraw etc
			}
		}

		//Highlight next instr. not if app running... or if stepping
		if (Getstatus() != STAT_RUNNING || forceStep==1)
			pgr_scriptpos=script_pos+1;

		cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
		setProgLineEIP(pgr_scriptpos,cpuasm->sel0);			
		
		if(forceStep == 1) {
			InvalidateProgWindow();
			break;
		}

	}

	if (wndProg.hw!=NULL) //Visible && Not a RUN command 
		Selectandscroll(&wndProg,pgr_scriptpos,2);
	return true;
}

//Executed after some commands to clean memory or to get something after ollydbg processing
bool OllyLang::ProcessAddonAction()
{

	t_thread* thr = Findthread(Getcputhreadid());	 

	t_dbgmemblock todo={0};

	vector<t_dbgmemblock>::iterator block;
	
	block = tMemBlocks.end();

	for (int b=tMemBlocks.size(); b>0; b--) {
		
		block--;
		if (block->free_at_eip == thr->reg.ip) {
			HANDLE hDbgPrc = (HANDLE)Plugingetvalue(VAL_HPROCESS);
			VirtualFreeEx(hDbgPrc, block->hmem, block->size, MEM_DECOMMIT);
			
			//DbgMsgHex((ulong)block->hmem,"VirtualFreeEx");

			if (block->result_register) {
				variables["$RESULT"] = thr->reg.r[block->reg_to_return];
				setProgLineResult(block->script_pos+1,variables["$RESULT"].dw);
			}

			if (block->listmemory) todo.listmemory=true;
			if (block->restore_registers) todo.restore_registers=true;

			tMemBlocks.erase(block);

			require_addonaction=0;
			//return true;
		}
		if (block == tMemBlocks.begin())
			break;
	}

	if (todo.listmemory) DoREFRESH(""); //Listmemory();	
	if (todo.restore_registers) RestoreRegisters(true);
	
	return true;
}


bool OllyLang::OnBreakpoint(int reason, int details)
{
	break_reason=reason;
	
		//debug hint
		if(reason&&!PP_HWBREAK)
		break_reason=reason;

	if(bInternalBP) {
		//dont process temporary bp (exec/ende Go command)
		bInternalBP=false;
	}
	else if(EOB_row > -1)
	{
		script_pos = GetFirstCodeLine(EOB_row);
		return true;
	}
	else 
	{
		if (reason != PP_EXCEPTION) {
			t_dump* cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
			DWORD addr = cpuasm->sel0;
			if(bpjumps.find(addr) != bpjumps.end())
			{
				if (bpjumps[addr]) {
					script_pos = GetFirstCodeLine(bpjumps[addr] + 1);
					pgr_scriptpos = script_pos + 1;
					setProgLineEIP(pgr_scriptpos,addr);
					Selectandscroll(&wndProg,pgr_scriptpos,2);
					if (script_state == SS_PAUSED) {
						InvalidateProgWindow();
					}
					return true;
				}
			}
			//pgr_scriptpos = script_pos + 1;
			if (pgr_scriptpos_paint!=pgr_scriptpos && script_state == SS_PAUSED) {
				Selectandscroll(&wndProg,pgr_scriptpos,2);
				InvalidateProgWindow();
			}
		}
	}

	return false;
}

bool OllyLang::OnException(DWORD ExceptionCode)
{
	if(EOE_row > -1)
	{
		script_pos = EOE_row;
		return true;
	}
	else
		return false;
}

int OllyLang::getStringOperatorPos(string &ops) {
	
	string cache=ops;
	string operators = "+";
	int b=0,e=0,p=0,c;

	//hide [pointer operations]
	while ( (p=cache.find_first_of("[",p)) != string::npos) {
		if ( (e=cache.find_first_of("]",p)) != string::npos) {
			for (c=p;c<=e;c++) cache[c] = 'X';
		}
	}
	e=0;p=0;

	//Search for operator(s) outside "quotes" 
	while ( (b=cache.find_first_of("\"",b)) != string::npos) {

		//Check Before
		if ((p=cache.find_first_of(operators),e) != string::npos) {
			if (p<b) return p;
		}

		if ( (e=cache.find_first_of("\"",b+1)) != string::npos) {
	
			//Check between
			if ((p=cache.find_first_of(operators,e+1)) != string::npos) {
				if (p<cache.find_first_of("\"",e+1)) return p;
			}
			b=e;
		}
		b++;
	}

	//Check after
	return cache.find_first_of(operators,e);

}

int OllyLang::getDWOperatorPos(string &ops) {
	
	string operators = "+-*/&|^<>";
	int b=0,e=0,p;

	//Search for operator(s) outside [pointers]
	while ( (b=ops.find("[",b)) != string::npos) {

		//Check Before
		if ((p=ops.find_first_of(operators),e) != string::npos) {
			if (p<b) return p;
		}

		if ( (e=ops.find("]",b+1)) != string::npos) {
	
			//Check between
			if ((p=ops.find_first_of(operators,e+1)) != string::npos) {
				if (p<ops.find("[",e+1)) return p;
			}
			b=e;
		}
		b++;
	}

	//Check after
	return ops.find_first_of(operators,e);
}

int OllyLang::getFLTOperatorPos(string &ops) {
	
	string operators = "+-*/";
	int b=0,e=0,p;

	//Search for operator(s) outside [pointers]
	while ( (b=ops.find("[",b)) != string::npos) {

		//Check Before
		if ((p=ops.find_first_of(operators),e) != string::npos) {
			if (p<b) return p;
		}

		if ( (e=ops.find("]",b+1)) != string::npos) {
	
			//Check between
			if ((p=ops.find_first_of(operators,e+1)) != string::npos) {
				if (p<ops.find("[",e+1)) return p;
			}
			b=e;
		}
		b++;
	}

	//Check after
	return ops.find_first_of(operators,e);
}

bool OllyLang::CreateOperands(string &args, string ops[], uint len, bool preferstr)
{

	vector<string> v;
	bool bResult = true;

	if (len==1 || (split(v, args, ',') && v.size() == len))
	{
		if (len==1) v.push_back(args);

		int p;
		DWORD dw,result=0;
		long double flt=0, fltresult=0;
		string str;
		string svar,op,oper="";

		var vResult;

		for(uint i = 0; i < len; i++) 
		{
			op = trim(v[i]);
			ops[i] = op;

			if ((p=getFLTOperatorPos(op)) != string::npos) 
			{
				do {
					if (op.find_first_of("\"") != string::npos || preferstr)
						goto try_str_operation;
					
					svar=trim(op.substr(0,p));

					var_logging=false;
					if (!GetFLTOpValue(svar,flt)) 
						//Convert integer to float (but not for first operand)
						if (oper.length()!=0 && GetDWOpValue(svar,dw))
							flt = dw;
						else
							goto try_dw_operation;

					if (oper.length()==0)
						//first value
						fltresult += flt;
					else if (oper[0]=='+')
						fltresult += flt;
					else if (oper[0]=='-')
						fltresult -= flt;
					else if (oper[0]=='*')
						fltresult *= flt;
					else if (oper[0]=='/')
						fltresult /= flt;
			
					oper = op[p];

					op.erase(0,p+1);

				} while ( (p=getFLTOperatorPos(op)) != string::npos);

				svar=trim(op);
				var_logging=false;
				if (!GetFLTOpValue(svar,flt)) 
					//Convert integer to float (but not for first operand)
					if (oper.length()!=0 && GetDWOpValue(svar,dw))
						flt = dw;
					else
						goto operation_failed;

				if (oper[0]=='+')
					fltresult += flt;
				else if (oper[0]=='-')
					fltresult -= flt;
				else if (oper[0]=='*')
					fltresult *= flt;
				else if (oper[0]=='/')
					fltresult /= flt;

				char value[128];
				sprintf(value,"%lf",fltresult);

				ops[i]=value;
				while (true) 
				{
					if (ops[i].length() <= 3) break; 
					if (ops[i][ops[i].length()-1] == '0')
						ops[i].erase(ops[i].length()-1,1);
					else 
						break;
				}
				if (nIgnoreNextValuesHist==0)
					var_logging=true;
//				if (!sub_operand)
					setProgLineValueFloat(script_pos+1,fltresult);
				goto operation_ok;

			} 
			else if ( (p=getDWOperatorPos(op)) != string::npos)
			{	
				do {
			
			try_dw_operation:

					if (op.find_first_of("\"") != string::npos || preferstr)
						goto try_str_operation;
					
					svar=trim(op.substr(0,p));

					var_logging=false;
					if (!GetDWOpValue(svar,dw)) 
						goto try_str_operation;

					if (oper.length()==0)
						//first value
						result += dw;
					else if (oper[0]=='+')
						result += dw;
					else if (oper[0]=='-')
						result -= dw;
					else if (oper[0]=='*')
						result *= dw;
					else if (oper[0]=='/')
						result /= dw;
					else if (oper[0]=='&')
						result &= dw;
					else if (oper[0]=='|')
						result |= dw;
					else if (oper[0]=='^')
						result ^= dw;
					else if (oper[0]=='>')
						result >>= dw;
					else if (oper[0]=='<')
						result <<= dw;
			
					oper = op[p];

					op.erase(0,p+1);

				} while ( (p=getDWOperatorPos(op)) != string::npos);

				svar=trim(op);
				var_logging=false;
				if (!GetDWOpValue(svar,dw)) 
					goto operation_failed;

				if (oper[0]=='+')
					result += dw;
				else if (oper[0]=='-')
					result -= dw;
				else if (oper[0]=='*')
					result *= dw;
				else if (oper[0]=='/')
					result /= dw;
				else if (oper[0]=='&')
					result &= dw;
				else if (oper[0]=='|')
					result |= dw;
				else if (oper[0]=='^')
					result ^= dw;
				else if (oper[0]=='>')
					result >>= dw;
				else if (oper[0]=='<')
					result <<= dw;

				char value[16];
				itoa(result,value,16);

				ops[i]=value;

				if (nIgnoreNextValuesHist==0) {
					var_logging=true;
//				if (!sub_operand)
					setProgLineValue(script_pos+1,result);
				}
				goto operation_ok;

			//STRING OPERATIONS
			try_str_operation:
				
				oper="";
				while ((p = getStringOperatorPos(op)) != string::npos) 
				{
					
					svar=trim(op.substr(0,p));
					var_logging=false;
					if (!GetSTROpValue(svar,str)) 
						goto operation_failed;

					if (oper.length()==0)
						//first value
						vResult = str;
					else if (oper[0]=='+')
						vResult += str; 

					oper = op[p];

					op.erase(0,p+1);
				} 

				svar=trim(op);

				var_logging=false;
				if (!GetSTROpValue(svar,str)) 
					goto operation_failed;
				
				if (oper.length()==0)
					//first value
					vResult = str;
				if (oper[0]=='+')
					vResult += str;

				if (!vResult.isbuf)
					ops[i]="\""+vResult.str+"\"";
				else
					ops[i]=vResult.str;

				if (!sub_operand && var_logging) 
					setProgLineValue(script_pos+1,ops[i]);
				goto operation_ok;

			operation_failed:
				bResult = false;
				break;

			operation_ok:
				bResult = bResult;

			} //if find_first_of

		} //for

		if (nIgnoreNextValuesHist>0)
			nIgnoreNextValuesHist--;
		if (nIgnoreNextValuesHist==0)
			var_logging=true;

		return bResult;
	}
	return false;
}

bool OllyLang::CreateOp(string &args, string ops[], uint len, bool preferstr)
{
	vector<string> v;
	if (len==1 || (split(v, args, ',') && v.size() == len))
	{
		if (len==1) v.push_back(args);

		int p;
		DWORD dw,result=0;
		long double flt=0, fltresult=0;
		string str,vResult="";
		string op,oper="";

		for(uint i = 0; i < len; i++)
		{
			op = trim(v[i]);
			ops[i] = op;
		}
		return true;
	}
    return false;
}

bool OllyLang::ParseLabels()
{
	vector<string>::iterator iter;
	iter = script.begin();
	string s;
	int loc = 0;
	t_wndprog_data *ppl;

	while(iter != script.end())
	{
		s = *iter;
		if(s.length() > 0) {
			if(s.at(s.length() - 1) == ':') {

				ppl = (t_wndprog_data *) Getsortedbyselection(&wndProg.data,loc+1);
				if (ppl->type != PROG_TYPE_COMMENT) {
					ppl->type = PROG_TYPE_LABEL;
					labels.insert(pair<string, int>(s.substr(0, s.length() - 1), loc));
				}
			}
		}
		iter++;
		loc++;
	}

	return false;
}

int OllyLang::SearchInScript(string text, ulong fromPos)
{
	vector<string>::iterator iter;
	string s;
	int loc = 0;
	bool oneTime = true;

	int (*pf)(int) = tolower; 
	transform(text.begin(), text.end(), text.begin(), pf);

	SearchFromStart:
	iter = script.begin();

	while(iter != script.end())
	{
		if (loc >= fromPos) {
			s = *iter;
			transform(s.begin(), s.end(), s.begin(), pf);
			if(s.length() > 0) {
				if(s.find(text) != string::npos) {
					return loc;
				}
			}
		}
		iter++;
		loc++;
	}
	if (fromPos > 0 && oneTime) {
		fromPos = 0; loc = 0;
		oneTime = false;
		goto SearchFromStart;
	}

	return -1;
}

bool OllyLang::AddBPJump(int bpaddr,int labelpos)
{
	map<int,int>::iterator iter;
	pair<int,int> p;
	iter = bpjumps.begin();
	int loc = 0;

	while(iter != bpjumps.end())
	{
		p = *iter;
		if(p.first == bpaddr) {
			p.second=labelpos;
			return true;
		}
		iter++;
		loc++;
	}

	bpjumps.insert(pair<int, int>(bpaddr,labelpos));

	return true;
}

bool OllyLang::GetANYOpValue(string op, string &value, bool hex8forExec)
{
	if(is_variable(op))
	{
		// We have a declared variable
		if(variables[op].vt == STR)
		{
			// It's a string var, return value
			value = variables[op].str;
			goto values_ok_str;
		}
		else if(variables[op].vt == DW)
		{
			char buffer[12] = {0};
			if (hex8forExec)
				//For Assemble Command (EXEC/ENDE) ie "0DEADBEEF"
				sprintf(buffer, "%09X", variables[op].dw);
			else 
				sprintf(buffer, "%X", variables[op].dw);
			value = buffer;
			goto values_ok;
		}
		return false;
	} 
	if(is_hex(op))
	{
		if (hex8forExec)
			value = "0"+op;
		else
			value = op;
		return true;
	}
	else if(UnquoteString(op, '"', '"')) 
	{
		value = op;
		return true;
	}
	else if(UnquoteString(op, '#', '#')) 
	{
		value = "#"+op+"#";
		return true;
	}
	else if(is_dec(op))
	{
		op.erase(op.length()-1,1);
		Int2Hex(strtoul(op.c_str(), 0, 10),value);
		return true;
	}
	else if(is_float(op))
	{
		value = op;
		return true;
	}
	else if(UnquoteString(op, '[', ']'))
	{
		// We read a string (max 256) at memory address
		DWORD addr = 0;
		sub_operand=true;
		if (!CreateOp(op,&op,1)) {
			DbgMsg(0,"failed to create pointer suboperand "+op);
			sub_operand=false;
			return false;
		}
		sub_operand=false;

		if(GetDWOpValue(op, addr)) 
		{
			if (addr!=0) {

				//Pointer Mark in Values column
				setProgLineValue(script_pos+1,(string) "®");
				
				char buffer[STRING_READSIZE] = {0};
				Readmemory(&buffer[0], addr, STRING_READSIZE, MM_SILENT);

				if (strlen(buffer) >= STRING_READSIZE-1) 
				{
					buffer[STRING_READSIZE-1] = 0;
				}

				value.assign(buffer);
			} else 
				value = "0";

			goto values_ok_str;
		}
		
	}
	else
	{
		DWORD dw=0;
		if (GetDWOpValue(op,dw)) 
		{
			char buffer[12] = {0};
			if (hex8forExec)
				sprintf(buffer, "%09X", dw);
			else 
				sprintf(buffer, "%X", dw);
			value = buffer;
			goto values_ok;
		}
	}
	return false;

values_ok:
	if (var_logging)
		setProgLineValue(script_pos+1,value);
	return true;

values_ok_str:
	if (var_logging) {
		var val=value;
		if(val.isbuf) 
			setProgLineValue(script_pos+1,value);
		else
			setProgLineValue(script_pos+1,"\""+value+"\"");
	}
	return true;

}

bool OllyLang::GetSTROpValue(string op, string &value, int size)
{
//	if (size>0)
//		DbgMsg(size,op);

	if(is_variable(op))
	{
		// We have a declared variable
		if(variables[op].vt == STR)
		{
			// It's a string var, return value
			if (size<variables[op].size && (size != 0))
				value = variables[op].strbuff().substr(0,size);
			else
				value = variables[op].str;
			goto values_ok2;
		}
		return false;
	}
	else if(UnquoteString(op, '"', '"')) 
	{
		if (size<op.length() && (size != 0))
			value = op.substr(0,size);
		else
			value = op;
		return true; 
	}
	else if(UnquoteString(op, '#', '#')) 
	{
		if ((size*2)<op.length() && (size != 0))
			value = "#"+op.substr(0,size*2)+"#";
		else
			value = "#"+op+"#";
		return true;
	}
	else if(UnquoteString(op, '[', ']'))
	{
		// We read a string (max STRING_READSIZE) at memory address
		DWORD addr = 0;

		sub_operand=true;
		if (!CreateOp(op,&op,1)) {
			DbgMsg(0,"failed to create pointer suboperand "+op);
			sub_operand=false;
			return false;
		}

		sub_operand=false;

		if(GetDWOpValue(op, addr)) {

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "®");

			if (size>0) {
				
				char* buffer = new char[size+1];
				Readmemory(buffer, addr, size, MM_SILENT);
				buffer[size] = 0;
				value.assign(buffer,size);
				if (strlen(buffer) != size) {
					var v=value;
					value="#"+v.strbuffhex()+"#";
				}
				delete[] buffer;

			} else {
				char buffer[STRING_READSIZE] = {0};
				Readmemory(buffer, addr, STRING_READSIZE, MM_SILENT);
			
				if (strlen(buffer) >= STRING_READSIZE-1) {
					buffer[STRING_READSIZE-1] = 0;
				}
				value = buffer;
			}

			goto values_ok2;

		}
		
	}
	return false;

values_ok2:
	if (var_logging) {
		var val=value;
		if(val.isbuf) 
			setProgLineValue(script_pos+1,value);
		else
			setProgLineValue(script_pos+1,"\""+value+"\"");
	}
	return true;
}

int OllyLang::GetRegNr(string& name)
{
	string r16=name;

	//R16
	if (r16.length()==3)
		r16.erase(0,1);

	//R8
	if (r16[1] == 'l' || r16[1] == 'h')	r16[1]='x';

	if(r16 == "ax")
		return REG_EAX;
	else if(r16 == "bx")
		return REG_EBX;
	else if(r16 == "cx")
		return REG_ECX;
	else if(r16 == "dx")
		return REG_EDX;
	else if(r16 == "sp")
		return REG_ESP;
	else if(r16 == "bp")
		return REG_EBP;
	else if(r16 == "si")
		return REG_ESI;
	else if(r16 == "di")
		return REG_EDI;
	else 
		return -1;
}

bool OllyLang::GetBYTEOpValue(string op, BYTE &value)   //new add
{
	if(is_hex(op))
	{
		op = "0" + op;
		if (op.length() > 3) {
			errorstr = "GetBYTEOpValue : Too Long";
			return false;
		}
		value = strtoul(op.c_str(), 0, 16);
		if (var_logging)
			setProgLineValue(script_pos+1,value);
		return true;
	}
	else if(is_variable(op))
	{
		if (variables[op].dw > 0xff){
            errorstr = "GetBYTEOpValue : Too High";
			return false;
			}
		// We have a declared variable
		if(variables[op].vt == DW)
		{
			// It's a DWORD var, return value
			value = variables[op].dw;            
			if (var_logging)
				setProgLineValue(script_pos+1,value);
			return true;
		}
		return false;
	}
  return false;
}

bool OllyLang::GetAddrOpValue(string op, DWORD &value)
{
		if(UnquoteString(op, '[', ']'))
		{
		// We have a memory address
			DWORD addr = 0;
//			sub_operand=true;
				if (!CreateOperands(op,&op,1)) {
					DbgMsg(0,"GetAddrOpValue "+op);
					sub_operand=false;
					return false;
				}
//			sub_operand=false;
            value=strtoul(op.c_str(),0,16);
			
			return true;
		}
        return true;
}

bool OllyLang::GetDWOpValue(string op, DWORD &value, DWORD default_val)
{
	value=default_val;

	if(is_register(op))
	{
		// We have a register
		int regnr = GetRegNr(op);

		t_thread* pt = Findthread(Getcputhreadid());
		
		if(regnr != -1)
		{
			// It's a usual register
			value = pt->reg.r[regnr];
			if (op.length()==2) {
				if (op[1] == 'l')
					value &= 0xFF;
				else if (op[1] == 'h') {
					value &= 0xFF00;
					value /= 0x100;
				} else
					value &= 0xFFFF;
			}

			if (var_logging)
				setProgLineValue(script_pos+1,value);
			return true;
		}
		else if(op == "eip")
		{
			//cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
			//value = cpuasm->sel0;
			// It's EIP
			value = pt->reg.ip;
			if (var_logging)
				setProgLineValue(script_pos+1,value);
			return true;
		}
		return false;
	}
	else if(is_flag(op))
	{
		eflags flags;
		ZeroMemory(&flags, sizeof DWORD);
		t_thread* pt = Findthread(Getcputhreadid());
        flags.dwFlags = pt->reg.flags;

		if(stricmp(op.c_str(), "!af") == 0)
			value = flags.bitFlags.AF;
		else if(stricmp(op.c_str(), "!cf") == 0)
			value = flags.bitFlags.CF;
		else if(stricmp(op.c_str(), "!df") == 0)
			value = flags.bitFlags.DF;
		else if(stricmp(op.c_str(), "!of") == 0)
			value = flags.bitFlags.OF;
		else if(stricmp(op.c_str(), "!pf") == 0)
			value = flags.bitFlags.PF;
		else if(stricmp(op.c_str(), "!sf") == 0)
			value = flags.bitFlags.SF;
		else if(stricmp(op.c_str(), "!zf") == 0)
			value = flags.bitFlags.ZF;

		if (var_logging)
			setProgLineValue(script_pos+1,value);
		
		return true;
	}
	else if(is_variable(op))
	{
		// We have a declared variable
		if(variables[op].vt == DW)
		{
			// It's a DWORD var, return value
			value = variables[op].dw;
			if (var_logging)
				setProgLineValue(script_pos+1,value);
			return true;
		}
		return false;
	}
	else if(is_hex(op))
	{
		// We have a HEX constant
//		op = "0" + op;
		value = strtoul(op.c_str(), 0, 16);
		return true;
	}
	else if(is_dec(op))
	{
		// We have a DEC constant
		op.erase(op.length()-1,1);
		value = strtoul(op.c_str(), 0, 10);
		return true;
	}
	else if(UnquoteString(op, '[', ']'))
	{
		// We have a memory address
		DWORD addr = 0;
		sub_operand=true;
		if (!CreateOperands(op,&op,1)) {
			DbgMsg(0,"failed to create pointer suboperand "+op);
			sub_operand=false;
			return false;
		}
		sub_operand=false;

		if(GetDWOpValue(op, addr)) {

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "®");

			Readmemory(&value, addr, 4, MM_SILENT);
			if (var_logging)
				setProgLineValue(script_pos+1,value);
			return true;

		}
	}
	return false;
}

bool OllyLang::GetFLTOpValue(string op, long double &value)
{

	if(is_float(op))
	{
		value = strtof(op);
		return true;
	}
	else if(is_floatreg(op))
	{
		t_thread* pt = Findthread(Getcputhreadid());
		value =  pt->reg.f[(op[3]-0x30)];
		if (var_logging)
			setProgLineValueFloat(script_pos+1,value);
		return true;
	}
	else if(is_variable(op))
	{
		// We have a declared variable
		if(variables[op].vt == FLT)
		{
			// It's a DWORD var, return value
			value = variables[op].flt;

			if (var_logging)
				setProgLineValueFloat(script_pos+1,value);
			return true;
		}
		return false;
	}
	else if(UnquoteString(op, '[', ']'))
	{
		// We have a memory address
		DWORD addr = 0;
		sub_operand=true;
		if (!CreateOp(op,&op,1)) {
			DbgMsg(0,"failed to create pointer suboperand "+op);
			sub_operand=false;
			return false;
		}
		sub_operand=false;

		if(GetDWOpValue(op, addr)) {

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "®");

			Readmemory(&value, addr, 8, MM_SILENT);
			if (var_logging)
				setProgLineValueFloat(script_pos+1,value);
			return true;

		}
	}
	return false;
}

bool OllyLang::is_register(string s)
{
	int (*pf)(int) = tolower; 
	transform(s.begin(), s.end(), s.begin(), pf);

	if(reg_names.find(s) != reg_names.end())
		return true;

	return false;
}

bool OllyLang::is_floatreg(string s)
{
	int (*pf)(int) = tolower; 
	transform(s.begin(), s.end(), s.begin(), pf);

	if(s.length()==5)
		if (s[0]=='s' && s[1]=='t' && s[2]=='(' && s[4]==')') 
			if (s[3] >= '0' && s[3] <= '7') 
				return true;

	return false;
}

bool OllyLang::is_flag(string s)
{
	int (*pf)(int) = toupper; 
	transform(s.begin(), s.end(), s.begin(), pf);

	if(flag_names.find(s) != flag_names.end())
		return true;
	return false;
}

bool OllyLang::is_variable(string &s)
{
	if(variables.find(s) != variables.end())
		return true;
	return false;
}

string OllyLang::ResolveVarsForExec(string in, bool hex8forExec)
{
	string out, varname;
	bool in_var = false;
	in = trim(in);

	for(int i = 0; i < in.length(); i++)
	{
		if(in[i] == '{')
		{
			in_var = true;
		}
		else if(in[i] == '}')
		{
			in_var = false;
			GetANYOpValue(varname, varname, hex8forExec);
			out += varname;
			varname = "";
		}
		else
		{
			if(in_var)
				varname += in[i];
			else
				out += in[i];
		}
	}				
	return out;

}

void OllyLang::menuListLabels(HMENU mLabels,int cmdIndex) {

	map<string, int>::iterator iter;
	iter = labels.begin();
	
	vector<HMENU>::iterator imenu;

	char buffer[32];
	string str;

	pair<string, int> p;
	while(iter != labels.end())
	{
		p = *iter;

		AppendMenu(mLabels,MF_STRING,cmdIndex+p.second,p.first.c_str());		
		iter++;
		//cmdIndex++;
	}
}

void OllyLang::menuListVariables(HMENU mVars,int cmdIndex) {

	map<string, var>::iterator iter;
	iter = variables.begin();
	
	vector<HMENU> Popups;
	vector<HMENU>::iterator imenu;
	HMENU menu;

	char buffer[32];
	string str;

	pair<string, var> p;
	while(iter != variables.end())
	{
		p = *iter;
		menu = CreateMenu();
		Popups.push_back(menu);
		AppendMenu(mVars,MF_POPUP,(DWORD) menu,p.first.c_str());
		if (p.second.vt == STR) {

			if (p.second.size < 50) {
				AppendMenu(menu,MF_STRING,cmdIndex,("\""+p.second.strclean()+"\"").c_str());
				AppendMenu(menu,MF_STRING,cmdIndex,("#"+p.second.strbuffhex()+"#").c_str());
			} else {
				AppendMenu(menu,MF_STRING,cmdIndex,("\""+p.second.strclean().substr(0,50)+"...").c_str());
				AppendMenu(menu,MF_STRING,cmdIndex,("#"+p.second.strbuffhex().substr(0,50)+"...").c_str());
			}
			strcpy(buffer,"Length: ");
			itoa(p.second.size,&buffer[8],10);
			strcat(buffer,".");
			AppendMenu(menu,MF_STRING,cmdIndex,buffer);

		} else if (p.second.vt == FLT) {
		
			sprintf(buffer,"%.2f",p.second.flt);
			AppendMenu(menu,MF_STRING,cmdIndex,buffer);
		
		} else {

			strcpy(buffer,"0x");
			itoa(p.second.dw,&buffer[2],16);
			AppendMenu(menu,MF_STRING,cmdIndex,buffer);
			itoa(p.second.dw,buffer,10);
			strcat(buffer,".");
			AppendMenu(menu,MF_STRING,cmdIndex,buffer);
		}
		
		iter++;
		cmdIndex++;
	}
	imenu = Popups.begin();
	while(imenu != Popups.end())
	{
		DestroyMenu(*imenu);
		imenu++;
	}
}

bool OllyLang::editVariable(int nVar) {

	map<string, var>::iterator iter;
	pair<string, var> p;
	int n=0;
	iter = variables.begin();

	while(iter != variables.end()) {
		if (n==nVar) {

			RECT Rect={0};
			if (wndProg.hw)	GetWindowRect(wndProg.hw,&Rect);
			
			p = *iter;
			if (p.second.vt == STR) {
				
				t_hexstr hexbuf;
				hexbuf.n=min(p.second.size,TEXTLEN);
				ZeroMemory(hexbuf.data,TEXTLEN);
				ZeroMemory(hexbuf.mask,TEXTLEN);
				FillMemory(hexbuf.mask,hexbuf.n,0xFF);
				memcpy(hexbuf.data,p.second.strbuff().c_str(),hexbuf.n);
				int font = Plugingetvalue(VAL_WINDOWFONT);

				if (Gethexstringxy("Edit variable...",&hexbuf, DIA_DEFHEX, font, 0, Rect.left,Rect.bottom)!=-1) {
					string s;
					s.assign((char*) hexbuf.data,hexbuf.n);

					if (variables[p.first].isbuf) {
						var v = s;
						variables[p.first] = "#"+v.strbuffhex()+"#";
					} else {
						variables[p.first] = s;
					}
				}

			} else if (p.second.vt == DW) {

				Getlongxy("Edit variable...",&p.second.dw, 4, 0, 0, Rect.left,Rect.bottom);
				variables[p.first] = p.second.dw;

			} else if (p.second.vt == FLT) {

				Getfloatxy("Edit variable...",&p.second.flt, 8, 0, 0, Rect.left,Rect.bottom);
				variables[p.first] = p.second.flt;
			}
			
			return true;
		}
		iter++;
		n++;
	}
	return false;
}


void OllyLang::execCommand() {
	char buffer[TEXTLEN]={0};
	string codeLine;
	RECT Rect={0};
	if (&wndProg.hw)	GetWindowRect(wndProg.hw,&Rect);
	int font = Plugingetvalue(VAL_WINDOWFONT);
	if (Gettextxy("Execute Script Command...",buffer, 0, 201, font, Rect.left,Rect.bottom)!=-1) {

		codeLine = buffer;
	
		// Create command and arguments
		string command = codeLine;
		string args = "";
		size_t pos = codeLine.find_first_of(" \t\r\n");
		if(pos != -1)
		{
			command = trim(codeLine.substr(0, pos));
			int (*pf)(int)=tolower; 
			transform(command.begin(), command.end(), command.begin(), pf);
			args = trim(codeLine.substr(pos));
		}

		bool result = false;

		// Find command and execute it
		if(commands.find(command) != commands.end())
		{			
			// Command found, execute it
			PFCOMMAND func = commands[command];
			result = (this->*func)(args);
		}
		else 
		{
			// No such command
			errorstr = "No such command: " + codeLine;
		}

		// Error in processing, show error and die
		if(!result)
		{
			string message = "Error";
			message.append("\n");
			message.append("Text: ");
			message.append(codeLine);
			message.append("\n");
			if(errorstr != "")
			{
				message.append(errorstr);
				message.append("\n");
			}
			MessageBox(hwmain, message.c_str(), "OllyScript error!", MB_ICONERROR | MB_OK | MB_TOPMOST);
			errorstr = "";
		}
	}
}

//modify script execution cursor
bool OllyLang::jumpToLine(int number) {
	script_pos = number-1;
	pgr_scriptpos = number;
	return true;
}

void OllyLang::DumpLabels()
{
	cerr << "_ LABELS _" << endl;
	map<string, int>::iterator iter;
	iter = labels.begin();

	pair<string, int> p;
	while(iter != labels.end())
	{
		p = *iter;

		cerr << "Name: " << p.first << ", ";
		cerr << "Row: " << p.second << endl;
		iter++;
	}
	cerr << endl;
}

void OllyLang::DumpBPJumps()
{
	cerr << "_ LABELS _" << endl;
	map<int, int>::iterator iter;
	iter = bpjumps.begin();

	pair<int, int> p;
	while(iter != bpjumps.end())
	{
		p = *iter;

		cerr << "Addr: " << hex << p.first << ", ";
		cerr << "Row: " << p.second << endl;
		iter++;
	}
	cerr << endl;
}

void OllyLang::DumpVars()
{
	cerr << "_ VARS _" << endl;
	map<string, var>::iterator iter;
	iter = variables.begin();

	pair<string, var> p;
	while(iter != variables.end())
	{
		p = *iter;

		cerr << "Name: " << p.first << ", ";
		cerr << "Value: " << hex << p.second.dw << endl;
		iter++;
	}
	cerr << endl;
}

void OllyLang::DumpScript()
{
	cerr << "_ SCRIPT _" << endl;
	vector<string>::iterator iter;
	iter = script.begin();

	while(iter != script.end())
	{
		cerr << "--> " << *iter << endl;
		iter++;
	}
	cerr << endl;
}

void OllyLang::DropVariable(string var)
{
	if (variables.find(var) != variables.end())
		variables.erase(variables.find(var));
}

//Add zero char before dw values, ex: 0DEADBEEF (to be assembled) usefull if first char is letter
string OllyLang::FormatAsmDwords(string asmLine)
{
		// Create command and arguments
 		string command = asmLine;
		string arg, sSep, args = "";
		size_t pos = asmLine.find_first_of(" \t\r\n");
		DWORD nbArgs=0;
		if(pos != string::npos)
		{
			command = trim(asmLine.substr(0, pos));
			args = trim(asmLine.substr(pos));

		} else {
			//no args
			return asmLine;
		}

		command+=" ";

		while ((pos = args.find_first_of("+,[")) != string::npos)
		{
			arg=trim(args.substr(0, pos));
			ForLastArg:
			if (sSep == "[") {
				if (arg.substr(arg.size()-1,1) == "]") {
					arg = arg.substr(0,arg.size()-1);
					if (is_hex(arg) && arg.size()>0) {
						if (arg.substr(0,1).find_first_of("abcdefABCDEF") != string::npos)
							arg="0"+arg;
					}
					arg = arg + "]";
				}
			} else {
				if (is_hex(arg) && arg.size()>0) {
					if (arg.substr(0,1).find_first_of("abcdefABCDEF") != string::npos)
						arg="0"+arg;
				}
			}

//		if (nbArgs)	
			command.append(sSep);
			command.append(arg);

			nbArgs++;
			if (args!="") {
				sSep=args.substr(pos,1);
				args.erase(0,pos+1);
			}
		}

		args=trim(args);
		if (args!="") { 
			arg=args;
			args="";
			goto ForLastArg;
		}

		return trim(command);
}

DWORD OllyLang::AddProcessMemoryBloc(string data, int mode)
{
	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);
	DWORD pmem;

	// Allocate memory for data
	pmem = (DWORD) VirtualAllocEx(hDebugee, NULL, data.size(), MEM_COMMIT, mode);

	return pmem;
}

DWORD OllyLang::AddProcessMemoryBloc(int size, int mode)
{
	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);
	DWORD pmem;

	// Allocate memory for data
	pmem = (DWORD) VirtualAllocEx(hDebugee, NULL, size, MEM_COMMIT, mode);

	return pmem;
}

bool OllyLang::DelProcessMemoryBloc(DWORD address)
{
	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);

	t_memory* tmem = Findmemory(address);

	if (tmem!=NULL) {
		VirtualFreeEx(hDebugee, (void*) tmem->base, tmem->size, MEM_DECOMMIT);
		return true;
	}

	return false;
}

bool OllyLang::ExecuteASM(string command)
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
	t_thread* thr = Findthread(Getcputhreadid());
	eip = thr->reg.ip;

	HANDLE hDebugee = (HANDLE)Plugingetvalue(VAL_HPROCESS);

	// Allocate memory for code
	DWORD pmemexec = (DWORD) VirtualAllocEx(hDebugee, NULL, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

	// Assemble and write code to memory (everything between EXEC and ENDE)

	tmp = ResolveVarsForExec(command,true);
	sprintf(buffer, tmp.c_str());
	len = Assemble(buffer, pmemexec + totallen, &model, 0, 0, error);
	if (len < 0) {
		errorstr = tmp+"\n"+error;
		return false;
	} else {
		WriteProcessMemory(hDebugee, (LPVOID)(pmemexec + totallen), model.code, len, 0);
		totallen += len;
	}

	// Assemble and write jump to original EIP
	sprintf(buffer, "JMP %lX", eip);
	len = Assemble(buffer, pmemexec + totallen, &model, 0, 0, error);
	if (len < 0) {
		errorstr = error;
	} else {
		WriteProcessMemory(hDebugee, (LPVOID)(pmemexec + totallen), model.code, len, 0);
	}

	// Set new eip and run to the original one
	thr->reg.ip = pmemexec;
	thr->reg.modified = 1;
	thr->regvalid = 1;
	Broadcast(WM_USER_CHREG, 0, 0);
	Go(Getcputhreadid(), eip, STEP_RUN, 0, 1);
	//DbgMsgHex(pmemexec);

	// Free memory block after next ollyloop
	t_dbgmemblock block={0};
	block.hmem = (void *) pmemexec;
	block.size = 0x1000;
	block.script_pos = script_pos;
	block.free_at_eip = eip;
	block.autoclean = true;

	regBlockToFree(block);
	require_addonaction = 1;
	require_ollyloop = 1;

	return true;
}


bool OllyLang::isCommand(string cmd)
{
	return (commands.find(cmd) != commands.end());
}

bool OllyLang::callCommand(string cmd, string args)
{
	if(commands.find(cmd) != commands.end())
	{
		// Command found, execute it
		PFCOMMAND func = commands[cmd];

		return (this->*func)(args);
	}
	return false;
}

void OllyLang::regBlockToFree(t_dbgmemblock &block)
{
	tMemBlocks.push_back(block);
}

void OllyLang::regBlockToFree(void * hMem, ulong size, bool autoclean)
{
	t_dbgmemblock block={0};
	block.hmem = hMem;
	block.size = size;	
	block.autoclean = autoclean; 
	block.script_pos = script_pos;
	block.restore_registers = 0;
	block.listmemory = 0;

	tMemBlocks.push_back(block);
}

bool OllyLang::unregMemBlock(void * hMem)
{
	vector<t_dbgmemblock>::iterator block = tMemBlocks.end();
	for (int b=tMemBlocks.size(); b>0; b--) {
		block--;		
		if (block->hmem == hMem) {
			tMemBlocks.erase(block);
			return true;
		}
	}
	return false;
}


bool OllyLang::freeMemBlocks()
{
	if (!tMemBlocks.empty()) {

		HANDLE hDbgPrc = (HANDLE) Plugingetvalue(VAL_HPROCESS);

		vector<t_dbgmemblock>::iterator block = tMemBlocks.end();
		while (tMemBlocks.size() > 0) {
			block--;
			if (block->autoclean)
				VirtualFreeEx(hDbgPrc,block->hmem,block->size,MEM_DECOMMIT);
			tMemBlocks.pop_back();
		}

		tMemBlocks.clear();
		return true;
	}
	return false;
}

bool OllyLang::SaveRegisters(bool stackToo)
{

	t_thread* pt = Findthread(Getcputhreadid());			

	reg_backup.eax = pt->reg.r[REG_EAX];
	reg_backup.ebx = pt->reg.r[REG_EBX];
	reg_backup.ecx = pt->reg.r[REG_ECX];
	reg_backup.edx = pt->reg.r[REG_EDX];
	reg_backup.esi = pt->reg.r[REG_ESI];
	reg_backup.edi = pt->reg.r[REG_EDI];

	if (stackToo) {
		reg_backup.esp = pt->reg.r[REG_ESP];
		reg_backup.ebp = pt->reg.r[REG_EBP];
	}

	reg_backup.dwFlags = pt->reg.flags;

	reg_backup.eip = pt->reg.ip;

	reg_backup.threadid = pt->reg.threadid;
	reg_backup.loaded = script_pos;

	return true;
}

bool OllyLang::RestoreRegisters(bool stackToo)
{
	if (!reg_backup.loaded)
		return false;

	t_thread* pt = Findthread(Getcputhreadid());			

	if (pt->reg.threadid != reg_backup.threadid)
		return false;

	pt->reg.r[REG_EAX] = reg_backup.eax;
	pt->reg.r[REG_EBX] = reg_backup.ebx;
	pt->reg.r[REG_ECX] = reg_backup.ecx;
	pt->reg.r[REG_EDX] = reg_backup.edx;
	pt->reg.r[REG_ESI] = reg_backup.esi;
	pt->reg.r[REG_EDI] = reg_backup.edi;

	if (stackToo) {
		pt->reg.r[REG_ESP] = reg_backup.esp;
		pt->reg.r[REG_EBP] = reg_backup.ebp;
	}	

	pt->reg.flags = reg_backup.dwFlags;

	pt->reg.modified = 1;
	pt->regvalid = 1;
	Broadcast(WM_USER_CHREG, 0, 0);

	return true;
}

bool OllyLang::AllocSwbpMem(uint tmpSizet)
{	
	if (tmpSizet==0) {
		FreeBpMem();
		return true;
	}

	if (softbp_t && tmpSizet < alloc_bp)
		return true;

	try
	{
		if (softbp_t)
			softbp_t = (t_bpoint*)realloc((void *)softbp_t, sizeof(t_bpoint*) * tmpSizet);
		else
			softbp_t = (t_bpoint*)malloc(sizeof(t_bpoint*) * tmpSizet);
		alloc_bp = tmpSizet;
	}
	catch( ... )
	{
		return false;
	}
	return true;
}

void OllyLang::FreeBpMem()
{
	if (softbp_t) {
		free((void *) softbp_t);
		softbp_t = NULL;
	}
	saved_bp = 0;
	alloc_bp = 0;
}


#include "OllyLangCommands.cpp"