rc /l 0x409 BASE64.rc
cl -MT -O2 /Zl /FAs /GF /W3 /nologo -I../sdk BASE64.c BASE64.RES ollydbgvc.lib -o  BASE64.dll /link /LIBPATH:../sdk/odbg /dll /machine:I386 /OPT:REF /OPT:NOWIN98 /merge:.rdata=.text

