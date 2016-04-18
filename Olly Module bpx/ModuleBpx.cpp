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
    9/16/06 - 
				fixed potential bof
				fixed dialog display error (2nd display did not show)
				added code to remove breakpoints when item deleted from list
				couple misc cleanups
	9/20/06 -
				added calc form 
				couple misc cleanups


*/

#include <windows.h>
#include <stdio.h>
#include <StdDef.H> // offsetof()
#include "resource.h"
#include "plugin.h"

struct BpxEntry{
	char              dllName[50];
	unsigned int      rvaBpx;
	unsigned int      va;
};

BpxEntry   Entries[50];
int        numEntries = -1;
int        m_hProcess = 0;
bool       debug	   = false;
HINSTANCE  hinst;
HWND       hwmain;
HINSTANCE  myHnd;
char       szFileName[MAX_PATH]={0};
HWND       dlgWin;

 

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
		sprintf(buf, "%d:          %s          %x", i, Entries[i].dllName, Entries[i].rvaBpx );
		SendMessage(hList,LB_ADDSTRING,0,(int)&buf);
	}
}

void SetTop(HWND h, bool top=true){
	HWND flag = top ? HWND_TOPMOST : HWND_NOTOPMOST ;	
	SetWindowPos(h,flag,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
}

bool AddEntry(char* mod, int rva){

	if(numEntries > 49) return false;
	if(strlen(mod) > 49) mod[49]=0;

	numEntries++;
	strncpy(Entries[numEntries].dllName ,mod,strlen(mod));
	Entries[numEntries].rvaBpx = rva;
	
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

        if (strstr(buf, modName) != 0){
		     return module->base;
        }
    }

	return 0;

}

BOOL CALLBACK CalcProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	char buf [200]={0}, modName[50]={0},sva[50] = {0};
    unsigned int i=0, index=0, va=0, hexValue=0, ret=0;

	HWND hName = GetDlgItem(hWnd, IDC_EDIT1);
	HWND hVa   = GetDlgItem(hWnd, IDC_EDIT2);
	HWND hRva  = GetDlgItem(hWnd, IDC_EDIT3);
	
	switch(uMsg)
	{
		case WM_INITDIALOG : SetTop(hWnd);break;
		case WM_CLOSE:	EndDialog(hWnd,0);break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			case IDOK:
					GetWindowText(hName, modName,49);
					GetWindowText(hVa, sva,49);
					strlower(modName);
					ret = FindModuleFromName(modName);
					if(ret==0){
						SetWindowText(hRva,"NoMatch");
						break;
					}
					if(strlen(sva)==0){
						sprintf(sva,"Base=%x",ret);
						SetWindowText(hRva,(char*)&sva);
						break;
					}
					va = strtol(sva, NULL, 16);
					if(va > 0 && va < ret){
						sprintf(sva,"Va < Base(%x)", ret);
						SetWindowText(hRva,(char*)&sva);
						break;
					}
					ret = va - ret;
					sprintf(sva,"%x",ret);
					SetWindowText(hRva,(char*)&sva);
					break;

			case IDC_BUTTON1:
					GetWindowText(hName, modName,49);
					GetWindowText(hRva, sva,49);
					strlower(modName);
					va = strtol(sva, NULL, 16);
					if(va > 0 && strlen(modName) > 0 ){
						AddEntry(modName, va);
						MessageBox(hWnd,"Entry Added OK","",0);
						break;
					}
					MessageBox(hWnd,"Could not add entry","",0);
					break;


			}
	}

	return FALSE;

}




