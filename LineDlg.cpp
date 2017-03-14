// LineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "LineDlg.h"
#include "afxdialogex.h"
#include "PultConst.h"
#include "MFCDlg.h"
#include "InputBox.h"


// CLineDlg dialog

IMPLEMENT_DYNAMIC(CLineDlg, CDialog)

CLineDlg::CLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineDlg::IDD, pParent)

{

}

CLineDlg::~CLineDlg()
{
}

void CLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LINE1, m_Line1);
	DDX_Control(pDX, IDC_LINE2, m_Line2);
	DDX_Control(pDX, IDC_LINE3, m_Line3);
	DDX_Control(pDX, IDC_LINE4, m_Line4);
	DDX_Control(pDX, IDC_LINE5, m_Line5);
	DDX_Control(pDX, IDC_LINE6, m_Line6);
	DDX_Control(pDX, IDC_LINE7, m_Line7);
	DDX_Control(pDX, IDC_LINE8, m_Line8);
	DDX_Control(pDX, IDC_LINE9, m_Line9);
	DDX_Control(pDX, IDC_LINE10, m_Line10);
	DDX_Control(pDX, IDC_LINE11, m_Line11);
	DDX_Control(pDX, IDC_LINE12, m_Line12);
	DDX_Control(pDX, IDC_LINE13, m_Line13);
	DDX_Control(pDX, IDC_LINE14, m_Line14);
	DDX_Control(pDX, IDC_LINE15, m_Line15);
	DDX_Control(pDX, IDC_LINE16, m_Line16);
	DDX_Control(pDX, IDC_LINE17, m_Line17);
	DDX_Control(pDX, IDC_LINE18, m_Line18);
	DDX_Control(pDX, IDC_LINE19, m_Line19);
	DDX_Control(pDX, IDC_LINE20, m_Line20);
	DDX_Control(pDX, IDC_LINE21, m_Line21);
	DDX_Control(pDX, IDC_LINE22, m_Line22);
	DDX_Control(pDX, IDC_LINE23, m_Line23);
	DDX_Control(pDX, IDC_LINE24, m_Line24);
	DDX_Control(pDX, IDC_LINE25, m_Line25);
	DDX_Control(pDX, IDC_LINE26, m_Line26);
	DDX_Control(pDX, IDC_LINE27, m_Line27);
	DDX_Control(pDX, IDC_LINE28, m_Line28);
	DDX_Control(pDX, IDC_LINE29, m_Line29);
	DDX_Control(pDX, IDC_LINE30, m_Line30);
	DDX_Control(pDX, IDC_LINE31, m_Line31);
	DDX_Control(pDX, IDC_LINE32, m_Line32);
	DDX_Control(pDX, IDC_LINE33, m_Line33);
	DDX_Control(pDX, IDC_LINE34, m_Line34);
	DDX_Control(pDX, IDC_LINE35, m_Line35);
	DDX_Control(pDX, IDC_LINE36, m_Line36);
	DDX_Control(pDX, IDC_LINE37, m_Line37);
	DDX_Control(pDX, IDC_LINE38, m_Line38);
	DDX_Control(pDX, IDC_LINE39, m_Line39);
	DDX_Control(pDX, IDC_LINE40, m_Line40);
	DDX_Control(pDX, IDC_CURRENT_PULT, m_CurrentPult);
	DDX_Control(pDX, IDC_PROTOCOL, m_stProtocol);
	DDX_Control(pDX, IDC_PROGRESS_LINE, m_ProgressRpt);
	DDX_Control(pDX, IDC_BUTTON_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_BUTTON_WRITE_PRG, m_btnWrite);
	DDX_Control(pDX, IDC_BUTTON_CHECK_PRG, m_btnCheck);
	DDX_Control(pDX, IDC_BUTTON_CHANGE_NUMPULT, m_btnChange);
	DDX_Control(pDX, IDC_BUTTON_SYNC_TIME, m_SyncTime);
	DDX_Control(pDX, IDC_TIME_IMP, m_btnPulse);
	DDX_Control(pDX, IDC_LINE_INFO, m_LineInfo);
}


BEGIN_MESSAGE_MAP(CLineDlg, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_SYNC_TIME, &CLineDlg::OnBnClickedButtonSyncTime)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CLineDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_PRG, &CLineDlg::OnBnClickedButtonWritePrg)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_PRG, &CLineDlg::OnBnClickedButtonCheckPrg)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_NUMPULT, &CLineDlg::OnBnClickedButtonChangeNumpult)
	ON_BN_CLICKED(IDC_TIME_IMP, &CLineDlg::OnBnClickedTimeImp)
	ON_COMMAND_RANGE(IDC_RESCAN1, IDC_RESCAN40,&CLineDlg::OnBnClickedRescan)
