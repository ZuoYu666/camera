// GCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GCap.h"
#include "GCapDlg.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ThreadRecord(LPVOID pParam)
{
	CGCapDlg *pDlg = (CGCapDlg*)pParam;

	pDlg->RecordProc();

	return 0;
}


// CGCapDlg dialog
#define  UPDATE_FRAMERATE WM_USER+1
#define noID CIniFile::noID
CGCapDlg::CGCapDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGCapDlg::IDD, pParent)
, m_hCam(NULL)
, m_hProperty(NULL)
, m_nFrameCounter(0)
, m_fTriggerFps(5)
, m_nFlipRotate(0)
, m_bStarted(FALSE)
, m_bRGBToGray(FALSE)
, m_bRec(FALSE)
, m_bFirstRec(FALSE)
, m_bRecPause(FALSE)
, m_bCross(FALSE)
, m_evRecFrame(NULL)
, m_evRecExit(NULL)
, m_hRecProc(NULL)
, m_bDenoise(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	InitializeCriticalSection(&m_csSaveImage);
	InitializeCriticalSection(&m_csRecord);
	QueryPerformanceFrequency( &liFrequency);
	m_dFrequency = (double)liFrequency.QuadPart;
	m_bContinue = FALSE;
	m_bOpen = FALSE;
	m_nMax = 0;
	m_startMode = Stop_Mode;
	m_bSequenceStarted = FALSE;

	m_bSeqAll = FALSE;
	m_nIndex = 0;
	m_nSeqStep = 0;
	m_bSaving = FALSE;
	m_strImageSaveName = "";
	m_nNextImageDelay = 0;
	if (!InitRecConfByFile(m_RecConf))
	{
		InitRecConf(m_RecConf);
	}
	if (!InitByFile(m_SeqConf))
	{
		InitSeqConf(m_SeqConf);
	}
}

void CGCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PICTURESHOW, m_stcPic);
	DDX_Control(pDX, IDC_CMB_ZOOM, m_cmbPicSize);
	DDX_Control(pDX, IDC_COMBO_CMA_ID, m_cmbCamId);
	DDX_Control(pDX, IDC_CMB_FLIP_ROTATE,m_cmbFlipRotate);
	DDX_Check(pDX, IDC_CHECK_RGB_TO_GRAY, m_bRGBToGray);
	DDX_Text(pDX, IDC_EDIT_TRIGGERFPS, m_fTriggerFps);
	DDX_Check(pDX, IDC_CENTRE_CROSS, m_bCross);
	DDX_Check(pDX, IDC_DENOISE, m_bDenoise);
}

