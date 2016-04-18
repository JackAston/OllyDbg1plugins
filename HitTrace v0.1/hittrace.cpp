/*

  Author: David Zimmer <david@idefense, dzzie@yahoo.com>

  Notes: this is ugly and was done as quick as possible but it works
         if you need more than 50 bpx recompile;

		 THIS APP USES DLL NAME LISTED IN MODULES EXPORT TABLE FOR
		 STRING MATCHING. IF DLL NAME IS DIFFERENT IT WILL LOOK LIKE
		 THIS APP ISNT WORKING.

 
  VERY IMPORTANT NOTICE: COMPILE THIS DLL WITH BYTE ALIGNMENT OF STRUCTURES
  AND UNSIGNED CHAR!

  Bugfixes:
	9/15/06 -
			fixed potential bof
			fixed errs in loading saved lists & added msgs if save err

*/

#include <windows.h>
#include <stdio.h>
#include <StdDef.H> // offsetof()
#include "resource.h"
#include "plugin.h"

struct BpxEntry{
	char			  dllName[50];
	char			  LogExp[50];
	unsigned int      rvaBpx;
	unsigned int      hitcnt;
	unsigned int      va;
	bool              enabled; //should check olly to make sure not deleted?
};

BpxEntry Entries[50];  

int  numEntries           =  -1;
int  m_hProcess		      =  0;
bool debug			      =  false;
int  my_timer		      =  0;
int  abort_trace_index    =  -1;
char szFileName[MAX_PATH] = {0};

HINSTANCE        hinst;
HWND             hwmain;
HINSTANCE        myHnd;


void strlower(char* x){
	if( (int)x ){
		int y = strlen(x);
		for(int i=0;i<y;i++){
			x[i]=tolower(x[i]);
		}
	}
}

void FillList(HWND hList){
	char buf[200];
	SendMessage(hList,LB_RESETCONTENT,0,0);
	for(int i=0;i<=numEntries;i++){
		sprintf(buf, "%d:          %s          %x          %s", i, Entries[i].dllName, Entries[i].rvaBpx , Entries[i].LogExp );
		SendMessage(hList,LB_ADDSTRING,0,(int)&buf);
	}
}

bool AddEntry(char* mod, int rva, char* logexp){

	if(numEntries > 49) return false;
	if(strlen(mod) > 49) mod[49]=0;

	numEntries++;
	strncpy(Entries[numEntries].dllName ,mod,strlen(mod));
	strncpy(Entries[numEntries].LogExp  ,logexp,49);

	Entries[numEntries].rvaBpx = rva;
	Entries[numEntries].va =0;
	Entries[numEntries].enabled =false;
	Entries[numEntries].hitcnt =0;
	
	return true;

}


void RemoveEntry(int index){
	
	bool compact = false;

	for(int i=0;i<=numEntries;i++){
top:
		if(compact){
			if(i==numEntries) memset((void*)&Entries[i],0,sizeof(BpxEntry));
			else Entries[i] = Entries[i+1];
		}
		else if(i==index){
				compact = true;
				goto top;
		}
	}

	if(compact) numEntries--;

}

void SetTop(HWND h, bool top=true){
	HWND flag = top ? HWND_TOPMOST : HWND_NOTOPMOST ;	
	SetWindowPos(h,flag,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}
		
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
	
	//DLL instance MUST be used to load embedded resources.
	myHnd = (HINSTANCE)hModule;

    return TRUE;
}

//basically ripped from pedrams olly bp man ;)
int FindModuleFromName(char* modName){

	t_table *mod_table;
	t_module *module;
	char buf[250];

	mod_table = (t_table *)Plugingetvalue(VAL_MODULES);

	for (int i=0; i<mod_table->data.n; i++){

        if ((module = (t_module *) Getsortedbyselection(&(mod_table->data), i)) == NULL){
            if(debug) Addtolist(0, 1, "HitTrace> Unable to resolve module at index %d.", i);
            continue;
        }
		
		if(module->name == NULL) break;
		strncpy(buf, module->name, 249);
		strlower(buf);

        if (strstr(module->name, modName) != 0){
		     return module->base;
        }
    }

	return 0;

}

