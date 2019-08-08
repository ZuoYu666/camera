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
	 *  \brief 获取保存图像的下一个文件名
	 *  \param [out]	fname	文件名缓冲区
	 *  \param [in]		szBuf	文件名缓冲区的大小
	 *  \retval  	0:需要保存，并返回文件名。-1:此帧不需保存。-2:文件名缓冲区的大小错误。
	 */
	int		GetFileName(char *fname,int szBuf);
};
