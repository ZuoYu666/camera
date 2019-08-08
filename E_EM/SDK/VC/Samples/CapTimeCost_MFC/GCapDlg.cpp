// GCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCap.h"
#include "GCapDlg.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define USE_TOOL_CLASS 

// CGCapDlg dialog

CStopWatch g_StopWatch;
CEvent g_EventStop;
double g_fTriggerFps;

DWORD WINAPI ThreadTrigger(LPVOID pParam)
{
	DWORD dWait = 0;
	do 
	{
		DWORD dwMilliseconds = (DWORD)(1000/g_fTriggerFps);
		g_StopWatch.Start();
		MVTriggerSoftware((HANDLE)pParam);
		dWait = WaitForSingleObject(g_EventStop.m_hObject,dwMilliseconds);
	} while (dWait != WAIT_OBJECT_0);
	return 0;
}

CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGCapDlg::IDD, pParent)
, m_hCam(NULL)
, m_bRun(FALSE)
, m_nFPS(10)
, m_fExpo(0)
, m_nWidth(0)
, m_nHeight(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bTrigCont = FALSE;
	m_nTimeCostMax = 0;
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIMER_SET, m_nFPS);
	DDX_Text(pDX, IDC_EXPOSURE, m_fExpo);
	DDX_Text(pDX, IDC_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_HEIGHT, m_nHeight);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, &CGCapDlg::OnBnClickedOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CGCapDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_TRIGGER, &CGCapDlg::OnBnClickedTrigger)
	ON_BN_CLICKED(IDC_TRIG_CONTINUE, &CGCapDlg::OnBnClickedTrigContinue)
	ON_EN_KILLFOCUS(IDC_EXPOSURE, &CGCapDlg::OnEnKillfocusExposure)
	ON_EN_KILLFOCUS(IDC_WIDTH, &CGCapDlg::OnEnKillfocusWidth)
	ON_EN_KILLFOCUS(IDC_HEIGHT, &CGCapDlg::OnEnKillfocusHeight)
END_MESSAGE_MAP()


// CGCapDlg message handlers

BOOL CGCapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// TODO: Add extra initialization here
	MVInitLib();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGCapDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGCapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGCapDlg::OnBnClickedOpen()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if( nCams == 0 )
	{
		MessageBox("没有找到相机,请确认连接和相机IP设置","提示",MB_ICONWARNING);
		return ;
	}

	MVSTATUS_CODES r = MVOpenCamByIndex(0,&m_hCam);
	if( m_hCam == NULL )
	{
		if( r == MVST_ACCESS_DENIED )
		{
			MessageBox("无法打开相机，可能正被别的软件控制","提示",MB_ICONWARNING);
			return ;
		}
	}

	CString str;
	MVGetExposureTime(m_hCam,&m_fExpo);
	str.Format("%.2lf",m_fExpo);
	GetDlgItem(IDC_EXPOSURE)->SetWindowText(str);

	MVSetTriggerMode(m_hCam, TriggerMode_On);
	MVSetTriggerSource(m_hCam, TriggerSource_Software);
	MVSetStrobeSource(m_hCam,LineSource_ExposureActive);
	MVSetStrobeInvert(m_hCam,TRUE);
	
	MVGetWidth(m_hCam,&m_nWidth);
	MVGetHeight(m_hCam,&m_nHeight);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);
	
	UpdateFps();
	UpdateImage();

	GetDlgItem(IDC_WIDTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIMER_SET)->EnableWindow(TRUE);
	GetDlgItem(IDC_EXPOSURE)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CGCapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
		m_hCam = NULL;
	}

	MVTerminateLib();
}


int StreamCB(MV_IMAGE_INFO *pInfo,long nUserVal)
{
	CGCapDlg *pDlg = (CGCapDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CGCapDlg::OnBnClickedStart()
{
	//开始采集
	if( !m_bRun )
	{
		Invalidate(TRUE);
		MVStartGrab(m_hCam,StreamCB,(long)this);
		GetDlgItem(IDC_START)->SetWindowText(_T("停止"));
	}
	else
	{
		GetDlgItem(IDC_START)->SetWindowText(_T("开始"));
		if (m_bTrigCont)
		{
			OnBnClickedTrigContinue();
		}
		MVStopGrab(m_hCam);
	}
	m_bRun = !m_bRun;

	GetDlgItem(IDC_TRIGGER)->EnableWindow(m_bRun);
	GetDlgItem(IDC_TIMER_START)->EnableWindow(m_bRun);
	GetDlgItem(IDC_WIDTH)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_HEIGHT)->EnableWindow(!m_bRun);

}

int CGCapDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	double fMean;
	int nTime;
	CString str;
	if (m_bTrigCont)
	{
		nTime = g_StopWatch.Stop();
		fMean = g_StopWatch.GetAverageTime();

		if (nTime > m_nTimeCostMax)
		{
			m_nTimeCostMax = nTime;
		}
		str.Format("平均%.2f 最大%.2f",fMean/1000.0, m_nTimeCostMax/1000.0);
		GetDlgItem(IDC_STATIC_DELAY1)->SetWindowText(str);
	}

	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		ASSERT( m_image.GetPitch() == m_image.GetWidth() );
		memcpy(m_image.GetBits(),pInfo->pImageBuffer,w*h);
	}
	else
	{
		MVBayerToBGR(m_hCam,pInfo->pImageBuffer,m_image.GetBits(),m_image.GetPitch(),w,h,m_PixelFormat);
	}

	const int xDest = 8;
	const int yDest = 40;

	CDC *pDC = GetDC();
	//1:1 显示全图
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 显示全图, zoom 50%
	//	pDC->SetStretchBltMode(COLORONCOLOR);
	//	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2);

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);

	return 0;
}

