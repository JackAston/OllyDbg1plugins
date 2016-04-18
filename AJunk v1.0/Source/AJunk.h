// AJunk.h : main header file for the AJUNK DLL
//

#if !defined(AFX_AJUNK_H__B21716E1_C21E_4785_BA1C_FADC826829EE__INCLUDED_)
#define AFX_AJUNK_H__B21716E1_C21E_4785_BA1C_FADC826829EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "Plugin.h"
/////////////////////////////////////////////////////////////////////////////
// CAJunkApp
// See AJunk.cpp for the implementation of this class
//

class CAJunkApp : public CWinApp
{
public:
	CAJunkApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAJunkApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAJunkApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AJUNK_H__B21716E1_C21E_4785_BA1C_FADC826829EE__INCLUDED_)
