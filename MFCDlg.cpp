// MFCDlg.cpp : ���� ����������
//
#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "afxdialogex.h"
#include "function.h"
#include "PultConst.h"
#include "ftd2xx.h"
#include <Dbt.h>
#include "RS485.h"
#include "cputicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static BYTE Prg[MAX_SIZE_PRG];//����� ��� ������ ���������
//�������� ������� ��������� � 10 ��
volatile DWORD MainTimer = 0;
//������ ������� �������� ������� ����� � ������
volatile DWORD WaitTimer = 0;

//����� ������ �������������� �������
UINT FindPult(LPVOID pArg);
//����� ���������� ��������� �� ������ ���������
UINT WorkThread (LPVOID pArg);
//�������� ������� ������ � ������ � 
//���������� ������ ������
UINT WaitStartFromPult(LPVOID pArg);


//����� ������ �������������� �������
UINT FindPult(LPVOID pArg)
{
	//�������� ��������� �� ��������� ����� �������
	CMFCDlg *pMyHndl = (CMFCDlg *)pArg;
	if(pMyHndl->m_FindStart) return 0;

	pMyHndl->m_FindStart = TRUE;

	int i=1;
	while(pMyHndl->m_FindStart)
	{
		pMyHndl->m_ProgressRpt.SetPos(i);
	//	pMyHndl->m_ProgressRpt.UpdateData(FALSE);
		pMyHndl->UpdateWindow();
		//���� ����� ��������� � ��� ��� � ������, �� ��������� � ������
		if(pMyHndl->m_DevRS485->PultIsPresent(i) && !pMyHndl->PultInList(i))
		{

			pMyHndl->m_CurrentPult++;
			pMyHndl->AddPult(i);
			pMyHndl->UpdateWindow();
		}
		i++;
		if (i > MAX_PULT) i=1;


	}

	//���� ���������� ������
	if (pMyHndl->m_CurrentPult >= 0)
	{
		pMyHndl->m_PultList.SetSel(pMyHndl->m_CurrentPult,TRUE);
		//������� ������������� ������� � �����������
		pMyHndl->m_DevRS485->SyncTime();
	}


	if (pMyHndl->FindDouble()) AfxMessageBox(_T("���������� ������ � ����������� ��������"));


return 0;
}

//�������� ������� ������ � ������
UINT WaitStartFromPult(LPVOID pArg)
{
	//�������� ��������� �� ��������� ����� �������
	CMFCDlg *pMyHndl = (CMFCDlg *)pArg;
	if(pMyHndl->m_WaitStart) return 0;
	pMyHndl->fContinueWaitStart = TRUE;
	//���� - ����� �������
	pMyHndl->m_WaitStart = TRUE;
	COleDateTimeSpan dtDelta;

	//��������� � �������� �� 10 ����������� ��������� �����
	pMyHndl->m_WaitTime = GetTickCount()/10;

	while (pMyHndl->fContinueWaitStart)
	{
		//���� ������ �� ������� � ����������
		if (pMyHndl->m_WaitFromClock)
		{
			dtDelta = pMyHndl-> m_TimeStart-COleDateTime::GetCurrentTime();
			//WaitTimer � �������� �� 10 ��
			WaitTimer = (DWORD)(dtDelta.GetTotalSeconds()*100);

			if (WaitTimer<100)
			{
			
			//������������� � ����� ���������� � ����������
			pMyHndl->OnBnClickedCompReg();

			//�������� ������� � ������
			SendMessage(pMyHndl->GetSafeHwnd(), START_PROGRAM, 0, 0);
			//����� � ������
			pMyHndl->fContinueWaitStart = FALSE;
			}



			
		}
		//���� ������ �� �������� ������� � ������
		else
		{

		//������� ������� ������������
		//WaitTimer � �������� �� 10 ��
		WaitTimer = GetTickCount()/10 - pMyHndl->m_WaitTime;
		for (int i = 0; i < pMyHndl->m_PultArray.GetCount(); i++)
			//�������� ����������� 
			if (pMyHndl->m_DevRS485->CheckStart(pMyHndl->m_PultArray[i].m_SerNumPult))
			{
				//������������� � ����� ���������� � ����������
				pMyHndl->OnBnClickedCompReg();

				//�������� ������� � ������
				SendMessage(pMyHndl->GetSafeHwnd(), START_PROGRAM, 0, 0);
				//����� � ������
				pMyHndl->fContinueWaitStart = FALSE;
				break;
			} 
		}//if else m_WaitFromClock
	}//while

	pMyHndl->m_WaitStart = FALSE;
	return 0;
}

UINT WorkThread (LPVOID pArg)
{

	DWORD dwWaitReturn;
	double delta;

	//�������� ��������� �� ��������� ����� �������
	CMFCDlg *pMyHndl = (CMFCDlg *)pArg;


	if(pMyHndl->m_WorkRunning) return 0;
	//T�� LinePrg
	LinePrg* pLine;
			
	//���� - ����� �������
	pMyHndl->m_WorkRunning = TRUE;

	//�������� ������� ������ ���������
	dwWaitReturn = WaitForSingleObject(pMyHndl->m_hEventWork, INFINITE);

	//������ ���� ���� pMyHndl->fContinue = TRUE
	while(pMyHndl->fContinue)
	{
		//���� ������ ��������� �������
		if (pMyHndl->m_TimerRun)
		{
			//��������� ������� ������� ���������

			//��������� ������� �� ������ 
			if (pMyHndl->m_CompControl)
			{
			//==========================================================
			//���� ��������� ��������� � ������
			//==========================================================
			//���� ���� �������� � ������ ���������
			while(pMyHndl->m_posCurrentPosPrg != NULL)
			{
				//�������� ������� ���������� ���������
				if (!pMyHndl->m_TimerRun) break;
				
				//������� ������� ������ ���������
				pLine = &pMyHndl->m_ListLine.GetAt(pMyHndl->m_posCurrentPosPrg);
				//������� ������� ������������
				pMyHndl->m_TimerWork.Measure();

				// �������� ����� � ������� ������ ���� � ��������
				delta = (pMyHndl->m_TimerWork.GetTickCountAsSeconds() - 
					pMyHndl->m_StartTime);
				//MainTimer � �������� �� 10 ��
				MainTimer = static_cast<DWORD>(delta*100);


				if ((MainTimer + pMyHndl->m_StopTime) > (pLine->TimePusk + pMyHndl->m_Delay.GetPos()/10 ))
				{
					
					if (pMyHndl->m_PrgInComp)
					{
						//������ �����
						pMyHndl->SendSetLineToPult(pLine->NumPult,pLine->NumLine);
					}
					//������� � ��������� �����
					pMyHndl->m_intCurrentPosPrg++;
					POSITION& refPos =  pMyHndl->m_posCurrentPosPrg;
					pLine = &pMyHndl->m_ListLine.GetNext(refPos);
					//��������� ��������� ������� �����
					pMyHndl->m_ListPrg.SetCurSel(pMyHndl->m_intCurrentPosPrg);

					if ((pMyHndl->m_intCurrentPosPrg - pMyHndl->m_ListPrg.GetTopIndex())>5)
					{
						pMyHndl->m_ListPrg.SetTopIndex(pMyHndl->m_intCurrentPosPrg-5);
					} 
		
				} 
			}// while(pMyHndl->m_posCurrentPos != NULL)

			//����� �� ����� �� ��������� ���������
			if (pMyHndl->m_posCurrentPosPrg == NULL)
			{
				pMyHndl->m_TimerRun = FALSE;
				SendMessage(pMyHndl->GetSafeHwnd(), END_PROGRAM, 0, 0);
			}
			//==========================================================
		//��������� � ������ ��������� ��� ��� ���������
		// �� ������ ������ ����
		} //if (pMyHndl->m_CompControl)
		else
		{
			//������� ������� ������������
			pMyHndl->m_TimerWork.Measure();
			// �������� ����� � ������� ������ ���� � ��������
			delta = (pMyHndl->m_TimerWork.GetTickCountAsSeconds() - 
				pMyHndl->m_StartTime);
			//MainTimer � �������� �� 10 ��
			MainTimer = static_cast<DWORD>(delta*100);
		}

			//����� �� ����� � ���������
			if (!pMyHndl->m_TimerRun)
			{
				//��������� �������� ������� ��������� ���������
				pMyHndl->m_StopTime = MainTimer + pMyHndl->m_StopTime;
				MainTimer = 0;
				SendMessage(pMyHndl->GetSafeHwnd(), PAUSE_PROGRAM, 0, 0);
			}
			
		}//if (pMyHndl->m_TimerRun)


	}//while(pMyHndl->fContinue)
	//����� ����� ������ ������
	pMyHndl->m_WorkRunning = FALSE;
	return 0;
}
// ���������� ���� CMFCDlg
CMFCDlg::CMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCDlg::IDD, pParent)
	, m_PultList(COLUMNS_m_ColListCtrl)
	, m_PrgFileName(_T(""))
	, m_WorkReg(FALSE)
	, playing(false)
	, m_CompControl(true)
	, m_SoundLoaded(false)
	, m_PrgLoaded(false)
	, m_AccPercent(_T(""))
	, m_AccPower(_T(""))
	, m_HandBtnValue(FALSE)
	, m_Protocol(_T(""))
	, m_WorkRunning(FALSE)
	, fContinue(FALSE)
	, fContinueWaitStart(FALSE)
	, m_WaitStart(FALSE)
	, m_FindStart(FALSE)
	, m_posCurrentPosPrg(NULL)
	, m_intCurrentPosPrg(0)
	, m_TimerRun(FALSE)
	, m_CurrentPult(-1)
	, m_PultInfo(_T("")
	)
	, m_StartTime(0)
	, m_StopTime(0)
	, m_WaitTime(0)
	, m_PrefBtnValue(FALSE)
	, m_DelayTxt(_T(""))
	, m_TimePrg(0)
	, m_TimeSound(0)
	, m_MsgDlg(NULL)
	, m_PrgInComp(FALSE)
	, m_log(NULL)
	, m_BlinkWork(false)
	, m_WaitFromClock(false)
	, m_CurrentPrg(0)
{

	m_Key = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
//�������� ������� ������������� �� �������
	//MAKEINTRESOURCE - ������ �������������� ��������� �������������� �������
	//� ����������� ��������� - IDR_ACCEL_TABLE
	hAccel = LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACCEL_TABLE));


}

void CMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PRG, m_ListPrg);
	DDX_Text(pDX, IDC_PRG_FILE_NAME, m_PrgFileName);
	DDX_Text(pDX, IDC_SOUND_FILE_NAME, m_SoundFileName);
	//	DDX_Control(pDX, IDC_RS485, m_btnRS485);
	//	DDX_Control(pDX, IDC_LOAD_PRG, m_btnPrg);
	//	DDX_Control(pDX, IDC_LOAD_MP3, m_btnSound);
	// DDX_Control(pDX, IDC_TEST_REG, m_btnTest);
	// DDX_Control(pDX, IDC_WORK_REG, m_btnWork);
	// DDX_Control(pDX, IDC_COMP_REG, m_btnComp);
	// DDX_Control(pDX, IDC_PULT_REG, m_btnPult);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_PROGRESS_POWER, m_ProgressPower);
	DDX_Text(pDX, IDC_ACC_PERCENT, m_AccPercent);
	DDX_Text(pDX, IDC_ACC_POWER, m_AccPower);
	DDX_Control(pDX, IDC_HAND_BTN, m_btnHand);
	DDX_Check(pDX, IDC_HAND_BTN, m_HandBtnValue);
	// DDX_Control(pDX, IDC_FIND, m_btnFind);
	DDX_Text(pDX, IDC_PROTOCOL, m_Protocol);
	DDX_Control(pDX, IDC_LIST_PULT, m_PultList);
	DDX_Control(pDX, IDC_BEGIN, m_btnBegin);
	DDX_Control(pDX, IDC_WRITE, m_btnWrite);
	DDX_Control(pDX, IDC_CHECK, m_btnCheck);
	DDX_Text(pDX, IDC_INFO_PULT, m_PultInfo);
	DDX_Control(pDX, IDC_CHECK, m_btnCheck);
	DDX_Control(pDX, IDC_PROGRESS_REPORT, m_ProgressRpt);
	DDX_Control(pDX, IDC_MAINTIME, m_MainTimer);
	DDX_Check(pDX, IDC_PREFERENCE, m_PrefBtnValue);
	DDX_Control(pDX, IDC_DELAY, m_Delay);
	DDX_Text(pDX, IDC_DELAY_TXT, m_DelayTxt);
	DDX_Control(pDX, IDC_PREFERENCE, m_PrefBtn);
	DDX_Control(pDX, IDC_END_PRG, m_EndPrg);
	DDX_Control(pDX, IDC_END_SOUND, m_EndSound);
	// DDX_Control(pDX, IDC_PRG_IN_PULT, m_btnPrgInPult);
	// DDX_Control(pDX, IDC_PRG_IN_COMP, m_btnPrgInComp);
	// DDX_Control(pDX, IDC_WORK, m_btnWorkF1);
	// DDX_Control(pDX, IDC_STEP_REG, m_btnStepReg);
	//DDX_Control(pDX, IDC_PRG1, m_Prg1);
	//DDX_Control(pDX, IDC_PRG2, m_Prg2);
	//DDX_Control(pDX, IDC_PRG3, m_Prg3);
	//DDX_Control(pDX, IDC_PRG4, m_Prg4);
	//DDX_Control(pDX, IDC_PRG5, m_Prg5);
	//DDX_Control(pDX, IDC_PRG6, m_Prg6);
	//DDX_Control(pDX, IDC_PRG7, m_Prg7);
	//DDX_Control(pDX, IDC_PRG8, m_Prg8);
	DDX_Control(pDX, IDC_WORK_BMP, m_imgWork);
}

