#pragma once
#include "afxwin.h"

class CMFCDlg;

// CDebugDlg dialog

class CDebugDlg : public CDialog
{
	DECLARE_DYNAMIC(CDebugDlg)

public:
	CDebugDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDebugDlg();


// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Напряжение на линии сопротивления
	CString m_Res;
	// Напряжение на линии емкости
	CString m_Cap;

	CMFCDlg* m_pParent;


	afx_msg void OnBnClickedButtonOnHi();
	afx_msg void OnBnClickedButtonOffHi();
	afx_msg void OnBnClickedButtonOnLo();
	afx_msg void OnBnClickedButtonOffLo();
	afx_msg void OnBnClickedButtonOnCapRelay();
	afx_msg void OnBnClickedButtonOffCapRelay();
	afx_msg void OnBnClickedButtonOnResRelay();
	afx_msg void OnBnClickedButtonOffResRelay();
	afx_msg void OnBnClickedButtonGetRes();
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonGetCap();
	// Надпись емкости
	CStatic m_stCap;
	afx_msg void OnBnClickedSetLine1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedClearLine();
	// Элемент измеренного сопротивления
	CStatic m_stRes;
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
