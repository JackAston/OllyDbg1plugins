from ollylib import*

Log("open.py running.....")
buf = ""
Browsefile("Test browse",buf,".exe",0)
Log(buf)
Runfile(buf)
Flashmes("Open example finished. See log window!")