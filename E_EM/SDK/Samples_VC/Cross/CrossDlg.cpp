// CrossDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Cross.h"
#include "CrossDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrossDlg dialog


CCrossDlg::CCrossDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCrossDlg::IDD, pParent)
, m_hCam(NULL)
, m_bRun(FALSE)
, m_nFlip(0)
, m_nRotate(0)
, m_nZoom(2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_csInvalidate);
}

void CCrossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FLIP, m_cmbFlip);
	DDX_Control(pDX, IDC_COMBO_ROTATE, m_cmbRotate);
	DDX_Control(pDX, IDC_COMBO_ZOOM, m_cmbZoom);
	DDX_CBIndex(pDX, IDC_COMBO_FLIP, m_nFlip);
	DDX_CBIndex(pDX, IDC_COMBO_ROTATE, m_nRotate);
	DDX_CBIndex(pDX, IDC_COMBO_ZOOM, m_nZoom);
}

BEGIN_MESSAGE_MAP(CCrossDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CCrossDlg::OnBnClickedStart)
	ON_CBN_SELCHANGE(IDC_COMBO_ROTATE, &CCrossDlg::OnCbnSelchangeComboRotate)
	ON_CBN_SELCHANGE(IDC_COMBO_ZOOM, &CCrossDlg::OnCbnSelchangeComboZoom)
	ON_CBN_SELCHANGE(IDC_COMBO_FLIP, &CCrossDlg::OnCbnSelchangeComboFlip)
END_MESSAGE_MAP()


// CCrossDlg message handlers

BOOL CCrossDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	MVInitLib();
	OpenCamera();
	m_cmbFlip.SetCurSel(0);
	m_cmbRotate.SetCurSel(0);
	m_cmbZoom.SetCurSel(2);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCrossDlg::OnPaint()
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
		if(!m_bRun && !m_image.IsNull())
		{
			DrawImage();
		}
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCrossDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCrossDlg::OpenCamera()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if( nCams == 0 )
	{
		AfxMessageBox(IDS_NO_CAM);
		return ;
	}

	MVSTATUS_CODES r = MVOpenCamByIndex(0,&m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			AfxMessageBox(IDS_ERR_ACCESS_DENY);			
		else
			AfxMessageBox(IDS_ERR_OPEN);
		return;
	}

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}

void CCrossDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();
}


int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CCrossDlg *pDlg = (CCrossDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CCrossDlg::OnBnClickedStart()
{
	//开始采集
	if( !m_bRun )
	{
		if (m_image.IsNull())
		{
			int w,h;			
			MVGetWidth(m_hCam,&w);
			MVGetHeight(m_hCam,&h);
			MVGetPixelFormat(m_hCam,&m_PixelFormat);

			if( m_PixelFormat == PixelFormat_Mono8 )
			{
				m_imageRotate.Create(h, w, 8);
				m_image.Create(w,h,8);
				m_imageFlip.Create(w,h,8);
			}
			else
			{
				m_imageRotate.Create(h, w, 24);
				m_image.Create(w,h,24);
				m_imageFlip.Create(w,h,24);
			}
		}


		MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);

		CString str(MAKEINTRESOURCE(IDS_STOP));
		GetDlgItem(IDC_START)->SetWindowText(str);
	}
	else
	{
		MVStopGrab(m_hCam);

		CString str(MAKEINTRESOURCE(IDS_START));
		GetDlgItem(IDC_START)->SetWindowText(str);
	}
	m_bRun = !m_bRun;
}

int CCrossDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image);
	//////////////////////////////////////////////////////////////////////////
	DrawImage();

	return 0;
}


void CCrossDlg::OnCbnSelchangeComboRotate()
{
	// TODO: Add your control notification handler code here
	EnterCriticalSection(&m_csInvalidate);
	UpdateData(TRUE);
	Invalidate(TRUE);
	LeaveCriticalSection(&m_csInvalidate);
}

void CCrossDlg::DrawImage( MVImage* pImg, int m_nZoom )
{
	int w;
	int h;
	//////////////////////////////////////////////////////////////////////////
	// 显示出来
	const int xDest = 8;
	const int yDest = 40;
	int nStretch;

	CDC *pDC = GetDC();

	CDC m_MemDC;
	CBitmap  m_Bitmap;

	if( m_nZoom == 2 )
	{
		nStretch = 1;
	}
	else if( m_nZoom == 1)
	{
		nStretch = 2;

	}
	else if (m_nZoom == 0)
	{
		nStretch = 4;
	}
	w = pImg->GetWidth()/nStretch;
	h = pImg->GetHeight()/nStretch;


	m_MemDC.CreateCompatibleDC(pDC);
	m_Bitmap.CreateCompatibleBitmap(pDC,w,h);
	m_MemDC.SelectObject(m_Bitmap);

	m_MemDC.SetStretchBltMode(COLORONCOLOR);
	StretchBlt(m_MemDC.GetSafeHdc(),0,0,w,h,pImg->GetDC() ,0,0,pImg->GetWidth(),pImg->GetHeight(),SRCCOPY);

	CPen pen(PS_SOLID, 2, RGB(255,0,0));

	SelectObject(m_MemDC.GetSafeHdc(),pen);

	MoveToEx(m_MemDC.GetSafeHdc(),w/2,0,NULL);
	LineTo(m_MemDC.GetSafeHdc(),w/2,h-1);
	MoveToEx(m_MemDC.GetSafeHdc(),0,h/2,NULL);
	LineTo(m_MemDC.GetSafeHdc(),w-1,h/2);

	pDC->BitBlt(xDest,yDest,w,h,&m_MemDC,0,0,SRCCOPY);

	m_MemDC.DeleteDC();
	m_Bitmap.DeleteObject();

	ReleaseDC(pDC);
}

void CCrossDlg::DrawImage()
{
	MVImage *pImage = &m_image;
	EnterCriticalSection(&m_csInvalidate);
	switch(m_nFlip)
	{
	case 0:
		break;
	case 1:
		MVImageFlip(m_hCam, &m_image, &m_imageFlip, FlipHorizontal);
		pImage = &m_imageFlip;
		break;
	case 2:
		MVImageFlip(m_hCam, &m_image, &m_imageFlip, FlipVertical);
		pImage = &m_imageFlip;
		break;
	case 3:
		MVImageFlip(m_hCam, &m_image, &m_imageFlip, FlipBoth);
		pImage = &m_imageFlip;
		break;
	default:
		break;
	}

	switch(m_nRotate)
	{
	case 0:
		DrawImage(pImage,m_nZoom);
		break;
	case 1:
		MVImageRotate(m_hCam,pImage, &m_imageRotate, Rotate90DegCw);
		DrawImage(&m_imageRotate,m_nZoom);
		break;
	case 2:
		MVImageRotate(m_hCam,pImage, &m_imageRotate, Rotate90DegCcw);
		DrawImage(&m_imageRotate,m_nZoom);
		break;
	default:
		break;
	}
	LeaveCriticalSection(&m_csInvalidate);
}


void CCrossDlg::OnCbnSelchangeComboZoom()
{
	// TODO: Add your control notification handler code here
	EnterCriticalSection(&m_csInvalidate);
	UpdateData(TRUE);
	Invalidate(TRUE);
	LeaveCriticalSection(&m_csInvalidate);
}

void CCrossDlg::OnCbnSelchangeComboFlip()
{
	// TODO: Add your control notification handler code here
	EnterCriticalSection(&m_csInvalidate);
	UpdateData(TRUE);
	Invalidate(TRUE);
	LeaveCriticalSection(&m_csInvalidate);
}
