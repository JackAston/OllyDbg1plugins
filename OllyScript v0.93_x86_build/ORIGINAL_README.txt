-------------------------------
OllyScript plugin v0.92 by SHaG
-------------------------------

1. About OllyScript
2. Status
2.1 What's new in v0.92?
3. Documentation
3.1 Language
3.1.1 Reserved variables
3.1.2 Commands
3.2 Labels
3.3 Comments
3.4 Menus
4. Integration with other plugins
5. Contact me
6. License and source code
7. Thanks!

------------------------------

1. About OllyScript
-------------------
OllyScript is a plugin for OllyDbg, which is, in my opinion, 
the best application-mode debugger out there. One of the best 
features of this debugger is the plugin architecture which allows 
users to extend its functionality. OllyScript is a plugin 
meant to let you automate OllyDbg by writing scripts in an 
assembly-like language. Many tasks involve a lot of repetitive 
work just to get to some point in the debugged application. By 
using my plugin you can write a script once and for all. 

------------------------------

2. Status (10 July 2004)
----------------------------
v0.92
A big bug in script synchronization fixed (thanks loveboom!).
GN behaviour updated.
MOV can now write strings to memory.

v0.91
A bug related to pausing the application fixed, the GN command added, ASM returns $RESULT.

v0.9
OllyScript has now been downloaded more then 10000 times! That means more then 2Gb of raw
scripting power flowing down the optic cable veins of the Internet. Not bad if you ask me!
The development of the plugin has been a bit slow, I've got a job programming xray systems
which has taken a lot of time. Sorry about that.

2.1 What's new?
---------------
+ New commands: ASK, BPL, BPLCND, COB, COE, EVAL, EXEC/ENDE, GN, TICND, TOCND
+ Execution of code in the target process context
+ String concateration with ADD or EVAL
+ Input box
+ Logging breakpoints
+ Removal of EOB and EOE
+ Tracing with condition
+ Get name of address
# ASM now returns assembled length in $RESULT
# Fixed pause crash bug
# Fixed bug with JBE, hopefully it was the last of the Jxx bugs
# OllyScript now REQUIRES OllyDbg v1.10. No other versions are officially supported.

------------------------------

3. Documentation
----------------
Two example scripts (tElock098.osc and UPX.osc) are available with this release. 
The scripts will when run immediately find the OEP packed executable. 

3.1 Language
------------
The scripting language of OllyScript is an assembly-like language.

In the document below, src and dest can be (unless stated otherwise):
 - Constant in the form of a hex number withot prefixes and suffixes (i.e. 00FF, not 0x00FF or 00FFh)
 - Variable previously declared by VAR
 - A 32-bit register (one of EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP, EIP). Non 32-bit registers are not supported at
   the moment, but you can use SHL/SHR and AND to get their values.
 - A memory reference in square brackets (i.e. [401000] points to the memory at address 401000, 
	[ecx] points to the memory at address ecx).
 - A flag with an exclamation mark in front (one of !CF, !PF, !AF, !ZF, !SF, !DF, !OF)
 - Sometimes byte strings are required. those are scripted as #6A0000# (values between two #) and 
	must have an even number of characters.
 - Some byte strings can contain the wildcard '?', for exampla #6A??00# or #6?0000#

3.1.1 Reserved variables
------------------------

$RESULT
-------
Return value for some functions like FIND etc.
$RESULT_1 and $RESULT_2 are available for some commands.

$VERSION
--------
Contains current version of OllyScript
Example
	cmp $VERSION, "0.8"
	ja version_above_08

3.1.2 Commands
--------------

#INC file
---------
Includes a script file in another script file
Example:
	#inc "anotherscript.txt"
	
#LOG
----
Enables logging of executed commands.
The commands will appear in OllyDbg log window, and will be prefixed with -->
Example:
	#log

ADD dest, src
-------------
Adds src to dest and stores result in dest
Example: 
	add x, 0F
	add eax, x
	add [401000], 5
	add y, " times" // If y was 1000 before this command then y is "1000 times" after it

AI
--
Executes "Animate into" in OllyDbg
Example:
	ai
	
