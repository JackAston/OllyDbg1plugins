# $Id: stepin.py,v 1.2 2006/06/30 16:27:52 hochoa Exp $
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2006, Core Security Technologies

import proxy

debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str

# LONG RegCloseKey( HKEY hKey )
def advapi32_regclosekey_handler(hookcall):
	print_debug( str(hex(hookcall.threadid)) )
	print_debug( str(hex(hookcall.procid)) )
	print_debug ( "advapi32.dll.RegCloseKey() called!" )
	print_debug ("eip: " + str( hex(hookcall.regs['eip']) ) 
	myproxy = proxy.Proxy()
	print_debug "stepin"
	(retcode, nregs) = myproxy.stepin(hookcall.threadid)
	print_debug "retcode: " + str(retcode)
	print_debug "eip: " + str( hex(nregs['eip']) )
	hookcall.sendack()


