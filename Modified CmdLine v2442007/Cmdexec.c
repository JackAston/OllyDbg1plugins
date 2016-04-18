////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                     COMMAND LINE PLUGIN FOR OLLYDBG                        //
//                                                                            //
//                          (Command processing)                              //
//                                                                            //
//                                                                            //
// This code is distributed "as is", without warranty of any kind, expressed  //
// or implied, including, but not limited to warranty of fitness for any      //
// particular purpose. In no event will Oleh Yuschuk be liable to you for any //
// special, incidental, indirect, consequential or any other damages caused   //
// by the use, misuse, or the inability to use of this code, including any    //
// lost profits or lost savings, even if Oleh Yuschuk has been advised of the //
// possibility of such damages. Or, translated into English: use at your own  //
// risk!                                                                      //
//                                                                            //
// This code is free. You can modify this code, include parts of it into your //
// own programs and redistribute modified code.                               //
//                                                                            //
// Any contributions are welcome. Please send them to Ollydbg@t-online.de     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
// AND UNSIGNED CHAR! THIS IS OLLYDBG'S DEFAULT!

#define STRICT                         // Avoids some type mismatches
#include <windows.h>
#include <stdio.h>
#include "plugin.h"
int Print(char *answer,ulong parm);
int Loaddll(char *answer,ulong parm);
int Loadplugin(char *answer,ulong parm);
int Loadpdb(char *answer,ulong parm);



// Prototype for function that executes command. Must return 0 on success and
// -1 if command is invalid and should not be added to the history. Anything
// written to answer will be displayed in error/message line of the command
// line window, input parameter parm is the corresponding field from cmdlist.
typedef int t_exefunc(char *answer,ulong parm);

typedef struct t_command {             // Descriptor of command
  char           *cmd;                 // Command (must be uppercase)
  char           *args;                // List of arguments
  ulong          parm;                 // Parameter
  t_exefunc      *func;                // Responsible function
} t_command;

static char      cmd[TEXTLEN];         // Command  (uppercase)
static ulong     address;              // Operands A/a (address expression)
static t_result  lvalue;               // Operand  L   (lvalue address)
static t_result  value;                // Operands V/v (expression)
char      string[TEXTLEN];      // Operand  S   (any string)
static int       count;                // Operands N/n (count)

#define DECR_ISREG     0x20            // Mask to check that operand is register

struct {
  char           *regname;
  int            type;
  int            index;
} reglist[] = {
  { "EAX",  DEC_DWORD|DECR_ISREG,  0 },
  { "ECX",  DEC_DWORD|DECR_ISREG,  1 },
  { "EDX",  DEC_DWORD|DECR_ISREG,  2 },
  { "EBX",  DEC_DWORD|DECR_ISREG,  3 },
  { "ESP",  DEC_DWORD|DECR_ISREG,  4 },
  { "EBP",  DEC_DWORD|DECR_ISREG,  5 },
  { "ESI",  DEC_DWORD|DECR_ISREG,  6 },
  { "EDI",  DEC_DWORD|DECR_ISREG,  7 },
  { "AX",   DEC_WORD|DECR_ISREG,   0 },
  { "CX",   DEC_WORD|DECR_ISREG,   1 },
  { "DX",   DEC_WORD|DECR_ISREG,   2 },
  { "BX",   DEC_WORD|DECR_ISREG,   3 },
  { "SP",   DEC_WORD|DECR_ISREG,   4 },
  { "BP",   DEC_WORD|DECR_ISREG,   5 },
  { "SI",   DEC_WORD|DECR_ISREG,   6 },
  { "DI",   DEC_WORD|DECR_ISREG,   7 },
  { "AL",   DEC_BYTE|DECR_ISREG,   0 },
  { "CL",   DEC_BYTE|DECR_ISREG,   1 },
  { "DL",   DEC_BYTE|DECR_ISREG,   2 },
  { "BL",   DEC_BYTE|DECR_ISREG,   3 },
  { "AH",   DEC_BYTE|DECR_ISREG,   4 },
  { "CH",   DEC_BYTE|DECR_ISREG,   5 },
  { "DH",   DEC_BYTE|DECR_ISREG,   6 },
  { "BH",   DEC_BYTE|DECR_ISREG,   7 }
};

