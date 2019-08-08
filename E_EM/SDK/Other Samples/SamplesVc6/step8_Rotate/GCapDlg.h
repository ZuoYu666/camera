// GCapDlg.h : header file
//

#if !defined(AFX_GCAPDLG_H__0F82A3E3_33E3_4E18_A748_9A25B3C9F418__INCLUDED_)
#define AFX_GCAPDLG_H__0F82A3E3_33E3_4E18_A748_9A25B3C9F418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGCapDlg dialog

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
class CGCapDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MVImage m_imageRotate;
	MV_PixelFormatEnums m_PixelFormat;
	bool m_bLive;
	int	m_nRoate;
	int m_nZoom;
	// Construction
public:
	CGCapDlg(CWnd* pParent = NULL);	// standard constructor
	int OnStreamCB( MV_IMAGE_INFO *pInfo);

// Dialog Data
	//{{AFX_DATA(CGCapDlg)
	enum { IDD = IDD_GCAP_DIALOG };
	CComboBox	m_cmbRotate;
	CComboBox	m_cmbZoom;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGCapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGCapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnStart();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeZoom();
	afx_msg void OnSave();
	afx_msg void OnSelchangeRotate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GCAPDLG_H__0F82A3E3_33E3_4E18_A748_9A25B3C9F418__INCLUDED_)
