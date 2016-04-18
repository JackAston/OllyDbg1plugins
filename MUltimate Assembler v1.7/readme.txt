-----------------------------------------
MUltimate Assembler v1.7
http://rammichael.com/multimate-assembler
-----------------------------------------

General:
- MUltimate Assembler is a multiline (and ultimate) assembler (and 
  disassembler)
- To disassemble code, select it, and choose "MUltimate Assembler" from 
  the right click menu
- To assemble code, click the Assemble button in the assembler window

Rules:
- You must define the address your code should be assembled on, like 
  this: <00401000>
- You can use any asm commands that OllyDbg can assemble
- You can use RVA (relative virtual) addressess with a module name, 
  like this: $module.1000 or $"module".1000, or $$1000 to use the 
  module of the address definition (e.g. <$m.1000>PUSH $$3 is the same 
  as <$m.1000>PUSH $m.3)
- You can use labels, that must begin with a '@', and contain only 
  letters, numbers, and _
- You can use anonymous labels, which are defined as '@@' and are 
  referenced to as @b (or @r) for the preceding label and @f for the 
  following label
- You can use C-style strings for text and binary data (use the L prefix 
  for unicode)

Example:

<00401000>

	NOP ; This is a nop
	JMP SHORT @f

@str:
	"Hello World!\0"
	; L"Hello World!\0" ; for unicode

@@:
	PUSH @str
	CALL @print_str
	RET

<00401030>

@print_str:
	RET 4 ; TODO: Write the function
