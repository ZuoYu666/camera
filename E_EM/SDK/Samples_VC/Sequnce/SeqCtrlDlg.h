#pragma once
#include "resource.h"

#include "SeqConfDlg.h"
// CSeqCtrlDlg dialog

class CSeqCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CSeqCtrlDlg)
private:
	int m_nCountDown;
	int	m_nSnapInterval;	
	BOOL m_bSave;
	BOOL m_bSaveAll;
	CRITICAL_SECTION m_csSave;	
	CSeqParam m_SeqConf;

	int m_nIndex;
	BOOL	m_bCamRun;
	BOOL	m_bSequenceRun;

	void	EnableButton();
	void	UpdateCountDown();
public:
	CSeqCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSeqCtrlDlg();

// Dialog Data
	enum { IDD = IDD_SEQ_CTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSequenceStart();
	afx_msg void OnBnClickedManual();
	afx_msg void OnBnClickedButtonSequenceConfig();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	
	void	CamRun(BOOL bRun);

	/*!
	 *  \brief ��ȡ����ͼ�����һ���ļ���
	 *  \param [out]	fname	�ļ���������
	 *  \param [in]		szBuf	�ļ����������Ĵ�С
	 *  \retval  	0:��Ҫ���棬�������ļ�����-1:��֡���豣�档-2:�ļ����������Ĵ�С����
	 */
	int		GetFileName(char *fname,int szBuf);
};