BEGIN_MESSAGE_MAP(CMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD_PRG, &CMFCDlg::OnBnClickedLoadPrg)
	ON_BN_CLICKED(IDC_LOAD_MP3, &CMFCDlg::OnBnClickedLoadMp3)
	ON_WM_DESTROY()
	ON_WM_DEVICECHANGE()
	ON_BN_CLICKED(IDC_TEST_REG, &CMFCDlg::OnBnClickedTestReg)
	ON_BN_CLICKED(IDC_WORK_REG, &CMFCDlg::OnBnClickedWorkReg)
	ON_BN_CLICKED(IDC_COMP_REG, &CMFCDlg::OnBnClickedCompReg)
	ON_BN_CLICKED(IDC_PULT_REG, &CMFCDlg::OnBnClickedPultReg)
	ON_BN_CLICKED(IDC_START, &CMFCDlg::OnBnClickedStart)
	//��� ��������� �� ������ � ���������� ������
	ON_MESSAGE(START_PROGRAM,  &CMFCDlg::OnStartFromPult)
	ON_MESSAGE(WM_POWERBROADCAST, OnPowerChange )
	ON_BN_CLICKED(IDC_HAND_BTN, &CMFCDlg::OnBnClickedHandBtn)
	ON_BN_CLICKED(IDC_STOP, &CMFCDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_FIND, &CMFCDlg::OnBnClickedFind)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PULT, &CMFCDlg::OnLvnItemchangedListPult)
	ON_BN_CLICKED(IDC_WRITE, &CMFCDlg::OnBnClickedWrite)
	ON_BN_CLICKED(IDC_BEGIN, &CMFCDlg::OnBnClickedBegin)
	ON_MESSAGE(END_PROGRAM,  &CMFCDlg::OnEndProgram)
	ON_MESSAGE(PAUSE_PROGRAM,  &CMFCDlg::OnPauseProgram)
	ON_BN_CLICKED(IDC_CHECK, &CMFCDlg::OnBnClickedCheck)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PREFERENCE, &CMFCDlg::OnBnClickedPreference)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CMFCDlg::OnBnClickedButtonDebug)
	ON_BN_CLICKED(IDC_PRG_IN_PULT, &CMFCDlg::OnBnClickedPrgInPult)
	ON_BN_CLICKED(IDC_PRG_IN_COMP, &CMFCDlg::OnBnClickedPrgInComp)
	ON_BN_CLICKED(IDC_WORK, &CMFCDlg::OnBnClickedWork)
	ON_BN_CLICKED(IDC_STEP_REG, &CMFCDlg::OnBnClickedStepReg)
	ON_COMMAND_RANGE(IDC_PRG1, IDC_PRG8,&CMFCDlg::OnBnClickedPrg)
END_MESSAGE_MAP()

// ����������� ��������� CMFCDlg
BOOL CMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ������� ��� ����
	m_log = new CReport();

	m_log->Add(_T("�������� ���������."));

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������


	//��������� ����� �������
	CStringArray Head;
	CByteArray   Cols;
	Head.Add(_T("�"));
	Cols.Add(10);			//10%
	Head.Add(_T("12�"));
	Cols.Add(20);			//30%
	Head.Add(_T("24�"));
	Cols.Add(20);			//50%
	Head.Add(_T("������"));
	Cols.Add(20);			//70%
	Head.Add(_T("���, ��"));
	Cols.Add(30);			//100%


	m_PultList.InitCtrl(&Head, &Cols);
    m_PultList.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT); 
	m_HandBtnValue=TRUE;
	
	m_Player.LoadDLL(L"LibMP3DLL.dll");		

	//m_imgWork.SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_WORK)));


	// �� ������ ��������� - ���������� �����
	m_FontPrg.CreatePointFont(100, _T("Courier New"));
	m_ListPrg.SetFont(&m_FontPrg);
	
	//��������� ������ ������ ��������
	m_Prg1.SubclassDlgItem(IDC_PRG1,this);
	m_Prg2.SubclassDlgItem(IDC_PRG2,this);
	m_Prg3.SubclassDlgItem(IDC_PRG3,this);
	m_Prg4.SubclassDlgItem(IDC_PRG4,this);
	m_Prg5.SubclassDlgItem(IDC_PRG5,this);
	m_Prg6.SubclassDlgItem(IDC_PRG6,this);
	m_Prg7.SubclassDlgItem(IDC_PRG7,this);
	m_Prg8.SubclassDlgItem(IDC_PRG8,this);

		//��� �������� �������� 1 ���������
	m_Prg1.SetColour(COLOR_BLACK,COLOR_GREEN);

	//����������� ������ ������ �������
	m_btnFind.SubclassDlgItem(IDC_FIND,this);
	m_btnFind.ResetColour(true);

	//��� �������� ��������� ��� �� ��������� (������� ������)
	m_btnPrg.SubclassDlgItem(IDC_LOAD_PRG,this);
	//m_btnPrg.SetColour(COLOR_GREEN,COLOR_RED);
	m_btnPrg.ResetColour(true);
	//��� �������� ���������� ��� �� ��������� (������� ������)

	m_btnSound.SubclassDlgItem(IDC_LOAD_MP3,this);
	//m_btnSound.SetColour(COLOR_GREEN,COLOR_RED);
	m_btnSound.ResetColour(true);

	m_btnRS485.SubclassDlgItem(IDC_RS485,this);

	//������� ��������������� RS485
	m_DevRS485 = new CRS485;
	m_DevRS485->LoadDLL();

	//���������� ������������� RS485
	if (m_DevRS485->m_CountRS485 > 0)
	{
		//������������ ���� ���� ���������� RS485
		m_btnRS485.SetColour(COLOR_BLACK,COLOR_GREEN);
		//�������������� RS485
		if (!m_DevRS485->OpenPort())
			AfxMessageBox(TEXT("������ �������� �����!"));
	}
	else 
	{
		//������������ 0 RS485
		m_btnRS485.SetColour(COLOR_GREEN,COLOR_RED);
	}

	//��� �������� ������� ����� �����
	m_btnTest.SubclassDlgItem(IDC_TEST_REG,this);
	//m_btnTest.SetColour(COLOR_BLACK,COLOR_GREEN);
	m_btnTest.ResetColour(true);

	//������ ������ - ��������� �����
	m_btnWorkF1.SubclassDlgItem(IDC_WORK,this);
	m_btnWorkF1.ResetColour(true);


	//������ �������� ������ - ��������� �����
	m_btnWork.SubclassDlgItem(IDC_WORK_REG,this);
	m_btnWork.ResetColour(true);

	//��� �������� - ���������� � ����������
	m_btnComp.SubclassDlgItem(IDC_COMP_REG,this);
	m_btnComp.SetColour(COLOR_BLACK,COLOR_GREEN);

	//��� �������� ������ �������� ������ - ��������� ����� 
	m_btnStepReg.SubclassDlgItem(IDC_STEP_REG,this);
	m_btnStepReg.ResetColour(true);



	//������ - ���������� � ������ ��������� �����
	m_btnPult.SubclassDlgItem(IDC_PULT_REG,this);
	m_btnPult.ResetColour(true);

	//��� �������� - ��������� ��������� � ������
	m_btnPrgInPult.SubclassDlgItem(IDC_PRG_IN_PULT,this);
	m_btnPrgInPult.SetColour(COLOR_BLACK,COLOR_GREEN);

	//������ - ��������� � ���������� -  ��������� �����
	m_btnPrgInComp.SubclassDlgItem(IDC_PRG_IN_COMP,this);
	m_btnPrgInComp.ResetColour(true);

	//���������� ������ ������������ � ���������
	m_ProgressPower.SetRange(0,100);
	RefreshPower();

	//��������� �������� ���������� ���������
	m_Delay.SetRange(0,MAX_DELAY_PRG,TRUE);
	m_Delay.SetPos(0);
	m_DelayTxt = _T("�������� 0 �� (0 �)");

	//����������� ������� �����������-���������� RS485
	DEV_BROADCAST_DEVICEINTERFACE dbch;
	dbch.dbcc_size = sizeof(dbch);
	dbch.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for (int i = 0; i < sizeof(GuidInterfaceList); i++)
	{
		//dbch.dbcc_classguid = FTDI_D2XX_GUID;//FTDI_VCP_GUID;
		dbch.dbcc_classguid = GuidInterfaceList[i];
		dbch.dbcc_name[0] = '\0';
		NotificationHandle = RegisterDeviceNotification( GetSafeHwnd(), &dbch, DEVICE_NOTIFY_WINDOW_HANDLE);
	}

	m_MsgDlg = new CMsgDlg(this);
	m_MsgDlg->Create(MAKEINTRESOURCE(IDD_MESSAGE_DLG),this);
	m_MsgDlg->SetColorDlg(RGB(255,0,0));
	m_MsgDlg->SetTextDlg(_T("��������� ���������"));
	m_MsgDlg->ShowWindow(SW_HIDE);

	//�������� ������� ����������� ������
	m_DelayStart = new CDelayStart(this);
	m_DelayStart->Create(MAKEINTRESOURCE(IDD_DELAY_START),this);
	m_DelayStart->ShowWindow(SW_HIDE);


	m_Key = new CKey(this);
	//������� ������� ���������
	m_Key->Create(MAKEINTRESOURCE(IDD_KEY),this);
	CRect rct;
	m_Key->GetClientRect(&rct);
	m_Key->MoveWindow(290,400,rct.Width(),rct.Height(),FALSE);
	m_Key->ShowWindow(SW_SHOW);

	//������ ��������� ����� 
	m_LineDlg = new CLineDlg(this);
	m_LineDlg->Create(MAKEINTRESOURCE(IDD_INFO_LINE),this);
	m_LineDlg->ShowWindow(SW_HIDE);



	//������� ������ �� 8 ������ ��������
	for (int i = 0; i < 8; i++)
	{
		CPrg* pPrg = new CPrg;
		m_PrgArray.SetAtGrow(i,*pPrg);
		delete pPrg;
	}





	//������ �������� ������ ���������
	//���� ������� ������
	fContinue = TRUE;
	
	//������� ������ ������ ���������
	m_hEventWork = CreateEvent(NULL, FALSE, FALSE, NULL);

	//�������� ������ ���������� ���������
	pWorkThread	= AfxBeginThread(WorkThread, this, THREAD_PRIORITY_NORMAL);

	//�������� ������� �������� ������ �������
	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);

	UpdateData(FALSE);
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.
void CMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCDlg::OnBnClickedLoadPrg()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_TimerRun) return;
	if (m_FindStart) return;
	if (!m_btnPrg.IsWindowEnabled()) return;
	// �������� ����������� ������ ������ ����� Open
	CFileDialog DlgOpen(TRUE,_T("txt"),NULL,
		OFN_HIDEREADONLY,_T(" Text Files (*.txt) |*.txt||"));
	//���� �������� �����
