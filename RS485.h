#pragma once

#include "ftd2xx.h"

#define TX_BUFFER_SIZE	14 //128
#define RX_BUFFER_SIZE	15 //384

#define TIMEOUT_READ 10
#define TIMEOUT_WRITE 10


#define TEXTSIZE 256

class CRS485
{
public:
	CRS485(void);
	~CRS485(void);
	// Количество подключенных пультов
	int m_CountPult;
	// Возращает количество преобразователей 
	int GetNumDev(void);
	// Буфер передачи преобразователя
	BYTE* m_TxBuffer;
	// Буфер приема преобразователя
	BYTE* m_RxBuffer;

public:
	// Хендл преобразователя RS485
	FT_HANDLE m_hRS485;
	// Количество преобразователей
	int m_CountRS485;
	// Номер преобразователя RS485
	int m_NumRS485;
	//Флаги открытия порта
	DWORD dwListDescFlags;
	//Функция открытия порта
	BOOL OpenPort(void);
	//Закрытие порта 
	BOOL ClosePort(void);
	// Функция присутствия проинициализированного преобразователя
	BOOL IsOpen(void) const
	{
		return m_hRS485 != NULL;
	}

	//Модуль DLL ftd2xx.dll
	HMODULE m_hmodule;

	// Загрузка DLL
	void LoadDLL(void);


	typedef FT_STATUS (WINAPI *PtrToOpen)(PVOID, FT_HANDLE *); 
	PtrToOpen m_pOpen; 
	FT_STATUS Open(PVOID);

	typedef FT_STATUS (WINAPI *PtrToOpenEx)(PVOID, DWORD, FT_HANDLE *); 
	PtrToOpenEx m_pOpenEx; 

	typedef FT_STATUS (WINAPI *PtrToListDevices)(PVOID, PVOID, DWORD);
	PtrToListDevices m_pListDevices; 

	typedef FT_STATUS (WINAPI *PtrToClose)(FT_HANDLE);
	PtrToClose m_pClose;
	FT_STATUS Close();

