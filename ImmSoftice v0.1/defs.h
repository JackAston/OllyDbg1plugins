#include        <windows.h>
#include        <strsafe.h>
#include        "pe64.h"
#include        "plugin.h"

typedef BOOL (WINAPI *PEEKMESSAGEA)(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg
);


BOOL WINAPI ImmSofticePeekMessageA(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg
);


typedef SHORT (WINAPI *GETKEYSTATE)(
        int nVirtKey
);

SHORT WINAPI ImmSofticeGetKeyState(
        int nVirtKey
);