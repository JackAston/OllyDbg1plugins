from ollypy import *
if __name__ == "__main__" :
		address = 0x4166d5
		teststr = "Insertname"
		
		
		Insertname(address,NM_COMMENT,teststr)
		
		Quickinsertname(address+10,NM_COMMENT,"Quickinsertname")
		
		Mergequicknames()
		
		Discardquicknames()
		
		size,name =  Findname(address,NM_COMMENT)
		Addtolist(0,-1,"Findname size=%d,name = %s"%(size,name))
		
		size,name = Decodename(address,NM_COMMENT)
		Addtolist(0,-1,"Decodename size=%d,name = %s"%(size,name))
		
		addr,name = Findnextname()
		Addtolist(0,-1,"Findnextname addr=0x%x,name = %s"%(addr,name))
		
		Insertname(address,NM_LABEL,teststr)
		
		size,name = Findlabel(address)
		Addtolist(0,-1,"Findlabel size=%d,name = %s"%(size,name))
		
		Deletenamerange(address,address+10,NM_COMMENT)
		
		type,name,addr =  Findlabelbyname(address,address+10)
		Addtolist(0,-1,"Findlabelbyname type=%d,name = %s,addr = 0x%x"%(type,name,addr))
		
		Insertname(address,NM_IMPORT,teststr)
		addr,name =  Findimportbyname(address,address+10)
		Addtolist(0,-1,"Findimportbyname addr=0x%x,name = %s"%(addr,name))
		
		size,name = Demanglename("test",NM_IMPORT)
		Addtolist(0,-1,"Demanglename addr=0x%x,name = %s"%(addr,name))
		
		size,name = Findsymbolicname(address)
		Addtolist(0,-1,"Findsymbolicname addr=0x%x,name = %s"%(addr,name))
		
		"""
		search funcs 
		
		The functions described in this section have little value for plugin developer and exported mainly for use in command line plugin
		
		I don't use these funcs.
		
		"""