BEGIN_MESSAGE_MAP(CGCapDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PROPERTY_SET, &CGCapDlg::OnBnClickedPropertySet)
	ON_BN_CLICKED(IDC_STOP, &CGCapDlg::OnBnClickedStop)
	ON_CBN_SELCHANGE(IDC_CMB_ZOOM, &CGCapDlg::OnCbnSelchangeCmbZoom)
	ON_WM_SIZE()
	ON_MESSAGE(UPDATE_FRAMERATE,OnUpdateFramerate)
	ON_MESSAGE(WM_SCROLL_ZOOM_WIN_MOUSE_MOVE,OnScrollZoomWinMouseMove)
	ON_EN_KILLFOCUS(IDC_EDIT_TRIGGERFPS, &CGCapDlg::OnEnKillfocusEditTriggertfps)
	ON_BN_CLICKED(IDC_START_FREE_RUN, &CGCapDlg::OnBnClickedStartFreeRun)
	ON_BN_CLICKED(IDC_START_CONTINUE_TRIGGER, &CGCapDlg::OnBnClickedStartContinueTrigger)
	ON_BN_CLICKED(IDC_START_SINGLE_TRIGGER, &CGCapDlg::OnBnClickedStartSingleTrigger)
	ON_BN_CLICKED(IDC_START_SYNC_TRIGGER, &CGCapDlg::OnBnClickedStartSyncTrigger)
	ON_BN_CLICKED(IDC_SAVE_IMAGE, &CGCapDlg::OnBnClickedSaveImage)
	ON_BN_CLICKED(IDC_OPEN, &CGCapDlg::OnBnClickedOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_CMA_ID, &CGCapDlg::OnCbnSelchangeComboCmaId)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_RGB_TO_GRAY, &CGCapDlg::OnBnClickedCheckRgbToGray)
	ON_BN_CLICKED(IDC_SEQUENCE_CONFIG, &CGCapDlg::OnBnClickedSequence)
	ON_BN_CLICKED(IDC_SEQUENCE_TIMER, &CGCapDlg::OnBnClickedSeqCtr)
	ON_BN_CLICKED(IDC_SEQUENCE_CAP, &CGCapDlg::OnBnClickedSequenceCapture)
	ON_BN_CLICKED(IDC_RECORD, &CGCapDlg::OnBnClickedRecCtr)
	ON_BN_CLICKED(IDC_RECORD_CONF, &CGCapDlg::OnBnClickedRecordConf)

	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipText )
	ON_CBN_SELCHANGE(IDC_CMB_FLIP_ROTATE, &CGCapDlg::OnCbnSelchangeCmbFlipRotate)
	ON_BN_CLICKED(IDC_CENTRE_CROSS, &CGCapDlg::OnBnClickedCross)
	ON_WM_CLOSE()
	ON_MESSAGE(EDIT_RETURN,OnEditReturn)
	ON_BN_CLICKED(IDC_DENOISE, &CGCapDlg::OnBnClickedDenoise)
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
	MVUpdateCameraList();
	m_toolBar.Create(this);
	m_toolBar.LoadToolBar(IDR_TOOLBAR1);

	m_bmToolbarHi.LoadBitmap( IDB_MAIN );
	m_toolBar.SetBitmap( (HBITMAP)m_bmToolbarHi );

	m_toolList.Create(25, 25, ILC_COLOR24|ILC_MASK, 18, 1);

	// Load the list from resource, and select the transperency color

	CBitmap bmpFaces;
	bmpFaces.LoadBitmap(IDB_MAIN);
	m_toolList.Add(&bmpFaces, RGB(255, 255, 255));

	// Finally, set the new image list to the toolbar
	m_toolBar.GetToolBarCtrl().SetImageList(&m_toolList);

	m_toolBar.ShowWindow(SW_SHOW);
	m_toolBar.SetBarStyle(CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_toolBar.ModifyStyle(0, 0x0800);
	RECT rect = {0,0,450,40};
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0,0,NULL,&rect); 

	EnableButton(FALSE);

	UpdateId();

	m_nIndex = m_SeqConf.m_nIndex;
	m_nSeqStep = 60000*m_SeqConf.m_nMinute + 1000*m_SeqConf.m_nSecond;
	m_nNextImageDelay = m_nSeqStep/1000;
	m_strImageSaveName = m_SeqConf.m_strPath+m_SeqConf.m_strPrefixName;
	m_cmbFlipRotate.SetCurSel(0);
	m_cmbPicSize.SetCurSel(2);

	m_SeqCtr.Create(IDD_SEQ_CTR,this);
	m_SeqCtr.SetTimeSpace(m_nNextImageDelay);

	m_RecCtr.Create(IDD_REC_CTR,this);
	m_RecCtr.SetFileType("Avi");

	m_EditTriggerFps.SubclassWindow(::GetDlgItem(m_hWnd,IDC_EDIT_TRIGGERFPS));
	m_EditTriggerFps.SetAcceptType(Type_Float);

	LoadINIFile(FILE_NAME_MAIN);
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
		m_stcPic.InvalidateBg();
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
	// TODO: Add your control notification handler code here
	CWaitCursor cursor;
	CString str,strw;
	int nCam;
	MVSTATUS_CODES r;

	nCam = m_cmbCamId.GetCurSel();
	if (m_bOpen == FALSE)
	{
		str.LoadString(IDS_STRING_OPEN_FAILED);
		strw.LoadString(IDS_STRING_WARNING);
		r = MVOpenCamByIndex(nCam,&m_hCam);
		if( m_hCam == NULL )
		{
			if( r == MVST_ACCESS_DENIED )
			{
				CString strA;
				strA.LoadString(IDS_STRING_ACCESS_DENIED);
				str += _T(" ") + strA;
			}
			MessageBox(str,strw,MB_ICONWARNING);
			return ;
		}


		if (m_hProperty != NULL)
		{
			MVCamProptySheetDestroy(m_hProperty);
		}
			str.LoadString(IDS_STRING_PROPERTY_SET);
			MVCamProptySheetCreate(&m_hProperty, m_hCam, this, str);

		m_bOpen = TRUE;
		MVGetPixelFormat(m_hCam, &m_PixelFormat);
		if (m_PixelFormat == PixelFormat_Mono8)
		{
			m_bRGBToGray = FALSE;
			UpdateData(FALSE);
		}
		EnableButton(m_bStarted);
		MVSetStrobeSource(m_hCam,LineSource_ExposureActive);

		TBBUTTONINFO tbbi;
		memset(&tbbi,0,sizeof(tbbi));
		tbbi.dwMask = TBIF_IMAGE;
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		BOOL r = m_toolBar.GetToolBarCtrl().GetButtonInfo(IDC_OPEN,&tbbi );
		tbbi.iImage = 19;
		m_toolBar.GetToolBarCtrl().SetButtonInfo(IDC_OPEN,&tbbi );
	}
	else
	{
		if (m_startMode != Stop_Mode)
		{
			OnBnClickedStop();
		}
	
		m_bStarted = FALSE;
		m_bOpen = FALSE;
		str.LoadString(IDS_STRING_CONNECT);

		EnableButton(m_bStarted);
		MVCamProptySheetShow(m_hProperty,SW_HIDE);
		MVCamProptySheetDestroy(m_hProperty);
		m_hProperty = NULL;

		TBBUTTONINFO tbbi;
		memset(&tbbi,0,sizeof(tbbi));
		tbbi.dwMask = TBIF_IMAGE;
		tbbi.cbSize = sizeof(TBBUTTONINFO);
		BOOL r = m_toolBar.GetToolBarCtrl().GetButtonInfo(IDC_OPEN,&tbbi );
		tbbi.iImage = 0;
		m_toolBar.GetToolBarCtrl().SetButtonInfo(IDC_OPEN,&tbbi );
		MVCloseCam(m_hCam);
		m_hCam = NULL;
	}

	m_toolBar.GetToolBarCtrl().EnableButton(IDC_PROPERTY_SET,m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_COMBO_CMA_ID,!m_bOpen);
	GetDlgItem(IDC_COMBO_CMA_ID)->EnableWindow(!m_bOpen);
}

void CGCapDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (m_RecCtr.GetSafeHwnd())
	{
		m_RecCtr.DestroyWindow();
	}

	if( m_hCam != NULL )
	{
		MVStopGrab(m_hCam);
		MVSetTriggerMode(m_hCam,TriggerMode_Off);
		MVCloseCam(m_hCam);
	}
	if (m_hProperty != NULL)
	{
		MVCamProptySheetDestroy(m_hProperty);
		m_hProperty = NULL;
	}

	MVTerminateLib();
}


BOOL CGCapDlg::IsStart()
{
	return m_bStarted;
}

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CGCapDlg *pDlg = (CGCapDlg *)nUserVal;
	return ( pDlg->OnStreamCB(pInfo) );
}

void CGCapDlg::Start(StartMode mode)
{
	TriggerModeEnums TriggerMode1, TriggerMode2;
	CreateImage();
	MVGetTriggerMode(m_hCam, &TriggerMode2);
	switch(mode)
	{
	case Start_Free_Run:
		MVSetTriggerMode(m_hCam,TriggerMode_Off);
		break;
	case Start_Continue_Trigger:
	case Start_Single_Trigger:
		MVSetTriggerMode(m_hCam,TriggerMode_On);
		MVSetTriggerSource(m_hCam, TriggerSource_Software);
		break;
	case Start_Sync_Trigger:
		MVSetTriggerMode(m_hCam,TriggerMode_On);
		MVSetTriggerSource(m_hCam, TriggerSource_Line1);
		break;
	}
	MVGetTriggerMode(m_hCam,&TriggerMode1);
	if (TriggerMode1 != TriggerMode2)
	{
		double fpsMax,fpsMin;
		MVGetFrameRateRange(m_hCam, &fpsMin,&fpsMax);
		Sleep(1500/fpsMax);
	}
	MVSTATUS_CODES r = MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
	if(r != MVST_SUCCESS)
	{
		CString str;
		str.LoadString(IDS_STRING_START_FAILED);
		AfxMessageBox(str);
		return ;
	}
	m_bStarted = TRUE;
	m_nFrameCounter = 0;
	m_startMode = mode;
	EnableButton(m_bStarted);
}

void CGCapDlg::OnBnClickedStartFreeRun()
{
	Start(Start_Free_Run);
}

void CGCapDlg::OnBnClickedStartContinueTrigger()
{	
	Start(Start_Continue_Trigger);
	SetTimer(CONTINOUSE_TRIGGER_EVENT,(int)(1000/m_fTriggerFps),NULL);	
}

void CGCapDlg::OnBnClickedStartSingleTrigger()
{
	if (m_startMode!=Start_Single_Trigger)
	{
		Start(Start_Single_Trigger);
	}
	MVTriggerSoftware(m_hCam);
}

void CGCapDlg::OnBnClickedStartSyncTrigger()
{
	Start(Start_Sync_Trigger);
}

void CGCapDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	MVStopGrab(m_hCam);

	if(m_startMode = Start_Continue_Trigger)
	{
		KillTimer(CONTINOUSE_TRIGGER_EVENT);
	}

	m_SeqCtr.OnBnClickedButtonSequenceStop();
	m_RecCtr.OnBnClickedButtonRecordStop();

	if( m_bRec )
	{
		OnBnClickedRecordStop();
	}

	m_startMode = Stop_Mode;
	m_nMax = 0;
	m_bStarted = FALSE;
	EnableButton(m_bStarted);
}

int CGCapDlg::OnStreamCB( MV_IMAGE_INFO *pInfo)
{

	int w = m_image.GetWidth();
	int h = m_image.GetHeight();

	m_stcPic.BeginSetImage();
	EnterCriticalSection(&m_csSaveImage);
	MVInfo2Image(m_hCam, pInfo, &m_image);
	//MVInfo2ImageEx(m_hCam, pInfo, &m_image);
	ImageProcess(&m_image);
	LeaveCriticalSection(&m_csSaveImage);
	m_stcPic.EndSetImage();


	if (m_stcPic.GetSafeHwnd())
	{
		CPoint point; 
		::GetCursorPos(&point);

		m_stcPic.ScreenToClient(&point);
		::PostMessage(this->GetSafeHwnd(),WM_SCROLL_ZOOM_WIN_MOUSE_MOVE,point.x,point.y);

	}
	m_stcPic.SetImage(m_pImage);

	if (m_bSaving||m_bSeqAll)
	{
		SaveImage();
		m_bSaving = FALSE;
	}

	if (m_bRec)
	{
		static int nRecIndex = 0;
		if (m_bFirstRec)
		{
			nRecIndex = m_nFrameCounter%m_RecConf.m_nInterval;
			m_tRecStart = time(NULL);
			m_bFirstRec = FALSE;
			QueryPerformanceCounter( &m_liRecStart);
		}
		BOOL bRec = TRUE;
		if( m_RecConf.m_nInterval > 1 )
		{
			if( m_nFrameCounter%m_RecConf.m_nInterval != nRecIndex )
			{
				bRec = FALSE;
			}
		}
		if(bRec)
		{
			EnterCriticalSection(&m_csRecord);
			memcpy(m_imageRecord.GetBits(), m_pImage->GetBits(), m_pImage->GetPitch()*m_pImage->GetHeight());
			SetEvent(m_evRecFrame);
			LeaveCriticalSection(&m_csRecord);
		}
	}

	if( ((++m_nFrameCounter)%20==0) && (m_startMode == Start_Free_Run || m_startMode == Start_Continue_Trigger || Start_Sync_Trigger) )
	{
		QueryPerformanceCounter( &m_liCounterStop);
		m_dTime = (m_liCounterStop.QuadPart - m_liCounterStart.QuadPart)/m_dFrequency;
		m_dTimePerFrame = m_dTime/20.0;
		int nFrameRate =(INT)(1000.0/m_dTimePerFrame);

		//a thread must not have direct access to dialog items. So we have to make an
		//indirection with the message. We send the frame rate as wParam
		::PostMessage(m_hWnd,UPDATE_FRAMERATE, nFrameRate,0);

		QueryPerformanceCounter( &m_liCounterStart);
	}

	return 0;
}

