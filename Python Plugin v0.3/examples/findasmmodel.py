from ollylib import *

Log("Example findasmmodel.py started")
md = t_asmmodel()
asmfindmodel(md)
Log("Length of code %X" % md.length)
Flashmes("Script finished. See log windows for more detalies")