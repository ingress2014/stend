#pragma once
#include "afxcoll.h"
class CReport
{
public:
	CReport(void);
	~CReport(void);
	// Список строк протокола работы
	CStringList* m_Rpt;
	// Наименование файла протокола
	CString m_FileName;
	//Файл логов
	CStdioFile m_logFile;
	// Добавление в лог сообщения
	void Add(CString str);
};

