// OllyTiper.h : main header file for the OLLYTIPER DLL
//

#if !defined(AFX_OLLYTIPER_H__BE023A7B_F7BB_44E7_8168_9BD342F2E60C__INCLUDED_)
#define AFX_OLLYTIPER_H__BE023A7B_F7BB_44E7_8168_9BD342F2E60C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COllyTiperApp
// See OllyTiper.cpp for the implementation of this class
//

class COllyTiperApp : public CWinApp
{
public:
	COllyTiperApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COllyTiperApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COllyTiperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OLLYTIPER_H__BE023A7B_F7BB_44E7_8168_9BD342F2E60C__INCLUDED_)
