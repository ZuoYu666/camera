// GCap.h : main header file for the GCAP application
//

#if !defined(AFX_GCAP_H__4C0D630A_C5E6_4B44_9F8A_F01D1CB4DB96__INCLUDED_)
#define AFX_GCAP_H__4C0D630A_C5E6_4B44_9F8A_F01D1CB4DB96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGCapApp:
// See GCap.cpp for the implementation of this class
//

class CGCapApp : public CWinApp
{
public:
	CGCapApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCapApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGCapApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCAP_H__4C0D630A_C5E6_4B44_9F8A_F01D1CB4DB96__INCLUDED_)
