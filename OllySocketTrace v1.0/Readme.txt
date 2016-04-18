OllySocketTrace v1.0 (21 August 2008)

By Stephen Fewer of Harmony Security (www.harmonysecurity.com)

----[About]-------------------------------------------------------------
OllySocketTrace is a plugin for OllyDbg (version 1.10) to trace the 
socket operations being performed by a process. It will record all 
buffers being sent and received. All parameters as well as return values 
are recorded and the trace is highlighted with a unique color for each 
socket being traced. 

The socket operations currently supported are: WSASocket, WSAAccept, 
WSAConnect, WSARecv, WSARecvFrom, WSASend, WSASendTo, WSAAsyncSelect, 
WSAEventSelect, WSACloseEvent, listen, ioctlsocket, connect, bind, 
accept, socket, closesocket, shutdown, recv, recvfrom, send and sendto. 


----[Usage]-------------------------------------------------------------
Simply install the plugin and activate OllySocketTrace when you wish to 
begin tracing socket operations. OllySocketTrace will automatically 
create the breakpoints needed and record the relevant information when 
these breakpoints are hit. To view the socket trace select the 
OllySocketTrace Log. 

Double clicking on any row in the OllySocketTrace Log window will bring 
you to the callers location in the OllyDbg disassembly window. The 
recorded socket trace is highlighted with a unique color for each 
socket being traced. Right clicking on any row will give you some 
options such as to view the recorded data trace. You can also filter out 
unwanted information if you are only concerned with a specific socket. 


----[Changelog]---------------------------------------------------------
v1.0 - 21 August 2008
+ Initial release
