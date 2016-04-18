cd "D:\VC\MyProjects\ODbgScript"

move Release\ODbgScript.dll .\
move Release\msvcrt.dll .\
move Release\msvcp60.dll .\
del /Q /F Release\*.*
del /Q /F Debug\*.*
del /Q /f *.ncb
del /Q /f *.user
del /Q /f *.opt
del /Q /f odbgscript.plg
del /Q /f doc\plugins.gid
del /Q /f doc\plugins.chw
attrib -h ODbgScript.VC9.suo
del /Q /f ODbgScript.VC9.suo
move ODbgScript.dll Release\
move msvcrt.dll Release\
move msvcp60.dll Release\
