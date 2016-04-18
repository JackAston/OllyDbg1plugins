-------------------------------
OllyScript plugin v0.93 by Izik
-------------------------------

1. About OllyScript
1.1 What's new in v0.93?
2. API reference
2.1 Commands
2.2 Global features
3. License and source code
4. Contact me

------------------------------

1. About OllyScript
-------------------

OllyScript is a plug in for OllyDbg (v1.10) which provides a powerfull
automation features to the debugger, in a form of an assembly-like language
that can be easily picked by people who knew assembly to begin with ;-)

------------------------------

1.1 What's new in v0.93?
----------------------------

* New pseudo opcodes were added:
	TEST, NOT, PUSH, POP, LEA, XCHG

* The ability to use 16/8bits registers versions of GP registers

* The ability to use expression as an arugment

* The ability to add comments to the debugged code on the fly

------------------------------

2. API reference
----------------

The API reference is been splited between ORIGINAL_README.txt and this one.
For any commands/features prior to this version (v0.93), look at ORIGINAL_README.txt

In addition, the ORIGINAL_README.txt also contains a proper introduction to the language
and it's features. If you're new to this plug-in - it is strongly recommended to review it.

2.1 Commands
------------

TEST dest, src
--------------

TESTs src and dest and updates the *INNER* ZF flag.

Example:
	test ax, ax
	je _foobar

NOT dest
--------

NOTs the dest (which is also the source) bits

Example:
	not eax

PUSH src
--------

PUSHs src to the stack, and updates ESP

Example:
	push eax

POP dest
--------

POPs from the stack to dest, and updates ESP

Example:
	pop eax

LEA dest, src
-------------

Sets the effective address of src to dest

Example:
	lea eax, [ebp+4]

XCHG dest, src
--------------

XCHG values between dest and src.

Example:
	xchg eax, ebx

COMMENT [addr,] comment
-----------------------

Adds a USER defined comment to the debugged code on the fly.
The 1st arg is optinial and by default sets to the current value of EIP

Example:
	COMMENT deadbeef, "This is where FOOBAR function gets to work!" 
	COMMENT "Here is where the unpacking takes place!"

2.2 Global features
-------------------

The following are newly global features added to the OllyScript language.
It has been done with a big respect to keep backward compatibility.

16/8bits support
----------------

Now it is possible to address within the script to a 16/8 bits registers.
This option will only be working for 16/8 version of a general purpose register.

32=>16/8, Table:
================

	32bit | 16bit | 8bit
	----------------------	
	EAX     AX      AL, AH
	EBX     BX      BL, BH
	ECX     CX      CL, CH
	EDX     DX      DL, DH
	ESI     SI      -
	EDI     DI      -

Unlike any real assembly flavors (AT&T, Intel) there is no need to define the operation size.
By default all variables and registers are 32bit (DWORD) within the script. And the 16/8bit
layer is currently been implemented via bitmask. As I did not found a real support, to access 
such registers from within Ollydbg API.

Expressions as argument
-----------------------

Now it is possible to use an expressions as argument within the script itself.
The numeric values within the expression are always considered to be in hex base.
Currently only minus ('-') and plus ('+') actions are been supported.

Example:
	mov eax, [ebp-4]
		
Again, unlike real assembly flavors (AT&T, Intel) there are no stricts.

Example:
	mov eax, [ebp+1+1+1+1] == mov eax, [ebp+4]

It is also possible to use variable(s) within the expression

Example:
	mov eax, [ebp+foobar+5]

This will take ``foobar`` variable value, and use it to calculate the rest of the expression.

Also notice that when unknown string appears within the expression (aka. syntax error).
It will not cause a fault, but rather assume that string value is 0.

3. License and source code
--------------------------

This is an open source project. The original author (SHaG) did not set up a proper license
for this code. But for the project sake, keep it free and open with respect to its authors.

The source code of this version and its matching binary release, can be found @ http://www.tty64.org

4. Contact me
-------------

Izik, <izik@tty64.org>
http://www.tty64.org

------------------------------