int Setcmnd(char *answer,ulong parm) {
  ulong size;
  t_thread *pthr;
  if (lvalue.type & DECR_ISREG) {
    // Assignment to register.
    pthr=Findthread(Getcputhreadid());
    if (pthr==NULL || pthr->regvalid==0) {
      strcpy(answer,"Unable to modify register");
      return -1; };
    // Make backup copy if first modification.
    if (pthr->reg.modifiedbyuser==0) {
      pthr->oldreg=pthr->reg;
      pthr->reg.modifiedbyuser=1; };
    // Modify register (32/16/8 bit).
    if (lvalue.type==(DEC_DWORD|DECR_ISREG))
      pthr->reg.r[lvalue.lvaddr]=value.u;
    else if (lvalue.type==(DEC_WORD|DECR_ISREG))
      pthr->reg.r[lvalue.lvaddr]=
      (pthr->reg.r[lvalue.lvaddr] & 0xFFFF0000) | (value.u & 0x0000FFFF);
    else if (lvalue.lvaddr<4)
      pthr->reg.r[lvalue.lvaddr]=
      (pthr->reg.r[lvalue.lvaddr] & 0xFFFFFF00) | (value.u & 0x000000FF);
    else
      pthr->reg.r[lvalue.lvaddr-4]=
      (pthr->reg.r[lvalue.lvaddr-4] & 0xFFFF00FF) | ((value.u & 0x000000FF)<<8);
    pthr->reg.modified=1;
    Broadcast(WM_USER_CHREG,0,0); }
  else {
    // Assignment to memory. Drawback: backup copy is not created!
    if (lvalue.type==DEC_DWORD) size=4;
    else if (lvalue.type==DEC_WORD) size=2;
    else if (lvalue.type==DEC_BYTE) size=1;
    else {
      strcpy(answer,"Memory size must be 1, 2 or 4 bytes");
      return -1; };
    size=Writememory(&(value.u),lvalue.lvaddr,size,MM_RESILENT|MM_DELANAL);
    if (size==0) {
      strcpy(answer,"Unable to modify memory");
      return -1; };
    Broadcast(WM_USER_CHALL,0,0); };
  return 0;
};

int Express(char *answer,ulong parm) {
  strcpy(answer,value.value);
  return 0;
};

int Addwtch(char *answer,ulong parm) {
  if (string[0]!='\0')
    Insertwatch(1000000,string);
  Createwatchwindow();
  return 0;
};

int Dasmcmd(char *answer,ulong parm) {
  Setcpu(0,address,0,0,CPU_ASMHIST|CPU_ASMCENTER);
  return 0;
};

int Dorigin(char *answer,ulong parm) {
  Sendshortcut(PM_DISASM,0,WM_CHAR,0,0,'*');
  return 0;
};

int Dumpcmd(char *answer,ulong parm) {
  Setcpu(0,0,address,0,CPU_DUMPHIST|CPU_DUMPFIRST|CPU_DUMPFOCUS);
  if (parm!=0) Setdumptype((t_dump *)Plugingetvalue(VAL_CPUDDUMP),parm);
  return 0;
};

int Stakcmd(char *answer,ulong parm) {
  Setcpu(0,0,0,address,CPU_DUMPHIST|CPU_DUMPFIRST|CPU_DUMPFOCUS);
  return 0;
};