// This is the message loop
BOOL CALLBACK MyDlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	char buf [200]={0},modName[50]={0},sRva[50] = {0},sLog[50] = {0};
    unsigned int i=0, index=0, rva=0, hexValue=0,base=0;
	int ret=0;
	FILE* dat;

	HWND hName = GetDlgItem(hWnd, IDC_EDIT1);
	HWND hRva  = GetDlgItem(hWnd, IDC_EDIT2);
	HWND hAbort= GetDlgItem(hWnd, IDC_EDIT3);
	HWND hLog  = GetDlgItem(hWnd, IDC_EDIT4);
	HWND hList = GetDlgItem(hWnd, IDC_LIST2);
	HWND hCheck= GetDlgItem(hWnd, IDC_CHECK1);


	switch(uMsg)
	{
		case WM_INITDIALOG :
			
			FillList(hList);
			if(debug) SendMessage(hCheck, BM_SETCHECK, BST_CHECKED ,0);
			if(abort_trace_index >=0){
				sprintf(sLog,"%d",abort_trace_index);
				SetWindowText(hAbort,sLog);
			}
			SetTop(hWnd);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_BUTTON1: //delete item from listbox

					index = SendMessage(hList, LB_GETCURSEL, 0,0 );
					if(index== LB_ERR) break;

					SendMessage(hList,LB_GETTEXT,index, (int)&buf);
					index = atoi(buf); //now EntryIndex
					RemoveEntry(index);
					FillList(hList);
					
					break;
			
				case IDC_BUTTON2: //OK button ID
					
	
					GetWindowText(hName, modName,49);
					GetWindowText(hRva, sRva,49);
					GetWindowText(hLog, sLog,49);

					strlower(modName);
					rva = strtol(sRva, NULL, 16);
					
					if(rva==0 || strlen(modName) == 0){
						MessageBox(0,"Invalid Input","",0);
						break;
					}

					if( AddEntry(modName, rva,sLog) ){
						sprintf(buf, "%d:          %s          %x          %s", numEntries, modName, rva, sLog);
						SendMessage(hList,LB_ADDSTRING, 0, (int)&buf);
					}
					else{
						MessageBox(0,"Failed to add (maxed out)","",0);
					}
						

					break;

				case IDC_BUTTON3: //save list

					try{
						SetTop(hWnd,false);
						ret = Browsefilename("Save File As",(char*)&szFileName,".htl",0x80);
						SetTop(hWnd);

						if(debug) Addtolist(0,0,"Saving to file: %s", szFileName);
						if(ret != TRUE) break;
						dat = fopen(szFileName, "wb");
						fwrite((void*)Entries, sizeof(Entries),1,dat);
						fwrite((void*)&numEntries, 4,1,dat);
						fclose(dat);
					}
					catch(...){
						MessageBox(0,"Error Saving List","Error",0);
					}

					break;

				case IDC_BUTTON4: //load saved

					try{
						
						SetTop(hWnd,false);
						ret = Browsefilename("Open File",(char*)&szFileName,".htl;",0);
						SetTop(hWnd);

						if(debug) Addtolist(0,0,"Saving to file: %s", szFileName);
						if(ret != TRUE) break;
						if(debug) Addtolist(0,0,"Loading file: %s", szFileName);
						dat = fopen(szFileName, "rb");
						fread((void*)Entries, sizeof(Entries),1,dat);
						fread((void*)&numEntries, 4,1,dat);
						if(numEntries > 49) numEntries=49;

						for(i=0;i<numEntries;i++){
							Entries[i].enabled = false ;
						}

						fclose(dat);
						FillList(hList);
					}
					catch(...){
						MessageBox(0,"Error Loading List","Error",0);
					};

					break;

				case IDC_BUTTON5: //clear list

					memset( (void*)Entries, 0, sizeof(Entries));
					numEntries = -1;
					FillList(hList);
					break;


				case IDC_BUTTON6: //begin hit trace

					if(numEntries == -1){
						MessageBox(0,"You have not set any bpx","",0);
						break;
					}

					GetWindowText(hAbort, sRva,49);
					if(strlen(sRva)==0){
						abort_trace_index = -1;
					}
					else{
						abort_trace_index = atoi(sRva);
					}

					for(ret=0;ret <=numEntries;ret++){
						//we have already calculated the va so set bpx
						if(!Entries[ret].enabled && Entries[ret].va > 0){
							if(debug) Addtolist(0,0,"Not enabled va set");
							Setbreakpoint(Entries[ret].va , TY_ACTIVE,0);
							Entries[ret].enabled =false;
						}
						//breakpoint is in main module 
						else if(!Entries[ret].enabled && strcmp(Entries[ret].dllName, "main")==0){
							if(debug) Addtolist(0,0,"Bp in Main");
							base=Plugingetvalue(VAL_MAINBASE);
							Entries[ret].va = base + Entries[ret].rvaBpx;
							Entries[ret].enabled = true;
							Setbreakpoint( Entries[ret].va ,  TY_ACTIVE, 0);
						}
						//we have not calculated the va
						else if(!Entries[ret].enabled && Entries[ret].va == 0){
							if(debug) Addtolist(0,0,"Not enabled va 0");
							i = FindModuleFromName(Entries[ret].dllName);
							if(i>0){
								Entries[ret].va = i+Entries[ret].rvaBpx ;
								Entries[ret].enabled = true;
								Setbreakpoint( Entries[ret].va ,  TY_ACTIVE, 0);
							}
						}
						
					}

					Go(0,0,STEP_RUN,0,0);
					EndDialog(hWnd,0);
					break;

				case IDC_BUTTON7: //show results

					Suspendprocess(1);

					Addtolist(0,1,"Dumping hit trace results");
					for(ret=0;ret <=numEntries;ret++){
						if(Entries[ret].hitcnt > 0 ){
							Addtolist(0,0,"HitTrace %s.0x%x  = %d", Entries[ret].dllName, Entries[ret].rvaBpx , Entries[ret].hitcnt  );
						}
					}
					Addtolist(0,1,"Trace results done");
					Createlistwindow();
					

					break;


				case IDC_BUTTON8: //disable hit trace

					Suspendprocess(1);

					for(ret=0;ret <= numEntries;ret++){
						Setbreakpoint(Entries[ret].va , TY_DISABLED,0);
						Entries[ret].enabled =false;
					}


					break;

				case IDC_BUTTON9: //reset counts
					
					Suspendprocess(1);
					for(ret=0;ret < numEntries;ret++){
						Entries[ret].hitcnt = 0 ;
					}
					break;


			}

			debug=false;
			if(SendMessage(hCheck,BM_GETCHECK,0,0) == BST_CHECKED) debug = true;

			break;

		case WM_CLOSE:

			debug=false;
			if(SendMessage(hCheck,BM_GETCHECK,0,0) == BST_CHECKED) debug = true;
			EndDialog(hWnd,0);
			break;

	}

	return FALSE;

}



