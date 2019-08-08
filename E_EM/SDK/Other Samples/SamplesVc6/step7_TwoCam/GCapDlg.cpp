// GCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCap.h"
#include "GCapDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGCapDlg dialog

CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGCapDlg::IDD, pParent)
	, m_dGain1(0)
	, m_dGain2(0)
	, m_dExpo1(0)
	, m_dExpo2(0)
{
	for(int i=0; i<CAM_MAX; i++)
	{
		m_hCam[i] = NULL;
	}
	m_nCams = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_picSize = Pic_Size_1_1;
	m_dZoom = 1;
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1_GAIN, m_dGain1);
	DDX_Text(pDX, IDC_EDIT2_GAIN, m_dGain2);
	DDX_Text(pDX, IDC_EDIT1_EXPO, m_dExpo1);
	DDX_Text(pDX, IDC_EDIT2_EXPO, m_dExpo2);
	DDX_Control(pDX, IDC_COMBO1, m_cmbZoom);
	DDX_Control(pDX, IDC_SPIN_GAIN2, m_spinGain2);
	DDX_Control(pDX, IDC_SPIN_GAIN1, m_spinGain1);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1_GAIN, OnBnClickedButton1Gain)
	ON_BN_CLICKED(IDC_BUTTON2_GAIN, OnBnClickedButton2Gain)
	ON_BN_CLICKED(IDC_BUTTON1_EXPO, OnBnClickedButton1Expo)
	ON_BN_CLICKED(IDC_BUTTON2_EXPO, OnBnClickedButton2Expo)
	ON_CBN_SELCHANGE(IDC_COMBO_ZOOM, OnCbnSelchangeComboZoom)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN1, OnDeltaposSpinGain1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN2, OnDeltaposSpinGain2)
END_MESSAGE_MAP()


// CGCapDlg message handlers

BOOL CGCapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

//	ShowWindow(SW_MAXIMIZE);
	m_cmbZoom.SetCurSel(2);

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
	m_nCams = min(nCams,CAM_MAX);
	if( m_nCams == 0 )
	{
		MessageBox("没有找到相机,请确认连接和相机IP设置","提示",MB_ICONWARNING);
		return ;
	}

	for(int i=0; i<m_nCams; i++)
	{
		MVSTATUS_CODES r = MVOpenCamByIndex(i,&m_hCam[i]);
		if( m_hCam[i] == NULL )
		{
			if( r == MVST_ACCESS_DENIED )
			{
				MessageBox("无法打开相机，可能正被别的软件控制","提示",MB_ICONWARNING);
				return ;
			}
		}

		int w,h;			
		MVGetWidth(m_hCam[i],&w);
		MVGetHeight(m_hCam[i],&h);
		MVGetPixelFormat(m_hCam[i],&m_PixelFormat[i]);

		if( m_PixelFormat[i] == PixelFormat_Mono8 )
		{
			m_image[i].Create(w,h,8);
		}
		else
		{
			m_image[i].Create(w,h,24);
		}
	}
	double fGainMin,fGainMax;
	if (m_hCam[0]!=NULL)
	{
		MVGetGainRange(m_hCam[0],&fGainMin,&fGainMax);
		m_spinGain1.SetRange(ceil(fGainMin),(int)fGainMax); 
		MVGetGain(m_hCam[0],&m_dGain1);
		m_spinGain1.SetPos(ceil(m_dGain1));
		MVGetExposureTime(m_hCam[0],&m_dExpo1);
	}
	if (m_hCam[1]!=NULL)
	{
		MVGetGainRange(m_hCam[1],&fGainMin,&fGainMax);
		m_spinGain2.SetRange(ceil(fGainMin),(int)fGainMax); 
		MVGetGain(m_hCam[1],&m_dGain2);
		m_spinGain2.SetPos(ceil(m_dGain2));
		MVGetExposureTime(m_hCam[1],&m_dExpo2);
	}
	UpdateData(FALSE);
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
}

void CGCapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	for(int i=0; i<CAM_MAX; i++)
	{
		if( m_hCam[i] != NULL )
		{
			MVCloseCam(m_hCam[i]);
		}
	}

	MVTerminateLib();
}


int StreamCB0(MV_IMAGE_INFO *pInfo,long nUserVal)
{
	CGCapDlg *pDlg = (CGCapDlg *)nUserVal;
	return ( pDlg->OnStreamCB0(pInfo) );
}

int StreamCB1(MV_IMAGE_INFO *pInfo,long nUserVal)
{
	CGCapDlg *pDlg = (CGCapDlg *)nUserVal;
	return ( pDlg->OnStreamCB1(pInfo) );
}

