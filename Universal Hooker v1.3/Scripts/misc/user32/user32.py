# $Id
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy

log_filename = "c:\\user32.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str


#BOOL PostMessage(      
#    HWND hWnd,
#    UINT Msg,
#    WPARAM wParam,
#    LPARAM lParam
#);
def PostMessageA_handler(hookcall):
#	print_debug( "user32.dll_PostMessageA called!" )
	myproxy = hookcall.proxy

	_hWnd = hookcall.params[0]
	_Msg = hookcall.params[1]
	_wParam = hookcall.params[2]
	_lParam = hookcall.params[3]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: user32.dll,PostMessageA( %x, %x, %x, %x)\r\n" % (_hWnd, _Msg, _wParam, _lParam) )
	
	hookcall.sendack()
	return



#BOOL PostMessage(      
#    HWND hWnd,
#    UINT Msg,
#    WPARAM wParam,
#    LPARAM lParam
#);
def PostMessageW_handler(hookcall):
	print_debug( "user32.dll_PostMessageW called!" )
	myproxy = hookcall.proxy

	_hWnd = hookcall.params[0]
	_Msg = hookcall.params[1]
	_wParam = hookcall.params[2]
	_lParam = hookcall.params[3]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: user32.dll,PostMessageW( %x, %x, %x, %x)\r\n" % (_hWnd, _Msg, _wParam, _lParam) )

	hookcall.sendack()
	return

