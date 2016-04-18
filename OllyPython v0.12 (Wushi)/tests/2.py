from ollypy import *
"""
		no need these funcs in python.
		
		int Decodeascii(ulong addr,char *s,int len,int mode)
		int      Decodecharacter(char *comment,uint c);
		int Decodeunicode(ulong addr,char *s,int len);
		int Printfloat4(char *s,float f);
		int Printfloat8(char *s,double d);
		int Printfloat10(char *s,long double ext);
		int Printsse(char *s,char *f);
		int Print3dnow(char *s,char *f);
		int Stringtotext(char *data,int ndata,char *text,int ntext);
		
		we don't need data input funcs in python.
		the data conversion func is no use.
		sort data funcs we can use python do it very easy, if u want a GUI, use wxpython
		
"""		
if __name__ == "__main__" :

		address = 0x4113ca
		baseaddress = 0x400000
		size = 0x199
		
		xx,yy,qq=Decodeaddress(address,baseaddress,ADC_INMODULE,1024)
		Addtolist(0,-1,"Decodeaddress xx=%s,yy = %s,qq=%s"%(xx,yy,qq))
		
		xx,yy = Decoderelativeoffset(address,ADC_INMODULE,1024)
		Addtolist(0,-1,"Decoderelativeoffset xx=%s,yy = %s"%(xx,yy))
		
		xx,yy = Decoderange(address,size)
		Addtolist(0,-1,"Decoderange xx=%s,yy = %s"%(xx,yy))
		
		
		
		
		
		
		xx,zz = Isretaddr(0x41685a)
		
		Addtolist(0,-1,"Isretaddr xx=0x%x,zz = 0x%x"%(xx,zz))
		
	

