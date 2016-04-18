#pragma once

// General functions
string ToLower(string in);
int searchx(char *SearchBuff, int BuffSize, char *SearchString, int StringLen, char wc);
bool GetWildcardBytePositions(string bytestring, vector<int>* wildcardpos);
bool RgchContains(char* container, uint containerlen, char* containee, uint containeelen);
bool is_hex(string& s);
bool is_hexwild(string& s);
bool is_dec(string &s);
bool is_float(string &s);
bool split(vector<string> &vec, const string &str, const char delim);
char GetWildcard(string &s);
int Str2Rgch(string &s, char* arr, uint size);
int Str2RgchWithWC(string &s, char* arr, uint size, char wc);
string trim(const string& sData);
bool UnquoteString(string& s, char cstart, char cend);
void DbgMsg(int n, char* title);
void DbgMsg(int n,string title="");
void DbgMsgHex(int n, char* title);
void DbgMsgHex(int n,string title="");
int Str2Hex(string &s, string &dst);
int Str2Hex(string &s, string &dst, ulong size);
long double strtof(string &s);
int Int2Hex(DWORD dw, string &dst);
void ReplaceString(string &s, char* what, char* with);

bool ESPRun(void);

HWND hwndOllyDbg();
HINSTANCE hinstModule();
HWND FindHandle(DWORD dwThreadId, string wdwClass, long x, long y);

// PE Dumper
bool SaveDump(string fileName, DWORD ep);
bool GetPEInfo(DWORD ep);


DWORD  resizeDW(DWORD dw, DWORD size);
void resizeSTR(string &str, DWORD size);
