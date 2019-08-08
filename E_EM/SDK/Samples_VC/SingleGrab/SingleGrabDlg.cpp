// GCapDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SingleGrab.h"
#include "SingleGrabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGCapDlg dialog
CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/) 
: CDialog(CGCapDlg::IDD, pParent)
, m_hCam(NULL)
, m_bRun(FALSE)
, m_hPropDlg(NULL)
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

	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FREE_RUN, &CGCapDlg::OnBnClickedFreeRun)
	ON_BN_CLICKED(IDC_STOP, &CGCapDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_SAVE, &CGCapDlg::OnBnClickedSave)

	ON_BN_CLICKED(IDC_PROPERTIES, &CGCapDlg::OnBnClickedProperties)
	ON_BN_CLICKED(IDC_SINGLE_GRAB, &CGCapDlg::OnBnClickedSingleGrab)
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

	// ��ʼ�����������
	MVInitLib();

	// �����
	OpenCamera();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGCapDlg::DrawImage()
{
	int w = m_image.GetWidth();
	int h = m_image.GetHeight();
	const int xDest = 8;
	const int yDest = 40;

	CDC* pDC = GetDC();

	//1:1 ��ʾȫͼ
	m_image.Draw(pDC->GetSafeHdc(),xDest,yDest);

	//1:2 ��ʾȫͼ, zoom 50%
	//pDC->SetStretchBltMode(COLORONCOLOR);
	//m_image.Draw(pDC->GetSafeHdc(), xDest, yDest, w / 2, h / 2);

	//1:1 ��ʾ�м䲿��ͼ
	//m_image.Draw(pDC->GetSafeHdc(),xDest,yDest,w/2,h/2,w/4,h/4,w/2,h/2);
	ReleaseDC(pDC);	
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
		if(!m_bRun && !m_image.IsNull())
		{
			DrawImage();
		}
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGCapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGCapDlg::OpenCamera()
{
	CWaitCursor cursor;

	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		AfxMessageBox(IDS_NO_CAM);
		return ;
	}

	//�����ӵ������ϵĵ�һ�����
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			AfxMessageBox(IDS_ERR_ACCESS_DENY);
		else
		{
			AfxMessageBox(IDS_ERR_OPEN);
		}			
		return;
	}

	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		//����Ϊ�����Ǵ���ģʽ
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
	}

	GetDlgItem(IDC_PROPERTIES)->EnableWindow(TRUE);
	GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);

	if (m_hPropDlg == NULL)
	{
		//��������ʼ������ҳ�Ի���
		CString strCaption(MAKEINTRESOURCE(IDS_PROPERTY));
		MVCamProptySheetInit(&m_hPropDlg, m_hCam, this,strCaption);
	}

	EnableButton();
}

void CGCapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_hPropDlg != NULL)
	{
		//��������ҳ�Ի���
		MVCamProptySheetDestroy(m_hPropDlg);
		m_hPropDlg = NULL;
	}

	if (m_hCam != NULL)
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();
}


int __stdcall StreamCB(MV_IMAGE_INFO* pInfo, ULONG_PTR nUserVal)
{
	CGCapDlg* pDlg = (CGCapDlg*)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void CGCapDlg::OnBnClickedFreeRun()
{
	int w, h;			
	MV_PixelFormatEnums PixelFormat;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &PixelFormat);

	//��������Ŀ��ߡ����ظ�ʽ����ͼ�����ڽ��պͻ��Ʋɼ�����ͼ��
	m_image.CreateByPixelFormat(w,h,PixelFormat);

	//��ʼ�����ɼ�
	MVStartGrab(m_hCam, StreamCB, (ULONG_PTR)this);
	m_bRun = TRUE;

	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetCameraRun(m_hPropDlg, MVCameraRun_ON);
	}
	EnableButton();
}

void CGCapDlg::OnBnClickedSingleGrab()
{
	int w, h;			
	MV_PixelFormatEnums PixelFormat;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &PixelFormat);

	//��������Ŀ��ߡ����ظ�ʽ����ͼ�����ڽ��պͻ��Ʋɼ�����ͼ��
	m_image.CreateByPixelFormat(w,h,PixelFormat);

	//��֡�ɼ�
	MVSTATUS_CODES r = MVSingleGrab(m_hCam, &m_image, 1000);
	if (r == MVST_SUCCESS)
	{
		DrawImage();
	}
	else
	{
		AfxMessageBox(IDS_ERR_SINGLE_GRAB);
	}
}

void CGCapDlg::OnBnClickedStop()
{
	//ֹͣ�����ɼ�
	MVStopGrab(m_hCam);
	m_bRun = FALSE;

	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetCameraRun(m_hPropDlg, MVCameraRun_OFF);
	}
	EnableButton();
}

void CGCapDlg::OnBnClickedSave()
{
	MVImage tImage;

	tImage.Create(m_image.GetWidth(), m_image.GetHeight(), m_image.GetBPP());
	memcpy(tImage.GetBits(), m_image.GetBits(), tImage.GetPitch() * tImage.GetHeight());

	CString strName(MAKEINTRESOURCE(IDS_UNNAMED));
	CFileDialog dlg(FALSE, _T("BMP"), strName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap (*.bmp)|*.bmp|Jpeg (*.jpg)|*.jpg|TIFF (*.tif)|*.tif|");
	if (dlg.DoModal() == IDOK)
	{
		tImage.Save(dlg.GetPathName());
	}
	tImage.Destroy();
}

void CGCapDlg::EnableButton()
{
	GetDlgItem(IDC_SINGLE_GRAB)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_FREE_RUN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_STOP)->EnableWindow(m_bRun);
}

int CGCapDlg::OnStreamCB(MV_IMAGE_INFO* pInfo)
{
	// ���²ɼ�����һ֡ͼ����ϢpInfoת��Ϊͼ��
	MVInfo2Image(m_hCam,pInfo,&m_image);

	// ��ʾͼ��
	DrawImage();

	return 0;
}

void CGCapDlg::OnBnClickedProperties()
{
	//��ʾ����ҳ�Ի��򣬷�ģʽ��
	if (m_hPropDlg != NULL)
	{
		MVCamProptySheetShow(m_hPropDlg, SW_SHOW);
	}
}
