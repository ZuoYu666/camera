// RecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Record.h"
#include "RecordDlg.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadRecord(LPVOID pParam)
{
	CRecordDlg *pDlg = (CRecordDlg*)pParam;
	if( pDlg != NULL )
	{
		pDlg->RecordProc();
	}
	return 0;
}


// CRecordDlg dialog
#define ID_TIME_REC				 3

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRecordDlg::IDD, pParent)
, m_hCam(NULL)
, m_bRun(FALSE)
, m_bRec(FALSE)
, m_bRecPause(FALSE)
, m_evFrame(NULL)
, m_evExit(NULL)
, m_hRecProc(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (!InitRecConfByFile(m_RecConf))
	{
		InitRecConf(m_RecConf);
	}

	QueryPerformanceFrequency( &liFrequency);
	m_dFrequency = (double)liFrequency.QuadPart;

	InitializeCriticalSection(&m_csRecord);
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_FREE_RUN, &CRecordDlg::OnBnClickedFreeRun)
	ON_BN_CLICKED(IDC_STOP, &CRecordDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RECORD, &CRecordDlg::OnBnClickedRecCtr)
	ON_BN_CLICKED(IDC_RECORD_CONF, &CRecordDlg::OnBnClickedRecordConf)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
// CRecordDlg message handlers

BOOL CRecordDlg::OnInitDialog()
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
	m_RecCtr.Create(IDD_REC_CTR,this);
	m_RecCtr.SetFileType("Avi");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRecordDlg::OnPaint()
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
HCURSOR CRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CRecordDlg::OpenCamera()
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

	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}

	EnableButton();
}

void CRecordDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if( m_hCam != NULL )
	{
		MVCloseCam(m_hCam);
	}

	MVTerminateLib();
}


int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CRecordDlg *pDlg = (CRecordDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CRecordDlg::OnBnClickedFreeRun()
{
	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);
	m_image.CreateByPixelFormat(w,h,m_PixelFormat);
	m_imageRec.CreateByPixelFormat(w,h,m_PixelFormat);

	//开始采集
	MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
	m_bRun = TRUE;
	EnableButton();
}



void CRecordDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	MVStopGrab(m_hCam);
	m_bRun = FALSE;
	m_RecCtr.OnBnClickedButtonRecordStop();
	if( m_bRec )
	{
		OnBnClickedRecordStop();
	}
	EnableButton();
}



int CRecordDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{
	MVInfo2Image(m_hCam,pInfo,&m_image);

	if (m_bRec&&!m_bRecPause)
	{
		static int nRecIndex = 0;
		if (m_bFirstRec)
		{
			nRecIndex = pInfo->nBlockId%m_RecConf.m_nInterval;
			m_tRecStart = time(NULL);
			m_bFirstRec = FALSE;
			QueryPerformanceCounter( &m_liRecStart);
		}
		BOOL bRec = TRUE;
		if( m_RecConf.m_nInterval > 1 )
		{
			if( pInfo->nBlockId%m_RecConf.m_nInterval != nRecIndex  )
			{
				bRec = FALSE;
			}
		}
		if(bRec)
		{
			EnterCriticalSection(&m_csRecord);
			memcpy(m_imageRec.GetBits(),m_image.GetBits(),m_image.GetPitch()*m_image.GetHeight());
			LeaveCriticalSection(&m_csRecord);
			SetEvent(m_evFrame);
		}
	}
	DrawImage();

	return 0;
}

void CRecordDlg::EnableButton()
{
	m_RecCtr.CamRun(m_bRun);
	GetDlgItem(IDC_FREE_RUN)->EnableWindow(!m_bRun);
	GetDlgItem(IDC_STOP)->EnableWindow(m_bRun);
}

