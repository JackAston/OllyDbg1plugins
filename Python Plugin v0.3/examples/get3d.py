from ollylib import *

Setcomment(GReg("eip"),"This is example comment")
buf = ""
e = Get3dNow("This is Get3dnow example",buf,0)
if e==0:
 Log("All good")
 Log(buf)
else: 
 Err("Error")
bufxy = ""
m = Get3dNowXY("This is Get3dnowxy example",bufxy,0,10,0)
if m==-1:
 Err("Error or input was canceled")
else:
 Log("All good [xy] ") 
 Log(bufxy)
Flashmes("Test done. See log window")