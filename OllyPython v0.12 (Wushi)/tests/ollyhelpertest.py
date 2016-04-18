from ollyhelper import *
if __name__ == "__main__" :
		eax = GetReg("EAX")
		
		Addtolist(0,-1,"eax = %d "%(eax))
		
		ss = GetReg("ss")
		
		Addtolist(0,-1,"ss = 0x%x "%(ss))
		
		cs = GetReg("cs")
		
		Addtolist(0,-1,"cs = 0x%x "%(cs))
		
		SetReg("eax", 0xffffffff)
		eax = GetReg("EAX")
		
		Addtolist(0,-1,"eax = 0x%x "%(eax))
		
		asml,err = SetAsm(0x416719,"mov eax,[0044b500]")
		size,asmd = GetAsm(0x416719)
		Addtolist(0,-1,"asm = %s,size = %d"%(asmd,size)) 
		memlist = Readmem(0x416719,50)
		Addtolist(0,-1,"memlist = %s"%(memlist))
		memlist[2]=0xff
		code = ""
		for i in range(len(memlist)):
				code+=chr(memlist[i])
				
		Writemem(0x416719, code)
		
		Addtolist(0,-1,"code = %s"%(code))  