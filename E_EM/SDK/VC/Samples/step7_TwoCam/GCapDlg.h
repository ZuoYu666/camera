// GCapDlg.h : header file
//

#pragma once

#include "..\..\include\MVGigE.h"
#include "..\..\include\MVImage.h"
#include "afxcmn.h"
#include "afxwin.h"

#define CAM_MAX 2
#define X_OFFEST 8
#define Y_OFFSET 40

typedef enum {	
	Pic_Size_1_1,
	Pic_Size_Fit,
	Pic_Size_2_1,
	Pic_Size_4_1,
	Pic_Size_1_2,
	Pic_Size_Free
}PicSize;

// CGCapDlg dialog
class CGCapDlg : public CDialog
{
private:
	HANDLE m_hCam[CAM_MAX];
	MVImage m_image[CAM_MAX];
	MV_PixelFormatEnums m_PixelFormat[CAM_MAX];
	int m_nCams;
	double m_dZoom;
	PicSize m_picSize;
	CRect m_rect[CAM_MAX];
// Construction
public:
	CGCapDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GCAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStart();
	int OnStreamCB0( MV_IMAGE_INFO *pInfo);
	int OnStreamCB1( MV_IMAGE_INFO *pInfo);
	void ImgDraw(MV_IMAGE_INFO *pInfo, int idx);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnKillfocusEditGain1();
	afx_msg void OnEnKillfocusEditGain2();
	CSliderCtrl m_sldGain1;
	CSliderCtrl m_sldGain2;
	double m_dGain1;
	double m_dGain2;
	afx_msg void OnBnClickedButton1Gain();
	afx_msg void OnBnClickedButton2Gain();
	afx_msg void OnBnClickedButton1Expo();
	afx_msg void OnBnClickedButton2Expo();
	double m_dExpo1;
	double m_dExpo2;
	CComboBox m_cmbZoom;
	afx_msg void OnCbnSelchangeComboZoom();
	CSpinButtonCtrl m_spinGain2;
	CSpinButtonCtrl m_spinGain1;
	afx_msg void OnDeltaposSpinGain1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinGain2(NMHDR *pNMHDR, LRESULT *pResult);
};
