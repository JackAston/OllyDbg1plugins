The Debugger further in this document means OllyDbg or ImmunityDbg as plugins for both these debuggers are based on the same principles

Ariadne optimizer plugin for Debugger consists of 3 files:
 - ariadne.dll - framework dll which can be obtained from the official site here http://ariadne.group-ib.ru/en/download and also changed from demo to full version
 - ariadne_plugin.dll - Debugger plugin which interacts with Debugger
 - ariadne_stub.exe - additional stub which gets parameters, does all the optimization and passes results back to the plugin

What is this?
This is a Debugger plugin which performs trace optimization of code from x86 PE file within given limits

How to install?
Just copy all these 3 files to Debugger plugins directory

How to run?
Open any x86 PE file in Debugger, follow necessary code fragment in disassembler window and go to Main menu->Plugins->Ariadne optimizer or use right mouse button in disassembler window and look for Ariadne optimizer item in context menu or use Alt+D shortcut in disassembler window to show the main plugin window

There are several fields to be filled in the window with the following meaning:
 - Source PE file - the source file to read code to be optimized from. It should be a valid x86 file. This field should already be filled with value obtained from Debugger but should be corrected in case of error
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
1.  Check is made if selected in disassembler window code belongs to any executable module, error otherwise
2.  Source PE file field is filled with the path of source file taken from Debugger
3.  Temporary log file is generated automatically in Temp directory with alf (Ariadne Log File) prefix
4.  Start trace RVA is filled with current cursor position in disassembler window
5.  End trace RVA is filled with end of current function in Debugger if possible or the next instruction after the current one
6.  Other fields are filled with default values
7.  The main window with filled fields is shown
When one presses the Run stub button the following happens:
1.  Check for existence of source PE file is made and exit if it failed
2.  As Ariadne disassembles file by itself and uses default imagebase Internal context start VA and Internal context end VA are recalculated if belong to the module being deobfuscated, warning otherwise
3.  Stub process is run with the given parameters using command line
4.  Stub does the following:
4.1.Parses passed parameters
4.2.Disassembles passed source file
4.3.Runs the process if passed parameter Run under debugger says so, stops at Start trace RVA and reads registers values from debugger to emulator context, fills emulator context with default values otherwise
4.4.Generates trace withing limits given in Start trace RVA and End trace RVA using emulator and initial values in its context from the previous step, Internal context start VA and Internal context end VA are also used here
4.5.Optimizes the generated trace with flags passed (Static optimization, Dynamic optimization and Preserve lower stack)
4.6.Writes the optimized listing to the passed Temporary log file
5.  The following actions are done in a separate thread which uses a marker to allow only one instance of optimizer to be run simultaneously:
5.1.Plugin is waiting for the stub to finish or for the user to press the Cancel button in waiting window. In latter case the stub process is killed and optimization fails
5.2.When the stub process is finished check for return value is made to ensure the optimization run successfully, exit otherwise
5.3.Temporary log file is checked for existence, exit if failed
5.4.Temporary log file is opened in notepad and then deleted

FAQ
Q: Why to use stub, why not to load dll into current Debugger process?
A: Debugger is a 32-bit application. Ariadne full version is x64 dll so it can't be loaded in x86 process that is why a separate x64 process is needed. Dll is made 64-bit because 32-bit processes have limitation of 4Gb on RAM usage and 64-bit process can take advantage of all the installed RAM which can be useful in case of handling large amounts of code