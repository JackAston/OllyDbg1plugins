# $Id: proxy_hooker.py,v 1.1 2006/06/23 22:32:45 hochoa Exp $
# (c) 2006, Core Security Technologies
# author: Hernan Ochoa (hochoa@corest.com)

import os
import sys
if sys.platform == 'win32':
	import win32api
	import win32process
	import win32event
from threading import Thread
import time
import struct
import sys
import string    
import socket
import re
import sys, time
from select import select
from socket import socket, AF_INET, SOCK_STREAM

class HookServer:
	def __init__(self):
		self.ListenPort = 6666
		self.ListenHost = '127.0.0.1'
		self.hexeditor_bin = "C:\Program Files\BreakPoint Software\Hex Workshop 4.2\hworks32.exe"

	def runScript(self, scriptName, buff_len, buff):
		pmodule, pfunc = string.split(scriptName, ".")
		# does the trick to allow changing everything in runtime! yahoooo!
		cmd = """
import sys
try:
	reload( sys.modules["%s"] )
	
except Exception, e:
	print e
""" % (pmodule)
		exec(cmd)
		cmd = "EsotericRetValue = __import__('" + pmodule + "')." + pfunc + "(buff_len, buff)"  
		exec(cmd)
		return EsotericRetValue

		
	def run(self):
		srvsock = socket( AF_INET, SOCK_STREAM )
		srvsock.bind( (self.ListenHost, self.ListenPort) )
		srvsock.listen(5)
		while 1:
			print "waiting for client connections"
			try:    
				(clientsock, address) = srvsock.accept()
				print "incoming connection"

				# read 1 byte containing direction of packet
				# 1 = send
				# 2 = recv
				resp = clientsock.recv( 1 )
				pkt_direction = struct.unpack("B", resp)[0]
				# read 4 bytes with length of data
				resp = clientsock.recv( 4 )
				buff_len = struct.unpack("L", resp )[0]
				buff = clientsock.recv( buff_len  )
				print "buff len: %d" % buff_len
				print "recibi buffff"

				if pkt_direction == 1:
					(new_len, newbuff) = self.runScript( "hooks.hook_send_packet", buff_len, buff)
				elif pkt_direction == 2:
					(new_len, newbuff) = self.runScript( "hooks.hook_recv_packet", buff_len, buff)
				else:
					print "Unkown packet direction!. Nothing done!"


				#(new_len, newbuff) = self.runHexEditor( buff_len, buff )	

				print "hexlen: %d" % new_len
				#print "buff: %s" % newbuff
				
				# write back new len
				clientsock.send( struct.pack("L", new_len ) )
				clientsock.send( newbuff )
				#clientsock.close()

			except NameError, e:
				print e


class HookServerThread(Thread):
   def __init__(self):
	Thread.__init__(self)

   def run(self):
	hserver = HookServer()
	hserver.run()

class HookClient:
    def __init__(self):
	self.verbose 	= 0
	self.TargetHost	= ""
	self.TargetPort	= 0

    def senddata(self, buff, pkt_direction):

	hcsocket = socket( AF_INET, SOCK_STREAM )
        hcsocket.connect( ( self.TargetHost, self.TargetPort ) )
	# send 1 byte containing packet direction
	# 1 = send
	# 2 = recv
	hcsocket.send( struct.pack("B", pkt_direction) )
	hcsocket.send( struct.pack("L", len(buff) ) )
	hcsocket.send( buff )
	resp = hcsocket.recv( 4 )
	resp_len = struct.unpack("L", resp )[0]
	print "resp_len: %d" % resp_len
	resp_buff = hcsocket.recv( resp_len )
	#print "resp_buff: %s" % resp_buff
	hcsocket.close()
	return (resp_len, resp_buff)


class TCPProxy:

    def __init__(self):
        self.sockhash1   = {}
        self.sockhash2   = {}
        self.ListenPort  = 8000
        self.ListenHost  = ''
        self.ConnectPort = 0
        self.ConnectHost = ''
        self.verbose     = 0
	self.hookclient =  HookClient()
	self.hookclient.TargetHost = "localhost"
	self.hookclient.TargetPort = 6666
	self.hookSend = 0
	self.hookRecv = 0
        
        return

    def run(self):
        
        mainsocks, readsocks, writesocks = [],[],[]
        msg = 'Waiting for connections at ' + 'here' + ':' + str(self.ListenPort)
       	print msg 

        try:    
            proxysock = socket( AF_INET, SOCK_STREAM )
            proxysock.bind( (self.ListenHost, self.ListenPort) )
            proxysock.listen(5)
            mainsocks.append(proxysock)
            readsocks.append(proxysock)
        except NameError, e:
            print e
            return

        while 1:
            try:
                readables, writeables, exceptions = select(readsocks, writesocks, [])
                for sockobj in readables:
                    if sockobj in mainsocks:
                        newsock, address = sockobj.accept()
                        print 'Connected from ' + address[0]
                        if self.verbose == 1:
                            print 'Connected from', address, 'on port', self.ListenPort, '(id ' + str(id(newsock)) + ')'
                        # Connecto to host where connections will be forwarded
                        readsocks.append(newsock)
                
                        dstsock = socket( AF_INET, SOCK_STREAM)
                        dstsock.connect( ( self.ConnectHost, self.ConnectPort) )
			#ssl = socket.ssl(dstsock, None, None);
                
                        readsocks.append(dstsock)
            
                        self.sockhash1[newsock] = dstsock                
                        self.sockhash2[dstsock] = newsock
                
                    elif sockobj in readables:
                        if self.sockhash1.has_key(sockobj):
                            #data = sockobj.recv(1024)
			    try:
			    	data = sockobj.recv( 32768 )
			    except Exception, e:
				print "connection closed in recv!"
				data = ""
                            dstsock = self.sockhash1[sockobj]
                            if not data:
                                sockobj.close()
                                dstsock.close()
                                del self.sockhash1[sockobj]
                                readsocks.remove(sockobj)
                                readsocks.remove(dstsock)
                                if self.verbose == 1:
                                    print 'Connection closed (id ' + str(id(sockobj)) + ')'
                            else:
				if self.HookSend == 1:
					# contact hook before sending data
				        (hook_resp_len, hook_resp) = self.hookclient.senddata( data , 1)
                	                dstsock.send( hook_resp )
				else :
					dstsock.send( data )
                    
                        if self.sockhash2.has_key(sockobj):
			    try:
                                data = sockobj.recv(1024)
			    except Exception, e:
		                print "connection closed in recv!"
				data = ""
                            dstsock = self.sockhash2[sockobj]
                            if not data:
                                sockobj.close()
                                dstsock.close()
                                del self.sockhash2[sockobj]
                                readsocks.remove(sockobj)
                                readsocks.remove(dstsock)
                                if self.verbose == 1:
                                    print 'Connection closed (id ' + str(id(sockobj)) + ')'
                            else:
				if self.HookRecv == 1:
					# contact hook before sending data
				        (hook_resp2_len, hook_resp2) = self.hookclient.senddata( data, 2 )
        	                        dstsock.send( hook_resp2 )
				else:
                                	dstsock.send( data )
            except Exception,e:
                pass
        return
        

tcpproxy = TCPProxy()
tcpproxy.ListenPort     =  1037
tcpproxy.ConnectHost    = "localhost"
tcpproxy.ConnectPort    =  1033 
tcpproxy.verbose        = 1
tcpproxy.HookSend	= 1
tcpproxy.HookRecv	= 1
# run hook server
hs = HookServerThread()
hs.start()
# run tcp proxy
tcpproxy.run()

