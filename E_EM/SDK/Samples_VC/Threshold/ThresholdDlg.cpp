// ThresholdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Threshold.h"
#include "ThresholdDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CThresholdDlg::CThresholdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThresholdDlg::IDD, pParent)
	, m_hCam(NULL)
	, m_bRun(FALSE)
	, m_nTh(128)
	, m_bBinary(FALSE)
	, m_bInvert(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThresholdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_TH, m_sldTh);
	DDX_Check(pDX, IDC_CHECK_BINARY, m_bBinary);
	DDX_Check(pDX, IDC_CHECK_INVERT, m_bInvert);
	DDX_Control(pDX, IDC_COMBO_DEPTH, m_cmbDepth);
}

BEGIN_MESSAGE_MAP(CThresholdDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, &CThresholdDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_SAVE, &CThresholdDlg::OnBnClickedSave)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_BINARY, &CThresholdDlg::OnBnClickedCheckBinary)
	ON_BN_CLICKED(IDC_CHECK_INVERT, &CThresholdDlg::OnBnClickedCheckInvert)
	ON_CBN_SELCHANGE(IDC_COMBO_DEPTH, &CThresholdDlg::OnCbnSelchangeComboDepth)
END_MESSAGE_MAP()


// CThresholdDlg message handlers

BOOL CThresholdDlg::OnInitDialog()
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

void CThresholdDlg::OnPaint()
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
HCURSOR CThresholdDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CThresholdDlg::OpenCamera()
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

	MVGetPixelFormat(m_hCam,&m_PixelFormat);
	if (m_PixelFormat == PixelFormat_Mono8)
	{
		m_sldTh.SetRange(0,255);
		m_sldTh.SetPos(m_nTh);
		m_cmbDepth.SetCurSel(0);
	}
	else if (m_PixelFormat == PixelFormat_Mono16)
	{
		m_cmbDepth.SetCurSel(1);
		m_sldTh.SetRange(0,65535);
		m_sldTh.SetPos(m_nTh);
		SetDlgItemInt(IDC_TH,m_nTh);
	}
	else if( m_PixelFormat == PixelFormat_BayerGR16 
		|| m_PixelFormat == PixelFormat_BayerRG16 
		|| m_PixelFormat == PixelFormat_BayerGB16
		|| m_PixelFormat == PixelFormat_BayerBG16 )
	{
		m_cmbDepth.SetCurSel(1);
		m_sldTh.SetRange(0,65535);
		m_sldTh.SetPos(m_nTh);
		SetDlgItemInt(IDC_TH,m_nTh);
		GetDlgItem(IDC_CHECK_BINARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_TH)->EnableWindow(FALSE);
	}
	else
	{
		m_sldTh.SetRange(0,255);
		m_sldTh.SetPos(m_nTh);
		m_cmbDepth.SetCurSel(0);
		GetDlgItem(IDC_CHECK_BINARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_TH)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}

void CThresholdDlg::OnDestroy()
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
	CThresholdDlg *pDlg = (CThresholdDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CThresholdDlg::OnBnClickedStart()
{
	//开始采集
	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	m_image.CreateByPixelFormat(w,h,m_PixelFormat);
	m_imageDst.CreateByPixelFormat(w,h,m_PixelFormat);
	if( !m_bRun )
	{
		MVStartGrab(m_hCam,StreamCB,(long)this);
		GetDlgItem(IDC_START)->SetWindowText(_T("停止"));
}
	else
	{
		MVStopGrab(m_hCam);
		GetDlgItem(IDC_START)->SetWindowText(_T("开始"));
	}
	m_bRun = !m_bRun;
	GetDlgItem(IDC_SAVE)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_COMBO_DEPTH)->EnableWindow(!m_bRun);
}

int CThresholdDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image);

	if( m_bInvert || m_bBinary )
	{
		ProcessImage();
	}
	DrawImage();
	return 0;
}

void CThresholdDlg::OnBnClickedSave()
{
	CFileDialog Dlg(FALSE,"BMP","Untitled",OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Bitmap (*.bmp)|*.bmp|Jpeg (*.jpg)|*.jpg|TIFF (*.tif)|*.tif|"); 
	if( Dlg.DoModal() == IDOK )
	{
			if( m_bInvert || m_bBinary )
			{
				m_imageDst.Save(Dlg.GetPathName());
			}
			else
			{
				m_image.Save(Dlg.GetPathName());
			}
	}
}


void CThresholdDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	m_nTh = m_sldTh.GetPos();
	CString str;
	str.Format("%d",m_nTh);
	GetDlgItem(IDC_TH)->SetWindowText(str);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	if( !m_bRun )
	{
		if( m_bInvert || m_bBinary )
		{
			ProcessImage();
		}

		Invalidate(FALSE);
	}
}

void CThresholdDlg::OnBnClickedCheckBinary()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if( !m_bRun )
	{
		if( m_bInvert || m_bBinary )
		{
			ProcessImage();
		}

		Invalidate(FALSE);
	}
}

void CThresholdDlg::OnBnClickedCheckInvert()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if( !m_bRun )
	{
		if( m_bInvert || m_bBinary )
		{
			ProcessImage();
		}

		Invalidate(FALSE);
	}
}

