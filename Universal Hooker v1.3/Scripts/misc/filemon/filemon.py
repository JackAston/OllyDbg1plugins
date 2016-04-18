# $Id
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy

log_filename = "c:\\filemon.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str


#HANDLE WINAPI CreateFile(
#  __in          LPCTSTR lpFileName,
#  __in          DWORD dwDesiredAccess,
#  __in          DWORD dwShareMode,
#  __in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
#  __in          DWORD dwCreationDisposition,
#  __in          DWORD dwFlagsAndAttributes,
#  __in          HANDLE hTemplateFile
#);
def CreateFileW_handler(hookcall):
	print_debug( "kernel32.CreateFileW() called!" )
	myproxy = hookcall.proxy

	_lpFilename = hookcall.params[0]
	_dwDesiredAccess = hookcall.params[1]
	_dwShareMode = hookcall.params[2]
	_lpSecurityAttributes = hookcall.params[3]
	_dwCreationDisposition = hookcall.params[4]
	_dwFlagsAndAttributes = hookcall.params[5]
	_hTemplateFile = hookcall.params[6]

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
		logf("-- kernel23.dll.CreateFileW( %s (%Xh), %Xh, %Xh, %Xh, %Xh, %Xh, %Xh) ---------------------------\r\n" % ( buffer, _lpFilename, _dwDesiredAccess, _dwShareMode, _lpSecurityAttributes, _dwCreationDisposition, _dwFlagsAndAttributes, _hTemplateFile))


	hookcall.sendack()
	return



#HANDLE WINAPI CreateFile(
#  __in          LPCTSTR lpFileName,
#  __in          DWORD dwDesiredAccess,
#  __in          DWORD dwShareMode,
#  __in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
#  __in          DWORD dwCreationDisposition,
#  __in          DWORD dwFlagsAndAttributes,
#  __in          HANDLE hTemplateFile
#);
def CreateFileA_handler(hookcall):
	print_debug( "kernel32.CreateFileA() called!" )
	myproxy = hookcall.proxy

        _lpFilename = hookcall.params[0]
	_dwDesiredAccess = hookcall.params[1]
	_dwShareMode = hookcall.params[2]
	_lpSecurityAttributes = hookcall.params[3]
	_dwCreationDisposition = hookcall.params[4]
	_dwFlagsAndAttributes = hookcall.params[5]
	_hTemplateFile = hookcall.params[6]

	buffer = '<invalid>'
	if _lpFilename != 0:
		buffer = myproxy.readasciiz( _lpFilename )
		print_debug("filename: " + buffer)

	logf("retaddr: %Xh\r\n" % (hookcall.retaddr) )
	logf("-- kernel23.dll.CreateFileA( %s (%Xh), %Xh, %Xh, %Xh, %Xh, %Xh, %Xh) ---------------------------\r\n" % ( buffer, _lpFilename, _dwDesiredAccess, _dwShareMode, _lpSecurityAttributes, _dwCreationDisposition, _dwFlagsAndAttributes, _hTemplateFile))


#	if buffer.find( "C:\\Documents and Settings\\All Users\\Application Data\\Logitech\\QuickCam\\ModelData\\I See A Ghost_{D5594FF7-6B1A-4AF3-8F21-D7A7F32ED6AD}\\texturefile.JPG" ) != -1:
#	if buffer.find("texturefile") != -1:
#			hookcall.sendacknocont()
#			return

	hookcall.sendack()
	return

#BOOL WINAPI ReadFile(
#  __in          HANDLE hFile,
#  __out         LPVOID lpBuffer,
#  __in          DWORD nNumberOfBytesToRead,
#  __out         LPDWORD lpNumberOfBytesRead,
#  __in          LPOVERLAPPED lpOverlapped
#);
def ReadFile_handler(hookcall):
	print_debug( "kernel32.ReadFile() called!" )
	myproxy = hookcall.proxy

	_hFile = hookcall.params[0]
	_lpBuffer = hookcall.params[1]
	_nNumberOfBytesToRead = hookcall.params[2]
	_lpNumberOfBytesRead = hookcall.params[3]
	_lpOverlapped = hookcall.params[4]

	logf("retaddr: %Xh\r\n" % (hookcall.retaddr) )
	logf(" -- kernel32.dll.ReadFile( %Xh, %Xh, %Xh, %Xh, %Xh) ------\r\n" % (_hFile, _lpBuffer, _nNumberOfBytesToRead, _lpNumberOfBytesRead, _lpOverlapped) )

	hookcall.sendack()
	return


#BOOL WINAPI WriteFile(
#  __in          HANDLE hFile,
#  __in          LPCVOID lpBuffer,
#__in          DWORD nNumberOfBytesToWrite,
#  __out         LPDWORD lpNumberOfBytesWritten,
#  __in          LPOVERLAPPED lpOverlapped
#);
def WriteFile_handler(hookcall):
	print_debug( "kernel32.WriteFile() called!" )
	myproxy = hookcall.proxy

	_hFile = hookcall.params[0]
	_lpBuffer = hookcall.params[1]
	_nNumberOfBytesToWrite = hookcall.params[2]
	_lpNumberOfBytesWritten = hookcall.params[3]
	_lpOverlapped = hookcall.params[4]

	logf("retaddr: %Xh\r\n" % (hookcall.retaddr) )
	logf(" -- kernel32.dll.WriteFile( %Xh, %Xh, %Xh, %Xh, %Xh) ------\r\n" % (_hFile, _lpBuffer, _nNumberOfBytesToWrite, _lpNumberOfBytesWritten, _lpOverlapped) )

	hookcall.sendack()
	return



