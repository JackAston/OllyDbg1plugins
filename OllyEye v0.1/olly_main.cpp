/*
Copyright (C) 2008 Stephan Chenette, Moti Joseph [Websense Inc. Websense Security Labs]
,Kobi Pariente
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include  <windows.h>
#include  <stdarg.h>

#include  <shlobj.h>
#include  <stdio.h>
#include  "plugin.h"


#pragma comment(lib,"ollydbg.lib")
#pragma comment(lib,"Ole32.lib")

HINSTANCE g_hInst;                    // DLL instance
HWND      g_hMainOllyWnd;             // handle of main OllyDbg window

void      DbgBufferPrint(unsigned char*  ,	unsigned int);
VOID      DbgMsg(LPCTSTR,...);




VOID
DbgMsg(LPCTSTR szError,...)
{
    char                        szBuff[0x100];
    va_list                     vl;

    va_start(vl, szError);
    vsprintf(szBuff, szError, vl);
    OutputDebugString(szBuff);
    va_end(vl);
}
void DbgBufferPrint(unsigned char* pBuffer ,	unsigned int uLength)
{


	unsigned int  i;
	char _t[1024],*pTmp;
	pTmp=_t;
	_snprintf(_t,1000,"------[Packet Size %d]------",uLength);
	OutputDebugString(_t);
	for ( i=0; i<uLength; i++){


	
		_snprintf(pTmp,1000,"%02X-",(unsigned char)pBuffer[i]);
		while(*pTmp!='-') pTmp++;
		pTmp++;
		if ((i%8)==0 && i ) {
			OutputDebugString(_t);
            pTmp=_t;

		}

	}
	OutputDebugString(_t);



}
BOOL WINAPI DllEntryPoint (HINSTANCE hi, DWORD reason, LPVOID reserved)
{

    if (reason == DLL_PROCESS_ATTACH) {

        g_hInst = hi;


	}

    return TRUE;
}


int __declspec(dllexport) __cdecl ODBG_Plugindata (char shortname[32])

{
 
	strcpy(shortname,"Olly Eye");
    return PLUGIN_VERSION;
}

 



int __declspec(dllexport) __cdecl ODBG_Plugininit (int OllyDbgVer, HWND hWnd, ulong *pFeatures)
{
    if (OllyDbgVer < PLUGIN_VERSION) {
		Addtolist(0, 0,  "OllySnake not compitable with the current olly version ");
		return -1;
		
	}

	CoInitialize(NULL);
   
    g_hMainOllyWnd = hWnd;



    Addtolist(0, 0,  "  OllyEye  Plug-in compiled on " __DATE__);
    Addtolist(0, -1, "  Copyright (c) 2008 Jospeh Moti (www.websense.com) & Kobi Pariente ");
	


    return 0;
}


void __declspec(dllexport) __cdecl ODBG_Plugindestroy (void)
{


	return;

}


int __declspec(dllexport) __cdecl ODBG_Pluginmenu (int Orig, char Data[4096], void *pItem)
{
    switch (Orig)
    {
		
		
	case   PM_DISASM:
		strcpy(Data ,"Olly Eye Snake {0 Clean ,1 +'*' ,2 + '-' ,3 + ' WORD PTR',4 +'BYTE PTR',"
			"5 +'DWORD PTR',6 About} ");
		
		return 1;
		
	default:
		break;
    }
	
    return 0;
}

struct  T_X86Instruction 
{
	
	unsigned short    Status;
	unsigned long     Addresss;
	unsigned long     OpCodeLength;
	unsigned char     Command[256];
	
};
BOOL XXX(LPVOID pItem,char *pSubString)
{
	
	
	T_X86Instruction      tX86Instruction;
	
	t_dump                *pX86Dasm=NULL;
    ulong                  Address;
	ulong                  SOffest,EOffset;
	ulong                  i;
	unsigned char         InstStr[MAXCMDSIZE];
	ulong                 InstLength;
    t_disasm              da;
    unsigned char         *pdecode=NULL; 

	t_dump *pDasmWnd=(t_dump*)Plugingetvalue(VAL_CPUDASM);
	
	
	pX86Dasm=(	t_dump *)pItem;
	Address=pX86Dasm->base;
	

	char                 cPattern[0x100]={0};
	if (Gettext("Search for pattern ...",cPattern,0,0,Plugingetvalue(VAL_WINDOWFONT))==-1){


		return FALSE;
	}
	
	
	
	
	while(Address=Findnextproc(Address)){
		
	  Getproclimits(Address,&SOffest,&EOffset);
		
		
		
		for (i=SOffest; i<EOffset; ){
			
			if (!Readcommand(i,(char*)InstStr)) break;
			
			
		
			InstLength=Disasm(InstStr,MAXCMDSIZE,i,pdecode,&da,DISASM_CODE,0); 
			
			
			
			tX86Instruction.Addresss=i;
			memcpy(tX86Instruction.Command,da.result,256);
			tX86Instruction.OpCodeLength=InstLength;
			
			if (strstr((char*)tX86Instruction.Command,cPattern) ) {
				
				if (pSubString){ 
					if (strstr((char*)tX86Instruction.Command,pSubString)){
						
						
						DbgMsg("0x%08X %d %s ",
							tX86Instruction.Addresss,
							tX86Instruction.OpCodeLength,
							tX86Instruction.Command);
						
							
						
						Setbreakpoint(tX86Instruction.Addresss,TY_ACTIVE|TY_KEEPCODE,0);
					}
						i+=InstLength;
						continue;
				}
					DbgMsg("0x%08X %d %s ",
							tX86Instruction.Addresss,
							tX86Instruction.OpCodeLength,
							tX86Instruction.Command);
							Setbreakpoint(tX86Instruction.Addresss,TY_ACTIVE|TY_KEEPCODE,0);
				

			}
				
				
			i+=InstLength;
			
		}
		
		
		
		
		
		
	}
	
	
	
	
	return TRUE;
	



}

void __declspec(dllexport) __cdecl ODBG_Pluginaction (int Orig, int Action, void *pItem)
{
	
	
	t_dump                *pX86Dasm=NULL;
	t_module              *pModule=NULL;
	
    pX86Dasm=(	t_dump *)pItem;
	pModule=Findmodule(pX86Dasm->addr);
    switch (Orig)
    {
		
	case PM_DISASM:
		
		
		switch (Action)
		{
			
			
		case 0:
			
		XXX(pItem,0);
			break;
			
		case 1:
		XXX(pItem,"*");
			
			break;
		case 2:
		XXX(pItem,"-");
			
			break;
		case 3:
		XXX(pItem,",WORD PTR");
		
			break;
		case 4:
			XXX(pItem,"BYTE PTR");
			break;
		case 5:
			XXX(pItem,"DWORD PTR");
			break;
		case 6:
			
			
			MessageBox(g_hMainOllyWnd,
				"OllyEye  Plug-in\n\n"
				"Copyright (C) 2008 Jospeh Moti  www.websense.com \nThanks to Kobi Pariente",
				"About OllyEye Plug-in",
				MB_OK | MB_ICONINFORMATION);
			break;
			
		}
		break;
		
		
		
		default:
			break;
    }
}
