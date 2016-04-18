# $Id: proxy.py,v 1.8 2006/08/14 14:52:12 hochoa Exp $
# Author: Hernan Ochoa (hochoa@corest.com)

import socket
import struct
import string


PROXYCODE_READMEMORY = 0x01
PROXYCODE_WRITEMEMORY = 0x02
PROXYCODE_READASCIIZ = 0x03
PROXYCODE_ALLOCMEM = 0x04
PROXYCODE_FREEMEM = 0x05
PROXYCODE_CALLFUNCTION = 0x06
PROXYCODE_READUNICODE = 0x07
PROXYCODE_CHANGEREGS = 0x08
PROXYCODE_STEPIN = 0x09
PROXYCODE_SETBREAKPOINT = 0x0A
PROXYCODE_CLOSE_CONNECTION = 0x90


class Proxy:
	def __init__(self):
		self.sock = None
		self.server_host = '127.0.0.1'
		self.server_port = 6699
		self._connect()
		return

	def _readanswer(self):
		pktlen = self.sock.recv(4)
		pktcode = self.sock.recv(4)
		pktdata = self.sock.recv( struct.unpack("L", pktlen)[0] )
		return pktlen + pktcode + pktdata
		
	def _connect(self):
		self.sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect( (self.server_host, self.server_port) )
	def _disconnect(self):
		#self.sock.close()
		return
		
	def readmemory( self, memaddr, len):
		#self._connect()
		msglen = struct.pack("L", 8)
		mcode = struct.pack("L", PROXYCODE_READMEMORY)
		memaddr = struct.pack("L", memaddr)	
		memlen = struct.pack("L", len)
		msg = msglen + mcode + memaddr + memlen
		self.sock.send( msg )
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return answer[8:]

	def writememory( self, memaddr, bufflen, data):
		#self._connect()
		tlen = 8 + bufflen
		msglen = struct.pack("L", 8+tlen)
		mcode = struct.pack("L", PROXYCODE_WRITEMEMORY)
		memaddr = struct.pack("L", memaddr)
		memlen = struct.pack("L", bufflen)
		msg = msglen + mcode + memaddr + memlen + data
		self.sock.send( msg )
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return answer[8:] 
	
	def allocmemory( self, bcount):
		#self._connect()
		msglen = struct.pack("L", 8)
		mcode = struct.pack("L", PROXYCODE_ALLOCMEM)
		bytecount = struct.pack("L", bcount)
		retvalue = struct.pack("L", 0)
		msg = msglen + mcode + bytecount + retvalue
		self.sock.send( msg )
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return (struct.unpack("L", answer[8:12])[0], struct.unpack("L", answer[12:16])[0])
	
	def callfunction( self, funcaddr, params_tuple):
		#self._connect()
		msglen = struct.pack("L", 8 + (len(params_tuple)*4) )
		mcode = struct.pack("L", PROXYCODE_CALLFUNCTION)
		addr = struct.pack("L", funcaddr)
		paramcount = struct.pack("L", len(params_tuple))
		msg = msglen + mcode + addr + paramcount
		for x in xrange(0, len(params_tuple)):
			msg += struct.pack("L", params_tuple[x])
		self.sock.send( msg )
		answer = self._readanswer()
		self._disconnect()
		return struct.unpack("L", answer[8:])[0]
		
	def freememory( self, inmemaddr):
		#self._connect()
		msglen = struct.pack("L", 4)
		mcode = struct.pack("L", PROXYCODE_FREEMEM)
		memaddr = struct.pack("L", inmemaddr)
		msg = msglen + mcode + memaddr
		self.sock.send( msg )
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return struct.unpack("L", answer[8:])[0]

	def readasciiz(self, straddr):
		#self._connect()
		msglen = struct.pack("L", 4)
		mcode = struct.pack("L", PROXYCODE_READASCIIZ)
		memaddr = struct.pack("L", straddr)
		msg = msglen + mcode + memaddr
		self.sock.send(msg)
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return answer[8:]

	def readunicode(self, straddr):
		#self._connect()
		msglen = struct.pack("L", 4)
		mcode = struct.pack("L", PROXYCODE_READUNICODE)
		memaddr = struct.pack("L", straddr)
		msg = msglen + mcode + memaddr
		self.sock.send(msg)
		#answer = self.sock.recv(8192)
		answer = self._readanswer()
		self._disconnect()
		return answer[8:]

	def setbreakpoint(self, breakaddr):
		#self._connect()
		msglen = struct.pack("L", 4)
		mcode = struct.pack("L", PROXYCODE_SETBREAKPOINT)
		mbaddr = struct.pack("L", breakaddr)
		msg = msglen + mcode + mbaddr
		self.sock.send(msg)
		answer = self._readanswer()
		self._disconnect()
		return answer[8:]

	def changeregs(self, threadid, regs):
		#self._connect()
		msglen = struct.pack("L", (4*16)+4)
		mcode = struct.pack("L", PROXYCODE_CHANGEREGS)
		tid = struct.pack("L", threadid)
		# now pack all the registers
		mregs = struct.pack("LLLLLLLLLLLLLLLL" ,regs['eax'], regs['ecx'], regs['edx'] , regs['ebx'], regs['esp'],  regs['ebp'] , regs['esi'], regs['edi'], regs['eip'], regs['eflags'], regs['es'], regs['cs'], regs['ss'], regs['ds'], regs['fs'], regs['gs']  )
		msg = msglen + mcode + tid + mregs
		self.sock.send(msg)
		answer = self._readanswer()
		self._disconnect()
		return struct.unpack("L", answer[8:])[0]

	def stepin(self, threadid):
		#self._connect()
		msglen = struct.pack("L", 4+(16*4))
		mcode = struct.pack("L", PROXYCODE_STEPIN)
		# pack threaid
		mthreadid = struct.pack("L", threadid)
		# pack empty registers so the server can return the new reg values to us
		mregs = struct.pack("LLLLLLLLLLLLLLLL", 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
		msg = msglen + mcode + mthreadid + mregs
		self.sock.send(msg)
		answer = self._readanswer()
		self._disconnect()
		# unmarshall retcode
		retcode = struct.unpack("L", answer[8:8+4])[0]
		# unmarshall new regs
		newregs = dict(zip(('eax', 'ecx','edx','ebx', 'esp', 'ebp','esi', 'edi', 'eip', 'eflags', 'es', 'cs', 'ss', 'ds', 'fs', 'gs'), struct.unpack("LLLLLLLLLLLLLLLL", answer[12:])))
		return (retcode, newregs)

			
	def readbyte( self, byteaddr):
		t = self.readmemory(byteaddr, 1)
		return struct.unpack("B", t)[0]
	
	def readword( self, wordaddr):
		t = self.readmemory(wordaddr, 2)
		return struct.unpack("H", t)[0]


	def readdword( self, dwordaddr):
		t = self.readmemory( dwordaddr, 4)
		return struct.unpack("L", t)[0]

		
				

#p = Proxy()
#addr = 0x40c6c0
#print p.readasciiz( addr )
#print p.readmemory( addr, 19)
#print p.readbyte( addr  )
#print p.readword( addr  )
#print p.readdword( addr )
#mydata = "hola a todo el mundooooooo\0"
#p.writememory( addr, len(mydata),mydata)
#print p.readasciiz( addr )

#data = "esta es mi data locooooo\0"
#(retcode, addr) = p.allocmemory( len(data) )
#print "retcode: %X" % retcode
#print "memaddr: %X" % addr
#p.writememory( addr, len(data), data)
#ret = p.callfunction( 0x401000, (addr,addr) )
#print "retvalue: %x\n" % ret
#p.freememory( addr )

#print "tipo: %s" % type(loco)
#print "len: %x " % len(loco)
#numero = struct.unpack("L", loco)[0]
#print "%x" % numero


#print p.readasciiz( addr )
#retcode = p.freememory( addr )
#print "retcode: %x" % retcode




		
			

	
	


	



