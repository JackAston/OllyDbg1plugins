#example Input Integer by 0x0c0de
from ollylib import*

Log("Example example_input.py running.....")
inp = Getlng("This is example",4)
Log("Your input is 0x%X " % int(str(inp))) # in log your input number
Flashmes("See log window for more detalies")
