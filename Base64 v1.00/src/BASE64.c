#define WIN32
#define _USRDLL
#define _CHAR_UNSIGNED
#define _WINDOWS
//#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")

#include <odbg/110.h>
#include "resource.h"

#define malloc(a) GlobalAlloc(GMEM_FIXED,a)
#define free GlobalFree

#define dllname "BASE64"
#define Verstr dllname " v1.00 by taku"

HINSTANCE        hinst;                // DLL instance
HWND             hwmain;               // Handle of main OllyDbg window

BOOL WINAPI _DllMainCRTStartup(HINSTANCE hi,DWORD reason,LPVOID reserved) {
  if (reason==DLL_PROCESS_ATTACH)
    hinst=hi;
  return 1;
}

extc int _export cdecl ODBG_Plugindata(char shortname[32]) {
  strcpy(shortname,dllname);       // Name of plugin
  return PLUGIN_VERSION;
};

extc int _export cdecl ODBG_Plugininit(int ollydbgversion,HWND hw,ulong *features) {
  if(ollydbgversion<PLUGIN_VERSION)
    return -1;
  hwmain=hw;
  InitCommonControls();
  Addtolist(0,0,dllname);
  return 0;
};

extc int _export cdecl ODBG_Pluginmenu(int origin,char data[4096],void *item) {
  switch (origin) {
  case PM_MAIN: // Plugin menu in main window
    strcpy(data,
           "1 " Verstr
          );
    return 1;
  case PM_CPUDUMP:
    if(Getstatus() == STAT_NONE) {
      return 0;
    }
    strcpy(data,"#0 BASE64 encode");
    return 1;
  default:
    break; // Any other window
  }
  return 0; // Window not supported by plugin
}

static _inline BYTE *MS_Base64Encodes(const BYTE *Base64Digits,const BYTE *cp, int len){
	int i,s;
	BYTE *ocp,*buff;
	if ((buff = (BYTE*)malloc((len*4)/3+4)) == NULL)
		return NULL;
	ocp=buff;

	s=len/3;
	for(i=0;i<s;i++){
		ocp[0] = Base64Digits[cp[0]>>2];
		ocp[1] = Base64Digits[((cp[0]<<4)|(cp[1]>>4))&0x3F];
		ocp[2] = Base64Digits[((cp[1]<<2)|(cp[2]>>6))&0x3F];
		ocp[3] = Base64Digits[cp[2]&0x3F];
		cp+=3;
		ocp+=4;
	}
	s=len%3;
	if(s){
		memset((void*)&cp[s],0,4);
		ocp[0] = Base64Digits[cp[0]>>2];
		ocp[1] = Base64Digits[((cp[0]<<4)|(cp[1]>>4))&0x3F];
		ocp[2] = Base64Digits[(cp[1]<<2)&0x3F];
		ocp[3] = Base64Digits[cp[2]&0x3F];
		     if(s==1){ ocp[2] = '=';}
		     if(s<=2){ ocp[3] = '=';}
		else if(s==2){ ocp[3] = '=';}
		ocp+=4;
	}
	ocp[0]='\r';
	ocp[1]='\n';
	ocp[2]=0;
	return buff;
}

#define FL_UNSIGNED   1       /* strtoul called */
#define FL_NEG        2       /* negative sign found */
#define FL_OVERFLOW   4       /* overflow occured */
#define FL_READDIGIT  8       /* we've read at least one correct digit */
#define ULONG_MAX     0xffffffffUL
#define LONG_MIN    (-2147483647L - 1)
#define LONG_MAX      2147483647L

