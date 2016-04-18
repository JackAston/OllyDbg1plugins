# $Id
# 
# Author: Hernan Ochoa (hochoa@coresecurity.com)
# (c) 2007 Core Security Technologies

import proxy

log_filename = "c:\\wintrust.log"
debug = 1

def logf( str ):
	f = open(log_filename, "ab")
	f.write( str )
	f.close()

def print_debug( str ):
	if debug:
		print str



# Defines

#WINTRUST_ACTION_GENERIC_VERIFY_V2  
#            { 0xaac56b,                                         
#              0xcd44,                                           
#              0x11d0,                                           
#              { 0x8c, 0xc2, 0x0, 0xc0, 0x4f, 0xc2, 0x95, 0xee } 
#            }

#typedef struct WINTRUST_FILE_INFO_ {
#  DWORD cbStruct;
#  LPCWSTR pcwszFilePath;
#  HANDLE hFile;
#  GUID* pgKnownSubject;
#} WINTRUST_FILE_INFO, 
# *PWINTRUCT_FILE_INFO;


#dwUIChoice
dwUIChoiceValues = { 1:"WTD_UI_ALL", 2:"WTD_UI_NONE", 3:"WTD_UI_NOBAD", 4:"WTD_UI_NOGOOD" }
WTD_UI_ALL = 1
WTD_UI_NONE = 2
WTD_UI_NOBAD = 3
WTD_UI_NOGOOD = 4
#fdwRevocationChecks
fdwRevocationChecksValues = { 0:"WTD_REVOKE_NONE", 1:"WTD_REVOKE_WHOLECHAIN" }
WTD_REVOKE_NONE = 0
WTD_REVOKE_WHOLECHAIN = 1
#dwUnionChoice
dwUnionChoiceValues = { 1:"WTD_CHOICE_FILE", 2:"WTD_CHOICE_CATALOG", 3:"WTD_CHOICE_BLOB", 4:"WTD_CHOICE_SIGNER", 5:"WTD_CHOICE_CERT"}
WTD_CHOICE_FILE = 1
WTD_CHOICE_CATALOG = 2
WTD_CHOICE_BLOB = 3
WTD_CHOICE_SIGNER = 4
WTD_CHOICE_CERT = 5
#dwStateAction
dwStateActionValues = { 0:"WTD_STATEACTION_IGNORE", 1:"WTD_STATEACTION_VERIFY", 2:"WTD_STATEACTION_CLOSE", 3:"WTD_STATE_ACTION_AUTO_CACHE", 4:"WTD_STATEACTION_AUTO_CACHE_FLUSH" }
WTD_STATEACTION_IGNORE = 0
WTD_STATEACTION_VERIFY = 1
WTD_STATEACTION_CLOSE = 2
WTD_STATEACTION_AUTO_CACHE = 3
WTD_STATEACTION_AUTO_CACHE_FLUSH = 4
#dwProvFlags (bitwise combination)
WTD_USE_IE4_TRUST_FLAG = 1
WTD_NO_IE4_CHAIN_FLAG = 2
WTD_NO_POLICY_USAGE_FLAG = 4
WTD_REVOCATION_CHECK_NONE = 16
WTD_REVOCATION_CHECK_END_CERT = 32
WTD_REVOCATION_CHECK_CHAIN = 64
WTD_REVOCATION_CHECK_CHAIN_EXCLUDE_ROOT = 128
WTD_SAFER_FLAG = 256
WTD_HASH_ONLY_FLAG = 512
WTD_USE_DEFAULT_OSVER_CHECK = 1024
WTD_LIFETIME_SIGNING_FLAG = 2048
#dwUIContext
dwUIContextValues = { 0:"WTD_UICONTEXT_EXECUTE", 1:"WTD_UICONTEXT_INSTALL" }
WTD_UICONTEXT_EXECUTE = 0
WTD_UICONTEXT_INSTALL = 1


#typedef struct _WINTRUST_DATA {
#  DWORD cbStruct;
#  LPVOID pPolicyCallbackData;
#  LPVOID pSIPClientData;
#  DWORD dwUIChoice;
#  DWORD fdwRevocationChecks;
#  DWORD dwUnionChoice;
#  union {
#    struct WINTRUST_FILE_INFO_* pFile;
#    struct WINTRUST_CATALOG_INFO_* pCatalog;
#    struct WINTRUST_BLOB_INFO_* pBlob;
#    struct WINTRUST_SGNR_INFO_* pSgnr;
#    struct WINTRUST_CERT_INFO_* pCert;
#  };
#  DWORD dwStateAction;
#  HANDLE hWVTStateData;
#  WCHAR* pwszURLReference;
#  DWORD dwProvFlags;
#  DWORD dwUIContext;
#} WINTRUST_DATA, 
# *PWINTRUST_DATA;