// This is the message loop
BOOL CALLBACK MyDlgProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{

	char buf [200]={0};
    unsigned int i=0, index=0, rva=0, hexValue=0;
	char modName[50]={0};
	char sRva[50] = {0};
	int ret=0;
	FILE* dat;

	dlgWin = hWnd;
	HWND hName = GetDlgItem(hWnd, IDC_EDIT1);
	HWND hRva  = GetDlgItem(hWnd, IDC_EDIT2);
	HWND hList = GetDlgItem(hWnd, IDC_LIST2);
	HWND hCheck= GetDlgItem(hWnd, IDC_CHECK1);

	switch(uMsg)
	{
		case WM_INITDIALOG :
			
			FillList(hList);
			if(debug) SendMessage(hCheck, BM_SETCHECK, BST_CHECKED ,0);
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

					if( Entries[index].va != 0 ) Setbreakpoint(Entries[index].va , TY_DISABLED,0);

					RemoveEntry(index);
					FillList(hList);
					break;
			
				case IDC_BUTTON2: //OK button ID
					
	
					GetWindowText(hName, modName,49);
					GetWindowText(hRva, sRva,49);

					strlower(modName);
					rva = strtol(sRva, NULL, 16);
					
					if(rva==0 || strlen(modName) == 0){
						MessageBox(0,"Invalid Input","",0);
						break;
					}

					if( AddEntry(modName, rva) ){
						sprintf(buf, "%d:          %s          %x", numEntries, modName, rva);
						SendMessage(hList,LB_ADDSTRING, 0, (int)&buf);
					}
					else{
						MessageBox(0,"Failed to add (maxed out)","",0);
					}
						

					break;

				case IDC_BUTTON3: //save list

					try{

						SetTop(hWnd,false);
						ret = Browsefilename("Save File As",(char*)&szFileName,".bpl",0x80);
						
						if(ret){
							if(debug) Addtolist(0,0,"Saving to file: %s", szFileName);
							dat = fopen(szFileName, "wb");
							fwrite((void*)Entries, sizeof(Entries),1,dat);
							fwrite((void*)&numEntries, 4,1,dat);
							fclose(dat);
						}
					}
					catch(...){
						MessageBox(0,"Error saving list","",0);
					}

					SetTop(hWnd);
					break;

				case IDC_BUTTON4: //load saved

					try{
						SetTop(hWnd,false);
						ret = Browsefilename("Open File",(char*)&szFileName,".bpl;",0);

						if(ret){
							if(debug) Addtolist(0,0,"Loading file: %s", szFileName);
							dat = fopen((char*)&szFileName, "rb");
							fread((void*)Entries, sizeof(Entries),1,dat);
							fread((void*)&numEntries, 4,1,dat);
							fclose(dat);
							if(numEntries > 49) numEntries = 49;
							for(i=0 ; i < numEntries; i++) Entries[i].va = 0 ;
							FillList(hList);
						}

					}
					catch(...){
						MessageBox(0,"Error loading list","",0);
					}

					SetTop(hWnd);
					break;

				case IDC_BUTTON5: //clear list

					for(i=0 ; i< numEntries; i++){
						if( Entries[i].va != 0 ) Setbreakpoint(Entries[i].va , TY_DISABLED,0);
					}

					memset( (void*)Entries, 0, sizeof(Entries));
					numEntries = -1;
					FillList(hList);
					break;

				case IDC_BUTTON6: //calc button

					SetTop(hWnd,false);
					DialogBox( myHnd,MAKEINTRESOURCE(IDD_DIALOG1),0,(DLGPROC)CalcProc); //modal
					FillList(hList);
					SetTop(hWnd);
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
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;
  return 1;
};

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,"Break on Module");
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit( int ollydbgversion,HWND hw,ulong *features) {
  if (ollydbgversion<PLUGIN_VERSION) return -1;
  Addtolist(0,0,"Initilizing ModuleBpx Plugin - David Zimmer (dzzie@yahoo.com)");
  hwmain=hw;
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {

  switch (origin) {

		case PM_MAIN:
			  strcpy(data,"0 &Break On Module, 1 About");
			  return 1;

		default: break;
  };

  return 0;

};

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  if (origin==PM_MAIN) {
    switch (action) {
      case 0:
			if( IsWindow(dlgWin) ){
				SetTop(dlgWin);
			}
			else{
				DialogBox( myHnd,MAKEINTRESOURCE(IDD_FORMVIEW),0,(DLGPROC)MyDlgProc); //modal
			}
			break;
	  case 1:
			 MessageBox(0,"Break On Module - David Zimmer <dzzie@yahoo.com>\n\n"
						   "Usage: Enter the dllname and the rva of the\n"
						   "code you want olly to break on\n\n"
						   "When the dll is loaded the va is calculated\n"
						   "from the dllBase and breakpoint set\n\n"
						   "(You have to use rva because dll could be rebased)",			 
							"Break on Module",0);
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






extc void _export cdecl ODBG_Pluginmainloop(DEBUG_EVENT *debugevent){

	
	if( (int)debugevent == 0) return;
	
	if(debugevent->dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT){
		m_hProcess = (int)debugevent->u.CreateProcessInfo.hProcess;
	}
	
	if(numEntries == -1) return;
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
			if(strstr(dllName, Entries[i].dllName) != 0){ 
				Entries[i].va = Entries[i].rvaBpx + (int)debugevent->u.LoadDll.lpBaseOfDll;
				if(debug){
					sprintf(buf, "ModBpx: Setting bpx on %s.%x", dllName, Entries[i].va);
					Addtolist(0,1,buf);
				}
				Setbreakpoint(Entries[i].va,  TY_ACTIVE, 0);
			}
		}

	}catch(...){
		Addtolist(0,1,"***** ModBpx: Caught Error in main loop ******");
	}



}

