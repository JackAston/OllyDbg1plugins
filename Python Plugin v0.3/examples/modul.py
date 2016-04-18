from ollylib import*

Log("Example modul.py running.....")
m = Findmod(GReg("eip"))
Log("Module name "+m.name)
Log("Module path "+m.path)
Log("Module base 0x%X" %int(str(m.base)))
Log("Module size 0x%X" %int(str(m.size)))
Log("Base address of module code block 0x%X" %int(str(m.codebase)))
Flashmes("Script finished. See log window for detalies")