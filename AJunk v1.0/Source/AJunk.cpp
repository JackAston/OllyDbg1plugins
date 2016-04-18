// AJunk.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "AJunk.h"
#include <AFXTEMPL.H>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND		g_hwMain;

typedef struct tagJUNKRECORD
{
	ulong	lAddr;
	ulong	lNopedBytes;
	uchar	buf[16];
}JUNKRECORD;

CArray< JUNKRECORD *, JUNKRECORD * > g_aryRecord;

//-----------------------------------------------------------------
_dllexport int _ODBG_Plugininit(int ollydbgversion,HWND hw,unsigned long *features)
{
	if (ollydbgversion<PLUGIN_VERSION)
		return -1;
	g_hwMain = hw;
	return 0;
}

//-----------------------------------------------------------------
_dllexport int ODBG_Plugindata(char *shortname)
{
	strcpy(shortname,"AJunk");       // Name of plugin
	return PLUGIN_VERSION;
}

//-----------------------------------------------------------------
_dllexport int _ODBG_Pluginmenu(int origin,char data[4096],void *item)
{
	t_dump * pd;
	CString s;

	switch (origin)
	{
    case PM_MAIN:                      // Plugin menu in main window
		strcpy(data,"0 &About");
		// If your plugin is more than trivial, I also recommend to include Help.
		break;
    case PM_DISASM:                    // Popup menu in Disassembler
		pd=static_cast<t_dump *>(item);
		if (pd==NULL || pd->size==0)
			return 0;                      // Window empty, don't add
		
		s = "AJunk{1 nop 1 bytes,2 nop 2 bytes,3 nop 3 bytes,4 nop 4 bytes,"
			"5 nop 5 bytes,6 nop 6 bytes,7 auto";
		if(g_aryRecord.GetSize())
			s += ",9 undo, 0 undo all";
		s += "}";

		strcpy(data, s);

		break;
	default:
		break;
	}
	return 1;
}

//-----------------------------------------------------------------
void	RemoveJunk(ulong lAddr, int nSize)
{
	if(nSize<0 || nSize>16)
		return;

	JUNKRECORD * pj = new JUNKRECORD;
	_Readmemory(pj->buf, lAddr, nSize, MM_SILENT);
	pj->lAddr = lAddr;
	pj->lNopedBytes = nSize;
	g_aryRecord.InsertAt(0, pj);

	char buf[16];
	for(int i=0; i<16; ++i)
		buf[i] = 0x90;

	_Writememory(buf, lAddr, nSize, MM_SILENT);

}

//-----------------------------------------------------------------
void	RestoreJunk()
{
	if(g_aryRecord.GetSize()==0)
		return;

	JUNKRECORD * pj = g_aryRecord.GetAt(g_aryRecord.GetSize()-1);
	_Writememory(pj->buf, pj->lAddr, pj->lNopedBytes, MM_SILENT);
	g_aryRecord.RemoveAt(0);
	delete pj;
}

//-----------------------------------------------------------------
void	FindNops(t_dump * pt)
{
	ulong lIp = pt->sel0;
	uchar buf[256];
	_Readmemory(buf, lIp, 256, MM_SILENT);

	t_disasm ta;
	ulong lSize = _Disasm(buf, 16, lIp, NULL, &ta, DISASM_ALL, NULL);

	if(ta.error!=0)
		return;

	if(ta.cmdtype!=C_JMP && ta.cmdtype!=C_JMC)
	{
		MessageBox(g_hwMain,"Not a JMP instruction.\r\nCan't finish JNNK!", "Warning", MB_OK);
		return;
	}

	if(ta.jmpconst==0)
	{
		MessageBox(g_hwMain, "JMP destination addr is 0.\r\nMaybe Wrong!","Warning", MB_OK);
		return;
	}

	if(ta.jmpconst-lIp>240)
	{
		MessageBox(g_hwMain, "JMP desination addr is too Far from here.\r\n"
			"This opration canceled.", "Waring", MB_OK);
		return;
	}

	if(ta.jmpconst < lIp)
	{
		MessageBox(g_hwMain, "JMP backward, Maybe not a junk instruction.\r\n"
			"If you sure, try manual deal with it.", "Tip", MB_OK);
		return;
	}
	// ok, we sure there are some junk instructions, do it....
	ulong	lDesAddr = ta.jmpconst;
	ulong	lPrevAddr;
	ulong lPos = 0;

	while(lIp<lDesAddr)
	{	
		lPrevAddr = lIp;
		lSize =  _Disasm(buf+lPos, 16, lIp, NULL, &ta, DISASM_SIZE, NULL);
		lPos += lSize;
		lIp += lSize;
	}
	if(lIp == lDesAddr)
	{
		MessageBox(g_hwMain, "It isn't a Junk instruction.", "Tip", MB_OK);
		return;
	}

	RemoveJunk(lPrevAddr, static_cast<int>(lDesAddr-lPrevAddr));
}
//-----------------------------------------------------------------
void	ProcessMenu(t_dump * pt, int nCmd)
{
	if (nCmd==7)
		FindNops(pt);
	else if(nCmd>0 && nCmd<=6)
		RemoveJunk(pt->sel0, nCmd);
	else if(nCmd == 9)
		RestoreJunk();
	else if(nCmd == 0)
	{
		while(g_aryRecord.GetSize())
			RestoreJunk();
	}
	_Redrawdisassembler();
}
//-----------------------------------------------------------------
_dllexport void _ODBG_Pluginaction(int origin,int action,void *item)
{
	if(origin==PM_MAIN && action==0){
		MessageBox(g_hwMain, "Function: to remove/restore junk instructions.\r\n"
							 "                  Ver 1.00\r\n"
							 "                  by lenglianxi", 
			"About this plugins",
			MB_OK);
	}
	else if(origin==PM_DISASM)
		ProcessMenu(static_cast<t_dump*>(item), action);
}

//-----------------------------------------------------------------
_dllexport int _ODBG_Pluginshortcut(int origin,int ctrl,int alt,int shift,int key,void *item)
{
	if(origin == PM_DISASM && ( (key>='1' && key<='7') || key=='0' || key=='9') )
	{
		ProcessMenu(static_cast<t_dump*>(item), key- '0');
		return 1;
	}
	else
		return 0;
}
//-----------------------------------------------------------------
void FreeMen()
{
	while(g_aryRecord.GetSize())
	{
		JUNKRECORD * pj = g_aryRecord[0];
		delete pj;
		g_aryRecord.RemoveAt(0);
	}

}

_dllexport void _ODBG_Pluginreset(void)
{
	FreeMen();
}

_dllexport int _ODBG_Pluginclose(void)
{
	FreeMen();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CAJunkApp

BEGIN_MESSAGE_MAP(CAJunkApp, CWinApp)
	//{{AFX_MSG_MAP(CAJunkApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAJunkApp construction

//-----------------------------------------------------------------
CAJunkApp::CAJunkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAJunkApp object

CAJunkApp theApp;
