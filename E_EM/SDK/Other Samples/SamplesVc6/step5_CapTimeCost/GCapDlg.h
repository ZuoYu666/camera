// GCapDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include "StopWatch.h"
#include "afxcmn.h"

// CGCapDlg dialog
class CGCapDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL m_bRun;
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
	afx_msg void OnBnClickedTrigger();
	afx_msg void OnBnClickedTrigContinue();
	afx_msg void OnEnKillfocusExposure();

	UINT m_nFPS;		//! 帧率
	BOOL m_bTrigCont;	//! 连续触发模式
	int m_nTimeCostMax;	//! 从发出触发指令到得到图像所花费时间的最大值
	double m_fExpo;		//! 曝光时间
	BOOL PreTranslateMessage(MSG* pMsg);
	int m_nWidth;
	int m_nHeight;
	afx_msg void OnEnKillfocusWidth();
	afx_msg void OnEnKillfocusHeight();
	int UpdateFps();
	int UpdateImage();
};
