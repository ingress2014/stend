#pragma once


// CLineInfo

#define LINEINFO_CLASSNAME   _T("LineInfoClass")

class CLineInfo : public CWnd
{
	DECLARE_DYNAMIC(CLineInfo)

public:
	CLineInfo();
	virtual ~CLineInfo();

protected:
	BOOL RegisterWndClass();
	DECLARE_MESSAGE_MAP()
	void SetFontHeight(CFont* pFont, int nMaxHeight);
public:

	//Статический контрол номера линии
	CStatic m_stNumLine;
	// Номер линии 
	int m_NumLine;
	//Цвет текста и фона
	COLORREF m_colorTxtNumLine;
	COLORREF m_colorBgdNumLine;
	//Кисть 
	HBRUSH m_NumLineBrush;



	//Статический контрол кол-во линий в программе
	CStatic m_stInPrg;
	int m_InPrg;
	//Цвет текста и фона
	COLORREF m_colorTxtInPrg;
	COLORREF m_colorBgdInPrg;
	//Кисть 
	HBRUSH m_InPrgBrush;

	//Статический контрол сопротивления линии
	CStatic m_stInFact;
	int m_InFact;
	//Цвет текста и фона
	COLORREF m_colorTxtInFact;
	COLORREF m_colorBgdInFact;
	//Кисть 
	HBRUSH m_InFactBrush;

	//Статический контрол количества линий на разъеме
	CStatic m_stDelta;
	int m_Delta;
	//Цвет текста и фона
	COLORREF m_colorTxtDelta;
	COLORREF m_colorBgdDelta;
	//Кисть 
	HBRUSH m_DeltaCountBrush;

	//Шрифт надписей 
	CFont m_Font;

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	virtual void PreSubclassWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// Установка цвета и фона количества линий в программе 
	void SetColorInPrg(COLORREF colorTxt, COLORREF colorBgd);
	// Установка цветов измеренного количества
	void SetColorInFact(COLORREF colorTxt, COLORREF colorBgd);
	// Установка цветов разницы
	void SetColorDelta(COLORREF colorTxt, COLORREF colorBgd);
	// Установка номера линии
	void SetNumLine(int NumLine);
	// Установка количества линий в программе
	void SetInPrg(int intInPrg);
	// Установка количества линий фактически
	void SetInFact(int intInFact);
	// Установка разницы количества линий
	void SetDelta(int intDelta);
};


