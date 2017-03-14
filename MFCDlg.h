
// MFCDlg.h : файл заголовка
//

#pragma once
#include <list>
#include "afxwin.h"
#include "handbtn.h"
#include "LibMP3DLL.h"
#include "ColorButton.h"
#include "afxcmn.h"
#include "key.h"
#include "RS485.h"
#include "pult.h"
#include "cputicker.h"
#include "ColorListCtrl.h"
#include "DebugDlg.h"
#include "LineDlg.h"
#include "Tablo.h"
#include "MsgDlg.h"
#include "Report.h"
#include "DelayStart.h"
#include "Prg.h"


//Событие завершения программы
#define END_PROGRAM				WM_APP+1
//Событие таймера
#define TIMER_MAIN				WM_APP+2
//Таймер ожидания пуска с пульта
#define TIMER_WAIT_START		WM_APP+3
//Событие таймера фонового опроса пультов
#define TIMER_ECHO				WM_APP+4
//Событие паузы программы
#define	PAUSE_PROGRAM			WM_APP+5
//Событие автономного старта программы
#define	START_PROGRAM			WM_APP+6








typedef struct _LinePrg{
	int NumPP; //Порядковый номер
	DWORD TimePusk; //Время пуска в 10 мс интервалах
	BYTE NumPult; // Номер пульта
	BYTE NumLine; // Номер линии на запуск
	CString Description; //Описание изделия
} LinePrg, *pLinePrg;

// диалоговое окно CMFCDlg
class CMFCDlg : public CDialog
{
// Создание
public:
	CMFCDlg(CWnd* pParent = NULL);	// стандартный конструктор

	#define COLUMNS_m_ColListCtrl 5

	//критическая секция для обновления режимов работы пульта
	CCriticalSection cs;

	// Данные диалогового окна
	enum { IDD = IDD_MFC_DIALOG };

// Реализация
protected:
	HICON m_hIcon;
	//Отслеживание подключения - отключения RS485 
	HDEVNOTIFY NotificationHandle;
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT EventType,DWORD dwData);
	DECLARE_MESSAGE_MAP()

protected:
	// Таблица акселераторов для формы
	HACCEL hAccel;
	// Шрифт в листбоксе программы
	CFont m_FontPrg;
	LRESULT OnPowerChange(WPARAM, LPARAM);
private:
	//Проигрыватель фонограммы
	CLibMP3DLL m_Player;
	//Состояние проигрывания
	bool playing;
	//Состояние паузы 
	bool pause;
//**************************************************
//             КНОПКИ
//**************************************************
	// Кнопка тестового режима
	CColorButton m_btnTest;
	// Кнопка рабочего режима
	CColorButton m_btnWork;
	// Кнопка управления с компьютера
	CColorButton m_btnComp;
	// Кнопка управления с пульта
	CColorButton m_btnPult;
	// Кнопка старта программы
	CColorButton m_btnStart;
	// Кнопка остановки программы
	CColorButton m_btnStop;
	// Кнопка в начало программы
	CColorButton m_btnBegin;
	// Кнопка записи программы
	CColorButton m_btnWrite;
	// Кнопка проверки программы
	CColorButton m_btnCheck;
	// Кнопка преобразователя RS-485
	CColorButton m_btnRS485;
	// Кнопка выбора программы
	CColorButton m_btnPrg;
	// Кнопка выбора фонограммы
	CColorButton m_btnSound;
	// Кнопка программа в пульте
	CColorButton m_btnPrgInPult;
	// Кнопка программа с компьютера
	CColorButton m_btnPrgInComp;
	
//*******************************************
//    ПИТАНИЕ
//*******************************************
	// Показатель заряда батареи в процентах
	CProgressCtrl m_ProgressPower;
	// Питание от сети или батареи
	CString m_AccPower;
	// Текстовое поле процента заряда аккумулятора
	CString m_AccPercent;
	// Обновление показаний питания
	void RefreshPower(void);
public:
	// Список пультов
	CArray <CPult,CPult&> m_PultArray;

	// Список программ
	CArray <CPrg,CPrg&> m_PrgArray;




