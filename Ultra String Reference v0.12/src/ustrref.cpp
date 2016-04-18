////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   ustrref.cpp
//  Version     :   1.0
//  Creater     :   Luo Cong
//  Date        :   2006-7-28 10:19:17
//  Comment     :   
//
////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ustrref.h"
#include "Plugin.h"
#include "StrFinder.h"
#include "StringList.h"

#ifdef _DEBUG
#   define DEBUG_NEW new (_NORMAL_BLOCK, THIS_FILE, __LINE__)
#   define new DEBUG_NEW
#   undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#define VERSIONHI   0
#define VERSIONLO   12

static HWND         g_hWndMain;
static HINSTANCE    g_hInstance;
static t_table      g_ustrreftbl;
static char         g_szustrrefclass[32];
static int          g_nCurEip_Str_Index     = 0;
static int          g_nTotalStrCount;
static CStringList  g_StrList;
static char         g_szTextToFind[TEXTLEN] = { 0 };
static DWORD        g_dwPrevAddr = -1;

DWORD g_dwCurEip;

typedef struct t_ustrref
{
    int             nIndex;             // ustrref index
    ulong           size;               // Size of index, always 1 in our case
    ulong           type;               // Type of entry, always 0
    ulong           addr;               // Address of string
    BOOL            bIsCurEip;          // Is Current EIP?
} t_ustrref;

BOOL APIENTRY DllMain(
    HINSTANCE hModule,
    DWORD  dwReason,
    LPVOID lpReserved
)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    if (DLL_PROCESS_ATTACH == dwReason)
        g_hInstance = hModule;
    return TRUE;
}

int FindNextAndScroll(HWND hWnd, int nStartPos)
{
    int     i;
    int     found = 0;
    char    *pszStr;
    char    *pdest = NULL;
    char    texttofind[TEXTLEN];
    
    strcpy(texttofind, g_szTextToFind);

    for (i = nStartPos; i < g_StrList.GetCount(); ++i)
    {
        pszStr = g_StrList.GetNodeStrAt(i);
        pdest = strstr(
            strlwr(pszStr),
            strlwr(texttofind)
        );
        if (NULL != pdest)
        {
            Selectandscroll(&g_ustrreftbl, i, 2);
            InvalidateRect(hWnd, NULL, FALSE);
            found = 1;
            break;
        }
    }
    return found;
}

int FindPreviousAndScroll(HWND hWnd, int nStartPos)
{
    int     i;
    int     found = 0;
    char    *pszStr;
    char    *pdest = NULL;
    char    texttofind[TEXTLEN];
    
    strcpy(texttofind, g_szTextToFind);

    for (i = nStartPos; i >= 0; --i)
    {
        pszStr = g_StrList.GetNodeStrAt(i);
        pdest = strstr(
            strlwr(pszStr),
            strlwr(texttofind)
        );
        if (NULL != pdest)
        {
            Selectandscroll(&g_ustrreftbl, i, 2);
            InvalidateRect(hWnd, NULL, FALSE);
            found = 1;
            break;
        }
    }
    return found;
}

void FindPrevious(HWND hWnd)
{
    int found = 0;
    t_ustrref *pb;

    pb = (t_ustrref *)Getsortedbyselection(
        &(g_ustrreftbl.data), g_ustrreftbl.data.selected);
    if (NULL != pb && '\0' != g_szTextToFind[0])
    {
        found = FindPreviousAndScroll(hWnd, pb->nIndex - 1);
        // if not found, scroll to the last index and find again
        if (!found)
        {
            Flash(defPASSEDTHEENDOFFILE);
            FindPreviousAndScroll(hWnd, g_StrList.GetCount() - 1);
        }
    }
}

void FindNext(HWND hWnd)
{
    int found = 0;
    t_ustrref *pb;

    pb = (t_ustrref *)Getsortedbyselection(
        &(g_ustrreftbl.data), g_ustrreftbl.data.selected);
    if (NULL != pb && '\0' != g_szTextToFind[0])
    {
        found = FindNextAndScroll(hWnd, pb->nIndex + 1);
        // if not found, scroll to index 0 and find again
        if (!found)
        {
            Flash(defPASSEDTHEENDOFFILE);
            FindNextAndScroll(hWnd, 0);
        }
    }
}

