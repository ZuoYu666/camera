// MDI.h : MDI Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CMDIApp:
// �йش����ʵ�֣������ MDI.cpp
//

class CMDIApp : public CWinApp
{
public:
	CMDIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMDIApp theApp;