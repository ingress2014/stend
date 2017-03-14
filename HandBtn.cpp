// HandBtn.cpp : implementation file
//
#include "stdafx.h"
#include "MFC.h"
#include "HandBtn.h"
#include "function.h"
#include "PultConst.h"



// CHandBtn

#define RESULTSWND_ID			1
#define NUM_BUTTON_ID			2

#define NUMBERBUTTONS_BASEID	100

IMPLEMENT_DYNAMIC(CHandBtn, CWnd)

CHandBtn::CHandBtn()
: m_ClickCount(0)
, m_NumBtn(0)
, m_clrResultsBk(RGB(200,200,200))
, m_clrResultsTxt(RGB(46,74,230))
, m_clrNumTxt(RGB(0,0,0))
, m_clrNumBk(RGB(255,255,255))
, m_NameBtn(_T("A"))
, m_State(FALSE)
, m_Checked(FALSE)
, m_IsMotor(FALSE)
, m_Key(0)
{   //����������� ����
	RegisterWndClass();
	//����������� ������� ������ � ������
	m_ResultsFont.CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial Narrow"));
	
	m_ButtonFont.CreateFont(100, 100, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Tahoma"));
}

CHandBtn::~CHandBtn()
{
	
}

BOOL CHandBtn::RegisterWndClass()
{
	WNDCLASS wndclass;
	//�������� ����� ���������������?
	if(::GetClassInfo(AfxGetInstanceHandle(),HANDBTN_CLASSNAME,&wndclass))
		return TRUE;
	// ����������� ������ ������������ ������ Windows

	wndclass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;;
	wndclass.lpfnWndProc = ::DefWindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = ::AfxGetInstanceHandle();
	wndclass.hIcon = NULL;
	wndclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = HANDBTN_CLASSNAME;
	return ::AfxRegisterClass(&wndclass);
}


BEGIN_MESSAGE_MAP(CHandBtn, CWnd)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(NUMBERBUTTONS_BASEID,OnPressButton)
	ON_WM_ERASEBKGND()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CHandBtn message handlers




void CHandBtn::OnPaint()
{
	CPaintDC dc(this); 
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect drawRect(wp.rcNormalPosition);
	drawRect.MoveToXY(0,0);
	dc.Draw3dRect(&drawRect, RGB(64,64,64), RGB(192,192,192));
	
}


void CHandBtn::OnSetFocus(CWnd* pOldWnd)
{
	Invalidate(FALSE);

}


void CHandBtn::OnKillFocus(CWnd* pNewWnd)
{

	Invalidate(FALSE);

}



int CHandBtn::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


void CHandBtn::PreSubclassWindow()
{
	WINDOWPLACEMENT wp;
	GetWindowPlacement(&wp);
	CRect cntrlRect=wp.rcNormalPosition; //������ ����� ��������
	cntrlRect.MoveToXY(0,0); //������������� � ������� ����� ����

	//�������� ������ ������
	//����� 50% �� �������� ����� ����
	int nCtrlHeight = cntrlRect.Height()/2;
	int nCtrlWidth = cntrlRect.Width()/2;
	CString tmp;
	tmp.Format(_T("%d"),m_NumBtn);
	VERIFY(m_NumCtrl.Create(_T(tmp),WS_VISIBLE|WS_CHILD|ES_CENTER|ES_AUTOHSCROLL|ES_READONLY,
		CRect(cntrlRect.left+2,cntrlRect.top+2,nCtrlWidth,nCtrlHeight),this,
		NUM_BUTTON_ID));

	// ������� ������ ������
	SetFontHeight(&m_ResultsFont, nCtrlHeight);
	//������������� ����� ����
	m_NumCtrl.SetFont(&m_ResultsFont);
	//������������� ��� ����
	m_NumCtrlBrush.CreateSolidBrush(m_clrNumBk);

	// �������� ���������
	// 1: ������� ������� 
	int nControlHeight=cntrlRect.Height()/2; //����� 50 % ������ ����
	VERIFY(m_ResultsWnd.Create(_T("0"),WS_VISIBLE|WS_CHILD|ES_CENTER|ES_AUTOHSCROLL|ES_READONLY,
		CRect(cntrlRect.left+2+nCtrlWidth, cntrlRect.top+2, cntrlRect.right, nControlHeight), this,
		RESULTSWND_ID)); //���� ������ ���������� �������
	// ������� ������ ������
	SetFontHeight(&m_ResultsFont, nControlHeight);
    //������������� ����� ����
	m_ResultsWnd.SetFont(&m_ResultsFont);
    //������������� ��� ����
	m_ResultsWndBrush.CreateSolidBrush(m_clrResultsBk);
	

	//������

	// ��������� ����� � ������ ������
	int nBtnWidth=(cntrlRect.Width())-4;
	int nBtnHeight=(cntrlRect.Height()-nControlHeight);


	// ��������� ������ � ������ ������ �� ������
	int nBtnMaxWdth=0, nBtnMaxHeight=0;
	CPaintDC dc(this);
	dc.SelectObject(m_ButtonFont);
	CRect lblRect;
 
	lblRect.SetRectEmpty();
	dc.DrawText(m_NameBtn, lblRect, DT_CALCRECT);
	nBtnMaxWdth=max(nBtnMaxWdth, lblRect.Width());
	nBtnMaxHeight=max(nBtnMaxHeight, lblRect.Height());

	LOGFONT lf;
	m_ButtonFont.GetLogFont(&lf);
	m_ButtonFont.Detach();
	lf.lfWidth*=(.6*nBtnWidth/nBtnMaxWdth);
	lf.lfHeight*=(.6*nBtnHeight/nBtnMaxHeight);
	m_ButtonFont.CreateFontIndirect(&lf);
	CColorButton* pButton = &m_Btn;
	//	CButton* pButton = &m_Btn;

	VERIFY(pButton->Create(m_NameBtn,WS_VISIBLE|WS_CHILD|BS_OWNERDRAW,
		 CRect(2,nControlHeight,nBtnWidth,cntrlRect.bottom -2), this, NUMBERBUTTONS_BASEID));


	CWnd::PreSubclassWindow();
}


// ��������� ������� ������ � ����������� �� ������� ����
void CHandBtn::SetFontHeight(CFont* pFont, int nMaxHeight)
{
	LOGFONT lf;
	pFont->GetLogFont(&lf);
	pFont->Detach();

	// �������� �����
	lf.lfHeight=nMaxHeight;

	// ���������
	pFont->CreateFontIndirect(&lf);
}



HBRUSH CHandBtn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	
	int nItem = pWnd->GetDlgCtrlID();
	switch(nItem)
	{
	case RESULTSWND_ID:
		pDC->SetTextColor(m_clrResultsTxt);
		pDC->SetBkColor(m_clrResultsBk);
		return (HBRUSH)(m_ResultsWndBrush.GetSafeHandle());

	case NUM_BUTTON_ID:
		pDC->SetTextColor(m_clrNumTxt);
		pDC->SetBkColor(m_clrNumBk);
		return (HBRUSH)(m_NumCtrlBrush.GetSafeHandle());

	}



	return hbr;
}


