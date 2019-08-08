// ScrollZoomWin.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ScrollZoomWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollZoomWin

CScrollZoomWin::CScrollZoomWin()
{
	m_bmpWidth=1;
	m_bmpHeight=1;
	m_bmpWidthOld=0;
	m_bmpHeightOld=0;
	m_pictureZoom = Pic_Zoom_1_1;
	m_pImage = NULL;
	m_vsPos=0;
	m_hsPos=0;
	m_dZoom = 1;
	m_bCross = FALSE;
	m_bRedraw = FALSE;
	InitializeCriticalSection(&m_csImage);
}
//
CScrollZoomWin::~CScrollZoomWin()
{
		
}
//
BEGIN_MESSAGE_MAP(CScrollZoomWin, CStatic)
	//{{AFX_MSG_MAP(CScrollZoomWin)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollZoomWin message handlers

void CScrollZoomWin::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// bar height
	int sbHei=GetSystemMetrics(SM_CYVSCROLL);
	CRect rc;
	GetClientRect(rc);
	// Scrolling
	switch (nSBCode)
	{
	case SB_LINEUP:
	case SB_PAGEUP:
		m_vsPos -= 10;
	break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		m_vsPos += 10;
	break;
	case SB_THUMBTRACK:
		m_vsPos= nPos;
	break;
	default: return;
	}
//
	SCROLLINFO si;
	GetScrollInfo(SB_VERT,&si);

	m_vsPos=max(0,min(m_vsPos,m_bmpHeight-(int)si.nPage));
	if( m_vsPos != GetScrollPos(SB_VERT))
	{
		SetScrollPos(SB_VERT,m_vsPos);
		RedrawWindow();// not Invalidate();
	}

}

void CScrollZoomWin::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// bar width
	int sbWid=GetSystemMetrics(SM_CXVSCROLL);

	CRect rc;
	GetClientRect(rc);
	// Scrolling
	switch (nSBCode)
	{
	case SB_LINELEFT:
	case SB_PAGELEFT:
		m_hsPos -= 10;
	break;
	case SB_LINERIGHT:
	case SB_PAGERIGHT:
		m_hsPos += 10;
	break;
	case SB_THUMBTRACK:
		m_hsPos= nPos;
	break;
	default: return;
	}
//
	SCROLLINFO si;
	GetScrollInfo(SB_HORZ,&si);
	m_hsPos=max(0,min(m_hsPos,m_bmpWidth-(int)si.nPage));
 	if( m_hsPos != GetScrollPos(SB_HORZ))
 	{
 		SetScrollPos(SB_HORZ,m_hsPos);
 		RedrawWindow();// not Invalidate();
	}
}

//#define DRAW_ON_IMAGE

void CScrollZoomWin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
// 	if (m_bRedraw)
// 	{
// 		
// 		m_bRedraw = FALSE;
// 		return ;
// 	}
	if (m_pImage==NULL)
	{
		return ;
	}

	HGDIOBJ hOldPen;
	int xMdl,yMdl;

	EnterCriticalSection(&m_csImage);
	MVImage tImage;
	tImage.Create(m_pImage->GetWidth(),m_pImage->GetHeight(),m_pImage->GetBPP());
	memcpy(tImage.GetBits(),m_pImage->GetBits(),tImage.GetPitch()*tImage.GetHeight());
	LeaveCriticalSection(&m_csImage);

	SCROLLINFO si;
	GetScrollInfo(SB_HORZ,&si);
	int nwid = si.nPage-1;
	GetScrollInfo(SB_VERT,&si);
	int nhei = si.nPage-1;
	CPen penNew;

	HDC hdc = tImage.GetDC();

	if( !m_bCross )
	{
		if (m_pictureZoom == Pic_Zoom_Fit)
		{
			if( (m_dZoom>0.4 && m_dZoom<0.6) || (m_dZoom>0.23 && m_dZoom<0.27) || (m_dZoom>0.11 && m_dZoom<0.18) )
			{
				tImage.Smooth();
			}
			dc.SetStretchBltMode(COLORONCOLOR);
			StretchBlt(dc.GetSafeHdc(),0,0,m_bmpWidth,m_bmpHeight,hdc,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),SRCCOPY);
		}
		else if (m_pictureZoom == Pic_Zoom_1_1)
		{
			BitBlt(dc.GetSafeHdc(),0,0,nwid,nhei,hdc,m_hsPos,m_vsPos,SRCCOPY);
		}
		else
		{
			dc.SetStretchBltMode(COLORONCOLOR);
			StretchBlt(dc.GetSafeHdc(),0,0,nwid,nhei,hdc,m_hsPos/m_dZoom,m_vsPos/m_dZoom,nwid/m_dZoom,nhei/m_dZoom,SRCCOPY);
		}
		tImage.ReleaseDC();
		return ;
	}
