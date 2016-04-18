# $Id: udpnet.py,v 1.1 2007/11/23 16:03:55 hochoa Exp $
# Author: Hernan Ochoa (hochoa@corest.com)
# (c) 2006, Core Security Technologies


import proxy
import struct
import win32api
import win32process
import win32event
import os


if 'allocated_buffers' not in globals():
	allocated_buffers = []



hexeditor_bin = "C:\Program Files\BreakPoint Software\Hex Workshop 5\hworks32.exe"

def runHookHexEditor(buff_len, data):
	data_filename = "phdata.dat"
	f = open( data_filename, "wb")
	f.write( data )
	f.close()
	
	si = win32process.STARTUPINFO()
	# Set the position in the startup info.
	si.dwX = 0
	si.dwY = 0
	si.dwXSize = 0
	si.dwYSize = 0
	si.dwFlags = 0
	# And indicate which of the items are valid.
	# Rest of startup info is default, so we leave it alone.
	# Create the process.
	info = win32process.CreateProcess(
None, # AppName
		("%s %s" % (hexeditor_bin, data_filename)), # Command line
		None, # Process Security
		None, # ThreadSecurity
		0, # Inherit Handles?
		win32process.NORMAL_PRIORITY_CLASS,
		None, # New environment
		None, # Current directory
		si) # startup info.
	# Return the handle to the process.
	# Recall info is a tuple of (hProcess, hThread, processId, threadId)
	#return info[0]
	win32event.WaitForSingleObject( info[0], win32event.INFINITE )

	f = open(data_filename, "rb")
	newdata = f.read()
	f.close();
      
	os.unlink(data_filename)

	return (len(newdata), newdata) 


#
#int sendto(
#  __in  SOCKET s,
#  __in  const char* buf,
#  __in  int len,
#  __in  int flags,
#  __in  const struct sockaddr* to,
#  __in  int tolen
#);
def sendto_handler(hookcall):
	global allocated_buffers

	uhookerAPI = hookcall.proxy
	
	# free previously allocated buffers, if they exists
	for addrs in allocated_buffers:
		print "freeing address %x" % addrs
		uhookerAPI.freememory( addrs )
	#empty list of addresses to free
	allocated_buffers = []
			
		
	


	_SOCKET = hookcall.params[0]
	_BUF = hookcall.params[1]
	_BUFLEN = hookcall.params[2]
	_FLAGS = hookcall.params[3]
	_SOCKADDRTO = hookcall.params[4]
	_TOLEN = hookcall.params[5]


        print "Socket: %x" % _SOCKET
        print "DataSize %xh (%d)" % (_BUFLEN, _BUFLEN)
       
       	if _BUF != 0:
		data = uhookerAPI.readmemory(_BUF, _BUFLEN);	
	else:
		print "BUF IS NULL!"
		hookcall.sendack()
		return


	

	(new_lendata, new_data) = runHookHexEditor(len(data), data)


	if new_lendata == _BUFLEN:
		uhookerAPI.writememory(_BUF, new_lendata, new_data)

	else:
		print "modified packet is bigger than original one!"
		print "allocating memory on intercepted process for new buffer.."
		print "NewSize: %X (%d)" % (new_lendata, new_lendata)
		(retcode, addr) = uhookerAPI.allocmemory( new_lendata )
		if retcode == 1:
			print "retcode: %d" % retcode
			print "addr: %X" % addr
			uhookerAPI.writememory(addr, new_lendata, new_data )
			pesp = hookcall.regs['esp']
			print "ESP of process: %X" % pesp
			print "Changing ptr to buff and len parameters on stack.."
			uhookerAPI.writememory( pesp+8, 4, struct.pack("L", addr) )
			uhookerAPI.writememory( pesp+0xC, 4, struct.pack("L", new_lendata) )
			# add the allocated buffer to our list to free it later
			allocated_buffers.append( addr )

		else:
			print "allocation failed!"
		print "done!"


	hookcall.sendack()
	return




	


