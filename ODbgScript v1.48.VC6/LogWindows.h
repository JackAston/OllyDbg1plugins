#define LOG_MSG_LEN TEXTLEN

// This is the table for Script Execution
typedef struct t_wndlog_data {

	unsigned long	line;
	unsigned long	size;             // Size occupied by element in address space
	unsigned long	type;             // Type of data element, TY_xxx
	unsigned long	eip;
	char     message[LOG_MSG_LEN];

} t_logwnd_data; 

LRESULT CALLBACK wndlog_winproc(HWND hw,UINT msg,WPARAM wp,LPARAM lp);

void initLogWindow();

int wndlog_sort_function(const t_sortheader *p1,const t_sortheader *p2,const int sort);
int wndlog_get_text(char *s, char *mask, int *select, t_sortheader *ph, int column);

int add2log(string & command);
int add2log(char * command);

void clearLogLines();

