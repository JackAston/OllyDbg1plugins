//#define WIN32_LEAN_AND_MEAN
#pragma warning(disable: 4995 4005 4242 4047 4024)
//#pragma comment(lib, "ntdll.lib")
//#pragma comment(lib, "ollydbg.lib")
#include <windows.h>
//#include <ntifs.h>
#include <Ntsecapi.h>
#include <winioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <strsafe.h>
#include <plugin.h>
long GFT;//, GFT2;
typedef struct _STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;
HANDLE hDriver;

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryObject(
  IN HANDLE               ObjectHandle OPTIONAL,
  IN int ObjectInformationClass,
  OUT PVOID               ObjectInformation,
  IN ULONG                Length,
  OUT PULONG              ResultLength );
  
BOOL SetPrivilege(char* Priv) {
  LUID Luid;
  TOKEN_PRIVILEGES tpToken;
  HANDLE hToken;

  if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken)) {
    return FALSE;
  }

  if (!LookupPrivilegeValue(NULL, Priv, &Luid)) {
    CloseHandle (hToken);
    return FALSE;
  }

  tpToken.PrivilegeCount = 1;
  tpToken.Privileges[0].Luid = Luid;
  tpToken.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  if (!AdjustTokenPrivileges(hToken, FALSE, &tpToken, 0, NULL, NULL)) {
    CloseHandle(hToken);
    return FALSE;
  }

  CloseHandle(hToken);
  return TRUE;

}

DWORD LoadDriver(char *Name, char *file, PHANDLE pHandle)
{
  int result; // eax@2
  char c; // dl@4
  FARPROC RtlAnsiStringToUnicodeString; // ebx@5
  FARPROC NtLoadDriver; // ebp@5
  FARPROC RtlFreeUnicodeString; // esi@5
  HMODULE ntdll; // eax@5
  int v15; // edi@7
  DWORD v16; // eax@9
  char *v17; // ebx@10
  HANDLE v19; // eax@10
  int Data; // [sp+Ch] [bp-53Ch]@1
  HKEY hKey; // [sp+10h] [bp-538h]@1
  char *v22; // [sp+14h] [bp-534h]@1
  int (__stdcall *RtlNtStatusToDosError)(int); // [sp+18h] [bp-530h]@5
  PHANDLE v24; // [sp+1Ch] [bp-52Ch]@1
  ANSI_STRING v25; // [sp+20h] [bp-528h]@7
  char *v26; // [sp+24h] [bp-524h]@5
  UNICODE_STRING v27; // [sp+28h] [bp-520h]@7
  char SubKey[260]; // [sp+30h] [bp-518h]@1
  char v29[260]; // [sp+134h] [bp-414h]@7
  char FileName[260]; // [sp+238h] [bp-310h]@10
  char v31[260]; // [sp+33Ch] [bp-20Ch]@3
  char v32[260]; // [sp+440h] [bp-108h]@5
  unsigned int v33; // [sp+544h] [bp-4h]@1

  if (!SetPrivilege(SE_LOAD_DRIVER_NAME))
    return 0;
  if (!SetPrivilege(SE_DEBUG_NAME))
    return 0;
  sprintf(SubKey, "System\\CurrentControlSet\\Services\\%s", Name);
  if ( RegCreateKeyA(HKEY_LOCAL_MACHINE, SubKey, &hKey) )
  {
    result = 0;
  }
  else
  {
    Data = 1;
    RegSetValueExA(hKey, "Type", 0, 4u, (const BYTE *)&Data, 4u);
    Data = 1;
    RegSetValueExA(hKey, "ErrorControl", 0, 4u, (const BYTE *)&Data, 4u);
    Data = 3;
    RegSetValueExA(hKey, "Start", 0, 4u, (const BYTE *)&Data, 4u);
    sprintf(v31, "\\??\\%s", file);
    RegSetValueExA(hKey, "ImagePath", 0, REG_EXPAND_SZ, (const BYTE *)v31, strlen(v31));
    RegCloseKey(hKey);
    ntdll = GetModuleHandleA("ntdll.dll");
    RtlNtStatusToDosError = (int (__stdcall *)(int))GetProcAddress(ntdll, "RtlNtStatusToDosError");
    RtlAnsiStringToUnicodeString = GetProcAddress(ntdll, "RtlAnsiStringToUnicodeString");
    NtLoadDriver = GetProcAddress(ntdll, "NtLoadDriver");
    RtlFreeUnicodeString = GetProcAddress(ntdll, "RtlFreeUnicodeString");
    sprintf(v32, "\\Registry\\Machine\\System\\CurrentControlSet\\Services\\%s", Name);
    v25.Buffer = v32;
    v25.Length = strlen(v32);
    ((void (__stdcall *)(UNICODE_STRING *, ANSI_STRING *, signed int))RtlAnsiStringToUnicodeString)(&v27, &v25, 1);
    v15 = ((int (__stdcall *)(UNICODE_STRING *))NtLoadDriver)(&v27);
    ((void (__stdcall *)(UNICODE_STRING *))RtlFreeUnicodeString)(&v27);

    sprintf(v29, "%s\\Enum", SubKey);
    RegDeleteKeyA(HKEY_LOCAL_MACHINE, v29);
    sprintf(v29, "%s\\Security", SubKey);
    RegDeleteKeyA(HKEY_LOCAL_MACHINE, v29);
    RegDeleteKeyA(HKEY_LOCAL_MACHINE, SubKey);
    if ( v15 && v15 != 0xC000010E )
    {
      SetLastError(RtlNtStatusToDosError(v15));
      result = 0;
    }
    else
    {
      sprintf(FileName, "\\\\.\\%s", Name);
      *pHandle = CreateFileA(FileName, 0xC0000000u, 0, 0, 3u, 128u, 0);
      if ( *pHandle == (HANDLE)-1 )
      {
        sprintf(FileName, "\\\\.\\Global\\%s", Name);
        *pHandle = CreateFileA(FileName, 0xC0000000u, 0, 0, 3u, 0x80u, 0);
      }
      result = *pHandle != (HANDLE)-1;
    }
  }
  return result;
}


