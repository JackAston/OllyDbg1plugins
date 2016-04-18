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
	commands["bc"] = &OllyLang::DoBC;
	commands["bp"] = &OllyLang::DoBP;
	commands["bpcnd"] = &OllyLang::DoBPCND;
	commands["bpd"] = &OllyLang::DoBPD;
	commands["bphwcall"] = &OllyLang::DoBPHWCALL;
	commands["bphwc"] = &OllyLang::DoBPHWC;
	commands["bphws"] = &OllyLang::DoBPHWS;
	commands["bpl"] = &OllyLang::DoBPL;
	commands["bplcnd"] = &OllyLang::DoBPLCND;
	commands["bpmc"] = &OllyLang::DoBPMC;
	commands["bprm"] = &OllyLang::DoBPRM;
	commands["bpwm"] = &OllyLang::DoBPWM;
	commands["bpx"] =&OllyLang::DoBPX;
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
	commands["esti"] = &OllyLang::DoESTI;
	commands["esto"] = &OllyLang::DoESTO;
	commands["eob"] = &OllyLang::DoEOB;
	commands["eoe"] = &OllyLang::DoEOE;
	commands["eval"] = &OllyLang::DoEVAL;
	commands["exec"] = &OllyLang::DoEXEC;
	commands["fill"] = &OllyLang::DoFILL;
	commands["find"] = &OllyLang::DoFIND;
	commands["findcmd"] = &OllyLang::DoFINDCMD;
	commands["findcmds"] = &OllyLang::DoFINDCMDS;
	commands["findop"] = &OllyLang::DoFINDOP;
	commands["findmem"] = &OllyLang::DoFINDMEM;
	commands["free"] = &OllyLang::DoFREE;
	commands["gapi"] = &OllyLang::DoGAPI;
	commands["gcmt"] = &OllyLang::DoGCMT;
	commands["gmemi"] = &OllyLang::DoGMEMI;
	commands["gmi"] = &OllyLang::DoGMI;
	commands["gn"] = &OllyLang::DoGN;
	commands["go"] = &OllyLang::DoGO;
	commands["gpa"] = &OllyLang::DoGPA;
	commands["gpi"] = &OllyLang::DoGPI;
	commands["gpp"] = &OllyLang::DoGPP;
	commands["handle"] = &OllyLang::DoHANDLE;
	commands["inc"] = &OllyLang::DoINC;
	commands["itoa"] = &OllyLang::DoITOA;
	commands["ja"] = &OllyLang::DoJA;
	commands["jae"] = &OllyLang::DoJAE;
	commands["jb"] = &OllyLang::DoJB;
	commands["jbe"] = &OllyLang::DoJBE;
	commands["je"] = &OllyLang::DoJE;
	commands["jmp"] = &OllyLang::DoJMP;
	commands["jne"] = &OllyLang::DoJNE;
	commands["key"] = &OllyLang::DoKEY;
	commands["lbl"] = &OllyLang::DoLBL;
    commands["lc"] = &OllyLang::DoLC;
    commands["lclr"] = &OllyLang::DoLCLR;
    commands["len"] = &OllyLang::DoLEN;
    commands["lm"] = &OllyLang::DoLM;
	commands["log"] = &OllyLang::DoLOG;
	commands["mov"] = &OllyLang::DoMOV;
	commands["msg"] = &OllyLang::DoMSG;
	commands["msgyn"] = &OllyLang::DoMSGYN;
	commands["mul"] = &OllyLang::DoMUL;
    commands["neg"] = &OllyLang::DoNEG;
	commands["not"] = &OllyLang::DoNOT;
	commands["or"] = &OllyLang::DoOR;
	commands["opcode"] = &OllyLang::DoOPCODE;
	commands["opentrace"] = &OllyLang::DoOPENTRACE;
	commands["pause"] = &OllyLang::DoPAUSE;
	commands["pop"] = &OllyLang::DoPOP;
	commands["preop"] = &OllyLang::DoPREOP;
	commands["push"] = &OllyLang::DoPUSH;
	commands["readstr"] = &OllyLang::DoREADSTR;
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
	commands["scmp"] = &OllyLang::DoSCMP;
	commands["scmpi"] = &OllyLang::DoSCMPI;
    commands["setoption"] = &OllyLang::DoSETOPTION;
	commands["shl"] = &OllyLang::DoSHL;
	commands["shr"] = &OllyLang::DoSHR;
	commands["sti"] = &OllyLang::DoSTI;
	commands["sto"] = &OllyLang::DoSTO;
	commands["sub"] = &OllyLang::DoSUB;
	commands["tc"] = &OllyLang::DoTC;
	commands["test"] = &OllyLang::DoTEST;
	commands["ti"] = &OllyLang::DoTI;
	commands["ticnd"] = &OllyLang::DoTICND;
	commands["to"] = &OllyLang::DoTO;
	commands["tocnd"] = &OllyLang::DoTOCND;
	commands["ubp"] = &OllyLang::DoBP;
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

	hwmain = hwndOllyDbg();
}

