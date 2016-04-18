from ollylib import*
import string

Log("Example get_text.py running.....")
buf = ""
Gettxt("This is get text example",buf,'',NM_ANYNAME,0)
Log(buf)
Flashmes("Finished. See log window")