AN addr
-------
Analyze module which contains the address addr.
Example:
	an eip // Same as pressing CTRL-A

AND dest, src
-------------
ANDs src and dest and stores result in dest
Example: 
	and x, 0F
	and eax, x
	and [401000], 5

ASK question
------------
Displays an input box with the specified question and lets user enter a response.
Sets the reserved $RESULT variable (0 if cancel button was pressed).
Example:
	ask "Enter new EIP"
	cmp $RESULT, 0
	je cancel_pressed
	mov eip, $RESULT
	
ASM addr, command
-----------------
Assemble a command at some address.
Returns bytes assembled in the reserved $RESULT variable
Example:
	asm eip, "mov eax, ecx"

AO
--
Executes "Animate over" in OllyDbg
Example:
	ao

BC addr
-------
Clear unconditional breakpoint at addr.
Example:
	bc 401000
	bc x
	bc eip

BP addr
--------
Set unconditional breakpoint at addr.
Example:
	bp 401000
	bp x
	bp eip

BPCND addr, cond
----------------
Set breakpoint on address addr with condition cond.
Example:
	bpcnd 401000, "ECX==1"
	
BPL addr, expr
--------------
Sets logging breakpoint at address addr that logs expression expr
Example:
	bpl 401000, "eax" // logs the value of eax everytime this line is passed
	
BPLCND addr, expr, cond
-----------------------
Sets logging breakpoint at address addr that logs expression expr if condition cond is true
Example:
	bplcnd 401000, "eax", "eax > 1" // logs the value of eax everytime this line is passed and eax > 1
	
BPMC
----
Clear memory breakpoint.
Example:
	bpmc

BPHWC addr
----------
Delete hardware breakpoint at a specified address
Example:
	bphwc 401000
	
BPHWS addr, mode
----------------
Set hardware breakpoint. Mode can be "r" - read, "w" - write or "x" - execute.
Example:
	bphws 401000, "x"

BPRM addr, size
---------------
Set memory breakpoint on read. Size is size of memory in bytes.
Example:
	bprm 401000, FF

BPWM addr, size
---------------
Set memory breakpoint on write. Size is size of memory in bytes.
Example:
	bpwm 401000, FF

CMP dest, src
-------------
Compares dest to src. Works like it's ASM counterpart.
Example: 
	cmp y, x
	cmp eip, 401000

CMT addr, text
--------------
Inserts a comment at the specified address
Example:
	cmt eip, "This is the entry point"

COB
---
Makes script continue execution after a breakpoint has occured (removes EOB)
Example:
	COB

COE
---
Makes script continue execution after an exception has occured (removes EOE)
Example:
	COE

DBH
---
Hides debugger
Example:
	dbh

DBS
---
Unhides debugger
Example:
	dbs

DEC var
-------
Substracts 1 from variable
Example:
	dec v

DM addr, size, file
-------------------
Dumps memory of specified size from specified address to specified file
Example:
	dm 401000, 1F, "c:\dump.bin"
	
DMA addr, size, file
-------------------
Dumps memory of specified size from specified address to specified file appending to that file if it exists
Example:
	dma 401000, 1F, "c:\dump.bin"

DPE filename, ep
----------------
Dumps the executable to file with specified name.
Entry point is set to ep.
Example:
	dpe "c:\test.exe", eip

EOB label
---------
Transfer execution to some label on next breakpoint.
Example:
	eob SOME_LABEL

EOE label
---------
Transfer execution to some label on next exception.
Example:
	eob SOME_LABEL

ESTI
----
Executes SHIFT-F7 in OllyDbg.
Example:
	esti

ESTO
----
Executes SHIFT-F9 in OllyDbg.
Example:
	esto

EVAL
----
Evaluates a string expression that contains variables.
The variables that are declared in the current script can be enclosed in curly braces {} to be inserted.
Sets the reserved $RESULT variable
Example:
	var x
	mov x, 1000
	eval "The value of x is {x}" // after this $RESULT is "The value of x is 00001000"

