from ollypy import *
if __name__ == "__main__" :
		address = 0x416700
		proc = Findprocbegin(address)
		proc2 = Findprocend(address)
		Addtolist(0,-1,"procbegin = %x , procend = %x "%(proc,proc2))
		
		
		errcode,procb,proce = Getproclimits(0x4166f2)
		
		Addtolist(0,-1,"procbegin = %x , procend = %x "%(procb,proce))
		
		proc =  Findprevproc(address)
		proc2 =  Findnextproc(address)
		Addtolist(0,-1,"Findprevproc = %x , Findnextproc = %x "%(proc,proc2))
		
		Insertwatch(0,"esp == 0x0")
		Deletewatch(0)
		Insertwatch(0,"esp == 0x0")
		text = Getwatch(0)
		Addtolist(0,-1,"watch = %s "%(text))
		
		#result1 = t_result()
		"""
		forget this func,
		I don't know how to use it,:(
		
		  Expression(char *expression, a, b, char *data,ulong database,ulong datasize,ulong threadid);
		
		"""