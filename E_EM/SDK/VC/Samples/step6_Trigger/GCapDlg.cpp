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
	, m_bRun(FALSE)
	, m_Ufps(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FPS, m_Ufps);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, &CGCapDlg::OnBnClickedOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CGCapDlg::OnBnClickedStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_TRIGGER, &CGCapDlg::OnBnClickedTrigger)
	ON_BN_CLICKED(IDC_PREVIEW, &CGCapDlg::OnBnClickedPreview)
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
		MessageBox("û���ҵ����,��ȷ�����Ӻ����IP����","��ʾ",MB_ICONWARNING);
		return ;
	}

	MVSTATUS_CODES r = MVOpenCamByIndex(0,&m_hCam);
	if( m_hCam == NULL )
	{
		if( r == MVST_ACCESS_DENIED )
		{
			MessageBox("�޷��������������������������","��ʾ",MB_ICONWARNING);
			return ;
		}
	}

	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);

	MVSetTriggerMode(m_hCam, TriggerMode_On);
	MVSetTriggerSource(m_hCam, TriggerSource_Software);

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		m_image.Create(w,h,8);

	}
	else
	{
		m_image.Create(w,h,24);
	}

	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}

void CGCapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
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
	//��ʼ�ɼ�
	if( !m_bRun )
	{
		MVStartGrab(m_hCam,StreamCB,(long)this);
		GetDlgItem(IDC_START)->SetWindowText(_T("Stop"));
	}
	else
	{
		GetDlgItem(IDC_START)->SetWindowText(_T("Start"));
		GetDlgItem(IDC_TIMER_START)->SetWindowText(_T("Start Preview"));
		KillTimer(1);
	}
	m_bRun = !m_bRun;
	
	GetDlgItem(IDC_TRIGGER)->EnableWindow(m_bRun);
	GetDlgItem(IDC_TIMER_START)->EnableWindow(m_bRun);
}

int CGCapDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
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
	//1:1 ��ʾȫͼ
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 ��ʾȫͼ, zoom 50%
	pDC->SetStretchBltMode(COLORONCOLOR);
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2);

	//1:1 ��ʾ�м䲿��ͼ
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);

	return 0;
}

void CGCapDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	MVTriggerSoftware(m_hCam);
	CDialog::OnTimer(nIDEvent);
}

void CGCapDlg::OnBnClickedTrigger()
{
	MVTriggerSoftware(m_hCam);
}

void CGCapDlg::OnBnClickedPreview()
{
	UpdateData(TRUE);
	static BOOL timer_started = FALSE;
	if(!timer_started)
	{
		if (m_Ufps > 0)
		{
			SetTimer(1,1000/m_Ufps,NULL);
		}
		timer_started = TRUE;
		GetDlgItem(IDC_TIMER_START)->SetWindowText(_T("Stop Preview"));
		GetDlgItem(IDC_TIMER_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_TRIGGER)->EnableWindow(FALSE);
	}
	else
	{
		KillTimer(1);
		timer_started = FALSE;
		GetDlgItem(IDC_TIMER_START)->SetWindowText(_T("Start Preview"));
		GetDlgItem(IDC_TIMER_SET)->EnableWindow(TRUE);
		GetDlgItem(IDC_TRIGGER)->EnableWindow(TRUE);
	}
}