EXEC/ENDE
---------
Executes instructions between EXEC and ENDE in the context of the target process.
Values in curly braces {} are replaced by their values.
Example:
// This does some movs
var x
var y
mov x, "eax"
mov y, "0DEADBEEF"
exec
mov {x}, {y} // mov eax, 0DEADBEEF will be executed
mov ecx, {x} // mov ecx, eax will be executed
ende
// This calls ExitProcess in the debugged application
exec
push 0
call ExitProcess
ende
ret

FILL addr, len, value
---------------------
Fills len bytes of memory at addr with value
Example:
	fill 401000, 10, 90 // NOP 10h bytes

FIND addr, what
---------------
Searches memory starting at addr for the specified value.
When found sets the reserved $RESULT variable. $RESULT == 0 if nothing found.
The search string can also use the wildcard "??" (see below).

Example:
	find eip, #6A00E8# // find a PUSH 0 followed by some kind of call
	find eip, #6A??E8# // find a PUSH 0 followed by some kind of call

FINDOP addr, what
-----------------
Searches code starting at addr for an instruction that begins with the specified bytes. 
When found sets the reserved $RESULT variable. $RESULT == 0 if nothing found.
The search string can also use the wildcard "??" (see below).
Example:
	findop 401000, #61# // find next POPAD
	findop 401000, #6A??# // find next PUSH of something

GN addr
-------
Gets the symbolic name of specified address (ex the API it poits to)
Sets the reserved $RESULT variable to the name. If that name is an API
$RESULT_1 is set to the library (ex kernel32) and $RESULT_2 to the name of the API (ex ExitProcess).
Example:
	gn 401000

GPA proc, lib
-------------
Gets the address of the specified procedure in the specified library.
When found sets the reserved $RESULT variable. $RESULT == 0 if nothing found.
Useful for setting breakpoints on APIs.
Example:
	gpa "MessageBoxA", "user32.dll" // After this $RESULT is the address of MessageBoxA and you can do "bp $RESULT".

GO addr
-------
Executes to specified address (like G in SoftIce)
Example:
	go 401005

GMI addr, info
--------------
Gets information about a module to which the specified address belongs.
"info" can be MODULEBASE, MODULESIZE, CODEBASE or CODESIZE (if you want other info in the future versions plz tell me).
Sets the reserved $RESULT variable (0 if data not found).
Example:
	GMI eip, CODEBASE // After this $RESULT is the address to the codebase of the module to which eip belongs

INC var
-------
Adds 1 to variable
Example:
	inc v

JA label
--------
Use this after cmp. Works like it's asm counterpart.
Example:
	ja SOME_LABEL

JAE label
---------
Use this after cmp. Works like it's asm counterpart.
Example:
	jae SOME_LABEL

JB label
--------
Use this after cmp. Works like it's asm counterpart.
Example:
	jb SOME_LABEL

JBE label
---------
Use this after cmp. Works like it's asm counterpart.
Example:
	jbe SOME_LABEL

JE label
--------
Use this after cmp. Works like it's asm counterpart.
Example:
	je SOME_LABEL

JMP label
---------
Unconditionally jump to a label.
Example:
	jmp SOME_LABEL

JNE label
---------
Use this after cmp. Works like it's asm counterpart.
Example:
	jne SOME_LABEL

LBL addr, text
--------------
Inserts a label at the specified address
Example:
	lbl eip, "NiceJump"

LOG src
-------
Logs src to OllyDbg log window.
If src is a constant string the string is logged as it is.
If src is a variable or register its logged with its name.
Example:
	log "Hello world" // The string "Hello world" is logged
	var x
	mov x, 10
	log x // The string "x = 00000010" is logged.

MOV dest, src
-------------
Move src to dest.
Src can be a long hex string in the format #<some hex numbers>#, for example #1234#.
Remember that the number of digits in the hex string must be even, i.e. 2, 4, 6, 8 etc.
Example: 
	mov x, 0F
	mov y, "Hello world"
	mov eax, ecx
	mov [ecx], #00DEAD00BEEF00#
	mov !CF, 1
	mov !DF, !PF
	mov [403000], "Hello world"

MSG message
-----------
Display a message box with specified message
Example:
	MSG "Script paused"

