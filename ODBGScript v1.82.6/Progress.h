#define PROG_CMD_LEN 256
#define PROG_RES_LEN 128
#define PROG_VAL_LEN 256

#define PROG_TYPE			0x0F
#define PROG_TYPE_COMMAND	0x01
#define PROG_TYPE_ASM		0x02
#define PROG_TYPE_LABEL		0x04
#define PROG_TYPE_COMMENT	0x08

#define PROG_ATTR			0xF0
#define PROG_ATTR_IFLEVELS	0x60

#define PROG_ATTR_EXECUTED	0x10
#define PROG_ATTR_IFLEVODD	0x20
#define PROG_ATTR_IFLEVEVN	0x40
#define PROG_ATTR_ERROR		0x80

#define MASK_POPUP_VAR		0x1000
#define MASK_POPUP_LABEL	0x2000

#define CMD_POPUP_FDISASM	0x100
#define CMD_POPUP_FDUMP		0x200
#define CMD_POPUP_FSTACK	0x400
#define CMD_POPUP_ODUMP		0x800
#define CMD_POPUP_MASK		0xF00

// This is the table for Script Execution
typedef struct t_wndprog_data {

	unsigned long	line;
	unsigned long	size;             // Size occupied by element in address space
	unsigned long	type;             // Type of data element, TY_xxx
	char     command[PROG_CMD_LEN];
	char     result[PROG_RES_LEN];
	char     values[PROG_VAL_LEN];
	unsigned long	eip;              // eip before execution
	unsigned char	pause;            // script breakpoint
	int				jumpto;           // to display jumps in script window

} t_wndPROG_data; 

LRESULT CALLBACK wndprog_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp);

void initProgTable();

int wndprog_sort_function(const t_sortheader *p1,const t_sortheader *p2,const int sort);
int wndprog_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column);

void InvalidateProgWindow(void);
void FocusProgWindow(void);

int  addProgLine(int line, string & command, int type=false);
int  setProgLineEIP(int line, int eip);
int  setProgLineValue(int line, string& value);
int  setProgLineValue(int line, DWORD value);
int  setProgLineValueFloat(int line, long double value);
int  setProgLineResult(int line, var& result);
int  setProgLineResult(int line, string& result);
int  setProgLineResult(int line, DWORD result);
int  setProgLineResultFloat(int line, long double result);
int  isProgLineBP(int line);
int  isProgLineComment(int line);
int  getProgLineType(int line);
int  setProgLineAttr(int line,int type) ;

void clearProgLines();
void resetProgLines();

bool editProgLine(t_wndprog_data *ppl) ;
int  analyseProgLineType(string &scriptline, int linenumber);