void CGCapDlg::OnBnClickedPropertySet()
{
	// TODO: Add your control notification handler code here
	MVCamProptySheetShow(m_hProperty);
}

void CGCapDlg::OnBnClickedSaveImage()
{
	MVImage tImage;
	CString str;
	if (m_image.IsNull())
	{
		str.LoadString(IDS_STRING_NO_IMAGE);
		AfxMessageBox(str);
		return ;
	}
	EnterCriticalSection(&m_csSaveImage);
	tImage.Create(m_pImage->GetWidth(),m_pImage->GetHeight(),m_pImage->GetBPP());
	memcpy(tImage.GetBits(),m_pImage->GetBits(),tImage.GetPitch()*tImage.GetHeight());
	LeaveCriticalSection(&m_csSaveImage);

	str.LoadString(IDS_STRING_NO_NAME);
	CFileDialog dlg(FALSE,"BMP",str,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Bitmap (*.bmp)|*.bmp|Jpeg (*.jpg)|*.jpg|TIFF (*.tif)|*.tif|");

	if (dlg.DoModal() == IDOK)
	{
		str = dlg.GetPathName();
		tImage.Save(str);
	}
	tImage.Destroy();
}

void CGCapDlg::OnCbnSelchangeCmbZoom()
{
	switch(m_cmbPicSize.GetCurSel())
	{
	case 0:
		m_stcPic.SetZoom(Pic_Zoom_1_4);
		break;
	case 1:
		m_stcPic.SetZoom(Pic_Zoom_1_2);
		break;
	case 2:
		m_stcPic.SetZoom(Pic_Zoom_1_1);
		break;
	case 3:
		m_stcPic.SetZoom(Pic_Zoom_2_1);
		break;
	case 4:
		m_stcPic.SetZoom(Pic_Zoom_Fit);
		break;
	}
}

void CGCapDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CWnd *pWnd = GetDlgItem(IDC_PICTURESHOW);
	if (pWnd != NULL)
	{	
		CRect rect;
		GetClientRect(&rect);
		m_stcPic.MoveWindow(8,40,(rect.Width()-16),rect.Height()-56,TRUE);
	}
}


LRESULT CGCapDlg::OnUpdateFramerate( WPARAM wParam, LPARAM lParam )
{
	CString strFramerate;
	strFramerate.Format("%.2f", (double)(wParam)/1000.0);

	SetDlgItemText(IDC_STATIC_FPS, strFramerate);

	return 0;
}

void CGCapDlg::DisplayPointAndRGB( CPoint point )
{
	static BOOL bInImage = TRUE;
	if (m_stcPic.GetImage() != NULL && m_pImage != NULL && !m_pImage->IsNull())
	{	
		CString str;
		int xoff, yoff;
		double fZoom = m_stcPic.GetZoom();
		xoff = (point.x+m_stcPic.GetHPos())/fZoom;
		yoff = (point.y+m_stcPic.GetVPos())/fZoom;
		if (xoff>=0&&yoff>=0&&xoff < m_pImage->GetWidth()&& yoff < m_pImage->GetHeight())
		{
			BYTE* pdata = (BYTE*)m_pImage->GetBits();
			str.Format("XY %d,%d",xoff, yoff);
			GetDlgItem(IDC_STATIC_MOUSELOCATION)->SetWindowText(str);
			if (m_PixelFormat == PixelFormat_Mono8 || m_bRGBToGray)
			{
				str.Format("RGB %d,%d,%d",*(pdata+xoff+yoff*m_pImage->GetWidth()),*(pdata+xoff+yoff*m_pImage->GetWidth()),*(pdata+xoff+yoff*m_pImage->GetWidth()));
			}
			else
			{
				str.Format("RGB %d,%d,%d",*(pdata+(xoff+yoff*m_pImage->GetWidth())*3+2),*(pdata+(xoff+yoff*m_pImage->GetWidth())*3+1),*(pdata+(xoff+yoff*m_pImage->GetWidth())*3));
			}
			if(GetDlgItem(IDC_STATIC_RGB))
				GetDlgItem(IDC_STATIC_RGB)->SetWindowText(str);
			bInImage = TRUE;
		}
		else
		{
			if (bInImage == TRUE)
			{
				if (GetDlgItem(IDC_STATIC_MOUSELOCATION))
					GetDlgItem(IDC_STATIC_MOUSELOCATION)->SetWindowText("");
				if (GetDlgItem(IDC_STATIC_RGB))
					GetDlgItem(IDC_STATIC_RGB)->SetWindowText("");
				bInImage = FALSE;
			}
		}
	}
}

