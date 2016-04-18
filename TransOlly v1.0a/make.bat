@echo off
\masm32\bin\rc TransOlly.rc
\masm32\bin\ml /c /coff /Cp TransOlly.asm
\masm32\bin\link /DLL /DEF:TransOlly.def /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:WINDOWS /LIBPATH:\masm32\lib TransOlly.obj TransOlly.res

del TransOlly.obj
del TransOlly.exp
del TransOlly.lib
del TransOlly.res

pause