#LONG WINAPI WinVerifyTrust(
#  __in          HWND hWnd,
#  __in          GUID* pgActionID,
#  __in          LPVOID pWVTData
#);
def WinVerifyTrust_handler(hookcall):
	print_debug( "wintrust.dll.WinVerifyTrust() called!" )
	myproxy = hookcall.proxy

	_hWnd = hookcall.params[0]
	_pgActionID = hookcall.params[1]
	_pWVTData = hookcall.params[2]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: wintrust.WinVerifyTrust( %x, %x, %x )\r\n" % ( _hWnd, _pgActionID, _pWVTData ))

	logf("HWND hWnd: %x\r\n" % _hWnd)
	if ( _pgActionID != 0 ):
		logf("GUID* pgActionID: ")
		id = myproxy.readmemory( _pgActionID, 16 )
		for value in id:
				logf( hex(ord(value)) + " " )
		logf("\r\n")
	else:
		logf("GUID* pgActionID: <NULL>")

	logf("\r\n")

	if( _pWVTData != 0 ):
		logf("LPVOID pWVTData: \r\n")

		_cbStruct = myproxy.readdword( _pWVTData)
		_pPolicyCallbackData = myproxy.readdword( _pWVTData+4 )
		_pSIPClientData = myproxy.readdword( _pWVTData+8 )
		_dwUIChoice = myproxy.readdword( _pWVTData+12 )
		_fdwRevocationChecks = myproxy.readdword( _pWVTData+16)
		_dwUnionChoice = myproxy.readdword( _pWVTData+20)
		#    struct WINTRUST_FILE_INFO_* pFile;
		#    struct WINTRUST_CATALOG_INFO_* pCatalog;
		#    struct WINTRUST_BLOB_INFO_* pBlob;
		#    struct WINTRUST_SGNR_INFO_* pSgnr;
		#    struct WINTRUST_CERT_INFO_* pCert;
		_WINTRUST_STRUCT = myproxy.readdword( _pWVTData+24)
		_dwStateAction = myproxy.readdword( _pWVTData+28)
		_hWVTStateData = myproxy.readdword( _pWVTData+32)
		_pwszURLReference = myproxy.readdword( _pWVTData+36)
		_dwProvFlags = myproxy.readdword( _pWVTData+40)
		_dwUIContext = myproxy.readdword( _pWVTData+44)
	
		logf("\tcbStruct: %x\r\n" % (_cbStruct) )
		logf("\tpPolicyCallbackData: %x\r\n" % (_pPolicyCallbackData) )
		logf("\tpSIPClientData: %x\r\n" % (_pSIPClientData) )
		logf("\tdwUIChoice: %x (%s)\r\n" % (_dwUIChoice, dwUIChoiceValues[_dwUIChoice] ) )
		logf("\tfdwRevocationChecks: %x (%s)\r\n" % (_fdwRevocationChecks, fdwRevocationChecksValues[_fdwRevocationChecks] ) )
		logf("\tdwUnionChoice: %x (%s)\r\n" % (_dwUnionChoice, dwUnionChoiceValues[_dwUnionChoice]) )
		logf("\tWINTRUST_STRUCT: %x\r\n" % (_WINTRUST_STRUCT) )
		logf("\tdwStateAction: %x (%s)\r\n" % (_dwStateAction, dwStateActionValues[_dwStateAction]) )
		logf("\thWVTStateData: %x\r\n" % (_hWVTStateData) )
		logf("\tpwszURLReference: %x\r\n" % (_pwszURLReference) )
		logf("\tdwProvFlags: %x\r\n" % (_dwProvFlags) )
		logf("\tdwUIContext: %x (%s)\r\n" % (_dwUIContext, dwUIContextValues[_dwUIContext] ) )

		if _dwUnionChoice == WTD_CHOICE_FILE and _WINTRUST_STRUCT != 0:
				_file_cbStruct = myproxy.readdword( _WINTRUST_STRUCT)
				_file_pcwszFilePath = myproxy.readdword( _WINTRUST_STRUCT+4)
				_file_hFile = myproxy.readdword( _WINTRUST_STRUCT+8 )
				_file_pgKnownSubject = myproxy.readdword( _WINTRUST_STRUCT+12 )
				_file_filename = "<NULL>"
				if _file_pcwszFilePath != 0:
					_file_filename = myproxy.readunicode( _file_pcwszFilePath )
					buff2 = ''
					for j in _file_filename:
						if j != '\x00':
							buff2 = buff2 + j
					_file_filename = buff2
				logf("\tWINTRUST_FILE_INFO\r\n")
				logf("\t\tcbStruct: %x\r\n" % (_file_cbStruct) )
				logf("\t\tpcwszFilePath: %x (%s)\r\n" % (_file_pcwszFilePath, _file_filename) )
				logf("\t\thFile: %x\r\n" % (_file_hFile) )
				logf("\t\tpgKnownSubject: %x\r\n" % (_file_pgKnownSubject) )
				if _file_pgKnownSubject != 0:
					_file_subject_data = myproxy.readmemory( _file_pgKnownSubject, 16 )
					for value in _file_subject_data:
						logf( hex(ord(value)) + " " )
					logf("\r\n")

				logf("\r\n")

	else:
		logf("LPVOID pWVTDATA: <NULL>\r\n")

	logf("\r\n")

	hookcall.sendack()
	return

#HRESULT WinVerifyTrustEx(
#  __in          HWND hwnd,
#  __in          GUID* pgActionID,
#  __in          WINTRUST_DATA* pWinTrustData
#);
def WinVerifyTrustEx_handler(hookcall):
	print_debug( "wintrust.dll.WinVerifyTrustEx() called!" )
	myproxy = hookcall.proxy

	_hWnd = hookcall.params[0]
	_pgActionID = hookcall.params[1]
	_pWVTData = hookcall.params[2]

	logf("RETADDR: %Xh\r\n" % (hookcall.retaddr) )
	logf("CALLB: wintrust.WinVerifyTrustEx( %x, %x, %x )\r\n" % ( _hWnd, _pgActionID, _pWVTData ))

	hookcall.sendack()
	return


