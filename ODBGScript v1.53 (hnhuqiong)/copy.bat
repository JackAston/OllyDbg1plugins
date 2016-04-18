@echo off
zip -u -1 ..\backup.zip *.* doc\*.txt release\*.dll
zip -u -1 ..\ODbgScrip_V1.53_Release.zip *.* doc\*.txt release\*.dll
copy /y Release\ODbgScript.dll "D:\cracktools\Debuggers\OllyICE06217\plugin"