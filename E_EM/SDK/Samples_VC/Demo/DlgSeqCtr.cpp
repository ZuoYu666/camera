// DlgSeqCtr.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSeqCtr.h"
#include "GCapDlg.h"


// DlgSeqCtr dialog

IMPLEMENT_DYNAMIC(DlgSeqCtr, CDialog)

DlgSeqCtr::DlgSeqCtr(CWnd* pParent /*=NULL*/)
	: CDialog(DlgSeqCtr::IDD, pParent)
{
	m_strFileName = "";
	m_nTimeSpace = 0;
	m_nNextImageDelay = 0;
	m_bCamRun = FALSE;
	m_bSequenceRun = FALSE;
}

DlgSeqCtr::~DlgSeqCtr()
{
}

void DlgSeqCtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSeqCtr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENCE_START, &DlgSeqCtr::OnBnClickedButtonSequenceStart)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENCE_STOP, &DlgSeqCtr::OnBnClickedButtonSequenceStop)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENCE_CONFIG, &DlgSeqCtr::OnBnClickedButtonSequenceConfig)
END_MESSAGE_MAP()

void DlgSeqCtr::SetFileName( CString str )
{
	m_strFileName = str;
	if (GetSafeHwnd()!=NULL)
	{
		GetDlgItem(IDC_STATIC_NEXT_IMAGE_PATH)->SetWindowText(m_strFileName);
	}
}

void DlgSeqCtr::SetTimeSpace( int nSpace )
{
	m_nTimeSpace = nSpace;
	if (GetSafeHwnd()!=NULL)
	{
		CString str;
		str.Format(_T("%d"),m_nTimeSpace);
		GetDlgItem(IDC_STATIC_TIME_SPACE)->SetWindowText(str);
	}
}

void DlgSeqCtr::SetNextImageDelay( int nDelay )
{
	m_nNextImageDelay = nDelay;
	if (GetSafeHwnd()!=NULL)
	{
		CString str;
		str.Format(_T("%d"),m_nNextImageDelay);
		GetDlgItem(IDC_STATIC_NEXT_IMAGE_DELAY)->SetWindowText(str);
	}
}

BOOL DlgSeqCtr::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetTimeSpace(m_nTimeSpace);
	SetFileName(m_strFileName);
	SetNextImageDelay(m_nNextImageDelay);
	EnableButton();

	CenterWindow();
	return TRUE;
}


// DlgSeqCtr message handlers

void DlgSeqCtr::OnBnClickedButtonSequenceStart()
{
	// TODO: Add your control notification handler code here
	m_bSequenceRun = TRUE;
	((CGCapDlg*)GetParent())->OnBnClickedSequenceStart();
	EnableButton();
}

void DlgSeqCtr::OnBnClickedButtonSequenceStop()
{
	// TODO: Add your control notification handler code here
	m_bSequenceRun = FALSE;
	((CGCapDlg*)GetParent())->OnBnClickedSequenceStop();
	EnableButton();
}

void DlgSeqCtr::OnBnClickedButtonSequenceConfig()
{
	// TODO: Add your control notification handler code here
	((CGCapDlg*)GetParent())->OnBnClickedSequence();
}

void DlgSeqCtr::CamRun( BOOL bRun )
{
	m_bCamRun = bRun;
	EnableButton();
}

void DlgSeqCtr::EnableButton()
{
	if (GetSafeHwnd()!=NULL)
	{
		GetDlgItem(IDC_BUTTON_SEQUENCE_START)->EnableWindow(m_bCamRun&&!m_bSequenceRun);
		GetDlgItem(IDC_BUTTON_SEQUENCE_STOP)->EnableWindow(m_bCamRun&&m_bSequenceRun);
		GetDlgItem(IDC_BUTTON_SEQUENCE_CONFIG)->EnableWindow(!m_bSequenceRun);
	}
}
