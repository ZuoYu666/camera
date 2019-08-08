#pragma once

#ifndef __MV_EDIT__H__
#define __MV_EDIT__H__

#define EDIT_RETURN WM_USER + 1
// MyEdit

typedef enum
{
	Type_Int,
	Type_Float
}Accept_Type;

class MVEdit : public CEdit
{
	DECLARE_DYNAMIC(MVEdit)

public:
	MVEdit();
	virtual ~MVEdit();

protected:
	DECLARE_MESSAGE_MAP()
	CString m_strText;
	Accept_Type m_type;
public:
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL IsNumber(UINT nChar, UINT nRepCnt, UINT nFlags);
	BOOL IsFirsrtDot(UINT nChar, UINT nRepCnt, UINT nFlags);
	int GetCaretPosX(); 
	void SetAcceptType(Accept_Type type);

};

#endif

