// CSeqConfDlg.cpp : implementation file
//
#include "stdafx.h"
#include "SeqConfDlg.h"
#include "iniFile.h"
#include "IniValue.h"
// CSeqConfDlg dialog

IMPLEMENT_DYNAMIC(CSeqConfDlg, CDialog)
#define noID CIniFile::noID
void InitSeqConf(CSeqParam &Param)
{
		Param.m_bIndex = TRUE;
		Param.m_nIndex = 1;
		Param.m_nTimeStyle = 2;
		Param.m_nMinute = 0;
		Param.m_nSecond =5;
		Param.m_strPrefixName = _T("Image");
		Param.m_strPath = _T("");
		Param.m_strFileType = _T(".bmp");
}

BOOL InitByFile( CString strIniFilePath, CSeqParam &Param )
{
	CIniFile IniFile;
	IniFile.SetPath(strIniFilePath.LockBuffer());
	IniFile.CaseSensitive();

	if (IniFile.ReadFile() == FALSE)
		return FALSE;

	if (IniFile.FindKey(KEY_SEQUENCE_CONFIG) == noID)
		return FALSE;

	Param.m_bIndex		  = IniFile.GetValueB(KEY_SEQUENCE_CONFIG, VALUE_BOOLEAN_WITH_INDEX, TRUE);
	Param.m_nIndex		  = IniFile.GetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_INDEX, 1);
	if(Param.m_nIndex < 0)
		Param.m_nIndex = 0;
	Param.m_nTimeStyle	  = IniFile.GetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_TIME_STYLE, 2);
	if (Param.m_nTimeStyle<0 || Param.m_nTimeStyle>2)
		Param.m_nTimeStyle = 2;
	Param.m_nMinute		  = IniFile.GetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_MINUTE, 0);
	Param.m_nSecond		  = IniFile.GetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_SECOND, 5);
	Param.m_strPrefixName = IniFile.GetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_PREFIX_NAME, _T("Image")).c_str();
	Param.m_strPath		  = IniFile.GetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_PATH, _T("")).c_str();
	Param.m_strFileType   = IniFile.GetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_FILE_TYPE, _T(".bmp")).c_str();
	return TRUE;
}

CSeqConfDlg::CSeqConfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSeqConfDlg::IDD, pParent)
{
	InitSeqConf(m_Param);
}

CSeqConfDlg::CSeqConfDlg(CSeqParam param, CWnd* pParent)
: CDialog(CSeqConfDlg::IDD, pParent)
{
	m_Param = param;
}
CSeqConfDlg::CSeqConfDlg(CString strIniFilePath, CWnd* pParent)
: CDialog(CSeqConfDlg::IDD, pParent)
{
	InitByFile(strIniFilePath, m_Param);
}
CSeqConfDlg::~CSeqConfDlg()
{
}

void CSeqConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_FILENAME_PREFIX, m_Param.m_strPrefixName);
	DDX_Text(pDX, IDC_EDIT_PATH, m_Param.m_strPath);

	DDX_Text(pDX, IDC_EDIT_INDEX, m_Param.m_nIndex);
	DDV_MinMaxInt(pDX, m_Param.m_nIndex, 0, INT_MAX);

	DDX_Text(pDX, IDC_EDIT_MINUTE, m_Param.m_nMinute);
	DDV_MinMaxInt(pDX, m_Param.m_nMinute, 0, 59);
	DDX_Text(pDX, IDC_EDIT_SECOND, m_Param.m_nSecond);
	DDV_MinMaxInt(pDX, m_Param.m_nSecond, 0, 59);

	DDX_CBString(pDX, IDC_COMBO_FILE_TYPE, m_Param.m_strFileType);
	DDX_CBIndex(pDX, IDC_COMBO_TIME_STYLE, m_Param.m_nTimeStyle);
	DDX_Check(pDX, IDC_CHECK_INDEX, m_Param.m_bIndex);
}


BEGIN_MESSAGE_MAP(CSeqConfDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_PATH, &CSeqConfDlg::OnBnClickedButtonChangePath)
	ON_BN_CLICKED(IDC_CHECK_INDEX, &CSeqConfDlg::OnBnClickedCheckIndex)
	ON_CBN_SELCHANGE(IDC_COMBO_FILE_TYPE, &CSeqConfDlg::OnCbnSelchangeComboFileType)
	ON_EN_KILLFOCUS(IDC_EDIT_FILENAME_PREFIX, &CSeqConfDlg::OnEnKillfocusEditFilenamePrefix)
	ON_EN_KILLFOCUS(IDC_EDIT_INDEX, &CSeqConfDlg::OnEnKillfocusEditIndex)
	ON_EN_KILLFOCUS(IDC_EDIT_MINUTE, &CSeqConfDlg::OnEnKillfocusEditMinute)
	ON_EN_KILLFOCUS(IDC_EDIT_SECOND, &CSeqConfDlg::OnEnKillfocusEditSecond)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_STYLE, &CSeqConfDlg::OnCbnSelchangeComboTimeType)
	ON_BN_CLICKED(IDOK, &CSeqConfDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOAD, &CSeqConfDlg::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_SAVE, &CSeqConfDlg::OnBnClickedSave)
END_MESSAGE_MAP()

BOOL CSeqConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

	ShowWindow(SW_SHOW);

	CenterWindow();

	// TODO: Add extra initialization here
	return TRUE;
}


// CSeqConfDlg message handlers

int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)   
{   
	if  (uMsg == BFFM_INITIALIZED )
	{   
		char *str = (char *)lpData;
		::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);   
		::SendMessage(hwnd, BFFM_SETEXPANDED, TRUE, lpData);
	}   
	return 0;  
}

