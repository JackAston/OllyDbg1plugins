from ollylib import *

Log("Example breaks.py running.....")
laddr = Getfuncaddr("kernel32.LoadLibraryA")
Sethardbp(laddr,4,HB_CODE) 
Sethardbp(0x0401000,4,HB_ACCESS)
Sethardbp(0x0401010,4,HB_WRITE)
Deletehardbpbyaddr(laddr) 
Deletehardbp(1) #delete from 0x0401000
Setmembp(MEMBP_READ,0x401002,1)
Tempbp(0x0401007,TY_TEMP)
Setbp(0x401000,TY_ACTIVE,0)
Flashmes("This all! See hardware breakpoints and breakpoints windows for more detalies")