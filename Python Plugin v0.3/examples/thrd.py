from ollylib import *

Log("Example thrd.py running.....")
t_t = Findthrd(Getcputhrdid())
Log("Current thread id 0x%X" % int(str(t_t.threadid)))
Log("Current thread handle 0x%X" % int(t_t.thread))
Log("Thread entry point 0x%X" % int(str(t_t.entry)))
buf = ""
Decodethrdname(buf,t_t.threadid,ADC_WIDEFORM)
Log(buf)
Flashmes("This all. See log window for detalies")