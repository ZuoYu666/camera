// MDIView.h : CMDIView 类的接口
//


#pragma once
#include "..\..\include\MVGigE.h"
#include "..\..\include\MVCamProptySheet.h"
#include "..\..\include\MVImage.h"


class CMDIView : public CScrollView
{
protected: // 仅从序列化创建
	CMDIView();
	DECLARE_DYNCREATE(CMDIView)

// 属性
public:
	CMDIDoc* GetDocument() const;

// 操作
public:
	DWORD	m_nIdxCam;
	BOOL	m_bRun;
	HANDLE m_hCam;
	HANDLE m_hProperty;
	MVImage m_image;
	MV_PixelFormatEnums m_PixelFormat;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CMDIView();

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // MDIView.cpp 中的调试版本
inline CMDIDoc* CMDIView::GetDocument() const
   { return reinterpret_cast<CMDIDoc*>(m_pDocument); }
#endif