void CRecordDlg::OnBnClickedRecord()
{
	CString strFileName = "";

	m_nRecCnt = 0;
	m_tRecElapse = 0;
	m_bFirstRec = TRUE;

	if( !m_RecConf.m_strPath.IsEmpty() )
	{
		strFileName = m_RecConf.m_strPath;

		int c = strFileName.GetLength();

		if( strFileName[c-1] != '\\' )
		{
			strFileName = strFileName+"\\";
		}
	}
	if( !m_RecConf.m_strPrefixName.IsEmpty() )
	{
		strFileName = strFileName + m_RecConf.m_strPrefixName;
	}
	if( m_RecConf.m_bAutoFileName )
	{

		if( m_RecConf.m_bIndex )
		{
			CString str;
			str.Format("_%04d",m_RecConf.m_nIndex++);
			strFileName = strFileName + str;
		}

		CString str;
		CTime theCurrentTime=CTime::GetCurrentTime(); 
		if( m_RecConf.m_nTimeStyle == 0 )
		{
			str = theCurrentTime.Format("_%H%M%S");
		}
		else if (m_RecConf.m_nTimeStyle == 1)
		{
			str = theCurrentTime.Format("_%Y-%m-%d-%H-%M-%S");
		}
		else
		{
			str = _T("");
		}

		strFileName = strFileName + str;
	}

	if( strFileName.IsEmpty() )
	{
		strFileName = "temp.avi";
	}
	else
	{
		int c = strFileName.GetLength();
		if( strFileName[c-1] == '\\' )
		{
			strFileName = strFileName + "temp";
		}

		strFileName = strFileName + ".avi";
	}

	double fps = 10;
	MVGetFrameRate(m_hCam,&fps);

	if( m_RecConf.m_nInterval != 0 )
	{
		fps /= m_RecConf.m_nInterval;
	}

	if( m_avi.Create(strFileName.LockBuffer(),fps) != 0 )
	{
		MessageBox("无法创建avi","错误",MB_ICONWARNING);
	}
	else
	{
		HRESULT hr = 0;
		if( m_RecConf.m_CompressOpt.fccHandler != 0 )
		{
			hr = m_avi.SetVideoCompression(&m_image,&(m_RecConf.m_CompressOpt),false,NULL);
		}
		else
		{
			hr = m_avi.SetVideoCompression(&m_image,NULL,false,NULL);			
		}

		if( hr != 0 )
		{
			MessageBox("无法设置压缩属性","错误",MB_ICONWARNING);
			m_avi.Close();
		}
		else
		{
			m_tRecStart = time(NULL);
			m_bRec = TRUE;

			GetDlgItem(IDC_RECORD_CONF)->EnableWindow(!m_bRec);
			m_evExit = CreateEvent(0,FALSE,FALSE,NULL);
			m_evFrame = CreateEvent(0,FALSE,FALSE,NULL);
			m_hRecProc = CreateThread(NULL,0,ThreadRecord,this,0,NULL);	
		}
	}

	m_RecCtr.SetFileName(strFileName);
	UpdateRecCtr();

	SetTimer(ID_TIME_REC,1000,NULL);
}

void CRecordDlg::OnBnClickedRecordStop()
{
	if( !m_bRec )
	{
		return ;
	}
	m_bRec = FALSE;
	m_bRecPause = FALSE;
	SetEvent(m_evExit);;
	WaitForSingleObject(m_hRecProc,3000);
	m_avi.Close();

	KillTimer(ID_TIME_REC);

	GetDlgItem(IDC_RECORD_CONF)->EnableWindow(!m_bRec);
}

void CRecordDlg::OnBnClickedRecordPause()
{
	m_bRecPause = !m_bRecPause;

	if (m_bRecPause)
	{
		KillTimer(ID_TIME_REC);
		QueryPerformanceCounter( &m_liRecStop);
	}
	else
	{
		long long tRecorded = m_liRecStop.QuadPart - m_liRecStart.QuadPart;
		QueryPerformanceCounter( &m_liRecStart);
		m_liRecStart.QuadPart -= tRecorded;
		SetTimer(ID_TIME_REC,1000,NULL);		
	}
}

void CRecordDlg::OnBnClickedRecordConf()
{
	CDlgRecConf dlg;
	dlg.SetData(m_RecConf);

	if (m_RecCtr.GetSafeHwnd()!=NULL)
	{
		m_RecCtr.EnableWindow(FALSE);
	}

	if( dlg.DoModal() == IDOK )
	{
		dlg.GetData(m_RecConf);
	}

	if (m_RecCtr.GetSafeHwnd()!=NULL)
	{
		m_RecCtr.EnableWindow(TRUE);
	}
}

