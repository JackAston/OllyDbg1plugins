# $Id
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy

log_filename = "c:\\rsaenh.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str

#BOOL CPAcquireContext(
#  __out         HCRYPTPROV* phProv,
#  __in          CHAR* pszContainer,
#  __in          DWORD dwFlags,
#  __in          PVTableProvStruc pVTable
#);
def CPAcquireContext_handler(hookcall):
	print_debug( "rsaenh.CPAAcquireContext() called!" )
	myproxy = hookcall.proxy

	_phProv = hookcall.params[0]
	_pszContainer = hookcall.params[1]
	_dwFlags = hookcall.params[2]
	_pVTable = hookcall.params[3]

	buffer = '<invalid>'
	if _pszContainter != 0:
			buffer = myproxy.readasciiz(_pszContainer)

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPAAcquireContext( %x, %s (%x), %x, %x )\r\n" % ( _phProv, buffer, _pszContainer, _dwFlags, _pvTable ))

	hookcall.sendack()
	return

#BOOL CPCreateHash(
#  __in          HCRYPTPROV hProv,
#  __in          ALG_ID Algid,
#  __in          HCRYPTKEY hKey,
#  __in          DWORD dwFlags,
#  __out         HCRYPTHASH* phHash
#);
def CPCreateHash_handler(hookcall):
	print_debug( "rsaenh.CPCreateHash() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_Algid = hookcall.params[1]
	_hKey = hookcall.params[2]
	_dwFlags = hookcall.params[3]
	_phHash = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPCreateHash( %x, %x, %x, %x, %x )\r\n" % (_hProv, _Algid, _hKey, _dwFlags, _phHash) )

	hookcall.sendack()
	return

#BOOL CPDecrypt(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey,
#  __in          HCRYPTHASH hHash,
#  __in          BOOL Final,
#  __in          DWORD dwFlags,
#  __in_out      BYTE* pbData,
#  __in_out      DWORD* pdwDataLen
#);
def CPDecrypt_handler(hookcall):
	print_debug( "rsaenh.CPDecrypt() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]
	_hHash = hookcall.params[2]
	_Final = hookcall.params[3]
	_dwFlags = hookcall.params[4]
	_pbData = hookcall.params[5]
	_pdwDataLen = hookcall.params[6]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDecrypt( %x, %x, %x, %x, %x )\r\n" % (_hProv, _Algid, _hKey, _dwFlags, _phHash) )


	hookcall.sendack()
	return

#BOOL CPDeriveKey(
#  __in          HCRYPTPROV hProv,
#  __in          ALG_ID Algid,
#  __in          HCRYPTHASH hBaseData,
#  __in          DWORD dwFlags,
#  __out         HCRYPTKEY* phKey
#);
def CPDeriveKey_handler(hookcall):
	print_debug( "rsaenh.CPDeriveKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_Algid = hookcall.params[1]
	_hBaseData = hookcall.params[2]
	_dwFlags = hookcall.params[3]
	_phKey = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDeriveKey( %x, %x, %x, %x, %x )\r\n" % (_hProv, _Algid, _hBaseData, _dwFlags, _phKey) )

	hookcall.sendack()
	return

#BOOL CPDestroyHash(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash
#);
def CPDestroyHash_handler(hookcall):
	print_debug( "rsaenh.CPDestroyHash() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDestroyHash( %x, %x )\r\n" % (_hProv, _hHash) )

	hookcall.sendack()
	return

#BOOL CPDestroyKey(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey
#);
def CPDestroyKey_handler(hookcall):
	print_debug( "rsaenh.CPDestroyKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDestroyKey( %x, %x )\r\n" % (_hProv, _hKey) )

	hookcall.sendack()
	return

#BOOL CPDuplicateHash(
#                HCRYPTPROV hUID,
#  __in          HCRYPTHASH hHash,
#  __in          DWORD* pdwReserved,
#  __in          DWORD dwFlags,
#  __out         HCRYPTHASH* phHash
#);
def CPDuplicateHash_handler(hookcall):
	print_debug( "rsaenh.CPDuplicateHash() called!" )
	myproxy = hookcall.proxy

	_hUID = hookcall.params[0]
	_hHash =  hookcall.params[1]
	_pdwReserved = hookcall.params[2]
	_dwFlags = hookcall.params[3]
	_phHash = hookcall.params[4]


	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDuplicateHash( %x, %x, %x, %x, %x )\r\n" % (_hUID, _hHash, _pdwReserved, _dwFlags, _phHash) )

	hookcall.sendack()
	return

#BOOL CPDuplicateKey(
#  __in          HCRYPTPROV hUID,
#  __in          HCRYPTKEY hKey,
#  __in          DWORD* pdwReserved,
#  __in          DWORD dwFlags,
#  __out         HCRYPTKEY* phKey
#);
def CPDuplicateKey_handler(hookcall):
	print_debug( "rsaenh.CPDuplicateKey() called!" )
	myproxy = hookcall.proxy

	_hUID = hookcall.params[0]
	_hKey = hookcall.params[1]
	_pdwReserved = hookcall.params[2]
	_dwFlags = hookcall.params[3]
	_phKey = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPDuplicateKey( %x, %x, %x, %x, %x )\r\n" % (_hUID, _hKey, _pdwReserved, _dwFlags, _phKey) )


	hookcall.sendack()
	return

#BOOL CPEncrypt(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey,
#  __in          HCRYPTHASH hHash,
#  __in          BOOL Final,
#  __in          DWORD dwFlags,
#  __in_out      BYTE* pbData,
#  __in_out      DWORD* pdwDataLen,
#  __in          DWORD dwBufLen
#);
def CPEncrypt_handler(hookcall):
	print_debug( "rsaenh.CPEncrypt() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]
	_hHash = hookcall.params[2]
	_Final =  hookcall.params[3]
	_dwFlags = hookcall.params[4]
	_pbData = hookcall.params[5]
	_pdwDataLen = hookcall.params[6]
	_dwBufLen = hookcall.params[7]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPEncrypt( %x, %x, %x, %x, %x, %x, %x, %x )\r\n" % (_hProv, _hKey, _hHash, _Final, _dwFlags, _pbData, _pdwDataLen, _dwBufLen ) )

	hookcall.sendack()
	return


#BOOL CPExportKey(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey,
#  __in          HCRYPTKEY hPubKey,
#  __in          DWORD dwBlobType,
#  __in          DWORD dwFlags,
#  __out         BYTE* pbData,
#  __in_out      DWORD* pdwDataLen
#);
def CPExportKey_handler(hookcall):
	print_debug( "rsaenh.CPExportKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]
	_hPubKey = hookcall.params[2]
	_dwBlobType = hookcall.params[3]
	_dwFlags = hookcall.params[4]
	_pbData = hookcall.params[5]
	_pdwDataLen = hookcall.params[6]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPExportKey( %x, %x, %x, %x, %x, %x, %x )\r\n" % (_hProv, _hKey, _hPubKey, _dwBlobType, _dwFlags, _pbData, _pdwDataLen ) )

	hookcall.sendack()
	return


#BOOL CPGenKey(
#  __in          HCRYPTPROV hProv,
#  __in          ALG_ID Algid,
#  __in          DWORD dwFlags,
#  __out         HCRYPTKEY* phKey
#);
def CPGenKey_handler(hookcall):
	print_debug( "rsaenh.CPGenKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_Algid = hookcall.params[1]
	_dwFlags = hookcall.params[2]
	_phKey = hookcall.params[3]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGenKey( %x, %x, %x, %x )\r\n" % (_hProv, _Algid, _dwFlags, _phKey ) )


	hookcall.sendack()
	return

#BOOL CPGenRandom(
#  __in          HCRYPTPROV hProv,
#  __in          DWORD dwLen,
#  __in_out      BYTE* pbBuffer
#);
def CPGenRandom_handler(hookcall):
	print_debug( "rsaenh.CPGenRandom() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_dwLen = hookcall.params[1]
	_pbBuffer = hookcall.params[2]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGenRandom( %x, %x, %x )\r\n" % (_hProv, _dwLen, _pbBuffer ) )



	hookcall.sendack()
	return


#BOOL CPGetHashParam(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          DWORD dwParam,
#  __out         BYTE* pbData,
#  __in_out      DWORD* pdwDataLen,
#  __in          DWORD dwFlags
#);
def CPGetHashParam_handler(hookcall):
	print_debug( "rsaenh.CPGetHashParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash =  hookcall.params[1]
	_dwParam = hookcall.params[2]
	_pbData = hookcall.params[3]
	_pdwDataLen = hookcall.params[4]
	_dwFlags = hookcall.params[5]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGetHashParam( %x, %x, %x, %x, %x, %x )\r\n" % (_hProv, _hHash, _dwParam, _pbData, _pdwDataLen, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPGetKeyParam(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey,
#  __in          DWORD dwParam,
#  __in          BYTE* pbData,
#  __in_out      DWORD* pdwDataLen,
#  __in          DWORD dwFlags
#);
def CPGetKeyParam_handler(hookcall):
	print_debug( "rsaenh.CPGetKeyParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]
	_dwParam = hookcall.params[2]
	_pbData = hookcall.params[3]
	_pdwDataLen = hookcall.params[4]
	_dwFlags = hookcall.params[5]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGetKeyParam( %x, %x, %x, %x, %x, %x )\r\n" % (_hProv, _hKey, _dwParam, _pbData, _pdwDataLen, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPGetProvParam(
#  __in          HCRYPTPROV hProv,
#  __in          DWORD dwParam,
#  __out         BYTE* pbData,
#  __in_out      DWORD* pdwDataLen,
#  __in          DWORD dwFlags
#);
def CPGetProvParam_handler(hookcall):
	print_debug( "rsaenh.CPGetProvParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_dwParam = hookcall.params[1]
	_pbData = hookcall.params[2]
	_pdwDataLen = hookcall.params[3]
	_dwFlags = hookcall.params[4]


	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGetProvParam( %x, %x, %x, %x, %x )\r\n" % (_hProv, _dwParam, _pbData, _pdwDataLen, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPGetUserKey(
#  __in          HCRYPTPROV hProv,
#  __in          DWORD dwKeySpec,
#  __out         HCRYPTKEY* phUserKey
#);
def CPGetUserKey_handler(hookcall):
	print_debug( "rsaenh.CPGetUserKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_dwKeySpec = hookcall.params[1]
	_phUserKey = hookcall.params[2]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPGetuserKey( %x, %x, %x )\r\n" % (_hProv, _dwKeySpec, _phUserKey ) )


	hookcall.sendack()
	return

#BOOL CPHashData(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          const BYTE* pbData,
#  __in          DWORD dwDataLen,
#  __in          DWORD dwFlags
#);
def CPHashData_handler(hookcall):
	print_debug( "rsaenh.CPHashData() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]
	_pbData = hookcall.params[2]
	_dwDataLen = hookcall.params[3]
	_dwFlags = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPHashData( %x, %x, %x, %x, %x )\r\n" % (_hProv, _hHash, _pbData, _dwDataLen, _dwFlags ) )

	hookcall.sendack()
	return


#BOOL CPHashSessionKey(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          HCRYPTKEY hKey,
#  __in          DWORD dwFlags
#);
def CPHashSessionKey_handler(hookcall):
	print_debug( "rsaenh.CPHashSessionKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]
	_hKey = hookcall.params[2]
	_dwFlags = hookcall.params[3]


	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPHashSessionKey( %x, %x, %x, %x )\r\n" % (_hProv, _hHash, _hKey, _dwFlags ) )


	
	hookcall.sendack()
	return

#BOOL CPImportKey(
#  __in          HCRYPTPROV hProv,
#  __in          const BYTE* pbData,
#  __in          DWORD dwDataLen,
#  __in          HCRYPTKEY hPubKey,
#  __in          DWORD dwFlags,
#  __out         HCRYPTKEY* phKey
#);
def CPImportKey_handler(hookcall):
	print_debug( "rsaenh.CPImportKey() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_pbData = hookcall.params[1]
	_dwDataLen = hookcall.params[2]
	_hPubKey = hookcall.params[3]
	_dwFlags = hookcall.params[4]
	_phKey = hookcall.params[5]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPImportKey( %x, %x, %x, %x, %x, %x )\r\n" % (_hProv, _pbData, _dwDataLen, _hPubKey, _dwFlags, _phKey ) )

	hookcall.sendack()
	return

#BOOL CPReleaseContext(
#  __in          HCRYPTPROV hProv,
#  __in          DWORD dwFlags
#);
def CPReleaseContext_handler(hookcall):
	print_debug( "rsaenh.CPReleaseContext() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_dwFlags = hookcall.params[1]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPReleaseContext( %x, %x )\r\n" % (_hProv, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPSetHashParam(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          DWORD dwParam,
#  __in          BYTE* pbData,
#  __in          DWORD dwFlags
#);
def CPSetHashParam_handler(hookcall):
	print_debug( "rsaenh.CPSetHashParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]
	_dwParam = hookcall.params[2]
	_pbData = hookcall.params[3]
	_dwFlags = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPSetHashParam( %x, %x, %x, %x, %x )\r\n" % (_hProv, _hHash, _dwParam, _pbData, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPSetKeyParam(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTKEY hKey,
#  __in          DWORD dwParam,
#  __in          BYTE* pbData,
#  __in          DWORD dwFlags
#);
def CPSetKeyParam_handler(hookcall):
	print_debug( "rsaenh.CPSetKeyParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hKey = hookcall.params[1]
	_dwParam = hookcall.params[2]
	_pbData = hookcall.params[3]
	_dwFlags = hookcall.params[4]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPSetKeyParam( %x, %x, %x, %x, %x )\r\n" % ( _hProv, _hKey, _dwParam, _pbData, _dwFlags ) )

	hookcall.sendack()
	return

#BOOL CPSetProvParam(
#  __in          HCRYPTPROV hProv,
#  __in          DWORD dwParam,
#  __in          BYTE* pbData,
#  __in          DWORD dwFlags
#);
def CPSetProvParam_handler(hookcall):
	print_debug( "rsaenh.CPSetProvParam() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_dwParam = hookcall.params[1]
	_pbData = hookcall.params[2]
	_dwFlags = hookcall.params[3]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPSetProvParam( %x, %x, %x, %x )\r\n" % ( _hProv, _dwParam, _pbData, _dwFlags ) )


	hookcall.sendack()
	return

#BOOL CPSignHash(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          DWORD dwKeySpec,
#  __in          LPCWSTR sDescription,
#  __in          DWORD dwFlags,
#  __out         BYTE* pbSignature,
#  __in_out      DWORD* pdwSigLen
#);
def CPSignHash_handler(hookcall):
	print_debug( "rsaenh.CPSignHash() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]
	_dwKeySpec = hookcall.params[2]
	_sDescription = hookcall.params[3]
	_dwFlags = hookcall.params[4]
	_pbSignature = hookcall.params[5]
	_pdwSigLen = hookcall.params[6]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPSignHash( %x, %x, %x, %x, %x, %x, %x )\r\n" % ( _hProv, _hHash, _dwKeySpec, _sDescription, _dwFlags, _pbSignatue, _pdwSigLen ) )


	hookcall.sendack()
	return

#BOOL CPVerifySignature(
#  __in          HCRYPTPROV hProv,
#  __in          HCRYPTHASH hHash,
#  __in          const BYTE* pbSignature,
#  __in          DWORD dwSigLen,
#  __in          HCRYPTKEY hPubKey,
#  __in          LPCWSTR sDescription,
#  __in          DWORD dwFlags
#);
def CPVerifySignature_handler(hookcall):
	print_debug( "rsaenh.CPVerifySignature() called!" )
	myproxy = hookcall.proxy

	_hProv = hookcall.params[0]
	_hHash = hookcall.params[1]
	_pbSignature = hookcall.params[2]
	_dwSigLen = hookcall.params[3]
	_hPubKey = hookcall.params[4]
	_sDescription = hookcall.params[5]
	_dwFlags = hookcall.params[6]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: rsaenh.CPVerifySignature( %x, %x, %x, %x, %x, %x, %x )\r\n" % ( _hProv, _hHash, _pbSignature, _dwSigLen, _hPubKey, _sDescription, _dwFlags ) )


	hookcall.sendack()
	return



