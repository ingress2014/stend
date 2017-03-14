#include "StdAfx.h"
#include "Report.h"


CReport::CReport(void)
	: m_FileName(_T(""))
{
	SYSTEMTIME st;
	CString strFolder, temp;
	TCHAR buff[MAX_PATH];
	CFileException FileExc;

	m_Rpt = new CStringList();
	//���������� ������� ����������
	memset(buff, 0, MAX_PATH);
	//� buff ������ ���� � ������ ���������� ���������
	::GetModuleFileName(NULL,buff,sizeof(buff));    
	strFolder = buff;
	//� strFolder ������ ���� � ����������� ������
	strFolder = strFolder.Left(strFolder.ReverseFind(_T('\\'))+1);
	//�������� ��������� �����
	GetLocalTime(&st);
	//��������� ��� ����� � ����������� �� ������� ��������
	temp.Format(_T("%02d.%02d.%4d_%02d_%02d.log"),st.wDay,st.wMonth,st.wYear,st.wHour,st.wMinute); 
	m_FileName = strFolder + _T("logs\\") + temp;

	if (!m_logFile.Open(m_FileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText,	&FileExc))
			FileExc.ReportError();
	
}


CReport::~CReport(void)
{
	delete m_Rpt;
	m_logFile.Close();
}


// ���������� � ��� ���������
void CReport::Add(CString str)
{
SYSTEMTIME st;
CString temp;

//�������� ��������� �����
GetLocalTime(&st);
//��������� ����� ������
temp.Format(_T("%02d:%02d:%02d.%03d - "),st.wHour,st.wMinute,st.wSecond,st.wMilliseconds); 
m_logFile.WriteString(temp + str + "\n");
}
