// Key.cpp : implementation file
//

#include "stdafx.h"
#include "MFC.h"
#include "Key.h"
#include "afxdialogex.h"
#include "PultConst.h"
#include "MFCDlg.h"



// CKey dialog
IMPLEMENT_DYNAMIC(CKey, CDialog)

CKey::CKey(CWnd* pParent /*=NULL*/)
	: CDialog(CKey::IDD, pParent)
	, m_Group(FALSE)
	, m_SaveGrp1(0)
	, m_SaveGrp2(0)
	, m_SaveGrp3(0)
{
	m_pParent = (CMFCDlg*) pParent;
}


CKey::~CKey()
{
}

void CKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEY1, m_key1);
	DDX_Control(pDX, IDC_KEY2, m_key2);
	DDX_Control(pDX, IDC_KEY3, m_key3);
	DDX_Control(pDX, IDC_KEY4, m_key4);
	DDX_Control(pDX, IDC_KEY5, m_key5);
	DDX_Control(pDX, IDC_KEY6, m_key6);
	DDX_Control(pDX, IDC_KEY7, m_key7);
	DDX_Control(pDX, IDC_KEY8, m_key8);
	DDX_Control(pDX, IDC_KEY9, m_key9);
	DDX_Control(pDX, IDC_KEY10, m_key10);
	DDX_Control(pDX, IDC_KEY11, m_key11);
	DDX_Control(pDX, IDC_KEY12, m_key12);
	DDX_Control(pDX, IDC_KEY13, m_key13);
	DDX_Control(pDX, IDC_KEY14, m_key14);
	DDX_Control(pDX, IDC_KEY15, m_key15);
	DDX_Control(pDX, IDC_KEY16, m_key16);
	DDX_Control(pDX, IDC_KEY17, m_key17);
	DDX_Control(pDX, IDC_KEY18, m_key18);
	DDX_Control(pDX, IDC_KEY19, m_key19);
	DDX_Control(pDX, IDC_KEY20, m_key20);
	DDX_Control(pDX, IDC_KEY21, m_key21);
	DDX_Control(pDX, IDC_KEY22, m_key22);
	DDX_Control(pDX, IDC_KEY23, m_key23);
	DDX_Control(pDX, IDC_KEY24, m_key24);
	DDX_Control(pDX, IDC_KEY25, m_key25);
	DDX_Control(pDX, IDC_KEY26, m_key26);
	DDX_Control(pDX, IDC_KEY27, m_key27);
	DDX_Control(pDX, IDC_KEY28, m_key28);
	DDX_Control(pDX, IDC_KEY29, m_key29);
	DDX_Control(pDX, IDC_KEY30, m_key30);
	DDX_Control(pDX, IDC_KEY31, m_key31);
	DDX_Control(pDX, IDC_KEY32, m_key32);
	DDX_Control(pDX, IDC_KEY33, m_key33);
	DDX_Control(pDX, IDC_KEY34, m_key34);
	DDX_Control(pDX, IDC_KEY35, m_key35);
	DDX_Control(pDX, IDC_KEY36, m_key36);
	DDX_Control(pDX, IDC_KEY37, m_key37);
	DDX_Control(pDX, IDC_KEY38, m_key38);
	DDX_Control(pDX, IDC_KEY39, m_key39);
	DDX_Control(pDX, IDC_KEY40, m_key40);
	DDX_Check(pDX, IDC_GROUP, m_Group);
	DDX_Control(pDX, IDC_GROUP_PUSK, m_GroupPusk);
	DDX_Control(pDX, IDC_GROUP, m_btnGroup);
	DDX_Control(pDX, IDC_GRP_SAVE1, m_stGrp1);
	DDX_Control(pDX, IDC_GRP_SAVE2, m_stGrp2);
	DDX_Control(pDX, IDC_GRP_SAVE3, m_stGrp3);
}


BEGIN_MESSAGE_MAP(CKey, CDialog)
	ON_BN_CLICKED(IDC_RESET_GROUP, &CKey::OnBnClickedResetGroup)
	ON_BN_CLICKED(IDC_GROUP, &CKey::OnBnClickedGroup)
	ON_BN_CLICKED(IDC_GROUP_PUSK, &CKey::OnBnClickedGroupPusk)
	ON_BN_CLICKED(IDC_STEP, &CKey::OnBnClickedStep)
	ON_BN_CLICKED(IDC_GRP_SAVE1, &CKey::OnBnClickedGrpSave1)
	ON_BN_CLICKED(IDC_GRP_SAVE2, &CKey::OnBnClickedGrpSave2)
	ON_BN_CLICKED(IDC_GRP_SAVE3, &CKey::OnBnClickedGrpSave3)
