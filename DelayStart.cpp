// DelayStart.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "DelayStart.h"
#include "afxdialogex.h"
#include "MFCDlg.h"
#include "function.h"


// CDelayStart dialog

IMPLEMENT_DYNAMIC(CDelayStart, CDialog)

CDelayStart::CDelayStart(CWnd* pParent /*=NULL*/)
	: CDialog(CDelayStart::IDD, pParent)
	, m_TimeStart(COleDateTime::GetCurrentTime())

{
		m_pParent = (CMFCDlg*) pParent;
}

CDelayStart::~CDelayStart()
{
}

void CDelayStart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_TIME_PUSK, m_TimeStart);
	DDX_Control(pDX, IDC_TIME_COMP, m_StartComp);
	DDX_Control(pDX, IDC_TIME_PULT, m_StartPult);
}


BEGIN_MESSAGE_MAP(CDelayStart, CDialog)
	ON_BN_CLICKED(IDOK, &CDelayStart::OnBnClickedOk)
END_MESSAGE_MAP()


// CDelayStart message handlers


BOOL CDelayStart::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_StartPult.SetCheck(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDelayStart::OnBnClickedOk()
{
	COleDateTimeSpan delta;
	UpdateData(TRUE);
	delta = m_TimeStart-COleDateTime::GetCurrentTime();


	if (m_StartPult.GetCheck())
	{
		// ќжидание старта с пульта
		m_pParent->m_WaitFromClock=false;
	} 
	else
	{

		if(delta.m_span<0)
		{
			AfxMessageBox (_T("¬рем€ пуска меньше системного времени")); 
			return;
		}

		// ќжидание старта по системным часам
		m_pParent->m_TimeStart = m_TimeStart;
		m_pParent->m_WaitFromClock=true;
	}
	m_pParent->m_CompControl=true;
	m_pParent->m_PrgArray[m_pParent->m_CurrentPrg].m_CompControl=true;
	m_pParent->ShowWaitStart();


	CDialog::OnOK();
}
