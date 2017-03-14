
// MFC.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMFCApp:
// О реализации данного класса см. MFC.cpp
//

class CMFCApp : public CWinApp
{
public:
	CMFCApp();
	CMutex* mutex;
	CSingleLock* si; 

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CMFCApp theApp;