void CGCapDlg::OnBnClickedStart()
{
	//开始采集
	MVStartGrab(m_hCam[0],StreamCB0,(long)this);
	MVStartGrab(m_hCam[1],StreamCB1,(long)this);
}

int CGCapDlg::OnStreamCB0( MV_IMAGE_INFO *pInfo)
{
	ImgDraw(pInfo,0);
	return 0;
}
int CGCapDlg::OnStreamCB1( MV_IMAGE_INFO *pInfo)
{
	ImgDraw(pInfo,1);
	return 0;
}

void CGCapDlg::ImgDraw( MV_IMAGE_INFO *pInfo, int idx )
{
	int w = m_image[idx].GetWidth();
	int h = m_image[idx].GetHeight();
	
	if( m_PixelFormat[idx] == PixelFormat_Mono8 )
	{
		ASSERT( m_image[idx].GetPitch() == m_image[idx].GetWidth() );
		memcpy(m_image[idx].GetBits(),pInfo->pImageBuffer,w*h);
	}
	else
	{
		MVBayerToBGR(m_hCam[idx],pInfo->pImageBuffer,m_image[idx].GetBits(),m_image[idx].GetPitch(),w,h,m_PixelFormat[idx]);
	}

	const int xDest = 8;
	const int yDest = 40;

	CDC *pDC = GetDC();

	RECT rect;
	GetWindowRect(&rect);

	if (m_picSize == Pic_Size_Fit)
	{
		pDC->SetStretchBltMode(COLORONCOLOR);

		m_image[idx].Draw(pDC->GetSafeHdc(),m_rect[idx].left,m_rect[idx].top,m_rect[idx].Width(),m_rect[idx].Height());
	}
	else
	{
		pDC->SetStretchBltMode(COLORONCOLOR);

		m_image[idx].Draw(pDC->GetSafeHdc(),m_rect[idx].left,m_rect[idx].top,m_rect[idx].Width(),m_rect[idx].Height(),0,0,m_rect[idx].Width()/m_dZoom,m_rect[idx].Height()/m_dZoom);

	}
	ReleaseDC(pDC);
}


void CGCapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here

	CRect rect;
	GetClientRect(&rect);

	m_rect[0].left = X_OFFEST;
	m_rect[0].right = rect.right/2 -10;
	m_rect[0].top = Y_OFFSET;
	m_rect[0].bottom = rect.bottom-10;

	m_rect[1].left = rect.right/2+X_OFFEST;
	m_rect[1].right = rect.right-10;
	m_rect[1].top = Y_OFFSET;
	m_rect[1].bottom = rect.bottom-10;

}


void CGCapDlg::OnBnClickedButton1Gain()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MVSetGain(m_hCam[0],m_dGain1);
}

void CGCapDlg::OnBnClickedButton2Gain()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MVSetGain(m_hCam[1],m_dGain2);
}

void CGCapDlg::OnBnClickedButton1Expo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MVSetExposureTime(m_hCam[0],m_dExpo1);
}

void CGCapDlg::OnBnClickedButton2Expo()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	MVSetExposureTime(m_hCam[1],m_dExpo2);
}

void CGCapDlg::OnCbnSelchangeComboZoom()
{
	// TODO: Add your control notification handler code here
	CString strCOM;
	m_cmbZoom.GetLBText(m_cmbZoom.GetCurSel(),strCOM);

	if (strcmp(strCOM.LockBuffer(),"100%") == 0)
	{
		m_dZoom = 1;
		m_picSize = Pic_Size_1_1;
	}
	else if (strcmp(strCOM.LockBuffer(),"Fit") == 0)
	{
		m_picSize = Pic_Size_Fit;
	}
	else if (strcmp(strCOM.LockBuffer(),"50%")==0)
	{
		m_dZoom = 0.5;
		m_picSize = Pic_Size_2_1;
	}
	else if (strcmp(strCOM.LockBuffer(),"25%")==0)
	{
		m_dZoom = 0.25;
		m_picSize = Pic_Size_4_1;
	}
	else if (strcmp(strCOM.LockBuffer(),"200%")==0)
	{
		m_dZoom = 2;
		m_picSize = Pic_Size_1_2;
	}
	Invalidate(TRUE);
}

void CGCapDlg::OnDeltaposSpinGain1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int c = pNMUpDown->iDelta+pNMUpDown->iPos;
	MVSetGain(m_hCam[0],c);
	*pResult = 0;
}

void CGCapDlg::OnDeltaposSpinGain2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	int c = pNMUpDown->iDelta+pNMUpDown->iPos;
	MVSetGain(m_hCam[1],c);
	*pResult = 0;
}
