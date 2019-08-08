#pragma once
#include "Resource.h"
#include "vfw.h"
#include "afxcmn.h"

// CDlgRecConf dialog
typedef struct 
{
	BOOL m_bAutoFileName;

	BOOL m_bIndex;
	int m_nIndex;

	int m_nTimeStyle;

	CString m_strPath;
	CString m_strPrefixName;

	BOOL m_bAutoStopByTime;
	BOOL m_bAutoStopByFrame;
	BOOL m_bAutoStopByFileSize;
	int m_nAutoStopTime;
	int m_nAutoStopFrame;
	int m_nAutoStopFileSize;
	int m_nInterval;

	AVICOMPRESSOPTIONS m_CompressOpt;
}CRecConf;

void InitRecConf(CRecConf &recConf);
BOOL InitRecConfByFile(CRecConf &recConf, CString strIniFilePath = "");

class CDlgRecConf : public CDialog
{
private:
	CRecConf m_conf;
	AVICOMPRESSOPTIONS m_CmpressOpt;

	DECLARE_DYNAMIC(CDlgRecConf)

public:
	CDlgRecConf(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgRecConf();

	void SetData(CRecConf conf);
	void GetData(CRecConf &conf);

// Dialog Data
	enum { IDD = IDD_REC_CONF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonChangePath();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonChangeCodec();
	CSpinButtonCtrl m_spinRecInterval;
	virtual BOOL OnInitDialog();
	BOOL SaveRecConf(CString srtPath = "");
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
};