END_MESSAGE_MAP()


// CKey message handlers


BOOL CKey::OnInitDialog()
{
	CDialog::OnInitDialog();
    //1 ряд
	m_key1.SetNumBtn(1);
	m_key1.SetNameBtn("1");
	m_key1.m_Key = VK_KEY_1;

	m_key2.SetNumBtn(2);
	m_key2.SetNameBtn("2");
	m_key2.m_Key = VK_KEY_2;

	m_key3.SetNumBtn(3);
	m_key3.SetNameBtn("3");
	m_key3.m_Key = VK_KEY_3;

	m_key4.SetNumBtn(4);
	m_key4.SetNameBtn("4");
	m_key4.m_Key = VK_KEY_4;

	m_key5.SetNumBtn(5);
	m_key5.SetNameBtn("5");
	m_key5.m_Key = VK_KEY_5;

	m_key6.SetNumBtn(6);
	m_key6.SetNameBtn("6");
	m_key6.m_Key = VK_KEY_6;

	m_key7.SetNumBtn(7);
	m_key7.SetNameBtn("7");
	m_key7.m_Key = VK_KEY_7;

	m_key8.SetNumBtn(8);
	m_key8.SetNameBtn("8");
	m_key8.m_Key = VK_KEY_8;

	m_key9.SetNumBtn(9);
	m_key9.SetNameBtn("9");
	m_key9.m_Key = VK_KEY_9;

	m_key10.SetNumBtn(10);
	m_key10.SetNameBtn("0");
	m_key10.m_Key = VK_KEY_0;

	//2 ряд
	m_key11.SetNumBtn(11);
	m_key11.SetNameBtn("Q");
	m_key11.m_Key = VK_KEY_Q;

	m_key12.SetNumBtn(12);
	m_key12.SetNameBtn("W");
	m_key12.m_Key = VK_KEY_W;

	m_key13.SetNumBtn(13);
	m_key13.SetNameBtn("E");
	m_key13.m_Key = VK_KEY_E;

	m_key14.SetNumBtn(14);
	m_key14.SetNameBtn("R");
	m_key14.m_Key = VK_KEY_R;

	m_key15.SetNumBtn(15);
	m_key15.SetNameBtn("T");
	m_key15.m_Key = VK_KEY_T;

	m_key16.SetNumBtn(16);
	m_key16.SetNameBtn("Y");
	m_key16.m_Key = VK_KEY_Y;

	m_key17.SetNumBtn(17);
	m_key17.SetNameBtn("U");
	m_key17.m_Key = VK_KEY_U;

	m_key18.SetNumBtn(18);
	m_key18.SetNameBtn("I");
	m_key18.m_Key = VK_KEY_I;

	m_key19.SetNumBtn(19);
	m_key19.SetNameBtn("O");
	m_key19.m_Key = VK_KEY_O;

	m_key20.SetNumBtn(20);
	m_key20.SetNameBtn("P");
	m_key20.m_Key = VK_KEY_P;

	//3 ряд
	m_key21.SetNumBtn(21);
	m_key21.SetNameBtn("A");
	m_key21.m_Key = VK_KEY_A;

	m_key22.SetNumBtn(22);
	m_key22.SetNameBtn("S");
	m_key22.m_Key = VK_KEY_S;

	m_key23.SetNumBtn(23);
	m_key23.SetNameBtn("D");
	m_key23.m_Key = VK_KEY_D;

	m_key24.SetNumBtn(24);
	m_key24.SetNameBtn("F");
	m_key24.m_Key = VK_KEY_F;

	m_key25.SetNumBtn(25);
	m_key25.SetNameBtn("G");
	m_key25.m_Key = VK_KEY_G;

	m_key26.SetNumBtn(26);
	m_key26.SetNameBtn("H");
	m_key26.m_Key = VK_KEY_H;

	m_key27.SetNumBtn(27);
	m_key27.SetNameBtn("J");
	m_key27.m_Key = VK_KEY_J;

	m_key28.SetNumBtn(28);
	m_key28.SetNameBtn("K");
	m_key28.m_Key = VK_KEY_K;

	m_key29.SetNumBtn(29);
	m_key29.SetNameBtn("L");
	m_key29.m_Key = VK_KEY_L;

	m_key30.SetNumBtn(30);
	m_key30.SetNameBtn(";");
	m_key30.m_Key = VK_OEM_1;

	//1 ряд
	m_key31.SetNumBtn(31);
	m_key31.SetNameBtn("Z");
	m_key31.m_Key = VK_KEY_Z;

	m_key32.SetNumBtn(32);
	m_key32.SetNameBtn("X");
	m_key32.m_Key = VK_KEY_X;

	m_key33.SetNumBtn(33);
	m_key33.SetNameBtn("C");
	m_key33.m_Key = VK_KEY_C;

	m_key34.SetNumBtn(34);
	m_key34.SetNameBtn("V");
	m_key34.m_Key = VK_KEY_V;

	m_key35.SetNumBtn(35);
	m_key35.SetNameBtn("B");
	m_key35.m_Key = VK_KEY_B;

	m_key36.SetNumBtn(36);
	m_key36.SetNameBtn("N");
	m_key36.m_Key = VK_KEY_N;

	m_key37.SetNumBtn(37);
	m_key37.SetNameBtn("M");
	m_key37.m_Key = VK_KEY_M;

	m_key38.SetNumBtn(38);
	m_key38.SetNameBtn(",");
	m_key38.m_Key = VK_OEM_COMMA;

	m_key39.SetNumBtn(39);
	m_key39.SetNameBtn(".");
	m_key39.m_Key = VK_OEM_PERIOD;

	m_key40.SetNumBtn(40);
	m_key40.SetNameBtn("/");
	m_key40.m_Key = VK_OEM_2;


	CHandBtn* ci;
	// Отмечаем кнопки моторов
	// Переводим кнопки с 33 по 40 в режим мотора
		for (int x=IDC_KEY33;x<(IDC_KEY40+1);x++)
		{
			ci=(CHandBtn*)GetDlgItem(x);
			ci->m_IsMotor=TRUE;
			ci->SetResultWndBkClr(COLOR_MOTOR_NUM);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CKey::OnBnClickedGroup()
{
	CHandBtn* ci;
	// Переменные с формы (задержка отоло 0.5 сек)
	UpdateData(TRUE);
	if (m_Group == TRUE)
	{
		//Переводим первые 32 кнопки в режим группировки
		for (int x=IDC_KEY1;x<(IDC_KEY32+1);x++)
		{
			ci=(CHandBtn*)GetDlgItem(x);
			ci->m_State=TRUE;
		}

	} 
	else
	{
		//Снимаем режим группировки с кнопок
		for (int x=IDC_KEY1;x<(IDC_KEY32+1);x++)
		{
			ci=(CHandBtn*)GetDlgItem(x);
			ci->m_State=FALSE;
			ci->m_Checked=FALSE;
			ci->ResetColorBtn();
		}
	}

	//Фокус на диалог для избежания beep
	m_pParent->m_ListPrg.SetFocus();
}


void CKey::OnBnClickedResetGroup()
{
	CHandBtn* ci;
	for (int x=IDC_KEY1;x<(IDC_KEY40+1);x++)
	{
		ci=(CHandBtn*)GetDlgItem(x);
		ci->ResetClickCount();
	}

	m_pParent->m_ListPrg.SetFocus();
}


void CKey::OnBnClickedGroupPusk()
{
//Индикатор установленных в группу линий
DWORD tempLine = 0, GroupLine = 0; 

//Снятие и разотметка 32 кнопок,
//Увеличение счетчика числа нажатий отмеченных кнопок
//Подготовка 4-х байтов группового пуска
//Отправка команды в преобразователь
	CHandBtn* ci;
	for (int x=IDC_KEY1;x<(IDC_KEY32+1);x++)
	{
		ci=(CHandBtn*)GetDlgItem(x);
		ci->m_State=FALSE;
		if (ci->m_Checked)
		{
			//Разотметили кнопку
			ci->m_Checked=FALSE;
			//Увеличили счетчик нажатий
			ci->IncButton();
			//Установили бит соответствующий номеру нажатой кнопки
			tempLine = 1<<(x - IDC_KEY1);
			//Собрали 4 байта с установленными битами
			GroupLine = GroupLine | tempLine;
			tempLine = 0;
		}
		ci->ResetColorBtn();
	}
	m_pParent->SendGrpCmdToPult(GroupLine);
	//Разотметили кнопку группировки
	m_Group = FALSE;
	UpdateData(FALSE);


	m_pParent->m_ListPrg.SetFocus();

}


BOOL CKey::PreTranslateMessage(MSG* pMsg)
{
	if((pMsg->message == WM_KEYUP) || (pMsg->message == WM_KEYDOWN))
	{
		m_pParent->m_ListPrg.SetFocus();	

	}
	return CWnd::PreTranslateMessage(pMsg);
}


void CKey::OnBnClickedStep()
{
	LinePrg* pLine;

	if (m_pParent->m_posCurrentPosPrg != NULL)
	{
		//Выборка текущей строки программы
		pLine = &m_pParent->m_ListLine.GetAt(m_pParent->m_posCurrentPosPrg);
	
		if (m_pParent->m_WorkReg)
			//Запуск линии
			m_pParent->SendSetLineToPult(pLine->NumPult,pLine->NumLine); 
		
		//Переход к следующей линии
		m_pParent->m_intCurrentPosPrg++;
		POSITION& refPos =  m_pParent->m_posCurrentPosPrg;
		pLine = &m_pParent->m_ListLine.GetNext(refPos);
		//Установка выделения текущей линии
		m_pParent->m_ListPrg.SetCurSel(m_pParent->m_intCurrentPosPrg);
		if ((m_pParent->m_intCurrentPosPrg - m_pParent->m_ListPrg.GetTopIndex())>5)
		{
			m_pParent->m_ListPrg.SetTopIndex(m_pParent->m_intCurrentPosPrg-5);
		} 
	
		
	}
}


void CKey::OnBnClickedGrpSave1()
{
	OnBnClickedGrpSave(1);
}



void CKey::OnBnClickedGrpSave(int NumGrp)
{
	//Индикатор установленных в группу линий
	DWORD tempLine = 0, GroupLine = 0, SaveGrp = 0; 
	
	CHandBtn* ci;

	m_pParent->m_ListPrg.SetFocus();


	//Если включена группировка, то запоминаем значение кнопок группировки
	if (m_Group)
	{
		//Сохранение 4-х байтов группового пуска
		for (int x=IDC_KEY1;x<(IDC_KEY32+1);x++)
		{
			ci=(CHandBtn*)GetDlgItem(x);
			if (ci->m_Checked)
			{
				//Установили бит соответствующий номеру нажатой кнопки
				tempLine = 1<<(x - IDC_KEY1);
				//Собрали 4 байта с установленными битами
				GroupLine = GroupLine | tempLine;
				tempLine = 0;
			}

		}

		switch(NumGrp)
		{
		case 1:
			m_SaveGrp1 = GroupLine;
			break;
		case 2:
			m_SaveGrp2 = GroupLine;
			break;
		case 3:
			m_SaveGrp3 = GroupLine;
			break;
		}
	}

	// Кнопка группировки не нажата
	// Нажимаем кнопку группировки, отмечаем сохраненное состояние
	else
	{
		m_btnGroup.SetCheck(1);
		OnBnClickedGroup();

		switch(NumGrp)
		{
		case 1:
			SaveGrp = m_SaveGrp1;
			break;
		case 2:
			SaveGrp = m_SaveGrp2;
			break;
		case 3:
			SaveGrp = m_SaveGrp3;
			break;
		}

		//Восстановление сохраненных отмеченных кнопок
		for (int x=IDC_KEY1;x<(IDC_KEY32+1);x++)
		{
			ci=(CHandBtn*)GetDlgItem(x);
			//Установили бит соответствующий номеру нажатой кнопки
			tempLine = 1<<(x - IDC_KEY1);

			if ((tempLine & SaveGrp) != 0)
			{
				ci->SetColorBtn(COLOR_CHECK_TXT,COLOR_CHECK_BGR);
				ci->m_Checked = TRUE;
			}
			tempLine = 0;
		}
	}
}
//
void CKey::OnBnClickedGrpSave2()
{
	OnBnClickedGrpSave(2);
}
//
void CKey::OnBnClickedGrpSave3()
{
	OnBnClickedGrpSave(3);
}
