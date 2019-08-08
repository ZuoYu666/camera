// SequenceDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include "..\..\include\MVCamProptySheet.h"
#include "SeqCtrlDlg.h"

// CSequenceDlg dialog
class CSequenceDlg : public CDialog
{
private:
	HANDLE m_hCam;
	HANDLE m_hPropDlg;
	MVImage m_image;
	BOOL m_bRun;

	CSeqCtrlDlg m_dlgSeqCtrl;	
// Construction
public:
	CSequenceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SEQUENCE_DIALOG };

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
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedFreeRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedProperty();
	afx_msg void OnBnClickedSequenceSave();

	void DrawImage();
	void EnableButton();
	int OnStreamCB( MV_IMAGE_INFO *pInfo);
	void OpenCamera();
};
