// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "MsgDlg.h"
#include "afxdialogex.h"
#include "PultConst.h"
#include "MFCDlg.h"


// CMsgDlg dialog

IMPLEMENT_DYNAMIC(CMsgDlg, CDialog)

CMsgDlg::CMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDlg::IDD, pParent)
	, m_colorDlg(COLOR_RED)
	
	, m_EnableClose(false)
{
	m_Font.CreateFont(15, 9, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, _T("Tahoma"));

		m_pParent = (CMFCDlg*) pParent;
}

CMsgDlg::~CMsgDlg()
{
	m_Font.DeleteObject();
}

void CMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG, m_stText);
}


BEGIN_MESSAGE_MAP(CMsgDlg, CDialog)
	ON_WM_CTLCOLOR()
//	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CMsgDlg message handlers


void CMsgDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}


BOOL CMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Brush.CreateSolidBrush(m_colorDlg);
	m_stText.SetFont(&m_Font);
	CenterWindow();

	/*
	// Get current font.
	CFont* pFont = GetDlgItem( IDC_STATIC_ITALIC )->GetFont();
	LOGFONT LogFont = { 0 };
	pFont->GetLogFont( &LogFont );

	// Create new font with underline style.
	LogFont.lfUnderline = TRUE;
	m_StaticFont.CreateFontIndirect( &LogFont );

	// Sets the new font back to static text.
	GetDlgItem( IDC_STATIC_ITALIC )->SetFont( &m_StaticFont );

	
	*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



// Установка цвета фона диалога
void CMsgDlg::SetColorDlg(COLORREF colorDlg)
{
	m_Brush.DeleteObject();
	m_Brush.CreateSolidBrush(colorDlg);
	Invalidate(TRUE);
}


// Установка текста диалога сообщений
void CMsgDlg::SetTextDlg(CString textDlg)
{
	m_stText.SetWindowText(textDlg); 
}


HBRUSH CMsgDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(m_colorDlg);
	return (HBRUSH)m_Brush;
}



void CMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_EnableClose)	this->ShowWindow(SW_HIDE);
	CDialog::OnLButtonDown(nFlags, point);
}


void CMsgDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnRButtonUp(nFlags, point);
}


void CMsgDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_EnableClose)	this->ShowWindow(SW_HIDE);
	CDialog::OnCancel();
}


BOOL CMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message == WM_KEYUP) || (pMsg->message == WM_KEYDOWN))
	{
		m_pParent->m_ListPrg.SetFocus();	

	}
	return CWnd::PreTranslateMessage(pMsg);
}