OllyLang::~OllyLang()
{
	labels.clear();
	script.clear();
	clearProgLines();
	clearLogLines();
	Reset();
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

	string scriptline;
	int inc_pos = -1, linesAdded = 0;
	int p;
	bool is_comment = false;

	while(iter != toInsert.end())
	{
		scriptline = trim(*iter);
		// Handle comments
		if(scriptline == "/*")
		{
			is_comment = true;
			iter++;
			continue;
		}
		else if(scriptline == "*/")
		{
			is_comment = false;
			iter++;
			continue;
		}
		else if (scriptline.find("/*")!=string::npos && scriptline.find("*/")==string::npos) 
		{
			p=scriptline.find("/*");
			scriptline.erase(p,scriptline.length()-p);
			is_comment = true;
		}
		else if (scriptline.find("*/")!=string::npos && scriptline.find("/*")==string::npos) 
		{
			p=scriptline.find("*/");
			scriptline.erase(0,p+2);
			is_comment = false;
		}
		else if (scriptline.find("/*")!=string::npos && scriptline.find("*/")!=string::npos) 
		{
			p=scriptline.find("/*");
			scriptline.erase(p,scriptline.find("*/")-p+2);
		}
		// Check for comments at the end of rows and erase those
		else if(scriptline.find("//")!=string::npos) 
		{
			p=scriptline.find("//");
			if (scriptline.find("\"")!=string::npos) {

				if (p > scriptline.rfind("\"") || p < scriptline.find("\""))
					scriptline.erase(p,scriptline.length()-p);

			} else
				scriptline.erase(p,scriptline.length()-p);
		}
		else if(is_comment)
		{
			iter++;
			continue;
		} 

		scriptline = trim(scriptline);
		if (scriptline=="") {
			iter++;
			continue;
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
				MessageBox(hwmain, "¥ÌŒÛµƒ#incŒƒº˛÷∏∂®!", "OllyScript", MB_OK | MB_ICONERROR | MB_TOPMOST);
			}
		}
		// Logging
		else if((inc_pos = lcline.find("#log")) > -1)
		{
			enable_logging = true;
		}
		else
		{
			script.insert(script.begin() + posInScript, scriptline);
			linesAdded++;
			posInScript++;
			addProgLine(posInScript,scriptline);
		}

		iter++;
	}
	return linesAdded;
}

int OllyLang::GetState()
{
	return script_state;
}

bool OllyLang::LoadScript(LPSTR file)
{
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

	pgr_scriptpos=1;
	setProgLineEIP(pgr_scriptpos,0);

	return true;
}

bool OllyLang::Pause()
{
//	if(script_state == SS_RUNNING || script_state == SS_LOADED)
//	{
		script_state = SS_PAUSED;
		return true;
//	}

//	return false;
}

bool OllyLang::Reset()
{	

	variables.clear();
	char s[10] = {0};
	sprintf(s,"%i.%i", VERSIONHI, VERSIONLO);
	string str(s);
	variables["$VERSION"] = str;
	EOB_row = -1;
	EOE_row = -1;
	script_state = SS_LOADED;
	enable_logging = false;
	script_pos = 0;
	pgr_scriptpos = 1; 
	resetProgLines();
	if (wndProg.hw!=NULL)
		Selectandscroll(&wndProg,pgr_scriptpos,2);
	return true;
}

bool OllyLang::Resume()
{
//	if(script_state == SS_PAUSED)
//	{
		script_state = SS_RUNNING;
		return true;
//	}

//	return false;
}

