from ollylib import *

pmod = Findmod(GReg("eIp"))
Log("Current address %X" %int(str(GReg("eIp"))))
#Converts address belonging to some module into offset in executable file. Returns offset or 0 if offset cannot be calculated 
#Recomended to  use notepad.exe for example
offst = Searchfileoffset(pmod,GReg("eIp"))
Log("File offset %X" %int(str(offst)))
Flashmes("Script finished. See log windows for more detalies")