//**************************************************
//Флаги режима работы программы
//**************************************************
	// Флаг, программа поступает с компьютера
	BOOL m_PrgInComp;
	//Флаг паузы программы
	bool m_bPause;
	// Флаг загрузки фонограммы
	bool m_SoundLoaded;
	// Флаг загрузки программы
	bool m_PrgLoaded;
	// Флаг рабочего режима (клавиша РАБОТА зеленая и не мигает) 
	BOOL m_WorkReg;
	// Ручное управление с компьютера 
	bool m_CompControl;
	// Флаг мигания кнопки Работа
	bool m_BlinkWork;
	// Флаг того, что кнопка Работа выделена зеленым
	bool m_WorkGreen;





	// Флаг существования рабочего потока
	volatile BOOL m_WorkRunning;
	// Флаг существования 
	// потока ожидания старта
	volatile BOOL m_WaitStart;
	// потока поиска пультов
	volatile BOOL m_FindStart;

	// Флаг продолжения рабочего потока
	volatile BOOL fContinue;
	// Флаг продолжения потока ожидания старта программы 
	volatile BOOL fContinueWaitStart;


	//Список программы
	CListBox m_ListPrg;
	// Список обнаруженных пультов
	CColorListCtrl m_PultList;
	// Краткое имя программы
	CString m_PrgFileName;
	// Краткое имя файла фонограммы
	CString m_SoundFileName;
	afx_msg void OnDestroy();

	//Событие конца выполнения программы
	afx_msg LRESULT OnEndProgram(WPARAM wParam, LPARAM lParam);
	// Событие перехода на паузу
	afx_msg LRESULT OnPauseProgram(WPARAM wParam, LPARAM lParam);
	//Событие старта программы с пульта
	afx_msg LRESULT OnStartFromPult(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedLoadPrg();
	afx_msg void OnBnClickedLoadMp3();
	afx_msg void OnBnClickedTestReg();
	afx_msg void OnBnClickedWorkReg();
	afx_msg void OnBnClickedCompReg();
	afx_msg void OnBnClickedPultReg();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPreference();


	// Отправка групповой команды в пульт
	void SendGrpCmdToPult(DWORD dwData);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Преобразователь RS485
	CRS485* m_DevRS485;
	// Дочерний диалог ручного подстрела
	CKey* m_Key;
	// Кнопка подстрела
	CColorButton m_btnHand;
	afx_msg void OnBnClickedHandBtn();
	// Значение кнопки подстрела
	BOOL m_HandBtnValue;
	afx_msg void OnBnClickedStop();
	// Кнопка опроса подсоединенных пультов
	CColorButton m_btnFind;
	afx_msg void OnBnClickedFind();
	// Функция добавления пульта к списку
	void AddPult(BYTE SerNumPult);
	// Сигнальная строка выполнения 
	CString m_Protocol;

//*************************************************
//     ТАЙМЕРЫ 
//*************************************************
	// Высокоточный таймер рабочие замеры
	CCPUTicker m_TimerWork;


	//Основной таймер
	UINT m_Timer;
	// Таймер фонового опроса пультов
	UINT m_TimerEcho;
	// Таймер ожидания пуска программы
	UINT m_TimerWaitStart;


//*************************************************
//              ПОТОКИ
//*************************************************
	// Основной поток выполнения программы
	CWinThread* pWorkThread;
	// Поток ожидания старта в автономном режиме
	CWinThread* pWaitThread;
	// Поток опроса пультов
	CWinThread* pFindThread;

	// Массив строк программы
	CList <LinePrg,LinePrg> m_ListLine;
	// Текущая позиция в программе
	POSITION m_posCurrentPosPrg;
	// Положение текущей ячейки в программе
	int m_intCurrentPosPrg;
	// Флаг таймер программы запущен
	volatile BOOL m_TimerRun;
	// События запуска работы программы
	HANDLE m_hEventWork;
	//Значение таймера в момент старта в секундах
	volatile double m_StartTime;
	//Значение таймера в момент остановки
	volatile double m_StopTime;
	//Значение таймера ожидания пуска
	volatile DWORD m_WaitTime;
	

	// Текущий выбранный пульт
	int m_CurrentPult;
	// Номер выбранного пульта
	CString m_PultInfo;
	afx_msg void OnLvnItemchangedListPult(NMHDR *pNMHDR, LRESULT *pResult);
	// Команда на установку одной линии в пульте
	void SendSetLineToPult(BYTE NumPult, BYTE NumLine);
	afx_msg void OnBnClickedWrite();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedCheck();
	//Запись программы в пульт (информация, программа)
	BOOL WritePrgToPult(BYTE NumPult);
	// Запись линий программы в пульт
	BOOL WritePrg(BYTE NumPult);
	// Запись в пульт информации о программе 
	BOOL WriteInfo(BYTE NumPult);
	// Возвращает номер пульта по серийному номеру
	BYTE GetNumPult(BYTE SerNumPult);
	// Прогрессбар отображения длительных процессов 
	CProgressCtrl m_ProgressRpt;
//*********************************************************************
// Окно таймера в программе
//*********************************************************************
	// Статический элемент основного таймера
	CTablo m_MainTimer;
	// Обновление информации таймера
	void RefreshTimer(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// Состояние нажата, не нажата кнопка настройки
	BOOL m_PrefBtnValue;
	// Задержка выполнения программы
	CSliderCtrl m_Delay;
	// Текст задержки программы
	CString m_DelayTxt;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// Кнопка настройки
	CColorButton m_PrefBtn;
	afx_msg void OnBnClickedButtonDebug();
	CDebugDlg* m_dlgDebug;
	CLineDlg* m_LineDlg;
	// Проверка пульта
	BOOL CheckPrg(BYTE NumPult);
	// Перезапись программы из буфера во внутреннюю ППЗУ
	BOOL WritePrgToFlash(BYTE SerNumPult);
	// Подсчет количества линий в программе для пульта
	int CountLineInPult(BYTE NumPult);
	// Панель времени до конца программы
	CTablo m_EndPrg;
	// Панель времени до окончания фонограммы
	CTablo m_EndSound;
	//Длительность программы
	DWORD m_TimePrg;
	//Длительность фонограммы
	DWORD m_TimeSound;
	// Проверка линий пульта
	void ScanPult(int NumPult);
	// Проверка одной линии пульта
	void ScanPult(int NumPult, BYTE NumLine);
	virtual void OnOK();
	virtual void OnCancel();
	// Поиск дулей номеров пультов
	BOOL FindDouble(void);
	// Изменение номера текущего пульта
	BOOL ChangeNumPult(BYTE NewNumPult);
	// Диалог сообщений
	CMsgDlg* m_MsgDlg;
	//Диалог отложенного запуска
	CDelayStart* m_DelayStart;
	// Установка длительности импульса запуска
	BOOL SetPulse(int wPulse);
	// Опрос связи с пультом
	void CheckPult(void);
	// Включение, отключение кнопок 
	void EnableBtn(BOOL EN_DIS);
	afx_msg void OnBnClickedPrgInPult();
	afx_msg void OnBnClickedPrgInComp();
	// Подсчет количества выстрелов на линии
	int CountLineInPult(BYTE NumPult, BYTE NumLine);
	// Файл рапорта об ошибках программы
	CReport* m_log;
	// Выключить линию в пульте
	void SendClearLineToPult(BYTE NumPult, BYTE NumLine);
	// Запрос длительности импульса текущего пульта
	int GetPulse();
	// Присутствует ли пульт в списке пультов
	bool PultIsPresent(int NumPult);
	// Проверка наличия пульта, присутствующего в программе
	CString CheckPultInPrg();
	// Нажатие на кнопку Работа или F1
	CColorButton m_btnWorkF1;
	afx_msg void OnBnClickedWork();
	// Кнопка ручного шагового режима
	CColorButton m_btnStepReg;
	afx_msg void OnBnClickedStepReg();
	// Безопасный опрос пульта
	bool SafeEcho(int NumPultInList);
	// Перевод в остановку всех пультов с подтверждением
	int PauseAllPult(void);
	// Запуск формы ожидания запуска с пульта
	void ShowWaitStart(void);
	// Флаг ожидания запуска по системным часам
	bool m_WaitFromClock;
	// Время старта по компьютеру
	COleDateTime m_TimeStart;

	// Выбрана первая программа
	CColorButton m_Prg1;
	// Выбрана вторая программа
	CColorButton m_Prg2;
	// Выбрана 3 программа
	CColorButton m_Prg3;
	CColorButton m_Prg4;
	CColorButton m_Prg5;
	CColorButton m_Prg6;
	CColorButton m_Prg7;
	CColorButton m_Prg8;
	// Текущая программа
	int m_CurrentPrg;
	afx_msg void OnBnClickedPrg(UINT nID);
	// Присутствует ли пульт с серийным номером в списке пультов 
	bool PultInList(BYTE SerNumPult);
	// Картинка рабочего режима работы 
	CStatic m_imgWork;
};
