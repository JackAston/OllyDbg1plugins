# $Id: regmonitor.py,v 1.2 2006/06/28 15:55:45 hochoa Exp $
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2006, Core Security Technologies

import proxy

log_filename = "regmonitor.log"
debug = 0

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str

# LONG RegOpenKeyA( HKEY hKey, LPCTSTR lpSubKey, phkResult)
def advapi32_regopenkeya_handler(hookcall):
	print_debug( "advapi32.lib.RegOpenKeyA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		buffer = myproxy.readasciiz( hookcall.params[1] )
		print_debug("subkey: " + buffer)

	logf("-- advapi32.lib.RegOpenKeyA( %Xh, %Xh, %Xh) ---------------------------\r\n" % (hookcall.params[0], hookcall.params[1], hookcall.params[2]))

	if hookcall.params[1] <> 0:
		logf("subkey" + buffer + "\r\n")

	hookcall.sendack()

# LONG RegOpenKeyExA( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
def advapi32_regopenkeyexa_handler(hookcall):
	print_debug ( "advapi32.lib.RegOpenKeyExA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		buffer = myproxy.readasciiz( hookcall.params[1] )
		print_debug (  "subkey: " + buffer  )
	logf("-- advapi32.lib.RegOpenKeyExA( %Xh, %Xh, %Xh, %Xh, %Xh) ---------------------------\r\n" % (hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3], hookcall.params[4] ))
	
	if hookcall.params[1] <> 0:
		logf("subkey: " + buffer + "\r\n")

	hookcall.sendack()

# LONG RegCloseKey( HKEY hKey )
def advapi32_regclosekey_handler(hookcall):
	print_debug ( "advapi32.dll.RegCloseKey() called!" )
	logf("-- advapi32.dll.RegCloseKey( %Xh ) ---------------------------\r\n" % (hookcall.params[0] ) )
	hookcall.sendack()

# LONG RegSetKeyValueA( HKEY hkey, LPCTSTR lpSubKey, LPCTSTR lpValueName,
#                     DWORD dwType, LPCVOID lpData, DWORD cbData)
def advapi32_regsetvaluea_handler(hookcall):
	print_debug ( "advapi32.dll.RegSetValueA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		subkey = myproxy.readasciiz( hookcall.params[1] )
		print_debug(  "subkey: " + subkey )
	if hookcall.params[2] != 0:
		valuename = myproxy.readasciiz( hookcall.params[2] )
		print_debug(  "valuename: " + valuename )

	logf("-- advapi32.dll.RegSetKeyValueA( %Xh, %Xh, %Xh, %Xh, %Xh, %Xh)\r\n" % ( hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3], hookcall.params[4], hookcall.params[5] ) )
	if hookcall.params[1] != 0:
		logf("subkey: " + subkey + "\r\n")

	if hookcall.params[2] != 0:
		logf("valuename: " + valuename + "\r\n")
	hookcall.sendack()

# LONG RegQueryValueA( HKEY hkey, LPCTSTR lpSubKey, LPTSTR lpValue,
#                   PLONG lpcbValue )
def advapi32_regqueryvaluea_handler(hookcall):
	print_debug ( "advapi32.dll.RegQueryValueA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		subkey = myproxy.readasciiz( hookcall.params[1] )
		print_debug ( "subkey: " + subkey )
	if hookcall.params[2] != 0:
		value = myproxy.readasciiz( hookcall.params[2] )
		print_debug( "value: " + value )

	logf("-- advapi32.dllRegQueryValueA( %Xh, %Xh, %Xh, %Xh )\r\n" % (hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3] ) )
	if hookcall.params[1] != 0:
		logf("subkey" + subkey + "\r\n")
	if hookcall.params[2] != 0:
		logf("value" + value + "\r\n")
	hookcall.sendack()

# LONG RegQueryValueExA( HKEY hkey, LPCTSTR lpValuename,
#      LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData,
#      LPDWORD lpcbData )
def advapi32_regqueryvalueexa_handler(hookcall):
	print_debug( "advapi32.dll.RegQueryValueExA() called!")
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		valuename = myproxy.readasciiz( hookcall.params[1] )
		print_debug( "valuename: " + valuename )
	logf("-- advapi32.RegQueryValueExA( %Xh, %Xh, %Xh, %Xh, %Xh, %Xh)\r\n" % (hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3], hookcall.params[4], hookcall.params[5]) )
	if hookcall.params[1] != 0:
		logf("valuename: " + valuename + " \r\n" )
	hookcall.sendack()


# LONG RegDeleteValueA( HKEy hKey, LPCTSTR lpValueName )
def advapi32_regdeletevaluea_handler(hookcall):
	print_debug( "advapi32.dll.RegDeleteValueA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		valuename = myproxy.readasciiz( hookcall.params[1] )
		print_debug( "valuename" + valuename )
	logf("-- advapi32.dll.RegDeleteValueA( %Xh, %Xh )\r\n" % (hookcall.params[0], hookcall.params[1]))
	if hookcall.params[1] != 0:
		logf("valuename: " +  valuename + "\r\n" )
	hookcall.sendack()


	
# LONG RegSetValueExA( HKEY hKey, LPCTSTR lpValuename, DWORD Reserved,
#                 DWORD dwType, const BYTE=lpData, DWORD cbData)
def advapi32_regsetvalueexa_handler(hookcall):
	print_debug( "advapi32.dll.RegSetValueExa() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[1] != 0:
		valuename = myproxy.readasciiz( hookcall.params[1] )
		print_debug( "valuename: " + valuename )
	logf("-- advapi32.dll.RegSetValueExa( %Xh, %Xh, %Xh, %Xh, %Xh, %Xh)\r\n" % ( hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3], hookcall.params[4], hookcall.params[5] ) )
	if hookcall.params[1] != 0:
		logf("valuename: " + valuename + "\r\n")
	hookcall.sendack()

# LONG RegEnumValue( HKEY hkey, DWORD dwIndex, LPTSTR lpValueName,
#       LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType,
#       LPBYTE lpData, LPDWORD lpcbData )
def advapi32_regenumvaluea_handler(hookcall):
	print_debug( "advapi32.dll.RegEnumValueA() called!" )
	myproxy = proxy.Proxy()
	if hookcall.params[2] != 0:
		valuename = myproxy.readasciiz( hookcall.params[2] )
		print_debug("valuename: " + valuename )
	if hookcall.params[3] != 0:
		valuename2 = myproxy.readasciiz( hookcall.params[3] )
		print_debug("valuename2: " + valuename2 )
	logf("-- advapi32.dll.RegEnumValueA( %Xh, %Xh, %Xh, %Xh, %Xh, %Xh, %Xh, %Xh)\r\n" % (hookcall.params[0], hookcall.params[1], hookcall.params[2], hookcall.params[3], hookcall.params[4], hookcall.params[5], hookcall.params[6], hookcall.params[7]) )
	if hookcall.params[2] != 0:
		logf("valuename: " + valuename + "\r\n" )
	if hookcall.params[3] != 0:
		logf("valuename2: " + valuename2 + "\r\n")
	hookcall.sendack()


