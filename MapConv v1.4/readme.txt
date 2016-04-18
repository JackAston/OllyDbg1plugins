MapConv 1.4 plugin for OllyDBG
by godfather+, TBD and SHaG

[:Purpose:] 
    converts map files from IDA or DeDe to OllyDBG

[:Introduction:]
    Here is my first plugin for OllyDBG.
    I don't know c++ but this magnificant debugger give me patience 
    and will to do that. Please don't laugh on my c code. [gf+]
    
[:Usage:]
    Copy mapconv.dll in OllyDBG directory and then
        1. You must create .map file using IDA or DeDe
        2. Run program from OllyDBG or atach to it
        3. Select what info to replace (comments or labels)
        4. Use plugin to select map file for this process
        5. Right-click on CPU window, and 
           "Search for"/"User-defined comment" or "User-defined labels" 
           to browse the imported info from map file

[:History:]
    1.4  added dynamic resolution of the address of code section.
         should now work for dlls and other processes that don't have
         codebase = 00401000h =)
    1.3  recompiled with OllyDbg Plugin Developement Kit v1.8 + BCB60
    1.03 bug fixes and some features added
    1.02 recompiled with OllyDbg Plugin Development Kit v1.06       
    1.01 added option to replace comments and/or labels
    1.00 first release  

[:Note:]
    if you mixed up map files just delete process.udd which 
    resides in OllyDBG directory.

[:Thanks:]
    Olly: based on his bookmark sample plugin

[:Contact:]
     godfather+ : gfplus@softhome.net
            TBD : tbd@despammed.com
           SHaG : bik78@mail.ru or SHaG on EFnet
          forum : http://ollydbg.has.it
                           