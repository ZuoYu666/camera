// MDIView.h : CMDIView ��Ľӿ�
//


#pragma once
#include "..\..\include\MVGigE.h"
#include "..\..\include\MVCamProptySheet.h"
#include "..\..\include\MVImage.h"


class CMDIView : public CScrollView
{
protected: // �������л�����
	CMDIView();
	DECLARE_DYNCREATE(CMDIView)

// ����
public:
	CMDIDoc* GetDocument() const;

// ����
public:
	DWORD	m_nIdxCam;
	BOOL	m_bRun;
	HANDLE m_hCam;
	HANDLE m_hProperty;
	MVImage m_image;
	MV_PixelFormatEnums m_PixelFormat;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CMDIView();

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCapture();
	afx_msg void OnStopCap();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCamProperty();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateStopcap(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCapture(CCmdUI *pCmdUI);

	BOOL OpenCamera();
	int OnStreamCB(MV_IMAGE_INFO *pInfo);
};

#ifndef _DEBUG  // MDIView.cpp �еĵ��԰汾
inline CMDIDoc* CMDIView::GetDocument() const
   { return reinterpret_cast<CMDIDoc*>(m_pDocument); }
#endif

