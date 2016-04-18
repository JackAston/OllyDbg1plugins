from ollypy import *
if __name__ == "__main__" :
		Browsefilename("temp","c:\\temp","*.exe",1)
		OpenEXEfile("c:\\tools\\putty.exe",0)
		Attachtoactiveprocess(2900)
		
		"""
		these funcs no need test,because I used them many times.
		
		int Go(ulong threadid,ulong tilladdr,int stepmode,int givechance,int backupregs);
		void Animate(int animation);
		int Suspendprocess(int processevents);
		
		ulong Runsinglethread(ulong threadid);
		void Restoreallthreads(void);
		
		"""