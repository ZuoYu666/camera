// DlgRecCtr.cpp : implementation file
//

#include "stdafx.h"
#include "DlgRecCtr.h"
#include "RecordDlg.h"


// DlgRecCtr dialog

IMPLEMENT_DYNAMIC(DlgRecCtr, CDialog)

extern char *FccToName(DWORD fccHandler);

DlgRecCtr::DlgRecCtr(CWnd* pParent /*=NULL*/)
	: CDialog(DlgRecCtr::IDD, pParent)
{
	m_strFileType = "";
	m_strFileName = "temp.avi";
	memset(&m_Codec,0,sizeof(AVICOMPRESSOPTIONS));
	m_nRecTime = 0;
	m_nRecTimeLimit = 0;
	m_nRecFrame = 0;
	m_nRecFrameLimit = 0;
	m_nCapStep = 0;
	m_bCamRun = FALSE;
	m_bRecRun = FALSE;
}

DlgRecCtr::~DlgRecCtr()
{
}

void DlgRecCtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgRecCtr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_START, &DlgRecCtr::OnBnClickedButtonRecordStart)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_PAUSE, &DlgRecCtr::OnBnClickedButtonRecordPause)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_STOP, &DlgRecCtr::OnBnClickedButtonRecordStop)
	ON_BN_CLICKED(IDC_BUTTON_RECORD_CONFIG, &DlgRecCtr::OnBnClickedButtonRecordConfig)
END_MESSAGE_MAP()

void DlgRecCtr::SetFileType( CString strFileType )
{
	m_strFileType = strFileType;
	if (GetSafeHwnd()!=NULL)
	{
		GetDlgItem(IDC_STATIC_FILE_TYPE)->SetWindowText(m_strFileType);
	}
}

void DlgRecCtr::SetCodec( AVICOMPRESSOPTIONS Codec )
{
	m_Codec = Codec;
	if (GetSafeHwnd()!=NULL)
	{
		CString str = FccToName(Codec.fccHandler);
		GetDlgItem(IDC_STATIC_CODEC)->SetWindowText(str);
	}
}

void DlgRecCtr::SetRecTime( int nRecTime )
{

	m_nRecTime = nRecTime;
	if (GetSafeHwnd()!=NULL)
	{	
		CString str;
		IntagerToTime(m_nRecTime,str);
		GetDlgItem(IDC_STATIC_RECORD_TIME)->SetWindowText(str);
	}
}

void DlgRecCtr::SetRecTimeLimit( int nRecTimeLimit )
{
	m_nRecTimeLimit = nRecTimeLimit;
	if (GetSafeHwnd()!=NULL)
	{	
		CString str;
		IntagerToTime(m_nRecTimeLimit,str);
		GetDlgItem(IDC_STATIC_RECORD_TIME_LIMIT)->SetWindowText(str);
	}
}

void DlgRecCtr::SetRecFrame( int nRecFrame )
{
	m_nRecFrame = nRecFrame;
	if (GetSafeHwnd()!=NULL)
	{	
		CString str;
		str.Format("%d", m_nRecFrame);
		GetDlgItem(IDC_STATIC_FRAMES)->SetWindowText(str);
	}
}

void DlgRecCtr::SetRecFrameLimit( int nRecFrameLimit )
{
	m_nRecFrameLimit = nRecFrameLimit;
	if (GetSafeHwnd()!=NULL)
	{	
		CString str;
		str.Format("%d", m_nRecFrameLimit);
		GetDlgItem(IDC_STATIC_FRAME_LIMIT)->SetWindowText(str);
	}
}

void DlgRecCtr::SetCapStep( int nCapStep )
{
	m_nCapStep = nCapStep;
	if (GetSafeHwnd()!=NULL)
	{	
		CString str;
		str.Format("每%d张图抓一张", m_nCapStep);
		GetDlgItem(IDC_STATIC_CAPTURE_STEP)->SetWindowText(str);
	}
}

void DlgRecCtr::SetFileName( CString strFileName )
{
	m_strFileName = strFileName;
	if (GetSafeHwnd()!=NULL)
	{
		GetDlgItem(IDC_STATIC_FILE_NAME)->SetWindowText(m_strFileName);
	}
}

void DlgRecCtr::IntagerToTime( int nTime, CString &strTime )
{
	int s, m, h;
	s = nTime%60;
	m = nTime/60;
	h = m/60;
	m = m%60;
	strTime.Format("%02d:%02d:%02d",h,m,s);
}

BOOL DlgRecCtr::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetFileType(m_strFileType);
	SetCodec(m_Codec);
	SetFileName(m_strFileName);
	SetRecTime(m_nRecTime);
	SetRecTimeLimit(m_nRecTimeLimit);
	SetRecFrame(m_nRecFrame);
	SetRecFrameLimit(m_nRecFrameLimit);
	SetCapStep(m_nCapStep);

	CenterWindow();
	EnableButton();
	return TRUE;
}


// DlgRecCtr message handlers

void DlgRecCtr::OnBnClickedButtonRecordStart()
{
	// TODO: Add your control notification handler code here
	((CRecordDlg*)GetParent())->OnBnClickedRecord();
	m_bRecRun = TRUE;
	EnableButton();
}

void DlgRecCtr::OnBnClickedButtonRecordPause()
{
	// TODO: Add your control notification handler code here
	CRecordDlg * pdlg = ((CRecordDlg*)GetParent());
	pdlg->OnBnClickedRecordPause();
	GetDlgItem(IDC_BUTTON_RECORD_PAUSE)->SetWindowText(pdlg->m_bRecPause?"继续":"暂停");
}

void DlgRecCtr::OnBnClickedButtonRecordStop()
{
	// TODO: Add your control notification handler code here
	((CRecordDlg*)GetParent())->OnBnClickedRecordStop();
	m_bRecRun = FALSE;
	EnableButton();
}

void DlgRecCtr::CamRun( BOOL bRun )
{
	m_bCamRun = bRun;
	if (GetSafeHwnd()!=NULL)
	{
		EnableButton();
	}
}

void DlgRecCtr::EnableButton()
{
	GetDlgItem(IDC_BUTTON_RECORD_START)->EnableWindow(m_bCamRun&&!m_bRecRun);
	GetDlgItem(IDC_BUTTON_RECORD_PAUSE)->EnableWindow(m_bCamRun&&m_bRecRun);
	GetDlgItem(IDC_BUTTON_RECORD_STOP)->EnableWindow(m_bCamRun&&m_bRecRun);
	GetDlgItem(IDC_BUTTON_RECORD_CONFIG)->EnableWindow(!m_bRecRun);
	if (!(m_bCamRun&&m_bRecRun))
	{
		GetDlgItem(IDC_BUTTON_RECORD_PAUSE)->SetWindowText("暂停");
	}
}

void DlgRecCtr::EnableButton( BOOL bEnable )
{
	m_bRecRun = FALSE;
	EnableButton();
}

void DlgRecCtr::OnBnClickedButtonRecordConfig()
{
	// TODO: Add your control notification handler code here
	((CRecordDlg*)GetParent())->OnBnClickedRecordConf();
}