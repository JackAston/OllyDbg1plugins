#include        "defs.h"

HINSTANCE       g_hinstance;
HWND            hwndMain;
PEEKMESSAGEA    g_PeekMessageA;
GETKEYSTATE     g_GetKeyState;

ULONG_PTR       g_fake_ctrl;

BOOL WINAPI DllMain(__in  HINSTANCE hinstDLL, __in  DWORD fdwReason, __in  LPVOID lpvReserved){
        PIMAGE_DOS_HEADER        pmz;
        PPEHEADER32              pe32;
        PIMAGE_IMPORT_DESCRIPTOR p_import;
        ULONG_PTR                imagebase;
        PULONG_PTR               pOriginalFirstThunk;
        PULONG_PTR               pFirstThunk;
        DWORD                    dwOldProt;
        
        if (fdwReason != DLL_PROCESS_ATTACH) return TRUE;
        g_hinstance = hinstDLL;
        
        imagebase = (ULONG_PTR)GetModuleHandle(0);
        
        pmz = (PIMAGE_DOS_HEADER)imagebase;
        pe32= (PPEHEADER32)(imagebase + pmz->e_lfanew);
        
        p_import = (PIMAGE_IMPORT_DESCRIPTOR)(pe32->pe_import + imagebase);
        
        while (p_import->Name){
                if (_stricmp("user32.dll", (char *)(imagebase + p_import->Name))){
                        p_import++;
                        continue;
                }
                        
                pOriginalFirstThunk = (PULONG_PTR)(p_import->OriginalFirstThunk + imagebase);
                pFirstThunk         = (PULONG_PTR)(p_import->FirstThunk + imagebase);
                
                while (*pOriginalFirstThunk){
                        if (*pOriginalFirstThunk & 0x80000000){
                                pOriginalFirstThunk++;
                                pFirstThunk++;
                                continue;
                        }
                        
                        if (!_stricmp((char *)(*pOriginalFirstThunk + 2 + imagebase), "PeekMessageA")){
                                g_PeekMessageA = (PEEKMESSAGEA)*pFirstThunk;
                                VirtualProtect(pFirstThunk, 4, PAGE_EXECUTE_READWRITE, &dwOldProt);
                                *pFirstThunk = (ULONG_PTR)ImmSofticePeekMessageA;
                                VirtualProtect(pFirstThunk, 4, dwOldProt, &dwOldProt);
                                pOriginalFirstThunk++;
                                pFirstThunk++;
                                continue;
                        }
                        
                        if (!_stricmp((char *)(*pOriginalFirstThunk + 2 + imagebase), "GetKeyState")){
                                g_GetKeyState = (GETKEYSTATE)*pFirstThunk;
                                VirtualProtect(pFirstThunk, 4, PAGE_EXECUTE_READWRITE, &dwOldProt);
                                *pFirstThunk = (ULONG_PTR)ImmSofticeGetKeyState;
                                VirtualProtect(pFirstThunk, 4, dwOldProt, &dwOldProt);
                                pOriginalFirstThunk++;
                                pFirstThunk++;
                                continue;
                        }
                        
                        pOriginalFirstThunk++;
                        pFirstThunk++;        
                }
                
                p_import++;
        }
        
        
        return TRUE;
}


BOOL WINAPI ImmSofticePeekMessageA(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg
){
        BOOL    b_ret;
        DWORD   dwLastError;
        
        b_ret = PeekMessage(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
        dwLastError = GetLastError();
        if (!b_ret) goto __Exit0; 
        if (lpMsg->message != WM_KEYDOWN && lpMsg->message != WM_SYSKEYDOWN) goto __Exit0;
        
        InterlockedExchange((volatile LONG *)&g_fake_ctrl, FALSE);
        
        if (lpMsg->wParam >= VK_F1 && lpMsg->wParam <= VK_F12){
                if (lpMsg->wParam == VK_F7){            //it's run to cursor in softice but ignore here
                        lpMsg->message = WM_NULL;
                        goto __Exit0;
                }
                
                if (lpMsg->wParam == VK_F9){            //simulate break point F9 in SoftICE
                        lpMsg->wParam = VK_F2;
                        goto __Exit0;
                }
                
                if (lpMsg->wParam == VK_F12){
                        if (lpMsg->message == WM_SYSKEYDOWN)
                                lpMsg->message = WM_KEYDOWN;
                        lpMsg->wParam = VK_F9;
                        InterlockedExchange((volatile LONG *)&g_fake_ctrl, TRUE);
                        goto __Exit0;
                }
                
                if (lpMsg->wParam == VK_F10){
                        if (lpMsg->message == WM_SYSKEYDOWN)
                                lpMsg->message = WM_KEYDOWN;
                        lpMsg->wParam = VK_F8;
                        goto __Exit0;        
                }
                
                if (lpMsg->wParam == VK_F8){               //simulate F8, softice single step
                        lpMsg->wParam = VK_F7;
                        goto __Exit0;
                }
                
                if (lpMsg->wParam == VK_F5){               //simulate RUN (F5 in SoftICE)
                        lpMsg->wParam = VK_F9;
                        goto __Exit0;
                }       
                
        }       
        
__Exit0:
        SetLastError(dwLastError);
        return b_ret;
}


SHORT WINAPI ImmSofticeGetKeyState(
        int nVirtKey
){
        if (nVirtKey != VK_CONTROL) goto __Exit0;
        if (g_fake_ctrl == FALSE) goto __Exit0;
                return 0x8000;


__Exit0:
        return g_GetKeyState(nVirtKey);
}

int cdecl ODBG_Plugindata(char shortname[32]) {
        StringCchCopyA(shortname, 32, "ImmSoftice");       
        return PLUGIN_VERSION;
}

int cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features){
        hwndMain = hw;
        if (ollydbgversion < PLUGIN_VERSION)
                return -1;
        Addtolist(0,-1,"ImmSoftice.");
        return 0;
}

int cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item){
        if (origin == PM_MAIN){
                StringCchCopyA(data, 32, "0 About");
                return 1;
        }

        return 0;

}


void _cdecl ODBG_Pluginaction(int origin,int action,void *item){

        if (origin == PM_MAIN){
                switch (action){
                        case 0:
                        case 1:
                                MessageBoxA(hwndMain, "ImmSoftice (c) 2012 deroko of ARTeam", "ImmSoftice", 0);
                                break;
                        default:
                                break;
                }
        }
}