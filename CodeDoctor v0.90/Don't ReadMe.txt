          CodeDoctor 0.90 - OllyDbg Plugin by hnedka (11.11.2009)

________________________________________________________________________________

Installation:
Copy CodeDoctor.dll and CodeDoctor.ini to OllyDbg/Plugins
          
________________________________________________________________________________

History:
0.90 (11.11.2009) - initial public release

________________________________________________________________________________

Functions:

1) Deobfuscate

Select instructions in disasm window and execute this command. It will try 
to clear the code from junk instructions.

Example:

Original:
00874372    57                      PUSH EDI                                     
00874373    BF 352AAF6A             MOV EDI,6AAF2A35
00874378    81E7 0D152A41           AND EDI,412A150D
0087437E    81F7 01002A40           XOR EDI,402A0001
00874384    01FB                    ADD EBX,EDI                                 
00874386    5F                      POP EDI                                     

Deobfuscated:
00874372    83C3 04                 ADD EBX,4

________________________________________________________

2) Deobfuscate - Single Step

This works like previous command, but does one transformation at a time

_______________________________________________________

3) Move NOPs to bottom

Converts this:

00874396    50                      PUSH EAX
00874397    90                      NOP
00874398    90                      NOP
00874399    52                      PUSH EDX                                    
0087439A    BA 3F976B00             MOV EDX,somesoft.006B973F
0087439F    90                      NOP
008743A0    90                      NOP
008743A1    90                      NOP

to this:

00874396    50                      PUSH EAX
00874397    52                      PUSH EDX                                    
00874398    BA 3F976B00             MOV EDX,somesoft.006B973F
0087439D    90                      NOP
0087439E    90                      NOP
0087439F    90                      NOP
008743A0    90                      NOP
008743A1    90                      NOP

Limitations: it breaks all jumps and calls pointing inwards

________________________________________________________

4) Undo / Redo

Undo or Redo last operation (from one of the above functions)

________________________________________________________

5) Retrieve Jumpy function

This will statically parse instructions and follow all jumps. This is useful
for situations, when program jumps here and there and here and there... When
it encounters some instruction, that can't be followed, it stop and copies
all parsed instruction to an allocated place in memory.

Use settings to set some parameters:
Step over calls - if set, it will step over calls, otherwise it will follow them
Step over jccs - dtto, but for Jccs
Deobfuscate - it will deobfuscate instruction, when it encounters Jcc, RET, 
  JMP reg/exp, CALL reg/exp; useful for multi-branch
  
Example:

Original:
00874389   /EB 05                   JMP SHORT somesoft.00874390
0087438B   |43                      INC EBX
0087438C   |41                      INC ECX
0087438D   |42                      INC EDX                                      
0087438E   |EB 07                   JMP SHORT somesoft.00874397
00874390   \B8 07000000             MOV EAX,7
00874395  ^ EB F4                   JMP SHORT somesoft.0087438B
00874397    C3                      RET

Result:
003B0000    B8 07000000             MOV EAX,7
003B0005    43                      INC EBX
003B0006    41                      INC ECX
003B0007    42                      INC EDX                                     
003B0008    C3                      RET

________________________________________________________  
   
6) Rebuild RSRC and Realign

This function has some limited use when unpacking. It opens the debugged file
from disc. Then it retrieves all resources and rebuilds them to one place
(currently it rebuilds them only to original place in exe). Then it realigns file
and saves it under new name.

When is this useful? For example when unpacking aspack/asprotect or some other
packers. These steal some resources from original place and put them to its own
section, therefore increasing overall size and preventing you from cutting
packer's section. It also prevents Resource hacker from displaying these resouces.
This puts all resources to one place.

I'm sure there are better tools for this, but it may come handy sometimes.

_____________________________________________________  
   
7) AsProtect Unpacker

This will unpack file packed by AsProtect, fix it, dump asprotect.dll
and print various information to text file. Please report targets, where it fails.

Limitations:
1) Doesn't find or fix SDK functions in 1.x versions (you need to find these manually).

  There are two types of these. One has a form of one or more functions called
  before OEP, that do various initializations. If they are not run, the program
  may appear expired or not run at all. Find them and run them :-)
  
  The second type is run after OEP and hides behind GetProcAddress
  with special parameters, which AsProtect (if available) redirects
  to its own code. You need to deal with these manually.

2) in 2.30 - 2.51, there are two types of stolen functions - one is PolyOEP style, the
  other is virtualized; it can fix only the former, while the latter is used
  in AsProtect itself only
  
3) it doesn't find CRC or envelope checks, but it can prevent one type of envelope
  check, which checks for E8 in jumps to API
  
4) it doesn't decrypt encrypted parts or sections

5) it doesn't find serial, fix trial etc.

6) if it has overlay, it may be broken after unpacking (for example if it needs
  to be in fixed offset in file or if it's a certificate)

Bugs:
- doesn't work with certain 1.10 variations, I will fix this when I have time

Notes:
- after unpacking files protected by AsProtect 2.x, you may need aspr_ide.dll;
get it from aspack.com and modify if needed 