bool OllyLang::Step(int forceStep)
{
	require_ollyloop = 0;
	t_dump *cpuasm;

	t_status st = Getstatus();

	while(!require_ollyloop && Getstatus() == STAT_STOPPED && (script_state == SS_RUNNING || script_state == SS_LOADED || (forceStep && script_state == SS_PAUSED)))
	{		
		if (pgr_scriptpos==1) {
			cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
			setProgLineEIP(pgr_scriptpos,cpuasm->sel0);
		}

		// Set state to RUNNING
		if(forceStep == 0)
			script_state = SS_RUNNING;

		// Check if script out of bounds
		DumpScript();
		if(script.size() <= script_pos)
			return false;

		// Get code line
		string codeLine = trim(script[script_pos]);

		// Log line of code if that is enabled
		if(enable_logging)
		{
			char buffer[4096] = {0};
			strcpy(buffer, "--> ");
			strcat(buffer, codeLine.c_str());
			Addtolist(0, -1, buffer);
		}

		// Check if its a label
		while(codeLine[codeLine.length() - 1] == ':')
		{
			script_pos++;
			codeLine = trim(script[script_pos]);
		}
		pgr_scriptpos=script_pos+1;
		cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
		setProgLineEIP(pgr_scriptpos,cpuasm->sel0);

		//HANDLE SCRIPT BREAKPOINTS
		if (isProgLineBP(pgr_scriptpos) && script_state!=SS_PAUSED) {
			script_state=SS_PAUSED;
			break;
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
			PFCOMMAND func = commands[command];
			result = (this->*func)(args);
		}
		else 
		{
			// No such command
			errorstr = "√ª”–’‚∏ˆ√¸¡Ó: " + codeLine;
		}
		pgr_scriptpos=script_pos;
		
		// Error in processing, show error and die
		if(!result)
		{
			string message = "¥ÌŒÛ––∫≈ ";
			char buffer[32] = {0};
			message.append(itoa(script_pos + 1, buffer, 10));
			message.append("\n");
			message.append("Text: ");
			message.append(script.at(script_pos));
			message.append("\n");
			if(errorstr != "")
			{
				message.append(errorstr);
				message.append("\n");
			}
			MessageBox(hwmain, message.c_str(), "Ω≈±æ¥ÌŒÛ!", MB_ICONERROR | MB_OK);
			errorstr = "";
			script_pos++;
			Pause();

			return false;
		} else {
			if (variables["$RESULT"].vt == DW)
				setProgLineResult(script_pos+1,variables["$RESULT"].dw);
			else if (variables["$RESULT"].vt == FLT)
				setProgLineResultFloat(script_pos+1,variables["$RESULT"].flt);
			else {
				if(variables["$RESULT"].str.length())
					setProgLineResult(script_pos+1,"\""+variables["$RESULT"].str+"\"");
				else {
					setProgLineResult(script_pos+1,variables["$RESULT"].str);
				}
			}
		}

		if (script_state == SS_LOADED) {
			//After a reset (ret)
			script_state = SS_PAUSED;
			pgr_scriptpos = 1;
			break;
		}
		
		script_pos++;
		pgr_scriptpos=script_pos+1;
		
		cpuasm = (t_dump *)Plugingetvalue(VAL_CPUDASM);
		setProgLineEIP(pgr_scriptpos,cpuasm->sel0);			
		
		if(forceStep == 1)
			break;
	}
	if (wndProg.hw!=NULL)
		Selectandscroll(&wndProg,pgr_scriptpos,2);
	return true;
}

