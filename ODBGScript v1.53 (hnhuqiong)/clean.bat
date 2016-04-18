cd "D:\crack\debug\odplus\ODbgScript.1.47.VC6\ODbgScript"
move Release\ODbgScript.dll .\
del /Q /F Release\*.*
del /Q /F Debug\*.*
move ODbgScript.dll Release\
del /f odbgscript.ncb
del /f odbgscript.plg
del /f doc\plugins.gid
