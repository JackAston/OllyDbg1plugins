// findcrypt.cpp : Defines the entry point for the DLL application.

#include <windows.h>
#include "stdafx.h"

HANDLE    g_hInst;                    // DLL instance
HWND      g_hMainOllyWnd;             // handle of main OllyDbg window

typedef unsigned char* ea_t;

ea_t ea1 = 0;
ea_t ea2 = 0;

unsigned char *memory = NULL;

//--------------------------------------------------------------------------
inline ea_t nextaddr(ea_t ea) {
	return ea+1;
}

//--------------------------------------------------------------------------
// this function is replacement for IDA API get_byte()
inline unsigned char get_byte(ea_t ea)
{
	unsigned char* uc_memory = (unsigned char*)memory;
	int index;

	index = (int)(ea-ea1);
	return uc_memory[index];
}

//--------------------------------------------------------------------------
// replacement of IDA API get_long()
inline unsigned long get_long(ea_t ea)
{
	unsigned long* ul_memory;
	int offset = (int)(ea-ea1);

	ul_memory = (unsigned long*)(memory+offset);
	return *ul_memory;
}

//--------------------------------------------------------------------------
// replacement of IDA API get_many_bytes()
inline bool get_many_bytes(ea_t ea, void *buf, size_t size)
{
	int off = (int)(ea-ea1);

	if(buf == NULL || size <= 0)
		return false;

	memcpy(buf, memory+off, size);
	return true;
}

//--------------------------------------------------------------------------
// match a sparse array against the database at the specified address
static bool match_sparse_pattern(ea_t ea, const array_info_t *ai)
{
	const word32 *ptr = (const word32*)ai->array;
	
	if ( get_long(ea) != *ptr++ )
		return false;
	
	ea += 4;
	
	unsigned int n = ai->size / 4;
	
	for ( size_t i=1; i < n; i++ ) {
		word32 c = *ptr++;
		
		// look for the constant in the next N bytes
		const size_t N = 64;
		uchar mem[N+4];
		
		get_many_bytes(ea, mem, sizeof(mem));

		int j;
		for ( j=0; j < N; j++ )
			if ( *(word32*)(mem+j) == c )
				break;
		
		if ( j == N )
			return false;
	
		ea += j + 4;
	}
	return true;
}

//--------------------------------------------------------------------------
// match a constant array against the database at the specified address
static bool match_array_pattern(ea_t ea, const array_info_t *ai)
{
	const uchar *ptr = (const uchar*)ai->array;
	
	for ( size_t i=0; i < ai->size; i++ )
		if ( get_byte(ea+i) != ptr[i] )
			return false;
	
	return true;
}

//--------------------------------------------------------------------------
// try to find constants at the given address range
static void recognize_constants(ea_t ea1, ea_t ea2)
{
	int count = 0;
	const array_info_t *ptr;
  
	Addtolist(0, 1, "[FindCrypt] Searching for crypto constants...");
  
	for ( ea_t ea = ea1; ea < ea2; ea = nextaddr(ea) ) {
		uchar b = get_byte(ea);

		// check against normal constants
		for ( ptr = consts; ptr->size != 0; ptr++ ) {
			if ( b != *(uchar*)ptr->array )
				continue;

			if ( match_array_pattern(ea, ptr) ) {
				Addtolist(0, 0, "[FindCrypt] 0x%08x: found const array %s (used in %s)", (unsigned long)ea, ptr->name, ptr->algorithm);
				count++;
				break;
			}
		}
	
		// check against sparse constants
		for ( ptr = consts; ptr->size != 0; ptr++ ) {
			if ( b != *(uchar*)ptr->array )
				continue;
			
			if ( match_sparse_pattern(ea, ptr) ) {
				Addtolist(0, 0, "[FindCrypt] 0x%08x: found sparse constants for %s", (unsigned long)ea, ptr->algorithm);
				count++;
				break;
			}
		}
	}
}

DWORD WINAPI FindCrypt_Start(LPVOID lpParameter)
{
	unsigned long mem_len = ea2-ea1;
	Addtolist(0, 0, "[FindCrypt] starting search in range 0x%08x - 0x%08x", ea1, ea2);

	memory = new unsigned char[mem_len];

	memset(memory, 0, mem_len);

	int ret = Readmemory(memory, (unsigned long)ea1, mem_len, MM_RESTORE);

	if(ret == 0) {
		Addtolist(0, 1, "[FindCrypt] Failed to read process memory");
		return 1;
	}

	Addtolist(0, 0, "[FindCrypt] Readmemory read %d bytes", ret);
	
	recognize_constants(ea1, ea2);

	Addtolist(0, 0, "[FindCrypt] finished");
	delete[] memory;

	return 0;
}

void __declspec(dllexport) __cdecl ODBG_Pluginaction (int Orig, int Action, void *pItem)
{	
	if(Orig == PM_MAIN) {
		if(Action == 1 && Getstatus() != STAT_NONE) {
			Getlong("Start address", (unsigned long*)&ea1, sizeof(ea1), 0, DIA_ASKGLOBAL|DIA_ALIGNED);
			Getlong("Stop address", (unsigned long*)&ea2, sizeof(ea2), 0, DIA_ASKGLOBAL|DIA_ALIGNED);
			
			if(ea2 <= ea1) {
				Addtolist(0, 1, "[FindCrypt] stop address have to be higher than start address");
				return;
			}

			DWORD ThreadID;
			::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE )FindCrypt_Start,(void*)NULL,0,&ThreadID);
			
			return;
		}
		if(Action == 0) {
			MessageBox(g_hMainOllyWnd, 
				"FindCrypt Plug-In\n"
				"ported to OllyDbg from Iflak Guilfanov IDA plugin\n\n"
				"02/2006 mcb <mc@hack.pl>", "About FindCrypt", MB_OK | MB_ICONINFORMATION);
			return;
		}
	}
}

int __declspec(dllexport) __cdecl ODBG_Plugindata (char shortname[32])
{
	strcpy(shortname, "FindCrypt");
	return PLUGIN_VERSION;
}


int __declspec(dllexport) __cdecl ODBG_Plugininit (int OllyDbgVer, HWND hWnd, ulong *pFeatures)
{
	if(OllyDbgVer < PLUGIN_VERSION) {
		Addtolist(0, 0, "FindCrypt not compatible with the current olly version");
		return -1;
	}
	

	Addtolist(0, 0, "FindCrypt Plug-in, 2006 by mcb (mc@hack.pl)");
	Addtolist(0, -1, "  this is ported from Iflak Guilfanov IDA plugin with the same name. Thanks Iflak");

	g_hMainOllyWnd = hWnd;

	return 0;
}

int __declspec(dllexport) __cdecl ODBG_Pluginmenu(int Orig, char Data[4096], void *pItem)
{
    switch (Orig)
    {
		case PM_MAIN:
			strcpy(Data, "1 Start search!|0 About");	
			return 1;
		default:
			break;
    }
	
    return 0;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	g_hInst = hModule;
    return TRUE;
}

