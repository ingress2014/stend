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

	//Номер кнопки
	int m_NumBtn;
	//Результат кол-ва нажатий
	CStatic m_ResultsWnd;
	//Наименование кнопки
	CString m_NameBtn; 
	// Количество нажатий на кнопку
	int m_ClickCount;

protected:
	BOOL RegisterWndClass();
	DECLARE_MESSAGE_MAP()
private:

	//Шрифт результата нажатий на кнопку
	CFont m_ResultsFont;
	//Шрифт кнопки
	CFont m_ButtonFont;
	//Кисть номера кнопки
	CBrush m_NumCtrlBrush;
	//Кисть заголовка кнопки
	CBrush m_ResultsWndBrush;
	
	//Цвет текста и фона номера кнопки
	COLORREF m_clrNumTxt;
	COLORREF m_clrNumBk;


	COLORREF m_clrResultsBk;
	COLORREF m_clrResultsTxt;
public:

	int GetClickCount(void)
	{
		return m_ClickCount;
	}

	// Сброс количества нажатий
	void ResetClickCount(void)
	{
		m_ClickCount = 0;
		m_ResultsWnd.SetWindowText(_T("0"));

	}
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	// Кнопка счетчика
	CColorButton m_Btn;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void PreSubclassWindow();
	// Изменение размера шрифта в зависимости от размера окна
	void SetFontHeight(CFont* pFont, int nMaxHeight);

	// Установка цвета окна вывода
	COLORREF SetResultWndTxtClr(COLORREF rgbNewColor)
	{
		COLORREF clrPrev=m_clrResultsTxt;
		m_clrResultsTxt=rgbNewColor;

		return clrPrev;
		
	}

	// Установка фона окна вывода
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
	// Статический контрол номера кнопки
	CStatic m_NumCtrl;
public:
	// Режим группировки 
	BOOL m_State;

	// Кнопка отмечена в режиме группировки 
	BOOL m_Checked;

	// Кнопка мотора
	BOOL m_IsMotor;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPressButton(void);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	// Установка номера кнопки
	void SetNumBtn(int NumBtn);
	// Установка лейбла кнопки
	void SetNameBtn(CString NameBtn);
	// Изменение цвета кнопки
	void SetColorBtn(COLORREF clrTxt, COLORREF clrBkgrnd);
	// Сброс цветов кнопки
	void ResetColorBtn(void);
	// Инкремент счетчика без нажатия кнопки
	void IncButton(void);
	// Нажатие клавиши клавиатуры
	UINT m_Key;
};


