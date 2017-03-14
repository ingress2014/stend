
// MFC.cpp : Определяет поведение классов для приложения.
//

#include "stdafx.h"
#include "MFC.h"
#include "MFCDlg.h"
#include "SplashWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApp

BEGIN_MESSAGE_MAP(CMFCApp, CWinApp)
//	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// создание CMFCApp

CMFCApp::CMFCApp()
{
	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
	
	//Создаем мютекс для запуска одного экземпляра приложения 
	mutex = new CMutex(FALSE,"FirstStep",NULL);
	si=new CSingleLock(mutex); 
}


// Единственный объект CMFCApp

CMFCApp theApp;


// инициализация CMFCApp

BOOL CMFCApp::InitInstance()
{
	if (!si->Lock(1))
	{
		AfxMessageBox(_T("Приложение уже запущено")); 
		return FALSE;
	}

	CCPUTicker tick1;

	//Проверка поддержки высокоточного таймера
	if (!tick1.IsAvailable())
	{
		AfxMessageBox(_T("Высокоточный таймер не поддерживается данным процессором"));
		return FALSE;
	}


	// InitCommonControlsEx() требуется для Windows XP, если манифест
	// приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
	// стилей отображения. В противном случае будет возникать сбой при создании любого окна.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
	// в вашем приложении.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

 	
	//Появление окна предзагрузки
	CSplashWindow::ShowSplashScreen( NULL, "Загрузка программы...");
	Sleep(500);
	CSplashWindow::ShowSplashScreen( NULL, "Калибровка таймера...");
	// Калибровка высокоточного таймера
	tick1.Measure();
	double temp = tick1.GetTickCountAsSeconds();
	CSplashWindow::ShowSplashScreen( NULL, "Программа загружена");




	// Создать диспетчер оболочки, в случае, если диалоговое окно содержит
	// представление дерева оболочки или какие-либо его элементы управления.
	CShellManager *pShellManager = new CShellManager;

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующих
	// конкретных процедур инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));

	//ИНИЦИАЛИЗАЦИЯ COM системы!!!!!!!!!!!!! 
	::CoInitialize(NULL);


	CMFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "ОК"
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Введите код для обработки закрытия диалогового окна
		//  с помощью кнопки "Отмена"
	}

////ДЕИНИЦИАЛИЗАЦИЯ COM системы!!!!!!!!!!!!!
	::CoUninitialize();


	// Удалить диспетчер оболочки, созданный выше.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Поскольку диалоговое окно закрыто, возвратите значение FALSE, чтобы можно было выйти из
	//  приложения вместо запуска генератора сообщений приложения.
	return FALSE;
}



int CMFCApp::ExitInstance()
{
 //Уничтожаем мютекс запуска приложения
	delete si;
	delete mutex;
	return CWinApp::ExitInstance();
}


BOOL CMFCApp::PreTranslateMessage(MSG* pMsg)
{
	if (CSplashWindow::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
