cd "D:\VC\MyProjects\ODbgScript"
move Release\ODbgScript.dll .\
del /Q /F Release\*.*
del /Q /F Debug\*.*
move ODbgScript.dll Release\
del /f odbgscript.ncb
del /f odbgscript.plg
del /f doc\plugins.gid
