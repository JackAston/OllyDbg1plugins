from ollylib import*
import string

Log("Example disasm_test.py running.....")
x = Disasmin(0x0401000,"IMUL ECX,ECX")
if x==0:
 Log("Command not found")
else:
 Log("Command found at address 0x%X" %int(str(x)))
x = Disasmback(0x0401005,"DEC EBP")
if x==0:
 Log("Command not found")
else:
 Log("Command found at address 0x%X" %int(str(x)))
dsm = Disassm(0x0401000,DISASM_ALL)
if dsm.result.count("DEC EBP")!=0:
 Flashmes("This is example")
else:
 Err("No original example")
Log("Disassembled command "+dsm.result)
Log("Hexadecimal dump of the command "+dsm.dump)
x = Disasminbyopcode(0x0401000,"52")
if x==0:
 Log("Command not found")
else:
 Log("Command found at address 0x%X" %int(str(x)))
x = Disasmbackbyopcode(0x040100C,"8BFC")
if x==0:
 Log("Command not found")
else:
 Log("Command found at address 0x%X" %int(str(x)))
Flashmes("See log window for detalies")