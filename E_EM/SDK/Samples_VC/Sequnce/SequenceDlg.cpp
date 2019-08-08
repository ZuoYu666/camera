// SequenceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sequence.h"
#include "SequenceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSequenceDlg dialog
CSequenceDlg::CSequenceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSequenceDlg::IDD, pParent)
	, m_hCam(NULL)
	, m_bRun(FALSE)
	, m_hPropDlg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSequenceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSequenceDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FREE_RUN, &CSequenceDlg::OnBnClickedFreeRun)
	ON_BN_CLICKED(IDC_STOP, &CSequenceDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PROPERTY, &CSequenceDlg::OnBnClickedProperty)
	ON_BN_CLICKED(IDC_SEQUENCE_SAVE, &CSequenceDlg::OnBnClickedSequenceSave)
END_MESSAGE_MAP()


// CSequenceDlg message handlers

BOOL CSequenceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ShowWindow(SW_MAXIMIZE);

	// 初始化相机函数库
	MVInitLib();
	
	OpenCamera();

	m_dlgSeqCtrl.Create(IDD_SEQ_CTR,this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CSequenceDlg::DrawImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();
	const int xDest = 8;
	const int yDest = 40;

	CDC* pDC = GetDC();

	//1:1 显示全图
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 显示全图, zoom 50%
	//pDC->SetStretchBltMode(COLORONCOLOR);
	//m_image.Draw(pDC->GetSafeHdc(), xDest, yDest, w / 2, h / 2);

	//1:1 显示中间部分图
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);

	ReleaseDC(pDC);	
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.


void CSequenceDlg::OnPaint()
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
HCURSOR CSequenceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSequenceDlg::OpenCamera()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if( nCams == 0 )
	{
		MessageBox(_T("没有找到相机,请确认连接和相机IP设置"), _T("提示"), MB_ICONWARNING);
		return ;
	}

	//打开连接到电脑上的第一个相机
	MVSTATUS_CODES r = MVOpenCamByIndex(0,&m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox(_T("无法打开相机，可能正被别的软件控制"), _T("提示"), MB_ICONWARNING);
		else
			MessageBox(_T("无法打开相机"), _T("提示"), MB_ICONWARNING); 
		return;
	}

	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		//设置为连续非触发模式
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
	}

	GetDlgItem(IDC_PROPERTY)->EnableWindow(TRUE);
	GetDlgItem(IDC_SEQUENCE_SAVE)->EnableWindow(TRUE);

	if (m_hPropDlg == NULL)
	{
		//创建及初始化属性页对话框
		MVCamProptySheetInit(&m_hPropDlg, m_hCam, this, _T("相机属性"));
	}

	EnableButton();
}

void CSequenceDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_hPropDlg != NULL)
	{
		//销毁属性页对话框
		MVCamProptySheetDestroy(m_hPropDlg);
		m_hPropDlg = NULL;
	}

	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();
}


int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CSequenceDlg *pDlg = (CSequenceDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CSequenceDlg::OnBnClickedFreeRun()
{
	int w,h;			
	MV_PixelFormatEnums PixelFormat;
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&PixelFormat);

	//根据相机的宽、高、像素格式创建图像，用于接收和绘制相机采集到的图像
	m_image.CreateByPixelFormat(w,h,PixelFormat);

	//开始连续采集
	MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
	m_bRun = TRUE;

	EnableButton();
}


void CSequenceDlg::OnBnClickedStop()
{
	//停止连续采集
	MVStopGrab(m_hCam);
	m_bRun = FALSE;
	
	EnableButton();
}



int CSequenceDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	// 将新采集到的一帧图像信息pInfo转换为图像
	MVInfo2Image(m_hCam,pInfo,&m_image);

	// 显示图像
	DrawImage();

	char fname[MAX_PATH];
	if( m_dlgSeqCtrl.GetFileName(fname,sizeof(fname)) == 0 )
	{
		m_image.Save(fname);
	}

	return 0;
}


void CSequenceDlg::EnableButton()
{
	m_dlgSeqCtrl.CamRun(m_bRun);
	MVCamProptySheetCameraRun(m_hPropDlg, m_bRun? MVCameraRun_ON:MVCameraRun_OFF);
	GetDlgItem(IDC_FREE_RUN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_STOP)->EnableWindow(m_bRun);	
}



void CSequenceDlg::OnBnClickedProperty()
{
	//显示属性页对话框，非模式框
	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetShow(m_hPropDlg, SW_SHOW);
	}

}

void CSequenceDlg::OnBnClickedSequenceSave()
{
	m_dlgSeqCtrl.ShowWindow(SW_SHOW);
}
