
// MFC.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CMFCApp:
// � ���������� ������� ������ ��. MFC.cpp
//

class CMFCApp : public CWinApp
{
public:
	CMFCApp();
	CMutex* mutex;
	CSingleLock* si; 

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CMFCApp theApp;