try
{
	// ����������� ����������� ������ ������ ����� Open
	if(DlgOpen.DoModal()==IDOK) 
	{	// �������� ������� � �������� ����� ��� ������
		CStdioFile File(DlgOpen.GetPathName(),CFile::
			modeRead|CFile::typeText);
		CString m_fileTitle = DlgOpen.GetFileTitle();

		m_PrgArray[m_CurrentPrg].m_FileTitle = m_fileTitle;

		CString tempText = "";
		// ������ �� ����� ������
		CString& ref = tempText;
		CString tempTime;
		CString tempNumPult;
		CString tempNumLine;
		CString tempDescr;
		
		m_ListLine.RemoveAll();

		int i = 1;
		m_ListPrg.ResetContent();
		while(File.ReadString(ref))// ���������� ������ �� ������ tempText
		{
			int nTokenPos = 0;
			tempTime = tempText.Tokenize(_T("\t"), nTokenPos);
			tempNumPult = tempText.Tokenize(_T("\t"), nTokenPos);
			tempNumLine = tempText.Tokenize(_T("\t"), nTokenPos);
			tempDescr = tempText.Tokenize(_T("\t"), nTokenPos);
			
			LinePrg* pLine = new LinePrg;
			pLine->NumPP = i;
			pLine->TimePusk = StringToTime(tempTime);
			pLine->NumPult = StrToInt(tempNumPult);
			pLine->NumLine = StrToNumLine(tempNumLine);
			pLine->Description = tempDescr;
			m_ListLine.AddTail(*pLine);

			delete pLine;			
			
		
			//StrToInt - �������� 0 ���� �� �����
			ref.Format(_T("%5d   %s   %5d   %5d   %s"),i,tempTime,StrToInt(tempNumPult),StrToNumLine(tempNumLine),tempDescr); 
			i++;
			m_ListPrg.AddString(ref);
		}
			//���� ��������� ���������, �� ������� ������� �������
			//� ������ � ����������� ��� ���������
			m_PrgLoaded = true;
			
			//������������ ������������ ��������� �� ������

			CColorButton* btn;
			btn = (CColorButton*)GetDlgItem(m_CurrentPrg+IDC_PRG1);
			btn->SetWindowText(tempTime);

			//���������� ������ ���� ����� � ����������
			m_PrgArray[m_CurrentPrg].m_FilePrg = DlgOpen.GetPathName();

			m_PrgFileName = _T("��������� - ") + DlgOpen.GetFileTitle();
			m_btnPrg.SetColour(COLOR_BLACK,COLOR_GREEN);


			// ����������������� ��������� � TempTime
			m_TimePrg = StringToTime(tempTime);

			//��������� � ������ ���������
			m_PrgArray[m_CurrentPrg].m_TimePrg = m_TimePrg;

			m_EndPrg.SetText(tempTime);
			m_intCurrentPosPrg = 0;
			MainTimer = 0;
			m_StopTime = 0;
			//�������� ������ ������ ������ ���������
			m_ListPrg.SetCurSel(m_intCurrentPosPrg);
			//������ ����� � ��������� - � ������
			m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
			UpdateData(FALSE);
			//��������� ������
		
			if (m_CurrentPult >= 0)
			{
				m_btnWrite.EnableWindow(TRUE);
				m_btnCheck.EnableWindow(TRUE);
			}
			//������� ���� � ������ ������ � ��������
			m_btnWrite.ResetColour(true);
			m_btnCheck.ResetColour(true);

	} 

} catch (...) 
	//��� ����� ������ ��������� �� �������,
	//���������� ������ ���������, �������� ������ ��������� 
	//������� ������, ������������� ����, ��� ��������� �� ���������

	{
		//������� ���������
		m_ListLine.RemoveAll();
		m_ListPrg.ResetContent();
		m_PrgLoaded = false;
		m_PrgFileName = _T("");
		//m_btnPrg.SetColour(COLOR_GREEN,COLOR_RED);
		m_btnPrg.ResetColour(true);
		UpdateData(FALSE);

		m_btnBegin.EnableWindow(FALSE);
		m_btnWrite.EnableWindow(FALSE);
		m_btnCheck.EnableWindow(FALSE);
	}
}
//
void CMFCDlg::OnBnClickedLoadMp3()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_FindStart) return;
	if (m_TimerRun) return;
	if (!m_btnSound.IsWindowEnabled()) return;

	CFileDialog fOpenDlg(
		TRUE, _T("mp3"), _T("*.mp3"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 
		_T("MP3 files (*.mp3)|*.mp3|All files (*.*)|*.*||"), this);

	fOpenDlg.m_pOFN->lpstrTitle=_T("MP3 file");

	
	if(fOpenDlg.DoModal()==IDOK)
	{
				
		USES_CONVERSION; //��� ����� ��������� �������� �� �����!
		CString szMkvPath = fOpenDlg.GetPathName();
	
		
		LPCWSTR unitest = T2CW(LPCTSTR(szMkvPath));

		m_SoundLoaded = m_Player.Load(unitest);

//���� ���� ������� ��������, �� ������� ������� ���� � ������
//���������� � ���������� ��� �����
		if (m_SoundLoaded)
		{
			//���������� ������ ���� � ����� ������
			m_PrgArray[m_CurrentPrg].m_FileSound = szMkvPath;

			m_btnSound.SetColour(COLOR_BLACK,COLOR_GREEN);
			m_PrgArray[m_CurrentPrg].m_FileSoundTitle = fOpenDlg.GetFileTitle();
			m_SoundFileName = _T("��������� - ") + fOpenDlg.GetFileTitle();
			//�������� ������������ ���������� � 10 ��
			//__int64 1 sec = 10 000 000
			DWORD tmp;
			tmp = static_cast<DWORD>(m_Player.GetDuration()/100000);
			m_TimeSound = tmp;
			//���������� ������������ ����������
			m_PrgArray[m_CurrentPrg].m_TimeSound = m_TimeSound;
			m_EndSound.SetText(TimeToString(tmp));

			UpdateData(FALSE);
		}
	}
	else
	{
		//m_btnSound.SetColour(COLOR_GREEN,COLOR_RED);
		m_btnSound.ResetColour(true);
		m_SoundFileName = "";
		m_Player.Cleanup();
		m_EndSound.SetText(TimeToString(0));
		UpdateData(FALSE);
	}
}
//���������� �������, ��������� �� �����
void CMFCDlg::OnDestroy()
{
	CDialog::OnDestroy();
	delete m_Key; 
	delete m_MsgDlg;
	delete m_DelayStart;
	delete m_LineDlg;
	if(playing)	m_Player.Stop();
	m_Player.Cleanup();
	m_Player.UnloadDLL();
	
	//��������� ������ ��������� �������
	fContinue = FALSE;
	m_TimerRun = FALSE;
	SetEvent(m_hEventWork);
	if(pWorkThread != NULL) 
	{
		// �������� �������� ������
		if((m_WorkRunning != FALSE) && (WaitForSingleObject(pWorkThread->m_hThread, THREAD_WAIT_TIMEOUT) == WAIT_TIMEOUT)) 
		{
			// ���� �� ��������, ������� �����
			TerminateThread(pWorkThread->m_hThread, 0xffffffff);
		}
	}

	//��������� ������� ������ � ������ � ����
	m_DevRS485->ClosePort();

	//�������� ����� ��������� �� ������
	m_ListLine.RemoveAll();

	//������������� ������ ������ �������
	KillTimer(m_TimerEcho);

	//������� ������ �������
	m_PultArray.RemoveAll();
	if (m_DevRS485 != NULL)	delete m_DevRS485;

	delete m_log;

}
//������ ������ �����, ������� ������� � ����� ����
void CMFCDlg::OnBnClickedTestReg()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_FindStart) return;
	m_MsgDlg->ShowWindow(SW_HIDE);
	if(m_CurrentPult == -1) return;
	// ���� �������� ���������, �� ������������ ������� ���������
	if (m_TimerRun) return;
	//��� �������, ������ ����������
	if (m_PultArray.GetSize() == 0) return;

	if (!m_btnTest.IsWindowEnabled()) return;


	//����� ������� ��������
	m_BlinkWork = false;
	//������� ������ ������ �����
	m_btnWorkF1.ResetColour(true);

	KillTimer(m_TimerEcho);
	//������������� �������� ����� �� ���� �������
	for (int i = 0; i < m_PultArray.GetSize(); i++)
	{
		// ���� ����� ������� ����������, �� ���������� ��� ����� 
		if	(m_DevRS485->SetWorkReg(m_PultArray[i].m_SerNumPult,false))
		{
			for (int z = 0; z < COLUMNS_m_ColListCtrl; z++)
				//��� ����������� ������ � �������� i 
				m_PultList.SetItemBackgndColor((ITEM_COLOR)WHITE, i, z);
			m_PultArray[i].m_WorkMode = REG_TEST;
		}
	}
	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);

	//������������� ������ ��������� , ����������, �����, ���������
	m_btnPrg.EnableWindow(TRUE);
	m_btnSound.EnableWindow(TRUE);
	m_PrefBtn.EnableWindow(TRUE);
	m_btnFind.EnableWindow(TRUE);
	m_btnCheck.EnableWindow(TRUE);
	m_btnWrite.EnableWindow(TRUE);
}
//������ ������ �������� ������, ������� ������� � ������� �����
void CMFCDlg::OnBnClickedWorkReg()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
//	m_MsgDlg->ShowWindow(SW_HIDE);
	if (m_FindStart) return;
	if(m_CurrentPult == -1) return;
	// ���� �������� ���������, �� ������������ ������� ���������
	if (m_TimerRun) return;
	if (!m_btnWork.IsWindowEnabled()) return;

	KillTimer(m_TimerEcho);
	
	//��������� ������ � ������� �����
	for (int i = 0; i < m_PultArray.GetSize(); i++)
	{
	
	// ���� ����� ������� ����������, �� ���������� ����� � ������ �������
		if	(m_DevRS485->SetWorkReg(m_PultArray[i].m_SerNumPult,true))
		{
			for (int z = 0; z < COLUMNS_m_ColListCtrl; z++)
				//��� ����������� ������ � �������� i
				m_PultList.SetItemBackgndColor((ITEM_COLOR)GREEN, i, z);
			m_PultArray[i].m_WorkMode = REG_WORK;
		
		}
	}

	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
}
//
void CMFCDlg::OnBnClickedCompReg()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_FindStart) return;

	KillTimer(m_TimerWaitStart);
	if (m_TimerEcho == NULL) m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	if (m_TimerRun) return;
	m_MsgDlg->ShowWindow(SW_HIDE);
	//if(m_CurrentPult == -1) return;

	//������������ ����������, ���� ������� �������������
	if (!m_btnComp.IsWindowEnabled()) return;
	
	fContinueWaitStart = FALSE;


	if (!m_CompControl)
	{
		m_btnComp.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnStepReg.ResetColour(true);
		m_CompControl = true;
		m_PrgArray[m_CurrentPrg].m_CompControl = true;
	}
	EnableBtn(TRUE);
	
}
//��������� �������� ������� ������ ��� �� ������� ���
//�� ������ ����� � ����� ������
void CMFCDlg::OnBnClickedPultReg()
{
	if (m_FindStart) return;
	if (!m_WorkReg)
	{
		AfxMessageBox(_T("��������� �� � ������� ������"));
	}
	else
	{
		m_DelayStart->CenterWindow();
		m_DelayStart->ShowWindow(SW_SHOW);
	}

}
//����������� ��� ����� ������������ 
BOOL CMFCDlg::OnDeviceChange(UINT EventType, DWORD dwData)
{
	char Word [50];
	char Word1 [57]; 
	char Word2 [50]; 
	char Word3 [50];
	char ch = ' ';
	CString Msg = _T("duh");
	switch( EventType)
	{
	case DBT_CONFIGCHANGECANCELED:
		Msg.Format(_T("DBT_CONFIGCHANGECANCELED"));
		break;
	case DBT_CONFIGCHANGED:
		Msg.Format(_T("DBT_CONFIGCHANGED"));
		break;
	case DBT_CUSTOMEVENT:
		Msg.Format(_T("DBT_CUSTOMEVENT"));
		break;
	case DBT_DEVICEARRIVAL:
		Msg.Format(_T("DBT_DEVICEARRIVAL"));
		break;
	case DBT_DEVICEQUERYREMOVE:
		Msg.Format(_T("DBT_DEVICEQUERYREMOVE"));
		break;
	case DBT_DEVICEQUERYREMOVEFAILED:
		Msg.Format(_T("DBT_DEVICEQUERYREMOVEFAILED"));
		break;
	case DBT_DEVICEREMOVEPENDING:
		Msg.Format(_T("DBT_DEVICEREMOVEPENDING"));
		break;
	case DBT_DEVICEREMOVECOMPLETE:
		Msg.Format(_T("DBT_DEVICEREMOVECOMPLETE"));
		break;
	case DBT_DEVICETYPESPECIFIC:
		Msg.Format(_T("DBT_DEVICETYPESPECIFIC"));
		break;
	case DBT_QUERYCHANGECONFIG:
		Msg.Format(_T("DBT_QUERYCHANGECONFIG"));
		break;
	case DBT_DEVNODES_CHANGED:
		Msg.Format(_T("DBT_DEVNODES_CHANGED"));
		break;
	case DBT_USERDEFINED:
		Msg.Format(_T("DBT_USERDEFINED"));
		break;
	default:
		Msg.Format(_T("Event type %d"),EventType);
	}

	PDEV_BROADCAST_DEVICEINTERFACE pdbch = (PDEV_BROADCAST_DEVICEINTERFACE)dwData;
	if( pdbch!=NULL && pdbch->dbcc_devicetype==DBT_DEVTYP_DEVICEINTERFACE)
	{
		CString Msg2;
		Msg2.Format(_T("%s: %s"),Msg,pdbch->dbcc_name);
		Msg = Msg2;

	}
	if (Msg == _T("DBT_DEVNODES_CHANGED"))
	{
		//CListBox* EventList = (CListBox*)GetDlgItem(IDC_EVENT_LIST);
		//EventList->AddString(Msg);
	}
	else
	{
		strncpy_s(Word,Msg,17); 
		Word[17] = '\0'; //DBT_DEVICEARRIVAL - Word[1 - 17]

		if ( strcmp ( Word, _T("DBT_DEVICEARRIVAL"))== 0 )
		{
			//������������ ����������
			// ���������� ��� ����������
			// 
			strncpy_s(Word1,Msg,44); // DBT_DEVICEARRIVAL:\\?\USB#Vid_0403&Pid_6001 Word1[0 .. 43 ]
			Word1[44] = '\0';
			strncpy_s(Word2, Word1, 17); //DBT_DEVICEARRIVAL - Word2[0 .. 16] 
			Word2[17] = '\0';
			strncpy_s(Word3, &Word1[27], 17); //Vid_0403&Pid_6001 Word3[0 .. 16] =  Word1[27 .. 43 ]
			Word3[17] = '\0';
			if (strcmp (Word3, _T("Vid_0403&Pid_6001")) == 0)
			{
			// ����������� RS485
				m_DevRS485->UnloadDLL();
				delete m_DevRS485;
				m_DevRS485 = new CRS485;
				m_DevRS485->LoadDLL();
			
				//���������� ������������� RS485
				if (m_DevRS485->m_CountRS485 > 0)
				{
					//������������ ���� ���� ���������� RS485
					m_btnRS485.SetColour(COLOR_BLACK,COLOR_GREEN);
					m_log->Add(_T("��������������� RS-485 ���������"));
					//�������������� RS485
					if (!m_DevRS485->OpenPort())
						AfxMessageBox(TEXT("������ �������� �����!"));
				}

				m_btnRS485.SetColour(::GetSysColor(COLOR_BTNTEXT),COLOR_GREEN);
			}


		}
		else
		{
			strncpy_s(Word,Msg,24); //DBT_DEVICEREMOVECOMPLETE - Word[0 .. 23]
			Word[24] = '\0';
			if ( strcmp ( Word, _T("DBT_DEVICEREMOVECOMPLETE"))== 0 )
			{
				// ����������� ����������
				// ���������� ��� ����������
				strncpy_s(Word1,Msg,55); // DBT_DEVICEREMOVECOMPLETE:\\?\USB#Vid_0403&Pid_6001 Word1[0 .. 54] 
				Word1[55] = '\0';
				strncpy_s(Word2, Word1, 24); //DBT_DEVICEREMOVECOMPLETE - Word2[0 .. 23]
				Word2[24] = '\0';
				strncpy_s(Word3, &Word1[34], 17); //Vid_0403&Pid_6001 Word3[0 .. 16] =  Word1[34 .. 54]
				Word3[17] = '\0';
				if (strcmp (Word3, _T("Vid_0403&Pid_6001")) == 0)
				{
					m_DevRS485->ClosePort();
					m_DevRS485->UnloadDLL();
					m_btnRS485.SetColour(::GetSysColor(COLOR_BTNTEXT),COLOR_RED);
					m_log->Add(_T("��� ����� � ���������������� RS-485"));

				}
			}
		}
	}


	return TRUE;
}
//
BOOL CMFCDlg::PreTranslateMessage(MSG* pMsg)
{
//���� ������ ������� ���������� � ����� ������ ���������
//��� ������� ������ � ���������� �������
if((pMsg->message == WM_KEYUP) && (m_HandBtnValue) && (m_WorkReg) && (m_TimerRun))
{
 CHandBtn* ci;

//���������� ��� ������
 for (int x=IDC_KEY1;x<(IDC_KEY40+1);x++)
 {
 ci=(CHandBtn*)m_Key->GetDlgItem(x);
 if(pMsg->wParam != ci->m_Key) continue;
 else 
 {
	 //��������� ������ �������
	 //��������� � 32 �� 40 ������
	 if (ci->m_IsMotor)
	 {
		 //������ ��� ��������
		 if (ci->m_Checked)
		 {
			 //�����������
			 ci->ResetColorBtn();
			 ci->m_Checked = FALSE;
			 //��������� �����
			 SendClearLineToPult(0, (x - IDC_KEY1+1));
			 return TRUE;
		 } 
		 else
		 {
			 ci->SetColorBtn(COLOR_MOTOR_TXT,COLOR_MOTOR_BGR);
			 ci->m_Checked = TRUE;
			 //�������� �����
			 SendSetLineToPult(0, (x - IDC_KEY1+1));
			 return TRUE;
		 }
	 }

	 //��������� ������ �����
	 //���� ����������� �������� ��� ������
	 if (ci->m_State)
	 {
		 //������ ��� ��������
		 if (ci->m_Checked)
		 {
			 //�����������
			 ci->ResetColorBtn();
			 ci->m_Checked = FALSE;
		 } 
		 else
		 {
			 ci->SetColorBtn(COLOR_CHECK_TXT,COLOR_CHECK_BGR);
			 ci->m_Checked = TRUE;
		 }

	 } 
	 //����� ���������� �������� � ��������� �����
	 else
	 {

		 ci->m_ClickCount++;
		 CString m_string;
		 m_string.Format(_T("%d"),ci->m_ClickCount);
		 ci->m_ResultsWnd.SetWindowText(m_string);
		 // ���������� �� ��������� ������� ������� �� �����
		 // 0 - ����� ��� �� ���������
		 SendSetLineToPult(0, (x - IDC_KEY1+1));
	 }
 }//else

}//for
}// if WM_KEYUP

// ����� ������� �� ������ ����������� ������������
if ( TranslateAccelerator( m_hWnd, hAccel, pMsg ) ) return TRUE;
return CDialog::PreTranslateMessage(pMsg);
}
//
void CMFCDlg::OnBnClickedStart()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_FindStart) return;
	if (m_TimerRun) return;
	//if(m_CurrentPult == -1) return;
	//���� ����� ����������, �� �����
	if (!m_btnStart.IsWindowEnabled()) return;
	//if (!m_PrgLoaded) return;

	//���� ���������� ����� ��������� � ������ � ���������� ������� �����
	//������� ������� ����� �������
	if (m_WorkReg)
	{
		//���� ��������� � ������, �� ������� ������� ����� �������
		//�������������� ��������� ��������� WORK_MODE ()
		if (!m_PrgInComp)
		{

			m_DevRS485->SendStart();
			m_DevRS485->SendStart();
			m_DevRS485->SendStart();
		}
		//��������� ����� �������
		m_BlinkWork=false;
		//������ ������ ����� �������
		m_btnWorkF1.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_imgWork.ShowWindow(SW_SHOW);
	}


	//��������� ��� ������ ����� ������ ����
	EnableBtn(FALSE);
	m_btnStop.EnableWindow(TRUE);

	//���� ��������� ����������, �����
	if (m_SoundLoaded)
	{

		//�������� ����������
		m_Player.Play();
		// -10000 ������ ������ ���������
		// 0 ������������ ���������
		// ������������� ��������� - ������
		m_Player.SetVolume(0);
	}

		//����� ������� �� ������ ��������� 
		m_TimerWork.Measure();
		m_StartTime = m_TimerWork.GetTickCountAsSeconds();
	
	m_Timer = SetTimer(TIMER_MAIN,100,NULL);

	//��������� ������ ������� �������
	m_TimerRun = TRUE;
	m_Protocol = _T("��������� ��������");
	UpdateData(FALSE);
	//����� ���������
	SetEvent(m_hEventWork);
}
// ������� ��������� �������
LRESULT CMFCDlg::OnPowerChange(WPARAM, LPARAM)
{
	RefreshPower();
	return 1;
}
//����� ��������� � ������
LRESULT CMFCDlg::OnStartFromPult(WPARAM, LPARAM)
{
	m_MsgDlg->ShowWindow(SW_HIDE);
	KillTimer(m_TimerWaitStart);
	OnBnClickedStart();
	

	return 1;
}
// ���������� ��������� �������
void CMFCDlg::RefreshPower(void)
{
	int tempAcc = PowerAcc();
	m_ProgressPower.SetPos(tempAcc);
	CString str;
	str.Format(_T("����� %02d%%"),tempAcc); 
	m_AccPercent = str;
	if (IsAccPower())
	{
		m_AccPower = "������� �� ����";
	}  
	else
	{
		m_AccPower = "������� �� ������������";
		if (tempAcc < 30) m_log->Add(_T("������� ���������� ���������"));
	}



	UpdateData(FALSE);

}
//�����, ������� ������ � �������� ���������
void CMFCDlg::OnBnClickedHandBtn()
{

	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();

	CRect rect;
	m_ListPrg.GetWindowRect(&rect);
	ScreenToClient(&rect);

	if (m_HandBtnValue == TRUE)
	{
		m_ListPrg.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height()+250,TRUE);
		m_Key->ShowWindow(SW_HIDE);
		m_HandBtnValue = FALSE;
		m_PrgArray[m_CurrentPrg].m_HandBtnValue = FALSE;
		UpdateData(FALSE);
	} 
	else
	{
		m_ListPrg.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height()-250,TRUE);
		m_Key->ShowWindow(SW_SHOW);
		m_HandBtnValue = TRUE;
		m_PrgArray[m_CurrentPrg].m_HandBtnValue = TRUE;
		UpdateData(FALSE);
	}
	//����������� ������ � ����� � ���������� m_HandBtnValue

	UpdateData(TRUE);
}
//������ ����
void CMFCDlg::OnBnClickedStop()
{
	int NotReady;
	CString tmp;

	if (m_FindStart) return;
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	//if(m_CurrentPult == -1) return;
	if (!m_btnStop.IsWindowEnabled()) return; 

	// ���� ������ ��������� �� ������� (������ ������� ������ ����)
	if (!m_TimerRun) 
		if (m_SoundLoaded)
		{
			m_Player.Pause();
	//		__int64 duration = m_Player.GetDuration();
	//		__int64 pos =  1;
	//		m_Player.SetPositions(&pos, &duration, true);
		}


	//������������� ������ ������� �������
	m_TimerRun = FALSE;


	//���� ��������� ����������� � ������ � ������� �����,
	//�� ������� ������� ���� � ������
	if(!m_PrgInComp && m_WorkReg)
	{
		//m_DevRS485->SendStop();
	NotReady=PauseAllPult();
		if(NotReady != 0)
			{
				tmp.Format(_T("�� �������� ���������� ������� - %d"),NotReady);
				MessageBox(tmp); 
			}
	}
	//��������� � ����� ������� ������
	m_BlinkWork = true;
	m_imgWork.ShowWindow(SW_SHOW);
	m_btnWorkF1.ResetColour(true);
	EnableBtn(FALSE);
	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(TRUE);
	m_btnBegin.EnableWindow(TRUE);
	m_Protocol = _T("��������� �����������");
	UpdateData(FALSE);
}
//������ ������ �������
void CMFCDlg::OnBnClickedFind()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_TimerRun) return;
	//���� ���� ���������, �� ����� �� ������������(��� �������������)
	if (!m_btnFind.IsWindowEnabled()) return;

	if (m_FindStart) 
	{
		m_FindStart = FALSE;
		m_btnFind.ResetColour(true);
		m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
		m_ProgressRpt.ShowWindow(SW_HIDE);

		m_btnPrg.EnableWindow(TRUE);
		m_btnSound.EnableWindow(TRUE);
		m_btnFind.EnableWindow(TRUE);

		if (m_CurrentPult >= 0)
		{
			m_btnCheck.EnableWindow(TRUE);
			m_btnWrite.EnableWindow(TRUE);
			m_PrefBtn.EnableWindow(TRUE);
		}
	}
	else
	{
		KillTimer(m_TimerEcho);
		EnableBtn(FALSE);
		m_btnFind.EnableWindow(TRUE);
		//������� ��� ������ �� ������
		m_PultList.DeleteAllItems();
		//������� ������� ���
		m_CurrentPult = -1;
		//������� ������ �������
		m_PultArray.RemoveAll();
		m_ProgressRpt.SetRange(1,MAX_PULT);
		m_ProgressRpt.ShowWindow(SW_SHOW);
		m_Protocol.Format(_T("����� �������"));
		UpdateData(FALSE);
		//�������� ������ ������ �������
		pFindThread	= AfxBeginThread(FindPult, this, THREAD_PRIORITY_NORMAL);
		m_btnFind.SetColour(COLOR_BLACK,COLOR_GREEN);
	}


}
// ������� ���������� ������ � ������
void CMFCDlg::AddPult(BYTE SerNumPult)
{
	CPult* pPult = new CPult;
	pPult->m_SerNumPult = SerNumPult;
	

	CStringArray line;
	// ��������� ���������� ������� 12 �����
	// � ���������� ������������������� ����� ������

	//����� ������ ������
	pPult->m_WorkMode = m_DevRS485->m_RxBuffer[2]; 
	pPult->m_Power24 = m_DevRS485->m_RxBuffer[3];
	pPult->m_Power12 = m_DevRS485->m_RxBuffer[4];
	pPult->m_Temper = ((m_DevRS485->m_RxBuffer[6]<<8) | m_DevRS485->m_RxBuffer[5]);
	pPult->m_NumPult = m_DevRS485->m_RxBuffer[7];
	//������ ������������ �������� ������������
	pPult->m_Pulse = ((m_DevRS485->m_RxBuffer[9]<<8) | m_DevRS485->m_RxBuffer[8]);


	//��������� � ������ �������
	m_PultArray.SetAtGrow(m_CurrentPult,*pPult);
	delete pPult;

	//������� � ������ �������
	CString temper,minus,ost,str;
	WORD tempTemper,pulse;
	double tempV24,tempV12;

	tempV12 = m_PultArray[m_CurrentPult].m_Power12*33/255.0; 
	tempV24 = m_PultArray[m_CurrentPult].m_Power24*33/255.0;
	
	if(m_PultArray[m_CurrentPult].m_Temper & 0x8000){
		tempTemper = ~(m_PultArray[m_CurrentPult].m_Temper)+1;
		minus = _T("-");
	}else{
		tempTemper = m_PultArray[m_CurrentPult].m_Temper;
		minus = _T("+");
	}
	if (tempTemper & 0x08) ost=_T(".5"); else ost=_T(".0");

	tempTemper = tempTemper>>4;
	temper.Format(_T("%s%d"),minus,tempTemper);


	pulse=m_PultArray[m_CurrentPult].m_Pulse;

	//��������� � ��������� ������
	//����� ������
	str.Format(_T("%d"),m_PultArray[m_CurrentPult].m_NumPult);
	line.Add(str);
	//������� 12 �����
	str.Format(_T("%.2f"),tempV12);
	line.Add(str);
	//������� 24 ������
	str.Format(_T("%.2f"),tempV24);
	line.Add(str);
	//�����������
	temper.Format(_T("%s%d"),minus,tempTemper);
	temper = temper + ost;
	line.Add(temper);
	//������������ �������� �������
	str.Format(_T("%d"),pulse);
	line.Add(str);


	m_PultList.AddItem(&line);

	if ((m_PultArray[m_CurrentPult].m_WorkMode & REG_WORK) != 0)
	{
		//������������ ����� � ������� ������
		//��������� ������ ����, ������� ���
		for (int i = 0; i < COLUMNS_m_ColListCtrl; i++)
			m_PultList.SetItemBackgndColor((ITEM_COLOR)GREEN, m_CurrentPult, i);
	}
	else
	{
		//������������ ����� � �������� ������ 
		//��������� ������ ����, ����� ���
		for (int i = 0; i < COLUMNS_m_ColListCtrl; i++)
			m_PultList.SetItemBackgndColor((ITEM_COLOR) WHITE, m_CurrentPult, i);
	}

	//UpdateData(FALSE);
}
// �������� ��������� ������� � �����
void CMFCDlg::SendGrpCmdToPult(DWORD dwData)
{
	//���� ������� �����, �� ���������� ������� �� ������ � �����
	if (m_WorkReg)
	{
		//�������� ������� �����, ���������� � ������ �������
		if (m_CurrentPult != -1)
		{
			BYTE NumPult = m_PultArray[m_CurrentPult].m_SerNumPult;
			m_DevRS485->SetGroup(NumPult, dwData);
		}
	}
}
//������� ��������� ������ � ������ �������
void CMFCDlg::OnLvnItemchangedListPult(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ((pNMLV->uNewState & LVIS_SELECTED) != 0)
	{
		CString tempStr;
		m_CurrentPult = pNMLV->iItem;
		tempStr.Format(_T("������ ����� �%d"),m_PultArray[m_CurrentPult].m_NumPult);
		m_PultInfo = tempStr;
		m_LineDlg->m_CurrentPult.SetWindowText(m_PultInfo);
		UpdateData(FALSE);
	}
	*pResult = 0;
}
// ������� �� ��������� ����� ����� � ������
void CMFCDlg::SendSetLineToPult(BYTE NumPult, BYTE NumLine)
{
	//��� �������, ������ �� ������
	if (m_CurrentPult == -1) return;
	//���� ����� �� ���������, �� ������ �������� ����� �������� �����
	if (NumPult == 0) NumPult = m_PultArray[m_CurrentPult].m_SerNumPult;
	else
	{
		
	}
	//���� ������� �����, �� �������� � ����� 
	if(m_WorkReg) m_DevRS485->SetLineInPult(NumPult, NumLine);
}

