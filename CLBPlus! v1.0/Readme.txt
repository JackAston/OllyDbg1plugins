CLBPlus! plugin v1.0
Copyrights (C) 2005 by Robert Ayrapetyan [aka Broken Sword]

robert@k-medulla.com


CLBPlus! plugin extends standart capabilities of conditional log breakpointing. It uses new feature of OllyDbg (implemented from version 1.10) which allows you to pass commands to plugins from "Set conditional log breakpoint window"

NOTE:
For passing commands to plugins you MUST set "Pause program" radiobutton in "Set conditional log breakpoint" window to "On condition" or "Always" state.


Syntax of use:
CLBP command [param1] [param2] ... [paramN]

CLBP - identifier for CLBPlus! plugin
command - one of allowed commands
param1, param2, paramN - optional parameters


Example of use:
Set conditional breakpoint (SHIFT+F4) at kernel32.CreateFileA and pass
CLBP DUMP [ESP+4] 30

After each break on kernel32.CreateFileA address dumping of 30h bytes starting from "filename" address of CreteFileA function to "module.log+" file will occure ("module" - name of current debugged module)


Features of version 1.0:
- Command "DUMP" implemented. Use syntax:

CLBP DUMP dump_base_addr len


Thx to Eviloid's plugin.h
Thx to Four-F's Strings.mac

*********************************************
www.k-medulla.com
Best software protection system on the market