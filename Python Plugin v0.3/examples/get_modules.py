from ollylib import *

mod = mod_info()
n = Getnumberofmods()
Log("Loaded modules information")
while n>0: 
 mod =  Getmodule(n)
 Log("Module information: ")
 Log("  " + mod.path)
 Log("  Module entry point is 0x%X " %int(str(mod.Entry)))
 Log("  Module base is %X" %int(str(mod.base)))
 mod =  Getmodule(2)
 n = n-1
Log("Memory blocks information")
block = mem_blocks()
n = Getnumberofblocks()
while n>0:
 block = Getmemoryblock(n)
 Log("Block info:")
 Log("  Block addr is %X" %int(str(block.addr)))
 Log("  Block size is %X" %int(str(block.size)))
 n = n - 1
Flashmes("This all =)")