LRESULT CGCapDlg::OnScrollZoomWinMouseMove( WPARAM wParam, LPARAM lParam )
{
	CPoint point;
	point.x = wParam;
	point.y = lParam;
	DisplayPointAndRGB(point);
	return 0;
}

void CGCapDlg::OnEnKillfocusEditTriggertfps()
{
	// TODO: Add your control notification handler code here
	double fps = m_fTriggerFps;
	UpdateData(TRUE);
	if (m_fTriggerFps == 0)
	{
		m_fTriggerFps = fps;
		UpdateData(FALSE);
	}

	if (m_startMode == Start_Continue_Trigger)
	{
		KillTimer(CONTINOUSE_TRIGGER_EVENT);
		SetTimer(CONTINOUSE_TRIGGER_EVENT,1000/m_fTriggerFps,NULL);
	}
}

BOOL CGCapDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN) 
	{ 
		switch(pMsg->wParam)  
		{  
		case VK_RETURN:   //屏蔽Enter 

			if (pMsg->hwnd == this->GetSafeHwnd())
			{
				if(GetFocus() == GetDlgItem(IDC_EDIT_TRIGGERFPS))
				{
					OnEnKillfocusEditTriggertfps();
				}
				return TRUE;
			}
			else
			{
				break;
			}
		} 

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CGCapDlg::EnableButton( BOOL bCamStart )
{
	if (m_hProperty!=NULL)
	{
		if (bCamStart)
		{
			MVCamProptySheetCameraRun(m_hProperty, MVCameraRun_ON);
		}
		else
		{
			MVCamProptySheetCameraRun(m_hProperty, MVCameraRun_OFF);
		}
	}

	m_SeqCtr.CamRun(bCamStart);
	m_RecCtr.CamRun(bCamStart);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SAVE_IMAGE,m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_START_FREE_RUN,!bCamStart&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_STOP,bCamStart&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_PROPERTY_SET,m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_START_CONTINUE_TRIGGER,!bCamStart&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_START_SINGLE_TRIGGER,(!bCamStart||(m_startMode==Start_Single_Trigger))&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_START_SYNC_TRIGGER,!bCamStart&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SEQUENCE_TIMER,bCamStart&&m_bOpen);
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SEQUENCE_CAP,bCamStart&&m_bOpen);
	GetDlgItem(IDC_STATIC_FPS)->SetWindowText("0");
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_RECORD,!m_bRec&&m_bStarted);
}


int CGCapDlg::UpdateId()
{
	int nCams = 0;
	CWaitCursor cursor;
	CString str,strw;
	strw.LoadString(IDS_STRING_WARNING);
	MVSTATUS_CODES r = MVGetNumOfCameras(&nCams);

	if (r != MVST_SUCCESS)
	{
		str.LoadString(IDS_STRING_GETCAMERANUM);
		MessageBox(str,strw,MB_ICONWARNING);
		return r;
	}
	else if( nCams == 0 )
	{
		str.LoadString(IDS_STRING_GETCAMERANUM);
		MessageBox(str,strw,MB_ICONWARNING);
		return nCams;
	}

	for(int i=0; i<nCams; i++)
	{ 
		CString CamId;
		MVCamInfo CamInfo;
		r = MVGetCameraInfo(i,&CamInfo);
		CamId.Format("%s%s%s",CamInfo.mModelName,_T(" "), CamInfo.mSerialNumber);
		m_cmbCamId.AddString(CamId.LockBuffer());
	}
	m_cmbCamId.SetCurSel(0);
	m_cmbPicSize.SetCurSel(2);
	SetDlgTitle();
	GetDlgItem(IDC_CMB_ZOOM)->EnableWindow(TRUE);
	return nCams;
}


void CGCapDlg::OnCbnSelchangeComboCmaId()
{
	// TODO: Add your control notification handler code here
	m_stcPic.SetZoom(Pic_Zoom_1_1);
	m_cmbPicSize.SetCurSel(2);
	SetDlgTitle();
}

void CGCapDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == CONTINOUSE_TRIGGER_EVENT)
	{
		MVTriggerSoftware(m_hCam);
	}
	else if (nIDEvent == SEQUENCE_EVENT)
	{
		m_nNextImageDelay = m_nSeqStep/1000;
		m_SeqCtr.SetNextImageDelay(m_nNextImageDelay);
		m_bSaving = TRUE;
	}
	else if (nIDEvent == SEQUENCE_TIMER_EVENT)
	{
		m_nNextImageDelay -= 1;
		m_SeqCtr.SetNextImageDelay(m_nNextImageDelay);
	}
	else if( nIDEvent == ID_TIME_REC)
	{
		m_RecCtr.SetRecTime(m_tRecElapse);
		m_RecCtr.SetRecFrame(m_nRecCnt);
	}
	CDialog::OnTimer(nIDEvent);
}

