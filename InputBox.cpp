// InputBox.cpp : implementation file
//

#include "stdafx.h"
#include "InputBox.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputBox
#define IDC_EDIT_INPUT		11476
#define IDC_MSGBOX_STATIC	65535
// some metrics
#define CAPTION_HEIGHT	GetSystemMetrics(SM_CYCAPTION)
#define BORDER_HEIGHT	GetSystemMetrics(SM_CYEDGE)
#define BORDER_WIDTH 	GetSystemMetrics(SM_CXEDGE)
#define MARGIN			30

//
// Move horizontally all the buttons of a dialog
//
BOOL CALLBACK MoveButtonsHorz(HWND hwnd, LPARAM lParam)
{
    INT nOffSet = *(INT*)lParam;
	
    TCHAR szBuf[256];
    GetClassName(hwnd, szBuf, 256);
    if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hwnd, &placement);
        CRect rc = placement.rcNormalPosition;	
        ::SetWindowPos(hwnd, NULL, 
			rc.left+nOffSet/2, 
			rc.top, 
			rc.Width(), rc.Height(), SWP_NOZORDER);
    }
    return TRUE;
}

//
// Move vertically all the buttons of a dialog
//
BOOL CALLBACK MoveButtonsVert(HWND hwnd, LPARAM lParam)
{
    INT newTopPos = *(INT*)lParam;
	
    TCHAR szBuf[256];
    GetClassName(hwnd, szBuf, 256);
    if (lstrcmp(szBuf, _T("Button")) == 0)
    {
        // get the window placement (position and size)
        WINDOWPLACEMENT placement;
        ::GetWindowPlacement(hwnd, &placement);
        CRect rc = placement.rcNormalPosition;	
        ::SetWindowPos(hwnd, NULL, 
			rc.left, 
			newTopPos, 
			rc.Width(), rc.Height(), SWP_NOZORDER);
    }
    return TRUE;
}

CInputBox::CInputBox(IN CWnd* pParent)
{
	m_pParent = pParent;
}

CInputBox::~CInputBox()
{
}


BEGIN_MESSAGE_MAP(CInputBox, CWnd)
	//{{AFX_MSG_MAP(CInputBox)
	ON_WM_CREATE()
	ON_MESSAGE(WM_INITDIALOG, OnInitMsgBox)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, OnKillfocusEditInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInputBox message handlers
INT CInputBox::Show(IN LPCTSTR lpszText, OUT CString& strUserInput, IN UINT nIDHelp)
{
    AfxHookWindowCreate(this);
	m_pstrUserInput = &strUserInput;
    return MessageBox(lpszText, _T("Изменение номера"), MB_YESNO);
}

int CInputBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    AfxUnhookWindowCreate();
	return CWnd::OnCreate(lpCreateStruct);
}

LRESULT CInputBox::OnInitMsgBox(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = Default();

	// TODO: Add extra initialization here
	CRect rcDlg, rcStatic;
	HFONT hFont = NULL;

	GetStaticFontNRect(rcStatic, hFont);
	GetDlgRect(rcDlg);

	CenterMe(rcDlg);
	// ajust rect
	rcDlg.bottom -= CAPTION_HEIGHT + BORDER_HEIGHT;
	CRect rcEdit = CreateEdit(rcDlg, rcStatic, hFont);

	CenterButtons(rcDlg, rcEdit);	 	
	return lRes;
}

BOOL CInputBox::CenterMe(CRect& rect)
{
	// set the window pos
	int nOffset = 20 + MARGIN;
    rect.OffsetRect(-BORDER_WIDTH, -CAPTION_HEIGHT-BORDER_HEIGHT);
    MoveWindow(rect.left, rect.top-nOffset/2, rect.Width(), rect.Height()+nOffset);
    CenterWindow(m_pParent);
	return TRUE;
}

BOOL CInputBox::CenterButtons(CRect& rcDlg, CRect& rcEdit)
{
	// make sure the Msgbox is wide enough for the editbox
	CSize szSize(rcEdit.Height() , rcEdit.Width());
    szSize.cx += 2*MARGIN + 2*BORDER_WIDTH + 20;
    if (rcDlg.Width() < szSize.cx)
    {
        // move each button so they appear centered
        int offset = szSize.cx - rcDlg.Width();
        EnumChildWindows(GetSafeHwnd(), MoveButtonsHorz, (LPARAM) &offset);
        // adjust window width
        rcDlg.right = rcDlg.left + szSize.cx;
    }

	rcEdit.bottom += 5;
    EnumChildWindows(GetSafeHwnd(), MoveButtonsVert, (LPARAM)&rcEdit.bottom);

	return TRUE;
}

CRect CInputBox::CreateEdit(CRect& rcDlg, CRect& rcStatic, HFONT& hFont)
{
	INT nHeight = max(rcDlg.Height()-rcDlg.Height()/8, 
					rcStatic.top+rcStatic.Height());
	CRect rcEdit(rcStatic.left, 
				rcStatic.top+rcStatic.Height()+2,
				rcStatic.right, 
				nHeight);
	//Add ES_NUMBER if you want to limit the edit for numeric input
	if (!m_cEdit.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP |
		ES_NUMBER | ES_WANTRETURN, rcEdit, this, IDC_EDIT_INPUT))
		return FALSE;
    m_cEdit.SetFont(CFont::FromHandle(hFont));
	m_cEdit.SetFocus();
	return rcEdit;
}

BOOL CInputBox::GetStaticFontNRect(CRect& rcStatic, HFONT& hFont)
{
	// get standard font (from static)
    HWND hStatic = ::GetDlgItem(GetSafeHwnd(), IDC_MSGBOX_STATIC);
	
	hFont = (HFONT) ::SendMessage(hStatic, WM_GETFONT, 0, 0);
	
	// get the window placement (position and size)
    WINDOWPLACEMENT placement;
    //get the placement of Static
	::GetWindowPlacement(hStatic, &placement);
	rcStatic = placement.rcNormalPosition;

	return TRUE;
}

BOOL CInputBox::GetDlgRect(CRect& rcDlg)
{
	WINDOWPLACEMENT placement;
	GetWindowPlacement(&placement);
    rcDlg = placement.rcNormalPosition;
	rcDlg.bottom -= MARGIN;
	return TRUE;
}

void CInputBox::OnKillfocusEditInput()
{
	CString strUserInput;
	m_cEdit.GetWindowText(strUserInput);
	*m_pstrUserInput = strUserInput;
}
