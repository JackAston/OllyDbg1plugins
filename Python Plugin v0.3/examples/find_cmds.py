from ollylib import *

md = t_asmmodel()
pd = t_dump()
asmfindmodel(md)
m = Findmod(GReg("Eip"))
pd.base = m.base
pd.size = m.size
Findallcmds(pd,md,GReg("Eip"),"This is find example")
Flashmes("Script finished. See log windows for more detalies")