#ifdef DRAW_ON_IMAGE
	penNew.CreatePen(PS_SOLID,1/m_dZoom+0.9,RGB(0,255,0));
	hOldPen = SelectObject(hdc,penNew);
	//int oldRop = SetROP2(hdc,R2_NOT);
	xMdl = m_pImage->GetWidth()/2;
	yMdl = m_pImage->GetHeight()/2;
	if (m_bCross)
	{
		MoveToEx(hdc,xMdl, 0,NULL);
		LineTo(hdc,xMdl, m_pImage->GetHeight());

		MoveToEx(hdc,0, yMdl,NULL);
		LineTo(hdc,m_pImage->GetWidth(), yMdl);
	}
	//SetROP2(hdc,oldRop);
	SelectObject(hdc,hOldPen);

	if (m_pictureZoom == Pic_Zoom_Fit)
	{
		dc.SetStretchBltMode(COLORONCOLOR);
		StretchBlt(dc.GetSafeHdc(),0,0,m_bmpWidth,m_bmpHeight,hdc,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),SRCCOPY);
	}
	else if (m_pictureZoom == Pic_Zoom_1_1)
	{
		BitBlt(dc.GetSafeHdc(),0,0,nwid,nhei,hdc,m_hsPos,m_vsPos,SRCCOPY);
	}
	else
	{
		dc.SetStretchBltMode(COLORONCOLOR);
		StretchBlt(dc.GetSafeHdc(),0,0,nwid,nhei,hdc,m_hsPos/m_dZoom,m_vsPos/m_dZoom,nwid/m_dZoom,nhei/m_dZoom,SRCCOPY);
	}
#else

	CDC MemDC;
	CBitmap  Bitmap;
	MemDC.CreateCompatibleDC(&dc);
	Bitmap.CreateCompatibleBitmap(&dc,nwid,nhei);
	MemDC.SelectObject(Bitmap);

	if (m_pictureZoom == Pic_Zoom_Fit)
	{
		MemDC.SetStretchBltMode(COLORONCOLOR);
		StretchBlt(MemDC.GetSafeHdc(),0,0,m_bmpWidth,m_bmpHeight,hdc ,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),SRCCOPY);
	}
	else if (m_pictureZoom == Pic_Zoom_1_1)
	{
		BitBlt(MemDC.GetSafeHdc(),0,0,nwid,nhei,hdc ,m_hsPos,m_vsPos,SRCCOPY);
	}
	else
	{
		MemDC.SetStretchBltMode(COLORONCOLOR);
		StretchBlt(MemDC.GetSafeHdc(),0,0,nwid,nhei,hdc ,m_hsPos/m_dZoom,m_vsPos/m_dZoom,nwid/m_dZoom,nhei/m_dZoom,SRCCOPY);
	}

	penNew.CreatePen(PS_SOLID,1,RGB(0,255,0));
	hOldPen = MemDC.SelectObject(penNew);
	xMdl = m_pImage->GetWidth()*m_dZoom/2 - m_hsPos;
	yMdl = m_pImage->GetHeight()*m_dZoom/2 - m_vsPos;
	if (m_bCross)
	{
		if (xMdl>0&&xMdl<nwid)
		{
			MemDC.MoveTo(xMdl, 0);
			MemDC.LineTo(xMdl, nhei);
		}
		if (yMdl>0&&yMdl<nhei)
		{
			MemDC.MoveTo(0, yMdl);
			MemDC.LineTo(nwid, yMdl);
		}
	}

	MemDC.SelectObject(hOldPen);

	dc.BitBlt(0,0,nwid,nhei,&MemDC,0,0,SRCCOPY);

	MemDC.DeleteDC();
	Bitmap.DeleteObject();
