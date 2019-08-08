#pragma once
#include "Resource.h"
//#include "afxwin.h"

// CSeqConfDlg dialog

typedef struct _S_CSeqParam
{
	BOOL m_bIndex;
	int m_nIndex;
	int m_nTimeStyle;
	int m_nMinute;
	int m_nSecond;

	CString m_strPath;
	CString m_strFileType;
	CString m_strPrefixName;
}CSeqParam;

void InitSeqConf(CSeqParam &Param);
BOOL InitByFile(CSeqParam &param, CString strPath = "");


class CSeqConfDlg : public CDialog
{
	DECLARE_DYNAMIC(CSeqConfDlg)

public:
	CSeqConfDlg(CWnd* pParent = NULL);   // standard constructor
	CSeqConfDlg(CSeqParam param, CWnd* pParent = NULL);
	CSeqConfDlg(CString srtPath, CWnd* pParent = NULL);
	virtual ~CSeqConfDlg();

	// Dialog Data
	enum
	{
		IDD = IDD_SEQ_CONF
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	void SetBackUp();
	void GetBackUp();
public:
	afx_msg void OnBnClickedButtonChangePath();
	CSeqParam m_Param;

public:
	afx_msg void OnBnClickedCheckIndex();
	afx_msg void OnCbnSelchangeComboFileType();

	afx_msg void OnEnKillfocusEditFilenamePrefix();
	afx_msg void OnEnKillfocusEditIndex();
	afx_msg void OnEnKillfocusEditMinute();
	afx_msg void OnEnKillfocusEditSecond();

	BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboTimeType();
	void GetParam(CSeqParam &param);
	BOOL SaveParam(CString srtPath = _T(""));
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
};
