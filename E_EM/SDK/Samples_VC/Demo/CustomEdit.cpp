// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CustomEdit.h"


// MyEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

CCustomEdit::CCustomEdit()
{
	m_strText = "0";
	m_type = Type_Int;
}

CCustomEdit::~CCustomEdit()
{
}

//IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	ON_WM_GETDLGCODE()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

UINT CCustomEdit::OnGetDlgCode()
{
	// Assure we will receive Enter key
	return CEdit::OnGetDlgCode() | DLGC_WANTALLKEYS;
}

void CCustomEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bChange = FALSE;
	GetWindowText(m_strText);
	if (nChar == '\r')
	{
		::SendMessage(GetParent()->GetSafeHwnd(),EDIT_RETURN,GetDlgCtrlID(),0);
	}
	else if ( nChar == 8)	//backspace
	{
		if (m_strText.GetLength() <= 1)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			SetWindowText(_T("0"));
		}
		else 
		{
			bChange = TRUE;
		}
	}
	else
	{
		if ( m_type == Type_Int)
			bChange = IsNumber(nChar, nRepCnt, nFlags);
		else 
			bChange = IsNumber(nChar, nRepCnt, nFlags) || IsFirsrtDot(nChar, nRepCnt, nFlags);
	}
	if (bChange)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

BOOL CCustomEdit::IsNumber( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if (nChar>='0' && nChar<='9')
	{
		return TRUE;
	}
	else if( nChar == '-' )
	{
		if( m_strText.Find(TCHAR('-')) < 0 ) 
		{
			return TRUE;
		}
	}

		return FALSE;
}

BOOL CCustomEdit::IsFirsrtDot( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	if(nChar != '.') 
	{  
		//����С����
		return FALSE; 
	}  

	if(m_strText.Find(TCHAR('.')) >=0) 

	{  
		//����С����
		return FALSE; 
	}  

	int iPos = GetCaretPosX(); 
	if(iPos == 0) 
	{  
		//��һ���ַ�����С���㣡
		return FALSE; 
	}  
	return TRUE; 
}

int CCustomEdit::GetCaretPosX()
{
	POINT cp = GetCaretPos();//��ȡ�������ڿؼ���λ��
	int x = CharFromPos(cp);//��ȡ���λ��
	int CharIndex = LOWORD(x);//��ȡ������ڵ��ַ�
	return CharIndex;
}

void CCustomEdit::SetAcceptType( Accept_Type type )
{
	m_type = type;
}


// MyEdit message handlers


