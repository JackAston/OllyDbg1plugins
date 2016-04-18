from ollylib import *

AnalyseCurrentModule()
Log("Proc begin is %X" %int(str(Findprocbgn(0x010060A3))))  
Log("Proc end is %X" %int(str(Findprcend(0x010060A3))))
Log("Proc nxt is %X" %int(str(Findnxtproc(0x010060A3))))
Log("Prev proc is %X" %int(str(Findprevprc(0x010060A3))))
Flashmes("This all =) See log window")