void CGCapDlg::OnBnClickedCheckRgbToGray()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}



void CGCapDlg::CreateImage()
{
	DestroyImage();
	int w,h;			
	MVGetWidth(m_hCam,&w);
	MVGetHeight(m_hCam,&h);
	MVGetPixelFormat(m_hCam,&m_PixelFormat);

	if( m_PixelFormat == PixelFormat_Mono8 || m_PixelFormat == PixelFormat_Mono16 )
	{
		//m_image.Create(w,h,8);
		m_imageRotate.Create(h,w,8);
		GetDlgItem(IDC_CHECK_RGB_TO_GRAY)->EnableWindow(FALSE);
	}
	else
	{
		//m_image.Create(w,h,24);
		m_imageGray.Create(w,h,8);
		m_imageRotate.Create(h,w,24);
		m_imageRotateGray.Create(h,w,8);
		GetDlgItem(IDC_CHECK_RGB_TO_GRAY)->EnableWindow(TRUE);
	}
	m_image.CreateByPixelFormat(w,h,m_PixelFormat);

	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SAVE_IMAGE,TRUE);
}

void CGCapDlg::DestroyImage()
{
	if (!m_imageGray.IsNull())
	{
		m_imageGray.Destroy();
	}
	if (!m_image.IsNull())
	{
		m_image.Destroy();
	}

	if (!m_imageRotateGray.IsNull())
	{
		m_imageRotateGray.Destroy();
	}
	if (!m_imageRotate.IsNull())
	{
		m_imageRotate.Destroy();
	}

	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SAVE_IMAGE,FALSE);
}

void CGCapDlg::OnBnClickedSequence()
{
	// TODO: Add your control notification handler code here
	CSeqConfDlg dlg(m_SeqConf);
	if (m_SeqCtr.GetSafeHwnd()!=NULL)
	{
		m_SeqCtr.EnableWindow(FALSE);
	}
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetParam(m_SeqConf);
		m_nIndex = m_SeqConf.m_nIndex;
		m_nSeqStep = 60000*m_SeqConf.m_nMinute + 1000*m_SeqConf.m_nSecond;
		m_nNextImageDelay = m_nSeqStep/1000;
		m_SeqCtr.SetTimeSpace(m_nNextImageDelay);
		if(!m_SeqConf.m_strPath.IsEmpty())
		{
			m_strImageSaveName = m_SeqConf.m_strPath;

			int c = m_strImageSaveName.GetLength();

			if( m_strImageSaveName[c-1] != '\\' )
			{
				m_strImageSaveName += "\\";
			}
		}
		if (!m_SeqConf.m_strPrefixName.IsEmpty())
		{
			m_strImageSaveName = m_SeqConf.m_strPath+m_SeqConf.m_strPrefixName;
		}
	}
	if (m_SeqCtr.GetSafeHwnd()!=NULL)
	{
		m_SeqCtr.EnableWindow(TRUE);
	}
}
void CGCapDlg::OnBnClickedSequenceStart()
{
	if (m_nSeqStep!=0)
	{
		SetTimer(SEQUENCE_EVENT,m_nSeqStep,NULL);
		SetTimer(SEQUENCE_TIMER_EVENT,1000,NULL);
		m_nNextImageDelay = m_nSeqStep/1000;
		m_SeqCtr.SetNextImageDelay(m_nNextImageDelay);
	}
	else
	{
		m_bSeqAll = TRUE;
	}
	m_bSequenceStarted = TRUE;
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SEQUENCE_CONFIG,FALSE);
}

void CGCapDlg::OnBnClickedSequenceStop()
{
	if (m_nSeqStep!=0)
	{
		KillTimer(SEQUENCE_EVENT);
		KillTimer(SEQUENCE_TIMER_EVENT);
	}

	m_bSeqAll = FALSE;
	m_bSequenceStarted = FALSE;
	m_toolBar.GetToolBarCtrl().EnableButton(IDC_SEQUENCE_CONFIG,TRUE);


}

void CGCapDlg::OnBnClickedSeqCtr()
{
	m_SeqCtr.ShowWindow( SW_SHOW );

}

void CGCapDlg::OnBnClickedSequenceCapture()
{
	m_bSaving = TRUE;
}

void CGCapDlg::SaveImage()
{
	CWaitCursor cursor;

	MVImage tImage;
	EnterCriticalSection(&m_csSaveImage);
	tImage.Create(m_pImage->GetWidth(),m_pImage->GetHeight(),m_pImage->GetBPP());
	memcpy(tImage.GetBits(),m_pImage->GetBits(),tImage.GetPitch()*tImage.GetHeight());
	LeaveCriticalSection(&m_csSaveImage);
	CString str,strFinal;
	strFinal += m_strImageSaveName;

	CTime theCurrentTime=CTime::GetCurrentTime(); 
	switch(m_SeqConf.m_nTimeStyle)
	{
	case 0:
		str = theCurrentTime.Format("_%H-%M-%S");
		break;
	case 1:
		str = theCurrentTime.Format("_%Y-%m-%d-%H-%M-%S");
		break;
	default:
		str = _T("");
	}
	strFinal += str;
	if (m_SeqConf.m_bIndex)
	{
		str.Format(_T("_%04d"),m_nIndex);
		strFinal += str;
		m_nIndex += 1;
	}

	strFinal += m_SeqConf.m_strFileType;
	m_SeqCtr.SetFileName(strFinal);

	tImage.Save(strFinal);
	tImage.Destroy();
}

