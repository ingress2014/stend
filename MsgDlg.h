#pragma once
#include "afxwin.h"

// CMsgDlg dialog
class CMFCDlg;

class CMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgDlg)

public:
	CMsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgDlg();
	CMFCDlg* m_pParent;
// Dialog Data
	enum { IDD = IDD_MESSAGE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	// Кисть далога
	CBrush m_Brush;
	virtual BOOL OnInitDialog();
	// Цвет диалога
	COLORREF m_colorDlg;
	// Статический элемент сообщения
	CStatic m_stText;
	//Шрифт диалога
	CFont m_Font;


	// Установка цвета фона диалога
	void SetColorDlg(COLORREF colorDlg);
	// Установка текста диалога сообщений
	void SetTextDlg(CString textDlg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	// Возможность закрытия при щелчке или ESC
	bool m_EnableClose;
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
