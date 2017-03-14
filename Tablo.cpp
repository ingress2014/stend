// Tablo.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "Tablo.h"
#include "PultConst.h"


// CTablo


#define TABLO_ID	1107

IMPLEMENT_DYNAMIC(CTablo, CWnd)

CTablo::CTablo()
	: m_colorBgd(COLOR_WHITE)
	, m_colorText(COLOR_BLACK)
	, m_Text(_T("0:00:00.00"))
{
	//Регистрация окна
	RegisterWndClass();
	//Регистрация шрифта 
	m_Font.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial Narrow"));
	//Устанавливаем фон окна
	m_hBrush = ::CreateSolidBrush(COLOR_WHITE);
}

CTablo::~CTablo()
{
	m_Font.DeleteObject();
	::DeleteObject(m_hBrush);
}

BOOL CTablo::RegisterWndClass()
{
	WNDCLASS wndclass;
	//Проверка класс зарегистрирован?
	if(::GetClassInfo(AfxGetInstanceHandle(),TABLO_CLASSNAME,&wndclass))
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
	wndclass.lpszClassName = TABLO_CLASSNAME;
	return ::AfxRegisterClass(&wndclass);
}


// Изменение размера шрифта в зависимости от размера окна
void CTablo::SetFontHeight(CFont* pFont, int nMaxHeight)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	pFont->Detach();

	// Изменяем шрифт
	lf.lfHeight=nMaxHeight;

	// Обновляем
	pFont->CreateFontIndirect(&lf);
}



BEGIN_MESSAGE_MAP(CTablo, CWnd)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CTablo message handlers





void CTablo::OnSetFocus(CWnd* pOldWnd)
{
	Invalidate(FALSE);

}


void CTablo::OnKillFocus(CWnd* pNewWnd)
{
	Invalidate(FALSE);
}


void CTablo::OnPaint()
{
	CPaintDC dc(this); 
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect drawRect(wp.rcNormalPosition);
	drawRect.MoveToXY(0,0);
	dc.Draw3dRect(&drawRect, RGB(64,64,64), RGB(192,192,192));
}


void CTablo::PreSubclassWindow()
{
	CString tmp;


	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect cntrlRect=wp.rcNormalPosition; //Размер всего элемента
	cntrlRect.MoveToXY(0,0); //Переместились в верхний левый угол

	VERIFY(m_stText.Create(_T(m_Text),WS_VISIBLE|WS_CHILD|SS_CENTER|SS_SUNKEN,
		CRect(cntrlRect.left,cntrlRect.top,cntrlRect.right,cntrlRect.bottom),this,
		TABLO_ID));

	SetFontHeight(&m_Font,cntrlRect.bottom);
	//Устанавливаем шрифт окна
	m_stText.SetFont(&m_Font);

	CWnd::PreSubclassWindow();
}


HBRUSH CTablo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetTextColor(m_colorText);
	pDC->SetBkColor(m_colorBgd);
	return m_hBrush;

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


// Установка значения табло
void CTablo::SetText(CString str)
{
	m_stText.SetWindowText(str);
}