#endif

	tImage.ReleaseDC();
}

void CScrollZoomWin::PreSubclassWindow() 
{
	static int nBorder = -1;
	CRect rc;
	CRect rect;
	GetWindowRect(rect);
	GetClientRect(rc);
	
	//TRACE(_T("rc_width:%d bmpwidth:%d\n"),rc.right,m_bmpWidth);
	if (nBorder<0)
	{
 		nBorder = rect.right-rect.left-rc.right;
	}
	
	InitImageSize(&rc, &rect, nBorder);

	rect.right = rect.right-rect.left-nBorder;
	rect.bottom = rect.bottom-rect.top-nBorder;
	rect.left = 0;
	rect.top = 0;

// bar width
	int sbWid=GetSystemMetrics(SM_CXVSCROLL);
// bar height
	int sbHei=GetSystemMetrics(SM_CYVSCROLL);
//
	//判断是否有水平滚动条
	BOOL NoHSb = (m_bmpHeight <= rect.bottom && m_bmpWidth <= rect.right) || m_bmpWidth <= rect.right - sbWid;
	//判断是否有垂直滚动条
	BOOL NoVSb = (m_bmpHeight <= rect.bottom && m_bmpWidth <= rect.right) || m_bmpHeight <= rect.bottom - sbHei;
//
	SCROLLINFO si;

//============================================================================================//
	GetScrollInfo(SB_VERT,&si,SIF_POS);
	si.cbSize=sizeof(si);
	si.fMask=SIF_RANGE|SIF_PAGE;
	si.nMin=0;
	si.nMax=m_bmpHeight;

	//设置单幅页面高度
	//没有水平滚动条
	if(NoHSb)
	{
		if (m_bmpHeight==rect.Height())
		{
			si.nMax -= 1;
		}
		si.nPage=rect.Height();
		//滑块大小
		m_vsBoxSz=(si.nPage)*(si.nPage-sbWid)/m_bmpHeight;
	}
	//有水平滚动条
	else
	{
		if (m_bmpHeight==rect.Height()-sbWid)
		{
			si.nMax -= 1;
		}

		si.nPage=rect.Height()-sbWid;
		//滑块大小
		m_vsBoxSz=(si.nPage)*(si.nPage-2*sbWid)/m_bmpHeight;
	}

	//图片或页面变化时，重绘滑块位置
	if (NoVSb)
	{
		m_vsPos = 0;
	}
	else if (m_bmpHeight-(int)si.nPage-m_vsPos<0)
	{
		m_vsPos = m_bmpHeight - si.nPage;
	}

	SetScrollInfo(SB_VERT,&si,SIF_POS);
//
	m_VsbRc.SetRect(rect.right-sbWid,rect.top,rect.right,rect.bottom-sbHei);


//============================================================================================//
	GetScrollInfo(SB_HORZ,&si);
	si.nMax=m_bmpWidth;

	//设置单幅页面宽度
	//没有垂直滚动条
	if(NoVSb)
	{
		if (m_bmpWidth==rect.Width())
		{
			si.nMax -= 1;
		}
		si.nPage=rect.Width();
		//滑块大小
		m_hsBoxSz=(si.nPage)*(si.nPage-sbHei)/m_bmpWidth;
	}
	//有垂直滚动条	
	else
	{
		if (m_bmpWidth==rect.Width()-sbHei)
		{
			si.nMax -= 1;
		}

		si.nPage=rect.Width()-sbHei;
		//滑块大小
		m_hsBoxSz=(si.nPage)*(si.nPage-2*sbHei)/m_bmpWidth;
	}

	//图片或页面变化时，重绘滑块位置
	if (NoHSb)
	{
		m_hsPos = 0;
	}
	else if (m_bmpWidth-(int)si.nPage-m_hsPos<0)
	{
		m_hsPos = m_bmpWidth - si.nPage;
	}
	

	//TRACE(_T("nPage:%d\n\n"),si.nPage);
//
	SetScrollInfo(SB_HORZ,&si);
	m_HsbRc.SetRect(rect.left,rect.bottom-sbHei,rect.right-sbWid,rect.bottom);
	
	EnableScrollBar(SB_VERT,0);
	EnableScrollBar(SB_HORZ,0);
	InvalidateBg();
	CStatic::PreSubclassWindow();
}
//
LRESULT CScrollZoomWin::OnNcHitTest(CPoint point) 
{
	static CPoint lastPt;
	int sbCmd;
	ScreenToClient(&point);
	//TRACE(_T("point: x = %d, y = %d\n"),point.x,point.y);

	if(m_VsbRc.PtInRect(point))
	{
		//TRACE(_T("client: l=%d, r=%d, t=%d, b=%d\n"),m_VsbRc.left,m_VsbRc.right,m_VsbRc.top,m_VsbRc.bottom);
		int sbHei=GetSystemMetrics(SM_CYVSCROLL);
		int vsBox=m_vsPos;
		vsBox = vsBox*(m_VsbRc.Height()-2*sbHei)/m_bmpHeight+sbHei;
		//TRACE(_T("vsbox = %d\n"),vsBox);
		if      (point.y < sbHei)                      sbCmd=SB_LINEUP;
		else if (point.y < vsBox)					   sbCmd=SB_PAGEUP;
		else if (point.y < vsBox + m_vsBoxSz)		   sbCmd=SB_THUMBTRACK;
		else if (point.y < m_VsbRc.bottom - sbHei)     sbCmd=SB_PAGEDOWN;
		else if (point.y < m_VsbRc.bottom )            sbCmd=SB_LINEDOWN;

		while(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{//  get new cursor pos
			GetCursorPos(&point);
			ScreenToClient(&point);
			if(sbCmd == SB_THUMBTRACK)
			{// continue
				if(point.y != lastPt.y)
				{
					OnVScroll(sbCmd,(vsBox+point.y-lastPt.y)*m_bmpHeight/m_VsbRc.Height(),0);
				}
			}
			else if(sbCmd == SB_PAGEUP)
			{// 1 atfer 1
				int vsBoxReal=m_vsPos;
				vsBoxReal = vsBoxReal*(m_VsbRc.Height()-2*sbHei)/m_bmpHeight+sbHei;
				if (point.y < vsBoxReal)
				{
					OnVScroll(sbCmd,0,0);
					Sleep(150);
				}
			}
			else if(sbCmd == SB_PAGEDOWN)
			{// 1 atfer 1
				int vsBoxReal=m_vsPos;
				vsBoxReal = vsBoxReal*(m_VsbRc.Height()-2*sbHei)/m_bmpHeight+sbHei;
				if (point.y >= vsBoxReal + m_vsBoxSz)
				{
					OnVScroll(sbCmd,0,0);
					Sleep(150);
				}
			}
			else
			{
				OnVScroll(sbCmd,0,0);
				Sleep(150);
			}
		}// end while
	}// end if(m_VsbRc.PtInRect(point))
	else if(m_HsbRc.PtInRect(point))
	{
		//TRACE(_T("client: l=%d, r=%d, t=%d, b=%d\n"),m_HsbRc.left,m_HsbRc.right,m_HsbRc.top,m_HsbRc.bottom);
		int sbWid=GetSystemMetrics(SM_CXVSCROLL);
		int hsBox=m_hsPos;
		hsBox = hsBox*(m_HsbRc.Width()-2*sbWid)/m_bmpWidth+sbWid;
		//TRACE(_T("hsbox = %d\n"),hsBox);
		if      (point.x < sbWid)                      sbCmd=SB_LINELEFT;
		else if (point.x < hsBox)					   sbCmd=SB_PAGELEFT;
		else if (point.x < hsBox + m_hsBoxSz)		   sbCmd=SB_THUMBTRACK;
		else if (point.x < m_HsbRc.right - sbWid)      sbCmd=SB_PAGERIGHT;
		else if (point.x < m_HsbRc.right)              sbCmd=SB_LINERIGHT;

		while(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{//  get new cursor pos
			GetCursorPos(&point);
			ScreenToClient(&point);
			int hsBoxReal=m_hsPos;
			hsBoxReal = hsBoxReal*(m_HsbRc.Width()-2*sbWid)/m_bmpWidth+sbWid;
			if(sbCmd==SB_THUMBTRACK)
			{// continue
				if(point.x != lastPt.x)
				{
					OnHScroll(sbCmd,(hsBox+point.x-lastPt.x)*m_bmpWidth/m_HsbRc.Width(),0);
				}
			}
			else if(sbCmd == SB_PAGELEFT)
			{// 1 atfer 1

				if (point.x < hsBoxReal)
				{
					OnHScroll(sbCmd,0,0);
					Sleep(150);
				}
			}
			else if(sbCmd == SB_PAGERIGHT)
			{// 1 atfer 1
				if (point.x >= hsBoxReal + m_hsBoxSz)
				{
					OnHScroll(sbCmd,0,0);
					Sleep(150);
				}
			}
			else
			{
				OnHScroll(sbCmd,0,0);
				Sleep(150);
			}
		}// end while
	}
	else
	{
		::SendMessage(GetParent()->GetSafeHwnd(),WM_SCROLL_ZOOM_WIN_MOUSE_MOVE,point.x,point.y);
	}// end 
//
	lastPt=point;
	return HTNOWHERE;
}

void CScrollZoomWin::ImgShow()
{
//	Invalidate(TRUE);
}


void CScrollZoomWin::SetpictureZoom( PictureZoom pictureZoom )
{
	m_pictureZoom = pictureZoom;
}

void CScrollZoomWin::InitImageSize( const CRect *p_rcClient, const CRect *p_rcWindow, int nBorder )
{
	if (m_pImage != NULL)
	{
		if ( m_pictureZoom == Pic_Zoom_Fit )
		{
			double viewwidth = p_rcWindow->right - p_rcWindow->left - nBorder;
			double viewheight = p_rcWindow->bottom - p_rcWindow->top - nBorder;
			double wFit = (double)(m_pImage->GetWidth())/viewwidth;
			double hFit = (double)(m_pImage->GetHeight())/viewheight;
			if (wFit > hFit)
			{
				m_bmpWidth = p_rcWindow->right - p_rcWindow->left - nBorder;
				m_bmpHeight = m_pImage->GetHeight()/wFit;
				m_dZoom = (1.0/wFit);
			}
			else
			{
				m_bmpHeight = p_rcWindow->bottom - p_rcWindow->top - nBorder;
				m_bmpWidth = m_pImage->GetWidth()/hFit;
				m_dZoom = (1.0/hFit);
			}
		}
		else
		{
			m_bmpHeight = m_pImage->GetHeight()*m_dZoom;
			m_bmpWidth = m_pImage->GetWidth()*m_dZoom;
		}
	}
	else
	{
		m_bmpHeight = 1;
		m_bmpWidth = 1;
	}


}

void CScrollZoomWin::SetZoom( PictureZoom pictureZoom, double dFree )
{
	if (pictureZoom == Pic_Zoom_1_1 )
	{
		m_pictureZoom = Pic_Zoom_1_1;
		m_dZoom = 1;
	}
	else if (pictureZoom == Pic_Zoom_Fit)
	{
		m_pictureZoom = Pic_Zoom_Fit;
	}
	else if ( pictureZoom == Pic_Zoom_1_2 )
	{
		m_pictureZoom = Pic_Zoom_1_2;
		m_dZoom = 0.5;
	}
	else if ( pictureZoom == Pic_Zoom_1_4 )
	{
		m_pictureZoom = Pic_Zoom_1_4;
		m_dZoom = 1.0/4;
	}
	else if ( pictureZoom == Pic_Zoom_2_1 )
	{
		m_pictureZoom = Pic_Zoom_2_1;
		m_dZoom = 2.0;
	}
	else 
	{
		m_pictureZoom = Pic_Zoom_Free;
		if (dFree > 3.0)
		{
			CString str;
			str.LoadString(IDS_STRING_ZOOM_MAX);
			AfxMessageBox(str);
			m_dZoom = 3;
		}
		else if (dFree < 0.1)
		{
			CString str;
			str.LoadString(IDS_STRING_ZOOM_MIN);
			AfxMessageBox(str);
			m_dZoom = 0.1;
		}
		else
		{
			m_dZoom = dFree;
		}
	}

	PreSubclassWindow();
}

void CScrollZoomWin::SetImage( MVImage* pImg )
{
	BOOL bImageChange = FALSE;
	if (pImg != NULL && !pImg->IsNull())
	{
		if (m_pImage == NULL)
		{

			m_pImage = pImg;
			bImageChange = TRUE;
		}
		else
		{
			if (m_bmpWidthOld!=pImg->GetWidth() || m_bmpHeightOld!=pImg->GetHeight())
			{
				bImageChange = TRUE;
			}

			if(m_pImage != pImg)
			{
				m_pImage = pImg;
			}
		}
	}
	else
	{
		if (m_bmpWidthOld != 1 || m_bmpHeightOld != 1)
		{
			PreSubclassWindow();
			InvalidateBg();
			m_bmpWidthOld = 1;
			m_bmpHeightOld = 1;
		}
		return ;
	}

	if (bImageChange)
	{
		m_bmpWidthOld=pImg->GetWidth();
		m_bmpHeightOld=pImg->GetHeight();
		PreSubclassWindow();
		InvalidateBg();
	}

	Invalidate(TRUE);
}

void CScrollZoomWin::DrawCross( BOOL Cross )
{
	m_bCross = Cross;
}

double CScrollZoomWin::GetZoom()
{
	return m_dZoom;
}

int CScrollZoomWin::GetHPos()
{
	return m_hsPos;
}

int CScrollZoomWin::GetVPos()
{
	return m_vsPos;
}

MVImage* CScrollZoomWin::GetImage()
{
	return m_pImage;
}

PictureZoom CScrollZoomWin::GetpictureZoom()
{
	return m_pictureZoom;
}

void CScrollZoomWin::BeginSetImage()
{
	EnterCriticalSection(&m_csImage);
}

void CScrollZoomWin::EndSetImage()
{
	LeaveCriticalSection(&m_csImage);
}

void CScrollZoomWin::Redraw()
{
	m_bRedraw = TRUE;
	Invalidate(TRUE);
}

void CScrollZoomWin::MoveWindow( int x, int y, int nWidth, int nHeight, BOOL bRepaint/*=TRUE*/ )
{
	nWidth = nWidth>50 ? nWidth:50;
	nHeight = nHeight>50 ? nHeight:50;
	CWnd::MoveWindow(x, y, nWidth, nHeight, bRepaint);
	PreSubclassWindow();
}

void CScrollZoomWin::MoveWindow( LPCRECT lpRect, BOOL bRepaint/*=TRUE*/ )
{
	MoveWindow(lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint);
}

void CScrollZoomWin::InvalidateBg()
{
	CDC *pDC = GetDC();
	RECT rect; 
	GetClientRect(&rect);
	int x; 
	int y; 
	if (m_pImage != NULL && m_pImage->IsNull())
	{
		x = 0;
		y = 0;
	}
	else
	{
		x = m_bmpWidth; 
		y = m_bmpHeight; 
	}

	if(rect.right > x) 
	{
		COLORREF c = ::GetSysColor(COLOR_3DFACE);
		CBrush br(c);
		RECT rectRight = rect; 
		rectRight.left = x; 
		rectRight.bottom = y; 
		pDC->FillRect(&rectRight, &br); 
	} 
	if(rect.bottom > y) 
	{ 
		COLORREF c = ::GetSysColor(COLOR_3DFACE);
		CBrush br(c);
		RECT rectBottom = rect; 
		rectBottom.top = y; 
		pDC->FillRect(&rectBottom, &br); 
	}

 	ReleaseDC(pDC);
}