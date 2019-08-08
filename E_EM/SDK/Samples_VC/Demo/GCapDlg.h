// GCapDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ScrollZoomWin.h"
#include "../../include/MVGigE.h"
#include "../../include/MVImage.h"
#include "../../include/MVCamProptySheet.h"
#include "../../include/MVAvi.h"

#include "StopWatch.h"
#include "SeqConfDlg.h"
#include "DlgSeqCtr.h"
#include "DLGBARS.H"
#include "DlgRecConf.h"
#include "DlgRecCtr.h"
#include "CustomEdit.h"
#include "iniFile.h"
#include "IniValue.h"

#define CONTINOUSE_TRIGGER_EVENT 0
#define SEQUENCE_EVENT			 1
#define SEQUENCE_TIMER_EVENT     2
#define ID_TIME_REC				 3

enum StartMode
{
	Start_Free_Run,
	Start_Continue_Trigger,
	Start_Single_Trigger,
	Start_Sync_Trigger,
	Stop_Mode
};

// CGCapDlg dialog
class CGCapDlg : public CDialog
{
private:
	MVImage m_image;
	MVImage m_imageGray;
	MVImage m_imageRotate;
	MVImage m_imageRotateGray;
	MVImage* m_pImage;
	MVImage m_imageRecord;
	BOOL   m_bStarted;
	BOOL   m_bOpen;
	MV_PixelFormatEnums m_PixelFormat;
	BOOL m_bContinue;

	CDlgToolBar		m_toolBar;
	CBitmap	m_bmToolbarHi;
	CImageList m_toolList;

	HANDLE m_evRecFrame;
	HANDLE m_evRecExit;
	HANDLE m_hRecProc;
// Construction
public:
	CGCapDlg(CWnd* pParent = NULL);	// standard constructor
	HICON m_hIcon;
// Dialog Data
	enum { IDD = IDD_GCAP_DIALOG };
	HANDLE		m_hCam;
	HANDLE		m_hProperty;
	BOOL m_bRecPause;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	unsigned int	m_nFrameCounter;

	LARGE_INTEGER	liFrequency;
	LARGE_INTEGER	m_liCounterStart, m_liCounterStop;  
	LARGE_INTEGER	m_liRecStart, m_liRecStop; 
	double			m_dFrequency, m_dTime, m_dTimePerFrame;
	
	int				m_nMax;

	CScrollZoomWin	m_stcPic;
	CComboBox		m_cmbPicSize;
	CComboBox		m_cmbCamId;
	CComboBox		m_cmbFlipRotate;
	BOOL			m_bRGBToGray;
	double			m_fTriggerFps;

	int				m_nNextImageDelay;

	int m_nFlipRotate;

	StartMode		m_startMode;

	CSeqParam m_SeqConf;
	DlgSeqCtr	m_SeqCtr;

	int	m_nIndex;
	int m_nSeqStep;
	BOOL m_bSaving;
	CString m_strImageSaveName;
	BOOL m_bSeqAll;
	BOOL m_bSequenceStarted;
	CRITICAL_SECTION m_csSaveImage;
	CRITICAL_SECTION m_csRecord;
	//get the first start time	

	CRecConf m_RecConf;

	DlgRecCtr m_RecCtr;

	BOOL m_bRec;
	BOOL m_bFirstRec;
	MVAvi m_avi;
	time_t m_tRecStart;
	int	m_nRecCnt;
	time_t m_tRecElapse;

	CCustomEdit m_EditTriggerFps;



// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpen();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedStartFreeRun();
	afx_msg void OnBnClickedStartContinueTrigger();
	afx_msg void OnBnClickedStartSingleTrigger();
	afx_msg void OnBnClickedStartSyncTrigger();
	afx_msg void OnBnClickedSaveImage();
	afx_msg void OnBnClickedPropertySet();
	afx_msg void OnCbnSelchangeCmbZoom();
	afx_msg void OnEnKillfocusEditTriggertfps();

	LRESULT OnUpdateFramerate( WPARAM wParam, LPARAM lParam );
	LRESULT OnScrollZoomWinMouseMove( WPARAM wParam, LPARAM lParam );

	void EnableButton(BOOL bCamStart);

	BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsStart();

	int OnStreamCB( MV_IMAGE_INFO *pInfo);
	afx_msg void OnBnClickedOpen();
	int UpdateId();
	afx_msg void OnCbnSelchangeComboCmaId();
	afx_msg void OnEnSetfocusEditTriggerfps();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckRgbToGray();
	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	void CreateImage();
	void DestroyImage();
	afx_msg void OnBnClickedSequence();
	afx_msg void OnBnClickedSequenceStart();
	afx_msg void OnBnClickedSeqCtr();
	afx_msg void OnBnClickedSequenceStop();
	afx_msg void OnBnClickedSequenceCapture();
	void SaveImage();
	afx_msg void OnCbnSelchangeCmbFlipRotate();
	afx_msg void OnBnClickedRecord();
	afx_msg void OnBnClickedRecordPause();
	afx_msg void OnBnClickedRecordStop();
	afx_msg void OnBnClickedRecordConf();
	afx_msg void OnBnClickedRecCtr();
	void Start(StartMode mode);
	int Record();
	void EnableSequenceSet(BOOL flag);
	void SetDlgTitle();
	void UpdateRecCtr();
	BOOL m_bCross;
	afx_msg void OnBnClickedCross();
	afx_msg void OnCancel();
	BOOL LoadINIFile(CString strPathName = _T(""));
	BOOL SaveINIFile(CString strPathName = _T(""));
	void ImageProcess(MVImage* pImage);
	void DisplayPointAndRGB( CPoint point );
	void RecordProc();
	LRESULT OnEditReturn( WPARAM wParam, LPARAM lParam );
	BOOL m_bDenoise;
	afx_msg void OnBnClickedDenoise();
};
