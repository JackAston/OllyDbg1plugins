@echo off
zip -u -1 ..\backup.zip *.* doc\*.txt 2>NUL

REM COMMENT THIS LINE IF YOU ARE ON WIN x64
SET ProgramFiles(x86)=%ProgramFiles%

copy /y Release\ODbgScript.dll "%ProgramFiles(x86)%\_Dev\ODBG\plugins" 1>NUL
copy /y Doc\ODbgScript.txt "%ProgramFiles(x86)%\_Dev\ODBG\plugins" 1>NUL

rem copy /y Release\ODbgScript.dll "%PROGRAMFILES%\_Dev\ODBG\plugins" 1>NUL