void CGCapDlg::OnBnClickedTrigger()
{
//	Invalidate(TRUE);
	MVTriggerSoftware(m_hCam);
}

void CGCapDlg::OnBnClickedTrigContinue()
{
	UpdateData(TRUE);

	if(!m_bTrigCont)
	{
		if (m_nFPS > 0)
		{
			m_nTimeCostMax = 0;
			g_StopWatch.Reset();
			g_EventStop.ResetEvent();
			CreateThread(NULL,0,ThreadTrigger,m_hCam,0,NULL);
			g_fTriggerFps = m_nFPS;
		}
		m_bTrigCont = TRUE;
		GetDlgItem(IDC_TIMER_START)->SetWindowText(_T("停止触发"));

	}
	else
	{
		g_EventStop.SetEvent();
		m_bTrigCont = FALSE;
		GetDlgItem(IDC_TIMER_START)->SetWindowText(_T("连续触发"));
	}
	GetDlgItem(IDC_TIMER_SET)->EnableWindow(!m_bTrigCont);
	GetDlgItem(IDC_TRIGGER)->EnableWindow(!m_bTrigCont);
}


void CGCapDlg::OnEnKillfocusExposure()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MVSetExposureTime(m_hCam,m_fExpo);
}

void CGCapDlg::OnEnKillfocusWidth()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nMinWidth;
	int nMaxWidth;
	MVGetWidthRange(m_hCam,&nMinWidth,&nMaxWidth);
	if( nMinWidth == nMaxWidth )
	{
		MessageBox("相机不支持改变图像大小","Error",MB_ICONWARNING);
		return;
	}

	MVSetWidth(m_hCam,m_nWidth);
	Sleep(100);
	MVGetWidth(m_hCam,&m_nWidth);
	
 	MVSetOffsetX(m_hCam,(nMaxWidth-m_nWidth)/2);

	UpdateFps();
	UpdateImage();
	UpdateData(FALSE);
}

void CGCapDlg::OnEnKillfocusHeight()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nMinHeight;
	int nMaxHeight;
	MVGetHeightRange(m_hCam,&nMinHeight,&nMaxHeight);
	if( nMinHeight == nMaxHeight )
	{
		MessageBox("相机不支持改变图像大小","Error",MB_ICONWARNING);
		return;
	}

	MVSetHeight(m_hCam,m_nHeight);
	Sleep(100);
	MVGetHeight(m_hCam,&m_nHeight);
 	MVSetOffsetY(m_hCam,(nMaxHeight-m_nHeight)/2);
	UpdateFps();
	UpdateImage();
	UpdateData(FALSE);
}

BOOL CGCapDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN) 
	{ 
		switch(pMsg->wParam)  
		{  
		case VK_RETURN:   //屏蔽Enter 
			if(GetFocus() == GetDlgItem(IDC_EXPOSURE))
			{
				OnEnKillfocusExposure();
				((CEdit*)GetDlgItem(IDC_EXPOSURE))->SetSel(0,-1,FALSE);
			}
			else if(GetFocus() == GetDlgItem(IDC_WIDTH))
			{
				OnEnKillfocusWidth();
				((CEdit*)GetDlgItem(IDC_WIDTH))->SetSel(0,-1,FALSE);
			}
			else if(GetFocus() == GetDlgItem(IDC_HEIGHT))
			{
				OnEnKillfocusHeight();
				((CEdit*)GetDlgItem(IDC_HEIGHT))->SetSel(0,-1,FALSE);
			}
			return TRUE;
		} 
	}
	return CDialog::PreTranslateMessage(pMsg);
}

int CGCapDlg::UpdateFps()
{
	//图像宽高改变后，最大帧率会随之变化
	double fFpsMin,fFpsMax;
	MVGetFrameRateRange(m_hCam,&fFpsMin,&fFpsMax);

	//设置到最大帧率
	return MVSetFrameRate(m_hCam,fFpsMax);
	//////////////////////////////////////////////////////////////////////////
}

int CGCapDlg::UpdateImage()
{
	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		m_image.Create(m_nWidth,m_nHeight,8);
	}
	else
	{
		m_image.Create(m_nWidth,m_nHeight,24);
	}
	return 0;
}
