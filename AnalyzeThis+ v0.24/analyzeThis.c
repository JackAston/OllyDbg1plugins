
#include <windows.h>
#include <stdio.h>
#include "plugin.h"
#include "sectionBox.h"

#define PLUGIN_NAME     "Analyze This"
#define PLUGIN_INFO     "Analyze This v0.2 (by smk)"

#define and &&
#define or ||

ulong hinst=0,hwmain=0;
char tmpstr[256];

BOOL WINAPI DllMain(HINSTANCE hi,DWORD reason,LPVOID reserved){
    if(reason==DLL_PROCESS_ATTACH) hinst=hi;
    return 1;
};

char customModules[256][SHORTLEN];
int customModulesLen=0;

void hookAddr(ulong hooked,ulong hookMod){
    int i;
    int hookSize=0;
    uchar *src=(uchar*)hooked;
    t_disasm tmpDisasm;
    while(hookSize<5){
        hookSize+=Disasm(src+hookSize,MAXCMDSIZE,hooked+hookSize,NULL,&tmpDisasm,DISASM_SIZE,NULL);
    }
    uchar *addr=(ulong*)malloc(hookSize+10);
    memcpy(addr+5,src,hookSize);
    *src=0xE9;
    *((ulong*)(src+1))=(ulong)(addr)-hooked-5;
    *addr=0xE8;
    *(ulong*)(addr+1)=hookMod-(ulong)(addr)-5;
    addr+=5+hookSize;
    *addr=0xE9;
    *(ulong*)(addr+1)=hooked+hookSize-(ulong)(addr)-5;
    for(i=5;i<hookSize;i++){
        *(src+i)=0x90;
    }
}

void noDeleteModuleHook(ulong retAddr,t_sorted *sd){
    //check if destfunc is unloadModule
    if(sd->destfunc!=0x45F8F0) return;
    int i,j;
    t_module *modules=sd->data;
    //check if custom module is in module list, if it is, set again to TY_CONFIRMED
    for(i=0;i<sd->n;i++){
        for(j=0;j<customModulesLen;j++){
            if(strcmp(modules[i].name,customModules[j])==0){
                modules[i].type=TY_CONFIRMED;
                break;
            }
        }
    }
};

extc int _export ODBG_Plugindata(char shortname[32]){
    strcpy(shortname,PLUGIN_NAME);
    return PLUGIN_VERSION;
};

extc int _export ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features){
    if (ollydbgversion<PLUGIN_VERSION) return -1;
    hwmain=hw;
    Addtolist(0,0,PLUGIN_INFO);
    //hook Deletenonconfirmedsorteddata to prevent unload of new analysed modules
    ulong base=GetModuleHandle(NULL);
    ulong sectionTable=base+*((ulong*)(base+0x3C));
    ulong sectSize=*(ulong*)(sectionTable+0x100);
    ulong sectAddr=*(ulong*)(sectionTable+0x104);
    ulong oldProtect;
    VirtualProtect(base+sectAddr,sectSize,PAGE_EXECUTE_READWRITE,&oldProtect);
    hookAddr(*(ulong*)(*(ulong*)(Deletenonconfirmedsorteddata+2)),noDeleteModuleHook);
    return 0;
};

extc int _export ODBG_Pluginmenu(int origin,char data[4096],void *item){
    if(origin==PM_MAIN){
        strcpy(data,"0 &About");
    }else if(origin==PM_DISASM){
        strcpy(data,"0 Analyze This!");
    }
    return 1;
};

ulong usrEntry;
char usrName[SHORTLEN];

INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
    char tmpStr[SHORTLEN];
    if(uMsg==WM_INITDIALOG){
        //get eip, and set ENTRY_EDIT to eip
        t_thread *curThread=Findthread(Getcputhreadid());
        sprintf(tmpStr,"%08X",curThread->reg.ip);
        SetDlgItemText(hwndDlg,ENTRY_EDIT,tmpStr);
        SendMessage(GetDlgItem(hwndDlg,ENTRY_EDIT),EM_LIMITTEXT,SHORTLEN,0);
        SendMessage(GetDlgItem(hwndDlg,NAME_EDIT),EM_LIMITTEXT,SHORTLEN,0);
    }else if(uMsg==WM_COMMAND){
        if((wParam&0xFFFF)==OK_BUTTON){
            int i;
            GetDlgItemText(hwndDlg,ENTRY_EDIT,tmpStr,SHORTLEN+1);
            //check if all chars are within hex-alphabet
            usrEntry=0;
            for(i=0;i<strlen(tmpStr);i++){
                if(tmpStr[i]>='0' and tmpStr[i]<='9'){
                    usrEntry<<=4;
                    usrEntry|=tmpStr[i]-'0';
                    continue;
                }else if(tmpStr[i]>='A' and tmpStr[i]<='F'){
                    usrEntry<<=4;
                    usrEntry|=tmpStr[i]-'A'+10;
                    continue;
                }else if(tmpStr[i]>='a' and tmpStr[i]<='f'){
                    usrEntry<<=4;
                    usrEntry|=tmpStr[i]-'a'+10;
                    continue;
                }else{
                    MessageBox(hwndDlg,"Entry point must be represented by hex chars","Error",MB_ICONERROR);
                    return TRUE;
                }
            }
            ulong start,size;
            Getdisassemblerrange(&start,&size);
            if(usrEntry<start or usrEntry>start+size){
                char errText[TEXTLEN];
                sprintf(errText,"Entry point must be between %x and %x",start,start+size);
                MessageBox(hwndDlg,errText,"Error",MB_ICONERROR);
                return TRUE;
            }
            GetDlgItemText(hwndDlg,NAME_EDIT,usrName,SHORTLEN);
            t_table* modTable=Plugingetvalue(VAL_MODULES);
            t_module* modules=modTable->data.data;
            if(strlen(usrName)==0){
                MessageBox(hwndDlg,"Please enter a module name","Error",MB_ICONERROR);
                return TRUE;
            }
            for(i=0;i<modTable->data.n;i++){
                memcpy(tmpStr,modules[i].name,SHORTLEN);
                tmpStr[SHORTLEN]=0;
                if(strcmp(usrName,tmpStr)==0){
                    MessageBox(hwndDlg,"Module name already exists","Error",MB_ICONERROR);
                    return TRUE;
                }
            }
            strcpy(customModules[customModulesLen],usrName);
            customModulesLen++;

            t_module curMod;
            curMod.base=start;
            curMod.size=size;
            curMod.type=TY_NEW;
            curMod.codebase=start;
            curMod.codesize=size;
            curMod.resbase=0;
            curMod.ressize=0;
            curMod.stringtable=NULL;
            curMod.nstringtable=0;
            curMod.maxstringtable=0;
            curMod.entry=usrEntry;
            curMod.database=0;
            curMod.idatatable=0;
            curMod.idatabase=0;
            curMod.edatatable=0;
            curMod.edatasize=0;
            curMod.reloctable=0;
            curMod.relocsize=0;
            strcpy(curMod.name,usrName);
            strcpy(curMod.path,usrName);
            curMod.nsect=1;

            IMAGE_SECTION_HEADER *newSects=(IMAGE_SECTION_HEADER*)malloc(sizeof(IMAGE_SECTION_HEADER)*curMod.nsect);
            IMAGE_SECTION_HEADER addedSect;
            addedSect.Name[0]=0;
            addedSect.Misc.PhysicalAddress=0;
            addedSect.Misc.VirtualSize=size;
            addedSect.VirtualAddress=0;
            addedSect.SizeOfRawData=0;
            addedSect.PointerToRawData=NULL;
            addedSect.PointerToRelocations=NULL;
            addedSect.PointerToLinenumbers=NULL;
            addedSect.NumberOfRelocations=0;
            addedSect.NumberOfLinenumbers=0;
            addedSect.Characteristics=IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE|IMAGE_SCN_MEM_EXECUTE;
            newSects[0]=addedSect;
            curMod.sect=newSects;

            //unkown module has no header, set to some constant
            curMod.headersize=0x600;
            curMod.fixupbase=0x400000;
            curMod.nfixup=NULL;
            curMod.fixup=NULL;
            curMod.codedec=NULL;
            curMod.codecrc=0;
            curMod.hittrace=NULL;
            curMod.hittracecopy=NULL;
            curMod.datadec=NULL;

            //in ollydbg.exe, this module has namelist a t_table with data.n: 0
            t_table nlTable;
            nlTable.hw=NULL;
            //
            t_sorted nlSorted;
            nlSorted.name[0]=0;
            nlSorted.n=0;
            nlSorted.nmax=0;
            nlSorted.selected=0;
            nlSorted.seladdr=0;
            nlSorted.itemsize=0;
            nlSorted.version=0;
            nlSorted.data=NULL;
            nlSorted.sortfunc=NULL;
            nlSorted.destfunc=NULL;
            nlSorted.sort=0;
            nlSorted.sorted=0;
            nlSorted.index=NULL;
            nlSorted.suppresserr=0;
            nlTable.data=nlSorted;
            //
            t_bar nlBar;
            nlBar.nbar=0;
            nlBar.font=0;
            nlBar.captured=CAPT_FREE;
            nlBar.active=0;
            nlBar.prevx=0;
            nlTable.bar=nlBar;
            //
            nlTable.showbar=0;
            nlTable.hscroll=0;
            nlTable.colsel=0;
            nlTable.mode=0;
            nlTable.font=0;
            nlTable.scheme=0;
            nlTable.hilite=0;
            nlTable.offset=0;
            nlTable.xshift=0;
            nlTable.drawfunc=NULL;
            curMod.namelist=nlTable;

            curMod.symvar=NULL;
            curMod.nsymvar=0;
            curMod.maxsymvar=0;
            curMod.mainentry=0;
            curMod.realsfxentry=0;
            curMod.updatenamelist=0;
            curMod.origcodesize=0;
            curMod.sfxbase=0;
            curMod.sfxsize=0;
            curMod.issystemdll=0;
            curMod.processed=0;
            curMod.dbghelpsym=0;
            strcpy(curMod.version,"0.0");
            curMod.jddata=NULL;
            curMod.njddata=0;

            Addsorteddata(&(modTable->data),(void*)(&curMod));
            t_module *curDasmMod=Findmodule(start);
            Analysecode(curDasmMod);

            SendMessage(hwndDlg,WM_CLOSE,0,0);
            return TRUE;
        }
    }else if(uMsg==WM_CLOSE){
        EndDialog(hwndDlg,0);
    }
    return FALSE;
}

extc void _export ODBG_Pluginaction(int origin,int action,void *item){
    if(origin==PM_MAIN){
        if(action==0)
            MessageBox(hwmain,PLUGIN_INFO,PLUGIN_NAME,MB_ICONINFORMATION);
    }else if(origin==PM_DISASM){
        if(Getcputhreadid()==NULL){
            MessageBox(hwmain,PLUGIN_INFO,PLUGIN_NAME,MB_ICONINFORMATION);
            return;
        }
        ulong start,size;
        Getdisassemblerrange(&start,&size);
        t_module *curDasmMod=Findmodule(start);
        if(curDasmMod!=NULL){
            Analysecode(curDasmMod);
            return;
        }
        //create a new module at current address, if current address is not in a module
        //ask user for module ep,name
        DialogBoxParam(hinst,SECTION_INFO,hwmain,DialogProc,0);
    };
};
