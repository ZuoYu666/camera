// DlgSelectCamera.cpp : implementation file
//

#include "stdafx.h"
#include "MDI.h"
#include "DlgSelectCamera.h"

#include "..\..\include\MVGigE.h"


// CDlgSelectCamera dialog

IMPLEMENT_DYNAMIC(CDlgSelectCamera, CDialog)

CDlgSelectCamera::CDlgSelectCamera(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectCamera::IDD, pParent)
{

}

CDlgSelectCamera::~CDlgSelectCamera()
{
}

void CDlgSelectCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CAMERAS, m_lstCameras);
}


BEGIN_MESSAGE_MAP(CDlgSelectCamera, CDialog)
	ON_BN_CLICKED(IDC_REFRESH, &CDlgSelectCamera::OnBnClickedRefresh)
	ON_BN_CLICKED(IDOK, &CDlgSelectCamera::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDlgSelectCamera::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_lstCameras.GetHeaderCtrl()->GetItemCount() == 0)
	{
		CString Str;
		int ix = 0;

		Str = L"允许访问";
		m_lstCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 65 );

		Str = L"型号";
		m_lstCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 120 );

		Str = L"序列号";
		m_lstCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 120 );

		m_lstCameras.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	}

	OnBnClickedRefresh();

	return TRUE;
}


void CDlgSelectCamera::OnBnClickedRefresh()
{
	CString strTemp;

	m_lstCameras.DeleteAllItems();

	int nCameraCount = 0;

	CWaitCursor cursor;

	MVUpdateCameraList();

	if (MVGetNumOfCameras(&nCameraCount) == 0)
	{
		int nFirst = -1;

		for(int i = 0; i < nCameraCount; i++)
		{
			int ix = 1;
			int icon = 0;

			MVCamInfo info;
			MVGetCameraInfo(i,&info);

			HANDLE hCam;
			char str[32] = "否";
			DWORD_PTR ptr = NULL;
			if( MVOpenCamByIndex(i,&hCam) == MVST_SUCCESS )
			{
				strcpy(str,"是");
				ptr = 1;
				MVCloseCam(hCam);

				if( nFirst < 0 )
				{
					nFirst = i;
				}
			}		
			m_lstCameras.InsertItem(i,str);

			m_lstCameras.SetItemText( i, ix++, info.mModelName);

			m_lstCameras.SetItemText( i, ix++, info.mSerialNumber);

			m_lstCameras.SetItemData( i, ptr);
		}

		if( nFirst >= 0 )
		{
			m_lstCameras.SetSelectionMark(nFirst);
			m_lstCameras.SetItemState(nFirst, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //选中行
		}
	}
	else
	{
		strTemp = L"无法刷新列表";
		AfxMessageBox(strTemp);
	}

	m_lstCameras.SetRedraw(TRUE);	
}

void CDlgSelectCamera::OnBnClickedOk()
{
	POSITION pos = m_lstCameras.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("没有选择相机或所有没有可访问的相机");
	}
	else
	{
		int id = m_lstCameras.GetNextSelectedItem(pos);

		if( m_lstCameras.GetItemData(id) != NULL )
		{
			m_nDevID = id;
			OnOK();
		}
		else
		{
			AfxMessageBox("选中的相机无法访问");
		}
	}
}
