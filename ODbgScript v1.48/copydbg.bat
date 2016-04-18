@echo off
zip -u -1 ..\backup.zip *.*
copy /y Debug\ODbgScript.dll "C:\Program Files\_Dev\ODBG\plugins\"