void CSeqConfDlg::OnBnClickedButtonChangePath()
{
	BROWSEINFO bi;
	ZeroMemory(&bi,sizeof(BROWSEINFO));//指定存放文件的默认文件夹路径

	LPITEMIDLIST pidlRoot = NULL;

	bi.hwndOwner    = GetSafeHwnd();
	bi.lpszTitle    = _T("请选择目录");
	bi.ulFlags		= BIF_NEWDIALOGSTYLE;
	bi.lpfn         = NULL;
	bi.pidlRoot     = pidlRoot;
	bi.lParam       = (LPARAM)m_Param.m_strPath.GetBuffer();
	bi.lpfn         = BrowseCallbackProc;

	LPMALLOC pMalloc;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);//以默认路径打开浏览文件夹对话框
	TCHAR * savepath = new TCHAR[MAX_PATH]; 
	CString savepath2="";
	if(pidl != NULL)
	{
		SHGetPathFromIDList(pidl,savepath);//把文件夹路径存放在savepath中 
		savepath2.Format(_T("%s"),savepath);
		savepath2=savepath2+"\\";//在路径后增加斜杠
		m_Param.m_strPath = savepath2;
		if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pid释放
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		} 
	}
	UpdateData(FALSE);
	free(savepath);
//	MessageBox(savepath2);//显示所选文件夹路径
}

void CSeqConfDlg::OnBnClickedCheckIndex()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CSeqConfDlg::OnCbnSelchangeComboFileType()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	TRACE(m_Param.m_strFileType);
}

void CSeqConfDlg::OnEnKillfocusEditFilenamePrefix()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void CSeqConfDlg::OnEnKillfocusEditIndex()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_Param.m_nIndex<0)
	{
		m_Param.m_nIndex = 0;
	}
	UpdateData(FALSE);
}

void CSeqConfDlg::OnEnKillfocusEditMinute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_Param.m_nMinute<0)
	{
		m_Param.m_nMinute = 0;
	}
	else if (m_Param.m_nMinute>59)
	{
		m_Param.m_nMinute = 59;
	}
	UpdateData(FALSE);
}

void CSeqConfDlg::OnEnKillfocusEditSecond()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_Param.m_nSecond<0)
	{
		m_Param.m_nSecond = 0;
	}
	else if (m_Param.m_nSecond>59)
	{
		m_Param.m_nSecond = 59;
	}

	UpdateData(FALSE);
}

BOOL CSeqConfDlg::PreTranslateMessage( MSG* pMsg )
{
	if(pMsg->message==WM_KEYDOWN) 
	{ 
		switch(pMsg->wParam)  
		{  
		case VK_RETURN:   //屏蔽Enter 
			if(GetFocus() == GetDlgItem(IDC_EDIT_FILENAME_PREFIX))
			{
				OnEnKillfocusEditFilenamePrefix();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_INDEX))
			{
				OnEnKillfocusEditIndex();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_MINUTE))
			{
				OnEnKillfocusEditMinute();
			}
			else if (GetFocus() == GetDlgItem(IDC_EDIT_SECOND))
			{
				OnEnKillfocusEditSecond();
			}
			return TRUE;
		} 

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSeqConfDlg::OnCbnSelchangeComboTimeType()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}


void CSeqConfDlg::GetParam( CSeqParam &param )
{
	param.m_nIndex=m_Param.m_nIndex;
	param.m_nTimeStyle=m_Param.m_nTimeStyle;
	param.m_nMinute=m_Param.m_nMinute;
	param.m_nSecond=m_Param.m_nSecond;
	param.m_strPrefixName=m_Param.m_strPrefixName;
	param.m_strPath=m_Param.m_strPath;
	param.m_strFileType=m_Param.m_strFileType;
	param.m_bIndex=m_Param.m_bIndex;	
}




void CSeqConfDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (!SaveParam(FILE_NAME_SEQ))
	{
		AfxMessageBox(_T("保存失败"));
	}
	OnOK();
}

BOOL CSeqConfDlg::SaveParam( CString srtPath )
{
	CIniFile IniFile;
	bool r = true;
	IniFile.SetPath(srtPath.LockBuffer());
	IniFile.CaseSensitive();

	r |= IniFile.SetValueB(KEY_SEQUENCE_CONFIG, VALUE_BOOLEAN_WITH_INDEX,	m_Param.m_bIndex);
	r |= IniFile.SetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_INDEX,		m_Param.m_nIndex);
	r |= IniFile.SetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_TIME_STYLE,	m_Param.m_nTimeStyle);
	r |= IniFile.SetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_MINUTE,		m_Param.m_nMinute);
	r |= IniFile.SetValueI(KEY_SEQUENCE_CONFIG, VALUE_INTEGER_SECOND,		m_Param.m_nSecond);
	r |= IniFile.SetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_PREFIX_NAME,	m_Param.m_strPrefixName.LockBuffer());
	r |= IniFile.SetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_PATH,			m_Param.m_strPath.LockBuffer());
	r |= IniFile.SetValue(KEY_SEQUENCE_CONFIG,  VALUE_STRING_FILE_TYPE,		m_Param.m_strFileType.LockBuffer());

	if (!r)
	{
		return FALSE;
	}

	if (IniFile.WriteFile() == false)
		return FALSE;
	return TRUE;
}

void CSeqConfDlg::OnBnClickedLoad()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, _T("INI"), _T("未命名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "INI (*.ini)|*.ini|");
	if (dlg.DoModal() == IDOK)
	{
		InitByFile(dlg.GetPathName(), m_Param);
		UpdateData(FALSE);
	}
}

void CSeqConfDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(FALSE, _T("INI"), _T("未命名"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "INI (*.ini)|*.ini|");
	if (dlg.DoModal() == IDOK)
	{
		SaveParam(dlg.GetPathName());
	}
}
