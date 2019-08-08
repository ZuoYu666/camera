// GCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCap.h"
#include "GCapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCapDlg dialog




CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCapDlg::IDD, pParent)
	, m_hCam(NULL)
	, m_bLive(FALSE)
	, m_nTh(128)
	, m_nMethod(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TH, m_sldTh);
	DDX_Control(pDX, IDC_COMBO_PROCESS, m_cmbProcess);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, &CGCapDlg::OnBnClickedOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CGCapDlg::OnBnClickedStart)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO_PROCESS, &CGCapDlg::OnCbnSelchangeComboProcess)
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

	m_sldTh.SetRange(1,255);
	m_sldTh.SetPos(m_nTh);

	m_cmbProcess.SetCurSel(0);

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

	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		m_image.Create(w,h,8);
		
		m_cvSrc.create(h,w,CV_8UC1);
	}
	else
	{
		m_image.Create(w,h,24);
		
		m_cvSrc.create(h,w,CV_8UC3);
	}

	m_cvDst = m_cvSrc.clone();

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
}

void CGCapDlg::OnDestroy()
{
	// TODO: Add your message handler code here
	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();

	CDialog::OnDestroy();
}


int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CGCapDlg *pDlg = (CGCapDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CGCapDlg::OnBnClickedStart()
{
	//开始采集
	if( !m_bLive )
	{
		MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
		GetDlgItem(IDC_START)->SetWindowText("Stop");
	}
	else
	{
		MVStopGrab(m_hCam);
		GetDlgItem(IDC_START)->SetWindowText("Start");
	}
	m_bLive = !m_bLive;
}

int CGCapDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		ASSERT( m_image.GetPitch() == m_image.GetWidth() );

		cv::Mat kernel(3,3,CV_32F,cv::Scalar(-1));   
		kernel.at<float>(1,1) = 8.9;  

		if( m_nMethod == 0 )
		{
			memcpy(m_image.GetBits(),pInfo->pImageBuffer,w*h);
		}
		else
		{
			memcpy(m_cvSrc.data,pInfo->pImageBuffer,w*h);
			switch( m_nMethod )
			{
			case 1:
				threshold( m_cvSrc, m_cvDst, m_nTh, 255, 0 );
				break;
			case 2:
				GaussianBlur(m_cvSrc,m_cvDst,cv::Size(5,5),0);
				break;
			case 3:
				medianBlur(m_cvSrc,m_cvDst,5); 
				break;
			case 4:
				filter2D(m_cvSrc,m_cvDst,m_cvDst.depth(),kernel); 
				break;
			}
			memcpy(m_image.GetBits(),m_cvDst.data,w*h);
		}
	}
	else
	{
		if( m_nMethod == 0 )
		{
		MVBayerToBGR(m_hCam,pInfo->pImageBuffer,m_image.GetBits(),m_image.GetPitch(),w,h,m_PixelFormat);
		}
		else
		{
		cv::Mat kernel(3,3,CV_32F,cv::Scalar(-1));   
		kernel.at<float>(1,1) = 8.9;  

			MVBayerToBGR(m_hCam,pInfo->pImageBuffer,m_cvSrc.data,w*3,w,h,m_PixelFormat);

			cv::Mat cvGray;
			switch( m_nMethod )
			{
			case 1:
				cvGray.create(h,w,CV_8UC1);
				cvtColor(m_cvSrc,cvGray,CV_BGR2GRAY,0);
				threshold( cvGray, cvGray, m_nTh, 255, 0 );
				cvtColor(cvGray,m_cvDst,CV_GRAY2BGR,0);
				break;
			case 2:
				GaussianBlur(m_cvSrc,m_cvDst,cv::Size(5,5),0);
				break;
			case 3:
				medianBlur(m_cvSrc,m_cvDst,5); 
				break;
			case 4:
				filter2D(m_cvSrc,m_cvDst,m_cvDst.depth(),kernel); 
				break;
			}
			memcpy(m_image.GetBits(),m_cvDst.data,w*h*3);
		}
	}

	const int xDest = 8;
	const int yDest = 40;

	CDC *pDC = GetDC();
	//1:1 显示全图
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 显示全图, zoom 50%
	//pDC->SetStretchBltMode(COLORONCOLOR);
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2);

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);

	return 0;
}

void CGCapDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	m_nTh = m_sldTh.GetPos();
	
	CString str;
	str.Format("%d",m_nTh);
	GetDlgItem(IDC_TH)->SetWindowText(str);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGCapDlg::OnCbnSelchangeComboProcess()
{
	m_nMethod = m_cmbProcess.GetCurSel();
}
