#if !defined(AFX_SCROLLZOOMWIN_H__9A749839_8481_4C56_AADC_86A820E774EB__INCLUDED_)
#define AFX_SCROLLZOOMWIN_H__9A749839_8481_4C56_AADC_86A820E774EB__INCLUDED_

#include "..\..\include\MVImage.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_SCROLL_ZOOM_WIN_MOUSE_MOVE WM_USER+2587

typedef enum {	
	Pic_Zoom_1_1,
	Pic_Zoom_Fit,
	Pic_Zoom_1_2,
	Pic_Zoom_1_4,
	Pic_Zoom_2_1,
	Pic_Zoom_Free
}PictureZoom;

class CScrollZoomWin : public CStatic
{
	// Construction
public:
	CScrollZoomWin();
	// Attributes
public:

protected:
	// bar area
	CRect m_VsbRc;
	CRect m_HsbRc;

	// scrolling pos
	int   m_vsPos;
	int   m_hsPos;
	// thumb size
	int   m_vsBoxSz;
	int   m_hsBoxSz;

	// bmp info
	int   m_bmpWidth;
	int   m_bmpHeight;

	int   m_bmpWidthOld;
	int   m_bmpHeightOld;

	MVImage* m_pImage;
	BOOL  m_bCross;
	BOOL  m_bRedraw;
	// bk bmp

	double m_dZoom;
	PictureZoom m_pictureZoom;
	CRITICAL_SECTION m_csImage;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollZoomWin)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CScrollZoomWin();
	
	void DrawCross(BOOL Cross);
	void SetImage(MVImage* pImg);
	MVImage* GetImage();
	void ImgShow();
	void SetScrollSize();
	void SetpictureZoom(PictureZoom pictureZoom);
	PictureZoom GetpictureZoom();
	void InitImageSize( const CRect *p_rcClient, const CRect *p_rcWindow, int nBorder);
	void SetZoom( PictureZoom pictureZoom, double dFree = 1 );
	int GetHPos();
	int GetVPos();
	double GetZoom();
	void BeginSetImage();
	void EndSetImage();
	// Generated message map functions
public:
	//{{AFX_MSG(CScrollZoomWin)
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	void Redraw();
	void InvalidateBg();
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint=TRUE);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint=TRUE);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLZOOMWIN_H__9A749839_8481_4C56_AADC_86A820E774EB__INCLUDED_)