/*int LoadHandles()
{
  int result; // eax@1
  signed int v1; // esi@1
  void *v2; // eax@1
  void *v3; // eax@4
  int v4; // [sp+10h] [bp-4h]@1

  v1 = 16384;
  v4 = 16384;
  v2 = malloc(0x4000u);
  HandlesInfo = v2;
  for ( result = NtQuerySystemInformation( 16 , v2, 16384, &v4);
        result;
        result = NtQuerySystemInformation( 16, v3, v4, &v4) )
  {
    if ( result == -1073741790 )
      break;
    if ( result == -1073741670 )
      break;
    free(HandlesInfo);
    v1 *= 2;
    v4 = v1;
    v3 = malloc(v1);
    HandlesInfo = v3;
  }
  return result;

} */

typedef struct tag_InBuf{
    long pid;
    void* Object;  
    long IsFile;  
    HANDLE handle;
  } InBuf;
BOOL IoControl(DWORD dwIoControlCode, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize)
{
  DWORD BytesReturned;

  return DeviceIoControl(
           hDriver,
           dwIoControlCode,
           lpInBuffer,
           nInBufferSize,
           lpOutBuffer,
           nOutBufferSize,
           &BytesReturned,
           0) != 0;
}
int __stdcall HookGetFileType(HANDLE h)
{
  HANDLE hTh;
  hTh = CreateThread(0,0,GFT,h,0,0);
  if (WaitForSingleObject(hTh, 50)==WAIT_OBJECT_0)
  {
    CloseHandle(hTh);
    return ((int (__stdcall *)(HANDLE h))GFT)(h);
  }
  TerminateThread(hTh, 0);
  CloseHandle(hTh);
  return FILE_TYPE_PIPE;
}

