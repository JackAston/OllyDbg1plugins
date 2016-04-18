#include "StdAfx.h"
//#include <cstring>

HINSTANCE hinst=0;                // DLL instance
HWND      hwmain=0;               // Handle of main OllyDbg window

#include "mru.cpp"
#include "dumper.h"

// -------------------------------------------------------------------------------
// HELPER FUNCTIONS
// -------------------------------------------------------------------------------

HWND hwndOllyDbg() {
	if (!hwmain)
	  hwmain = (HWND)Plugingetvalue(VAL_HWMAIN);
	return hwmain;
}

HINSTANCE hinstModule() {
	if (!hinst)
	  hinst = (HINSTANCE)GetModuleHandle("ODbgScript.dll");
	return hinst;
}

DWORD getPluginDirectory(string &directory) {
	DWORD result;
	HINSTANCE hinst = hinstModule();
	char * p;
	char buffer[TEXTLEN]={0};
	result = GetModuleFileName(hinst,buffer,TEXTLEN);
	p = strrchr(buffer,'\\');
	if (p) {
		*p = '\0';
		directory.assign(buffer);
		return 1;
	} else
		return 0;
}

void MsgBox(string sMsg, string sTitle)
{
	MessageBox(hwndOllyDbg(),sMsg.c_str(),sTitle.c_str(),MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND);
}

void DbgMsg(int n,char* title)
{
#ifdef _DEBUG
	char data[32];
	_itoa( n, data, 10 );
	MessageBox(hwndOllyDbg(),data,title,MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST|MB_SETFOREGROUND);
#endif
}

