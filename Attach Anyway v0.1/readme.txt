AttachAnyway v0.1
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
