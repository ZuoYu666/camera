// ThresholdDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include "afxwin.h"
#include "afxcmn.h"


// CThresholdDlg dialog
class CThresholdDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MVImage m_imageDst;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL	m_bRun;
	int		m_nTh;
// Construction
public:
	CThresholdDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THRESHOLD_DIALOG };

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
	afx_msg void OnBnClickedStart();
	int OnStreamCB( MV_IMAGE_INFO *pInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sldTh;
	BOOL m_bBinary;
	afx_msg void OnBnClickedCheckBinary();
	BOOL m_bInvert;
	afx_msg void OnBnClickedCheckInvert();
	void DrawImage();
	void ProcessImage();
	afx_msg void OnCbnSelchangeComboDepth();
	CComboBox m_cmbDepth;
};