void DbgMsg(int n,string title) 
{
#ifdef _DEBUG
	char data[32];
	_itoa( n, data, 10 );
	MessageBox(hwndOllyDbg(),data,title.c_str(),MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
#endif
}

void DbgMsgHex(int n,char* title) 
{
#ifdef _DEBUG
	char data[32];
	_itoa( n, data, 16 );
	MessageBox(hwndOllyDbg(),data,title,MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
#endif
}

void DbgMsgHex(int n,string title) 
{
#ifdef _DEBUG
	char data[32];
	_itoa( n, data, 16 );
	MessageBox(hwndOllyDbg(),data,title.c_str(),MB_OK|MB_ICONEXCLAMATION|MB_TOPMOST);
#endif
}

string ToLower(string in)
{
	in = trim(in);
	int (*pf)(int)=tolower; 
	transform(in.begin(), in.end(), in.begin(), pf);
	return in;
}

int searchx(char *SearchBuff, int BuffSize, char *SearchString, int StringLen, char wc)
{
	int i,j;

	if (BuffSize < StringLen) return -1;

	for(i=0;i<(BuffSize-StringLen);i++)
	{
		for(j=0;j<StringLen;j++)
		{
			if ((SearchString[j] != wc) 
				&&(SearchString[j] != SearchBuff[i+j]))
				break;
		}
		if (j==StringLen) return i;
	}
	return -1;
}

string trim(const string& s) // Thanks to A. Focht for this one
{
	string::size_type left = s.find_first_not_of(" \t\f\n\r");
	if( left == string::npos ) 
	{
		return string();
	} 

	string::size_type right = s.find_last_not_of(" \t\f\n\r");
	return s.substr( left, right-left+1 );
}

bool split(vector<string> &vec, const string &str, const char delim)  
{
	vector<int> pos;
	bool inQuotes = false;

	for(uint i = 0; i < str.size(); i++)
	{
		if(str[i] == '"')
			inQuotes = !inQuotes;

		if(str[i] == delim && !inQuotes)
			pos.push_back(i);
	}

	vector<int>::iterator iter = pos.begin();

	if(iter == pos.end())
	{
		vec.push_back(str);
		return false;
	}

	uint start = 0, end = 0;

	while(iter != pos.end())
	{
		end = *iter;
		vec.push_back(trim(str.substr(start, end - start)));
		start = end + 1;
		iter++;
	} 

	if(start != str.size())
		vec.push_back(trim(str.substr(start)));

	return true;
}

//reverse dword bytes
DWORD rev(DWORD dw) {
	__asm {
		push eax
		push ebx
		
		mov eax,dw
		xchg ah,al
		mov ebx, eax
		rol ebx, 16

		xchg bh,bl
		mov dw,ebx

		pop ebx
		pop eax
	}
	return dw;
}

bool is_hex(string& s)
{
    if(s.find_first_not_of("0123456789abcdefABCDEF-")!=string::npos)
		return false;

	return true;
}

bool is_dec(string &s) 
{
	int len=s.length();

	if (len<2)
		return false;

	if (s[len-1] != '.')
		return false;

	for(uint i = 0; i < len-1; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return false;
	}
	return true;
}

bool is_float(string &s) 
{
	int p=s.find('.');
	if (p == string::npos || p == s.length()-1)
		return false;

	for(uint i = 0; i < s.length()-1; i++)
	{
		if ((s[i] < '0' && s[i] != '.' ) || s[i] > '9')
			return false;
	}
	return true;
}

void ReplaceString(string &s, char* what, char* with) 
{ 
	int p=0;
	int li=strlen(with);
	int la=strlen(what);
	p=s.find(what,p);
	while (p!=string::npos) 
	{
		s.erase(p,la);
		s.insert(p,with);
		p=s.find(what,p+li);
	}
} 

//Remove characters in string for display
string CleanString(string &s)
{
	string str=s;
	size_t p;
	for (p=0; p<s.length(); p++)
		if (str[p]==0)
			str[p]=' ';
	//while ((p = str_display.find_first_of("\0")) != string::npos)
	//	str_display[p]=' ';
	return str;
}

string Str2Unicode(char* s,ulong len) 
{
	string sOut;
	sOut.resize(len*2);
    for(ulong i=0;i<len;i++) 
    { 
        sOut[i*2] = s[i]; 
    }
	return sOut;
}

string Str2Unicode(string &s)
{
	string sOut;
	ulong len=s.size();
	sOut.resize(len*2);
    for(ulong i=0;i<len;i++) 
    { 
        sOut[i*2] = s[i]; 
    }
	return sOut;
}

bool is_hexwild(string& s)
{
	string s2=s;
	ReplaceString(s2,"?","");
	return is_hex(s2);
}

bool UnquoteString(string &s, char cstart, char cend)
{
	//using namespace regex;
	if(s.length() == 0)
		return false;

	if( s.at(0) == cstart && s.at(s.length() - 1) == cend)
	{
		s = s.substr(1, s.length() - 2);
		if (cstart=='"') 
		{ 
			ReplaceString(s,"\\r\\n","\r\n");
		}
		return true;
	}
	else 
		return false;
}

int Str2Hex(string &s, string &dst) 
{
	return Str2Hex(s,dst,s.length());
}

int Str2Hex(string &s, string &dst, ulong size) 
{
	uint i = 0;
	char c[3]={0};
	dst="";
	dst.resize(size*2);
	char * p = (char*)dst.c_str();
	while(i < size)
	{
		sprintf(p+(i*2),"%02X",s.at(i));
		//dst.append(c);
		i++;
	}
	return i;
}

int Int2Hex(DWORD dw, string &dst) 
{
	char buffer [10];
	dw = sprintf(buffer,"%X",dw);
	dst = buffer; 
	return dw;
}

int Str2Rgch(string &s, char *arr, uint size)
{
	uint i = 0;
	ZeroMemory(arr, size);
	while(i < s.length() / 2)
	{
		arr[i] = (BYTE) strtoul(s.substr(i * 2, 2).c_str(), 0, 16);
		i++;
	}
	return i;
}

int Str2RgchWithWC(string &s, char* arr, uint size, char wc)
{
	uint i = 0;
	ZeroMemory(arr, size);
	while(i < s.length() / 2)
	{
		if(s[i*2] != '?')
			arr[i] = (BYTE) strtoul(s.substr(i * 2, 2).c_str(), 0, 16);
		else
			arr[i] = wc;
		i++;
	}
	return i;
}

long double strtof(string &s)
{
	long double result=0;
	sscanf(s.c_str(),"%lf",&result);
	return result;
}

char GetWildcard(string &s)
{
	vector<BYTE> stringchars;
	int i = 0;

	while(i < s.length() / 2)
	{
		char current = (char)s[i*2];
		if(current != '?')
			stringchars.push_back((BYTE) strtoul(s.substr(i * 2, 2).c_str(), 0, 16));
		i++;
	}

	// Find wildcard
	for(BYTE c = 1; i <= 0xFF; i++)
	{
		if(find(stringchars.begin(), stringchars.end(), c) == stringchars.end())
			return (char)c;
	}
	return '\0';
}

bool RgchContains(char* container, uint containerlen, char* containee, uint containeelen)
{
	if(containerlen < containeelen)
		return false;

	for(uint i = 0; i < containeelen; i++)
	{
		if(container[i] != containee[i])
			return false;
	}
	return true;
}

bool GetWildcardBytePositions(string bytestring, vector<int>* wildcardpos)
{
	int ix = 0;
	do 
	{
		ix = bytestring.find("??", ix);
		if(ix != -1)
		{
			wildcardpos->push_back(((int)ix / 2));
			ix++;
		}
	} while(ix != -1);

	for(int i = 0; i < wildcardpos->size(); i++)
		cout << wildcardpos->at(i) << endl;

	return true;
}

bool SaveDump(string fileName, DWORD ep)
{
	strCurEIP = ep;
	//OPENFILENAME ofn;
	HANDLE hHeap, hFile;
	PIMAGE_DOS_HEADER idosh;
	PIMAGE_NT_HEADERS ipeh;
	PIMAGE_SECTION_HEADER isech;
	LPBYTE lpDumpData;
	DWORD dwFrom,dwSize,dwAccBytes;
	unsigned int i;
	dwFrom = PEFileInfo.dwImageBase;
	dwSize = PEFileInfo.dwSizeOfImage;
	hHeap = HeapCreate(HEAP_NO_SERIALIZE,1,0);
	lpDumpData = (LPBYTE)HeapAlloc(hHeap,HEAP_NO_SERIALIZE | HEAP_ZERO_MEMORY,dwSize);
	dwSize = Readmemory(lpDumpData,dwFrom,dwSize,MM_RESTORE);
	idosh = (PIMAGE_DOS_HEADER)lpDumpData;
	if(idosh->e_magic != IMAGE_DOS_SIGNATURE) 
	{
		MessageBox(0, "DUMP: Bad DOS Signature!!",PNAME,MB_OK | MB_ICONEXCLAMATION);
		HeapFree(hHeap,HEAP_NO_SERIALIZE,lpDumpData);
		return false;
	}
	ipeh = (PIMAGE_NT_HEADERS)(lpDumpData + idosh->e_lfanew);
	if(ipeh->Signature != IMAGE_NT_SIGNATURE) 
	{
		MessageBox(0, "DUMP: Bad PE Signature!!",PNAME,MB_OK | MB_ICONEXCLAMATION);
		HeapFree(hHeap,HEAP_NO_SERIALIZE,lpDumpData);
		return false;
	}
	ipeh->FileHeader.NumberOfSections        = PEFileInfo.woNumOfSect;
	ipeh->OptionalHeader.ImageBase           = PEFileInfo.dwImageBase;
	ipeh->OptionalHeader.SizeOfImage         = PEFileInfo.dwSizeOfImage;
	ipeh->OptionalHeader.BaseOfCode          = PEFileInfo.dwBaseOfCode;
	ipeh->OptionalHeader.BaseOfData          = PEFileInfo.dwBaseOfData;
	ipeh->OptionalHeader.AddressOfEntryPoint = (strCurEIP - PEFileInfo.dwImageBase) ;
	isech = IMAGE_FIRST_SECTION(ipeh);
	if(FixSect) {
		for(i=0; i<(int)PEFileInfo.woNumOfSect; i++) 
		{
			strcpy((char *)(isech+i)->Name,(char *)(lpSectInfo+i)->byName);
			(isech+i)->Misc.VirtualSize = (lpSectInfo+i)->dwVSize;
			(isech+i)->VirtualAddress   = (lpSectInfo+i)->dwVOffset;
			(isech+i)->SizeOfRawData    = (lpSectInfo+i)->dwVSize;
			(isech+i)->PointerToRawData = (lpSectInfo+i)->dwVOffset;
			(isech+i)->Characteristics  = (lpSectInfo+i)->dwCharacteristics;
		}
	}

	strcpy( szFileName, fileName.c_str() );
	hFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE) 
	{
		SetFilePointer(hFile, 0, 0, FILE_BEGIN);
		WriteFile(hFile, lpDumpData, dwSize, &dwAccBytes, NULL);
		CloseHandle(hFile);
	}
	HeapFree(hHeap,HEAP_NO_SERIALIZE,lpDumpData);
	return true;
}

bool GetPEInfo(DWORD ep)
{
	unsigned int i;
	HANDLE hFile,hHeap;
	PIMAGE_DOS_HEADER idosh;
	PIMAGE_NT_HEADERS ipeh;
	PIMAGE_SECTION_HEADER isech;
	LPBYTE fbuf;
	HWND hwmain=hwndOllyDbg();
	DWORD dwFsiz,dwRsiz;
	strCurEIP = ep;
	DbgePath = (LPTSTR)Plugingetvalue(VAL_EXEFILENAME);
	DbgeName = (LPTSTR)strrchr(DbgePath,'\\');
	memset(szWorkPath,0,sizeof(szWorkPath));
	strncpy(szWorkPath,DbgePath,(DbgeName-DbgePath));
	DbgeName++;
   
	//Read Debuggee
	hFile = CreateFile(DbgePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE) 
	{
		MessageBox(hwmain,"DUMP: Can\'t Access File",PNAME,MB_OK);
		return false;
	}
	dwFsiz = GetFileSize(hFile,NULL);
	hHeap = HeapCreate(HEAP_NO_SERIALIZE,1,0);
	fbuf = (LPBYTE)HeapAlloc(hHeap, 0, dwFsiz);
	if(ReadFile(hFile,fbuf,dwFsiz,&dwRsiz,NULL) == 0) 
	{
		MessageBox(hwmain,"DUMP: Can\'t Read File ",PNAME,MB_OK);
		CloseHandle(hFile);
		HeapFree(hHeap,HEAP_NO_SERIALIZE,fbuf);
		return false;
	}
	CloseHandle(hFile);
	idosh = (PIMAGE_DOS_HEADER)fbuf;
	if(idosh->e_magic != IMAGE_DOS_SIGNATURE) 
	{
		MessageBox(hwmain,"DUMP: Bad DOS Signature!!",PNAME,MB_OK | MB_ICONEXCLAMATION);
		HeapFree(hHeap,HEAP_NO_SERIALIZE,fbuf);
		return false;
	}
	ipeh = (PIMAGE_NT_HEADERS)(fbuf + idosh->e_lfanew);
	if(ipeh->Signature != IMAGE_NT_SIGNATURE) 
	{
		MessageBox(hwmain,"DUMP: Bad PE Signature!!",PNAME,MB_OK | MB_ICONEXCLAMATION);
		HeapFree(hHeap,HEAP_NO_SERIALIZE,fbuf);
		return false;
	}
	PEFileInfo.woNumOfSect   = ipeh->FileHeader.NumberOfSections;
	PEFileInfo.dwImageBase   = ipeh->OptionalHeader.ImageBase;
	PEFileInfo.dwSizeOfImage = ipeh->OptionalHeader.SizeOfImage;
	PEFileInfo.dwBaseOfCode  = ipeh->OptionalHeader.BaseOfCode ;
	PEFileInfo.dwBaseOfData  = ipeh->OptionalHeader.BaseOfData ;
	PEFileInfo.dwAddrOfEP    = ipeh->OptionalHeader.AddressOfEntryPoint;
	lpSectInfo = (LPSECTIONINFO)malloc(sizeof(SECTIONINFO)*PEFileInfo.woNumOfSect+1);
	isech = IMAGE_FIRST_SECTION(ipeh);
	for(i=0; i<(int)PEFileInfo.woNumOfSect; i++) 
	{
		strcpy((char *)(lpSectInfo+i)->byName,(char *)(isech+i)->Name);
		(lpSectInfo+i)->dwVSize            = (isech+i)->Misc.VirtualSize;
		(lpSectInfo+i)->dwVOffset          = (isech+i)->VirtualAddress;
		(lpSectInfo+i)->dwRSize            = (isech+i)->SizeOfRawData;
		(lpSectInfo+i)->dwROffset          = (isech+i)->PointerToRawData;
		(lpSectInfo+i)->dwCharacteristics  = (isech+i)->Characteristics;
	}
	HeapFree(hHeap,HEAP_NO_SERIALIZE,fbuf);
	return true;
}

BOOL CALLBACK EnumThreadWndProc(HWND hwnd,LPARAM lParam) 
{
	memcpy((void*)lParam,&hwnd,4);
	if (IsWindowVisible(hwnd)) 
		return 0;
	return 1; 
}

HWND FindHandle(DWORD dwThreadId, string wdwClass, long x, long y) 
{

	int size,cnt=0;
	string str;
	char buffer[256]={0};
	HWND handle;
	EnumThreadWindows(dwThreadId,EnumThreadWndProc,(LPARAM) &handle);
	do {
		if (GetParent(handle)!=NULL && GetParent(handle)!=handle && GetParent(handle)!=GetDesktopWindow() )
			handle = GetParent(handle);
		else 
			break;
	} while (true); 

	POINT Point;
	Point.x = x;
	Point.y = y;

	handle = ChildWindowFromPoint(handle,Point);
	if (handle!=NULL) { 
 		GetClassName(handle,buffer,256); 
		if (StrCmpNI(buffer,wdwClass.c_str(),wdwClass.length())==0) {
			return handle;
		}
	}	
	return 0;
}

DWORD resizeDW(DWORD dw, DWORD size) {

	if (size!=0) {
		switch (size) {
		case 1:dw=dw & 0xFF;break;
		case 2:dw=dw & 0xFFFF;break;
		case 3:dw=dw & 0xFFFFFF;break;
		}
	}

	return dw;
}

void resizeSTR(string &str, DWORD size) 
{
	if (size>0 && size<str.length())
		str = str.substr(0,size);
}

bool ESPRun(void)
{
    DWORD EIP,ESP,ESP2;
	ulong cpuid;

    cpuid = Getcputhreadid();

	t_thread *pt = Findthread(cpuid);
    EIP = pt->reg.ip;
	ESP = pt->reg.r[4];

/*
    char code[2]={0};
	char pushad=0x60;
	char pushaf=0x9c;
	uint pushwf=0x669c;

   	Readmemory(code,EIP,1,MM_RESTORE);
	t_status st =  Getstatus();
	
	try
	{
		while(code[0] != pushad)
		{

          int er=Go(cpuid, 0, STEP_IN, 1, 1);
		  st =  Getstatus();
		  Suspendprocess(cpuid);
		  t_thread *pt = Findthread(cpuid);
		  EIP = pt->reg.ip;
		  Readmemory(code,EIP,1,MM_RESTORE);
		  Runsinglethread(cpuid);
		  st =  Getstatus();
		}
	}

	catch (...)
	{
		cout <<"error";
		exit(100);
	}


    ESP2 = pt->reg.r[4];
	DWORD tmpesp =ESP2 - 8;
*/
	ESP=ESP-0x04;
	Sethardwarebreakpoint(ESP, 1, HB_WRITE);
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 1, VK_F9); 

    return true;
}

