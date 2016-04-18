from ollylib import *

Log("Example vmWarecheck running.....")
pr = IsVmWare()
if pr == 0:
 Err("No VmWare")
if pr != 0:
 Err("VmWare present")
Info("Finished")