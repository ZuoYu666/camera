// GCapDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include "afxwin.h"
#include "afxcmn.h"

// CGCapDlg dialog
class CGCapDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL	m_bLive;
	CComboBox m_cmbZoom;
	int		m_nZoom;
	int m_nTh;
// Construction
public:
	CGCapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GCAP_DIALOG };

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
	afx_msg void OnBnClickedOpen();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStart();
	int OnStreamCB( MV_IMAGE_INFO *pInfo);
	afx_msg void OnBnClickedSave();
	afx_msg void OnCbnSelchangeCmbZoom();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sldTh;
	CSliderCtrl m_sldGain;
	CSliderCtrl m_sldExp;
	BOOL m_bThreshold;
	afx_msg void OnBnClickedProcessThreshold();
};