void Find(HWND hWnd)
{
    int found = 0;
    int nRetCode;
    t_ustrref *pb;
    char textnotfound[20 + TEXTLEN];

    nRetCode = Gettext("Find", g_szTextToFind, 0, NM_COMMENT, FIXEDFONT);
    if (-1 == nRetCode)
        return ;

    pb = (t_ustrref *)Getsortedbyselection(
        &(g_ustrreftbl.data), g_ustrreftbl.data.selected);

    if (NULL != pb && '\0' != g_szTextToFind[0])
    {
        // find from current selected index
        found = FindNextAndScroll(hWnd, pb->nIndex);
        // if not found, scroll to index 0 and find again
        if (!found)
        {
            found = FindNextAndScroll(hWnd, 0);
            if (!found)
            {
                sprintf(textnotfound, "\"%s\" not found!", g_szTextToFind);
                MessageBox(
                    hWnd,
                    textnotfound,
                    defPLUGINNAME,
                    MB_OK | MB_ICONINFORMATION
                );
            }
        }
    }
}

int cdecl UStrRefGetText(
    char *s,
    char *mask,
    int *select,
    t_sortheader *ph,
    int column
)
{
    t_disasm      da;
    int           n = 0;
    t_memory      *pmem;
    ulong         cmdsize;
    uchar         *pdecode;
    ulong         decodesize;
    unsigned char cmd[MAXCMDSIZE];
    t_ustrref     *pb = (t_ustrref *)ph;
    char          *pszStr;

    if (pb->bIsCurEip)   // Is Current EIP
        *select = DRAW_HILITE;
    else
        *select = NULL;

    switch (column)
    {
    case 0: // Address
        n = sprintf(s, "%08X", pb->addr);
        break;

    case 1: // Disassembly
        pmem = Findmemory(pb->addr);
        if (NULL == pmem)
        {
            *select = DRAW_GRAY;
            return sprintf(s, "???");
        }
        cmdsize = pmem->base + pmem->size - pb->addr;
        if (cmdsize > MAXCMDSIZE)
            cmdsize = MAXCMDSIZE;
        if (
            cmdsize != Readmemory(cmd, pb->addr, cmdsize, MM_RESTORE|MM_SILENT)
        )
        {
            *select = DRAW_GRAY;
            return sprintf(s, "???");
        }
        pdecode = Finddecode(pb->addr, &decodesize);
        if (decodesize < cmdsize)
            pdecode = NULL;
        Disasm(cmd, cmdsize, pb->addr, pdecode, &da, DISASM_CODE, 0);
        n = sprintf(s, "%s", da.result);
        break;

    case 2: // Text String
        pszStr = g_StrList.GetNodeStrAt(pb->nIndex);
        n = sprintf(s, "%s", pszStr);
        if (g_dwPrevAddr != pb->addr)
            Insertname(pb->addr, NM_COMMENT, pszStr);
        g_dwPrevAddr = pb->addr;
        break;
    }
    return n;
}

void DoComment(ulong addr, char *cmt)
{
    int nRetCode;

    char text[TEXTLEN];

    nRetCode = Findname(addr, NM_ANALYSE, text);
    if (0 == nRetCode)
    {
        Insertname(addr, NM_COMMENT, cmt);
        Broadcast(WM_USER_CHALL, 0, 0);
    }
    /*
    else
    {
        Deletenamerange(addr, addr + 1, NM_COMMENT);
        Broadcast(WM_USER_CHALL, 0, 0);
    }
    */
}

