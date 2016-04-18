from ollypy import *

def GetAsm( addr ):
				
		    size,code = Readcommand( addr )
		    dm = t_disasm()
		    threadid = Getcputhreadid()
		    size1,data = Disasm( code,size, addr, dm, DISASM_CODE, threadid )
		    return dm.result, size1
	
def Readmem(addr, lens):
		    """
		    Read the memory from addr,
		    the mem's lenth is lens.
		    return the value list.
		    """
		    List = []
		    len, text = Readmemory(addr, lens, MM_RESTORE)
		    if len > 0 :
		        for i in range(0, len):
		            List.append(ord(get_chararray_value(text, i)))
		                        
		    return List		 
		    
def SetAsm( addr, asmcode ):
		    m = t_asmmodel()
		    leng,err= Assemble(asmcode,addr,m,0,0)
		    Writememory(m.code,addr,leng,MM_RESTORE)
		    return leng,err
		    

		    	    
if __name__ == "__main__" :
		address = 0x416719
		
		
		
		
		    
		asmd,size = GetAsm(address)
		
		Addtolist(0,-1,"asm = %s,data = %d"%(asmd,size)) 
		Followcall(0x4113ca)
		
				  
		
		size, code = Readcommand(address)
		
		errcode = Isfilling(0,code,size,1)
		
		
		Addtolist(0,-1,"Isfilling = %d"%(errcode))
		
		
		memlist = Readmem(address,50)
		Addtolist(0,-1,"memlist = %s"%(memlist))
		
		code = ""
		for i in range(len(memlist)):
				code+=chr(memlist[i])
		
		addr = Disassembleforward(code,address,50,address,8,1)
		
		Addtolist(0,-1,"addr = 0x%x"%(addr))
		
		
		
		#Disassembleback(0,ulong base,ulong size,ulong ip,int n,int usedec)
		
		#Disassembleforward(char *block,ulong base,ulong size,ulong ip,int n,int usedec)
		
		regtest = t_reg()
		size, code = Readcommand(address)
		
		errcode,errstr = Issuspicious(code,size,address,Getcputhreadid(),regtest)
		
		Addtolist(0,-1,"Issuspicious = %d ,errstr = %s "%(errcode,errstr))
		
		
		    
		asml,err = SetAsm(0x416719,"mov eax,[0044b500]")
		asmd,size = GetAsm(0x416719) 
		
		"""
		need my ollyext.py to test it
		"""
		errcode = Checkcondition(memlist[0],1)
		Addtolist(0,-1,"Checkcondition = %d"%(errcode))
		
		
