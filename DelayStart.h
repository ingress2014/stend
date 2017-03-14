#pragma once
#include "atlcomtime.h"
#include "afxwin.h"

class CMFCDlg;
// CDelayStart dialog

class CDelayStart : public CDialog
{
	DECLARE_DYNAMIC(CDelayStart)

public:
	CDelayStart(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDelayStart();
	CMFCDlg* m_pParent;

// Dialog Data
	enum { IDD = IDD_DELAY_START };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// Время старта по компьютеру
	COleDateTime m_TimeStart;
	virtual BOOL OnInitDialog();
	// Старт по времени с компьютера
	CButton m_StartComp;
	// Ожидание старта с пульта
	CButton m_StartPult;
	afx_msg void OnBnClickedOk();
};
