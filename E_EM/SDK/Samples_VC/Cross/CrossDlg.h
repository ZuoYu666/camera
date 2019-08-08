// CrossDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include <atlimage.h>
#include "afxcmn.h"
#include "afxwin.h"

// CCrossDlg dialog
class CCrossDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MVImage m_imageFlip;
	MVImage m_imageRotate;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL	m_bRun;
	CRITICAL_SECTION m_csInvalidate;	
// Construction
public:
	CCrossDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CROSS_DIALOG };

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
//	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sldTh;
	CStatic m_ImageWin;
	CComboBox m_cmbFlip;
	CComboBox m_cmbRotate;
	CComboBox m_cmbZoom;
	afx_msg void OnCbnSelchangeComboRotate();
	void DrawImage();
	void DrawImage(MVImage *pImg, int nZoom);
	afx_msg void OnCbnSelchangeComboZoom();
	afx_msg void OnCbnSelchangeComboFlip();
	int m_nFlip;
	int m_nRotate;
	int m_nZoom;
};

