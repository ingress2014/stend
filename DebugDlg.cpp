// DebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "DebugDlg.h"
#include "afxdialogex.h"
#include "MFCDlg.h"
#include "PultConst.h"



// CDebugDlg dialog

IMPLEMENT_DYNAMIC(CDebugDlg, CDialog)

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
	, m_Res(_T(""))
	, m_Cap(_T(""))
{

}

CDebugDlg::~CDebugDlg()
{
	
}

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RES, m_Res);
	DDX_Text(pDX, IDC_CAP, m_Cap);
	DDX_Control(pDX, IDC_CAP, m_stCap);
	DDX_Control(pDX, IDC_RES, m_stRes);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ON_HI, &CDebugDlg::OnBnClickedButtonOnHi)
	ON_BN_CLICKED(IDC_BUTTON_OFF_HI, &CDebugDlg::OnBnClickedButtonOffHi)
	ON_BN_CLICKED(IDC_BUTTON_ON_LO, &CDebugDlg::OnBnClickedButtonOnLo)
	ON_BN_CLICKED(IDC_BUTTON_OFF_LO, &CDebugDlg::OnBnClickedButtonOffLo)
	ON_BN_CLICKED(IDC_BUTTON_ON_CAP_RELAY, &CDebugDlg::OnBnClickedButtonOnCapRelay)
	ON_BN_CLICKED(IDC_BUTTON_OFF_CAP_RELAY, &CDebugDlg::OnBnClickedButtonOffCapRelay)
	ON_BN_CLICKED(IDC_BUTTON_ON_RES_RELAY, &CDebugDlg::OnBnClickedButtonOnResRelay)
	ON_BN_CLICKED(IDC_BUTTON_OFF_RES_RELAY, &CDebugDlg::OnBnClickedButtonOffResRelay)
	ON_BN_CLICKED(IDC_BUTTON_GET_RES, &CDebugDlg::OnBnClickedButtonGetRes)
	ON_WM_CREATE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_GET_CAP, &CDebugDlg::OnBnClickedButtonGetCap)
	ON_BN_CLICKED(IDC_SET_LINE1, &CDebugDlg::OnBnClickedSetLine1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDebugDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDebugDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CLEAR_LINE, &CDebugDlg::OnBnClickedClearLine)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDebugDlg message handlers


void CDebugDlg::OnBnClickedButtonOnHi()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnHi(1,TRUE);
}


void CDebugDlg::OnBnClickedButtonOffHi()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnHi(1,FALSE);
}


void CDebugDlg::OnBnClickedButtonOnLo()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnLo(1,TRUE);
}


void CDebugDlg::OnBnClickedButtonOffLo()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnLo(1,FALSE);
}


void CDebugDlg::OnBnClickedButtonOnCapRelay()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnCapRelay(1,TRUE);
}


void CDebugDlg::OnBnClickedButtonOffCapRelay()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnCapRelay(1,FALSE);
}


void CDebugDlg::OnBnClickedButtonOnResRelay()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnResRelay(1,TRUE);
}


void CDebugDlg::OnBnClickedButtonOffResRelay()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->OnResRelay(1,FALSE);
}


void CDebugDlg::OnBnClickedButtonGetRes()
{

	if (m_pParent->m_CurrentPult == -1) return;
	KillTimer(m_pParent->m_TimerEcho);
	int tmp;
	CString str;
	m_pParent->m_DevRS485->GetRes(m_pParent->m_PultArray[m_pParent->m_CurrentPult].m_SerNumPult,1);
	m_pParent->m_PultArray[0].m_ResLine[0] = ((m_pParent->m_DevRS485->m_RxBuffer[11] << 8) | m_pParent->m_DevRS485->m_RxBuffer[10]);
	tmp = m_pParent->m_PultArray[m_pParent->m_CurrentPult].m_ResLine[0];
	str.Format(_T("%d"),tmp);
	m_stRes.SetWindowText(str);
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);

}



void CDebugDlg::PostNcDestroy()
{
	delete this;
	CDialog::PostNcDestroy();

}


void CDebugDlg::OnOK()
{
	DestroyWindow();
}


void CDebugDlg::OnCancel()
{
	DestroyWindow();
}


int CDebugDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pParent = (CMFCDlg*)GetParent();

	SetClassLong(this->GetSafeHwnd(),GCL_HBRBACKGROUND,LONG(RGB(0,0,0)));


	return 0;
}


void CDebugDlg::OnBnClickedButtonGetCap()
{
	if (m_pParent->m_CurrentPult == -1) return;

	KillTimer(m_pParent->m_TimerEcho);
	int tmp;
	CString str;
	m_pParent->m_DevRS485->GetCap(m_pParent->m_PultArray[m_pParent->m_CurrentPult].m_SerNumPult,1);
	m_pParent->m_PultArray[0].m_CapLine[0] = ((m_pParent->m_DevRS485->m_RxBuffer[11] << 8) | m_pParent->m_DevRS485->m_RxBuffer[10]);
	tmp = m_pParent->m_PultArray[m_pParent->m_CurrentPult].m_CapLine[0];
	str.Format(_T("%d"),tmp);
	m_stCap.SetWindowText(str);
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);


}


void CDebugDlg::OnBnClickedSetLine1()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->SetLineInPult(1,1);
}


void CDebugDlg::OnBnClickedButton2()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_MsgDlg->ShowWindow(SW_SHOW);
}


void CDebugDlg::OnBnClickedButton3()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_MsgDlg->ShowWindow(SW_HIDE);
}


void CDebugDlg::OnBnClickedClearLine()
{
	if (m_pParent->m_CurrentPult == -1) return;

		m_pParent->m_DevRS485->ResetLine(1);
}



BOOL CDebugDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	CenterWindow();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDebugDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


	return CDialog::OnEraseBkgnd(pDC);
}
