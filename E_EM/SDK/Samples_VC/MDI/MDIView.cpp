// MDIView.cpp : CMDIView 类的实现
//

#include "stdafx.h"
#include "MDI.h"

#include "MDIDoc.h"
#include "MDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMDIView

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo,ULONG_PTR nUserVal)
{
	CMDIView *pView = (CMDIView *)nUserVal;
	return ( pView->OnStreamCB(pInfo) );
}

IMPLEMENT_DYNCREATE(CMDIView, CScrollView)

BEGIN_MESSAGE_MAP(CMDIView, CScrollView)
	ON_COMMAND(ID_CAPTURE, OnCapture)
	ON_COMMAND(ID_STOPCAP, OnStopCap)
	ON_WM_ACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_CAM_PROPERTY, OnCamProperty)
	ON_COMMAND(ID_FILE_SAVE, &CMDIView::OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE, &CMDIView::OnUpdateCapture)
	ON_UPDATE_COMMAND_UI(ID_STOPCAP, &CMDIView::OnUpdateStopcap)
END_MESSAGE_MAP()

// CMDIView 构造/析构

CMDIView::CMDIView()
{
	m_hCam = NULL;
	m_hProperty = NULL;
	m_bRun = FALSE;
}

CMDIView::~CMDIView()
{
	OnStopCap();

	if (m_hProperty!=NULL)
	{
		MVCamProptySheetDestroy(m_hProperty);
	}
	MVCloseCam(m_hCam);
	m_hCam = NULL;
}

BOOL CMDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CMDIView 绘制

void CMDIView::OnDraw(CDC* /*pDC*/)
{
	CMDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int yPos = GetScrollPos(SB_VERT);
	int xPos = GetScrollPos(SB_HORZ);

	if (!m_image.IsNull())
	{
		CRect rect;
		int nWidth, nHeight;

 		GetClientRect(&rect);
		nWidth = rect.Width();
		nHeight = rect.Height();
 
		CDC *pDC = GetDC();
		OnPrepareDC(pDC);

		m_image.Draw(pDC->GetSafeHdc(),xPos,yPos,m_image.GetWidth()-xPos,m_image.GetHeight()-yPos,
					 xPos,yPos,m_image.GetWidth()-xPos,m_image.GetHeight()-yPos);
		ReleaseDC(pDC);
	}
}

BOOL CMDIView::OpenCamera()
{
	m_nIdxCam = GetDocument()->m_nCamDevID;

	if (m_hCam == NULL)
	{
		MVSTATUS_CODES r = MVOpenCamByIndex(m_nIdxCam, &m_hCam);
		if (m_hCam == NULL)
		{
			if (r == MVST_ACCESS_DENIED)
				MessageBox("无法打开相机，可能正被别的软件控制", "提示", MB_ICONWARNING);
			else
				MessageBox("无法打开相机","提示",MB_ICONWARNING); 
			return FALSE;
		}

		CString strTemp;
		char buf[256];
		int szBuf = sizeof(buf);
		MVGetDeviceModelName(m_hCam,buf,&szBuf);
		strTemp = buf;
		szBuf = sizeof(buf);
		MVGetDeviceDeviceID(m_hCam,buf,&szBuf);
		strTemp += "_";
		strTemp += buf;
		GetDocument()->SetPathName(strTemp,FALSE);
	}

	if (m_hProperty==NULL)
	{
		MVCamInfo CamInfo;
		int r = MVGetCameraInfo(m_nIdxCam,&CamInfo);
		CString str;
		str = CamInfo.mModelName;
		str += _T(" ");
		str += CamInfo.mSerialNumber;
		str += _T(" 属性");
		MVCamProptySheetInit(&m_hProperty, m_hCam, this, str);
	}

	return TRUE;
}

void CMDIView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	OpenCamera();

	CSize sizeTotal;
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1536;

	CSize sizePage,sizeLine;
	sizePage.cx =sizeTotal.cx /2;
	sizePage.cy =sizeTotal.cy /2;
	sizeLine.cx =sizeTotal.cx /50;
	sizeLine.cy =sizeTotal.cy /50;
	SetScrollSizes(MM_TEXT, sizeTotal,sizePage,sizeLine);
}


#ifdef _DEBUG
CMDIDoc* CMDIView::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMDIDoc)));
	return (CMDIDoc*)m_pDocument;
}
#endif //_DEBUG

void CMDIView::OnCapture()
{
	if( !m_bRun )
	{
		DWORD ret = 0;

		CSize sizeTotal;

		MVGetWidth(m_hCam, (int*)&sizeTotal.cx);
		MVGetHeight(m_hCam, (int*)&sizeTotal.cy);
		MVGetPixelFormat(m_hCam,&m_PixelFormat);
		m_image.CreateByPixelFormat(sizeTotal.cx,sizeTotal.cy,m_PixelFormat);

		CSize sizePage,sizeLine;
		sizePage.cx =sizeTotal.cx /2;
		sizePage.cy =sizeTotal.cy /2;
		sizeLine.cx =sizeTotal.cx /50;
		sizeLine.cy =sizeTotal.cy /50;
		SetScrollSizes(MM_TEXT, sizeTotal,sizePage,sizeLine);
		MVStartGrab(m_hCam,StreamCB,(ULONG_PTR)this);
		MVCamProptySheetCameraRun(m_hProperty,MVCameraRun_ON);
		Invalidate(TRUE);
		m_bRun = TRUE;
	}
}

void CMDIView::OnStopCap()
{
	if( m_bRun )
	{
		MVStopGrab(m_hCam);
		MVCamProptySheetCameraRun(m_hProperty,MVCameraRun_OFF);
		m_bRun = FALSE;
	}
}

void CMDIView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CScrollView::OnActivate(nState, pWndOther, bMinimized);
}

void CMDIView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CMDIView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

int CMDIView::OnStreamCB( MV_IMAGE_INFO *pInfo )
{
	MVInfo2Image(m_hCam, pInfo, &m_image);
	Invalidate(FALSE);
	return 0;
}


void CMDIView::OnCamProperty()
{
	MVCamProptySheetShow(m_hProperty);
}

BOOL CMDIView::OnEraseBkgnd( CDC* pDC )
{
	RECT rect; 
	GetClientRect(&rect);
	int x; 
	int y; 
	if (m_image.IsNull())
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = m_image.GetWidth(); 
		y = m_image.GetHeight(); 
	}

	if(rect.right > x) 
	{ 
		CBrush br(RGB(192,192,192)); 
		RECT rectRight = rect; 
		rectRight.left = x; 
		rectRight.bottom = y; 
		pDC->FillRect(&rectRight, &br); 
	} 
	if(rect.bottom > y) 
	{ 
		CBrush br(RGB(192,192,192)); 
		RECT rectBottom = rect; 
		rectBottom.top = y; 
		pDC->FillRect(&rectBottom, &br); 
	} 

	return TRUE; 
}

void CMDIView::OnFileSave()
{
	MVImage img;
	img.Create(m_image.GetWidth(),m_image.GetHeight(),m_image.GetBPP());
	memcpy(img.GetBits(),m_image.GetBits(),img.GetPitch()*img.GetHeight());

	CFileDialog dlg(FALSE, ".bmp", "未命名", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bitmap (*.bmp)|*.bmp|Jpeg (*.jpg)|");
	if (dlg.DoModal() == IDOK)
	{
		img.Save(dlg.GetPathName());
	}
}

void CMDIView::OnUpdateCapture(CCmdUI *pCmdUI)
{	
	pCmdUI->Enable(!m_bRun);
}

void CMDIView::OnUpdateStopcap(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bRun);
}