void CMFCDlg::OnBnClickedWrite()
{
	int nopult=0;
	CString str, tmp;
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_TimerRun) return;

	//���� ��������� �� ���������
	if (!m_PrgLoaded) return;

	if (!m_btnWrite.IsWindowEnabled()) return;

	KillTimer(m_TimerEcho);

	EnableBtn(FALSE);

	m_ProgressRpt.ShowWindow(SW_SHOW);
	str = CheckPultInPrg();

	if (str != "")
	{
		tmp.Format(_T("� ��������� ���� ������������� ������ %s, ����������?"),str);

		if (IDNO == AfxMessageBox(tmp,MB_YESNO))
		{
			m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
			m_ProgressRpt.ShowWindow(SW_HIDE);
			EnableBtn(TRUE);
			m_intCurrentPosPrg = 0;
			MainTimer = 0;
			m_StopTime = 0;
			UpdateData(FALSE);
			return;
		}
	}

	//������ � �����
	for (int i = 0; i < m_PultArray.GetCount(); i++)
	{
	//�������� ����������� ������ � ��������� � � ������
	if(PultInList(m_PultArray.GetAt(i).m_SerNumPult))
	
		if (WritePrgToPult(m_PultArray.GetAt(i).m_SerNumPult))
		{
			m_Protocol = _T("��������� ������� �������� � �����");
			m_btnWrite.SetColour(COLOR_BLACK,COLOR_GREEN);
			//������� ���� � ������ ��������
			m_btnCheck.ResetColour(true);

		}
		else
		{
			m_Protocol = _T("������ ������ ���������");
			m_btnWrite.SetColour(COLOR_BLACK,COLOR_RED);
		}



	}//for

	