int Assembl(char *answer,ulong parm) {
  int i,j,k,n,good;
  char s[TEXTLEN];
  t_asmmodel model,attempt;
  t_memory *pmem;
  t_dump *pasm;
  // Visualize changes.
  Setcpu(0,address,0,0,CPU_ASMHIST|CPU_ASMCENTER);
  if (string[0]=='\0')                 // No immediate command
    Sendshortcut(PM_DISASM,address,WM_CHAR,0,0,' ');
  else {
    // Assemble immediate command. If there are several possible encodings,
    // select the shortest one.
    model.length=0;
    for (j=0; ; j++) {                 // Try all possible encodings
      good=0;
      for (k=0; k<4; k++) {            // Try all possible constant sizes
        n=Assemble(string,address,&attempt,j,k,model.length==0?answer:s);
        if (n>0) {
          good=1;
          // If another decoding is found, check if it is shorter.
          if (model.length==0 || n<model.length)
            model=attempt;             // Shortest encoding so far
          ;
        };
      };
      if (good==0) break;              // No more encodings
    };
    if (model.length==0)
      return -1;                       // Invalid command
    // Check for imprecise parameters.
    k=model.mask[0];
    for (i=1; i<model.length; i++) k&=model.mask[i];
    if (k!=0xFF) {
      strcpy(answer,"Command contains imprecise operands");
      return -1; };
    // If there is no backup copy, create it. Dump window always assumes that
    // backup has the same base and size as the dump, so check it to avoid
    // strange ireproducible errors.
    pmem=Findmemory(address);
    if (pmem==NULL) {
      strcpy(answer,"Attempt to assemble to non-existing memory");
      return -1; };
    pasm=(t_dump *)Plugingetvalue(VAL_CPUDASM);
    if (pasm!=NULL && pmem->copy==NULL &&
      pmem->base==pasm->base && pmem->size==pasm->size)
      Dumpbackup(pasm,BKUP_CREATE);
    // Now write assembled code to memory.
    Writememory(model.code,address,model.length,MM_RESTORE|MM_DELANAL);
  };
  return 0;
};

int Commlab(char *answer,ulong parm) {
  Insertname(address,parm,string);
  Broadcast(WM_USER_CHALL,0,0);
  return 0;
};

int Breakpt(char *answer,ulong parm) {
  Setbreakpoint(address,TY_ACTIVE,0);
  Insertname(address,NM_BREAK,string);
  Deletenamerange(address,address+1,NM_BREAKEXPL);
  Deletenamerange(address,address+1,NM_BREAKEXPR);
  Broadcast(WM_USER_CHALL,0,0);
  return 0;
};

int Brkname(char *answer,ulong parm) {
  int i;
  char name[TEXTLEN];
  t_table *reftable;
  t_ref *pref;
  if (strlen(string)==0) {
    strcpy(answer,"Function name missed"); return -1; };
  // Find all intermodule calls.
  Findalldllcalls((t_dump *)Plugingetvalue(VAL_CPUDASM),0,"Intermodular calls");
  // Get sorted data.
  reftable=(t_table *)Plugingetvalue(VAL_REFERENCES);
  if (reftable==NULL || reftable->data.n==0) {
    strcpy(answer,"No references"); return -1; };
  if (reftable->data.itemsize<sizeof(t_ref)) {
    strcpy(answer,"Old version of OllyDbg"); return -1; };
  // Loop through all found calls.
  for (i=0; i<reftable->data.n; i++) {
    // The safest way: size of t_ref may change in the future!
    pref=(t_ref *)((char *)reftable->data.data+reftable->data.itemsize*i);
    if (Findlabel(pref->dest,name)==0) // Unnamed destination
      continue;
    if (strcmp(name,string)!=0)        // Different function
      continue;
    if (parm==0) {                     // Set breakpoint
      Setbreakpoint(pref->addr,TY_ACTIVE,0);
      Deletenamerange(pref->addr,pref->addr+1,NM_BREAK);
      Deletenamerange(pref->addr,pref->addr+1,NM_BREAKEXPL);
      Deletenamerange(pref->addr,pref->addr+1,NM_BREAKEXPR); }
    else
      Deletebreakpoints(pref->addr,pref->addr+1,0);
    ;
  };
  Broadcast(WM_USER_CHALL,0,0);
  return 0;
};

