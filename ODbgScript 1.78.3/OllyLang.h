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

// This is the table for Script Execution
typedef struct t_dbgmemblock {

	void *	hmem;		 //Memory Adress
	ulong	size;		 //Block Size
	int		script_pos;	 //Registred at script pos
	bool	autoclean;	 //On script restart/change

	ulong	free_at_eip; //To free memory block used in ASM commands
	
	//Optional actions to do
	bool	restore_registers;
	bool	listmemory;

	//Delayed Result Origin
	bool	result_register;
	int		reg_to_return;

} t_dbgmemblock; 

typedef struct t_export {

	ulong addr;
	char  label[TEXTLEN];

} t_export;

class OllyLang
{
public:
	
	//Window Objects
	t_table wndProg;
	vector<t_wndprog_data> tProgLines;
	uint pgr_scriptpos;
	uint pgr_scriptpos_paint;

	t_table wndLog;
	vector<t_wndlog_data> tLogLines;
	
	string scriptpath;
	string currentdir;

	//allocated memory blocks to free at end of script
	vector<t_dbgmemblock> tMemBlocks;

	//to know if dialog is opened, to destroy on reset
	HWND hwndinput;
	
	//last breakpoint reason
	ulong break_reason;
	ulong break_memaddr;

	// Constructor & destructor
	OllyLang();
	~OllyLang();
	
	// Public methods
//	int GetState();
	bool LoadScript(LPSTR fileName);
	bool SaveBreakPoints(LPSTR fileName);
	bool LoadBreakPoints(LPSTR fileName);

	bool Pause();
	bool Resume();
	bool Reset();
	bool Step(int forceStep);
	bool ProcessAddonAction();

	// Script Window
	ulong GetFirstCodeLine(ulong from=0);

	void menuListLabels(HMENU mLabels,int cmdIndex);
	void menuListVariables(HMENU mVars,int cmdIndex);

	bool editVariable(int nVar);
	void execCommand(void);
	
	bool jumpToLine(int number);

	int SearchInScript(string text, ulong fromPos);

	// "Events"
	bool OnBreakpoint(int reason, int details);
	bool OnException(DWORD ExceptionCode);

	// For ODBG_Plugincmd (external calls)
	bool isCommand(string cmd);
	bool callCommand(string cmd, string args);

	// Free Allocated Virtual Memory
	bool freeMemBlocks();
	void regBlockToFree(t_dbgmemblock &block);
	void regBlockToFree(void * hMem, ulong size, bool autoclean);
	bool unregMemBlock(void * hMem);

	// The script that is being executed
	vector<string> script;
	// Variables that exist
	map<string, var> variables;
	// Labels in script
	map<string, int> labels;
	// Breakpoint Auto Jumps 
	map<int, int> bpjumps;
	// Call/Ret in script
	vector<ulong> calls;

	bool showVarHistory;

	int script_state;
	bool require_ollyloop;
	bool require_addonaction;

	//hwnd to t_dump (OPENDUMP/BACKUP)
	map<HWND, t_dump*> dumpWindows;

private:
	
	typedef bool (OllyLang::*PFCOMMAND)(string);
	// Commands that can be executed
	map<string, PFCOMMAND> commands;
	// Possible register names
	set<string> reg_names;
	// Possible flag names
	set<string> flag_names;

	bool bUnicode;

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

	bool enable_logging;
	uint script_pos;

	bool var_logging;
	bool sub_operand;

	int EOB_row;
	int EOE_row;

	bool bInternalBP;
	ulong nIgnoreNextValuesHist;

	string errorstr;

	DWORD tickcount;
	DWORD tickcounthi;
	ULONGLONG tickcount_startup;

