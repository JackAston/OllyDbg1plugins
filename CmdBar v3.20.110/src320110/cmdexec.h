#ifndef __CMDEXEC_H
#define __CMDEXEC_H

typedef struct _MACROCMD {
  char cmd[TEXTLEN];
} MACROCMD, *PMACROCMD;

typedef struct t_cmdque {
  int  n;
  char cmd[TEXTLEN];
  struct t_cmdque *next;
} COMMAND_QUE, *PCOMMAND_QUE;


int IsHex(const char* str);

int Express(char *answer,ulong parm);
int Addwtch(char *answer,ulong parm);
int Dasmcmd(char *answer,ulong parm);
int Dorigin(char *answer,ulong parm);
int Dumpcmd(char *answer,ulong parm);
int Stakcmd(char *answer,ulong parm);
int Assembl(char *answer,ulong parm);
int LineAsm(char *answer,ulong parm);
int LineDsm(char *answer,ulong parm);
int Commlab(char *answer,ulong parm);
int Breakpt(char *answer,ulong parm);
int Brkname(char *answer,ulong parm);
int Delbkpt(char *answer,ulong parm);
int Membkpt(char *answer,ulong parm);
int Hwbreak(char *answer,ulong parm);
int Runprog(char *answer,ulong parm);
int Stepper(char *answer,ulong parm);
int Tracing(char *answer,ulong parm);
int Keystrk(char *answer,ulong parm);
int Ctrlkey(char *answer,ulong parm);
int CtlDasm(char *answer,ulong parm);
int Altkeyd(char *answer,ulong parm);
int Openexe(char *answer,ulong parm);
int Heeeelp(char *answer,ulong parm);
int LoadMac(char *answer,ulong parm);

int Execute(char *s,char *answer);
int GetCmdlistSize(void);
void SetCmdInfo(HWND hw, char *cmd);

#endif //__CMDEXEC_H
