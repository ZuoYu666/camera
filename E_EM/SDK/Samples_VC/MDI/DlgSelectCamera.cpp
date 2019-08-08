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

		Str = L"�������";
		m_lstCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 65 );

		Str = L"�ͺ�";
		m_lstCameras.InsertColumn( ix++, Str,	LVCFMT_LEFT, 120 );

		Str = L"���к�";
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
			char str[32] = "��";
			DWORD_PTR ptr = NULL;
			if( MVOpenCamByIndex(i,&hCam) == MVST_SUCCESS )
			{
				strcpy(str,"��");
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
			m_lstCameras.SetItemState(nFirst, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);   //ѡ����
		}
	}
	else
	{
		strTemp = L"�޷�ˢ���б�";
		AfxMessageBox(strTemp);
	}

	m_lstCameras.SetRedraw(TRUE);	
}

void CDlgSelectCamera::OnBnClickedOk()
{
	POSITION pos = m_lstCameras.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("û��ѡ�����������û�пɷ��ʵ����");
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
			AfxMessageBox("ѡ�е�����޷�����");
		}
	}
}
