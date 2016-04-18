Ollydbg Polymorphic Breakpoint Manager
 Copyright (c) 2005 - Mattwood^FRET - mattwood9@gmail.com - http://reverseengineering.online.fr/spip/

- * What does this plugin?

Classical breakpoint like 0CCh or 0EBh,0FEh are very easy to find just with a little antibreakpoint tricks likes :

00DE31FA  |.  B8 60060000   MOV     EAX,660
00DE31FF  |.  C1E8 03       SHR     EAX,3
00DE3202  |.  F2:AE         REPNE   SCAS BYTE PTR ES:[EDI]
00DE3204  |.  85C9          TEST    ECX,ECX
00DE3206  |.  74 04         JE      SHORT <0x90f.badboy>

The common way was to set a 0EBh,0FEh for bypass this little protection, but it's not very efficient.

The way of the plugin is to set a polymorphic EBFE likes :

00DE321C   .  E8 02000000   CALL    0x90f.00DE3223
00DE3221   .  DCED          FSUB    ST(5),ST
00DE3223   $  813424 371300>XOR     DWORD PTR SS:[ESP],1337
00DE322A   .- FF2424        JMP     NEAR DWORD PTR SS:[ESP]

Here the routine decrypt DCED with 1337 and the word at [00DE3221] = EBFE, but this way need to special right : the section must be writable.

So i created a second way who use the stack :

00DE320C   > \E8 00000000   CALL    0x90f.00DE3211
00DE3211   $  66:68 21FE    PUSH    0FE21
00DE3215   .  803424 CA     XOR     BYTE PTR SS:[ESP],0CA
00DE3219   .- FFE4          JMP     NEAR ESP
This way is universal.

- * The Polymorphic isn't to big for small function?

I already look at function likes GetCommandLineA :), they have a very nice padding :)

GetCommand>  A1 F435887C     MOV     EAX,DWORD PTR DS:[7C8835F4]
7C812C92     C3              RETN
7C812C93     90              NOP
7C812C94     90              NOP
7C812C95     90              NOP
7C812C96     90              NOP
7C812C97     90              NOP
7C812C98     FFFF            ???                                      ; Unknown command
7C812C9A     FFFF            ???                                      ; Unknown command
7C812C9C     0000            ADD     BYTE PTR DS:[EAX],AL
7C812C9E     0000            ADD     BYTE PTR DS:[EAX],AL

nice no? :)

- * How to delete this breakpoint?

I coded a manager for this plugin who can delete breakpoint.

- * Note?

Yep, it's a concept of the polymorphic breakpoint so i suggest you to change the default breakpoint wich i added.

Cheers,

Mattwood