BOOL CGCapDlg::OnToolTipText( UINT, NMHDR* pNMHDR, LRESULT* pResult )
{
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	CString strTipText;
	CString strStatusText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0)
	{
		strTipText.LoadString(nID);
		int len =strTipText.Find('\n',0);
		strStatusText = strTipText.Left(len);
		strTipText = strTipText.Mid(len+1);

		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
		*pResult = 0;
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); 
		return TRUE;
	}
	return TRUE;
}

void CGCapDlg::OnCbnSelchangeCmbFlipRotate()
{
	EnterCriticalSection(&m_csSaveImage);
	m_nFlipRotate = m_cmbFlipRotate.GetCurSel();
	LeaveCriticalSection(&m_csSaveImage);
}

int CGCapDlg::Record()
{
	MVImage image;
	image.Create(m_imageRecord.GetWidth(),m_imageRecord.GetHeight(),m_imageRecord.GetBPP());

	EnterCriticalSection(&m_csRecord);
	MVImageFlip(m_hCam,&m_imageRecord,&image,FlipVertical);
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
void CGCapDlg::OnBnClickedRecord()
{
	CString strFileName = "";

	m_nRecCnt = 0;
	m_tRecElapse = 0;

	if( m_RecConf.m_bAutoFileName )
	{
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
		else if (m_RecConf.m_nTimeStyle == 1 )
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
	if( m_startMode == Start_Free_Run )
	{
		MVGetFrameRate(m_hCam,&fps);
	}
	else
	{
		fps = m_fTriggerFps;
	}

	if( m_RecConf.m_nInterval != 0 )
	{
		fps /= m_RecConf.m_nInterval;
	}

	if( m_avi.Create(strFileName.LockBuffer(),fps) != 0 )
	{
		AfxMessageBox(IDS_ERR_CREATE_AVI);
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
			AfxMessageBox(IDS_ERR_SET_COMPRESSION);
			m_avi.Close();
		}
		else
		{
			m_bRec = TRUE;
			m_bFirstRec = TRUE;
			m_toolBar.GetToolBarCtrl().EnableButton(IDC_RECORD_CONF,!m_bRec);
			m_imageRecord.Create(m_pImage->GetWidth(), m_pImage->GetHeight(), m_pImage->GetBPP());
			m_evRecExit = CreateEvent(NULL,FALSE,FALSE,"RecExit");
			m_evRecFrame = CreateEvent(NULL,FALSE,FALSE,"RecExit");
			m_hRecProc = CreateThread(NULL,0,ThreadRecord,this,0,NULL);
		}
	}

	m_RecCtr.SetFileName(strFileName);
	UpdateRecCtr();

	SetTimer(ID_TIME_REC,1000,NULL);
}

void CGCapDlg::OnBnClickedRecordStop()
{
	if( !m_bRec )
	{
		return ;
	}
	m_bRec = FALSE;
	m_bRecPause = FALSE;
	SetEvent(m_evRecExit);

	WaitForSingleObject(m_hRecProc,3000);
	m_avi.Close();

	CloseHandle(m_evRecFrame);
	CloseHandle(m_evRecExit);
	CloseHandle(m_hRecProc);


	KillTimer(ID_TIME_REC);

	m_toolBar.GetToolBarCtrl().EnableButton(IDC_RECORD_CONF,!m_bRec);
}

void CGCapDlg::OnBnClickedRecordPause()
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

void CGCapDlg::OnBnClickedRecordConf()
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

void CGCapDlg::OnBnClickedRecCtr()
{
	UpdateRecCtr();
	m_RecCtr.ShowWindow( SW_SHOW );
}

void CGCapDlg::SetDlgTitle()
{
	CString str;
	m_cmbCamId.GetWindowText(str);
	SetWindowText(str);
}

void CGCapDlg::UpdateRecCtr()
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
void CGCapDlg::OnBnClickedCross()
{
	UpdateData(TRUE);
	m_stcPic.DrawCross(m_bCross);
}

void CGCapDlg::OnCancel()
{
	CString str,strc;
	strc.LoadString(IDS_STRING_WARNING);
	str.LoadString(IDS_CONFIRM_EXIT);
	if (MessageBox(str,strc, MB_YESNO| MB_ICONQUESTION) == IDYES)
	{
		SaveINIFile();
		CDialog::OnCancel();
	}
}

BOOL CGCapDlg::LoadINIFile( CString strPathName /*= _T("")*/ )
{
	CIniFile IniFile;
	if (strPathName.IsEmpty())
	{
		TCHAR szFilePath[MAX_PATH + 1]; 
		GetModuleFileName(NULL, szFilePath, MAX_PATH); 
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
		CString str_url =  szFilePath;
		strPathName = FILE_NAME_MAIN;
		strPathName = str_url + strPathName;
	}
	IniFile.SetPath(strPathName.LockBuffer());
	IniFile.CaseSensitive();

	if (IniFile.ReadFile() == FALSE)
		return FALSE;

	if (IniFile.FindKey(KEY_MAIN) == noID)
		return FALSE;

	m_fTriggerFps = IniFile.GetValueF(KEY_MAIN, VALUE_FLOAT_TRIGGERFPS, 5);
	int nFlipRotate = IniFile.GetValueI(KEY_MAIN, VALUE_INTEGER_FLIPROTATE, 0);

	m_bRGBToGray  = IniFile.GetValueB(KEY_MAIN, VALUE_BOOLEAN_RGBTOGTY, FALSE);
	m_bCross	  = IniFile.GetValueB(KEY_MAIN, VALUE_BOOLEAN_CROSS, FALSE);
	int nZoom	  = IniFile.GetValueI(KEY_MAIN, VALUE_INTEGER_ZOOM, 2);
	UpdateData(FALSE);
	m_cmbFlipRotate.SetCurSel(nFlipRotate);
	OnCbnSelchangeCmbFlipRotate();
	OnBnClickedCross();
	m_cmbPicSize.SetCurSel(nZoom);
	OnCbnSelchangeCmbZoom();

	return TRUE;
}

BOOL CGCapDlg::SaveINIFile( CString strPathName /*= _T("")*/ )
{
	CIniFile IniFile;
	bool r = true;
	if (strPathName.IsEmpty())
	{
		TCHAR szFilePath[MAX_PATH + 1]; 
		GetModuleFileName(NULL, szFilePath, MAX_PATH); 
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0;//删除文件名，只获得路径
		CString str_url =  szFilePath;
		strPathName = FILE_NAME_MAIN;
		strPathName = str_url + strPathName;
	}
	IniFile.SetPath(strPathName.LockBuffer());
	IniFile.CaseSensitive();

	r |= IniFile.SetValueF(KEY_MAIN, VALUE_FLOAT_TRIGGERFPS, m_fTriggerFps);
	r |= IniFile.SetValueI(KEY_MAIN, VALUE_INTEGER_FLIPROTATE, m_nFlipRotate);
	r |= IniFile.SetValueB(KEY_MAIN, VALUE_BOOLEAN_RGBTOGTY, m_bRGBToGray);
	r |= IniFile.SetValueB(KEY_MAIN, VALUE_BOOLEAN_CROSS, m_bCross);
	r |= IniFile.SetValueI(KEY_MAIN, VALUE_INTEGER_ZOOM, m_cmbPicSize.GetCurSel());

	if (!r)
	{
		return FALSE;
	}

	if (IniFile.WriteFile() == false)
		return FALSE;
	return TRUE;
}


void CGCapDlg::ImageProcess( MVImage* pImage )
{
	if( pImage->GetBPP() == 8 )
	{
		if( m_bDenoise )
		{
			MVFilterAdaptiveMedian(m_hCam,pImage,40);
		}
	}

	if( m_nFlipRotate > 3 )
	{
		if( m_nFlipRotate == 4 )
		{
			MVImageRotate(m_hCam,&m_image,&m_imageRotate,Rotate90DegCw);
		}
		else if(  m_nFlipRotate == 5 )
		{
			MVImageRotate(m_hCam,&m_image,&m_imageRotate,Rotate90DegCcw);
		}

		if (m_bRGBToGray&&m_PixelFormat!=PixelFormat_Mono8)
		{
			MVImageBGRToGray(m_hCam,&m_imageRotate,&m_imageRotateGray);
			m_pImage = &m_imageRotateGray;
		}
		else
		{
			m_pImage = &m_imageRotate;
		}
	}
	else
	{
		if( m_nFlipRotate == 1 )
		{
			MVImageFlip(m_hCam,&m_image,&m_image,FlipHorizontal);
		}
		else if( m_nFlipRotate == 2 )
		{
			MVImageFlip(m_hCam,&m_image,&m_image,FlipVertical);
		}
		else if( m_nFlipRotate == 3 )
		{
			MVImageFlip(m_hCam,&m_image,&m_image,FlipBoth);
		}

		if (m_bRGBToGray&&m_PixelFormat!=PixelFormat_Mono8)
		{
			MVImageBGRToGray(m_hCam,&m_image,&m_imageGray);
			m_pImage = &m_imageGray;
		}
		else
		{
			m_pImage = &m_image;
		}
	}
}


void CGCapDlg::RecordProc()
{
	DWORD dwWait = 0;
	HANDLE hEvents[] = {m_evRecFrame,m_evRecExit};
	do{
		dwWait = WaitForMultipleObjects(2,hEvents,FALSE,INFINITE);
		if( dwWait == WAIT_OBJECT_0 )
		{
			Record();
		}		
	}while( dwWait != (WAIT_OBJECT_0+1) );
}

LRESULT CGCapDlg::OnEditReturn(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case IDC_EDIT_TRIGGERFPS:
		OnEnKillfocusEditTriggertfps();
		((CEdit*)GetDlgItem(IDC_EDIT_TRIGGERFPS))->SetSel(0,-1,FALSE);
		break;
	}

	return 0;
}
void CGCapDlg::OnBnClickedDenoise()
{
	UpdateData(TRUE);
}
