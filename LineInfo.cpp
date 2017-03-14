// LineInfo.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "LineInfo.h"
#include "PultConst.h"


// CLineInfo

#define NUMLINE_ID			1103
#define CAPWND_ID			1104
#define RESWND_ID			1105
#define LINECOUNTWND_ID		1106

IMPLEMENT_DYNAMIC(CLineInfo, CWnd)

CLineInfo::CLineInfo()
: m_NumLine(0)
, m_colorBgdNumLine(COLOR_WHITE)
, m_colorTxtNumLine(COLOR_BLACK)
, m_colorBgdInPrg(COLOR_BG_CAP)
, m_colorTxtInPrg(COLOR_BLACK)
, m_colorBgdInFact(COLOR_BG_RES)
, m_colorTxtInFact(COLOR_BLACK)
, m_colorBgdDelta(COLOR_BG_COUNT_LINE)
, m_colorTxtDelta(COLOR_BLACK)
, m_InPrg(0)
, m_InFact(0)
, m_Delta(0)
, m_NumLineBrush(NULL)
, m_InPrgBrush(NULL)
, m_InFactBrush(NULL)
, m_DeltaCountBrush(NULL)


{
	//Регистрация окна
	RegisterWndClass();
	//Регистрация шрифта
	m_Font.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial Narrow"));
}

CLineInfo::~CLineInfo()
{

}

BOOL CLineInfo::RegisterWndClass()
{
	WNDCLASS wndclass;
	//Проверка класс зарегистрирован?
	if(::GetClassInfo(AfxGetInstanceHandle(),LINEINFO_CLASSNAME,&wndclass))
		return TRUE;
	// Регистрация нового управляющего класса Windows

	wndclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;;
	wndclass.lpfnWndProc = ::DefWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = ::AfxGetInstanceHandle();
	wndclass.hIcon = NULL;
	wndclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = LINEINFO_CLASSNAME;
	return ::AfxRegisterClass(&wndclass);
}

// Изменение размера шрифта в зависимости от размера окна
void CLineInfo::SetFontHeight(CFont* pFont, int nMaxHeight)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	pFont->Detach();

	// Изменяем шрифт
	lf.lfHeight=nMaxHeight;

	// Обновляем
	pFont->CreateFontIndirect(&lf);
}



BEGIN_MESSAGE_MAP(CLineInfo, CWnd)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CLineInfo::OnSetFocus(CWnd* pOldWnd)
{
		Invalidate(FALSE);
		
}


void CLineInfo::OnKillFocus(CWnd* pNewWnd)
{
		Invalidate(FALSE);
}


void CLineInfo::OnPaint()
{
	CPaintDC dc(this); 
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect drawRect(wp.rcNormalPosition);
	drawRect.MoveToXY(0,0);
	dc.Draw3dRect(&drawRect, RGB(64,64,64), RGB(192,192,192));
}


UINT CLineInfo::OnGetDlgCode()
{
	// TODO: Add your message handler code here and/or call default
	   // return DLGC_WANTALLKEYS;
	 return CWnd::OnGetDlgCode();
}


void CLineInfo::PreSubclassWindow()
{
	CString tmp;

	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect cntrlRect=wp.rcNormalPosition; //Размер всего элемента
	cntrlRect.MoveToXY(0,0); //Переместились в верхний левый угол

	//Создаем окно номера линии
	int NumLineHeight = cntrlRect.Height()/3;
	int NumLineWidth = NumLineHeight;

	//Делаем квадратное окно capHeight,capHeight
	tmp.Format(_T("%d"),m_NumLine);
	VERIFY(m_stNumLine.Create(tmp,WS_VISIBLE|WS_CHILD|SS_CENTER|SS_SUNKEN,
		CRect(cntrlRect.left,cntrlRect.top,NumLineWidth,NumLineHeight),this,
		NUMLINE_ID));
	// Считаем размер шрифта
	SetFontHeight(&m_Font, NumLineHeight);
	//Устанавливаем шрифт окна
	m_stNumLine.SetFont(&m_Font);
	//Устанавливаем фон окна
	m_NumLineBrush = CreateSolidBrush(m_colorBgdNumLine);


	//Создаем окно емкости линии
	tmp = _T("X");
	VERIFY(m_stInPrg.Create(tmp,WS_VISIBLE|WS_CHILD|SS_CENTER|SS_SUNKEN,
		CRect(cntrlRect.left + NumLineWidth,cntrlRect.top,cntrlRect.Width(),NumLineHeight),this,
		CAPWND_ID));
	// Считаем размер шрифта
	SetFontHeight(&m_Font, NumLineHeight);
	//Устанавливаем шрифт окна
	m_stInPrg.SetFont(&m_Font);
	//Устанавливаем фон окна
	m_InPrgBrush = CreateSolidBrush(m_colorBgdInPrg);


	//Создаем окно сопротивления линии
	int resHeight = NumLineHeight;
	int resWidth = cntrlRect.Width();
	tmp = _T("X");
	VERIFY(m_stInFact.Create(_T(tmp),WS_VISIBLE|WS_CHILD|SS_CENTER|SS_SUNKEN,
		CRect(cntrlRect.left,cntrlRect.top + NumLineHeight,resWidth,resHeight + NumLineHeight),this,
		RESWND_ID));
	// Считаем размер шрифта
	SetFontHeight(&m_Font, resHeight);
	//Устанавливаем шрифт окна
	m_stInFact.SetFont(&m_Font);
	//Устанавливаем фон окна
	m_InFactBrush = CreateSolidBrush(m_colorBgdInFact);

	//Создаем окно количества линий
	int lineHeight = NumLineHeight;
	int lineWidth = cntrlRect.Width();
	tmp = _T("X");
	VERIFY(m_stDelta.Create(tmp,WS_VISIBLE|WS_CHILD|SS_CENTER|SS_SUNKEN,
		CRect(cntrlRect.left,cntrlRect.top + 2*NumLineHeight,lineWidth,lineHeight+ 2*NumLineHeight),this,
		LINECOUNTWND_ID));
	// Считаем размер шрифта
	SetFontHeight(&m_Font, lineHeight);
	//Устанавливаем шрифт окна
	m_stDelta.SetFont(&m_Font);
	//Устанавливаем фон окна
	m_DeltaCountBrush = CreateSolidBrush(m_colorBgdDelta);

	CWnd::PreSubclassWindow();
}


