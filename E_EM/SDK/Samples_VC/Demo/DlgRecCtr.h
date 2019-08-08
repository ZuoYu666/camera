#pragma once
#include "resource.h"
#include "vfw.h"

// DlgRecCtr dialog

class DlgRecCtr : public CDialog
{
	DECLARE_DYNAMIC(DlgRecCtr)

public:
	DlgRecCtr(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgRecCtr();

// Dialog Data
	enum { IDD = IDD_REC_CTR  };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileType;
	AVICOMPRESSOPTIONS m_Codec;
	CString m_strFileName;
	int		m_nRecTime;
	int		m_nRecTimeLimit;
	int		m_nRecFrame;
	int		m_nRecFrameLimit;
	int		m_nCapStep;
	BOOL	m_bCamRun;
	BOOL	m_bRecRun;

public:
	BOOL OnInitDialog();
	void SetFileType(CString strFileType);
	void SetCodec(	AVICOMPRESSOPTIONS Codec);
	void SetFileName(CString strFileName);
	void SetRecTime(int nRecTime);
	void SetRecTimeLimit(int nRecTimeLimit);
	void SetRecFrame(int nRecFrame);
	void SetRecFrameLimit(int nRecFrameLimit);
	void SetCapStep(int nCapStep);

	void IntagerToTime(int nTime, CString &strTime);
	afx_msg void OnBnClickedButtonRecordStart();
	afx_msg void OnBnClickedButtonRecordPause();
	afx_msg void OnBnClickedButtonRecordStop();
	void EnableButton();
	void EnableButton(BOOL bEnable);
	void CamRun(BOOL bRun);
	afx_msg void OnBnClickedButtonRecordConfig();
};