int Delbkpt(char *answer,ulong parm) {
  Deletebreakpoints(address,address+1,0);
  Broadcast(WM_USER_CHALL,0,0);
  return 0;
};

int Membkpt(char *answer,ulong parm) {
  ulong size;
  if (parm==0)
    Setmembreakpoint(0,0,0);
  else {
    if (value.dtype!=DEC_DWORD) {
      strcpy(answer,"Integer expression expected");
      return -1; }
    else if (value.u==0)
      size=1;
    else if (value.u<=address) {
      strcpy(answer,"Invalid memory range");
      return -1; }
    else
      size=value.u-address;
    Setmembreakpoint(parm,address,size); };
  return 0;
};

int Hwbreak(char *answer,ulong parm) {
  int i;
  if (Plugingetvalue(VAL_HARDBP)==0) {
    strcpy(answer,"OS doesn't support hardware breakpoints");
    return -1; };
  if (address==0)
    Hardbreakpoints(0);
  else if (parm==HB_FREE) {
    i=Deletehardwarebreakbyaddr(address);
    if (i==0) sprintf(answer,"No HW breakpoint on address %08X",address); }
  else {
    i=Sethardwarebreakpoint(address,1,parm);
    if (i!=0) strcpy(answer,"Unable to set HW breakpoint"); };
  return 0;
};

int Runprog(char *answer,ulong parm) {
  Go(0,address,STEP_RUN,parm,1);
  return 0;
};

int Stepper(char *answer,ulong parm) {
  if (parm==0)                         // Step into
    Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F7);
  else                                 // Step over
    Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F8);
  return 0;
};

int Tracing(char *answer,ulong parm) {
  ulong threadid;
  t_thread *pthr;
  // If run trace buffer is not active, create it. It requires actual registers
  // of current thread.
  if (Runtracesize()==0) {
    threadid=Getcputhreadid();
    if (threadid==0) threadid=Plugingetvalue(VAL_MAINTHREADID);
    pthr=Findthread(threadid);
    if (pthr!=NULL) Startruntrace(&(pthr->reg)); };
  // If buffer is open, set optional condition and start run trace.
  if (Runtracesize()>0) {
    if (address==0)
      Settracecondition(string,0,0,0,0,0);
    else
      Settracecondition(string,0,address,address+1,0,0);
    if (parm==0) {
      Animate(ANIMATE_TRIN);
      Go(0,0,STEP_IN,0,1); }
    else {
      Animate(ANIMATE_TROVER);
      Go(0,0,STEP_OVER,0,1);
    };
  };
  return 0;
};

int Keystrk(char *answer,ulong parm) {
  Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,parm);
  return 0;
};

int Ctrlkey(char *answer,ulong parm) {
  Sendshortcut(PM_MAIN,0,WM_KEYDOWN,1,0,parm);
  return 0;
};

int Altkeyd(char *answer,ulong parm) {
  Sendshortcut(PM_MAIN,0,WM_SYSKEYDOWN,0,0,parm);
  return 0;
};

int Openexe(char *answer,ulong parm) {
  if (string[0]=='\0')
    Sendshortcut(PM_MAIN,0,WM_KEYDOWN,0,0,VK_F3);
  else
    OpenEXEfile(string,0);
  return 0;
};

