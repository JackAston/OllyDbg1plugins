Ariadne optimizer plugin for IDA consists of 3 files:
 - ariadne.dll - framework dll which can be obtained from the official site here http://ariadne.group-ib.ru/en/download and also changed from demo to full version
 - ariadne.plw - IDA plugin which interacts with IDA
 - ariadne_stub.exe - additional stub which gets parameters, does all the optimization and passes results back to the plugin

What is this?
This is an IDA plugin which performs trace optimization of code from x86 PE file within given limits

How to install?
Just copy all these 3 files to IDA\plugins directory

How to run?
Open any x86 PE file in IDA and go to Main menu->Edit->Plugins->Ariadne optimizer or use Alt+O shortcut to show the main plugin window. Both shortcut and plugin name in the menu can be overridden by adding to the plugins.cfg the following line:
Ariadne another name  ariadne   Ctrl+D  0    WIN
description of the fields as follows, detailed description can be found in plugins.cfg file
plugin_name           filename  hotkey  arg  flags

There are several fields to be filled in the window with the following meaning:
 - Source PE file - the source file to read code to be optimized from. It should be a valid x86 file. This field should already be filled with value obtained from IDA but should be corrected in case of error
 - Temporary log file - just temporary log file in Temp directory which will be used to pass optimized code from stub to plugin and will be automatically deleted
The following parameters are passed to AirGenerateTrace function which is described in documentation:
 - Start trace RVA - start RVA in the file where the trace should start
 - End trace RVA - end RVA in the file. When reached, trace generation stops
 - Internal context start VA - start value of the internal context in VA form, may be 0. The internal context is a memory area which can be added during obfuscation (a virtual machine context, byte code, byte code handlers, and so on). If its location is known in advance, this makes the optimization much simpler. Mostly it should be used for virtual machines only
 - Internal context end VA - end value of the internal context in VA form, may be 0. The internal context is a memory area which can be added during obfuscation (a virtual machine context, byte code, byte code handlers, and so on). If its location is known in advance, this makes the optimization much simpler. Mostly it should be used for virtual machines only
The following parameters are passed to AirOptimize function which is described in documentation:
 - Static optimization - enable static optimization techniques
 - Dynamic optimization - enable dynamic optimization techniques (full version only, does nothing in demo version)
 - Preserve lower stack - preserve instructions that work with the stack above rSP pointer (closer to zero)
The following parameter is passed to AirCreateCtx function which is described in documentation:
 - Run under debugger - a flag that defines whether the file should be run under debugger and initial values should be taken from the debugger, or default values should be used
 - Run stub - run the stub with the parameters given to optimize
 - Close window - just close Ariadne optimizer window and do nothing

Under the hood
When one opens Ariadne optimizer window the following happens:
1.  Source PE file field is filled with the path of source file taken from IDA
2.  Temporary log file is generated automatically in Temp directory with alf (Ariadne Log File) prefix
3.  Start trace RVA is filled with current cursor position in IDA window
4.  End trace RVA is filled with end of current function in IDA if possible or the next instruction after the current one
5.  Other fields are filled with default values
6.  The main window with filled fields is shown
When one presses the Run stub button the following happens:
1.  Check for existence of source PE file is made and exit if it failed
2.  Stub process is run with the given parameters using command line
3.  Stub does the following:
3.1.Parses passed parameters
3.2.Disassembles passed source file
3.3.Runs the process if passed parameter Run under debugger says so, stops at Start trace RVA and reads registers values from debugger to emulator context, fills emulator context with default values otherwise
3.4.Generates trace withing limits given in Start trace RVA and End trace RVA using emulator and initial values in its context from the previous step, Internal context start VA and Internal context end VA are also used here
3.5.Optimizes the generated trace with flags passed (Static optimization, Dynamic optimization and Preserve lower stack)
3.6.Writes the optimized listing to the passed Temporary log file
4.  Plugin is waiting for the stub to finish or for the user to press the Cancel button in waiting window. In latter case the stub process is killed and optimization fails
5.  When the stub process is finished check for return value is made to ensure the optimization run successfully, exit otherwise
6.  Temporary log file is opened, read into separate IDA window and then deleted
7.  A new IDA window with optimized listing is shown

FAQ
Q: Why to use stub, why not to load dll into current IDA process?
A: IDA is a 32-bit application. Ariadne full version is x64 dll so it can't be loaded in x86 process that is why a separate x64 process is needed. Dll is made 64-bit because 32-bit processes have limitation of 4Gb on RAM usage and 64-bit process can take advantage of all the installed RAM which can be useful in case of handling large amounts of code