MSGYN message
-----------
Display a message box with specified message and YES and NO buttons.
Sets the reserved $RESULT variable to 1 if YES is selected and 0 otherwise.
Example:
	MSGYN "Continue?"

OR dest, src
-------------
ORs src and dest and stores result in dest
Example: 
	or x, 0F
	or eax, x
	or [401000], 5

PAUSE
-----
Pauses script execution. Script can be resumed from plugin menu.
Example:
	pause

REPL addr, find, repl, len
--------------------------
Replace find with repl starting att addr for len bytes.
Wildcards are allowed
Example:
	repl eip, #6a00#, #6b00#, 10
	repl eip, #??00#, #??01#, 10
	repl 401000, #41#, #90#, 1F

RET
---
Exits script.
Example:
	ret

RTR
---
Executes "Run to return" in OllyDbg
Example:
	rtr

RTU
---
Executes "Run to user code" in OllyDbg
Example:
	rtu

RUN
---
Executes F9 in OllyDbg
Example:
	run

SHL dest, src
-------------
Shifts dest to the left src times and stores the result in dest.
Example:
	mov x, 00000010
	shl x, 8 // x is now 00001000

SHR dest, src
-------------
Shifts dest to the right src times and stores the result in dest.
Example:
	mov x, 00001000
	shr x, 8 // x is now 00000010

STI
---
Execute F7 in OllyDbg.
Example:
	sti

STO
---
Execute F8 in OllyDbg.
Example:
	sto

SUB dest, src
-------------
Substracts src from dest and stores result in dest
Example: 
	sub x, 0F
	sub eax, x
	sub [401000], 5

TI
--
Executes "Trace into" in OllyDbg
Example:
	ti

TICND cond
----------
Traces into calls until cond is true
Example:
	ticnd "eip > 40100A" // will stop when eip > 40100A

TO
--
Executes "Trace over" in OllyDbg
Example:
	to

TOCND cond
----------
Traces over calls until cond is true
Example:
	tocnd "eip > 40100A" // will stop when eip > 40100A

VAR
---
Declare a variable to be used in the script. 
Must be done before the variable is used.
Example: 
	var x

XOR dest, src
-------------
XORs src and dest and stores result in dest
Example: 
	xor x, 0F
	xor eax, x
	xor [401000], 5


3.2 Labels
----------
Labels are defined bu using the label name followed by a colon.
Example:
	SOME_LABEL:


3.3 Comments
------------
Comments can be put anywhere and have to start with "//". Block
comments must start with "/*" on a new line and and with "*/"
also on a new line.


3.4 Menus
---------
The main OllyScript menu consists of the following items:
- Run script...: lets the user select a script file and starts it
- Abort: aborts a running script
- Pause: pauses a running script
- Resume: resumes a paused script
- About: shows information about this plugin

------------------------------

4. Integration with other plugins
---------------------------------
You can call OllyScript from your plugin and make it execute a script.
Use something like the source code below:

HMODULE hMod = GetModuleHandle("OllyScript.dll");
if(hMod) // Check that the other plugin is present and loaded
{
	// Get address of exported function
	int (*pFunc)(char*) = (int (*)(char*)) GetProcAddress(hMod, "ExecuteScript");
	if(pFunc) // Check that the other plugin exports the correct function
		pFunc("myscript.txt"); // Execute exported function
}

------------------------------

5. Contact me
-------------
To contact me you can post your question in the forum or go on IRC 
and message SHaG on EFnet. You can also mail me to shag-at-apsvans-dot-com.

------------------------------

6. License and source code
--------------------------
Soon I'm going to armadildo this plugin and charge an awful lot of money
for it! :P Seriously, you are free to use this plugin and the source code however 
you see fit. However please name me in your documentation/about box and if 
the project you need my code for is on a larger scale please also notify 
me - I am curious.

Source code for this plugin is available on request only. Please send me
a mail if you need it!

------------------------------

7. Thanks!
----------
I'd like to thank all the wonderful people who reported bugs, wrote scripts, came
with improvement ideas etc. 
R@dier for the great dumping engine.
And of course Olly for developing this great debugger!

------------------------------