from ollylib import *

Log("Example dump.py running.....")
t_mem = Findmem(GReg("eip"))
Dumpreg("test_file.dmp",t_mem.base,t_mem.size)
Info("Example dump finished")