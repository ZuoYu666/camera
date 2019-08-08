/*****************************************************************************
  DLGBARS.CPP

  Purpose: 
  	Interface for CDlgToolBar, a special type of CToolBar which does not
  	expect a parent frame window to be available, and CDlgStatusBar, which
  	does the same for CStatusBars.  This allows the control bars
  	to be used in applications where the main window is a dialog bar.

  Functions:
    CDlgToolBar::CDlgToolBar()          -- constructor
    CDlgToolBar::~CDlgToolBar()         -- destructor
    CDlgToolBar::OnIdleUpdateCmdUI()    -- WM_IDLEUPDATECMDUI handler
    
    CDlgStatusBar::CDlgStatusBar()      -- constructor
    CDlgStatusBar::~CDlgStatusBar()     -- destructor
    CDlgStatusBar::OnIdleUpdateCmdUI()	-- WM_IDLEUPDATECMDUI handler

  Development Team:
	Mary Kirtland
  Ported to 32-bit by:
    Mike Hedley	
  Written by Microsoft Product Support Services, Premier ISV Support
  Copyright (c) 1996 Microsoft Corporation. All rights reserved.
\****************************************************************************/

#include "stdafx.h" 
#include <afxpriv.h>
#include "dlgbars.h"

#ifdef _DEBUG
	#undef THIS_FILE
	static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar

BEGIN_MESSAGE_MAP(CDlgToolBar, CToolBar)
	//{{AFX_MSG_MAP(CDlgToolBar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
                 
/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar Construction/Destruction

CDlgToolBar::CDlgToolBar()
{
}

CDlgToolBar::~CDlgToolBar()
{
} 

/////////////////////////////////////////////////////////////////////////////
// CDlgToolBar::OnIdleUpdateCmdUI
//		OnIdleUpdateCmdUI handles the WM_IDLEUPDATECMDUI message, which is 
//      used to update the status of user-interface elements within the MFC 
//		framework.
//
// 		We have to get a little tricky here: CToolBar::OnUpdateCmdUI 
//      expects a CFrameWnd pointer as its first parameter.  However, it
//      doesn't do anything but pass the parameter on to another function
//      which only requires a CCmdTarget pointer.  We can get a CWnd pointer
//      to the parent window, which is a CCmdTarget, but may not be a 
//   	CFrameWnd.  So, to make CToolBar::OnUpdateCmdUI happy, we will call
//      our CWnd pointer a CFrameWnd pointer temporarily.  	

LRESULT CDlgToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) 
{
	if (IsWindowVisible()) 
	{
		CFrameWnd *pParent = (CFrameWnd *)GetParent();
		if (pParent)
			OnUpdateCmdUI(pParent, (BOOL)wParam);
	}
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusBar

BEGIN_MESSAGE_MAP(CDlgStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CDlgStatusBar)
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
                 
/////////////////////////////////////////////////////////////////////////////
// CDlgStatusBar Construction/Destruction

CDlgStatusBar::CDlgStatusBar()
{
}

CDlgStatusBar::~CDlgStatusBar()
{
} 

/////////////////////////////////////////////////////////////////////////////
// CDlgStatusBar::OnIdleUpdateCmdUI
//		OnIdleUpdateCmdUI handles the WM_IDLEUPDATECMDUI message, which is 
//      used to update the status of user-interface elements within the MFC 
//		framework.
//
// 		We have to get a little tricky here: CStatusBar::OnUpdateCmdUI 
//      expects a CFrameWnd pointer as its first parameter.  However, it
//      doesn't do anything but pass the parameter on to another function
//      which only requires a CCmdTarget pointer.  We can get a CWnd pointer
//      to the parent window, which is a CCmdTarget, but may not be a 
//   	CFrameWnd.  So, to make CStatusBar::OnUpdateCmdUI happy, we will call
//      our CWnd pointer a CFrameWnd pointer temporarily.  	

LRESULT CDlgStatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) 
{
	if (IsWindowVisible()) 
	{
		CFrameWnd *pParent = (CFrameWnd *)GetParent();
		if (pParent)
			OnUpdateCmdUI(pParent, (BOOL)wParam);
	}
	return 0L;
}

