\masm32\bin\ml /c /coff /Cp PluginOlly.asm


\masm32\bin\Link /SECTION:.text,ERW  /DLL /DEF:PluginOlly.def /SUBSYSTEM:WINDOWS /LIBPATH:\masm32\lib PluginOlly.obj 

pause