string StrLastError(void) 
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
	
	string sError;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
	
	sError=(char *)lpMsgBuf;

	int len=sError.length();
	if (len>2)
		if (sError.substr(len-3,2)=="\r\n")
			sError.erase(len-3,2);
	
    LocalFree(lpMsgBuf);

	return sError; 
}

// microsec
LARGE_INTEGER MyGetTickCount(ULONGLONG timeref, bool bUseTickCount)
{
	LARGE_INTEGER result, *lpPerformanceCount, *Frequency;
	lpPerformanceCount=(LARGE_INTEGER *) malloc(8);
	if (bUseTickCount || !QueryPerformanceCounter(lpPerformanceCount))
	{	// il n'y a pas d'horloge haute précision sur le micro. On se contente de GetTickCount
		result.QuadPart = (GetTickCount()*1000) - timeref;
	}
	else
	{	// il y en a une. On va l'utiliser
		Frequency=(LARGE_INTEGER *) malloc(8);
		QueryPerformanceFrequency(Frequency);
		if (Frequency->QuadPart != 0)
			result.QuadPart = ((lpPerformanceCount->QuadPart * 1000000) / Frequency->QuadPart) - timeref;
		else
			result.QuadPart = (GetTickCount()*1000) - timeref;
		free(Frequency);
	}
	free(lpPerformanceCount);
	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Thanks Pedram Amini for this function (From Olly BP Manager).
//
BOOL get_filename_from_handle (HANDLE h_file, char *target_filename)
{
    HANDLE h_filemap;
    DWORD  file_size_hi = 0;
    char   filename[MAX_PATH + 1];
    char   *stripped;
    void   *mem;

    HMODULE h_psapi=NULL;
	lpfGetMappedFileName pGetMappedFileName = NULL;

    //dynamically resolve function pointer.
    //if (!pGetMappedFileName) {
		// resolve function pointers.
		if ((h_psapi = LoadLibraryEx("psapi.dll",NULL,LOAD_WITH_ALTERED_SEARCH_PATH)) != NULL)
			pGetMappedFileName = (lpfGetMappedFileName)  GetProcAddress(h_psapi, "GetMappedFileNameA");
	
		if (!pGetMappedFileName) 
			return FALSE;
	//}

	// initialize the filename variable.
    // memset(filename, 0, sizeof(filename));
	ZeroMemory(filename, sizeof(filename));

    // get the file size and ensure it's not zero.
    if (GetFileSize(h_file, &file_size_hi) == 0)
        return FALSE;

    // create a file mapping object.
    if ((h_filemap = CreateFileMapping(h_file, NULL, PAGE_READONLY, 0, 1, NULL)) == NULL)
        return FALSE;

    // create a file mapping to get the file name.
    if ((mem = MapViewOfFile(h_filemap, FILE_MAP_READ, 0, 0, 1)) == NULL)
        return FALSE;

    // get the mapped file's name.
    if (!pGetMappedFileName (GetCurrentProcess(), mem, filename, MAX_PATH))
        return FALSE;

    // strip the device path and copy the name to the target buffer.
    stripped = strrchr(filename, '\\');
    strncpy(target_filename, stripped + 1, 256);

    // cleanup.
    UnmapViewOfFile(mem);
    CloseHandle(h_filemap);

    return TRUE;
}

BOOL str_filename_from_handle (HANDLE h_file, string &target_filename)
{
	char strbuf[256];
	bool result;
	result=get_filename_from_handle(h_file,strbuf);
	target_filename.assign(strbuf);
	return result;
}

/* need ntdll.h, http://www.koders.com/c/fid189758E75BE39CF44E8BE174A7E0CC4605194FDE.aspx
BOOL str_filename_from_handle2 (HANDLE h_file, string &target_filename)
{
    UNICODE_STRING AdjustedName;
    UNICODE_STRING FullDosName;
    NTSTATUS Status;
    PLDR_DATA_TABLE_ENTRY tmpModule;
    HANDLE SectionHandle;
    ULONG ViewSize;
    PVOID ImageBase;
    PIMAGE_NT_HEADERS NtHeaders;
    BOOLEAN MappedAsDataFile;
    PVOID ArbitraryUserPointer;

	// Map the dll into the process 

    ViewSize = 0;
    ImageBase = 0;
    ArbitraryUserPointer = NtCurrentTeb()->Tib.ArbitraryUserPointer;
    NtCurrentTeb()->Tib.ArbitraryUserPointer = FullDosName.Buffer;
    Status = NtMapViewOfSection(SectionHandle,
                                NtCurrentProcess(),
                                &ImageBase,
                                0,
                                0,
                                NULL,
                                &ViewSize,
                                0,
                                MEM_COMMIT,
                                PAGE_READONLY);
    NtCurrentTeb()->Tib.ArbitraryUserPointer = ArbitraryUserPointer;
    if (!NT_SUCCESS(Status))
      {
        DPRINT1("map view of section failed (Status 0x%08lx)\n", Status);
        RtlFreeUnicodeString(&FullDosName);
        NtClose(SectionHandle);
        return(Status);
      }
    if (NULL != BaseAddress)
      {
        *BaseAddress = ImageBase;
      }

    //Get and check the NT headers
    NtHeaders = RtlImageNtHeader(ImageBase);
    if (NtHeaders == NULL)
      {
        DPRINT1("RtlImageNtHeaders() failed\n");
        NtUnmapViewOfSection (NtCurrentProcess (), ImageBase);
        NtClose (SectionHandle);
        RtlFreeUnicodeString(&FullDosName);
        return STATUS_UNSUCCESSFUL;
      }
}
*/

HWND GetODBGWindow(string &title, string &classname) {

	HWND main, hwnd=0;
	main = (HWND) Plugingetvalue(VAL_HWCLIENT);
	hwnd = FindWindowEx(main,NULL,classname.c_str(),title.c_str());

	return hwnd;
}

HWND GetODBGWindow(char * title, char * classname) {

	HWND main, hwnd=0;
	main = (HWND) Plugingetvalue(VAL_HWCLIENT);
	hwnd = FindWindowEx(main,NULL,classname,title);

	return hwnd;
}