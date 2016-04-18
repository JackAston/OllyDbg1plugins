# $Id: change_connect.py,v 1.1 2007/11/30 15:04:54 hochoa Exp $
# Author: Hernan Ochoa (hochoa@corest.com),(hernan@gmail.com)
# (c) 2007, Core Security Technologies

import proxy
import sys
import socket
import struct

#int connect(
# __in  SOCKET s,
#  __in  const struct sockaddr* name,
#  __in  int namelen
#);
def connect_handler_before(hookcall):
	uhookerAPI = hookcall.proxy

	_SOCKET = hookcall.params[0]
	_SOCKADDR = hookcall.params[1]
	_SOCKADDRLEN = hookcall.params[2]

	if _SOCKET == 0:
		print "Socket is null!"
		hookcall.sendack()
		return

	if _SOCKADDR == 0:
		print "SockAddr is NULL!"
		hookcall.sendack()
		return

	#print "SOCKET: %X" % (_SOCKET)	
	#print "SOCKADDR: %X" % (_SOCKADDR)
	#print "SOCKADDRLEN: %d" % (_SOCKADDRLEN)

	data = uhookerAPI.readmemory( _SOCKADDR, _SOCKADDRLEN )

	#for x in data:
	#	print hex(ord(x))

	sin_family = struct.unpack("H", data[0:2] )[0]
	sin_port = struct.unpack("!H", data[2:4] )[0]
	ipv4_addr = struct.unpack("!L", data[4:8])[0]

	if sin_family == 2:
		print "-> %d.%d.%d.%d:%d" % ( ord(data[4]), ord(data[5]), ord(data[6]), ord(data[7]), sin_port)

	else:
		print "family: %X " % sin_family
		print "familiy not supported!"
		hookcall.sendack()
		return

	answer = ' '
	while answer != 'y' and answer != 'n':
		print "change connect() params? (y/n): ",
		answer = sys.stdin.readline()
		answer = answer[0]

	IPaddr_str = ''
	IPaddr_num = '' 
	Port_str = ''	
	Port_num = 0 

	if answer == 'y':
		cont = 1
		while cont:
			print "New IP Address (e.g.:192.168.1.2): ",
			answer = sys.stdin.readline()
			answer = answer.replace('\n','').replace('\r','')
			try:
				IPaddr_num = socket.inet_aton( answer )
				cont = 0
				IPaddr_str = answer
			except:
				print "Invalid ip address!"
				IPaddr_num = '' 

		cont = 1
		while cont:
			print "New Port (.e.g: 21): ",
			answer = sys.stdin.readline()
			answer = answer.replace('\n', '').replace('\r','')
			try:
				Port_num = int(answer)
				if Port_num > 65535:
					print "Invalid Port!"
					Port_num =0 
					continue
				Port_str = answer
				cont = 0
			except:
				print "Invalid Port!"


		print "New address: %s:%s" % (IPaddr_str, Port_str)

		newport = struct.pack("!H", Port_num)
		newip = IPaddr_num

		uhookerAPI.writememory( _SOCKADDR+2, 2, newport)
		uhookerAPI.writememory( _SOCKADDR+4, 4, newip)

		hookcall.sendack()
		return

	else:
		hookcall.sendack()
		return