HBRUSH CLineInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	int nItem = pWnd->GetDlgCtrlID();
	switch(nItem)
	{
	
	case NUMLINE_ID:
		pDC->SetTextColor(m_colorTxtNumLine);
		pDC->SetBkColor(m_colorBgdNumLine);
		return m_NumLineBrush;

	
	case RESWND_ID:
		pDC->SetTextColor(m_colorTxtInFact);
		pDC->SetBkColor(m_colorBgdInFact);
		return m_InFactBrush;
	
	case CAPWND_ID:

		pDC->SetTextColor(m_colorTxtInPrg);
		pDC->SetBkColor(m_colorBgdInPrg);
		return m_InPrgBrush;
		
	case LINECOUNTWND_ID:

		pDC->SetTextColor(m_colorTxtDelta);
		pDC->SetBkColor(m_colorBgdDelta);
		return m_DeltaCountBrush;
		
	}
		
	return hbr;
}
BOOL CLineInfo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnEraseBkgnd(pDC);
}
// Установка цвета и фона результатов емкости 
void CLineInfo::SetColorInPrg(COLORREF colorTxt, COLORREF colorBgd)
{
	m_colorBgdInPrg = colorBgd;
	m_colorTxtInPrg = colorTxt;
	if (m_InPrgBrush != NULL)
	{
		::DeleteObject(m_InPrgBrush);
		m_InPrgBrush = NULL;
	}
	m_InPrgBrush = CreateSolidBrush(m_colorBgdInPrg);
	Invalidate(TRUE);
}
// Установка цветов сопротивления линий
void CLineInfo::SetColorInFact(COLORREF colorTxt, COLORREF colorBgd)
{
	m_colorBgdInFact = colorBgd;
	m_colorTxtInFact = colorTxt;
	if (m_InFactBrush != NULL)
	{
		::DeleteObject(m_InFactBrush);
		m_InFactBrush = NULL;
	}
	m_InFactBrush = CreateSolidBrush(m_colorBgdInFact);
	Invalidate(TRUE);
}
// Установка цветов количества линий на разъеме
void CLineInfo::SetColorDelta(COLORREF colorTxt, COLORREF colorBgd)
{
	m_colorBgdDelta = colorBgd;
	m_colorTxtDelta = colorTxt;
	if (m_DeltaCountBrush != NULL)
	{
		::DeleteObject(m_DeltaCountBrush);
		m_DeltaCountBrush = NULL;
	}
	m_DeltaCountBrush = CreateSolidBrush(m_colorBgdDelta);
	Invalidate(TRUE);
}
// Установка номера линии
void CLineInfo::SetNumLine(int NumLine)
{
	CString m_string;
	m_string.Format(_T("%d"),m_NumLine);
	m_stNumLine.SetWindowText(m_string);
}
// Установка количества линий в программе
void CLineInfo::SetInPrg(int intInPrg)
{
	CString m_string;
	m_InPrg = intInPrg;
	if (intInPrg == ERROR_VALUE)
	{
		m_stInPrg.SetWindowText(_T("X"));
	} 
	else
	{
		m_string.Format(_T("%d"),m_InPrg);
		m_stInPrg.SetWindowText(m_string);

	}
}
// Установка фактического количества линий 
void CLineInfo::SetInFact(int intInFact)
{
	CString m_string;
	m_InFact = intInFact;
	if (intInFact == ERROR_VALUE)
	{
		m_stInFact.SetWindowText(_T("X"));
	} 
	else
	{
		m_string.Format(_T("%d"),m_InFact);
		m_stInFact.SetWindowText(m_string);
	}
}
// Установка разницы между фактическим и измеренным количеством
void CLineInfo::SetDelta(int intDelta)
{
	CString m_string;
	m_Delta = intDelta;

	if (intDelta > ERROR_VALUE - 101) 
	{
		m_stDelta.SetWindowText(_T("X"));
		return;
	}

	if (abs(m_Delta) > 5) 
		SetColorDelta(COLOR_BLACK,COLOR_RED);
	else if (m_Delta == 0) 
		SetColorDelta(COLOR_BLACK,COLOR_GREEN);
	else if ( abs(m_Delta) <= 5 )
		SetColorDelta(COLOR_BLACK,COLOR_YELLOW);
	

	m_string.Format(_T("%d"),m_Delta);
	m_stDelta.SetWindowText(m_string);
	UpdateWindow();
}
