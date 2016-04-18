if exist CLBPlus!.dll del CLBPlus!.dll
\masm32\bin\ml /c /coff /Cp CLBPlus!.asm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /MERGE:.rdata=.text /RELEASE /DLL /DEF:CLBPlus!.def CLBPlus!.obj
del CLBPlus!.obj
del CLBPlus!.exp
del CLBPlus!.lib
pause