#pragma once
#include "afxwin.h"


// CTablo

#define TABLO_CLASSNAME   _T("TabloClass")

class CTablo : public CWnd
{
	DECLARE_DYNAMIC(CTablo)

public:
	CTablo();
	virtual ~CTablo();

protected:
	BOOL RegisterWndClass();
	DECLARE_MESSAGE_MAP()
	void SetFontHeight(CFont* pFont, int nMaxHeight);
public:
	// Шрифт панели
	CFont m_Font;
	COLORREF m_colorText;
	COLORREF m_colorBgd;
	// Фон окна
	HBRUSH m_hBrush;
	// Текст окна
	CString m_Text;
	CStatic m_stText;
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// Установка значения табло
	void SetText(CString str);
};


