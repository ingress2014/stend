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

	//����������� ������� ������ �����
	CStatic m_stNumLine;
	// ����� ����� 
	int m_NumLine;
	//���� ������ � ����
	COLORREF m_colorTxtNumLine;
	COLORREF m_colorBgdNumLine;
	//����� 
	HBRUSH m_NumLineBrush;



	//����������� ������� ���-�� ����� � ���������
	CStatic m_stInPrg;
	int m_InPrg;
	//���� ������ � ����
	COLORREF m_colorTxtInPrg;
	COLORREF m_colorBgdInPrg;
	//����� 
	HBRUSH m_InPrgBrush;

	//����������� ������� ������������� �����
	CStatic m_stInFact;
	int m_InFact;
	//���� ������ � ����
	COLORREF m_colorTxtInFact;
	COLORREF m_colorBgdInFact;
	//����� 
	HBRUSH m_InFactBrush;

	//����������� ������� ���������� ����� �� �������
	CStatic m_stDelta;
	int m_Delta;
	//���� ������ � ����
	COLORREF m_colorTxtDelta;
	COLORREF m_colorBgdDelta;
	//����� 
	HBRUSH m_DeltaCountBrush;

	//����� �������� 
	CFont m_Font;

	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg UINT OnGetDlgCode();
	virtual void PreSubclassWindow();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// ��������� ����� � ���� ���������� ����� � ��������� 
	void SetColorInPrg(COLORREF colorTxt, COLORREF colorBgd);
	// ��������� ������ ����������� ����������
	void SetColorInFact(COLORREF colorTxt, COLORREF colorBgd);
	// ��������� ������ �������
	void SetColorDelta(COLORREF colorTxt, COLORREF colorBgd);
	// ��������� ������ �����
	void SetNumLine(int NumLine);
	// ��������� ���������� ����� � ���������
	void SetInPrg(int intInPrg);
	// ��������� ���������� ����� ����������
	void SetInFact(int intInFact);
	// ��������� ������� ���������� �����
	void SetDelta(int intDelta);
};