//	str.Format(_T("�� ���������� %d �������, �������������� � ���������"),nopult);

//	if(nopult !=0)
//		MessageBox(str);


	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	m_ProgressRpt.ShowWindow(SW_HIDE);

	m_intCurrentPosPrg = 0;
	MainTimer = 0;
	m_StopTime = 0;
	
	//�������� ������ ������ ������ ���������
	m_ListPrg.SetCurSel(m_intCurrentPosPrg);
	//������ ����� � ��������� - � ������
	m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
	EnableBtn(TRUE);
	m_btnBegin.EnableWindow(FALSE);
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
	UpdateData(FALSE);
}
// ������� � ������ ���������
void CMFCDlg::OnBnClickedBegin()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();

	if (m_FindStart) return;
	m_MsgDlg->ShowWindow(SW_HIDE);
	
	if (m_TimerRun) return;
	//���� ���� ���������� �� ����� (��� ������ ������������)
	if (!m_btnBegin.IsWindowEnabled()) return; 

	m_intCurrentPosPrg = 0;
	MainTimer = 0;
	m_StopTime = 0;
	if (m_Timer) KillTimer(m_Timer);
	m_MainTimer.SetText(_T("0:00:00.00"));
	m_EndPrg.SetText(TimeToString(m_TimePrg));
	m_EndSound.SetText(TimeToString(m_TimeSound));

	//���� ��������� ���������
	if (m_PrgLoaded) 
	{
		//�������� ������ ������ ������ ���������
		m_ListPrg.SetCurSel(m_intCurrentPosPrg);
		//������ ����� � ��������� - � ������
		m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
	}


	//���� ��������� ����������, �� � ������
	if (m_SoundLoaded)
	{
		m_Player.Pause();
		__int64 duration = m_Player.GetDuration();
		__int64 pos =  1;
		m_Player.SetPositions(&pos, &duration, true);
	}
	//������� ���� ������� � ��������� �������
	for (int i = 0; i < m_PultArray.GetCount(); i++)
		if (!m_DevRS485->SendBegin(m_PultArray.GetAt(i).m_SerNumPult))
		{
			CString str;
			str.Format(_T("��� ����� � ������� � %d"),m_PultArray.GetAt(i).m_SerNumPult);
			MessageBox(str,_T("��������!!!"),MB_ICONERROR);
		}
	m_Protocol = _T("��������� � ������");
	UpdateData(FALSE);

}
//������� �� ������������ ���������
LRESULT CMFCDlg::OnEndProgram(WPARAM wParam, LPARAM lParam)
{
	//m_MsgDlg->SetColorDlg(COLOR_LLGREY);
	//m_MsgDlg->SetTextDlg(_T("��������� ���������"));
	//m_MsgDlg->ShowWindow(SW_SHOW);
	KillTimer(m_Timer);
	m_Protocol = _T("��������� ���������");
	UpdateData(FALSE);
	EnableBtn(TRUE);
return 1;
}
//�������� ��������� � �������
void CMFCDlg::OnBnClickedCheck()
{
	BOOL okCheck = FALSE;
	CString tmp = "";
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	//���� ��������� �� ���������
	if (!m_PrgLoaded) return;
	if (m_FindStart) return;
	if (m_TimerRun) return;
	//��� ������ �������������, ���� ������ ����������, �� �����
	if (!m_btnCheck.IsWindowEnabled()) return;



	m_ProgressRpt.ShowWindow(SW_SHOW);

	CString str = CheckPultInPrg();

	if (str != "")
	{
		tmp.Format(_T("� ��������� ���� ������������� ������ %s, ����������?"),str);
		if (IDNO == AfxMessageBox(tmp,MB_YESNO)) return;
	}

	EnableBtn(FALSE);

	KillTimer(m_TimerEcho);

	//�������� �������
	for (int i = 0; i < m_PultArray.GetCount(); i++)
	{
		tmp.Format(_T("�������� ��������� ������ �%d"),m_PultArray.GetAt(i).m_SerNumPult);
		if (!CheckPrg(m_PultArray.GetAt(i).m_SerNumPult))
		{
			tmp.Format(_T("��������� �� ��������� ��� ������ � %d"),i);			
			m_btnCheck.SetColour(COLOR_BLACK,COLOR_RED);
			okCheck = FALSE;
			UpdateData(FALSE);
			break;
		}
		else okCheck = TRUE;
	}
	if (okCheck)
	{
		m_Protocol = _T("��������� ������� ���������");
		m_btnCheck.SetColour(COLOR_BLACK,COLOR_GREEN);
		UpdateData(FALSE);
	}
	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	m_ProgressRpt.ShowWindow(SW_HIDE);

	m_intCurrentPosPrg = 0;
	MainTimer = 0;
	m_StopTime = 0;
	//�������� ������ ������ ������ ���������
	m_ListPrg.SetCurSel(m_intCurrentPosPrg);
	//������ ����� � ��������� - � ������
	m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
	EnableBtn(TRUE);
	m_btnBegin.EnableWindow(FALSE);
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);
}
// ������ ��������� � �����
BOOL CMFCDlg::WritePrgToPult(BYTE SerNumPult)
{
	//��������� ����������� ������ ������
	if (!m_DevRS485->EnableRedraw(SerNumPult,FALSE)) return FALSE;

	if (!m_PrgLoaded) return FALSE;
	
	

	//������ �������������� ���������� � �����
	if (!WriteInfo(SerNumPult))	return FALSE;

	if (!WritePrg(SerNumPult))	return FALSE;
	
	if (!WritePrgToFlash(SerNumPult)) return FALSE;
	
	//�������� ����������� ������ ������
	if (!m_DevRS485->EnableRedraw(SerNumPult,TRUE))	return FALSE;



	return TRUE;
}
// ������ ��������� � �����
BOOL CMFCDlg::WritePrg(BYTE SerNumPult)
{
	//������� ���� � ������ ���������
	int prgCount = 0;
	
	//������� ����� ������ �� ��������� ������
	int NumPult = GetNumPult(SerNumPult);

	if (NumPult == -1) return FALSE;

	for (POSITION pos = m_ListLine.GetHeadPosition(); pos != NULL;)
	{
		if (NumPult == m_ListLine.GetAt(pos).NumPult)
		{
			//����� ����� ����� �� ��������
			Prg[prgCount] = m_ListLine.GetAt(pos).NumLine;
			prgCount++;
			
			//����� � ����� ����� �������� �����
			DWORD tempTime = m_ListLine.GetAt(pos).TimePusk;
			Prg[prgCount] = static_cast<BYTE>(tempTime & 0x00FF);
			prgCount++;

			Prg[prgCount] = static_cast<BYTE>((tempTime>>8) & 0x00FF);
			prgCount++;

			Prg[prgCount] = static_cast<BYTE>((tempTime>>16) & 0x00FF);
			prgCount++;
		}//if
		m_ListLine.GetNext(pos);

	}//for

	if (prgCount > 16384)
	{
		AfxMessageBox(_T("� ��������� ��� ������ ������ ����� 4000 ���������"));
		return FALSE;
	}


	//������ ������ 0xFF ������� ��������� ���������
	Prg[prgCount]=0x00FF;
	prgCount++;
	Prg[prgCount]=0x00FF;
	prgCount++;
	Prg[prgCount]=0x00FF;
	prgCount++;
	Prg[prgCount]=0x00FF;
	prgCount++;

	CString rptStr;
	rptStr.Format(_T("�������� ��������� � ����� �%d"),NumPult);

	m_ProgressRpt.SetRange(0,prgCount);
	m_ProgressRpt.SetPos(0);
	m_Protocol = rptStr; 
	UpdateData(FALSE);

	// CMD_START_PRG param1 - ������� ���� ������� ���������
	//               param2 -������� ���� ������� ���������
	int tmp = prgCount;
	BYTE param1 = tmp & 0x00FF;
	tmp = prgCount;
	BYTE param2 = tmp>>8;

	m_DevRS485->m_TxBuffer[1] = CMD_START_PRG;
	m_DevRS485->m_TxBuffer[2] = param1;
	m_DevRS485->m_TxBuffer[3] = param2;
	m_DevRS485->m_TxBuffer[4] = 0;
	m_DevRS485->m_TxBuffer[5] = 0;

	if (!m_DevRS485->SetDataToPult(SerNumPult,5000)) return FALSE;
	//�������� ���������� ��������� � ����������
	//������ - 1-����� �����,2-��. ����,3-��,4-������� ���� �������
	for (int w = 0; w < prgCount; w=w+8)
	{
		m_DevRS485->m_TxBuffer[1] = CMD_SET_REG;
		m_DevRS485->m_TxBuffer[2] = Prg[w];
		m_DevRS485->m_TxBuffer[3] = Prg[w+1];
		m_DevRS485->m_TxBuffer[4] = Prg[w+2];
		m_DevRS485->m_TxBuffer[5] = Prg[w+3];
		m_DevRS485->m_TxBuffer[6] = Prg[w+4];
		m_DevRS485->m_TxBuffer[7] = Prg[w+5];
		m_DevRS485->m_TxBuffer[8] = Prg[w+6];
		m_DevRS485->m_TxBuffer[9] = Prg[w+7];
		// 10 � 11 ����� ������� - ������ ������ ������
		m_DevRS485->m_TxBuffer[10] = w & 0x00FF;
		m_DevRS485->m_TxBuffer[11] = w >> 8;

		if (!m_DevRS485->SetDataToPult(SerNumPult,2000))
					return FALSE;
		m_ProgressRpt.SetPos(w);
	}
	return TRUE;
}
// ������ � ����� ���������� � ��������� 
//���������� �������� ���������, ��� ����� ��������� � ���� ������
//����� 4 ������� ��� ����� - 16 ����, 2 ������� ���� ������ 8 ����
// ���� 1-���� ����, 2 ����-�����, 3 ���� -���, 4 ����- ����, 5 ���� -������
// 6-���� - ����� �������� - ����, 7 - ������, 8 - �������

