# $Id: genhooks.py,v 1.3 2007/11/07 19:02:43 hochoa Exp $
# (c) 2007, Hernan Ochoa (hochoa@corest.com)


import os
import sys
import getopt
import string


print "Genhooks.py, complaints to hernan@gmail.com"

if len(sys.argv) < 2:
	print "This scripts takes a .DLL file and generates a .cfg and .py file to use with uhooker. it requires dumpbin.exe.\n"
	print "syntax: genhooks.py -f dllname -t hooktype <-c .cfg filename> <-p .py filename>"
	print "\t-t:  hook type. b = before, a = after, * = address"
	print "example: genhooks.py -f kernel32.dll -t b"
	sys.exit(0)


dllname = ''
hooktype = ''
cfgname = ''
pyname = ''

try:
	opts, args = getopt.getopt(sys.argv[1:], 't:f:c:p:')

except getopt.GetoptError, e:
	print e
	sys.exit(0)

for o, a in opts:
	if o == '-f':
		dllname = a
	elif o == '-t':
		hooktype = a
	elif o == '-c':
		cfgname = a
	elif o == '-p':
		pyname = a


if len(dllname) < 1:
	print "dllname is invalid!"
	sys.exit(0)

if hooktype == '':
	print 'hooktype is invalid!'
	sys.exit(0)

if hooktype != 'b' and hooktype != 'a' and hooktype != '*':
	print "Invalid hooktype!: %s" % hooktype
	sys.exit(0)
	
if dllname.find('.') == -1:
	print "the extension is missing!"
	sys.exit(0)

dllname_noext = dllname[:dllname.find('.')]

if len(pyname) < 1:
	pyname = dllname_noext + '.py'

if len(cfgname) < 1:
	cfgname = dllname_noext + '.cfg'

logname = dllname + '.log'

print "dllname: %s" % dllname
print "hooktype: %s" % hooktype
print ".cfg name: %s" % cfgname
print ".py name: %s" % pyname

print "dumping exports from " + dllname
os.system('dumpbin.exe /exports ' + dllname + ' > ' + logname)

f = open(logname, "r")
line = f.readline()


outfile = open(cfgname, "wb")
outscript = open(pyname, "wb")
outscript.write("import proxy\r\n")
general_funcs = """
log_filename = "%s.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str
""" % (dllname_noext)

outscript.write(general_funcs + "\r\n" )

breakpoint_type = string.upper(hooktype)

while line:
	if line.find("ordinal hint RVA      name") != -1:
		f.readline()	# skip empty line
		expline = f.readline()
		while expline:
			if len(expline) < 10:
				break
			parts = expline.split(" ")
			linel = len(parts)
			name = parts[linel-1].replace("\n","")
			outfile.write( dllname + ":" + name + ":" + "1" + ":" + dllname_noext + "." + name + "_handler" + ":" + breakpoint_type + "\r\n")
			expline = f.readline()

			outscript.write("def " + name + "_handler(hookcall):\r\n")
			outscript.write("\tfuncname =  \"" + dllname + "." + name + "() Called!.\"" + "\r\n")
			outscript.write("\tprint funcname\r\n")

			outscript.write("\tmyproxy = hookcall.proxy\r\n")

			outscript.write("\tlogf( \"RETADDR: %x\\r\\n\" % (hookcall.retaddr))\r\n" )
			outscript.write("\tlogf(funcname + \"\\r\\n\")\r\n")



			outscript.write("\r\n\r\n")
			outscript.write("\thookcall.sendack()\r\n")
			outscript.write("\treturn\r\n\r\n")


	try:				
		line = f.readline()
	except NameError, e:
		continue	

	
f.close()
outfile.close()
outscript.close()
			

