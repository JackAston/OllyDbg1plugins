@echo off
zip -u -1 ..\backup.zip *.*

REM COMMENT THIS LINE IF YOU ARE ON WIN x64
SET ProgramFiles(x86)=%ProgramFiles%

copy /y Debug\ODbgScript.dll "%ProgramFiles(x86)%\_Dev\ODBG\plugins\"
rem copy /y Debug\ODbgScript.dll "%PROGRAMFILES%\_Dev\ODBG\plugins\"