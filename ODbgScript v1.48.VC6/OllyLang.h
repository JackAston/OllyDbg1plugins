#pragma once

#include "StdAfx.h"


typedef unsigned long DWORD;
typedef unsigned int uint;
typedef unsigned char BYTE;

enum {SS_NONE, SS_INITIALIZED, SS_LOADED, SS_RUNNING, SS_PAUSED};

using namespace std;

// Dialogs
INT_PTR CALLBACK InputDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#include "Progress.h"
#include "LogWindows.h"

class OllyLang
{
public:
	
	//Window Objects
	t_table wndProg;
	vector<t_wndprog_data> tProgLines;
	uint pgr_scriptpos;
	
	t_table wndLog;
	vector<t_wndlog_data> tLogLines;
	
	string scriptpath;
	string currentdir;

	// Constructor & destructor
	OllyLang();
	~OllyLang();
	
	// Public methods
	int GetState();
	bool LoadScript(LPSTR fileName);
	bool Pause();
	bool Resume();
	bool Reset();
	bool Step(int forceStep);

	// Script Window
	void menuListVariables(HMENU mVars,int cmdIndex);
	bool editVariable(int nVar);
	void execCommand(void);
	
	bool jumpToLine(int number);

	// "Events"
	bool OnBreakpoint();
	bool OnException(DWORD ExceptionCode);

	// Variables that exist
	map<string, var> variables;
	// The script that is being executed
	vector<string> script;

private:
	
	typedef bool (OllyLang::*PFCOMMAND)(string);
	// Commands that can be executed
	map<string, PFCOMMAND> commands;
	// Labels that exist
	map<string, int> labels;
	// Possible register names
	set<string> reg_names;
	// Possible flag names
	set<string> flag_names;

	union eflags
	{
		DWORD dwFlags;
		struct flagbits
		{
			unsigned CF : 1;
			unsigned dummy1 : 1;
			unsigned PF : 1;
			unsigned dummy2 : 1;
			unsigned AF : 1;
			unsigned dummy3 : 1;
			unsigned ZF : 1;
			unsigned SF : 1;
			unsigned TF : 1;
			unsigned IF : 1;
			unsigned DF : 1;
			unsigned OF : 1;
		} bitFlags;
	};



	bool require_ollyloop;
	bool enable_logging;
	uint script_pos;

	bool var_logging;
	bool sub_operand;

	int script_state;
	int EOB_row;
	int EOE_row;
	string errorstr;

	// Pseudo-flags to emulate CMP
	BYTE zf, cf;
	HWND hwmain;
	// Cursor for REF / (NEXT)REF function
	int adrREF; int curREF;

	// Commands
	bool DoADD(string args);
	bool DoAI(string args);
	bool DoALLOC(string args);
	bool DoAN(string args);
	bool DoAND(string args);
	bool DoAO(string args);
	bool DoASK(string args);
	bool DoASM(string args);
	bool DoATOI(string args);
	bool DoBC(string args);
	bool DoBP(string args);
	bool DoBPCND(string args);
	bool DoBPHWCALL(string args);
	bool DoBPHWC(string args);
	bool DoBPHWS(string args);
	bool DoBPL(string args);
	bool DoBPLCND(string args);
	bool DoBPMC(string args);
	bool DoBPRM(string args);
	bool DoBPWM(string args);
	bool DoCMP(string args);
	bool DoCMT(string args);
	bool DoCOB(string args);
	bool DoCOE(string args);
	bool DoDBH(string args);
	bool DoDBS(string args);
	bool DoDEC(string args);
	bool DoDM(string args);
	bool DoDMA(string args);
	bool DoDPE(string args);
	bool DoENDE(string args);
	bool DoEOB(string args);
	bool DoEOE(string args);
	bool DoESTI(string args);
	bool DoESTO(string args);
	bool DoEVAL(string args);
	bool DoEXEC(string args);
	bool DoFILL(string args);
	bool DoFIND(string args);
	bool DoFINDOP(string args);
	bool DoFINDMEM(string args);
	bool DoFREE(string args);
	bool DoGCMT(string args);
	bool DoGMEMI(string args);
	bool DoGMI(string args);
	bool DoGN(string args);
	bool DoGO(string args);
	bool DoGPA(string args);
	bool DoGPP(string args);
	bool DoGPI(string args);
	bool DoHANDLE(string args);
	bool DoINC(string args);
	bool DoITOA(string args);
	bool DoJA(string args);
	bool DoJAE(string args);
	bool DoJB(string args);
	bool DoJBE(string args);
	bool DoJE(string args);
	bool DoJMP(string args);
	bool DoJNE(string args);
	bool DoKEY(string args);
	bool DoLBL(string args);
	bool DoLC(string args);	
	bool DoLCLR(string args);
	bool DoLEN(string args);
	bool DoLOG(string args);
    bool DoLM(string args);
	bool DoMOV(string args);
	bool DoMSG(string args);
	bool DoMSGYN(string args);
	bool DoOR(string args);
	bool DoOPCODE(string args);
	bool DoPAUSE(string args);
	bool DoPREOP(string args);
	bool DoREPL(string args);
	bool DoRESET(string args);
	bool DoREF(string args);
	bool DoRET(string args);
	bool DoREV(string args);
	bool DoRTR(string args);
	bool DoRTU(string args);
	bool DoRUN(string args);
	bool DoSCMP(string args);
	bool DoSCMPI(string args);
	bool DoSHL(string args);
	bool DoSHR(string args);
	bool DoSTI(string args);
	bool DoSTO(string args);
	bool DoSUB(string args);
	bool DoTC(string args);
	bool DoTI(string args);
	bool DoTICND(string args);
	bool DoTO(string args);
	bool DoTOCND(string args);
	bool DoVAR(string args);
	bool DoXOR(string args);
	bool DoWRT(string args);
	bool DoWRTA(string args);

	// Helper functions
	vector<string> GetScriptFromFile(LPSTR fileName);
	int InsertScript(vector<string> toInsert, int posInScript);
	int getStringOperatorPos(string &ops);
	int getDWOperatorPos(string &ops);
	int getFLTOperatorPos(string &ops);
	bool CreateOperands(string& opstring, string ops[], uint len, bool preferstr=false);
	bool GetDWOpValue(string op, DWORD &value);
	bool GetFLTOpValue(string op, long double &value);
	bool GetSTROpValue(string op, string &value, int size=0);
	bool GetANYOpValue(string op, string &value, bool hex8char);
	void LogRegNr(string& name);
	int GetRegNr(string& name);
	bool is_register(string s);
	bool is_floatreg(string s);
	bool is_flag(string s);
	bool is_variable(string& s);

	bool ParseLabels();
	bool Process(string& codeLine);

	string ResolveVarsForExec(string in);


	// Debug functions
	void DumpVars();
	void DumpLabels();
	void DumpScript();

};