LRESULT CALLBACK UStrRefWndProc(
    HWND hWnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
)
{
    int         i;
    t_ustrref   *pb;
    HMENU       menu;

    switch (msg)
    {
    // Standard messages. You can process them, but - unless absolutely sure -
    // always pass them to Tablefunction().
    case WM_DESTROY:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_HSCROLL:
    case WM_VSCROLL:
    case WM_TIMER:
    case WM_SYSKEYDOWN:
        Tablefunction(&g_ustrreftbl, hWnd, msg, wParam, lParam);
        break;                           // Pass message to DefMDIChildProc()
    // Custom messages responsible for scrolling and selection. User-drawn
    // windows must process them, standard OllyDbg windows without extra
    // functionality pass them to Tablefunction().
    case WM_USER_SCR:
    case WM_USER_VABS:
    case WM_USER_VREL:
    case WM_USER_VBYTE:
    case WM_USER_STS:
    case WM_USER_CNTS:
    case WM_USER_CHGS:
        return Tablefunction(&g_ustrreftbl, hWnd, msg, wParam, lParam);
    // WM_WINDOWPOSCHANGED is responsible for always-on-top MDI windows.
    case WM_WINDOWPOSCHANGED:
        return Tablefunction(&g_ustrreftbl, hWnd, msg, wParam, lParam);
    case WM_USER_MENU:
        menu = CreatePopupMenu();
        // Find selected string. Any operations with string make sense only
        // if at least one string exists and is selected. Note that sorted data
        // has special sort index table which is updated only when necessary.
        // Getsortedbyselection() does this; some other sorted data functions
        // don't and you must call Sortsorteddata().
        pb = (t_ustrref *)Getsortedbyselection(
                &(g_ustrreftbl.data), g_ustrreftbl.data.selected);
        if (NULL != menu && NULL != pb)
        {
            AppendMenu(menu, MF_STRING, 1, "&Follow\tEnter");
            AppendMenu(menu, MF_SEPARATOR, NULL, NULL);
            //AppendMenu(menu, MF_STRING, 2, "&Search\tCtrl+F");
            AppendMenu(menu, MF_STRING, 2, "F&ind\tCtrl+F or Insert");
            AppendMenu(menu, MF_STRING, 3, "Find &Next\tb or n");
            AppendMenu(menu, MF_STRING, 4, "Find &Previous\tz or p");
        }
        // Even when menu is NULL, call Tablefunction is still meaningful.
        i = Tablefunction(
            &g_ustrreftbl,
            hWnd,
            WM_USER_MENU,
            0,
            (LPARAM)menu
        );
        if (NULL != menu)
            DestroyMenu(menu);
        if (1 == i)                 // Follow address in Disassembler
        {
            Setcpu(
                0,
                pb->addr,
                0,
                0,
                CPU_ASMHIST | CPU_ASMCENTER | CPU_ASMFOCUS
            );
        }
        else if (2 == i)            // Find
        {
            Find(hWnd);
        }
        else if (3 == i)            // Find next
        {
            FindNext(hWnd);
        }
        return 0;
    case WM_KEYDOWN:
        // Processing of WM_KEYDOWN messages is - surprise, surprise - very
        // similar to that of corresponding menu entries.
        switch (wParam)
        {
        case VK_RETURN:
            // Return key follows address in Disassembler.
            pb = (t_ustrref *)Getsortedbyselection(
                &(g_ustrreftbl.data), g_ustrreftbl.data.selected
            );
            if (NULL != pb)
            {
                Setcpu(
                    0,
                    pb->addr,
                    0,
                    0,
                    CPU_ASMHIST | CPU_ASMCENTER | CPU_ASMFOCUS
                );
            }
            break;
        case VK_INSERT:
            Find(hWnd);
            break;
        case 0x46:  // Ctrl+'F' or Ctrl+'f'
            if (GetKeyState(VK_CONTROL) < 0)    // Ctrl key is pressing
                Find(hWnd);
            break;
        case 0x42:  // VK_B
        case 0x4e:  // VK_N
            FindNext(hWnd);
            break;
        case 0x5a:  // VK_Z
        case 0x50:  // VK_P
            FindPrevious(hWnd);
            break;
        default:
            // Add all this arrow, home and pageup functionality.
            Tablefunction(&g_ustrreftbl, hWnd, msg, wParam, lParam);
            break;
        }
        break;
    case WM_USER_DBLCLK:
        // Doubleclicking row follows address in Disassembler.
        pb = (t_ustrref *)Getsortedbyselection(
            &(g_ustrreftbl.data), g_ustrreftbl.data.selected);
        if (NULL != pb)
        {
            Setcpu(
                0,
                pb->addr,
                0,
                0,
                CPU_ASMHIST | CPU_ASMCENTER | CPU_ASMFOCUS
            );
        }
        return 1;                        // Doubleclick processed
    case WM_USER_CHALL:
    case WM_USER_CHMEM:
        // Something is changed, redraw window.
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    case WM_PAINT:
        // Fill background of search window with default button colour.
        // Necessary because Registerpluginclass() sets hbrBackground to NULL.

        // Painting of all OllyDbg windows is done by Painttable(). Make custom
        // drawing only if you have important reasons to do this.
        Painttable(hWnd, &g_ustrreftbl, UStrRefGetText);
        return 0;
    default:
        break;
    }

    return DefMDIChildProc(hWnd, msg, wParam, lParam);
}