BOOL CMFCDlg::WriteInfo(BYTE SerNumPult)
{
	CString NamePrg;

	//����� ������������ ���������
	NamePrg = m_PrgFileName.Mid(11);
	NamePrg.GetBufferSetLength(16);
	
	m_DevRS485->m_TxBuffer[1] = CMD_START_TXT;
	m_DevRS485->m_TxBuffer[2] = static_cast<BYTE>(NamePrg[0]);
	m_DevRS485->m_TxBuffer[3] = static_cast<BYTE>(NamePrg[1]);
	m_DevRS485->m_TxBuffer[4] = static_cast<BYTE>(NamePrg[2]);
	m_DevRS485->m_TxBuffer[5] = static_cast<BYTE>(NamePrg[3]);
	m_DevRS485->m_TxBuffer[6] = static_cast<BYTE>(NamePrg[4]);
	m_DevRS485->m_TxBuffer[7] = static_cast<BYTE>(NamePrg[5]);
	m_DevRS485->m_TxBuffer[8] = static_cast<BYTE>(NamePrg[6]);
	m_DevRS485->m_TxBuffer[9] = static_cast<BYTE>(NamePrg[7]);
	m_DevRS485->m_TxBuffer[10] = static_cast<BYTE>(NamePrg[8]);
	m_DevRS485->m_TxBuffer[11] = static_cast<BYTE>(NamePrg[9]);
	if(!m_DevRS485->SetDataToPult(SerNumPult,DELAY_COMMAND)) return FALSE;

	//����� ���� ������ ���������
	CTime t = CTime::GetCurrentTime();
	BYTE dd = static_cast<BYTE>(t.GetDay());
	BYTE mm = static_cast<BYTE>(t.GetMonth());
	BYTE yy = static_cast<BYTE>(t.GetYear()-2000);
	BYTE hh = static_cast<BYTE>(t.GetHour());
	BYTE minute = static_cast<BYTE>(t.GetMinute());

	m_DevRS485->m_TxBuffer[1] = CMD_SEND_TXT;
	m_DevRS485->m_TxBuffer[2] = static_cast<BYTE>(NamePrg[10]);
	m_DevRS485->m_TxBuffer[3] = static_cast<BYTE>(NamePrg[11]);
	m_DevRS485->m_TxBuffer[4] = static_cast<BYTE>(NamePrg[12]);
	m_DevRS485->m_TxBuffer[5] = static_cast<BYTE>(NamePrg[13]);
	m_DevRS485->m_TxBuffer[6] = static_cast<BYTE>(NamePrg[14]);
	m_DevRS485->m_TxBuffer[7] = static_cast<BYTE>(NamePrg[15]);
	m_DevRS485->m_TxBuffer[8] = dd;
	m_DevRS485->m_TxBuffer[9] = mm;
	m_DevRS485->m_TxBuffer[10] = yy;
	m_DevRS485->m_TxBuffer[11] = hh;
	if(!m_DevRS485->SetDataToPult(SerNumPult,DELAY_COMMAND)) return FALSE;

	// ������ ����� ��������� �� 10 ��
	DWORD timePrg = m_ListLine.GetTail().TimePusk;
	BYTE h = static_cast<BYTE>(timePrg/360000);
	BYTE m = static_cast<BYTE>((timePrg%360000)/6000);
	BYTE s = static_cast<BYTE>(((timePrg%36000)%6000)/100);

	m_DevRS485->m_TxBuffer[1] = CMD_END_TXT;
	m_DevRS485->m_TxBuffer[2] = minute;
	m_DevRS485->m_TxBuffer[3] = h;
	m_DevRS485->m_TxBuffer[4] = m;
	m_DevRS485->m_TxBuffer[5] = s;

	if(!m_DevRS485->SetDataToPult(SerNumPult,DELAY_COMMAND)) return FALSE;
	return TRUE;
}
// ���������� ����� ������ �� ��������� ������
BYTE CMFCDlg::GetNumPult(BYTE SerNumPult)
{
	//���������� ��� ������ � ������� � �� �������
	//������ ������ ���������� �o���
	for (int i = 0; i < m_PultArray.GetSize(); i++)
	{
		if (m_PultArray[i].m_SerNumPult == SerNumPult)
				return m_PultArray[i].m_NumPult;
	
	}
	return -1;
}
// ���������� ���������� �������
void CMFCDlg::RefreshTimer(void)
{
	// �������� ����� � ������� ������ ���� � ������ �� 10 ��
	DWORD delta = static_cast<DWORD>(MainTimer + m_StopTime);			
	

	if (m_TimePrg > delta)
	{
		m_MainTimer.SetText(TimeToString(delta));
		m_EndPrg.SetText(TimeToString(m_TimePrg - delta));
	}
	else
	{
		//���� ����� ������� ������, �� ���������� ������� �����
		if (m_CompControl)	m_MainTimer.SetText(TimeToString(m_TimePrg));
		else m_MainTimer.SetText(TimeToString(delta));

		m_EndPrg.SetText(_T("0:00:00.00"));
	}
	if (m_SoundLoaded)
	{
		DWORD tmp;
		tmp = static_cast<DWORD>(m_Player.GetCurrentPosition()/100000);
			if (m_TimeSound > tmp) 
				m_EndSound.SetText(TimeToString(m_TimeSound - tmp));
			else
				m_EndSound.SetText(_T("0:00:00.00"));
	}
}
//������
void CMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	CString str;

	switch(nIDEvent)
	{
	case TIMER_MAIN:
		RefreshTimer();
		break;
	case TIMER_WAIT_START:
		if (m_WaitFromClock)
		{
			str = _T("�������� �� ����� - ")+ TimeToString(WaitTimer).Mid(0,7);
			m_MsgDlg->SetTextDlg(str);
			break;
		} 
		else
		{
			str = _T("�������� ����� ��������� � ������ - ")+ TimeToString(WaitTimer);
			m_MsgDlg->SetTextDlg(str);
			break;
		}

	
	case TIMER_ECHO:
		CheckPult();
		break;
	default:
		break;
	}
	return;
	//CDialog::OnTimer(nIDEvent);
}
//������� �� ����� ���������
LRESULT CMFCDlg::OnPauseProgram(WPARAM wParam, LPARAM lParam)
{
	RefreshTimer();
	return 1;
}
//��������� ������
void CMFCDlg::OnBnClickedPreference()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_TimerRun) return;
	if (m_FindStart) return;
	if (!m_PrefBtn.IsWindowEnabled()) return;

	if(m_CurrentPult == -1)
	{
		m_PrefBtn.SetCheck(0);
		return;
	}

	if (m_PrefBtnValue == TRUE)
	{
		m_PrefBtnValue = FALSE;
		UpdateData(FALSE);

		m_LineDlg->ShowWindow(SW_HIDE);
	} 
	else
	{
		m_PrefBtnValue = TRUE;
		UpdateData(FALSE);
		//���������� ������� ���� ���������
		if (m_PrgLoaded)
		{
			CLineInfo* ci;
			for (int x=IDC_LINE1;x<(IDC_LINE40+1);x++)
			{
				//��������� ���������� ��������� ��� ������ ����� �������� ������
				ci=(CLineInfo*)m_LineDlg->GetDlgItem(x);
				ci->SetInPrg(CountLineInPult(m_PultArray[m_CurrentPult].m_SerNumPult,x-IDC_LINE1+1));
			}

		}
		m_LineDlg->m_CurrentPult.SetWindowText(m_PultInfo);
		m_LineDlg->ShowWindow(SW_SHOW);
	}
	//����������� ������ � ����� � ���������� m_PrefBtnValue
	UpdateData(TRUE);
}
//������� �������� ���������
void CMFCDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString tmp;
	int delay = m_Delay.GetPos();
	double m = delay/3.0;
	tmp.Format(_T("�������� %d �c (%.0f �)"),delay,m);
	m_DelayTxt = tmp;
	UpdateData(FALSE);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
