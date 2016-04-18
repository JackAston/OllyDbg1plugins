OllyCallTrace v1.0 (23 October 2007)

By Stephen Fewer of Harmony Security (www.harmonysecurity.com)

----[About]-------------------------------------------------------------
OllyCallTrace is a plugin for  OllyDbg (version 1.10) to trace  the call
chain of a thread allowing you  to monitor it for irregularities to  aid
in the debugging of stack based  buffer overflows as well as to  quickly
plot the execution flow of a program you are reversing.

----[Usage]-------------------------------------------------------------
Simply install the plugin and set a breakpoint on a location you want to
trace from, e.g. ReadFile() or  WSARecv(). When this breakpoint is  hit,
activate  OllyCallTrace  and  press F7  to  begin  the automated  single
stepping and recording of the call chain. When you are finished  tracing
the  code,  pause  execution  or  disable  OllyCallTrace  and  view  the
OllyCallTrace Log to see the recorded call chain.

Double clicking on any Call/Return instruction in the OllyCallTrace  Log
window  will  bring you  to  that location  in  the OllyDbg  disassembly
window. The recorded call chain  is highlighted with blue being  for the
main module, yellow for system modules and green for all other  modules.
The call chain is also displayed in a nested format to make it easier to
read. All irregularities are marked in red. 
