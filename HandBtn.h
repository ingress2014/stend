#pragma once
#include "afxwin.h"
#include "ColorButton.h"


// CHandBtn
#define HANDBTN_CLASSNAME   _T("HandBtnClass")

class CHandBtn : public CWnd
{
	DECLARE_DYNAMIC(CHandBtn)


public:
	CHandBtn();
	virtual ~CHandBtn();

	//����� ������
	int m_NumBtn;
	//��������� ���-�� �������
	CStatic m_ResultsWnd;
	//������������ ������
	CString m_NameBtn; 
	// ���������� ������� �� ������
	int m_ClickCount;

protected:
	BOOL RegisterWndClass();
	DECLARE_MESSAGE_MAP()
private:

	//����� ���������� ������� �� ������
	CFont m_ResultsFont;
	//����� ������
	CFont m_ButtonFont;
	//����� ������ ������
	CBrush m_NumCtrlBrush;
	//����� ��������� ������
	CBrush m_ResultsWndBrush;
	
	//���� ������ � ���� ������ ������
	COLORREF m_clrNumTxt;
	COLORREF m_clrNumBk;


	COLORREF m_clrResultsBk;
	COLORREF m_clrResultsTxt;
public:

	int GetClickCount(void)
	{
		return m_ClickCount;
	}

	// ����� ���������� �������
	void ResetClickCount(void)
	{
		m_ClickCount = 0;
		m_ResultsWnd.SetWindowText(_T("0"));

	}
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// ������ ��������
	CColorButton m_Btn;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	// ��������� ������� ������ � ����������� �� ������� ����
	void SetFontHeight(CFont* pFont, int nMaxHeight);

	// ��������� ����� ���� ������
	COLORREF SetResultWndTxtClr(COLORREF rgbNewColor)
	{
		COLORREF clrPrev=m_clrResultsTxt;
		m_clrResultsTxt=rgbNewColor;

		return clrPrev;
		
	}

	// ��������� ���� ���� ������
	COLORREF SetResultWndBkClr(COLORREF rgbNewColor)
	{
		COLORREF clrPrev=m_clrResultsBk;
		m_ResultsWndBrush.Detach();
		m_ResultsWndBrush.CreateSolidBrush(rgbNewColor);
		m_clrResultsBk=rgbNewColor;

		return clrPrev;
	}
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	// ����������� ������� ������ ������
	CStatic m_NumCtrl;
public:
	// ����� ����������� 
	BOOL m_State;

	// ������ �������� � ������ ����������� 
	BOOL m_Checked;

	// ������ ������
	BOOL m_IsMotor;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPressButton(void);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	// ��������� ������ ������
	void SetNumBtn(int NumBtn);
	// ��������� ������ ������
	void SetNameBtn(CString NameBtn);
	// ��������� ����� ������
	void SetColorBtn(COLORREF clrTxt, COLORREF clrBkgrnd);
	// ����� ������ ������
	void ResetColorBtn(void);
	// ��������� �������� ��� ������� ������
	void IncButton(void);
	// ������� ������� ����������
	UINT m_Key;
};