//����� ������� �����
void CMFCDlg::OnBnClickedButtonDebug()
{
	m_dlgDebug = new CDebugDlg;
	m_dlgDebug->Create(CDebugDlg::IDD, this);
	m_dlgDebug->ShowWindow(SW_SHOWNORMAL);

}
// �������� ������
BOOL CMFCDlg::CheckPrg(BYTE NumPult)
{

	if (!CheckPultInPrg())
	{
		AfxMessageBox(_T("� ��������� ���� ������������� ������!!!"),MB_OK);
		return FALSE;
		
	}


	//��������� ����������� ������ ������
	if (!m_DevRS485->EnableRedraw(NumPult,FALSE)) return FALSE;


	//�������� ��������� � ����� ������
	if (!WritePrg(NumPult)) return FALSE;
	//��������������� ������� �������� ���������
    if (!m_DevRS485->CheckPrg(NumPult)) return FALSE;

	//�������� ����������� ������ ������
	if (!m_DevRS485->EnableRedraw(NumPult,TRUE)) return FALSE;


	return TRUE;
}
// ���������� ��������� �� ������ �� ���������� ����
BOOL CMFCDlg::WritePrgToFlash(BYTE SerNumPult)
{
	//�������������� ��������� �� ���������� ����-������
	m_Protocol = _T("���������� �� ���������� ������");
	m_DevRS485->m_TxBuffer[1] = CMD_STOP_PRG;
	UpdateData(FALSE);
	if (!m_DevRS485->SetDataToPult(SerNumPult,10000))
	{
		m_Protocol = _T("������ ���������� �� ���������� ������");
		return FALSE;
	}
	// ����� ��� ���������� ���������� ����� �� ������
	return TRUE;
}
// ������� ���������� ����� � ��������� ��� ������
int CMFCDlg::CountLineInPult(BYTE NumPult)
{
	int CountLine = 0;
	for (POSITION pos = m_ListLine.GetHeadPosition(); pos != NULL; pos++)
	{
		if (NumPult == m_ListLine.GetAt(pos).NumPult) 
			CountLine++;
	}
	return CountLine;
}
// ������� ���������� ��������� �� �����
int CMFCDlg::CountLineInPult(BYTE NumPult, BYTE NumLine)
{
	int CountLine = 0;
	LinePrg Line;

	for (POSITION pos = m_ListLine.GetHeadPosition(); pos != NULL;)
	{
		Line =  m_ListLine.GetNext(pos);
		if	(
			(NumPult == Line.NumPult) &&
			(NumLine == Line.NumLine)
			)
			CountLine++;
	}
	return CountLine;
}
// �������� ����� ������
void CMFCDlg::ScanPult(int NumPult)
{
	KillTimer(m_TimerEcho);
	if (m_CurrentPult == -1) return;
	for (BYTE i = 0; i < MAX_LINE; i++)
	{
		ScanPult(NumPult,i);
	}
	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
}
// �������� ����� ����� ������
void CMFCDlg::ScanPult(int NumPult, BYTE NumLine)
{
	int tmp=0;
	int tmp1=0;
	if (m_CurrentPult == -1) return;
	KillTimer(m_TimerEcho);
	m_DevRS485->m_RxBuffer[10] = 0;
	m_DevRS485->m_RxBuffer[11] = 0;
	//�������� ������������� �����
	if (m_DevRS485->GetRes(m_PultArray[NumPult].m_SerNumPult,NumLine))
	{
		m_PultArray[NumPult].m_ResLine[NumLine-1] = ((m_DevRS485->m_RxBuffer[11] << 8) | m_DevRS485->m_RxBuffer[10]);
	} 
	else
	{
		m_PultArray[NumPult].m_ResLine[NumLine-1] = ERROR_VALUE;
	}
	
	tmp1 = m_PultArray[NumPult].m_ResLine[NumLine-1];

	//����� �����
	if (tmp1 < MIN_RES)
	{
		m_PultArray[NumPult].m_CountLine[NumLine-1] = 0;
		m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
		return;
	}

	//���� ����� �� �������
	if (tmp1 < RES_1_LINE+70)
	{
		m_PultArray[NumPult].m_CountLine[NumLine-1] = 1;
		m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
		return;
	}

	//���� ����� �� �������
	if (tmp1 < RES_2_LINE+30)
	{
		m_PultArray[NumPult].m_CountLine[NumLine-1] = 2;
		m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
		return;
	}
	//���� ��� ������ �� �����, �� �������� �������
	//�������� ������� �����
	if (m_DevRS485->GetCap(m_PultArray[NumPult].m_SerNumPult,NumLine))
		{
			m_PultArray[NumPult].m_CapLine[NumLine-1] = ((m_DevRS485->m_RxBuffer[11] << 8) | m_DevRS485->m_RxBuffer[10]);
		} 
	else
		{
			m_PultArray[NumPult].m_CapLine[NumLine-1] = ERROR_VALUE;
		}

	//��� �������
	tmp = m_PultArray[NumPult].m_CapLine[NumLine-1];
	

		//��������� ���������� ����� �� �������
	//� ����� ������ �� ������������ (������������ �������������, ����������� �������)
	if(m_PultArray[NumPult].m_CapLine[NumLine-1] < MIN_CAP)
	{
		m_PultArray[NumPult].m_CountLine[NumLine-1] = 0;
	}
	// ������� � ��������� MIN_CAP .. MAX_CAP
	else if (
		(m_PultArray[NumPult].m_CapLine[NumLine-1] >= MIN_CAP) &&
		(m_PultArray[NumPult].m_CapLine[NumLine-1] < MAX_CAP)
		)
	{
		//�������� ��������� �� 10 ����� �� �������
		if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 400)
		{
			if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 18) 
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 1;	
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 48)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 2;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 100)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 3;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 145)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 4;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 189)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 5;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 233)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 6;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 277)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 7;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 319)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 8;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 361)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 9;
			else 
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 10;
		} 
		//�������� ��������� ����� 10 ����� �� �������
		else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 840)
		{
			if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 456) 
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 11;	
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 502)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 12;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 548)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 13;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 390)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 14;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 630)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 15;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 675)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 16;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 720)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 17;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 760)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 18;
			else if (m_PultArray[NumPult].m_CapLine[NumLine-1] < 800)
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 19;
			else 
				m_PultArray[NumPult].m_CountLine[NumLine-1] = 20;
		}
		else
			m_PultArray[NumPult].m_CountLine[NumLine-1] = m_PultArray[NumPult].m_CapLine[NumLine-1]/40;

	}
	// ������� ������ ����������� ������� �� �� ������
	else if (
		(m_PultArray[NumPult].m_CapLine[NumLine-1] > MAX_CAP) 
		&&
		(m_PultArray[NumPult].m_CapLine[NumLine-1] != ERROR_VALUE)
		)
	{
			m_PultArray[NumPult].m_CountLine[NumLine-1] = 1;
	}
	else
	{
		m_PultArray[NumPult].m_CountLine[NumLine-1] = ERROR_VALUE;
	}		

	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);
	return;
}
//����� �� ���������
void CMFCDlg::OnOK()
{
	int userReply;
	userReply = MessageBox(_T("�� �������, ��� ������ �����?"),_T("����� �� ���������"),MB_YESNO|MB_ICONQUESTION);

	if (IDYES == userReply)
	{
		m_log->Add(_T("����� �� ���������"));
		CDialog::OnCancel();
	}
	return;
}
//����� �� ���������
void CMFCDlg::OnCancel()
{
	int userReply;

	userReply = MessageBox(_T("�� �������, ��� ������ �����?"),_T("����� �� ���������"),MB_YESNO|MB_ICONQUESTION);
	if (IDYES == userReply)
	{
		m_log->Add(_T("����� �� ���������"));
		CDialog::OnCancel();
	}
	return;
}
//����� ������ ������� ������� ���������� TRUE ���� ���� ���������� ������ 
BOOL CMFCDlg::FindDouble(void)
{
	BOOL result = TRUE;
	
	//������� ���� ��������� �� 1 �� ��������������
	for (int i = 0; i < m_PultArray.GetCount()-1; i++)
		// ������� ��������� �� ���������� ����� �������� 
		// i �� ����������
		for (int j = i+1; j < m_PultArray.GetCount(); j++)
			if (m_PultArray.GetAt(i).m_NumPult == m_PultArray.GetAt(j).m_NumPult)
				return TRUE;
	return FALSE;
}
// ��������� ������ �������� ������
BOOL CMFCDlg::ChangeNumPult(BYTE NewNumPult)
{
	CString tmp;
	if (m_CurrentPult == -1) return FALSE;


	BYTE SerNumPult = m_PultArray[m_CurrentPult].m_SerNumPult;

	if (m_DevRS485->ChangeNumPult(SerNumPult,NewNumPult))
	{
		m_PultArray[m_CurrentPult].m_NumPult= NewNumPult;
		tmp.Format(_T("%d"),NewNumPult);
		m_PultList.SetItemText(m_CurrentPult,0,tmp);
		m_btnWrite.ResetColour(true);
		m_btnCheck.ResetColour(true);
		return TRUE;
	}
	return FALSE;
}
// ��������� ������������ �������� �������
BOOL CMFCDlg::SetPulse(int wPulse)
{
	if (m_CurrentPult == -1) return FALSE;

	return m_DevRS485->SetPulse(m_PultArray[m_CurrentPult].m_SerNumPult,wPulse);
}
// ����� ����� � �������
void CMFCDlg::CheckPult(void)
{
	CString tmp, minus, ost;
	double tempV24,tempV12,pulse;
	WORD tempTemper;
	bool result_echo;
	
	//���� ��� �������, �� �����
	if (m_CurrentPult == -1) return;

	for (int i = 0; i < m_PultArray.GetCount(); i++)
	{
		result_echo = SafeEcho(i);

		if(result_echo)
		{
	
			tempV12 = m_PultArray[i].m_Power12*33/255.0; 
			tempV24 = m_PultArray[i].m_Power24*33/255.0;
			pulse = m_PultArray[i].m_Pulse;

			
			tmp.Format(_T("%.2f"),tempV12);
			m_PultList.SetItemText(i,1,tmp);
			tmp.Format(_T("%.2f"),tempV24);
			m_PultList.SetItemText(i,2,tmp);
			tmp.Format(_T("%.0f"),pulse);
			m_PultList.SetItemText(i,4,tmp);


			if(m_PultArray[m_CurrentPult].m_Temper & 0x8000){
				tempTemper = ~(m_PultArray[m_CurrentPult].m_Temper)+1;
				minus = _T("-");
			}else{
				tempTemper = m_PultArray[m_CurrentPult].m_Temper;
				minus = _T("+");
			}
			if (tempTemper & 0x08) ost=_T(".5"); else ost=_T(".0");

			tempTemper = tempTemper>>4;
			tmp.Format(_T("%s%d"),minus,tempTemper);
			tmp = tmp + ost;
			m_PultList.SetItemText(i,3,tmp);
			tmp.Format(_T("%s%d"),minus,tempTemper);


			//������������ ���� ������ �� ���������� 24 �
			if ((tempV24 < MIN_V24) && !(m_PultArray[i].m_f24VNotReady))
			{
				m_PultArray[i].m_f24VNotReady = true;
				tmp.Format(_T("������� 24� ������ %03d = %.2f"),m_PultArray[i].m_NumPult,tempV24);
				m_log->Add(tmp);
			}

			//
			//������� ��� �������������� ������� 24�
			if ((tempV24 >= MIN_V24) && (m_PultArray[i].m_f24VNotReady))
			{
				m_PultArray[i].m_f24VNotReady = false;
			}


			//�������� ������ ������ ���������
			if (((m_PultArray[i].m_WorkMode & REG_PAUSE) != 0) && (m_TimerRun != FALSE))
			{
				OnBnClickedStop();
				//������������ ����� � ������ ��������
				//�������� ���� ��������� ���������
			//	m_MsgDlg->SetColorDlg(COLOR_RED);
			//	m_MsgDlg->SetTextDlg(_T("��������� ���������"));
			//	m_MsgDlg->ShowWindow(SW_SHOW);

				m_Protocol = _T("��������� ���������");
				UpdateData(FALSE);

			}
			//����� � ������ ��������� ���������
			if	((m_PultArray[i].m_WorkMode & REG_END) != 0)
			{
				OnEndProgram(0,0);	
			}
			//�������� ������� ������
			if ((m_PultArray[i].m_WorkMode & REG_WORK) != 0)
			{
				//������������ ����� � ������� ������
				//��������� ������ ����, ������� ���
				for (int z = 0; z < COLUMNS_m_ColListCtrl; z++)
					m_PultList.SetItemBackgndColor((ITEM_COLOR)GREEN, i, z);
			}
			else if ((m_PultArray[i].m_WorkMode & REG_TEST) != 0)
			{
				//������������ ����� � �������� ������ 
				//��������� ������ ����, ����� ���
				for (int z = 0; z < COLUMNS_m_ColListCtrl; z++)
					m_PultList.SetItemBackgndColor((ITEM_COLOR)WHITE, i, z);
				/*�������� ��������� 24�
				if (tempV24 < MIN_V24)	
					m_PultList.SetItemBackgndColor((ITEM_COLOR)RED, i, 2);
				else 
					m_PultList.SetItemBackgndColor((ITEM_COLOR)WHITE, i, 2);

				*/
			}
		}
		else
		{
			//����� ����������, ������� ������ ������ ������
			for (int z = 0; z < COLUMNS_m_ColListCtrl; z++)
					m_PultList.SetItemBackgndColor((ITEM_COLOR)RED, i, z);
		}
	}

	//�������� ������ ������ ��� ���������� ������ �������
	if (m_BlinkWork)
	{
		if (m_WorkGreen)
		{
			m_btnWorkF1.ResetColour(true);
			m_imgWork.ShowWindow(SW_HIDE);
			m_WorkGreen=false;
		} 
		else
		{
			m_btnWorkF1.SetColour(COLOR_BLACK,COLOR_GREEN);
			m_imgWork.ShowWindow(SW_SHOW);
			m_WorkGreen=true;
		}
	} 
}
// ���������, ���������� ������ 
void CMFCDlg::EnableBtn(BOOL EN_DIS)
{
	//���� ��������� �� ���������, �� ��������� ������
	//������, ��������, � ������, �����, ����
	if (!m_PrgLoaded)
	{
		m_btnWrite.EnableWindow(FALSE);
		m_btnCheck.EnableWindow(FALSE);
	}
	else
	{
		m_btnWrite.EnableWindow(EN_DIS);
		m_btnCheck.EnableWindow(EN_DIS);
		m_btnBegin.EnableWindow(EN_DIS);
		if (m_WorkReg)
		{
			m_btnStart.EnableWindow(TRUE);
			m_btnStop.EnableWindow(TRUE);
		}
		else
		{
			m_btnStart.EnableWindow(FALSE);
			m_btnStop.EnableWindow(FALSE);
		}
	}

	if (m_PultArray.GetCount() == 0)
	{
		m_PrefBtn.EnableWindow(FALSE);
	}
	else
	{
		m_PrefBtn.EnableWindow(EN_DIS);
	}

	m_btnPrg.EnableWindow(EN_DIS);
	m_btnSound.EnableWindow(EN_DIS);
	m_btnFind.EnableWindow(EN_DIS);
	return;
}
//��������� ������ ������ ��������� � ������
void CMFCDlg::OnBnClickedPrgInPult()
{
	m_ListPrg.SetFocus();
	if (m_TimerRun) return;
	if (m_FindStart) return;
	if(m_CurrentPult == -1) return;
	// ���� �������� ���������, �� ������������ ������� ���������
	if (m_TimerRun) return;

	if (!m_btnPrgInPult.IsWindowEnabled()) return;
		
	// ���� ���������� � ����������
	// �� ������ �� ���������� � ������ 
	if (m_PrgInComp)
	{
		m_PrgInComp = FALSE;
		m_PrgArray[m_CurrentPrg].m_PrgInComp=FALSE;
		m_btnPrgInPult.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnPrgInComp.ResetColour(true);
		//�������� ��������� �� ������
		OnBnClickedBegin();
	}
}
//��������� ������ ������ ��������� � ����������
void CMFCDlg::OnBnClickedPrgInComp()
{
	m_ListPrg.SetFocus();
	//if(m_CurrentPult == -1) return;
	// ���� �������� ���������, �� ������������ ������� ���������
	if (m_TimerRun) return;
	if (m_FindStart) return;
	if (!m_btnPrgInComp.IsWindowEnabled()) return;

	// ���� ���������� � ����������
	// �� ������ �� ���������� � ������ 
	if (!m_PrgInComp)
	{
		m_PrgInComp = TRUE;
		m_PrgArray[m_CurrentPrg].m_PrgInComp=TRUE;
		m_btnPrgInComp.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnPrgInPult.ResetColour(true);
		OnBnClickedBegin();

	}
}


// ��������� ����� � ������
void CMFCDlg::SendClearLineToPult(BYTE NumPult, BYTE NumLine)
{
	//��� �������, ������ �� ������
	if (m_CurrentPult == -1) return;
	//���� ����� �� ���������, �� ������ �������� ����� �������� �����
	if (NumPult == 0) NumPult = m_PultArray[m_CurrentPult].m_SerNumPult;
	else
	{

	}
	//���� ������� �����, �� �������� � ����� 
	if(m_WorkReg) m_DevRS485->ClearLineInPult(NumPult, NumLine);
}

// ������ ������������ ��������
int CMFCDlg::GetPulse()
{
	int Pulse;
	//��� �������, ������ �� ������
	if (m_CurrentPult == -1) return 0;
	KillTimer(m_TimerEcho);
	if (m_DevRS485->GetPulse(m_PultArray[m_CurrentPult].m_SerNumPult))
		Pulse = (m_DevRS485->m_RxBuffer[9]<<8) | m_DevRS485->m_RxBuffer[8];
	else Pulse=0;

	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);

	return Pulse;
	 
}


// ������������ �� ����� � ������ ������� �� ������ ������
bool CMFCDlg::PultIsPresent(int NumPult)
{
	//���� ��� �������, �� �����
	if (m_CurrentPult == -1) return false;

	for (int i = 0; i < m_PultArray.GetCount(); i++)
		if (NumPult == m_PultArray[i].m_NumPult) return true;
	

	return false;
}