	unsigned char * search_buffer;

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
	bool DoASMTXT(string args);
	bool DoATOI(string args);
	bool DoBACKUP(string args);
	bool DoBC(string args);
	bool DoBCA(string args);
	bool DoBD(string args);
	bool DoBDA(string args);
	bool DoBEGINSEARCH(string args);
	bool DoBP(string args);
	bool DoBPCND(string args);
	bool DoBPD(string args);
	bool DoBPGOTO(string args);
	bool DoBPHWCA(string args);
	bool DoBPHWC(string args);
	bool DoBPHWS(string args);
	bool DoBPL(string args);
	bool DoBPLCND(string args);
	bool DoBPMC(string args);
	bool DoBPRM(string args);
	bool DoBPWM(string args);
	bool DoBPX(string args);
	bool DoBUF(string args);
	bool DoCALL(string args);
	bool DoCLOSE(string args);
	bool DoCMP(string args);
	bool DoCMT(string args);
	bool DoCOB(string args);
	bool DoCOE(string args);
	bool DoDBH(string args);
	bool DoDBS(string args);
	bool DoDEC(string args);
	bool DoDIV(string args);
	bool DoDM(string args);
	bool DoDMA(string args);
	bool DoDPE(string args);
	bool DoENDE(string args);
	bool DoENDSEARCH(string args);
	bool DoEOB(string args);
	bool DoEOE(string args);
	bool DoERUN(string args);
	bool DoESTEP(string args);
	bool DoESTI(string args);
	bool DoEVAL(string args);
	bool DoEXEC(string args);
	bool DoFILL(string args);
	bool DoFIND(string args);
	bool DoFINDCALLS(string args);
	bool DoFINDCMD(string args);
	bool DoFINDOP(string args);
	bool DoFINDOPREV(string args);
	bool DoFINDMEM(string args);
	bool DoFREE(string args);
	bool DoGAPI(string args);
	bool DoGBPM(string args);
	bool DoGBPR(string args);
	bool DoGCI(string args);
	bool DoGCMT(string args);
	bool DoGFO(string args);
	bool DoGLBL(string args);
	bool DoGMA(string args);
	bool DoGMEMI(string args);
	bool DoGMEXP(string args);
	bool DoGMI(string args);
	bool DoGMIMP(string args);
	bool DoGN(string args);
	bool DoGO(string args);
	bool DoGOPI(string args);
	bool DoGPA(string args);
	bool DoGPP(string args);
	bool DoGPI(string args);
	bool DoGREF(string args);
	bool DoGRO(string args);
	bool DoGSL(string args);
	bool DoGSTR(string args);
	bool DoGSTRW(string args);
	bool DoHANDLE(string args);
	bool DoHISTORY(string args);
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
	bool DoLOADLIB(string args);
	bool DoLOG(string args);
	bool DoLOGBUF(string args);
    bool DoLM(string args);
	bool DoMEMCOPY(string args);
	bool DoMOV(string args);
	bool DoMSG(string args);
	bool DoMSGYN(string args);
	bool DoMUL(string args);
	bool DoNAMES(string args);
	bool DoNEG(string args);
	bool DoNOT(string args);
	bool DoOLLY(string args);
	bool DoOR(string args);
	bool DoOPCODE(string args);
	bool DoOPENDUMP(string args);
	bool DoOPENTRACE(string args);
	bool DoPAUSE(string args);
	bool DoPOP(string args);
	bool DoPOPA(string args);
	bool DoPREOP(string args);
	bool DoPUSH(string args);
	bool DoPUSHA(string args);
	bool DoRBP(string args);
	bool DoREADSTR(string args);
	bool DoREFRESH(string args);
	bool DoREPL(string args);
	bool DoRESET(string args);
	bool DoREF(string args);
	bool DoRET(string args);
	bool DoREV(string args);
	bool DoROL(string args);
	bool DoROR(string args);
	bool DoRTR(string args);
	bool DoRTU(string args);
	bool DoRUN(string args);
	bool DoSBP(string args);
	bool DoSCMP(string args);
	bool DoSCMPI(string args);
	bool DoSETOPTION(string args);
	bool DoSHL(string args);
	bool DoSHR(string args);
	bool DoSTI(string args);
	bool DoSTO(string args);
	bool DoSTR(string args);
	bool DoSUB(string args);
	bool DoTC(string args);
	bool DoTEST(string args);
	bool DoTI(string args);
	bool DoTICK(string args);
	bool DoTICND(string args);
	bool DoTO(string args);
	bool DoTOCND(string args);
	bool DoUNICODE(string args);
	bool DoVAR(string args);
	bool DoXOR(string args);
	bool DoXCHG(string args);
	bool DoWRT(string args);
	bool DoWRTA(string args);

	// Helper functions
	vector<string> GetScriptFromFile(LPSTR fileName);
	int InsertScript(vector<string> toInsert, int posInScript);
	int getStringOperatorPos(string &ops);
	int getDWOperatorPos(string &ops);
	int getFLTOperatorPos(string &ops);
	bool CreateOperands(string& args, string ops[], uint len, bool preferstr=false);
	bool CreateOp(string& args, string ops[], uint len, bool preferstr=false);
	bool GetBYTEOpValue(string op, BYTE &value);
	bool GetDWOpValue(string op, DWORD &value, DWORD default_val=0);
	bool GetAddrOpValue(string op, DWORD &value);
	bool GetFLTOpValue(string op, long double &value);
	bool GetSTROpValue(string op, string &value, int size=0);
	bool GetANYOpValue(string op, string &value, bool hex8forExec=false);
	void LogRegNr(string& name);
	int GetRegNr(string& name);
	bool is_register(string s);
	bool is_floatreg(string s);
	bool is_flag(string s);
	bool is_variable(string& s);

	bool ParseLabels();
	bool Process(string& codeLine);
	bool AddBPJump(int bpaddr,int labelpos);

	string ResolveVarsForExec(string in,bool hex8forExec);

	// Debug functions
	void DumpVars();
	void DumpLabels();
	void DumpBPJumps();
	void DumpScript();

	void DropVariable(string var);

	string FormatAsmDwords(string asmLine);

	DWORD AddProcessMemoryBloc(string data, int mode=PAGE_READWRITE);
	DWORD AddProcessMemoryBloc(int size, int mode=PAGE_READWRITE);
	bool  DelProcessMemoryBloc(DWORD address);

	bool ExecuteASM(string command);

	// Save / Restore Breakpoints
	t_hardbpoint hwbp_t[4];
	t_sorted sortedsoftbp_t;
	t_bpoint* softbp_t;

	uint saved_bp;
	uint alloc_bp;
	bool AllocSwbpMem(uint tmpSizet);
	void FreeBpMem();

	// Save / Restore Registers
	struct t_reg_backup
	{
		int loaded;
		DWORD threadid;

		DWORD eax;
		DWORD ebx;
		DWORD ecx;
		DWORD edx;
		DWORD esi;
		DWORD edi;

		DWORD esp;
		DWORD ebp;

		DWORD dwFlags;

		DWORD eip;

	} reg_backup;
	
	bool SaveRegisters(bool stackToo);
	bool RestoreRegisters(bool stackToo);

	//cache for GMEXP
	vector<t_export> tExportsCache;
	ulong exportsCacheAddr;

	//cache for GMIMP
	vector<t_export> tImportsCache;
	ulong importsCacheAddr;
		
};