int Heeeelp(char *answer,ulong parm) {
  int n;
  char *apihelp;
  n=strlen(string);
  if (n==0) {
    WinHelp((HWND)Plugingetvalue(VAL_HWMAIN),
      "ollydbg.hlp",HELP_KEY,(ulong)"Command line plugin");
    return 0; };
  if (stricmp(string,"OllyDbg")==0) {
    WinHelp((HWND)Plugingetvalue(VAL_HWMAIN),
      "ollydbg.hlp",HELP_CONTENTS,0);
    return 0; };
  apihelp=(char *)Plugingetvalue(VAL_APIHELP);
  if (apihelp==NULL || apihelp[0]=='\0') {
    strcpy(answer,"No API help file selected in OllyDbg");
    return -1; };
  // Check for possible ANSI/UNICODE function suffix and remove it. Topics in
  // API help usually contain generic names.
  if (n>2 && islower(string[n-2]) &&
    (string[n-1]=='A' || string[n-1]=='W'))
    string[n-1]='\0';
  WinHelp((HWND)Plugingetvalue(VAL_HWMAIN),
    apihelp,HELP_PARTIALKEY,(ulong)string);
  return 0;
};


static t_command cmdlist[] = {
  // "Assignment" command, must be first in the list.
  { "SET",   "LV", 0,                 Setcmnd },   // Assign to lvalue
  // Calculator and watch commands.
  { "CALC",  "V",  0,                 Express },   // Estimate expression
  { "?",     "V",  0,                 Express },   // Ditto
  { "",      "V",  0,                 Express },   // Ditto
  { "WATCH", "S",  0,                 Addwtch },   // Add watch expression
  { "W",     "S",  0,                 Addwtch },   // Ditto
  // Disassembler commands.
  { "AT",    "A",  0,                 Dasmcmd },   // Disassemble at address
  { "FOLLOW","A",  0,                 Dasmcmd },   // Ditto
  { "ORIG",  "",   0,                 Dorigin },   // Disassemble at EIP
  { "*",     "",   0,                 Dorigin },   // Ditto
  // Dump and stack commands.
  { "D",     "A",  0,                 Dumpcmd },   // Dump at address
  { "DUMP",  "A",  0,                 Dumpcmd },   // Dump at address
  { "DA",    "a",  DU_DISASM|0x0011,  Dumpcmd },   // Dump as disassembly
  { "DB",    "a",  DU_HEXTEXT|0x0101, Dumpcmd },   // Dump in hex byte format
  { "DC",    "a",  DU_TEXT|0x0401,    Dumpcmd },   // Dump in ASCII format
  { "DD",    "a",  DU_ADDR|0x0014,    Dumpcmd },   // Dump in stack format
  { "DU",    "a",  DU_UNICODE|0x0402, Dumpcmd },   // Dump in UNICODE format
  { "DW",    "a",  DU_IHEX|0x0082,    Dumpcmd },   // Dump in hex word format
  { "STK",   "A",  0,                 Stakcmd },   // Go to address in stack
  // Assembling commands.
  { "A",     "AS", 0,                 Assembl },   // Assemble at address
  // Comments and labels.
  { "L",     "AS", NM_LABEL,          Commlab },   // Label at address
  { ":",     "AS", NM_LABEL,          Commlab },   // Ditto
  { "C",     "AS", NM_COMMENT,        Commlab },   // Comment at address
  // Breakpoint commands.
  { "BP",    "AS", 0,                 Breakpt },   // Break with condition
  { "BPX",   "S",  0,                 Brkname },   // Break on all calls
  { "BPD",   "S",  1,                 Brkname },   // Delete break on all calls
  { "BC",    "A",  0,                 Delbkpt },   // Delete breakpoint
  { "MR",    "Av", MEMBP_READ,        Membkpt },   // Memory breakpt on access
  { "MW",    "Av", MEMBP_WRITE,       Membkpt },   // Memory breakpt on write
  { "MD",    "",   0,                 Membkpt },   // Remove memory breakpoint
  { "HR",    "A",  HB_ACCESS,         Hwbreak },   // HW break on access
  { "HW",    "A",  HB_WRITE,          Hwbreak },   // HW break on write
  { "HE",    "A",  HB_CODE,           Hwbreak },   // HW break on execution
  { "HD",    "a",  HB_FREE,           Hwbreak },   // Remove HW breakpoint
  // Stepping and tracing commands.
  { "STOP",  "",   VK_F12,            Keystrk },   // Pause execution
  { "PAUSE", "",   VK_F12,            Keystrk },   // Ditto
  { "RUN",   "",   0,                 Runprog },   // Run program
  { "G",     "a",  0,                 Runprog },   // Run till address
  { "GE",    "a",  1,                 Runprog },   // Run and pass exception
  { "S",     "",   0,                 Stepper },   // Step into
  { "SI",    "",   0,                 Stepper },   // Ditto
  { "SO",    "",   1,                 Stepper },   // Step over
  { "T",     "a",  0,                 Tracing },   // Trace in till address
  { "TI",    "a",  0,                 Tracing },   // Ditto
  { "TO",    "a",  1,                 Tracing },   // Trace over till address
  { "TC",    "S",  0,                 Tracing },   // Trace in till condition
  { "TOC",   "S",  1,                 Tracing },   // Trace over till condition
  { "TR",    "",   VK_F9,             Ctrlkey },   // Till return
  { "TU",    "",   VK_F9,             Altkeyd },   // Till user code
  // Table window commands.
  { "LOG",   "",   'L',               Altkeyd },   // View Log window
  { "MOD",   "",   'E',               Altkeyd },   // View Modules window
  { "MEM",   "",   'M',               Altkeyd },   // View Memory window
  { "CPU",   "",   'C',               Altkeyd },   // View CPU window
  { "CS",    "",   'K',               Altkeyd },   // View Call Stack
  { "BRK",   "",   'B',               Altkeyd },   // View Breakpoints window
  { "OPT",   "",   'O',               Altkeyd },   // Open Options
  // Application commands.
  { "EXIT",  "",   'X',               Altkeyd },   // Quit OllyDbg
  { "QUIT",  "",   'X',               Altkeyd },   // Ditto
  // Miscellaneous commands.
  { "OPEN",  "S",  0,                 Openexe },   // Open executable file
  { "CLOSE", "",   VK_F2,             Altkeyd },   // Close executable
  { "RST",   "",   VK_F2,             Ctrlkey },   // Restart current program
  { "HELP",  "S",  0,                 Heeeelp },   // Help on API function
  { "H",     "S",  0,                 Heeeelp },   // Ditto
  { "PRINT", "S",  0,                 Print   },
  { "LOADDLL", "S",  0,               Loaddll },
  { "LOADPLUGIN", "S" , 0,            Loadplugin},
  { "LOADPDB",	  "S" , 0,            Loadpdb} 
  
};

