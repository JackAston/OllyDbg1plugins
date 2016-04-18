from ollypy import *
"""
test breakpoint funcs.

"""
address = 0x4166d5
address2 = 0x4166da
address3 = 0x4166df
address4 = 0x4166e5
"""
can't use the func, because VK_F2, FIXEDFONT not defined.
#errocode = Manualbreakpoint(address,VK_F2,0,0,FIXEDFONT)
"""

if __name__ == "__main__" :

		Tempbreakpoint(address2,TY_ONESHOT|TY_KEEPCOND)
		
		Setmembreakpoint(MEMBP_READ,0x41eb40,4)
		Sethardwarebreakpoint(0x41eb45,4,HB_CODE)
		Hardbreakpoints(1)
		cmd = 1
		Setbreakpoint(address3,TY_ACTIVE,cmd)
		
		Setbreakpointext(address4,TY_ACTIVE,cmd,1)
		
		type,passcount = Getbreakpointtypecount(address4)
		
		Addtolist(0,-1,"type = %x , passcount = %x "%(type,passcount))
		
		
		Deletehardwarebreakpoint(1)
		
		Sethardwarebreakpoint(0x41eb45,4,HB_CODE)
		
		Deletehardwarebreakbyaddr(0x41eb45)
		
		Hardbreakpoints(1)
		
		"""
		the below funcs not define in plugin api v1.10
		but in plugin.h,faint......
		
		"""
		
		Deletebreakpoints(address,address4,1)
		
		type = Getbreakpointtype(address4)
		
		Addtolist(0,-1,"type = %x  "%(type))
		
				