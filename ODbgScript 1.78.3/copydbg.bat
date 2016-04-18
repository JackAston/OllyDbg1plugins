@echo off
zip -u -1 ..\backup.zip *.*
copy /y Debug\ODbgScript.dll "%ProgramFiles(x86)%\_Dev\ODBG\plugins\"
rem copy /y Debug\ODbgScript.dll "%PROGRAMFILES%\_Dev\ODBG\plugins\"