// �������� ������� ������, ��������������� � ���������
CString CMFCDlg::CheckPultInPrg()
{
	//T�� LinePrg
	LinePrg* pLine;
	CString result = "",tmp = "";
	

	//���� ��������� �� ���������, �� �������
	if (!m_PrgLoaded) return "";


	//������� ���� ������� ���������
	m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
	while(m_posCurrentPosPrg != NULL)
	{
		//������� ������� ������ ���������
		pLine = &m_ListLine.GetNext(m_posCurrentPosPrg);
		//���� ����� ����������� � ������ ������� 
		if(!PultIsPresent(pLine->NumPult))
		{
			tmp.Format(_T("-%d-"), pLine->NumPult);
			//���� ������������� ����� ��� �� ������� � ������
			if (result.Find(tmp) == -1)
				result = result + tmp;
			
		}
	}// while(pMyHndl->m_posCurrentPos != NULL)

	return result;
}


void CMFCDlg::OnBnClickedWork()
{
	//���� ��� �������, �� ����� (��� ������� ��������)
	if (m_CurrentPult == -1) return;
	if (m_FindStart) return;

	// ������������ ��� �������� � ���������� ��������� ��� ������ ���������
	if (m_TimerRun) return;

	if (!m_btnWorkF1.IsWindowEnabled()) return;

	//���� �� ������� ����� �������, �� �������� 
	if (!m_BlinkWork)
		{
		m_BlinkWork = true;
		m_imgWork.ShowWindow(SW_SHOW);
		m_WorkReg= TRUE;
		//��������������� ������ �����
		EnableBtn(FALSE);
		m_btnStart.EnableWindow(TRUE);
		}
	else
	{
		m_btnWorkF1.ResetColour(true);
		m_WorkReg= FALSE;
		EnableBtn(TRUE);
		m_btnStart.EnableWindow(FALSE);
		m_btnStop.EnableWindow(FALSE);
		m_BlinkWork = false;
		m_imgWork.ShowWindow(SW_HIDE);
	}
}

//����� ������� ���������� ���������� � ������
void CMFCDlg::OnBnClickedStepReg()
{
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	if (m_FindStart) return;
	if(m_CurrentPult == -1) return;
	// ������������ ��������� ��� ������ ���������
	if (m_TimerRun) return;
	if (!m_btnStepReg.IsWindowEnabled()) return;

	//m_MsgDlg->ShowWindow(SW_HIDE);

	if (m_CompControl)
	{
		m_btnStepReg.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnComp.ResetColour(true);
		m_CompControl = false;
		m_PrgArray[m_CurrentPrg].m_CompControl= false;
	}
}


// ���������� ����� ������
bool CMFCDlg::SafeEcho(int NumPultInList)
{
bool result;
	cs.Lock();
	if(m_DevRS485->SendEcho(m_PultArray[NumPultInList].m_SerNumPult))
	{
		m_PultArray[NumPultInList].m_WorkMode = m_DevRS485->m_RxBuffer[2];
		m_PultArray[NumPultInList].m_Power24 = m_DevRS485->m_RxBuffer[3];
		m_PultArray[NumPultInList].m_Power12 = m_DevRS485->m_RxBuffer[4];
		m_PultArray[NumPultInList].m_Temper = ((m_DevRS485->m_RxBuffer[6]<<8) | m_DevRS485->m_RxBuffer[5]);
		m_PultArray[NumPultInList].m_NumPult = m_DevRS485->m_RxBuffer[7];
		m_PultArray[NumPultInList].m_Pulse = ((m_DevRS485->m_RxBuffer[9]<<8) | m_DevRS485->m_RxBuffer[8]);
		result = true;
	}
	else
	{
		m_PultArray[NumPultInList].m_WorkMode = 0;
		m_PultArray[NumPultInList].m_Power24 = 0;
		m_PultArray[NumPultInList].m_Power12 = 0;
		m_PultArray[NumPultInList].m_Temper = 0;
		m_PultArray[NumPultInList].m_NumPult = 0;
		m_PultArray[NumPultInList].m_Pulse = 0;
		result = false;
	}
	cs.Unlock();

	return result;
}


// ������� � ��������� ���� ������� � ��������������
int CMFCDlg::PauseAllPult(void)
{
	int result = 0;

	if(m_CurrentPult == -1) return 0;
	KillTimer(m_TimerEcho);

	//��������� ��� ������ � ����� �����
	for (int i = 0; i < m_PultArray.GetSize(); i++)
	{
		// ���� ����� ������� ��������� � �����, ��
		if	(!m_DevRS485->SendCommand(m_PultArray[i].m_SerNumPult,CMD_RIT_OFF,30))
		{
			result++;
		}
	}

	m_TimerEcho = SetTimer(TIMER_ECHO,TIME_ECHO_MS,NULL);

	return result;
}


// ������ ����� �������� ������� � ������
void CMFCDlg::ShowWaitStart(void)
{
	const COleDateTime dtNow = COleDateTime::GetCurrentTime();
	//����� �� ������ ��� ���������� beep
	m_ListPrg.SetFocus();
	// ���� �������� ���������, �� ������������ ������� ���������
	if (m_TimerRun) return;
	m_MsgDlg->ShowWindow(SW_HIDE);
	if(m_CurrentPult == -1) return;
	if (!m_btnPult.IsWindowEnabled()) return;

	// ���� ���������� � ����������
	// �� ������ �� ���������� � ������ 
	if (m_CompControl)
	{
		m_btnPult.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnComp.ResetColour(true);
		m_PrgArray[m_CurrentPrg].m_CompControl= false;
		m_CompControl = false;
		EnableBtn(FALSE);
	
		//���������� ���-�������
		KillTimer(m_TimerEcho);
		m_TimerEcho = NULL;
		m_TimerWaitStart = SetTimer(TIMER_WAIT_START,100,NULL);
		//�������� ������ �������� ������ ���������
		pWaitThread	= AfxBeginThread(WaitStartFromPult, this, THREAD_PRIORITY_NORMAL);
		m_MsgDlg->SetColorDlg(COLOR_LLGREY);
		m_MsgDlg->m_EnableClose = false;
		m_MsgDlg->ShowWindow(SW_SHOW);
	}
}


void CMFCDlg::OnBnClickedPrg(UINT nID)
{
//��� ���������� ��������� ������������ ���������
if (m_TimerRun) return;
if (m_FindStart) return;

m_CurrentPrg=nID-IDC_PRG1;
CColorButton* btn;


for(int i=IDC_PRG1; i<IDC_PRG8+1; i++)
{
	btn = (CColorButton*)GetDlgItem(i);
	btn->ResetColour();
}

btn = (CColorButton*)GetDlgItem(nID);
btn->SetColour(COLOR_BLACK,COLOR_GREEN);



//�������� ���������, ���� ����������
if (!m_PrgArray[m_CurrentPrg].m_FilePrg.IsEmpty())
{

	
	// �������� ������� � �������� ����� ��� ������
	CStdioFile File(m_PrgArray[m_CurrentPrg].m_FilePrg,CFile::
		modeRead|CFile::typeText);

	CString tempText = "";
	// ������ �� ����� ������
	CString& ref = tempText;
	CString tempTime;
	CString tempNumPult;
	CString tempNumLine;
	CString tempDescr;

	m_ListLine.RemoveAll();

	int i = 1;
	m_ListPrg.ResetContent();
	while(File.ReadString(ref))// ���������� ������ �� ������ tempText
	{
		int nTokenPos = 0;
		tempTime = tempText.Tokenize(_T("\t"), nTokenPos);
		tempNumPult = tempText.Tokenize(_T("\t"), nTokenPos);
		tempNumLine = tempText.Tokenize(_T("\t"), nTokenPos);
		tempDescr = tempText.Tokenize(_T("\t"), nTokenPos);

		LinePrg* pLine = new LinePrg;
		pLine->NumPP = i;
		pLine->TimePusk = StringToTime(tempTime);
		pLine->NumPult = StrToInt(tempNumPult);
		pLine->NumLine = StrToNumLine(tempNumLine);
		pLine->Description = tempDescr;
		m_ListLine.AddTail(*pLine);

		delete pLine;			


		//StrToInt - �������� 0 ���� �� �����
		ref.Format(_T("%5d   %s   %5d   %5d   %s"),i,tempTime,StrToInt(tempNumPult),StrToNumLine(tempNumLine),tempDescr); 
		i++;
		m_ListPrg.AddString(ref);
	}
	//���� ��������� ���������, �� ������� ������� �������
	//� ������ � ����������� ��� ���������
	m_PrgLoaded = true;

	m_PrgFileName = _T("��������� - ") + m_PrgArray[m_CurrentPrg].m_FileTitle;
	m_btnPrg.SetColour(COLOR_BLACK,COLOR_GREEN);


	// ����������������� ��������� � TempTime
	m_TimePrg = StringToTime(tempTime);

	m_EndPrg.SetText(tempTime);
	m_intCurrentPosPrg = 0;
	MainTimer = 0;
	m_StopTime = 0;
	//�������� ������ ������ ������ ���������
	m_ListPrg.SetCurSel(m_intCurrentPosPrg);
	//������ ����� � ��������� - � ������
	m_posCurrentPosPrg = m_ListLine.GetHeadPosition();
	UpdateData(FALSE);
	//��������� ������
	//��������� ��� � ������
	m_btnBegin.EnableWindow(FALSE);

	if (m_CurrentPult >= 0)
	{
		m_btnWrite.EnableWindow(TRUE);
		m_btnCheck.EnableWindow(TRUE);
	}
	//������� ���� � ������ ������ � ��������
	m_btnWrite.ResetColour(true);
	m_btnCheck.ResetColour(true);

}
//��������� ��� �� ���������, ������� ���� ��������� � �����
else
{
	m_ListLine.RemoveAll();
	m_ListPrg.ResetContent();

	m_PrgLoaded = false;

	m_PrgFileName = _T("��������� �� ���������");
	//m_btnPrg.SetColour(COLOR_GREEN,COLOR_RED);
	m_btnPrg.ResetColour(true);
	m_EndPrg.SetText(_T("0:00:00.00"));
	m_TimePrg=0;
	UpdateData(FALSE);

	m_btnWrite.EnableWindow(FALSE);
	m_btnCheck.EnableWindow(FALSE);
}

//�������� ����������, ���� ����������
if (!m_PrgArray[m_CurrentPrg].m_FileSound.IsEmpty())
{
	USES_CONVERSION; //��� ����� ��������� �������� �� �����!
	LPCWSTR unitest = T2CW(LPCTSTR(m_PrgArray[m_CurrentPrg].m_FileSound));
	m_SoundLoaded = m_Player.Load(unitest);

	if (m_SoundLoaded)
	{
	
		m_btnSound.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_SoundFileName = _T("��������� - ") + m_PrgArray[m_CurrentPrg].m_FileSoundTitle;
		//�������� ������������ ���������� � 10 ��
		//__int64 1 sec = 10 000 000
		DWORD tmp;
		tmp = static_cast<DWORD>(m_Player.GetDuration()/100000);
		m_TimeSound = tmp;
		//���������� ������������ ����������
		m_PrgArray[m_CurrentPrg].m_TimeSound = m_TimeSound;
		m_EndSound.SetText(TimeToString(tmp));

		UpdateData(FALSE);
	}
	else
	{
		//m_btnSound.SetColour(COLOR_GREEN,COLOR_RED);
		m_btnSound.ResetColour(true);
		m_SoundFileName = "";
		m_Player.Cleanup();
		m_EndSound.SetText(TimeToString(0));
		UpdateData(FALSE);
	}
}
else
{
	//m_btnSound.SetColour(COLOR_GREEN,COLOR_RED);
	m_btnSound.ResetColour(true);
	m_SoundFileName = "";
	m_Player.Cleanup();
	m_EndSound.SetText(TimeToString(0));
	UpdateData(FALSE);
}

//��������������� ������ ���������
if (m_PrgArray[m_CurrentPrg].m_PrgInComp)
	{
		m_btnPrgInComp.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnPrgInPult.ResetColour(true);
		//�������� ��������� �� ������
		m_PrgInComp=TRUE;
		OnBnClickedBegin();
	}
else
	{
		m_btnPrgInPult.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnPrgInComp.ResetColour(true);
		m_PrgInComp=FALSE;
		OnBnClickedBegin();
	}

if (m_PrgArray[m_CurrentPrg].m_CompControl)
	{
		m_btnComp.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnStepReg.ResetColour(true);
		m_CompControl=true;
	}
else
	{
		m_btnStepReg.SetColour(COLOR_BLACK,COLOR_GREEN);
		m_btnComp.ResetColour(true);
		m_CompControl=false;
	}

//���� ����� ��������� ������ �� ���������, �� ���� �� ������
if (m_PrgArray[m_CurrentPrg].m_HandBtnValue != m_HandBtnValue)
	OnBnClickedHandBtn();
 
}

// ������������ �� ����� � �������� ������� � ������ ������� 
bool CMFCDlg::PultInList(BYTE SerNumPult)
{
	//���� ��� �������, �� �����
	if (m_CurrentPult == -1) return false;
	for (int i = 0; i < m_PultArray.GetCount(); i++)
		if (SerNumPult == m_PultArray[i].m_SerNumPult) return true;

	return false;
}
