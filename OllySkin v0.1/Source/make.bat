@echo off
set file=OllySkin
\masm32\bin\rc %file%.rc
\masm32\bin\ml /c /coff /Cp %file%.asm
\masm32\bin\link /DLL /DEF:%file%.def /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:WINDOWS /LIBPATH:\masm32\lib %file%.obj %file%.res
if exist %file%.obj del %file%.obj
if exist %file%.exp del %file%.exp
if exist %file%.lib del %file%.lib
if exist %file%.res del %file%.res
if exist %file%.dll move "%file%.dll" "..\"