void CRecordDlg::OnBnClickedRecCtr()
{
	UpdateRecCtr();
	m_RecCtr.ShowWindow( SW_SHOW );
}

void CRecordDlg::UpdateRecCtr()
{
	m_RecCtr.SetFileType("Avi");
	m_RecCtr.SetCodec(m_RecConf.m_CompressOpt);
	m_RecCtr.SetCapStep(m_RecConf.m_nInterval);
	m_RecCtr.SetRecFrame(0);
	if (m_RecConf.m_bAutoStopByFrame)
	{
		m_RecCtr.SetRecFrameLimit(m_RecConf.m_nAutoStopFrame);
	}
	else
	{
		m_RecCtr.SetRecFrameLimit(0);
	}
	m_RecCtr.SetRecTime(0);
	if (m_RecConf.m_bAutoStopByTime)
	{
		m_RecCtr.SetRecTimeLimit(m_RecConf.m_nAutoStopTime);
	}
	else
	{
		m_RecCtr.SetRecTimeLimit(0);
	}
}

void CRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	if( nIDEvent == ID_TIME_REC)
	{
		m_RecCtr.SetRecTime(m_tRecElapse);
		m_RecCtr.SetRecFrame(m_nRecCnt);
	}
	CDialog::OnTimer(nIDEvent);
}

int CRecordDlg::AppendFrame()
{
	MVImage image;
	image.Create(m_image.GetWidth(),m_image.GetHeight(),m_image.GetBPP());

	EnterCriticalSection(&m_csRecord);
	MVImageFlip(m_hCam,&m_imageRec,&image,FlipVertical);
	LeaveCriticalSection(&m_csRecord);
	
	if( m_avi.AddFrame(&image) == 0 )
	{
		int nSize;
		if (m_PixelFormat == PixelFormat_Mono8 || m_PixelFormat == PixelFormat_Mono8 )
		{
			nSize = image.GetWidth()*image.GetHeight();
		}
		else
		{
			nSize = image.GetWidth()*image.GetHeight()*3;
		}

		if( m_RecConf.m_bAutoStopByFileSize && (m_RecConf.m_nAutoStopFileSize>0))
		{
			unsigned long sz = m_avi.GetImageDataBytesWritten();

			if( sz > m_RecConf.m_nAutoStopFileSize*1024*1024 - nSize)
			{
				OnBnClickedRecordStop();
				m_RecCtr.EnableButton(FALSE);
			}
		}

		m_nRecCnt = m_avi.GetCountOfAddedImages();

		if(  m_RecConf.m_bAutoStopByFrame && (m_RecConf.m_nAutoStopFrame>0) )
		{
			if( m_nRecCnt >= m_RecConf.m_nAutoStopFrame )
			{
				OnBnClickedRecordStop();
				m_RecCtr.EnableButton(FALSE);
			}
		}


		QueryPerformanceCounter( &m_liRecStop);
		m_tRecElapse = (m_liRecStop.QuadPart - m_liRecStart.QuadPart)/m_dFrequency;

		if( m_RecConf.m_bAutoStopByTime && (m_RecConf.m_nAutoStopTime>0) )
		{
			if( m_tRecElapse >= m_RecConf.m_nAutoStopTime  )
			{
				OnBnClickedRecordStop();
				m_RecCtr.EnableButton(FALSE);
			}
		}
	}

	image.Destroy();

	return 0;
}

void CRecordDlg::DrawImage()
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

void CRecordDlg::RecordProc()
{
	HANDLE events[] = {m_evFrame,m_evExit};
	DWORD dwWait = 0;

	do
	{
		dwWait = WaitForMultipleObjects(2,events,FALSE,INFINITE);
		if( dwWait == WAIT_OBJECT_0)
		{
			AppendFrame();
		}
	}while( dwWait != (WAIT_OBJECT_0+1) );
}