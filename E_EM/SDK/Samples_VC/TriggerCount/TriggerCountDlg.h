// TriggerCountDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\\MVImage.h"
#include "afxwin.h"

// CTriggerCountDlg dialog
class CTriggerCountDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL m_bRun;
// Construction
public:
	CTriggerCountDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TRIGGERCOUNT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OpenCamera();
	afx_msg void OnDestroy();
	int OnStreamCB( MV_IMAGE_INFO *pInfo);
	afx_msg void OnBnClickedFreeRun();
	void EnableButton(BOOL bStartTrigger = FALSE);
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedTriggerLine1();
	afx_msg void OnBnClickedTriggerSoft();
	int m_nCap;
	int m_nRec;
	CComboBox m_cmbActivation;
	afx_msg void OnCbnSelchangeComboActivation();
	void UpdateCount();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawImage();
};
