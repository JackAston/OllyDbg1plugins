from ollylib import *

hs = t_hexstr()
GetHexString("This is hex str example",hs,DIA_DEFHEX,Plugingetvalue(VAL_WINDOWFONT),'1')
Log("Input length is 0x%X" % hs.n)
GetHexStringXY("This is hex str (xy) example",hs,DIA_DEFHEX,Plugingetvalue(VAL_WINDOWFONT),'1',10,10)
Log("Input length is 0x%X" % hs.n)
Flashmes("Script finished. See log windows for more detalies")