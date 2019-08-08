#pragma once
#include "afxcmn.h"


// CDlgSelectCamera dialog

class CDlgSelectCamera : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelectCamera)

public:
	CDlgSelectCamera(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSelectCamera();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SELECT_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstCameras;
	int m_nDevID;

	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOk();
};