bool OllyLang::OnBreakpoint()
{
	if(EOB_row > -1)
	{
		script_pos = EOB_row;
		return true;
	}
	else

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
	
	string operators =""; 
		//"+-*/";
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
	if (len==1 || (split(v, args, ',') && v.size() == len))
	{
		if (len==1) v.push_back(args);

		int p;
		DWORD dw,result=0;
		long double flt=0, fltresult=0;
		string str,strresult="";
		string var,op,oper="";

		for(uint i = 0; i < len; i++) 
		{
			op = trim(v[i]);
			ops[i] = op;

			if ((p=getFLTOperatorPos(op)) != string::npos) 
			{
				do {			
					if (op.find_first_of("\"") != string::npos || preferstr)
						goto try_str_operation;			
					var=trim(op.substr(0,p));
					var_logging=false;
					if (!GetFLTOpValue(var,flt)) 
						//Convert integer to float (but not for first operand)
						if (oper.length()!=0 && GetDWOpValue(var,dw))
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

				var=trim(op);
				var_logging=false;
				if (!GetFLTOpValue(var,flt)) 
					//Convert integer to float (but not for first operand)
					if (oper.length()!=0 && GetDWOpValue(var,dw))
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
					
					var=trim(op.substr(0,p));
					var_logging=false;
					if (!GetDWOpValue(var,dw)) 
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

				var=trim(op);
				var_logging=false;
				if (!GetDWOpValue(var,dw)) 
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

				var_logging=true;
//				if (!sub_operand)
					setProgLineValue(script_pos+1,result);
				goto operation_ok;

			//STRING OPERATIONS
			try_str_operation:
				
				oper="";
				while ((p = getStringOperatorPos(op)) != string::npos) 
				{
					
					var=trim(op.substr(0,p));
					var_logging=false;
					if (!GetSTROpValue(var,str)) 
						goto operation_failed;
					
					if (oper.length()==0)
						//first value
						strresult = str;
					else if (oper[0]=='+')
						strresult += str; 

					oper = op[p];

					op.erase(0,p+1);
				} 

				var=trim(op);

				var_logging=false;
				if (!GetSTROpValue(var,str)) 
					goto operation_failed;
				
				if (oper.length()==0)
					//first value
					strresult = str;
				if (oper[0]=='+')
					strresult += str;

				ops[i]="\""+strresult+"\"";

				if (!sub_operand) 
					setProgLineValue(script_pos+1,"\""+strresult+"\"");
				goto operation_ok;

			operation_failed:
				var_logging = true;	
				return false;

			operation_ok:
				var_logging=true;

			} //if find_first_of

		} //for
		return true;
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
		string str,strresult="";
		string var,op,oper="";

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

	while(iter != script.end())
	{
		s = *iter;
		if(s.at(s.length() - 1) == ':')
			labels.insert(pair<string, int>(s.substr(0, s.length() - 1), loc));
		iter++;
		loc++;
	}

	return false;
}

bool OllyLang::GetANYOpValue(string op, string &value, bool hex8char)
{
	if(variables.find(op) != variables.end())
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
			char buffer[255] = {0};
			if(hex8char)
				sprintf(buffer, "%08X", variables[op].dw);
			else
				sprintf(buffer, "%X", variables[op].dw);
			value = buffer;
			goto values_ok;
		}
		return false;
	} 
	if(is_hex(op))
	{
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
//	else if(is_hex(op))
//	{
//		value = op;
//		return true;
//	}
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

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "Æ"");
			
			char buffer[STRING_READSIZE] = {0};
			Readmemory(&buffer[0], addr, STRING_READSIZE, MM_SILENT);

			if (strlen(buffer) >= STRING_READSIZE-1) 
			{
			buffer[STRING_READSIZE-1] = 0;
			}

			value="";
			value.append(buffer);
			goto values_ok_str;

		}
		
	}
	else
	{
		DWORD dw=0;
		if (GetDWOpValue(op,dw)) 
		{
			char buffer[255] = {0};
			if(hex8char)
				sprintf(buffer, "0%08X", dw);
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
	if (var_logging)
		setProgLineValue(script_pos+1,"\""+value+"\"");
	return true;

}

bool OllyLang::GetSTROpValue(string op, string &value, int size)
{
//	if (size>0)
//		DbgMsg(size,op);

	if(variables.find(op) != variables.end())
	{
		// We have a declared variable
		if(variables[op].vt == STR)
		{
			// It's a string var, return value
			value = variables[op].str;
			goto values_ok2;
		}
		return false;
	}
	else if(UnquoteString(op, '"','"')) 
	{
		value = op;
		return true; 
	}
	else if(UnquoteString(op, '#', '#')) 
	{
		value = "#"+op+"#";
		return true;
	}
	else if(UnquoteString(op, '[', ']'))
	{
		// We read a string (max STRING_READSIZE) at memory address
		DWORD addr = 0;

		sub_operand=true;
		if (!CreateOp(op,&op,1)) {
			DbgMsg(0,"¥ÌŒÛµƒΩ®¡¢÷∏’Î≤Ÿ◊˜ ˝ "+op);
			sub_operand=false;
			return false;
		}

		sub_operand=false;

		if(GetDWOpValue(op, addr)) {

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "Æ"");

			if (size>0) {
				
				char* buffer = new char[size+1];
				Readmemory(buffer, addr, size, MM_SILENT);
				buffer[size] = 0;
				value.assign(buffer,size);
				delete[] buffer;
/*				string hex;
				Str2Hex(value,hex,size);
				if (hex.find('00') != string::npos)
					value = "#"+hex+"#";
*/     
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
		string val=value;
        if(UnquoteString(val, '#', '#')) 
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
			errorstr = " ‰»Îµƒ16Ω¯÷∆◊÷∑˚±ÿ–Î «–°”⁄ªÚµ»”⁄FF";
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
            errorstr = "±‰¡ø÷µ±ÿ–Î «–°”⁄ªÚµ»”⁄FF";
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
					DbgMsg(0,"¥ÌŒÛµƒΩ®¡¢÷∏’Î∏®÷˙≤Ÿ◊˜ ˝ "+op);
					sub_operand=false;
					return false;
				}
//			sub_operand=false;
            value=strtoul(op.c_str(),0,16);
			
			return true;
		}
        return true;
}

bool OllyLang::GetDWOpValue(string op, DWORD &value)
{

	if(is_register(op))
	{
		// We have a register
		int regnr = GetRegNr(op);

		t_thread *pt = Findthread(Getcputhreadid());
		
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
		else if(stricmp(op.c_str(), "!sf") == 0)
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
			DbgMsg(0,"¥ÌŒÛµƒΩ®¡¢÷∏’Î∏®÷˙≤Ÿ◊˜ ˝ "+op);
			sub_operand=false;
			return false;
		}
		sub_operand=false;

		if(GetDWOpValue(op, addr)) {

			//Pointer Mark in Values column
			setProgLineValue(script_pos+1,(string) "Æ"");

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
			setProgLineValue(script_pos+1,(string) "Æ"");

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

	if(reg_names.find("e"+s) != reg_names.end())
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

string OllyLang::ResolveVarsForExec(string in)
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
			GetANYOpValue(varname, varname, true);
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

void OllyLang::menuListVariables(HMENU mVars,int cmdIndex) {

	map<string, var>::iterator iter;
	iter = variables.begin();
	
	vector<HMENU> Popups;
	vector<HMENU>::iterator imenu;
	HMENU menu;

	char buffer[32];

	pair<string, var> p;
	while(iter != variables.end())
	{
		p = *iter;
		menu = CreateMenu();
		Popups.push_back(menu);
		AppendMenu(mVars,MF_POPUP,(DWORD) menu,p.first.c_str());
		if (p.second.vt == STR)
		
			AppendMenu(menu,MF_STRING,cmdIndex,p.second.str.c_str());
		
		else if (p.second.vt == FLT) {
		
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
				
				char buffer[TEXTLEN]={0};
				strncpy(buffer,p.second.str.c_str(),TEXTLEN);
				int font = Plugingetvalue(VAL_WINDOWFONT);

				if (Gettextxy("Edit variable...",buffer, 0, 200, font, Rect.left,Rect.bottom)!=-1) {

					variables[p.first] = buffer;
				}
			} else if (p.second.vt == DW) {

				Getlongxy("Edit variable...",&p.second.dw, 4, 0, 0, Rect.left,Rect.bottom);
				variables[p.first] = p.second.dw;

			} else if (p.second.vt == FLT) {

				Getfloatxy("±‡º≠±‰¡ø...",&p.second.flt, 8, 0, 0, Rect.left,Rect.bottom);
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
	if (Gettextxy("÷¥––Ω≈±æ√¸¡Ó...",buffer, 0, 201, font, Rect.left,Rect.bottom)!=-1) {

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
			string message = "¥ÌŒÛ";
			message.append("\n");
			message.append("Text: ");
			message.append(codeLine);
			message.append("\n");
			if(errorstr != "")
			{
				message.append(errorstr);
				message.append("\n");
			}
			MessageBox(hwmain, message.c_str(), "Ω≈±æ√¸¡Ó¥ÌŒÛ!", MB_ICONERROR | MB_OK | MB_TOPMOST);
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



#include "OllyLangCommands.cpp"