int __stdcall GetName(char* type, int pid, HANDLE handle, HANDLE lochandle, void *Object, void* buf, int bsz, int* pReqSz)
{
  InBuf InBuffer;
  int l, tmp, v;
 // char* type;

  if (pReqSz)
    *pReqSz = 0;

  InBuffer.pid = pid;
  InBuffer.Object = Object;
  

 // if (!ZwQueryObject(handle, 2, 0, 0, &l))
 //   return 123;
  
//  type = (char*)malloc(l+1);
  
 // InBuffer.IsFile = 0;
 // if (!ZwQueryObject(handle, 2, type, l, 0))
 // {
    InBuffer.IsFile = _stricmp(type, "file") == 0;
 // }

   
 // free(type);
  InBuffer.handle = handle;
  tmp = *(int*)(type-4);
  v = IoControl(0x83350000u, &InBuffer, 0x10u, type-4, 4099);
  *(int*)(type-4) = tmp;
  if ( v )
  {
    *pReqSz = *((DWORD*)buf+1);
    return 0;
  }
  else
  {
    return 123; 
  }
}

__declspec(naked) void __stdcall HookProc()
{               
  __asm
  {
    pop ecx //ret
    pop edx //loc handle
    pop eax // 1
    mov eax, [esp+2Ch+4-0ch]
    push dword ptr ds:[eax+8]
    push edx
    movzx edx, word ptr ds:[eax+6]
    push edx
    /*
    handle
    lochandle
    object
    buf
    bsz
    pReqSz
    */
    push dword ptr ds:[eax] //pid
    lea eax, [esp+0x40]
    push eax //name
    add ecx, 0x33
    push ecx //ret
    jmp GetName
  }
}

int __cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,unsigned long *features)
{
  char *olly, *ptc;
  char fn[260], fn2[260], fn3[260];
  int op;
  if (ollydbgversion!=PLUGIN_VERSION)
    return -1;
//  if (!SetLoadDriverPrivilege())
//    return -1;
  GetCurrentDirectory(260, fn);
  strcat(fn, "\\PROCEXP141.sys");

  GetSystemDirectoryA(&fn2, 0x104u);
  sprintf(fn3, "%s\\Drivers\\%s", fn2, "PROCEXP141.SYS");
  CopyFile(fn, fn3, 0);

  LoadDriver("PROCEXP141", fn3, &hDriver);
  olly = (char*)Plugingetvalue(VAL_HINST);
  ptc = olly+0x9D075;
  if (*(WORD*)(ptc) != 0x15FF)
    return -1;
  
  VirtualProtect(ptc, 6, PAGE_EXECUTE_READWRITE, &op);
  *(WORD*)ptc = 0xE890;
  *(DWORD*)(ptc+2) = (int)HookProc - ((int)ptc+6);
  VirtualProtect(ptc, 6, op, &op);
  GFT = GetProcAddress(GetModuleHandle("kernel32"),"GetFileType");
  //GFT2 = GFT+2;
  ptc = olly+0xAF054;
  VirtualProtect(ptc, 5, PAGE_EXECUTE_READWRITE, &op);
  *ptc = 0xE9;
  *(DWORD*)(ptc+1) = (int)HookGetFileType - ((int)ptc+5);
//  *(ptc+5) = 0xEB;
//  *(ptc+6) = 0xF9;
  VirtualProtect(ptc, 5, op, &op);
 
  Addtolist(0,1,"OllyFixQueryObject loaded");

  return 0;
}
//////////////////////////////////////////////////////////////////////////
int cdecl ODBG_Plugindata(char *shortname)
{
  strcpy(shortname,"OllyQueryObject");       // Name of plugin
  return PLUGIN_VERSION;
}

//////////////////////////////////////////////////////////////////////////
int cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
  switch (origin)
  {
    case PM_MAIN:                      // Plugin menu in main window
    strcpy_s(data,4096,"0 &About");
    break;
  default:
    data[0] = 0;
    return 0;
    break;
  }
  return 1;
}
//////////////////////////////////////////////////////////////////////////
void cdecl  ODBG_Pluginaction(int origin,int action,void *item)
{
  switch(origin)
  {
    case PM_MAIN:
      switch(action)
      {
        case 0:
          MessageBoxA(0,"Olly fix ZwQueryObject hang\n\n"
            "vptrlx, 2011\n",
            "About",
            MB_OK);
          break;
        default: 
          break;
      }
  }
}

int __stdcall DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved) 
{
  return 1;
}
