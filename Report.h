#pragma once
#include "afxcoll.h"
class CReport
{
public:
	CReport(void);
	~CReport(void);
	// ������ ����� ��������� ������
	CStringList* m_Rpt;
	// ������������ ����� ���������
	CString m_FileName;
	//���� �����
	CStdioFile m_logFile;
	// ���������� � ��� ���������
	void Add(CString str);
};

