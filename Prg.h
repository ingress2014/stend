#pragma once
class CPrg
{
public:
	CPrg(void);
	~CPrg(void);
	// ���� ���������
	CString m_FilePrg;
	// ���� ����������
	CString m_FileSound;
	//������������ ���������
	DWORD m_TimePrg;
	//������������ ����������
	DWORD m_TimeSound;
	// ������������ ����� ���������
	CString m_FileTitle;
	// ������������ ����� ����������
	CString m_FileSoundTitle;
	// ��������� ������� � ������
	BOOL m_PrgInComp;
	// ������ ���������� � ����������
	bool m_CompControl;
	// ��������� ������ ���������
	BOOL m_HandBtnValue;
};

