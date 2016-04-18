from ollypy import *
def AnalyzeCode(addr):
		return Analysecode(Findmodule(addr))
		
if __name__ == "__main__" :
		thr = t_thread()
		
		thrid = Getcputhreadid()
		
		thr = Findthread(thrid)
		
		thrid = thr.threadid
		Addtolist(0,-1,"thrid = %d"%(thrid))
		
		errcode,name =  Decodethreadname(Getcputhreadid(),ADC_VALID)
		Addtolist(0,-1,"errcode = %d,name = %s"%(errcode,name))
		
		dddd = "H\x00\x15eg\x09\x20"
		Writememory(dddd,0x42e000,len(dddd),MM_RESTORE)
		
		lens,text = Readmemory(0x416719,1000,MM_RESTORE)
		
		for i in range(0,lens):
			Addtolist(0,-1,"len=%d,str=%x"%(i,ord(get_chararray_value(text,i))))
			
			"""
			the follow funcs not test , don't use
			
			t_memory* Findmemory(ulong addr);
			ulong Readmemory(void *buf,ulong addr,ulong size,int mode);
			ulong Writememory(void *buf,ulong addr,ulong size,int mode);
		
			int Listmemory(void);
		
			"""

		
		AnalyzeCode(0x4166d0)
		
		"""
		the follow funcs not test , don't use
		
		t_fixup *Findfixup(t_module *pmod,ulong addr);
		char *Finddecode(ulong addr,ulong *psize);
		
		ulong Findfileoffset(t_module *pmod,ulong addr);
		
		"""