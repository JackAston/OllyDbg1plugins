Olly_sync v0.2
Copyright (C) 2005 Andrew Hintz < drew@overt.org, http://guh.nu >
Portions Copyright (C) 2005 Pedram Amini <pamini@idefense.com,pedram.amini@gmail.com>

Olly_sync is a plugin for ollydbg that enables synchronization with other ollydbg and IDA Pro users via ida_sync.  The ida_sync architecture enables collaboration among multiple analysts in real time.  It enables information to be shared both across different tools and different versions of the same tool.  Never again will you have to save a MAP file from IDA Pro and then load the MAP file into ollydbg.  Never again will you have to keep multiple copies of IDB files with different analyst’s notes.  Now all the information can be shared and synchronized in real time.

For a copy of the ida_sync server, please visit http://www.openrce.org

This plugin was developed for OllyDbg v1.10 (aka version 1.0.10.0)
It was tested with the ida_sync_server.py v1.0.


To Install:
	- Copy the olly_sync.dll file into your ollydbg directory.
	- The plugin will be loaded the next time ollydbg is run.


To Use:
	- Open your binary in ollydbg
	- Press Alt+1 to connect (or Plugins->olly_sync->Connect)
		+ Enter the information for the ida_sync server that you will be using.
	- Then play away:
		+ Alt+; to insert olly_sync comment (or right-click address, olly_sync->Set Comment)
		+ Alt+: to insert olly_sync name (label) (or right-click address, olly_sync->Set Name)
		+ Comments and names received will automatically appear in the ollydbg disassembly window.
		+ Alt+L will bring up the Log where you can view the names and comments that you have received from the ida_sync server.


To Compile:
	- A solution and project file has been included for Visual Studio .NET 2003.
	- Change the output path and command executable to suit your system.


Notes:

The bookmark plugin also claims the Alt+1, etc shortcut keys.

The Alt+; and Alt+: may not work on non-US keyboards.  It relies upon VK_OEM_1

OllyDbg only has one type of comments.  This plugin sends comments as regular comments and recvs all comments as regular comments.

OllyDbg's address labels are the same as IDA's address names.
