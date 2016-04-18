# $Id: string_handling.py,v 1.5 2007/12/13 15:56:48 hochoa Exp $
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy
import sys


# UTIL FUNCTIONS
log_filename = "string_handling.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str

##########################

#LPTSTR lstrcat(      
#    LPTSTR lpString1,
#    LPTSTR lpString2
#);
def lstrcat_handler(hookcall):
	print_debug( "kernel32.lstrcat() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcat( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcatA_handler(hookcall):
	print_debug( "kernel32.lstrcatA() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcatA( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcatW_handler(hookcall):
	print_debug( "kernel32.lstrcatW() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = ''
	str2 = ''
	ustr1 = ''
	ustr2 = ''
	if _lpString1 != 0:
		ustr1 = uhookerAPI.readunicode( _lpString1 )
		for j in ustr1:
			if j != '\x00':
				str1 = str1 + j
	else:
		str1 = '<invalid>'

	if _lpString2 != 0:
		ustr2 = uhookerAPI.readunicode( _lpString2 )
		for j in ustr2:
			if j != '\x00':
				str2 = str2 + j
	else:
		str2 = '<invalid>'
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcatW( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return


#int lstrcmp(      
#    LPCTSTR lpString1,
#    LPCTSTR lpString2
#);
def lstrcmp_handler(hookcall):
	print_debug( "kernel32.lstrcmp() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )


	logf("-- kernel32.lstrcmp( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )

	hookcall.sendack()
	return

def lstrcmpA_handler(hookcall):
	print_debug( "kernel32.lstrcmpA() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcmpA( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcmpW_handler(hookcall):
	print_debug( "kernel32.lstrcmpW() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = ''
	str2 = ''
	ustr1 = ''
	ustr2 = ''
	if _lpString1 != 0:
		ustr1 = uhookerAPI.readunicode( _lpString1 )
		for j in ustr1:
			if j != '\x00':
				str1 = str1 + j
	else:
		str1 = '<invalid>'

	if _lpString2 != 0:
		ustr2 = uhookerAPI.readunicode( _lpString2 )
		for j in ustr2:
			if j != '\x00':
				str2 = str2 + j
	else:
		str2 = '<invalid>'
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcmpW( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcmpi_handler(hookcall):
	print_debug( "kernel32.lstrcmpi() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcmpi( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcmpiA_handler(hookcall):
	print_debug( "kernel32.lstrcmpiA() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = '<invalid>'
	str2 = '<invalid>'
	if _lpString1 != 0:
		str1 = uhookerAPI.readasciiz( _lpString1 )
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )

	logf("-- kernel32.lstrcmpiA( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )


	hookcall.sendack()
	return

def lstrcmpiW_handler(hookcall):
	print_debug( "kernel32.lstrcmpiW() called!" )
	uhookerAPI = hookcall.proxy

	_lpString1 = hookcall.params[0]
	_lpString2 = hookcall.params[1]

	str1 = ''
	str2 = ''
	ustr1 = ''
	ustr2 = ''
	if _lpString1 != 0:
		ustr1 = uhookerAPI.readunicode( _lpString1 )
		for j in ustr1:
			if j != '\x00':
				str1 = str1 + j
	else:
		str1 = '<invalid>'

	if _lpString2 != 0:
		ustr2 = uhookerAPI.readunicode( _lpString2 )
		for j in ustr2:
			if j != '\x00':
				str2 = str2 + j
	else:
		str2 = '<invalid>'
	
	print_debug("String1: %s\nString2: %s\n" % (str1, str2) )


	logf("-- kernel32.lstrcmpiW( %.8X (%s), %.8X (%s) )\n" % (_lpString1, str1, _lpString2, str2) )

	hookcall.sendack()
	return

#LPTSTR lstrcpy(      
#    LPTSTR lpString1,
#    LPTSTR lpString2
#);
def lstrcpy_handler(hookcall):
	print_debug( "kernel32.lstrcpy() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]

	src = '<invalid>'
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		src = uhookerAPI.readasciiz( _lpSrc )

	print_debug("Src: %s\n" % src)

	logf("-- kernel32.lstrcpy( %.8X , %.8X (%s) )\n" % (_lpDest, _lpSrc, src) )

	hookcall.sendack()
	return

def lstrcpyA_handler(hookcall):
	print_debug( "kernel32.lstrcpyA() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]

	src = '<invalid>'
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		src = uhookerAPI.readasciiz( _lpSrc )

	print_debug("Src: %s" % src)

	logf("-- kernel32.lstrcpyA( %.8X , %.8X (%s) )\n" % (_lpDest, _lpSrc, src) )

	hookcall.sendack()
	return

def lstrcpyW_handler(hookcall):
	print_debug( "kernel32.lstrcpyW() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]

	src = ''
	usrc = ''
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		usrc = uhookerAPI.readunicode( _lpSrc )
		for j in usrc:
			if j != '\x00':
				src = src + j
	else:
		src = '<invalid>'

	print_debug("Src: %s" % src)

	logf("-- kernel32.lstrcpyW( %.8X , %.8X (%s) )\n" % (_lpDest, _lpSrc, src) )

	hookcall.sendack()
	return


#LPTSTR lstrcpyn(      
#    LPTSTR lpString1,
#    LPCTSTR lpString2,
#    int iMaxLength
#);
def lstrcpyn_handler(hookcall):
	print_debug( "kernel32.lstrcpyn() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]
	_iMaxLen = hookcall.params[2]

	src = '<invalid>'
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		src = uhookerAPI.readasciiz( _lpSrc )

	print_debug("Src: %s" % src)
	print_debug("Len: %d" % _iMaxLen )


	logf("-- kernel32.lstrcpyn( %.8X , %.8X (%s), %d )\n" % (_lpDest, _lpSrc, src, _iMaxLen) )

	hookcall.sendack()
	return

def lstrcpynA_handler(hookcall):
	print_debug( "kernel32.lstrcpynA() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]
	_iMaxLen = hookcall.params[2]

	src = '<invalid>'
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		src = uhookerAPI.readasciiz( _lpSrc )

	print_debug("Src: %s" % src)
	print_debug("Len: %d" % _iMaxLen )


	logf("-- kernel32.lstrcpynA( %.8X , %.8X (%s), %d )\n" % (_lpDest, _lpSrc, src, _iMaxLen) )

	hookcall.sendack()
	return

def lstrcpynW_handler(hookcall):
	print_debug( "kernel32.lstrcpynW() called!" )
	uhookerAPI = hookcall.proxy

	_lpDest = hookcall.params[0]
	_lpSrc  = hookcall.params[1]
	_iMaxLen = hookcall.params[2]

	src = ''
	usrc = ''
	print_debug( "Dst: %.8X" % int(_lpDest) )

	if _lpSrc != 0:
		usrc = uhookerAPI.readunicode( _lpSrc )
		for j in usrc:
			if j != '\x00':
				src = src + j
	else:
		src = '<invalid>'

	print_debug("Src: %s" % src)
	print_debug("Len: %d" % _iMaxLen)


	logf("-- kernel32.lstrcpynW( %.8X , %.8X (%s), %d )\n" % (_lpDest, _lpSrc, src, _iMaxLen) )

	hookcall.sendack()
	return



#int lstrlen(      
#    LPCTSTR lpString
#);
def lstrlen_handler(hookcall):
	print_debug( "kernel32.lstrlen() called!" )
	uhookerAPI = hookcall.proxy

	_lpString  = hookcall.params[0]

	src = '<invalid>'
	if _lpSrc != 0:
		src = uhookerAPI.readasciiz( _lpString )

	print_debug("String: %s" % src)

	logf("-- kernel32.lstrlen( %.8X (%s) )\n" % (_lpString, src ) )

	hookcall.sendack()
	return

def lstrlenA_handler(hookcall):
	print_debug( "kernel32.lstrlenA() called!" )
	uhookerAPI = hookcall.proxy

	_lpString  = hookcall.params[0]

	src = '<invalid>'
	if _lpString != 0:
		src = uhookerAPI.readasciiz( _lpString )

	print_debug("String: %s" % src)


	logf("-- kernel32.lstrlenA( %.8X (%s) )\n" % (_lpString, src ) )

	hookcall.sendack()
	return

def lstrlenW_handler(hookcall):
	print_debug( "kernel32.lstrlenW() called!" )
	uhookerAPI = hookcall.proxy

	_lpString  = hookcall.params[0]

	src = ''
	usrc = ''
	if _lpString != 0:
		usrc = uhookerAPI.readunicode( _lpString )
		for j in usrc:
			if j != '\x00':
				src = src + j
	else:
		src = '<invalid>'

	print_debug("String: %s" % src)


	logf("-- kernel32.lstrlenW( %.8X (%s) )\n" % (_lpString, src ) )

	hookcall.sendack()
	return


#int CompareString(      
#    LCID Locale,
#    DWORD dwCmpFlags,
#    LPCTSTR lpString1,
#    int cchCount1,
#    LPCTSTR lpString2,
#    int cchCount2
#);

def CompareStringA_handler(hookcall):
	print_debug( "kernel32.CompareStringA() called!" )
	uhookerAPI = hookcall.proxy

	_Locale = hookcall.params[0]
	_dwCmpFlags = hookcall.params[1]
	_lpString1 = hookcall.params[2]
	_cchCount1 = hookcall.params[3]
	_lpString2 = hookcall.params[4]
	_cchCount2 = hookcall.params[5]

	print_debug("Locale: %d" % int(_Locale) )
	print_debug("dwCmpFlags: %d" % int(_dwCmpFlags) )

	str1 = '<invalid>'
	if _lpString1 != 0:
		tmp = uhookerAPI.readasciiz( _lpString1 )
		for x in xrange(0, _cchCount1-1 ):
			if str1[x] != '\x00':
				tmp = tmp + str1[x]
		str1 = tmp

	str2 = '<invalid>'
	if _lpString2 != 0:
		str2 = uhookerAPI.readasciiz( _lpString2 )
		for x in xrange( 0, _cchCount2-1 ):
			if str2[x] != '\x00':
				tmp = tmp + str2[x]
		str2 = tmp

	print_debug("String1: %s" % str1)
	print_debug("String2: %s" % str2)


	logf("-- kernel32.CompareStringA( %d, %d, %.8X (%s), %d, %.8X (%s), %d )\n" % (_Locale, _dwCmpFlags, _lpString1, str1, _cchCount1, _lpString2, str2, _cchCount2 ) )



	hookcall.sendack()
	return

def CompareStringW_handler(hookcall):
	print_debug( "kernel32.CompareStringW() called!" )
	uhookerAPI = hookcall.proxy

	_Locale = hookcall.params[0]
	_dwCmpFlags = hookcall.params[1]
	_lpString1 = hookcall.params[2]
	_cchCount1 = hookcall.params[3]
	_lpString2 = hookcall.params[4]
	_cchCount2 = hookcall.params[5]


	print_debug("Locale: %d" % _Locale )
	print_debug("dwCmpFlags: %d" % _dwCmpFlags )

	str1 = ''
	ustr1 = ''
	if _lpString1 != 0:
		ustr1 = uhookerAPI.readunicode( _lpString1 )
		for x in xrange(0, (_cchCount1-1)*2 ):
			if ustr1[x] != '\x00':
				str1 = str1 + ustr1[x]
	else:
		str1 = '<invalid>'



	str2 = ''
	ustr2 = ''
	if _lpString2 != 0:
		ustr2 = uhookerAPI.readunicode( _lpString2 )
		for x in xrange(0, (_cchCount2-1)*2):
			if ustr2[x] != '\x00':
				str2 = str2 + ustr2[x]
	else:
		str2 = '<invalid>'


	print_debug("String1: %s" % str1)
	print_debug("String2: %s" % str2)

	logf("-- kernel32.CompareStringw( %d, %d, %.8X (%s), %d, %.8X (%s), %d )\n" % (_Locale, _dwCmpFlags, _lpString1, str1, _cchCount1, _lpString2, str2, _cchCount2 ) )


	hookcall.sendack()
	return