BOOL CHandBtn::OnEraseBkgnd(CDC* pDC)
{
	
	return CWnd::OnEraseBkgnd(pDC);
}


UINT CHandBtn::OnGetDlgCode()
{

    return DLGC_WANTALLKEYS;
	// return CWnd::OnGetDlgCode();
}


void CHandBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
		

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CHandBtn::OnPressButton(void)
{

	//��������� ������ �����
	//���� ����������� �������� ��� ������
	if (m_State)
	{
		//������ ��� ��������
		if (m_Checked)
		{
			//�����������
			ResetColorBtn();
			m_Checked = FALSE;
		} 
		else
		{
			SetColorBtn(COLOR_CHECK_TXT,COLOR_CHECK_BGR);
			m_Checked = TRUE;
		}
	} 
    //����� ���������� ��������
//	else
//	{

//		m_ClickCount++;
//		CString m_string;
//		m_string.Format(_T("%d"),m_ClickCount);
//		m_ResultsWnd.SetWindowText(m_string);
		
//	}


	//��������� ������ �������
	//��������� � 32 �� 40 ������
//	if (m_IsMotor)
//	{
		//������ ��� ��������
//		if (m_Checked)
//		{
			//�����������
//			ResetColorBtn();
//			m_Checked = FALSE;
//		} 
//		else
//		{
//			SetColorBtn(COLOR_MOTOR_TXT,COLOR_MOTOR_BGR);
//			m_Checked = TRUE;
//		}
//	} 
}


BOOL CHandBtn::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


// ��������� ������ ������
void CHandBtn::SetNumBtn(int NumBtn)
{
	m_NumBtn = NumBtn;
	CString m_string;
	m_string.Format(_T("%d"),m_NumBtn);
	m_NumCtrl.SetWindowText(m_string);
}


// ��������� ������ ������
void CHandBtn::SetNameBtn(CString NameBtn)
{
	m_Btn.SetWindowText(NameBtn);
}



// ��������� ����� ������
void CHandBtn::SetColorBtn(COLORREF clrTxt, COLORREF clrBkgrnd)
{
	m_Btn.SetColour(clrTxt,clrBkgrnd);
}


// ����� ������ ������
void CHandBtn::ResetColorBtn(void)
{
	m_Btn.ResetColour(true);
}


// ��������� �������� ��� ������� ������
void CHandBtn::IncButton(void)
{
	m_ClickCount++;
	CString m_string;
	m_string.Format(_T("%d"),m_ClickCount);
	m_ResultsWnd.SetWindowText(m_string);
}
