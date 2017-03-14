#pragma once
class CPrg
{
public:
	CPrg(void);
	~CPrg(void);
	// Файл программы
	CString m_FilePrg;
	// Файл фонограммы
	CString m_FileSound;
	//Длительность программы
	DWORD m_TimePrg;
	//Длительность фонограммы
	DWORD m_TimeSound;
	// Наименование файла программы
	CString m_FileTitle;
	// Наименование файла фонограммы
	CString m_FileSoundTitle;
	// Программа прошита в пульте
	BOOL m_PrgInComp;
	// Ручное управление с компьютера
	bool m_CompControl;
	// Видимость клавиш подстрела
	BOOL m_HandBtnValue;
};