static _inline unsigned long strtoxl(const char *nptr,const char **endptr,int ibase,int flags){
	const char *p;
	char c;
	unsigned long number;
	unsigned digval;
	unsigned long maxval;

	p = nptr;
	number = 0;
	c = *p++;

	while ( 0x20==((unsigned char)c) )
		c = *p++;

	if (c == '-') {
		flags |= FL_NEG;
		c = *p++;
	}
	else if (c == '+')
		c = *p++;

	if (ibase < 0 || ibase == 1 || ibase > 36) {
		if (endptr)
			*endptr = nptr;
		return 0L;
	}	else if (ibase == 0) {
		if (c != '0')
			ibase = 10;
		else if (*p == 'x' || *p == 'X')
			ibase = 16;
		else
			ibase = 8;
	}

	if (ibase == 16) {
		if (c == '0' && (*p == 'x' || *p == 'X')) {
			++p;
			c = *p++;
		}
	}
	maxval = ULONG_MAX / ibase;

	for (;;) {
		if ( ((unsigned char)c)>='0' && ((unsigned char)c)<='9' )
			digval = c - '0';
		else if ( ((unsigned char)c)>='A' && ((unsigned char)c)<='F' )
			digval = c - 'A' + 10;
		else if ( ((unsigned char)c)>='a' && ((unsigned char)c)<='f' )
			digval = c - 'a' + 10;
		else
			break;
		if (digval >= (unsigned)ibase)
			break;
		flags |= FL_READDIGIT;

		if (number < maxval || (number == maxval &&
			(unsigned long)digval <= ULONG_MAX % ibase)) {
				number = number * ibase + digval;
			}
		else {
			flags |= FL_OVERFLOW;
		}
		c = *p++;
	}
	--p;
	if (!(flags & FL_READDIGIT)) {
		if (endptr)
			p = nptr;
		number = 0L;
	}	else if ( (flags & FL_OVERFLOW) ||
		( !(flags & FL_UNSIGNED) &&
		( ( (flags & FL_NEG) && (number > -LONG_MIN) ) ||
		( !(flags & FL_NEG) && (number > LONG_MAX) ) ) ) )
	{
//		errno = ERANGE;
		if ( flags & FL_UNSIGNED )
			number = ULONG_MAX;
		else if ( flags & FL_NEG )
			number = (unsigned long)(-LONG_MIN);
		else
			number = LONG_MAX;
	}

	if (endptr != NULL)
		*endptr = p;

	if (flags & FL_NEG)
		number = (unsigned long)(-(long)number);
	return number;
}
static unsigned long strtoul (const char *nptr,char **endptr,int ibase){
	return strtoxl(nptr, endptr, ibase, FL_UNSIGNED);
}
#define GetWindowTextM(a,b,c)					SendDlgItemMessage(hwnd, a, WM_GETTEXT,(WPARAM)c,(LPARAM)b)
#define SetWindowTextM(a,b)						SendDlgItemMessage(hwnd, a, WM_SETTEXT,0,(LPARAM)b)
t_dump *pdump;
static _inline void b64conv(HWND hwnd){
	char B64D[8*10];
	char buf[2][4*4];
	DWORD adrs,s;
	t_memory *pmem;
	BYTE *buff[2];

	GetWindowTextM(IDC_EDT_TOPADDR,buf[0],sizeof(buf[0]));
	GetWindowTextM(IDC_EDT_NUMBER,buf[1],sizeof(buf[1]));
	s=GetWindowTextM(IDC_COMBO1,B64D,sizeof(B64D));
	if(s<64) return;
	adrs = strtoul(buf[0],NULL,16);
	s = strtoul(buf[1],NULL,10);
	if(adrs==0 || s==0) return;
	pmem=Findmemory(adrs);
	if(pmem==NULL) return;

	if ((buff[0] = (BYTE *)malloc(s+4)) == NULL)
		return;
  if(Readmemory(buff[0], pdump->sel0, s, MM_RESTORE | MM_SILENT) == s){
		buff[1]=MS_Base64Encodes(B64D,buff[0],s);
		if(buff[1]){
			SetWindowTextM(IDC_EDIT1,buff[1]);
			free(buff[1]);
		}
		free(buff[0]);
	}
}

const BYTE Base64Digits[5][65] ={
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/",
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/",
	"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/",
};
LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp){
  switch (msg) {
		case WM_INITDIALOG:{
      HWND htmp;
			int i;
			char buf[4*4];
			wsprintf(buf,"%08X",pdump->sel0);
			SetWindowTextM(IDC_EDT_TOPADDR,buf);
			htmp=GetDlgItem(hwnd,IDC_SPIN1);
			SendMessage(htmp,UDM_SETRANGE32,(WPARAM)1, (LPARAM)0x10000);
      SendMessage(htmp,UDM_SETPOS,    (WPARAM)0,    (LPARAM)1);
			SetDlgItemInt(hwnd,IDC_EDT_NUMBER,pdump->sel1-pdump->sel0,FALSE);
			htmp=GetDlgItem(hwnd,IDC_COMBO1);
			for(i=0;i<5;i++)
				SendMessage(htmp, CB_ADDSTRING, (WPARAM)0, (LPARAM)Base64Digits[i]);
			SendMessage(htmp, WM_SETTEXT, (WPARAM)0,(LPARAM)Base64Digits[0]);
			SendMessage(hwnd, WM_COMMAND, (WPARAM)IDC_RADIO1,(LPARAM)0);
		}return TRUE;
    case WM_COMMAND:
      switch (LOWORD(wp)) {
			case IDC_RADIO1:{
				b64conv(hwnd);
			}break;
      case IDCANCEL:
        EndDialog(hwnd, IDCANCEL);
        break;
			}
	}
  return FALSE;
}

extc void _export cdecl ODBG_Pluginaction(int origin,int action,void *item) {
  switch(origin) {
	case PM_CPUDUMP:{
    pdump=(t_dump *)item;
		if(pdump->sel0==0 || pdump->sel1==0 ||pdump->sel1-pdump->sel0==0) break;
    Addtolist(0,-1,"in PM_CPUDUMP sel0:%08X",pdump->sel0);
    DialogBoxParam(hinst,MAKEINTRESOURCE(IDD_DIALOG1),hwmain,(DLGPROC)DlgProc,(LPARAM)0);
	}break;
  default:
    break;
  }
}