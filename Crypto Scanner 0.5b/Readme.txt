
*************************************************
*						*
*   SnD Crypto Scanner - Plugin Version 0.5b	*
*   ----------------------------------------	*
*						*
*************************************************

Been coding this for a while and now kinda got bored with it so releasing it as a beta. Sure I'll go back to it again later... just need to do something else now.

Hopefully you will find this useful - the advantage of having it as a plugin means that breakpoints can easily be set where required, and signatures can be located quickly.


Setting Breakpoints:
The buttons try and use a little bit (not much :P) intelligence when setting breakpoints. In the data section, "hardware on access" or "memory access" breakpoints are set on the specific VA referenced. In the code section, a 'hardware on execution' breakpoint is set at the beginning of the disassembled line the referenced dword is on. Hope that makes a little sense :)


Limitations:
Signatures are either made up of dwords or byte sequences. This gives 2 main weaknesses:
- some algorithms use similar dwords, distinguishing between them is not always simple.
- the algorithm finds the first instance of a given dword in a signature. If you have code which has multiple algorithms which use some of the same dwords, the referenced VA will always point to the first instance in the file.

Without doing some in depth analysis, its impossible to determine which algorithm uses a specific instance of a dword. This tool is therefore only going to make analysis a little easier, not do it for you.


Future Development:
Currently the plugin uses the plugin API to get the current file name and then reads it into allocated memory. It does not read memory inside Olly. This means packed files will need to be unpacked and the unpacked instance debugged. In future I plan to give an option to either scan the file or memory (perhaps even a specified memory range).

If you have an idea for development, want to add signatures or just want to tell me how crap this is, please go for it :)


Changelog:

14.03.08 : SnD Crypto Scanner - Plugin Version 0.5b : Public Release
Functions:
	[+] Initial Public release


Key:
[+] 	= Function/Code Added
[-] 	= Function/Code Removed
[...] 	= In progress
[?] 	= Idea/Suggestion
[*] 	= Fixed bug