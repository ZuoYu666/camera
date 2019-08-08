// RecordDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\\MVImage.h"
#include "..\..\include\\MVAvi.h"
#include "DlgRecConf.h"
#include "DlgRecCtr.h"

// CRecordDlg dialog
class CRecordDlg : public CDialog
{
private:
	HANDLE m_hCam;
	MVImage m_image;
	MVImage m_imageRec;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL m_bSaving;
	BOOL m_bRun;
	CRecConf m_RecConf;

	DlgRecCtr m_RecCtr;	

	MVAvi m_avi;
	time_t m_tRecStart;
	int	m_nRecCnt;
	time_t m_tRecElapse;
	BOOL m_bFirstRec;
	double	m_dFrequency;
	LARGE_INTEGER	liFrequency;
	LARGE_INTEGER	m_liRecStart, m_liRecStop; 

	CRITICAL_SECTION m_csRecord;
	HANDLE m_evFrame;
	HANDLE m_evExit;
	HANDLE m_hRecProc;

// Construction
public:
	CRecordDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RECORD_DIALOG };
	BOOL m_bRecPause;
	BOOL m_bRec;

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
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordPause();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedRecordConf();
	afx_msg void OnBnClickedRecCtr();
	void UpdateRecCtr();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void EnableButton();
	int AppendFrame();
	void DrawImage();
	void RecordProc();
};
