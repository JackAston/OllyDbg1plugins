/*
    Olly Snake
    Copyright (C) 2006 Joseph  Moti  <xbxice@yahoo.com>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include  <windows.h>
#include  <stdarg.h>
#include  <shlobj.h>
#include  <stdio.h>
#include  "plugin.h"


#pragma comment(lib,"ollydbg.lib")
#pragma comment(lib,"Ole32.lib")

struct    T_X86Function;
struct    T_X86Instruction;
HINSTANCE g_hInst;                    // DLL instance
HWND      g_hMainOllyWnd;             // handle of main OllyDbg window
char      g_cSnapshotPath[MAX_PATH]="c:";

void      ConfigureSnapshotFolder();
void      DbgBufferPrint(unsigned char*  ,	unsigned int);
VOID      DbgMsg(LPCTSTR,...);

#pragma pack(1)
struct T_X86Function
{
	
	unsigned long        Addresss;
	unsigned long        SizeOfBytes;
	unsigned long        NumOfInstruction;
	unsigned long        NumOfHitsInstruction;
	
	
};

struct  T_X86Instruction 
{
	
	unsigned short    Status;
	unsigned long     Addresss;
	unsigned long     OpCodeLength;
	unsigned char     Command[256];
	
};
#pragma pack()




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
     strcpy(shortname, "Olly Hits Snake");
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



    Addtolist(0, 0,  "  OllySnake  Plug-in compiled on " __DATE__);
    Addtolist(0, -1, "  Copyright (c) 2005 Jospeh Moti & Kobi Par ");
	
	GetPrivateProfileString("OllySnake","Snapshot_path","c:",g_cSnapshotPath,MAX_PATH,"ollysnake.ini");


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
		
		
	case PM_DISASM:
		strcpy(Data ,"Olly Hits Snake {0 Save hits ,1 Load And Diff ,2 Add all recognized "
			"functions ,3 Configure Path for Snapshot files } ");
		
		return 1;
	case PM_MAIN:
		strcpy(Data,"0 About ");
		return 1;
		
	default:
		break;
    }
	
    return 0;
}


BOOL SaveSnapShot(LPVOID pItem)
{
	
	
	T_X86Function         tX86Function;
	T_X86Instruction      tX86Instruction;
	
	t_dump                *pX86Dasm=NULL;
    ulong                  Address;
	ulong                  SOffest,EOffset;
	unsigned short         NumOfInst,NumOfHitsInst;
	ulong                  i;
	uchar                 *pHitInfo;
	unsigned char         InstStr[MAXCMDSIZE];
	ulong                 InstLength;
    t_disasm              da;
    unsigned char         *pdecode=NULL; 
	ULONG                 NumOfWrittenBytes;
	char                  TmpBuffer[0x100];
	
	WCHAR                  ObjectName[0x100];
	IStorage              *pSnapShot=NULL;
	IStream               *pX86Func=NULL;
	HRESULT                hr;
	char                   *t;
	LARGE_INTEGER          StreamFuncOffset={0};
    t_dump *pDasmWnd=(t_dump*)Plugingetvalue(VAL_CPUDASM);

	char                   DBName[0x100];
	char                   ModuleName[0x32];

	char                   SnapShotName[0x100]={0};
	pX86Dasm=(	t_dump *)pItem;
	Address=pX86Dasm->base;

	if (Gettext("Give Snapshot Name",SnapShotName,0,0,Plugingetvalue(VAL_WINDOWFONT))==-1){


		return FALSE;
	}
	GetWindowText(GetParent(pDasmWnd->table.hw),ModuleName,sizeof(ModuleName));
	t=strstr(ModuleName,"module");
	if (t) strcpy(ModuleName,strchr(t,' '));	
	
	SYSTEMTIME             sysTime;
	GetLocalTime(&sysTime);



	_snprintf(DBName,sizeof(DBName),"%s\\%s%02d%02d%02d%s.snp",g_cSnapshotPath,ModuleName,sysTime.wHour,sysTime.wMinute,sysTime.wSecond,SnapShotName);
	Flash("Taking snap shot %s",DBName);


	
    mbstowcs(ObjectName, DBName, sizeof(ObjectName));
	
	if (hr=StgCreateDocfile(ObjectName,STGM_READWRITE | STGM_SHARE_EXCLUSIVE|STGM_CREATE,0,&pSnapShot)!=S_OK){
		
		DbgMsg("SaveSnapShot:StgCreateDocfile Failed Error #0x%08X",hr);
		return FALSE;
	}
	

	
//	Suspendprocess(0);

	while(Address=Findnextproc(Address)){
		
		tX86Function.Addresss=Address;
		Getproclimits(Address,&SOffest,&EOffset);
		
		tX86Function.SizeOfBytes=(USHORT)(EOffset-SOffest);
		wsprintf(TmpBuffer,"0x%08X",Address);
		
        mbstowcs(ObjectName, TmpBuffer, sizeof(ObjectName)); 
		
		pSnapShot->CreateStream(ObjectName,STGM_READWRITE | STGM_SHARE_EXCLUSIVE|STGM_CREATE,0,0,&pX86Func);
		
		//DbgMsg("Address [0x%08X] S[0x%08X] E[0x%08X] ",tX86Function.Addresss,SOffest,EOffset);
		NumOfInst=0;
		NumOfHitsInst=0;
		pX86Func->Write(&tX86Function,sizeof(tX86Function),&NumOfWrittenBytes);
		for (i=SOffest; i<EOffset; ){
			
			if (!Readcommand(i,(char*)InstStr)) break;
			
			
			NumOfInst++;
			InstLength=Disasm(InstStr,MAXCMDSIZE,i,pdecode,&da,DISASM_CODE,0); 
			
			pHitInfo=Findhittrace(i,0,0);
			
			
			tX86Instruction.Addresss=i;
			memcpy(tX86Instruction.Command,da.result,256);
			tX86Instruction.OpCodeLength=InstLength;
			tX86Instruction.Status=(pHitInfo)?*pHitInfo:0;
			
			if (tX86Instruction.Status==0x81 )NumOfHitsInst++;
		//	DbgMsg("0x%08X %d %s [0x%02X]",tX86Instruction.Addresss,tX86Instruction.OpCodeLength,tX86Instruction.Command,
		//		tX86Instruction.Status);
			pX86Func->Write(&tX86Instruction,sizeof(tX86Instruction),&NumOfWrittenBytes);
			
			i+=InstLength;
			
		}
		
		

		tX86Function.NumOfHitsInstruction=NumOfHitsInst;
		tX86Function.NumOfInstruction=NumOfInst;
		pX86Func->Seek(StreamFuncOffset,STREAM_SEEK_SET ,NULL);
		pX86Func->Write(&tX86Function,sizeof(tX86Function),&NumOfWrittenBytes);
		
		
		
	//	DbgMsg("==================================================================");
		pX86Func->Release();
	}
	
	
	
	
	pSnapShot->Release();
//	Go(0,0,STEP_RUN,0,0);


	return TRUE;
	
	
}
BOOL DiffSnapShots(LPVOID pItem)
{
	
	
	char           szPrimaryDBPath[0x100];
	char           szSecondaryDBPath[0x100];
	WCHAR          wPrimaryObjectPath[0x100];
	WCHAR          wSecondaryObjectPath[0x100];
	IStorage       *pPrimaryStorage,*pSecondaryStorage;
	IStream        *pPrimaryStream,*pSecondaryStream;
	IEnumSTATSTG   *pEnumPrimaryStorage;
	STATSTG        PrimaryStorageStat;
	ULONG          celtFetched,cdBytesRead;
	
	T_X86Function         tX86PrimaryFunction;
	T_X86Instruction      tX86PrimaryFuncInstruction;
	
	T_X86Function         tX86SecondaryFunction;
	T_X86Instruction      tX86SecondaryFuncInstruction;
		t_dump                *pX86Dasm=NULL;
	pX86Dasm=(	t_dump *)pItem;
	
	ZeroMemory(szPrimaryDBPath,sizeof(szPrimaryDBPath));
	ZeroMemory(szSecondaryDBPath,sizeof(szSecondaryDBPath));
	if (!Browsefilename("Diff Primary",szPrimaryDBPath,".snp",0))return FALSE;
	if (!Browsefilename("Diff Secondary",szSecondaryDBPath,".snp",0))return FALSE;
	
    mbstowcs(wPrimaryObjectPath, szPrimaryDBPath, sizeof(wPrimaryObjectPath));
	
    mbstowcs(wSecondaryObjectPath, szSecondaryDBPath, sizeof(wSecondaryObjectPath));
	
	if (StgOpenStorage (wPrimaryObjectPath,NULL,
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE,NULL,0,&pPrimaryStorage)!=S_OK){
		
		DbgMsg("Failed to open %s",szPrimaryDBPath);
		return FALSE;
	}
	if (StgOpenStorage (wSecondaryObjectPath,NULL,
		STGM_READWRITE | STGM_SHARE_EXCLUSIVE,NULL,0,&pSecondaryStorage)!=S_OK){
		
		DbgMsg("Failed to open %s",szSecondaryDBPath);
		return FALSE;
	}

	Suspendprocess(0);
	Modifyhittrace(pX86Dasm->base,pX86Dasm->base+pX86Dasm->size,ATR_REMOVE);

	
	pPrimaryStorage->EnumElements(0,0,0,&pEnumPrimaryStorage);
	pEnumPrimaryStorage->Reset();
	
	while(pEnumPrimaryStorage->Next(1,&PrimaryStorageStat,&celtFetched)==S_OK){
		
		//DbgMsg("======================================================================");
		if (pPrimaryStorage->OpenStream(PrimaryStorageStat.pwcsName,0,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,0,&pPrimaryStream)==S_OK){
			
			
			if (pSecondaryStorage->OpenStream(PrimaryStorageStat.pwcsName,0,
				            STGM_READWRITE | STGM_SHARE_EXCLUSIVE,0,&pSecondaryStream)==S_OK){
				
				
				if (pPrimaryStream->Read(&tX86PrimaryFunction,sizeof(tX86PrimaryFunction),&cdBytesRead)==S_OK) {
					
					if (pSecondaryStream->Read(&tX86SecondaryFunction,sizeof(tX86SecondaryFunction),&cdBytesRead)==S_OK){
						

						if (tX86PrimaryFunction.NumOfHitsInstruction==tX86SecondaryFunction.NumOfHitsInstruction){


							//DbgMsg("Primary [0x%08X] Secondary[0x%08X] [NO CHANGED]",tX86PrimaryFunction.Addresss,
							//	tX86SecondaryFunction.Addresss);


						}else{


							DbgMsg("Primary [0x%08X] Secondary[0x%08X] [YES CHANGED] HITS BEFORE [%d]  HITS AFTER[%d] TOTAL COMMMANDS[%d]",
								tX86PrimaryFunction.Addresss,
								tX86SecondaryFunction.Addresss,
								tX86PrimaryFunction.NumOfHitsInstruction,
								tX86SecondaryFunction.NumOfHitsInstruction,
								tX86SecondaryFunction.NumOfInstruction
								);

							for (unsigned long  i=0; i<tX86SecondaryFunction.NumOfInstruction; i++){

								pSecondaryStream->Read(&tX86SecondaryFuncInstruction,sizeof(tX86SecondaryFuncInstruction),&cdBytesRead);
								pPrimaryStream->Read(&tX86PrimaryFuncInstruction,sizeof(tX86PrimaryFuncInstruction),&cdBytesRead);
							
								if ((tX86SecondaryFuncInstruction.Status==0x81) && (tX86PrimaryFuncInstruction.Status==0x82)){
									DbgMsg("Address[0x%08X] %s [%s]",tX86SecondaryFuncInstruction.Addresss,
									tX86SecondaryFuncInstruction.Command,
									(tX86SecondaryFuncInstruction.Status==0x82)?"NO HIT":"HIT");
                                     Setbreakpoint(tX86SecondaryFuncInstruction.Addresss,TY_ACTIVE,0);
								

								}


							}



						}
						
						
					}
					
					
				}

			
				pSecondaryStream->Release();
				
			}
			
			
			pPrimaryStream->Release();
			
			
		}
		CoTaskMemFree(PrimaryStorageStat.pwcsName);
		
		
	}
	pEnumPrimaryStorage->Release();
	pPrimaryStorage->Release();
	pSecondaryStorage->Release();
	
	::SendMessage(g_hMainOllyWnd,WM_COMMAND,0x840,0);
	return TRUE;
	
}

void ConfigureSnapshotFolder()
{
	BROWSEINFO bi;
	LPITEMIDLIST pidl=0; 
	ZeroMemory(&bi,sizeof(bi));
	ZeroMemory(g_cSnapshotPath,MAX_PATH);
	bi.hwndOwner=GetDesktopWindow();
	bi.pidlRoot=NULL;
	bi.pszDisplayName=g_cSnapshotPath;
	bi.lpszTitle="Configure snapshot Folder.";
	bi.ulFlags=0;
	
	if (!(pidl=SHBrowseForFolder(&bi))) return;
	
	if (!SHGetPathFromIDList(pidl,g_cSnapshotPath)) return;
	WritePrivateProfileString("OllySnake","Snapshot_path",g_cSnapshotPath,"ollysnake.ini");

	 

}
void __declspec(dllexport) __cdecl ODBG_Pluginaction (int Orig, int Action, void *pItem)
{
	
	
	t_dump                *pX86Dasm=NULL;
	t_module              *pModule=NULL;
	
    pX86Dasm=(	t_dump *)pItem;
	
    switch (Orig)
    {
	case PM_MAIN:
		
		
		switch (Action)
		{
			
			
			
		case 0:
			
			MessageBox(GetTopWindow(GetDesktopWindow()),
				"OllySnake  Plug-in\n\n"
				"Copyright (C) 2006 Jospeh Moti  xbxice@yahoo.com\nThanks to Kobi par & scatterp",
				"About OllySnake Plug-in",
				MB_OK | MB_ICONINFORMATION);
			
		default:
			break;
		}
		
		break;
		
		case PM_DISASM:
			
			
			switch (Action)
			{
				
				
			case 0:
				
				
				SaveSnapShot(pItem);
				
				break;
				
			case 1:
				DiffSnapShots(pItem);
				
				break;
			case 2:
				pModule=Findmodule(pX86Dasm->addr);
				if(Analysecode(pModule)==-1){
					
					MessageBox(0,"Olly Unable to Analysecode brrr","?",0);
					
				}else{
					Modifyhittrace(pX86Dasm->base,pX86Dasm->base+pX86Dasm->size,ATR_ADDPROC);
					Flash("Adding All recogined functions...");
				}
				
				break;
			case 3:
				
				ConfigureSnapshotFolder();
				break;
				
				
			}
			break;
			
			
			
			default:
				break;
    }
}
