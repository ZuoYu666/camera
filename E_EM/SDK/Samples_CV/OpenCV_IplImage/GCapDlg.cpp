// GCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCap.h"
#include "GCapDlg.h"

#include <winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCapDlg dialog
CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCapDlg::IDD, pParent)
	, m_hCam(NULL)
	, m_bLive(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, &CGCapDlg::OnBnClickedOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CGCapDlg::OnBnClickedStart)
END_MESSAGE_MAP()


// CGCapDlg message handlers

BOOL CGCapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowPos(&CWnd::wndTop, 8, 8, 0, 0,SWP_NOSIZE);

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

	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		m_image.Create(w,h,8);
	}
	else
	{
		m_image.Create(w,h,24);
	}

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
}

void CGCapDlg::OnDestroy()
{
	// TODO: Add your message handler code here
	if( m_bLive )
	{
		MVStopGrab(m_hCam);
	}

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

LRESULT CALLBACK CV_NoCloseProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam)
{
	if( msg == WM_SYSCOMMAND && wParam == SC_CLOSE)
	{
		return 0;
	}
	else
	{
		WNDPROC DefWP = (WNDPROC)GetProp( hWnd, "CV_DEF_PROC" );
		return CallWindowProc( DefWP, hWnd, msg, wParam, lParam );		
	}
}

void CGCapDlg::OnBnClickedStart()
{
	//开始采集
	if( !m_bLive )
	{
		cvNamedWindow("src",CV_WINDOW_AUTOSIZE);
		HWND hParent = ::GetParent((HWND)cvGetWindowHandle("src"));
		if( hParent != NULL )
		{
			//Disable close button
			::EnableMenuItem(::GetSystemMenu(hParent,FALSE),SC_CLOSE,MF_BYCOMMAND|MF_GRAYED);

			//Disable Alt+F4
#ifndef _WIN64
			SetProp(hParent,"CV_DEF_PROC",(HANDLE)GetWindowLong(hParent,GWL_WNDPROC));
			SetWindowLong(hParent,GWL_WNDPROC,(LONG)CV_NoCloseProc);
#endif
		}

		MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
		GetDlgItem(IDC_START)->SetWindowText("Stop");
	}
	else
	{
		MVStopGrab(m_hCam);
		GetDlgItem(IDC_START)->SetWindowText("Start");

		cvDestroyWindow("src");
	}
	m_bLive = !m_bLive;
}

int CGCapDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		IplImage *iplSrc;

		CvSize size = {w,h};
		iplSrc = cvCreateImage(size,8,1);

		memcpy(iplSrc->imageData,pInfo->pImageBuffer,w*h);
		cvShowImage("src",iplSrc);
		cvWaitKey(2);
		cvReleaseImage(&iplSrc);
	}
	else
	{
		IplImage *iplSrc;

		CvSize size = {w,h};
		iplSrc = cvCreateImage(size,8,3);

		MVBayerToBGR(m_hCam,pInfo->pImageBuffer,iplSrc->imageData,iplSrc->widthStep,w,h,m_PixelFormat);
		
		cvShowImage("src",iplSrc);
		cvWaitKey(2);
		cvReleaseImage(&iplSrc);
	}


	return 0;
}