void CThresholdDlg::DrawImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	const int xDest = 8;
	const int yDest = 40;

	CDC *pDC = GetDC();
	//1:1 显示全图

	if( m_bBinary || m_bInvert )
	{
		m_imageDst.Draw(pDC->GetSafeHdc(),xDest,yDest);
	}
	else
	{
		m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);
	}

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);
}

void CThresholdDlg::ProcessImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	if( m_PixelFormat == PixelFormat_Mono8 )
	{
		unsigned char *p = (unsigned char *)m_image.GetBits();
		unsigned char *pDst = (unsigned char *)m_imageDst.GetBits();
		for(int j=0;j<h;j++)
		{
			for(int i=0;i<w;i++)
			{
				if (m_bBinary)
				{
					if( *p < m_nTh )
					{
						*pDst = 0;
					}
					else
					{
						*pDst = 255;
					}
					if (m_bInvert)
					{
						*pDst = 255-*pDst;							
					}
				}
				else
				{
					if (m_bInvert)
					{
						*pDst = 255 - *p;
					}
				}
				pDst++;
				p++;
			}
		}
	}
	else if (m_PixelFormat == PixelFormat_Mono16)
	{
		unsigned short *p = (unsigned short *)m_image.GetBits();
		unsigned short *pDst = (unsigned short *)m_imageDst.GetBits();
		for(int j=0;j<h;j++)
		{
			for(int i=0;i<w;i++)
			{
				if (m_bBinary)
				{
					if( *p < m_nTh )
					{
						*pDst = 0;
					}
					else
					{
						*pDst = 65535;
					}
					if (m_bInvert)
					{
						*pDst = 65535-*pDst;							
					}
				}
				else
				{
					if (m_bInvert)
					{
						*pDst = 65535 - *p;
					}
				}
				pDst++;
				p++;
			}
		}
	}
	else if( m_PixelFormat == PixelFormat_BayerGR16 
				|| m_PixelFormat == PixelFormat_BayerRG16 
				|| m_PixelFormat == PixelFormat_BayerGB16
				|| m_PixelFormat == PixelFormat_BayerBG16 )
	{
		unsigned short *p = (unsigned short *)m_image.GetBits();
		unsigned short *pDst = (unsigned short *)m_imageDst.GetBits();

		if (m_bInvert)
		{
			for(int j=0;j<h;j++)
			{
				for(int i=0;i<3*w;i++)
				{
					*pDst++ = 65535-*p++;
				}
			}
		}
	}
	else
	{
		unsigned char *p = (unsigned char *)m_image.GetBits();
		unsigned char *pDst = (unsigned char *)m_imageDst.GetBits();

		if (m_bInvert)
		{
			for(int j=0;j<h;j++)
			{
				for(int i=0;i<3*w;i++)
				{
					*pDst++ = 255-*p++;
				}
			}
		}
	}
}
void CThresholdDlg::OnCbnSelchangeComboDepth()
{
	int nsel;
	nsel = m_cmbDepth.GetCurSel();
	if (nsel == 0)
	{
		if (m_PixelFormat == PixelFormat_BayerBG16 )
		{
			m_PixelFormat = PixelFormat_BayerBG8;
		}
		if (m_PixelFormat == PixelFormat_BayerGB16 )
		{
			m_PixelFormat = PixelFormat_BayerGB8;
		}
		if (m_PixelFormat == PixelFormat_BayerGR16 )
		{
			m_PixelFormat = PixelFormat_BayerGR8;
		}
		if (m_PixelFormat == PixelFormat_BayerRG16 )
		{
			m_PixelFormat = PixelFormat_BayerRG8;
		}
		if (m_PixelFormat == PixelFormat_Mono16)
		{
			m_PixelFormat = PixelFormat_Mono8;
		}
		MVSetPixelFormat(m_hCam,m_PixelFormat);
		m_sldTh.SetRange(0,255);
		m_nTh = (255+1)/2;
		m_sldTh.SetPos(m_nTh);
		SetDlgItemInt(IDC_TH,m_nTh);
	}
	else if (nsel == 1)
	{
		if (m_PixelFormat == PixelFormat_BayerBG8)
		{
			m_PixelFormat = PixelFormat_BayerBG16;
		}
		if (m_PixelFormat == PixelFormat_BayerGB8)
		{
			m_PixelFormat = PixelFormat_BayerGB16;
		}
		if (m_PixelFormat == PixelFormat_BayerGR8)
		{
			m_PixelFormat = PixelFormat_BayerGR16;
		}
		if (m_PixelFormat == PixelFormat_BayerRG8)
		{
			m_PixelFormat = PixelFormat_BayerRG16;
		}
		if (m_PixelFormat == PixelFormat_Mono8)
		{
			m_PixelFormat = PixelFormat_Mono16;
		}
		MVSetPixelFormat(m_hCam,m_PixelFormat);
		m_sldTh.SetRange(0,65535);
		m_nTh = (65535+1)/2;
		m_sldTh.SetPos(m_nTh);
		SetDlgItemInt(IDC_TH,m_nTh);
	}
}
