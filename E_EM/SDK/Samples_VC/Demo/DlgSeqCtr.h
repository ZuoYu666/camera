#pragma once
#include "resource.h"

// DlgSeqCtr dialog

class DlgSeqCtr : public CDialog
{
	DECLARE_DYNAMIC(DlgSeqCtr)

public:
	DlgSeqCtr(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgSeqCtr();

// Dialog Data
	enum { IDD = IDD_SEQ_CTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strFileName;
	int		m_nTimeSpace;
	int		m_nNextImageDelay;
	BOOL	m_bCamRun;
	BOOL	m_bSequenceRun;
	void	SetFileName(CString str);
	void	SetTimeSpace(int nSpace);
	void	SetNextImageDelay(int nDelay);
	afx_msg void OnBnClickedButtonSequenceStart();
	afx_msg void OnBnClickedButtonSequenceStop();
	afx_msg void OnBnClickedButtonSequenceConfig();
	void	CamRun(BOOL bRun);
	void	EnableButton();
};