void CreateUStrRefWindow(void)
{
    if (0 == g_ustrreftbl.bar.nbar)
    {
        g_ustrreftbl.bar.name[0]    = "Address";
        g_ustrreftbl.bar.defdx[0]   = 9;
        g_ustrreftbl.bar.mode[0]    = 0;
        g_ustrreftbl.bar.name[1]    = "Disassembly";
        g_ustrreftbl.bar.defdx[1]   = 40;
        g_ustrreftbl.bar.mode[1]    = BAR_NOSORT;
        g_ustrreftbl.bar.name[2]    = "Text String";
        g_ustrreftbl.bar.defdx[2]   = 256;
        g_ustrreftbl.bar.mode[2]    = BAR_NOSORT;
        g_ustrreftbl.bar.nbar       = 3;
        g_ustrreftbl.mode =
            TABLE_COPYMENU |
            TABLE_SORTMENU |
            TABLE_APPMENU  |
            TABLE_SAVEPOS  |
            TABLE_ONTOP;
        g_ustrreftbl.drawfunc = UStrRefGetText;
    }

    Quicktablewindow(
        &g_ustrreftbl,
        15,
        3,
        g_szustrrefclass,
        defPLUGINNAME
    );
}

int UStrRefSortFunc(t_ustrref *b1, t_ustrref *b2, int sort)
{
    int i = 0;

    if (0 == sort)
    {
        // Sort by address
        if (b1->addr < b2->addr)
            i = -1;
        else if (b1->addr > b2->addr)
            i = 1;
    };
    // If elements are equal or sorting is by the first column, sort by index.
    if (0 == i)
    {
        if (b1->nIndex < b2->nIndex)
            i = -1;
        else if (b1->nIndex > b2->nIndex)
            i = 1;
    };
    return i;
}

void ShowAboutInfo()
{
    char szAboutInfo[255];

    sprintf(
        szAboutInfo,
        "Ultra String Reference plugin v%d.%02d\n"
        "Compiled on " __DATE__ ", " __TIME__ "\n\n"
        "CopyLeft (C) ÂÞ´Ï at Tencent\n"
        "Homepage: http://www.luocong.com\n"
        "E-Mail  : admin@luocong.com",
        VERSIONHI, VERSIONLO
    );

    MessageBox(
        g_hWndMain, szAboutInfo, defPLUGINNAME, MB_OK | MB_ICONINFORMATION
    );
}

static DWORD GetCurrentEip(void)
{
    t_thread* t2;

    t2 = Findthread(Getcputhreadid());
    return t2->reg.ip;
}

static void StrFinderCallBack(
    const int nStrIndex,
    const DWORD dwBase,
    const DWORD dwOffset,
    const DWORD dwSize,
    const STR_FINDER_STRING_TYPE StrType,
    const char cszStr[]
)
{
    BOOL bRetCode;
    t_ustrref item;
    BOOL bIsCurEip;
    PVOID pvData;
    DWORD dwAddr = dwBase + dwOffset;

    bIsCurEip = dwAddr == g_dwCurEip ? TRUE : FALSE;

    item.nIndex = nStrIndex;
    item.size = 1;
    item.type = 0;
    item.addr = dwAddr;
    item.bIsCurEip = bIsCurEip;

    pvData = Addsorteddata(&(g_ustrreftbl.data), &item);
    PROCESS_ERROR(pvData);

    bRetCode = g_StrList.AddHeadNode(cszStr);
    PROCESS_ERROR(bRetCode);

    Progress(dwOffset * 1000 / dwSize, "Strings found: %d", nStrIndex);

    if (bIsCurEip)
        g_nCurEip_Str_Index = nStrIndex;

    ++g_nTotalStrCount;

Exit0:
    return ;
}

static BOOL FindStr(const STR_FINDER_STRING_TYPE StrType)
{
    BOOL bRetResult = FALSE;
    BOOL bRetCode;
    DWORD dwBase;
    DWORD dwSize;
    CStrFinder StrFinder;

    g_nTotalStrCount = 0;
    g_nCurEip_Str_Index = -1;
    g_dwPrevAddr = -1;

    if (g_ustrreftbl.data.n)
    {
        Deletesorteddatarange(
            &(g_ustrreftbl.data),
            0,
            g_ustrreftbl.data.n
        );
    }

    g_dwCurEip = GetCurrentEip();

    Getdisassemblerrange(&dwBase, &dwSize);
    if (0 == dwBase || 0 == dwSize)
        goto Exit1;

    g_StrList.RemoveAllNodes();

    bRetCode = StrFinder.Find(StrType, dwBase, dwSize, StrFinderCallBack);
    PROCESS_ERROR(bRetCode);

    Progress(0, "$");
    Infoline(
        "Total strings found: %d  -  Ultra String Reference (%s Mode)",
        g_nTotalStrCount,
        g_szStrFinderStrType[StrType]
    );

    CreateUStrRefWindow();

    if (-1 != g_nCurEip_Str_Index)
    {
        Selectandscroll(&g_ustrreftbl, g_nCurEip_Str_Index, 2);
    }

Exit1:
    bRetResult = TRUE;
Exit0:
    return bRetResult;
}

