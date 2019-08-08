// TriggerCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TriggerCount.h"
#include "TriggerCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define UPDATE_COUNT 100
// CTriggerCountDlg dialog

CTriggerCountDlg::CTriggerCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerCountDlg::IDD, pParent)
	, m_hCam(NULL)
	, m_bRun(FALSE)
	, m_nCap(0)
	, m_nRec(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTriggerCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CAPTURE_COUNT, m_nCap);
	DDX_Text(pDX, IDC_RECEIVE_COUNT, m_nRec);
	DDX_Control(pDX, IDC_COMBO_ACTIVATION, m_cmbActivation);
}

BEGIN_MESSAGE_MAP(CTriggerCountDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FREE_RUN, &CTriggerCountDlg::OnBnClickedFreeRun)
	ON_BN_CLICKED(IDC_STOP, &CTriggerCountDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_TRIGGER_LINE1, &CTriggerCountDlg::OnBnClickedTriggerLine1)
	ON_BN_CLICKED(IDC_TRIGGER_SOFT, &CTriggerCountDlg::OnBnClickedTriggerSoft)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTIVATION, &CTriggerCountDlg::OnCbnSelchangeComboActivation)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTriggerCountDlg message handlers

BOOL CTriggerCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	MVInitLib();
	OpenCamera();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTriggerCountDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
		if (!m_image.IsNull())
		{
			DrawImage();
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTriggerCountDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTriggerCountDlg::OpenCamera()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if( nCams == 0 )
	{
		MessageBox("没有找到相机,请确认连接和相机IP设置","提示",MB_ICONWARNING);
		return ;
	}

	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox("无法打开相机，可能正被别的软件控制", "提示", MB_ICONWARNING);
		else
			MessageBox("无法打开相机","提示",MB_ICONWARNING); 
		return;
	}

	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);
	m_image.CreateByPixelFormat(w,h,m_PixelFormat);

	MVSetTriggerMode(m_hCam, TriggerMode_On);
	MVSetTriggerSource(m_hCam, TriggerSource_Software);

	TriggerActivationEnums act;
	MVGetTriggerActivation(m_hCam, &act);
	switch(act)
	{
	case TriggerActivation_RisingEdge:
		m_cmbActivation.SetCurSel(0);
		break;
	case TriggerActivation_FallingEdge:
		m_cmbActivation.SetCurSel(1);
		break;
	default:
		m_cmbActivation.SetCurSel(-1);
		AfxMessageBox(_T("获得触发极性失败"));
		break;
	}
	m_cmbActivation.EnableWindow(TRUE);
	EnableButton();
}

void CTriggerCountDlg::OnDestroy()
{
	CDialog::OnDestroy();

	MVSTATUS_CODES r;
	OnBnClickedStop();
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		r = MVSetTriggerMode(m_hCam, TriggerMode_Off);
	}

	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();
}


int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CTriggerCountDlg *pDlg = (CTriggerCountDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CTriggerCountDlg::OnBnClickedFreeRun()
{
	//开始采集
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
	m_nRec=0;
	m_bRun = TRUE;
	EnableButton();
}

void CTriggerCountDlg::OnBnClickedTriggerSoft()
{
	if (m_bRun == FALSE)
	{	
		TriggerModeEnums enumMode;
		TriggerSourceEnums enumSource;

		MVGetTriggerSource(m_hCam, &enumSource);
		if (enumSource != TriggerSource_Software)
		{
			MVSetTriggerSource(m_hCam, TriggerSource_Software);
		}

		MVGetTriggerMode(m_hCam, &enumMode);
		if (enumMode != TriggerMode_On)
		{
			MVSetTriggerMode(m_hCam, TriggerMode_On);
			Sleep(100);
		}

		MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
		m_nRec=0;
		m_bRun = TRUE;
		EnableButton(TRUE);
	}
	MVTriggerSoftware(m_hCam);
}

void CTriggerCountDlg::OnBnClickedTriggerLine1()
{
	TriggerModeEnums enumMode;
	TriggerSourceEnums enumSource;

	MVGetTriggerSource(m_hCam, &enumSource);
	if (enumSource != TriggerSource_Line1)
	{
		MVSetTriggerSource(m_hCam, TriggerSource_Line1);
	}

	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_On)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_On);
		Sleep(100);
	}

	MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
	m_nRec=0;
	m_bRun = TRUE;
	EnableButton(TRUE);
}

void CTriggerCountDlg::OnBnClickedStop()
{
	MVStopGrab(m_hCam);
	m_bRun = FALSE;
	EnableButton();
}



void CTriggerCountDlg::EnableButton( BOOL bStartTrigger )
{
	if (m_bRun)
	{
		SetTimer(UPDATE_COUNT,100, 0);
	}
	else
	{
		KillTimer(UPDATE_COUNT);
	}
	GetDlgItem(IDC_FREE_RUN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_TRIGGER_LINE1)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_TRIGGER_SOFT)->EnableWindow(!m_bRun||bStartTrigger);
	GetDlgItem(IDC_STOP)->EnableWindow(m_bRun);
}


int CTriggerCountDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	m_nCap = pInfo->nBlockId;
	m_nRec++;


	MVInfo2Image(m_hCam,pInfo,&m_image);
	Invalidate(FALSE);

	return 0;
}

void CTriggerCountDlg::OnCbnSelchangeComboActivation()
{
	switch(m_cmbActivation.GetCurSel())
	{
	case 0:
		MVSetTriggerActivation(m_hCam, TriggerActivation_RisingEdge);
		break;
	case 1:
		MVSetTriggerActivation(m_hCam, TriggerActivation_RisingEdge);
		break;
	default:
		break;
	}
}

void CTriggerCountDlg::UpdateCount()
{
	CString str;
	str.Format(_T("%d"),m_nCap);
	GetDlgItem(IDC_CAPTURE_COUNT)->SetWindowText(str);
	str.Format(_T("%d"),m_nRec);
	GetDlgItem(IDC_RECEIVE_COUNT)->SetWindowText(str);
}

void CTriggerCountDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == UPDATE_COUNT)
	{
		UpdateCount();
	}
	CDialog::OnTimer(nIDEvent);
}

void CTriggerCountDlg::DrawImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();
	const int xDest = 8;
	const int yDest = 40;

	CDC *pDC = GetDC();
	//1:1 显示全图
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 显示全图, zoom 50%
	// 	pDC->SetStretchBltMode(COLORONCOLOR);
	// 	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2);

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);

}