// Parses and executes command in s. Returns 0 on success (valid command) and
// -1 on error.
int Execute(char *s,char *answer) {
  int i,j,k,n;
  char *s0,type,name[32];
  t_result result;
  if (s==NULL || s[0]=='\0' || answer==NULL)
    return -1;                         // Error or nothing to execute
  s0=s;
  // Extract command and convert it to uppercase.
  while (*s==' ' || *s=='\t') s++;     // Skip spaces
  answer[0]='\0';
  i=0;
  // Valid command is either '?','*',':' or consists only of characters.
  while (i<32) {
    if (isalpha(*s)) cmd[i++]=toupper(*s++);
    else if (i==0 && (*s=='?' || *s=='*' || *s==':')) {
      cmd[i++]=*s++; break; }
    else break; };
  cmd[i]='\0';
  // Recognize command.
  for (i=0; i<sizeof(cmdlist)/sizeof(cmdlist[0]); i++)
    if (strcmp(cmd,cmdlist[i].cmd)==0) break;
  // If command is not recognized, check whether it is assignment to register.
  if (i>=sizeof(cmdlist)/sizeof(cmdlist[0])) {
    for (k=0; k<sizeof(reglist)/sizeof(reglist[0]); k++) {
      if (strcmp(reglist[k].regname,cmd)==0) break; };
    if (k<sizeof(reglist)/sizeof(reglist[0])) {
      s=s0; i=0; }                     // Assume SET and restore
    else {
      sprintf(answer,"Unrecognized command: %s",cmd);
      return -1;
    };
  };
  // Command found. It has index i in cmdlist. Now get operands.
  j=0;
  while (1) {
    while (*s==' ' || *s=='\t') s++;   // Skip spaces from the command
    type=cmdlist[i].args[j++];         // Get type of next expected operand
    if (type=='\0')
      break;                           // End of list of arguments reached
    switch (type) {
      case 'a':                        // Address expression or empty
        if (*s==',' || *s=='=' || *s=='\0' || *s==';') {
          address=0;
          if (*s!='\0') s++;
          break;                       // Note: conditional break
        };
      case 'A':                        // Address expression
        n=Expression(&result,s,0,0,NULL,0,0,Getcputhreadid());
        if (n<0 || result.type==DEC_UNKNOWN) {
          strcpy(answer,result.value); // Error in expression
          return -1; };
        if (result.dtype!=DEC_DWORD) { // Address must be integer
          strcpy(answer,"Integer expression expected"); return -1; };
        if (Findmemory(result.u)==NULL) {
          strcpy(answer,"Address points to nowhere");
          return -1; };
        address=result.u;
        s+=n; break;
      case 'L':                        // Lvalue (register or memory)
        // Check for register.
        n=k=0;
        while (s[n]==' ' || s[n]=='\t')
          n++;                         // Skip leading spaces
        while (isalpha(s[n]) && k<30)
          name[k++]=toupper(s[n++]);   // Get name of register in uppercase
        name[k]='\0';
        for (k=0; k<sizeof(reglist)/sizeof(reglist[0]); k++) {
          if (strcmp(reglist[k].regname,name)==0) break; };
        if (k<sizeof(reglist)/sizeof(reglist[0])) {
          // Yep, this is a register!
          lvalue.type=reglist[k].type;
          lvalue.lvaddr=reglist[k].index; }
        else {
          n=Expression(&lvalue,s,0,0,NULL,0,0,Getcputhreadid());
          if (n<0 || lvalue.type==DEC_UNKNOWN) {
            strcpy(answer,lvalue.value); return -1; };
          if (lvalue.lvaddr==0) {
            strcpy(answer,"Unable to assign"); return -1;
          };
        };
        s+=n;
        while (*s==' ' || *s=='\t')
          s++;                         // Skip trailing spaces
        if (s[0]!='=' || s[1]=='=') {
          strcpy(answer,"Assignment expected"); return -1; };
        s++;                           // Skip assignment
        break;
      case 'S':                        // String, rest of command
        strcpy(string,s);
        s+=strlen(s); break;
      case 'v':                        // Value of expression or empty
        if (*s==',' || *s=='=' || *s=='\0' || *s==';') {
          result.type=DEC_UNKNOWN;
          result.u=0;
          if (*s!='\0') s++;
          break;                       // Note: conditional break
        };
      case 'V':                        // Value of expression
        n=Expression(&value,s,0,0,NULL,0,0,Getcputhreadid());
        if (n<0 || result.type==DEC_UNKNOWN) {
          strcpy(answer,value.value);  // Error in expression
          return -1; };
        s+=n; break;
      default:
        sprintf(answer,"Internal error: undefined operand type %c",type);
        return -1;
      ;
    };
    if (cmdlist[i].args[j]!='\0') {
      // More operands will follow, skip delimiter if any.
      while (*s==' ' || *s=='\t') s++;
      if (*s==',' || *s=='=') s++;
    };
  };
  if (*s!='\0' && *s!=';') {           // Unprocessed characters
    sprintf(answer,"Extra characters on line: %.64s",s);
    return -1; };
  // Command parsed correctly, now call subroutine that executes it.
  return cmdlist[i].func(answer,cmdlist[i].parm);
};

