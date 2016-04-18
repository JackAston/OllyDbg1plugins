# $Id
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy

log_filename = "c:\\loadlibrary.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str

#HMODULE WINAPI LoadLibrary(
#  __in          LPCTSTR lpFileName
#);
def LoadLibraryA_handler(hookcall):
	print_debug( "kernel32.LoadLibraryA() called!" )
	myproxy = hookcall.proxy

	_lpFilename = hookcall.params[0]

	buffer = '<invalid>'

	if _lpFilename != 0:
		buffer = myproxy.readasciiz( _lpFilename )
		print_debug("filename: " + buffer)

	logf("retaddr: %Xh\r\n" % (hookcall.retaddr) )
	logf("-- kernel23.LoadLibraryA( %s (%Xh) ) ---------------------------\r\n" % ( buffer, _lpFilename))

	hookcall.sendack()
	return


#HMODULE WINAPI LoadLibrary(
#  __in          LPCTSTR lpFileName
#);
def LoadLibraryW_handler(hookcall):
	print_debug( "kernel32.LoadLibraryW() called!" )
	myproxy = hookcall.proxy

	_lpFilename = hookcall.params[0]

	buffer = '<invalid>'

	if _lpFilename != 0:
		buffer = myproxy.readunicode( _lpFilename )
		buffer2 = ''
		for j in buffer:
			if j != '\x00':
				buffer2 = buffer2 + j
		buffer = buffer2
		print_debug("filename: " + buffer)

	logf("retaddr: %Xh\r\n" % (hookcall.retaddr) )
	logf("-- kernel23.LoadLibraryW( %s (%Xh) ) ---------------------------\r\n" % ( buffer, _lpFilename))

	hookcall.sendack()
	return


