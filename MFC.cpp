
// MFC.cpp : ���������� ��������� ������� ��� ����������.
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


// �������� CMFCApp

CMFCApp::CMFCApp()
{
	// TODO: �������� ��� ��������,
	// ��������� ���� ������ ��� ������������� � InitInstance
	
	//������� ������ ��� ������� ������ ���������� ���������� 
	mutex = new CMutex(FALSE,"FirstStep",NULL);
	si=new CSingleLock(mutex); 
}


// ������������ ������ CMFCApp

CMFCApp theApp;


// ������������� CMFCApp

BOOL CMFCApp::InitInstance()
{
	if (!si->Lock(1))
	{
		AfxMessageBox(_T("���������� ��� ��������")); 
		return FALSE;
	}

	CCPUTicker tick1;

	//�������� ��������� ������������� �������
	if (!tick1.IsAvailable())
	{
		AfxMessageBox(_T("������������ ������ �� �������������� ������ �����������"));
		return FALSE;
	}


	// InitCommonControlsEx() ��������� ��� Windows XP, ���� ��������
	// ���������� ���������� ComCtl32.dll ������ 6 ��� ����� ������� ������ ��� ���������
	// ������ �����������. � ��������� ������ ����� ��������� ���� ��� �������� ������ ����.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �������� ���� �������� ��� ��������� ���� ����� ������� ����������, ������� ���������� ������������
	// � ����� ����������.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

 	
	//��������� ���� ������������
	CSplashWindow::ShowSplashScreen( NULL, "�������� ���������...");
	Sleep(500);
	CSplashWindow::ShowSplashScreen( NULL, "���������� �������...");
	// ���������� ������������� �������
	tick1.Measure();
	double temp = tick1.GetTickCountAsSeconds();
	CSplashWindow::ShowSplashScreen( NULL, "��������� ���������");




	// ������� ��������� ��������, � ������, ���� ���������� ���� ��������
	// ������������� ������ �������� ��� �����-���� ��� �������� ����������.
	CShellManager *pShellManager = new CShellManager;

	// ����������� �������������
	// ���� ��� ����������� �� ������������ � ���������� ��������� ������
	// ��������� ������������ �����, ���������� ������� �� ���������
	// ���������� �������� �������������, ������� �� ���������
	// �������� ������ �������, � ������� �������� ���������
	// TODO: ������� �������� ��� ������ �� ���-������ ����������,
	// �������� �� �������� �����������
	SetRegistryKey(_T("��������� ����������, ��������� � ������� ������� ����������"));

	//������������� COM �������!!!!!!!!!!!!! 
	::CoInitialize(NULL);


	CMFCDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "��"
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ������� ��� ��� ��������� �������� ����������� ����
		//  � ������� ������ "������"
	}

////��������������� COM �������!!!!!!!!!!!!!
	::CoUninitialize();


	// ������� ��������� ��������, ��������� ����.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ��������� ���������� ���� �������, ���������� �������� FALSE, ����� ����� ���� ����� ��
	//  ���������� ������ ������� ���������� ��������� ����������.
	return FALSE;
}



int CMFCApp::ExitInstance()
{
 //���������� ������ ������� ����������
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
