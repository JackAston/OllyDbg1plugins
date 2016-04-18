

 = OllyDbg PE Dumper v 3.01 = (c)FKMA 
 
 This is new PE Dumper plugin for best user mode debugger OlluDbg.
 The PE Dumper is similar to OllyDump by Gigapede but fully rewritten and have
 some features:
 
 - you can dump any *.exe and *.dll from debugged process address space;
 - you can add/remove sections to/from resulting dump. If you are add new  section,
   you specify VA and size of memory region  to add as section, attributes, File Offset,
   RAW size and section name. So, now you can add to dump any memory regions created
   by protectors during debug session;
 - antidump antiprotection and most correct save dump technics: during dumping, 
   against other dumpers, PE Dumper save only present memory pages (basing on VA & Virtual size). 
   So, if between memory regions present non-allocated space, most other dumpers
   (and OllyDump too) will not save dump correctly, but PE Dumper will save all 
   correctly.
 - fix raw sizes correct only RAW size of image according to Virtual Sizes;
 - paste header from disk - use header from disk, it's clear;


This plugin not fully tested yet. If you find bug, please e-mail me.

Revision history:

3.00
---- 
	initial public release

3.01
----
	+ Added autoselect main exe module in pick module list
	* some GUI minor changes
	- remove some unused code
 

e-mail:
fkma@mmtools.ru

16.03.2005