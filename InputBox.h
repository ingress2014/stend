#if !defined(AFX_INPUTBOX_H__493CF099_4C49_4717_9B3C_DF4979141AD6__INCLUDED_)
#define AFX_INPUTBOX_H__493CF099_4C49_4717_9B3C_DF4979141AD6__INCLUDED_

#pragma once
// InputBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBox window

class CInputBox : public CWnd
{
	// Construction
public:
	CInputBox(IN CWnd* pParent = NULL);
	
// Operations
public:
	INT Show(IN LPCTSTR lpszText, 
			OUT CString& strUserInput, 
			IN UINT nIDHelp = 0);
		
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBox)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CInputBox();
	// Generated message map functions
protected:
	CWnd*		m_pParent;
	CEdit		m_cEdit;
	CString*	m_pstrUserInput;
	
	BOOL	CenterMe(IN CRect& rect);
	BOOL	CenterButtons(IN OUT CRect& rcDlg, IN CRect& rcEdit);
	CRect	CreateEdit(IN OUT CRect& rcDlg, IN CRect& rcStatic, IN HFONT& hFont);
	BOOL	GetStaticFontNRect(OUT CRect& rcStatic, OUT HFONT& hFont);
	BOOL	GetDlgRect(IN OUT CRect& rcDlg);
	
	//{{AFX_MSG(CInputBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnInitMsgBox(WPARAM wParam, LPARAM lParam);
	afx_msg void OnKillfocusEditInput();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOX_H__493CF099_4C49_4717_9B3C_DF4979141AD6__INCLUDED_)
