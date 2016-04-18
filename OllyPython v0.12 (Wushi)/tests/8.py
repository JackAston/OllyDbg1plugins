from ollypy import *
if __name__ == "__main__" :
		address = 0x4166d5
		Createwatchwindow()
		Createwinwindow()
		Creatertracewindow()
		Createthreadwindow()
		Createpatchwindow()
		Createthreadwindow()
		Creatertracewindow()
		Createprofilewindow(address,100)
		"""
		the follow funcs not test
		don't use these funcs
		
		typedef int DRAWFUNC(char *s,char *mask,int *select,t_sortheader *ps,int column);
		
		void Defaultbar(t_bar *pb);
		int Tablefunction(t_table *pt,HWND hw,UINT msg,WPARAM wp,LPARAM lp);
		void Painttable(HWND hw,t_table *pt,DRAWFUNC getline);
		void Selectandscroll(t_table *pt,int index,int mode);
		void Sendshortcut(int where,ulong addr,int msg,int ctrl,int shift,int vkcode);
		HWND Newtablewindow(t_table *pt,int nlines,int maxcolumns,char *winclass,char *wintitle);
		HWND Quicktablewindow(t_table *pt,int nlines,int maxcolumns,char *winclass,char *wintitle);
		
		int Broadcast (UINT msg,WPARAM wp,LPARAM lp);
		
		HWND Createdumpwindow(char *name,ulong base,ulong size,ulong addr,int type,SPECFUNC *specdump);
		void Setdumptype(t_dump *pd,int type);
		void Dumpbackup(t_dump *pd,int action);
		
		"""