	typedef FT_STATUS (WINAPI *PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToRead m_pRead;
	FT_STATUS Read(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
	PtrToWrite m_pWrite;
	FT_STATUS Write(LPVOID, DWORD, LPDWORD);

	typedef FT_STATUS (WINAPI *PtrToResetDevice)(FT_HANDLE);
	PtrToResetDevice m_pResetDevice;
	FT_STATUS ResetDevice();

	typedef FT_STATUS (WINAPI *PtrToPurge)(FT_HANDLE, ULONG);
	PtrToPurge m_pPurge;
	FT_STATUS Purge(ULONG);

	typedef FT_STATUS (WINAPI *PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
	PtrToSetTimeouts m_pSetTimeouts;
	FT_STATUS SetTimeouts(ULONG, ULONG);

	typedef FT_STATUS (WINAPI *PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
	PtrToGetQueueStatus m_pGetQueueStatus;

	typedef FT_STATUS (WINAPI *PtrToSetDtr)(FT_HANDLE);
	PtrToSetDtr m_pSetDtr;

	typedef FT_STATUS (WINAPI *PtrToClrDtr)(FT_HANDLE);
	PtrToClrDtr m_pClrDtr;

	typedef FT_STATUS (WINAPI *PtrToSetRts)(FT_HANDLE);
	PtrToSetRts m_pSetRts;

	typedef FT_STATUS (WINAPI *PtrToClrRts)(FT_HANDLE);
	PtrToClrRts m_pClrRts;

	typedef FT_STATUS (WINAPI *PtrToGetModemStatus)(FT_HANDLE, LPDWORD);
	PtrToGetModemStatus m_pGetModemStatus;

	typedef FT_STATUS (WINAPI *PtrToSetChars)(FT_HANDLE, UCHAR, UCHAR, UCHAR, UCHAR);
	PtrToSetChars m_pSetChars;

	typedef FT_STATUS (WINAPI *PtrToSetLatencyTimer)(FT_HANDLE, UCHAR);
	PtrToSetLatencyTimer m_pSetLatencyTimer;

	typedef FT_STATUS (WINAPI *PtrToGetLatencyTimer)(FT_HANDLE, PUCHAR);
	PtrToGetLatencyTimer m_pGetLatencyTimer;

	typedef FT_STATUS (WINAPI *PtrToSetDataCharacteristics)(FT_HANDLE, UCHAR, UCHAR, UCHAR);
	PtrToSetDataCharacteristics m_pSetDataCharacteristics;
	FT_STATUS SetDataCharacteristics(UCHAR, UCHAR, UCHAR);

	typedef FT_STATUS (WINAPI *PtrToSetFlowControl)(FT_HANDLE, USHORT, UCHAR, UCHAR);
	PtrToSetFlowControl m_pSetFlowControl;
	FT_STATUS SetFlowControl(USHORT, UCHAR, UCHAR);

	typedef FT_STATUS (WINAPI *PtrToSetBreakOn)(FT_HANDLE);
	PtrToSetBreakOn m_pSetBreakOn;

	typedef FT_STATUS (WINAPI *PtrToSetBreakOff)(FT_HANDLE);
	PtrToSetBreakOff m_pSetBreakOff;

	typedef FT_STATUS (WINAPI *PtrToResetPort)(FT_HANDLE);
	PtrToResetPort m_pResetPort;

	typedef FT_STATUS (WINAPI *PtrToGetDeviceInfo)(FT_HANDLE, FT_DEVICE *, LPDWORD, PCHAR, PCHAR, LPVOID);
	PtrToGetDeviceInfo m_pGetDeviceInfo;

	typedef FT_STATUS (WINAPI *PtrToEE_UASize)(FT_HANDLE, LPDWORD);
	PtrToEE_UASize m_pEE_UASize;

	typedef FT_STATUS (WINAPI *PtrToEE_UARead)(FT_HANDLE, PUCHAR, DWORD, LPDWORD);
	PtrToEE_UARead m_pEE_UARead;

	typedef FT_STATUS (WINAPI *PtrToEE_UAWrite)(FT_HANDLE, PUCHAR, DWORD);
	PtrToEE_UAWrite m_pEE_UAWrite;

	typedef FT_STATUS (WINAPI *PtrToSetBitMode)(FT_HANDLE, UCHAR, UCHAR);
	PtrToSetBitMode m_pSetBitMode;

	typedef FT_STATUS (WINAPI *PtrToGetBitMode)(FT_HANDLE, PUCHAR);
	PtrToGetBitMode m_pGetBitMode;

	typedef FT_STATUS (WINAPI *PtrToSetEventNotification)(FT_HANDLE, DWORD, PVOID);
	PtrToSetEventNotification m_pSetEventNotification;
	FT_STATUS SetEventNotification(DWORD, PVOID);

	typedef FT_STATUS (WINAPI *PtrToGetStatus)(FT_HANDLE, LPDWORD, LPDWORD, LPDWORD);
	PtrToGetStatus m_pGetStatus;
	FT_STATUS GetStatus(LPDWORD, LPDWORD, LPDWORD);
	
	typedef FT_STATUS (WINAPI *PtrToGetEventStatus)(FT_HANDLE, LPDWORD);
	PtrToGetEventStatus m_pGetEventStatus;

	typedef FT_STATUS (WINAPI *PtrToSetBaudRate)(FT_HANDLE, DWORD);
	PtrToSetBaudRate m_pSetBaudRate;
	FT_STATUS SetBaudRate(DWORD);

	typedef FT_STATUS (WINAPI *PtrToGetDriverVersion)(FT_HANDLE, LPDWORD);
	PtrToGetDriverVersion m_pGetDriverVersion;

	typedef FT_STATUS (WINAPI *PtrToGetLibraryVersion)(LPDWORD);
	PtrToGetLibraryVersion m_pGetLibraryVersion;

	// Выгрузка DLL
	BOOL UnloadDLL(void);
	// Флаг готовности принятых данных
	volatile BOOL m_DataIsReady;
	// Флаг принятия неполного пакета данных или ошибки
	volatile BOOL m_DataTimeout;
	// Запись буфера в порт, генерация события записи для потока
	void WriteToPort(void);
	// Запрос о существовании пульта
	BOOL PultIsPresent(BYTE SerNumPult);
	// Задержка ожидания события
	DWORD m_DelayWait;
	// Считывание ожидаемых данных
	BOOL ReadFromPort(void);
	// Запрос температуры и питания 24 В с пульта
	BOOL GetTemper(BYTE SerNumPult);
	// Запуск линии пульта
	BOOL SetLineInPult(BYTE SerNumPult, BYTE NumLine);
	// Устанавливает или отменяет рабочий режим пульта
	BOOL SetWorkReg(BYTE SerNumPult, bool WorkReg);
	// Отправка команды группового пуска
	BOOL SetGroup(BYTE SerNumPult, DWORD dwCmd);
	// Запись данных из буфера записи в пульт
	BOOL SetDataToPult(BYTE SerNumPult, DWORD dwWait);
	// Разрешение перерисовки в пульте
	BOOL EnableRedraw(BYTE SerNumPult, BOOL Redraw);
	// Разграничение записи, чтения в порт
	HANDLE m_hMutexReadWrite;
	// Вспомогательная переменная возврат результата
	BOOL m_Result;
	// Включить, выключить верхнее плечо
	BOOL OnHi(BYTE SerNumPult, BOOL OnOff);
	// Включение, выключение нижнего плеча
	BOOL OnLo(BYTE SerNumPult, BOOL OnOff);
	// Включение, выключение реле емкости
	BOOL OnCapRelay(BYTE SerNumPult, BOOL OnOff);
	// Включение, выключение реле сопротивления
	BOOL OnResRelay(BYTE SerNumPult, BOOL OnOff);
	// Команда проверки программы
	BOOL CheckPrg(BYTE SerNumPult);
	// Запрос измерения емкости
	int GetCap(BYTE SerNumPult, BYTE NumLine);
	// Запрос измерения сопротивления 
	int GetRes(BYTE SerNumPult, BYTE NumLine);
	// Изменение номера пульта
	BOOL ChangeNumPult(BYTE SerNumPult, BYTE NumPult);
	// Проверка поступления команды СТАРТ с пульта
	BOOL CheckStart(void);
	// Проверка режима пульта на запуск
	BOOL CRS485::CheckStart(BYTE SerNumPult);
	// Синхронизация времени общая команда для всех пультов
	void SyncTime(void);
	// Установка импульса запуска
	BOOL SetPulse(BYTE SerNumPult, int wPulse);
	// Опрос присутствия пульта на линии
	BOOL SendEcho(BYTE SerNumPult);
	// Таймаут ожидания ответа с пульта
	bool m_FlagTimeout;
	// Сброс линии пульта
	void ResetLine(BYTE SerNumPult);
	// Повтор передачи при ошибке ожидания ответа
	BOOL RepeatReadFromPort(void);
	// Команда СТАРТ всем пультам
	void SendStart(void);
	// Посылка сигнала всем пультам СТОП
	void SendStop(void);
	// Установка только тестового режима, остальные сброшены
	void SendReset(void);
	// Посылка команды перехода в начало
	BOOL SendBegin(BYTE SerNumPult);

	// Выключить линию
	BOOL ClearLineInPult(BYTE SerNumPult, BYTE NumLine);
	// Прочитать длительность импульса запуска
	BOOL GetPulse(BYTE SerNumPult);
	// Посылка команды в пульт
	BOOL SendCommand(BYTE SerNumPult, BYTE cmd, DWORD dwWait);
};