END_MESSAGE_MAP()


// CLineDlg message handlers


int CLineDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

		m_pParent = (CMFCDlg*)GetParent();

	return 0;
}


void CLineDlg::OnBnClickedButtonSyncTime()
{
	if (m_pParent->m_CurrentPult == -1) return;

	m_pParent->m_DevRS485->SyncTime();
}


BOOL CLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CLineInfo* ci;
	for (int x=IDC_LINE1;x<(IDC_LINE40+1);x++)
	{
		ci=(CLineInfo*)GetDlgItem(x);
		ci->SetNumLine(x-IDC_LINE1+1);
	}

	m_CurrentPult.SetWindowText(_T(""));
	m_stProtocol.SetWindowText(_T(""));

	//Прогресс- бар скрыт
	m_ProgressRpt.ShowWindow(SW_HIDE);

	//Инициализация пояснительной инфорации
	m_LineInfo.m_stInPrg.SetWindowText(_T("в программе"));
	m_LineInfo.m_stInFact.SetWindowText(_T("обнаружено"));
	m_LineInfo.m_stDelta.SetWindowText(_T("разница"));
	m_LineInfo.m_stNumLine.SetWindowText(_T("№"));

	CenterWindow(this);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CLineDlg::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message == WM_KEYUP) || (pMsg->message == WM_KEYDOWN))
	{
		m_pParent->m_ListPrg.SetFocus();	

	}
	return CWnd::PreTranslateMessage(pMsg);
}
// Установить запись протокола работы
void CLineDlg::SetProtocol(CString str)
{
	m_stProtocol.SetWindowText(str);
}


void CLineDlg::OnBnClickedButtonScan()
{

	int CurrentPult = m_pParent->m_CurrentPult;

	if(CurrentPult == -1) return;
	KillTimer(m_pParent->m_TimerEcho);

	BYTE NumPult = m_pParent->m_PultArray[CurrentPult].m_NumPult;
	CString str;
	CLineInfo* ci;

	
	m_ProgressRpt.SetRange(0,MAX_LINE);
	m_ProgressRpt.ShowWindow(SW_SHOW);

	for (int i = 0; i<MAX_LINE; i++)
	{
		str.Format(_T("Опрос пульта №%d линии %d"), NumPult,i+1);
		SetProtocol(str);
		m_pParent->ScanPult(CurrentPult,i+1);
		ci=(CLineInfo*)GetDlgItem(i+IDC_LINE1);
		ci->SetInFact(m_pParent->m_PultArray[CurrentPult].m_CountLine[i]);
		ci->SetDelta(m_pParent->m_PultArray[CurrentPult].m_CountLine[i]
		- ci->m_InPrg);
		m_ProgressRpt.SetPos(i);
	}

	m_ProgressRpt.ShowWindow(SW_HIDE);
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
}


void CLineDlg::OnBnClickedButtonWritePrg()
{
	if (m_pParent->m_CurrentPult == -1) return;
	
	
	CString str = ""; 
	m_pParent->m_ProgressRpt.ShowWindow(SW_SHOW);


	str.Format(_T("Запись программы в пульт №%d"),m_pParent->m_PultArray.GetAt(m_pParent->m_CurrentPult).m_NumPult);
	SetProtocol(str);
	
	KillTimer(m_pParent->m_TimerEcho);
	if (!m_pParent->WritePrgToPult(m_pParent->m_PultArray.GetAt(m_pParent->m_CurrentPult).m_SerNumPult))
	{
		SetProtocol(_T("Не удалось записать программу"));
	} 
	else
	{
		SetProtocol(_T("Программа успешно записана"));
	}
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	m_pParent->m_ProgressRpt.ShowWindow(SW_HIDE);
}


void CLineDlg::OnBnClickedButtonCheckPrg()
{
	
	if (m_pParent->m_CurrentPult == -1) return;
	if (!m_pParent->m_PrgLoaded) return;

	CString str = "";
	m_pParent->m_ProgressRpt.ShowWindow(SW_SHOW);

	KillTimer(m_pParent->m_TimerEcho);
	if (!m_pParent->CheckPrg(m_pParent->m_PultArray.GetAt(m_pParent->m_CurrentPult).m_SerNumPult))
	{
		SetProtocol(_T("Программы не совпадают"));

	}
	else
	{
		SetProtocol(_T("Программа удачно проверена"));
	}
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	m_pParent->m_ProgressRpt.ShowWindow(SW_HIDE);
}


void CLineDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
	return;
}


void CLineDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnCancel();
	return;
}


void CLineDlg::OnBnClickedButtonChangeNumpult()
{
	if (m_pParent->m_CurrentPult == -1) return;

	int NewNumPult;
	CInputBox theInputBox(this);
	CString strUserInput,tmp;
	int userReply;




	userReply = theInputBox.Show(_T("Введите новый номер пульта ..."), strUserInput);

	if (IDYES == userReply)
	{

		try
		{
			NewNumPult = StrToInt(strUserInput);
		
			for(int i = 0; i<m_pParent->m_PultArray.GetCount(); i++)
				if (NewNumPult == m_pParent->m_PultArray[i].m_NumPult)
					{
					MessageBox(_T("Пульт с данным номером существует"),
						_T("Дублирование номера пульта"),MB_OK | MB_ICONWARNING);
					return;
					}
			if ((NewNumPult < 1) || (NewNumPult > 128))
			{
				MessageBox(_T("Номер пульта должен быть от 1 до 128"),
					_T("Неверный номер пульта"),MB_OK | MB_ICONWARNING);
				return;
			}

			if(m_pParent->ChangeNumPult(NewNumPult))
			{

				tmp.Format(_T("Выбран пульт №%d"),NewNumPult);
				m_CurrentPult.SetWindowText(tmp);
				UpdateData(FALSE);
				MessageBox(_T("Номер пульта успешно изменен"),
					_T("Изменение номера пульта"),MB_OK);

			}
			else
			{
				MessageBox(_T("Не удалось изменить номер пульта"),
					_T("Ошибка"),MB_OK | MB_ICONWARNING);
			}
		}
		catch (...)
		{
		MessageBox(_T("Не удалось изменить номер пульта"),
				_T("Ошибка"),MB_OK | MB_ICONWARNING);
		}
	}
}


void CLineDlg::OnBnClickedTimeImp()
{
	if (m_pParent->m_CurrentPult == -1) return;

	int NewPulse;
	int OldPulse;
	CInputBox theInputBox(this);
	CString strUserInput,tmp, tmp1;
	int userReply;

	//Считываем длительность импульса
	OldPulse = m_pParent->GetPulse();

	tmp1.Format(_T("В пульте установлено - %d мс \nВведите длительность импульса в мс ..."),OldPulse);
	
	userReply = theInputBox.Show(tmp1, strUserInput);


	if (IDYES == userReply)
	{

		try
		{
			NewPulse = StrToInt(strUserInput);
			if ((NewPulse < 1) || (NewPulse > 1000))
				{
					MessageBox(_T("Длительность импульса должна быть от 1 до 1000 мс"),
						_T("Неверная длительность импульса"),MB_OK | MB_ICONWARNING);
					return;
				}

				if(m_pParent->SetPulse(NewPulse))
				{

					tmp.Format(_T("Установлена длительность импульса запуска %d мс"),NewPulse);
					SetProtocol(tmp);
					UpdateData(FALSE);
					MessageBox(_T("Длительность импульса успешно изменена"),
						_T("Изменение длительности импульса"),MB_OK);

				}
				else
				{
					MessageBox(_T("Не удалось изменить длительность импульса"),
						_T("Ошибка"),MB_OK | MB_ICONWARNING);
				}
		}
		catch (...)
		{
			MessageBox(_T("Не удалось изменить длительность импульса"),
				_T("Ошибка"),MB_OK | MB_ICONWARNING);
		}
	}
}


void CLineDlg::OnBnClickedRescan(UINT nID)
{
	int CurrentPult = m_pParent->m_CurrentPult;
	if(CurrentPult == -1) return;
	KillTimer(m_pParent->m_TimerEcho);
	BYTE NumPult = m_pParent->m_PultArray[CurrentPult].m_NumPult;
	BYTE NumLine = nID - IDC_RESCAN1; //0..39


	CString str;
	CLineInfo* ci;

	str.Format(_T("Опрос пульта №%d линии %d"), NumPult,NumLine+1);
	SetProtocol(str);
	m_pParent->ScanPult(CurrentPult,NumLine+1);
	ci=(CLineInfo*)GetDlgItem(NumLine+IDC_LINE1);
	
	ci->SetInFact(m_pParent->m_PultArray[CurrentPult].m_CountLine[NumLine]);
	ci->SetDelta(m_pParent->m_PultArray[CurrentPult].m_CountLine[NumLine]
	- ci->m_InPrg);
	m_pParent->m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
}


