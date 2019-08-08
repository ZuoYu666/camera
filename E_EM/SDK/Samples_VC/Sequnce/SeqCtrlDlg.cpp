// CSeqCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeqCtrlDlg.h"
#include "SeqConfDlg.h"
#include "IniValue.h"

// CSeqCtrlDlg dialog

IMPLEMENT_DYNAMIC(CSeqCtrlDlg, CDialog)

#define TIMER_COUNT_DOWN 1002

CSeqCtrlDlg::CSeqCtrlDlg(CWnd * pParent /*=NULL*/) 
: CDialog(CSeqCtrlDlg::IDD, pParent)
, m_bSave(FALSE)
, m_nIndex(0)
, m_nSnapInterval(5)
, m_bSaveAll(FALSE)
{
	m_nCountDown = 0;
	m_bCamRun = FALSE;
	m_bSequenceRun = FALSE;

	if (!InitByFile(FILE_NAME_SEQ, m_SeqConf))
	{
		InitSeqConf(m_SeqConf);
		TCHAR Buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(Buffer),Buffer);
		m_SeqConf.m_strPath=Buffer;
		m_SeqConf.m_strPath += "\\";
	}
	


	InitializeCriticalSection(&m_csSave);
}

CSeqCtrlDlg::~CSeqCtrlDlg()
{
}

void CSeqCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSeqCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENCE_START, &CSeqCtrlDlg::OnBnClickedButtonSequenceStart)
	ON_BN_CLICKED(IDC_MANUAL, &CSeqCtrlDlg::OnBnClickedManual)
	ON_BN_CLICKED(IDC_BUTTON_SEQUENCE_CONFIG, &CSeqCtrlDlg::OnBnClickedButtonSequenceConfig)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CSeqCtrlDlg::UpdateCountDown()
{
	if (GetSafeHwnd() != NULL)
	{
		CString str;
		str.Format(_T("%d"), m_nCountDown);
		GetDlgItem(IDC_STATIC_NEXT_IMAGE_DELAY)->SetWindowText(str);
	}
}

BOOL CSeqCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateCountDown();
	EnableButton();

	CString str;
	str.Format(_T("%d"), m_nSnapInterval);
	GetDlgItem(IDC_STATIC_TIME_SPACE)->SetWindowText(str);

	CenterWindow();
	return TRUE;
}


// CSeqCtrlDlg message handlers

void CSeqCtrlDlg::OnBnClickedButtonSequenceStart()
{
	if( !m_bSequenceRun )
	{
		m_bSequenceRun = TRUE;

		if( m_nSnapInterval != 0 )
		{
			m_nCountDown = m_nSnapInterval;
			SetTimer(TIMER_COUNT_DOWN,1000,NULL);
		}
		GetDlgItem(IDC_BUTTON_SEQUENCE_START)->SetWindowText("停止");
	}
	else
	{
		m_bSequenceRun = FALSE;
		KillTimer(TIMER_COUNT_DOWN);
		GetDlgItem(IDC_BUTTON_SEQUENCE_START)->SetWindowText("开始");
	}

	EnableButton();
}

void CSeqCtrlDlg::OnBnClickedManual()
{
	EnterCriticalSection(&m_csSave);
	m_bSave = TRUE;
	LeaveCriticalSection(&m_csSave);
}

void CSeqCtrlDlg::OnBnClickedButtonSequenceConfig()
{
	CSeqConfDlg dlg(m_SeqConf);

	if( dlg.DoModal() == IDOK )
	{
		dlg.GetParam(m_SeqConf);

		m_nSnapInterval = m_SeqConf.m_nMinute*60+m_SeqConf.m_nSecond;

		CString str;
		str.Format(_T("%d"), m_nSnapInterval);
		GetDlgItem(IDC_STATIC_TIME_SPACE)->SetWindowText(str);

		m_nIndex = m_SeqConf.m_nIndex;

		if( m_nSnapInterval == 0 )
		{
			if( MessageBox("时间间隔设置为0,是否保存每一帧图像?","确认",MB_YESNO) == IDYES )
			{
				m_bSaveAll = TRUE;
			}			
		}
		else
		{
			m_bSaveAll = FALSE;
		}
	}
}

void CSeqCtrlDlg::CamRun(BOOL bRun)
{
	if( !bRun && m_bSequenceRun )
	{
		OnBnClickedButtonSequenceStart();
	}

	m_bCamRun = bRun;

	EnableButton();
}

void CSeqCtrlDlg::EnableButton()
{
	if (GetSafeHwnd() != NULL)
	{
		GetDlgItem(IDC_BUTTON_SEQUENCE_START)->EnableWindow(m_bCamRun);
		GetDlgItem(IDC_MANUAL)->EnableWindow(m_bCamRun && !m_bSaveAll);
		GetDlgItem(IDC_BUTTON_SEQUENCE_CONFIG)->EnableWindow(!m_bSequenceRun);
	}
}

void CSeqCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_COUNT_DOWN)
	{
		m_nCountDown --;
		
		UpdateCountDown();

		if( m_nCountDown == 0 )
		{
			EnterCriticalSection(&m_csSave);
			m_bSave = TRUE;
			LeaveCriticalSection(&m_csSave);

			m_nCountDown = m_nSnapInterval;
		}		
	}

	CDialog::OnTimer(nIDEvent);
}

int CSeqCtrlDlg::GetFileName(char *fname,int szBuf)
{	
	int hr = 0;
	EnterCriticalSection(&m_csSave);

	if( m_bSave || m_bSaveAll )
	{
		CString str;
		CString strFinal = m_SeqConf.m_strPath;

		if( strFinal.ReverseFind('\\') != (strFinal.GetLength()-1) )
		{
			strFinal = strFinal + "\\";
		}
		if (!m_SeqConf.m_strPrefixName.IsEmpty())
		{
			strFinal += m_SeqConf.m_strPrefixName;
			strFinal += _T("_");
		}
		
		CTime theCurrentTime = CTime::GetCurrentTime(); 
		switch (m_SeqConf.m_nTimeStyle)
		{
		case 0:
			str = theCurrentTime.Format("%H-%M-%S_");
			strFinal += str;
			break;
		case 1:
			str = theCurrentTime.Format("%Y-%m-%d-%H-%M-%S_");
			strFinal += str;
			break;
		case 2:
			break;
		default:
			break;
		}

		if (m_SeqConf.m_bIndex)
		{
			str.Format(_T("%04d"), m_nIndex);
			strFinal += str;
			m_nIndex += 1;
		}

		strFinal += m_SeqConf.m_strFileType;

		if( szBuf >= (strFinal.GetLength()+1) )
		{			
			strcpy(fname,strFinal.LockBuffer());
			strFinal.Replace("\\", "\\ ");
			if (GetSafeHwnd() != NULL)
			{
				GetDlgItem(IDC_STATIC_NEXT_IMAGE_PATH)->SetWindowText(strFinal);
			}
		}
		else
		{
			hr = -2;
		}

		m_bSave = FALSE;
	}
	else
	{
		hr = -1;
	}
	LeaveCriticalSection(&m_csSave);

	return hr;
}