extc void _export cdecl ODBG_Pluginaction(int origin, int action, void *item)
{
    BOOL bRetCode;

    switch (origin)
    {
    case PM_MAIN:
        switch (action)
        {
        case 0:     // Find ASCII
            bRetCode = FindStr(enumSFST_Ascii);
            PROCESS_ERROR(bRetCode);
            break;

        case 1:     // Find UNICODE
            bRetCode = FindStr(enumSFST_Unicode);
            PROCESS_ERROR(bRetCode);
            break;

        case 2:     // About
            ShowAboutInfo();
            break;
        }
        break;

    case PM_DISASM:
        switch (action)
        {
        case 0:     // Find ASCII
            bRetCode = FindStr(enumSFST_Ascii);
            PROCESS_ERROR(bRetCode);
            break;

        case 1:     // Find UNICODE
            bRetCode = FindStr(enumSFST_Unicode);
            PROCESS_ERROR(bRetCode);
            break;

        case 2:     // About
            ShowAboutInfo();
            break;
        }   // switch (action)
        break;  // case PM_DISASM
    }

Exit0:
    return ;
}

extc int _export cdecl ODBG_Plugininit(
    int ollydbgversion,
    HWND hw,
    ulong *features
)
{
    int nRetResult = 0;
    int nRetCode;

    if (ollydbgversion < PLUGIN_VERSION)
    {
        nRetResult = -1;
        goto Exit0;
    }

    g_hWndMain = hw;

    nRetCode = Createsorteddata(
        &(g_ustrreftbl.data),
        "ustrref",
        sizeof(t_ustrref),
        100,
        (SORTFUNC *)UStrRefSortFunc,
        NULL
    );

    nRetCode = Registerpluginclass(
        g_szustrrefclass,
        NULL,
        g_hInstance,
        UStrRefWndProc
    );
    if (nRetCode < 0)
    {
        Destroysorteddata(&(g_ustrreftbl.data));
        nRetResult = -1;
        goto Exit0;
    }

    // Report plugin in the log window.
    Addtolist(0, 0, "%s v%d.%02d", defPLUGINNAME, VERSIONHI, VERSIONLO);
    Addtolist(0, -1, "  Written by Luo Cong");
    Addtolist(0, -1, "  Compiled on " __DATE__ ", " __TIME__);

    if (
        (0 != Plugingetvalue(VAL_RESTOREWINDOWPOS)) &&
        (0 != Pluginreadintfromini(g_hInstance, defINIKEY, 0))
    )
    {
        CreateUStrRefWindow();
    }

Exit0:
    return nRetResult;
}

extc int _export cdecl ODBG_Pluginclose(void)
{
    Pluginwriteinttoini(
        g_hInstance,
        defINIKEY,
        NULL != g_ustrreftbl.hw
    );
    return 0;
}

extc void _export cdecl ODBG_Plugindestroy(void)
{
    Unregisterpluginclass(g_szustrrefclass);
    Destroysorteddata(&(g_ustrreftbl.data));
}

extc int _export cdecl ODBG_Plugindata(char shortname[32])
{
    strcpy(shortname, defPLUGINNAME);
    return PLUGIN_VERSION;
}

extc void _export cdecl ODBG_Pluginreset(void)
{
    Deletesorteddatarange(&(g_ustrreftbl.data), 0, 0xFFFFFFFF);
}

extc int _export cdecl ODBG_Pluginmenu(int origin, char data[4096], void *item)
{
    t_dump *pd;

    switch (origin)
    {
    case PM_MAIN:
        strcpy(data, "0 &1 Find ASCII|1 &2 Find UNICODE|2 &3 About");
        return 1;

    case PM_DISASM:     // Popup menu in Disassembler
        pd = (t_dump *)item;
        if (NULL == pd || 0 == pd->size)    // Window empty, don't add
            return 0;
        // Add item "Ultra String Reference"
        // if some part of Disassembler is selected
        if (pd->sel1 > pd->sel0)
        {
            sprintf(
                data,
                "0 &Ultra String Reference{"
                "0 &1 Find ASCII|1 &2 Find UNICODE|2 &3 About"
                "}"
            );
        }
        return 1;
    }

    return 0;
}