AttachAnyway
(c)2005 Joe Stewart <joe@joestewart.org>

AttachAnyway is a PoC OllyDbg plugin designed to show how to remove a 
process' hook on NtContinue by the anti-debugger-attach method devised by 
Piotr Bania here:

http://pb.specialised.info/all/anti-dattach.asm

This is not intended to be a universal plugin for all anti-attach methods, 
just one example of how you can do it. It works by enumerating all 
processes, searching their virtual memory space for a JMP hook on the 
NtContinue method, then replacing the jump with the original bytes from a 
non-hooked process, then calling the OllyDbg Attachtoactiveprocess
API.

attach-test.exe is an assembled version of Piotr's anti-dattach.asm you can 
use to test the plugin with.

Update:
AttachAnyway v0.2

AttachAnyway now works with Piotr's anti-debugger-attach method two:

http://pb.specialised.info/all/anti-dattach2.asm

Here we just find the PEB_LDR_DATA pointer and VirtualProtectEx the bytes
back to PAGE_READWRITE - thus the exception handler will never alert that
the page has been accessed by the debugger when attaching. 

Update:
AttachAnyway v0.3

Thanks go to anonymouse for giving me the method to reliably obtain the 
PEB of another process post-XPSP2 - it works much better than searching
by signature. And also for the suggestion to give you Windows users some
carriage-return characters in the readme and code files. Sometimes I 
forget that other people actually use Windows as their primary operating
system.

