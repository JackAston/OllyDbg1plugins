# $Id: server.py,v 1.11 2006/09/06 14:13:39 hochoa Exp $
# This is the server that handles calls to the universal hooker
# Author: Hernan Ochoa (hochoa@corest.com)

import socket
import struct
import string
import proxy

CODE_CALLBACK			= 1
CODE_INITHOOKENTRYTABLE		= 2
CODE_MSGACK			= 3
CODE_MSGACKNOCALL		= 4
CODE_MSGACK_NOCONT		= 5

HOOKTYPE_FUNC_BEFORE		= 1
HOOKTYPE_FUNC_AFTER		= 2


server_host = '127.0.0.1'
server_port = 6666


global g_hookentrytable 
global g_hookentryinitialized 
global g_proxy 

class HookCall:
	def __init__(self):
		self.HookEntry = None
		self.client_socket = None
		self.params = []
		self.regs = {}
		self.retaddr = 0
		self.threadid = 0
		self.procid = 0
		self.proxy = 0

	def sendack( self ):
		# msg len
		data = struct.pack("L", 0)
		# msg code
		data += struct.pack("L", CODE_MSGACK)
		# send ack
		self.client_socket.send( data )
		return

	def sendacknocont( self ):
		data = struct.pack("L", 0)
		data += struct.pack("L", CODE_MSGACK_NOCONT)
		self.client_socket.send(data)
		return

	def sendacknocall( self, retvalue):
		# msg len
		data = struct.pack("L", 4)
		# msg code
		data += struct.pack("L", CODE_MSGACKNOCALL)
		# send ack
		data += struct.pack("L", retvalue)
		self.client_socket.send( data )

		
		
class HookEntry:

	def __init__(self):
		self.Name = 'undefined'
		self.FunctionAddress = 0x0
		self.ParamCount = 0
		self.BRetValue = 0
		self.TrampolineAddress = 0x0
		self.PythonCallbackname = 'undefined.undefined'
		self.hookType = 0
		return

	

def process_inithookentrytable( data ):
	global g_hookentrytable
	global g_hookentryinitialized 

	if g_hookentryinitialized == 1:
		return

	print "processing hook entry table.."
	numentries = struct.unpack("L", data[0:4])[0]
	entries = data[4:]
	print "num entries: %d" % numentries
	g_hookentrytable = dict()

	off = 0
	while numentries:
		numentries = numentries - 1
		namelen = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		funcname = entries[off:off+namelen]
		off += namelen

		namelen = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		pcallback = entries[off:off+namelen]
		off += namelen
		
		funcaddr = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		#print "func addr: %x" % funcaddr
		paramcount = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		#print "param count: %d" % paramcount
		bretvalue = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		#print "bretvalue: %d" % bretvalue
		trampoline = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		#print "trampoline: %x" % trampoline
		hooktype = struct.unpack("L", entries[off:off+4])[0]
		off += 4
		#print "hooktype: %x" % hooktype

		hentry = HookEntry()
		hentry.Name = funcname
		hentry.FunctionAddress = funcaddr
		hentry.ParamCount = paramcount
		hentry.BRetValue = bretvalue
		hentry.TrampolineAddress = trampoline
		hentry.PythonCallbackname = pcallback
		hentry.hookType = hooktype
		dictkey = "%x" % funcaddr
		g_hookentrytable[dictkey] = hentry

	g_hookentryinitialized = 1 

def process_callback( data , clientsocket):
	global g_hookentrytable
	global g_hookentryinitialized 
	global g_proxy

	# if the connection with the uhooker proxy object
	# was not created, create it now
	if g_proxy == None:
		g_proxy = proxy.Proxy()

	# print arguments of the function
	funcaddr = struct.unpack("L", data[0:4])[0]
	paramcount = struct.unpack("L", data[4:8])[0]
		
	#print "func addr: %x" % funcaddr
	#print "param count: %d" % paramcount

	off = 8
	func_params = []
	while paramcount:
		paramcount = paramcount - 1
		arg = struct.unpack("L", data[off:off+4])[0]
		func_params.append(arg)
		#print "arg: %x" % arg
		off = off + 4

	# now obtain registers
	regs = {}
	regcount = 16
	regs['eax'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['ecx'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['edx'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['ebx'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['esp'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['ebp'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['esi'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['edi'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['eip'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['eflags'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['es'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['cs'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['ss'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['ds'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['fs'] = struct.unpack("L", data[off:off+4])[0]
	off = off + 4
	regs['gs'] = struct.unpack("L", data[off:off+4])[0]
	#now obtain retaddr
	off = off + 4
	retaddr = struct.unpack("L", data[off:off+4])[0]

	#now obtain threadid
	off = off + 4
	threadid = struct.unpack("L", data[off:off+4])[0]

	#now obtain procid
	off = off + 4
	procid = struct.unpack("L", data[off:off+4])[0]


	if g_hookentryinitialized == 0:
		return

	for a in g_hookentrytable.keys():
		if a == ("%x" % funcaddr):
			callback = g_hookentrytable[a].PythonCallbackname
			pmodule, pfunc = string.split(callback, ".")

			
			if pmodule <> "None"  and pfunc <> "None":
			# does the trick to allow changing everything in runtime! yahoooo!
				cmd = """
import sys
try:
	reload( sys.modules["%s"] )
except Exception, e:
	print "exception: " + str(e)
""" % (pmodule)
				exec(cmd)

				cmd = "__import__('" + pmodule + "')." + pfunc + "(hookcall)"
				hcall = HookCall()
				hcall.HookEntry = g_hookentrytable[a]
				hcall.client_socket = clientsocket
				hcall.params = func_params
				hcall.regs = regs
				hcall.retaddr = retaddr
				hcall.threadid = threadid
				hcall.procid = procid
				hcall.proxy = g_proxy
				local_vars = {}
				local_vars["hookcall"] = hcall
				eval(cmd, globals(), local_vars)
			else:
				print "No callback specified!"


		

print "Universal Hooker server started"
g_proxy = None 
g_hookentryinitialized  = 0
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind( (server_host, server_port) )
s.listen(10)

print "waiting for connections..."
(clientsocket, addr) = s.accept()
print "connection received from",addr[0]
#print clientsocket, addr

while 1:
	data =  clientsocket.recv(4)
	msglen = struct.unpack("L", data)[0]
	#print "msglen: %d" % msglen
	data = clientsocket.recv(4)
	msgcode = struct.unpack("L", data)[0]
	#print "msgcode: %d" % msgcode
	data = clientsocket.recv( msglen )
	#print type(CODE_CALLBACK)
	if msgcode == CODE_CALLBACK:
		process_callback( data, clientsocket )
	elif msgcode == CODE_INITHOOKENTRYTABLE:
		process_inithookentrytable( data )
	else:
		print "unknown code"

	

		
			

	
	


	