BOOL WINAPI DllEntryPoint(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH) hinst=hi;
  return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"HitTrace");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION) return -1;
  Addtolist(0,0,"Initilizing HitTrace Plugin - David Zimmer (dzzie@yahoo.com)");
  hwmain=hw;
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {

  switch (origin) {

		case PM_MAIN:
			  strcpy(data,"0 HitTrace Plugin, 1 About");
			  return 1;

		default: break;
  };

  return 0;

};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin==PM_MAIN) {
    switch (action) {
      case 0:
			 DialogBox( myHnd,MAKEINTRESOURCE(IDD_FORMVIEW),0,(DLGPROC)MyDlgProc); //modal
			 break;
	  case 1:
			 MessageBox(0,"HitTrace- David Zimmer <dzzie@yahoo.com>\n\n"
						   "Usage: Enter the dllname and the rva of the\n"
						   "code you want to hit trace\n\n"
						   "Optionally you can enter an expression to log\n"
						   "when the bpx is hit, click Begin to start\n\n"
						   "(You can also enter a index of bpx to stop at)","",0);
			 break;
      default: break;
    };
  };
};

extc void _export cdecl ODBG_Pluginreset(void) {
};

extc int _export cdecl ODBG_Pluginclose(void) {
  return 0;
};

extc void _export cdecl ODBG_Plugindestroy(void) {
};



