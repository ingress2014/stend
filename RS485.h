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
	// ���������� ������������ �������
	int m_CountPult;
	// ��������� ���������� ���������������� 
	int GetNumDev(void);
	// ����� �������� ���������������
	BYTE* m_TxBuffer;
	// ����� ������ ���������������
	BYTE* m_RxBuffer;

public:
	// ����� ��������������� RS485
	FT_HANDLE m_hRS485;
	// ���������� ����������������
	int m_CountRS485;
	// ����� ��������������� RS485
	int m_NumRS485;
	//����� �������� �����
	DWORD dwListDescFlags;
	//������� �������� �����
	BOOL OpenPort(void);
	//�������� ����� 
	BOOL ClosePort(void);
	// ������� ����������� ���������������������� ���������������
	BOOL IsOpen(void) const
	{
		return m_hRS485 != NULL;
	}

	//������ DLL ftd2xx.dll
	HMODULE m_hmodule;

	// �������� DLL
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

	// �������� DLL
	BOOL UnloadDLL(void);
	// ���� ���������� �������� ������
	volatile BOOL m_DataIsReady;
	// ���� �������� ��������� ������ ������ ��� ������
	volatile BOOL m_DataTimeout;
	// ������ ������ � ����, ��������� ������� ������ ��� ������
	void WriteToPort(void);
	// ������ � ������������� ������
	BOOL PultIsPresent(BYTE SerNumPult);
	// �������� �������� �������
	DWORD m_DelayWait;
	// ���������� ��������� ������
	BOOL ReadFromPort(void);
	// ������ ����������� � ������� 24 � � ������
	BOOL GetTemper(BYTE SerNumPult);
	// ������ ����� ������
	BOOL SetLineInPult(BYTE SerNumPult, BYTE NumLine);
	// ������������� ��� �������� ������� ����� ������
	BOOL SetWorkReg(BYTE SerNumPult, bool WorkReg);
	// �������� ������� ���������� �����
	BOOL SetGroup(BYTE SerNumPult, DWORD dwCmd);
	// ������ ������ �� ������ ������ � �����
	BOOL SetDataToPult(BYTE SerNumPult, DWORD dwWait);
	// ���������� ����������� � ������
	BOOL EnableRedraw(BYTE SerNumPult, BOOL Redraw);
	// ������������� ������, ������ � ����
	HANDLE m_hMutexReadWrite;
	// ��������������� ���������� ������� ����������
	BOOL m_Result;
	// ��������, ��������� ������� �����
	BOOL OnHi(BYTE SerNumPult, BOOL OnOff);
	// ���������, ���������� ������� �����
	BOOL OnLo(BYTE SerNumPult, BOOL OnOff);
	// ���������, ���������� ���� �������
	BOOL OnCapRelay(BYTE SerNumPult, BOOL OnOff);
	// ���������, ���������� ���� �������������
	BOOL OnResRelay(BYTE SerNumPult, BOOL OnOff);
	// ������� �������� ���������
	BOOL CheckPrg(BYTE SerNumPult);
	// ������ ��������� �������
	int GetCap(BYTE SerNumPult, BYTE NumLine);
	// ������ ��������� ������������� 
	int GetRes(BYTE SerNumPult, BYTE NumLine);
	// ��������� ������ ������
	BOOL ChangeNumPult(BYTE SerNumPult, BYTE NumPult);
	// �������� ����������� ������� ����� � ������
	BOOL CheckStart(void);
	// �������� ������ ������ �� ������
	BOOL CRS485::CheckStart(BYTE SerNumPult);
	// ������������� ������� ����� ������� ��� ���� �������
	void SyncTime(void);
	// ��������� �������� �������
	BOOL SetPulse(BYTE SerNumPult, int wPulse);
	// ����� ����������� ������ �� �����
	BOOL SendEcho(BYTE SerNumPult);
	// ������� �������� ������ � ������
	bool m_FlagTimeout;
	// ����� ����� ������
	void ResetLine(BYTE SerNumPult);
	// ������ �������� ��� ������ �������� ������
	BOOL RepeatReadFromPort(void);
	// ������� ����� ���� �������
	void SendStart(void);
	// ������� ������� ���� ������� ����
	void SendStop(void);
	// ��������� ������ ��������� ������, ��������� ��������
	void SendReset(void);
	// ������� ������� �������� � ������
	BOOL SendBegin(BYTE SerNumPult);

	// ��������� �����
	BOOL ClearLineInPult(BYTE SerNumPult, BYTE NumLine);
	// ��������� ������������ �������� �������
	BOOL GetPulse(BYTE SerNumPult);
	// ������� ������� � �����
	BOOL SendCommand(BYTE SerNumPult, BYTE cmd, DWORD dwWait);
};