// PURPOSE  : Retrieves the DLL module name for a given file handle of a
//            the module.  Reads the module name from the EXE header.
// from MS DEB Example in SDK I think
int GetModuleFileNameFromHeader(int hProcess, HANDLE hFile, 
								  int BaseOfDll, char* lpszPath,
								  int bufSize)
{
  #define IMAGE_SECOND_HEADER_OFFSET    (15 * sizeof(ULONG)) // relative to file beginning
  #define IMAGE_BASE_OFFSET             (13 * sizeof(DWORD)) // relative to PE header base
  #define IMAGE_EXPORT_TABLE_RVA_OFFSET (30 * sizeof(DWORD)) // relative to PE header base
  #define IMAGE_NAME_RVA_OFFSET         offsetof(IMAGE_EXPORT_DIRECTORY, Name)
	

  WORD   DosSignature;
  DWORD  NtSignature;
  DWORD  dwNumberOfBytesRead = 0;
  DWORD  PeHeader;
  int* ImageBase, ExportTableRVA, NameRVA;

  //-- verify that the handle is not 0L
  if( !hFile ) {
    strcpy(lpszPath,"Invalid File Handle");
    return( 0 );
  }

  //-- verify that the handle is for a disk file
  if( GetFileType(hFile) != FILE_TYPE_DISK ) {
    strcpy(lpszPath,"Invalid File Type");
    return( 0 );
  }

  //-- Extract the filename from the EXE header
  SetFilePointer(hFile, 0L, 0L, FILE_BEGIN );
  ReadFile( hFile, &DosSignature, sizeof(DosSignature), &dwNumberOfBytesRead,(LPOVERLAPPED) 0L);

  //-- verify DOS signature found
  if( DosSignature != IMAGE_DOS_SIGNATURE ) {
    sprintf(lpszPath,"Bad MZ Signature: 0x%x", DosSignature );
    return( 0 );
  }

  SetFilePointer( hFile, IMAGE_SECOND_HEADER_OFFSET, (LPLONG) 0L,FILE_BEGIN );
  ReadFile( hFile, &PeHeader, sizeof(PeHeader), &dwNumberOfBytesRead,(LPOVERLAPPED) 0L );
  SetFilePointer( hFile, PeHeader, (LPLONG) 0L, FILE_BEGIN );
  ReadFile( hFile, &NtSignature, sizeof(NtSignature), &dwNumberOfBytesRead,(LPOVERLAPPED) 0L);

  //-- verify Windows NT (PE) signature found
  if( NtSignature != IMAGE_NT_SIGNATURE ) {
    sprintf(lpszPath,"Bad PE Signature: 0x%x",DosSignature);
    return( 0 );
  }

  SetFilePointer( hFile, PeHeader + IMAGE_BASE_OFFSET, (LPLONG) 0L,FILE_BEGIN );
  ReadFile( hFile, &ImageBase, sizeof(ImageBase), &dwNumberOfBytesRead, (LPOVERLAPPED) 0L);
  SetFilePointer( hFile, PeHeader + IMAGE_EXPORT_TABLE_RVA_OFFSET,(LPLONG) 0L, FILE_BEGIN );
  ReadFile( hFile, &ExportTableRVA, sizeof(ExportTableRVA), &dwNumberOfBytesRead, (LPOVERLAPPED) 0L);

  //-- now read from the virtual address space in the process
  if (!ReadProcessMemory( (HANDLE)hProcess,
         (LPVOID) (BaseOfDll + ExportTableRVA + IMAGE_NAME_RVA_OFFSET),
         &NameRVA, sizeof(NameRVA), &dwNumberOfBytesRead ) ||
      dwNumberOfBytesRead != sizeof(NameRVA))
  {
      strcpy(lpszPath,"Access Denied!");
      dwNumberOfBytesRead = 0;
  }
  else if( !ReadProcessMemory( (HANDLE) hProcess,
              (LPVOID) (BaseOfDll + NameRVA), lpszPath, bufSize, &dwNumberOfBytesRead ))
  {
     strcpy(lpszPath,"Access Denied!");
  }

  return( dwNumberOfBytesRead );
}



VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime){

	//Addtolist(0,0,"Timerproc");
	Sendshortcut(PM_MAIN, 0, WM_KEYDOWN, 0, 0, VK_F9);
	KillTimer((HWND)0, my_timer);


}


extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent){

	
	if( (int)debugevent == 0) return;
	
	if(debugevent->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT){
		m_hProcess = (int)debugevent->u.CreateProcessInfo.hProcess;
	}
	
	if(numEntries == -1) return;

	if(debugevent->dwDebugEventCode == EXCEPTION_DEBUG_EVENT){
		if(debugevent->u.Exception.ExceptionRecord.ExceptionCode == EXCEPTION_BREAKPOINT){
			unsigned int adr = (int)debugevent->u.Exception.ExceptionRecord.ExceptionAddress;
			//if(debug) Addtolist(0,0,"Breakpoint at %x", adr);
			for(int i=0;i<=numEntries;i++){
				if(adr == Entries[i].va){
					if(abort_trace_index == i){
						//if(debug) Addtolist(0,1,"Time to abort");
						MessageBox(0,"Hit abort index breakpoint","",0);
						DialogBox( myHnd,MAKEINTRESOURCE(IDD_FORMVIEW),0,(DLGPROC)MyDlgProc); 
					}
					else{
						//if(debug) Addtolist(0,1,"Its ours!");
						//we should strtok for multiple expressions
						if(Entries[i].LogExp[0]!=0){
							t_result result;
							int n = Expression(&result,Entries[i].LogExp,0,0,NULL,0,0,Getcputhreadid());
							if (n>0 || result.type!=DEC_UNKNOWN) {
								Addtolist(0,1,"%s.0x%x  %s = %x",Entries[i].dllName , Entries[i].rvaBpx , Entries[i].LogExp, result.u );
							}
						}
						
						Entries[i].hitcnt++;
						my_timer = SetTimer(0,0,600,(TIMERPROC)TimerProc);
					}
					return;
				}
			}

		}
	}


	if(debugevent->dwDebugEventCode != LOAD_DLL_DEBUG_EVENT) return;

	try{ 

		char dllName[200] = {0};
		char buf[500] = {0};

		GetModuleFileNameFromHeader( m_hProcess, 
									 debugevent->u.LoadDll.hFile, 
									 (int)debugevent->u.LoadDll.lpBaseOfDll , 
									 (char*)&dllName, 
									 199
									);
		
		if( (int)dllName==0 ){
			Addtolist(0,1,"Error getting dllname @ base %x", debugevent->u.LoadDll.lpBaseOfDll);
			return;
		}

		strlower(dllName);
			
		if(debug){
			sprintf(buf, "ModBpx: Dll %s loaded at base %x", dllName, debugevent->u.LoadDll.lpBaseOfDll);
			Addtolist(0,0,buf);
		}
			
		for(int i=0;i<=numEntries;i++){
			//if(stricmp(Entries[i].dllName , dllName) == 0 ) {
			if(strstr(dllName, Entries[i].dllName) != 0){ 
				int base = Entries[i].rvaBpx + (int)debugevent->u.LoadDll.lpBaseOfDll;
				if(debug){
					sprintf(buf, "ModBpx: Setting bpx on %s.%x", dllName, base);
					Addtolist(0,1,buf);
				}
				Entries[i].va = base;
				Entries[i].enabled =true;
				Setbreakpoint(base,  TY_ACTIVE, 0);
			}
		}

	}catch(...){
		Addtolist(0,1,"***** ModBpx: Caught Error in main loop ******");
	}



}

