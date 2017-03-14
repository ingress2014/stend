#include "StdAfx.h"
#include "RS485.h"
#include "function.h"
#include "PultConst.h"

//�������� ����� 
BOOL CRS485::ClosePort(void)
{

	if(m_hRS485) {
		Close();
		m_hRS485 = NULL;
	}

	return TRUE;
}


CRS485::CRS485(void)
	: m_CountPult(0)
	, m_hRS485(NULL)
	, m_NumRS485(0)
	, m_DataIsReady(FALSE)
	, m_DataTimeout(FALSE)
	, m_DelayWait(20)
	, m_Result(FALSE)
	, m_FlagTimeout(false)
{
	m_RxBuffer = new BYTE[RX_BUFFER_SIZE];
	m_TxBuffer = new BYTE[TX_BUFFER_SIZE];
	m_CountRS485 = this->GetNumDev();
	m_hMutexReadWrite = CreateMutex(NULL, FALSE, "hMutexReadWrite");
}

CRS485::~CRS485(void)
{
	delete [] m_RxBuffer;
	delete [] m_TxBuffer;
	m_hRS485 = NULL;
	CloseHandle(m_hMutexReadWrite);

}

//����������� ���������� �������������� ���������
int CRS485::GetNumDev(void)
{
	FT_STATUS ftStatus;
	DWORD NumDevs;

	ftStatus = FT_CreateDeviceInfoList(&NumDevs);
	if (ftStatus == FT_OK) return NumDevs;
	else return 0;
}
//���������� �� ����������
FT_STATUS CRS485::Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	DWORD BytesAvail = 0;

	if (!m_pRead)
	{
		AfxMessageBox(TEXT("FT_Read is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pRead)(m_hRS485, lpvBuffer, dwBuffSize, lpdwBytesRead);
}	
//������ � ����������
FT_STATUS CRS485::Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!m_pWrite)
	{
		AfxMessageBox(TEXT("FT_Write is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pWrite)(m_hRS485, lpvBuffer, dwBuffSize, lpdwBytes);
}
// �������� DLL
void CRS485::LoadDLL(void)
{
	m_hmodule = LoadLibrary(TEXT("ftd2xx.dll"));	
	if(m_hmodule == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Load ftd2xx.dll"));
		return;
	}

	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, TEXT("FT_Write"));
	if (m_pWrite == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_Write"));
		return;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, TEXT("FT_Read"));
	if (m_pRead == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_Read"));
		return;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, TEXT("FT_Open"));
	if (m_pOpen == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_Open"));
		return;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, TEXT("FT_OpenEx"));
	if (m_pOpenEx == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_OpenEx"));
		return;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, TEXT("FT_ListDevices"));
	if(m_pListDevices == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_ListDevices"));
		return;
	}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, TEXT("FT_Close"));
	if (m_pClose == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_Close"));
		return;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, TEXT("FT_ResetDevice"));
	if (m_pResetDevice == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_ResetDevice"));
		return;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, TEXT("FT_Purge"));
	if (m_pPurge == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_Purge"));
		return;
	}

	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, TEXT("FT_SetTimeouts"));
	if (m_pSetTimeouts == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetTimeouts"));
		return;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, TEXT("FT_GetQueueStatus"));
	if (m_pGetQueueStatus == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetQueueStatus"));
		return;
	}

	m_pSetDtr = (PtrToSetDtr)GetProcAddress(m_hmodule, TEXT("FT_SetDtr"));
	if (m_pSetDtr == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetDtr"));
		return;
	}

	m_pClrDtr = (PtrToClrDtr)GetProcAddress(m_hmodule, TEXT("FT_ClrDtr"));
	if (m_pClrDtr == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_ClrDtr"));
		return;
	}

	m_pSetRts = (PtrToSetRts)GetProcAddress(m_hmodule, TEXT("FT_SetRts"));
	if (m_pSetRts == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetRts"));
		return;
	}

	m_pClrRts = (PtrToClrRts)GetProcAddress(m_hmodule, TEXT("FT_ClrRts"));
	if (m_pClrRts == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_ClrRts"));
		return;
	}

	m_pGetModemStatus = (PtrToGetModemStatus)GetProcAddress(m_hmodule, TEXT("FT_GetModemStatus"));
	if (m_pGetModemStatus == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetModemStatus"));
		return;
	}

	m_pSetChars = (PtrToSetChars)GetProcAddress(m_hmodule, TEXT("FT_SetChars"));
	if (m_pSetChars == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetChars"));
		return;
	}

	m_pSetLatencyTimer = (PtrToSetLatencyTimer)GetProcAddress(m_hmodule, TEXT("FT_SetLatencyTimer"));
	if (m_pSetLatencyTimer == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetLatencyTimer"));
		return;
	}


	m_pGetLatencyTimer = (PtrToGetLatencyTimer)GetProcAddress(m_hmodule, TEXT("FT_GetLatencyTimer"));
	if (m_pGetLatencyTimer == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetLatencyTimer"));
		return;
	}


	m_pSetDataCharacteristics = (PtrToSetDataCharacteristics)GetProcAddress(m_hmodule, TEXT("FT_SetDataCharacteristics"));
	if (m_pSetDataCharacteristics == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetDataCharacteristics"));
		return;
	}


	m_pSetFlowControl = (PtrToSetFlowControl)GetProcAddress(m_hmodule, TEXT("FT_SetFlowControl"));
	if (m_pSetFlowControl == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetFlowControl"));
		return;
	}


	m_pSetBreakOn = (PtrToSetBreakOn)GetProcAddress(m_hmodule, TEXT("FT_SetBreakOn"));
	if (m_pSetBreakOn == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetBreakOn"));
		return;
	}	


	m_pSetBreakOff = (PtrToSetBreakOff)GetProcAddress(m_hmodule, TEXT("FT_SetBreakOff"));
	if (m_pSetBreakOff == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetBreakOff"));
		return;
	}


	m_pResetPort = (PtrToResetPort)GetProcAddress(m_hmodule, TEXT("FT_ResetPort"));
	if (m_pResetPort == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_ResetPort"));
		return;
	}


	m_pGetDeviceInfo = (PtrToGetDeviceInfo)GetProcAddress(m_hmodule, TEXT("FT_GetDeviceInfo"));
	if (m_pGetDeviceInfo == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetDeviceInfo"));
		return;
	}


	m_pEE_UASize = (PtrToEE_UASize)GetProcAddress(m_hmodule, TEXT("FT_EE_UASize"));
	if (m_pEE_UASize == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_EE_UASize"));
		return;
	}


	m_pEE_UARead = (PtrToEE_UARead)GetProcAddress(m_hmodule, TEXT("FT_EE_UARead"));
	if (m_pEE_UARead == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_EE_UARead"));
		return;
	}


	m_pEE_UAWrite = (PtrToEE_UAWrite)GetProcAddress(m_hmodule, TEXT("FT_EE_UAWrite"));
	if (m_pEE_UAWrite == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_EE_UAWrite"));
		return;
	}


	m_pSetBitMode = (PtrToSetBitMode)GetProcAddress(m_hmodule, TEXT("FT_SetBitMode"));
	if (m_pSetBitMode == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetBitMode"));
		return;
	}

	m_pGetBitMode = (PtrToGetBitMode)GetProcAddress(m_hmodule, TEXT("FT_GetBitMode"));
	if (m_pGetBitMode == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetBitMode"));
		return;
	}

	m_pSetEventNotification = (PtrToSetEventNotification)GetProcAddress(m_hmodule, TEXT("FT_SetEventNotification"));
	if (m_pSetEventNotification == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetEventNotification"));
		return;
	}

	m_pGetStatus = (PtrToGetStatus)GetProcAddress(m_hmodule, TEXT("FT_GetStatus"));
	if (m_pGetStatus == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetStatus"));
		return;
	}

	m_pGetEventStatus = (PtrToGetEventStatus)GetProcAddress(m_hmodule, TEXT("FT_GetEventStatus"));
	if (m_pGetEventStatus == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetEventStatus"));
		return;
	}

	m_pSetBaudRate = (PtrToSetBaudRate)GetProcAddress(m_hmodule, TEXT("FT_SetBaudRate"));
	if (m_pSetBaudRate == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_SetBaudRate"));
		return;
	}

	m_pGetDriverVersion = (PtrToGetDriverVersion)GetProcAddress(m_hmodule, TEXT("FT_GetDriverVersion"));
	if (m_pGetDriverVersion == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetDriverVersion"));
		return;
	}

	m_pGetLibraryVersion = (PtrToGetLibraryVersion)GetProcAddress(m_hmodule, TEXT("FT_GetLibraryVersion"));
	if (m_pGetLibraryVersion == NULL)
	{
		AfxMessageBox(TEXT("Error: Can't Find FT_GetLibraryVersion"));
		return;
	}

}
// �������� DLL
BOOL CRS485::UnloadDLL(void)
{
	return FreeLibrary(m_hmodule);
}
//��������� �������� ��������
FT_STATUS CRS485::SetBaudRate(DWORD dwBaudRate)
{
	if (!m_pSetBaudRate)
	{
		AfxMessageBox(TEXT("FT_SetBaudRate is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetBaudRate)(m_hRS485, dwBaudRate);
}	
//��������� � ������������� ���������, ��������� ������� ����������� ������/������	
BOOL CRS485::OpenPort(void)
{
	FT_STATUS ftStatus;
	
	// �������� ���������������
	ftStatus = Open(0);
	if (!FT_SUCCESS(ftStatus)) return FALSE;
	ftStatus = SetBaudRate(FT_BAUD_115200);
	ftStatus |= SetDataCharacteristics(
		FT_BITS_8,
		FT_STOP_BITS_1,
		FT_PARITY_ODD);
	ftStatus |= SetFlowControl(FT_FLOW_NONE,0,0);
	ftStatus |= SetTimeouts(TIMEOUT_WRITE,TIMEOUT_READ);//�������� �� ������, ������ 
	ftStatus |= Purge(FT_PURGE_RX | FT_PURGE_TX);
	if (!FT_SUCCESS(ftStatus)) return FALSE;
	return TRUE;
}
// �������� �����
FT_STATUS CRS485::Open(PVOID pvDevice)
{
	if (!m_pOpen)
	{
		AfxMessageBox(TEXT("FT_Open is not valid!")); 
		return FT_INVALID_HANDLE;
	}
	return (*m_pOpen)(pvDevice, &m_hRS485);
}	
//������� �����
FT_STATUS CRS485::Purge(ULONG dwMask)
{
	if (!m_pPurge)
	{
		AfxMessageBox(TEXT("FT_Purge is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pPurge)(m_hRS485, dwMask);
}	
//��������� �������� �����
FT_STATUS CRS485::SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!m_pSetTimeouts)
	{
		AfxMessageBox(TEXT("FT_SetTimeouts is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetTimeouts)(m_hRS485, dwReadTimeout, dwWriteTimeout);
}
//��������� ���������� ��������
FT_STATUS CRS485::SetDataCharacteristics(UCHAR uWordLength, UCHAR uStopBits, UCHAR uParity)
{
	if (!m_pSetDataCharacteristics)
	{
		AfxMessageBox(TEXT("FT_SetDataCharacteristics is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetDataCharacteristics)(m_hRS485, uWordLength, uStopBits, uParity);
}
//��������� ���������� �����
FT_STATUS CRS485::SetFlowControl(USHORT usFlowControl, UCHAR uXon, UCHAR uXoff)
{
	if (!m_pSetFlowControl)
	{
		AfxMessageBox(TEXT("FT_SetFlowControl is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetFlowControl)(m_hRS485, usFlowControl, uXon, uXoff);
}
//��������� ��������� �� ��������
FT_STATUS CRS485::SetEventNotification(DWORD dwEventMask, PVOID pvArg)
{
	if (!m_pSetEventNotification)
	{
		AfxMessageBox(TEXT("FT_SetEventNotification is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pSetEventNotification)(m_hRS485, dwEventMask, pvArg);
}
//������ ���������������
FT_STATUS CRS485::GetStatus(LPDWORD lpdwAmountInRxQueue, LPDWORD lpdwAmountInTxQueue, LPDWORD lpdwEventStatus )
{
	if (!m_pGetStatus)
	{
		AfxMessageBox(TEXT("FT_GetStatus is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pGetStatus)(m_hRS485, lpdwAmountInRxQueue, lpdwAmountInTxQueue, lpdwEventStatus);
}
//�������� �����
FT_STATUS CRS485::Close()
{
	FT_STATUS status;
	CString str;
	if (!m_pClose)
	{
		AfxMessageBox(TEXT("FT_Close is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	status = (*m_pClose)(m_hRS485);
	m_hRS485 = NULL;
	return status;
}	
// ������ ������ � ����, ��������� ������� ������ ��� ������
void CRS485::WriteToPort(void)
{
	if (m_CountRS485 == 0) return;

	DWORD dwBytes, dwWaitReturn;
	//������� ����������� ����� � �������� 
	WORD tempCRC = CRC16(m_TxBuffer,TX_BUFFER_SIZE - 2);// 2 ��������� ����� �����. �����
	m_TxBuffer[TX_BUFFER_SIZE-2]=tempCRC & 0x00FF;
	m_TxBuffer[TX_BUFFER_SIZE-1]=(tempCRC & 0xFF00)>>8;

	//���� ��������� ������ � �����
	dwWaitReturn = ::WaitForSingleObject(m_hMutexReadWrite, INFINITE);

	switch(dwWaitReturn)
	{
	case WAIT_OBJECT_0:
		// ������ � ����������
		Write(m_TxBuffer,TX_BUFFER_SIZE,&dwBytes);
		break;
	case WAIT_ABANDONED:
		ASSERT(FALSE);
		break;
	default:
		ASSERT(FALSE);
	}
	//����������� ������
	VERIFY(::ReleaseMutex(m_hMutexReadWrite));

	//SetEvent(m_hEventWrite);
}
//����� ����������
FT_STATUS CRS485::ResetDevice()
{
	if (!m_pResetDevice)
	{
		AfxMessageBox(TEXT("FT_ResetDevice is not valid!")); 
		return FT_INVALID_HANDLE;
	}

	return (*m_pResetDevice)(m_hRS485);
}	
// ������ � ������������� ������
BOOL CRS485::PultIsPresent(BYTE SerNumPult)
{
	#ifdef TEST

	if (NumPult < 5)
	{
		m_RxBuffer[2] = SerNumPult; //
		m_RxBuffer[5] = REG_READY; //
		return TRUE;
	}
	return FALSE;


	#endif

	if (m_CountRS485 == 0) return FALSE;

	if(m_hRS485 == NULL) 
	{
		return FALSE;
	}

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_ECHO;
	WriteToPort();
	m_DelayWait = 20; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return ReadFromPort();
}
// ���������� ��������� ������
BOOL CRS485::ReadFromPort(void)
{
	// ������������� ��� �������� ������� ����� ������	DWORD dwBytes = 0;
	FT_STATUS ftStatus;
	HANDLE hEventRxChar; 
	DWORD EventMask, dwWaitReturn, dwBytes; 

	if (m_CountRS485 == 0) return FALSE;

	m_FlagTimeout = false;

	// ������� ����������� ������� �� ������
	hEventRxChar = CreateEvent( 	 
		NULL, //�����
		false, // bManualReset ��������� ������� (���� true - �� ����� ResetEvent)
		false, // ������� � ������������ ��������� 
		NULL 
		); 

	EventMask = FT_EVENT_RXCHAR; // ���� ������� ����������� �������
	ftStatus = SetEventNotification(EventMask, hEventRxChar); 


	DWORD dwRxQueue, dwTxQueue, dwEventStatus;

	//���� ��������� ������ � ����
	dwWaitReturn = ::WaitForSingleObject(m_hMutexReadWrite, INFINITE);
	// �������� ������� ������� ����������� ����� � ���������������
	dwWaitReturn = WaitForSingleObject(hEventRxChar, m_DelayWait);
	//�������� ������ � ���������������
	if(dwWaitReturn == WAIT_OBJECT_0)
	{
		// ������� ������ �������
		ftStatus = GetStatus(&dwRxQueue, &dwTxQueue, &dwEventStatus);
		if (FT_SUCCESS(ftStatus))
		{
			//���� � ������� ���� �������� �����
			if (dwRxQueue)
			{
				//������ �� ��������������� � ����� 9 ����
				ftStatus = Read(
					m_RxBuffer, // ����� ������
					RX_BUFFER_SIZE,  // � ������ 14 ���� + 1 post
					//dwRxQueue,  // ����� ���������
					&dwBytes    // ����� �������
					);

				if (FT_SUCCESS(ftStatus))
				{
					//������� �����
					if (dwBytes <= RX_BUFFER_SIZE)
					{
						//���� ������� ����� ����������� �������
						WORD tempCRC;
						tempCRC = m_RxBuffer[RX_BUFFER_SIZE-2]; //post - 1 ����, ���� �� 0
						tempCRC = tempCRC << 8;
						tempCRC |= m_RxBuffer[RX_BUFFER_SIZE-3];
						// ���� �� ��������� ����������� �����
						if (CRC16(m_RxBuffer,RX_BUFFER_SIZE-3) != tempCRC) //�������������� ������� 12 ���� 
						{
							Purge(FT_PURGE_RX | FT_PURGE_TX);
							ReleaseMutex(m_hMutexReadWrite);
							return FALSE;
						} 
						else
						{
							if((m_RxBuffer[1] & 0x80) == 0)
							{
								//������ � ���������� ����������� ������
								ReleaseMutex(m_hMutexReadWrite);
								return TRUE;
							}
							else
							{
								ReleaseMutex(m_hMutexReadWrite);
								return FALSE;
							}
						
						}
					} 
					else
					{
						//����� ������������� �������
						Purge(FT_PURGE_RX | FT_PURGE_TX);
						ReleaseMutex(m_hMutexReadWrite);
						return FALSE;
					}
				} 
				else
				{
					//��������� ����� Read status - fail
					Purge(FT_PURGE_RX | FT_PURGE_TX);
					ReleaseMutex(m_hMutexReadWrite);
					return FALSE;
				}
			} 
			else
			{
				//� ������� ��� �������� ����
				Purge(FT_PURGE_RX | FT_PURGE_TX);
				ReleaseMutex(m_hMutexReadWrite);
				return FALSE;
			}
		} 
		else
		{
			//GetStatus - �� ������ �������
			Purge(FT_PURGE_RX | FT_PURGE_TX);
			ReleaseMutex(m_hMutexReadWrite);
			return FALSE;
		}

	}
	//� ��������� ������� ������� ������ ���������
	//WAIT_ABANDONED,WAIT_TIMEOUT,WAIT_FAILED
	else
	{
		//m_FlagTimeout = true;
		Purge(FT_PURGE_RX | FT_PURGE_TX);
		ReleaseMutex(m_hMutexReadWrite);
		return FALSE;
	}
	Purge(FT_PURGE_RX | FT_PURGE_TX);
	ReleaseMutex(m_hMutexReadWrite);
	return FALSE;
}
// ������ ����������� � ������� 24 � � ������
BOOL CRS485::GetTemper(BYTE SerNumPult)
{

#ifdef TEST

	if (NumPult < 5)
	{
		m_RxBuffer[2] = SerNumPult; //
		m_RxBuffer[5] = REG_READY; //
		return TRUE;
	}
	return FALSE;

#endif

	if (m_CountRS485 == 0) return FALSE;


	if(m_hRS485 == NULL) 
	{
		return FALSE;
	}

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_GET_TEMPER;
	WriteToPort();
	m_DelayWait = 200; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return ReadFromPort();
}
// ������ ����� ������
BOOL CRS485::SetLineInPult(BYTE SerNumPult, BYTE NumLine)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	if (NumLine > 32)
	{

		m_TxBuffer[1] = CMD_MOTOR_ON;
		m_TxBuffer[2] = NumLine-32;
	}
	else
	{	
		m_TxBuffer[1] = CMD_SET_LINE;
		m_TxBuffer[2] = NumLine;
	}
	
	WriteToPort();
	m_DelayWait = 20; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return ReadFromPort();
}
//��������� ������ ������ ������ - �������/��������
BOOL CRS485::SetWorkReg(BYTE SerNumPult, bool WorkReg)
{

#ifdef TEST

	if (NumPult < 5)
	{
		m_RxBuffer[2] = SerNumPult; //
		m_RxBuffer[5] = WorkReg ? REG_READY : REG_TEST; //
		return TRUE;
	}
	
	return FALSE;

#endif

	if (m_CountRS485 == 0) return FALSE;

	BYTE tempCmd;

	tempCmd = WorkReg ? REG_WORK : REG_TEST; 

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_SET_MODE;
	m_TxBuffer[2] = tempCmd;
	WriteToPort();
	m_DelayWait = 20; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return ReadFromPort();
}
// �������� ������� ���������� �����
BOOL CRS485::SetGroup(BYTE SerNumPult, DWORD dwCmd)
{
	BYTE cmdByteL, cmdByteML, cmdByteMH,cmdByteH;

	if (m_CountRS485 == 0) return FALSE;


	if(m_hRS485 == NULL) 
	{
		return FALSE;
	}

	cmdByteL = static_cast<BYTE>(dwCmd & 0x000000FF);
	cmdByteML = static_cast<BYTE>((dwCmd & 0x0000FF00) >> 8);
	cmdByteMH = static_cast<BYTE>((dwCmd & 0x00FF0000) >> 16) ;
	cmdByteH =  static_cast<BYTE>((dwCmd & 0xFF000000) >> 24);

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_SET_GROUP_LINE;
	m_TxBuffer[2] = cmdByteL;
	m_TxBuffer[3] = cmdByteML;
	m_TxBuffer[4] = cmdByteMH;
	m_TxBuffer[5] = cmdByteH;
	WriteToPort();

	//������ �� ����
//	m_DelayWait = 200; //������� �������� ������
	//���������� ������ � ���� � ���� ������
//	return ReadFromPort();
	return 0;
}
// ������ ������ �� ������ ������ � �����
BOOL CRS485::SetDataToPult(BYTE SerNumPult, DWORD dwWait)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	WriteToPort();
	m_DelayWait = dwWait; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return RepeatReadFromPort();
}
// ���������� ����������� � ������
BOOL CRS485::EnableRedraw(BYTE SerNumPult, BOOL Redraw)
{
	BYTE tempCmd;

	if (m_CountRS485 == 0) return FALSE;

	tempCmd = Redraw ? CMD_REDRAW_ENABLE : CMD_REDRAW_DISABLE; 

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = tempCmd;
	WriteToPort();
	m_DelayWait = 2000; //������� �������� ������

	return RepeatReadFromPort();
}
// �������� ������� �����
BOOL CRS485::OnHi(BYTE SerNumPult, BOOL OnOff)
{
	BYTE tempCmd;

	if (m_CountRS485 == 0) return FALSE;

	tempCmd = OnOff ? CMD_ON_HI : CMD_OFF_HI; 

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = tempCmd;
	WriteToPort();
	m_DelayWait = 200; //������� �������� ������

	return ReadFromPort();
}
// ���������, ���������� ������� �����
BOOL CRS485::OnLo(BYTE SerNumPult, BOOL OnOff)
{
	BYTE tempCmd;

	if (m_CountRS485 == 0) return FALSE;


	tempCmd = OnOff ? CMD_ON_LO : CMD_OFF_LO; 
	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = tempCmd;
	WriteToPort();
	m_DelayWait = 200; //������� �������� ������

	return ReadFromPort();
}
// ���������, ���������� ���� �������
BOOL CRS485::OnCapRelay(BYTE SerNumPult, BOOL OnOff)
{
	BYTE tempCmd;

	if (m_CountRS485 == 0) return FALSE;



	tempCmd = OnOff ? CMD_ON_CAP_RELAY : CMD_OFF_CAP_RELAY; 
	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = tempCmd;
	WriteToPort();
	m_DelayWait = 200; //������� �������� ������
	return ReadFromPort();
}
// ���������, ���������� ���� �������������
BOOL CRS485::OnResRelay(BYTE SerNumPult, BOOL OnOff)
{
	BYTE tempCmd;

	if (m_CountRS485 == 0) return FALSE;


	tempCmd = OnOff ? CMD_ON_RES_RELAY : CMD_OFF_RES_RELAY; 
	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = tempCmd;
	WriteToPort();
	m_DelayWait = 200; //������� �������� ������
	return ReadFromPort();
}
// ������� �������� ���������
BOOL CRS485::CheckPrg(BYTE SerNumPult)
{

	if (m_CountRS485 == 0) return FALSE;


	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_CHECK;
	WriteToPort();
	m_DelayWait = 5000; //������� �������� ������
	return RepeatReadFromPort();
}
// ������ ��������� �������
int CRS485::GetCap(BYTE SerNumPult, BYTE NumLine)
{

	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_GET_LINE;
	m_TxBuffer[2] = NumLine;
	WriteToPort();
	m_DelayWait = 5000; //������� �������� ������
	return RepeatReadFromPort();
}
// ������ ��������� ������������� 
int CRS485::GetRes(BYTE SerNumPult, BYTE NumLine)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_GET_RES;
	m_TxBuffer[2] = NumLine;
	WriteToPort();
	m_DelayWait = 5000; //������� �������� ������
	return RepeatReadFromPort();
}
// ��������� ������ ������
BOOL CRS485::ChangeNumPult(BYTE SerNumPult, BYTE NumPult)
{

	if (m_CountRS485 == 0) return FALSE;


	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_SET_NUM;
	m_TxBuffer[2] = NumPult;
	WriteToPort();
	m_DelayWait = 2000; //������� �������� ������
	return ReadFromPort();
}
//�������������� �������� ������ � ������
BOOL CRS485::CheckStart(BYTE SerNumPult)
{
	if (SendEcho(SerNumPult))
		if ((m_RxBuffer[2] & REG_RUN) != 0)
			return TRUE;
	return FALSE;
}
// �������� ����������� ������� ����� � ������
BOOL CRS485::CheckStart(void)
{
	DWORD dwBytes = 0;
	FT_STATUS ftStatus;
	HANDLE hEventRxChar; 
	DWORD EventMask, dwWaitReturn; 

	if (m_CountRS485 == 0) return FALSE;


	// ������� ����������� ������� �� ������
	hEventRxChar = CreateEvent( 	 
		NULL, //�����
		false, // bManualReset ��������� ������� (���� true - �� ����� ResetEvent)
		false, // ������� � ������������ ��������� 
		NULL 
		); 

	EventMask = FT_EVENT_RXCHAR; // ���� ������� ����������� �������
	ftStatus = SetEventNotification(EventMask, hEventRxChar); 
	
	DWORD dwRxQueue, dwTxQueue, dwEventStatus;
	
	//���� ��������� ������ � ����
	dwWaitReturn = ::WaitForSingleObject(m_hMutexReadWrite, INFINITE);
	// �������� ������� ������� ����������� ����� � ���������������
	dwWaitReturn = WaitForSingleObject(hEventRxChar, DELAY_START_COMMAND);
	//�������� ������ � ���������������
	if(dwWaitReturn == WAIT_OBJECT_0)
	{
		// ������� ������ �������
		ftStatus = GetStatus(&dwRxQueue, &dwTxQueue, &dwEventStatus);
		if (FT_SUCCESS(ftStatus))
		{
			//���� � ������� ���� �������� �����
			if (dwRxQueue)
			{
				//������ �� ��������������� � ����� 14 ����
				ftStatus = Read(
					m_RxBuffer, // ����� ������
					RX_BUFFER_SIZE,  // � ������ 14 ���� + 1 post
					//dwRxQueue,  // ����� ���������
					&dwBytes    // ����� �������
					);

				if (FT_SUCCESS(ftStatus))
				{
					//������� �����
					if (dwBytes == RX_BUFFER_SIZE)
					{
						//���� ������� ����� ����������� �������
						WORD tempCRC;
						tempCRC = m_RxBuffer[RX_BUFFER_SIZE - 2];
						tempCRC = tempCRC << 8;
						tempCRC |= m_RxBuffer[RX_BUFFER_SIZE - 3];
						// ���� �� ��������� ����������� �����
						if (CRC16(m_RxBuffer,RX_BUFFER_SIZE - 3) != tempCRC)
						{
							Purge(FT_PURGE_RX | FT_PURGE_TX);
							ReleaseMutex(m_hMutexReadWrite);
							return FALSE;
						} 
						else
						{
							//������ � ���������� ����������� ������
							//��������, ��� ����������� ������� = CMD_PUSK

							if (m_RxBuffer[1] == CMD_PUSK)
							{
								ReleaseMutex(m_hMutexReadWrite);
								return TRUE;
							} 
							else
							{
								Purge(FT_PURGE_RX | FT_PURGE_TX);
								ReleaseMutex(m_hMutexReadWrite);
								return FALSE;
							}

						}
					} 
					else
					{
						//����� ������������� �������
						Purge(FT_PURGE_RX | FT_PURGE_TX);
						ReleaseMutex(m_hMutexReadWrite);
						return FALSE;
					}
				} 
				else
				{
					//��������� ����� Read status - fail
					Purge(FT_PURGE_RX | FT_PURGE_TX);
					ReleaseMutex(m_hMutexReadWrite);
					return FALSE;
				}
			} 
			else
			{
				//� ������� ��� �������� ����
				Purge(FT_PURGE_RX | FT_PURGE_TX);
				ReleaseMutex(m_hMutexReadWrite);
				return FALSE;
			}
		} 
		else
		{
			//GetStatus - �� ������ �������
			Purge(FT_PURGE_RX | FT_PURGE_TX);
			ReleaseMutex(m_hMutexReadWrite);
			return FALSE;
		}

	}
	//� ��������� ������� ������� ������ ���������
	//WAIT_ABANDONED,WAIT_TIMEOUT,WAIT_FAILED
	else
	{
		Purge(FT_PURGE_RX | FT_PURGE_TX);
		ReleaseMutex(m_hMutexReadWrite);
		return FALSE;
	}
	Purge(FT_PURGE_RX | FT_PURGE_TX);
	ReleaseMutex(m_hMutexReadWrite);
	return FALSE;
}
// ������������� ������� ����� ������� ��� ���� �������
void CRS485::SyncTime(void)
{
	SYSTEMTIME st;
	BYTE ss, minute, hh, dd, mm, yy;
	
	if (m_CountRS485 == 0) return;

	GetLocalTime(&st);
	ss = static_cast<BYTE>(st.wSecond);
	minute = static_cast<BYTE>(st.wMinute);
	hh = static_cast<BYTE>(st.wHour);
	dd = static_cast<BYTE>(st.wDay);
	mm = static_cast<BYTE>(st.wMonth);
	yy = static_cast<BYTE>(st.wYear - 2000);


	m_TxBuffer[0] = FOR_ALL;
	m_TxBuffer[1] = CMD_SET_TIME;
	m_TxBuffer[2] = ss;
	m_TxBuffer[3] = minute;
	m_TxBuffer[4] = hh;
	m_TxBuffer[5] = dd;
	m_TxBuffer[6] = mm;
	m_TxBuffer[7] = yy;
	WriteToPort();
	Sleep(30);
	return;
}
// ��������� �������� �������
BOOL CRS485::SetPulse(BYTE SerNumPult, int wPulse)
{

	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_SET_PULSE;
	m_TxBuffer[2] = static_cast<BYTE>(wPulse & 0x00FF)  ;
	m_TxBuffer[3] = static_cast<BYTE>(wPulse >> 8);
	m_TxBuffer[4] = 0;
	m_TxBuffer[5] = 0;
	WriteToPort();
	m_DelayWait = 2000;
	return ReadFromPort();
}
// ����� ����������� ������ �� �����
BOOL CRS485::SendEcho(BYTE SerNumPult)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_ECHO;
	WriteToPort();
	m_DelayWait = 30;
	return RepeatReadFromPort();
}
// ����� ����� ������
void CRS485::ResetLine(BYTE SerNumPult)
{
	if (m_CountRS485 == 0) return;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_CLEAR_LINE;
	m_TxBuffer[2] = 0;
	m_TxBuffer[3] = 0;
	m_TxBuffer[4] = 0;
	m_TxBuffer[5] = 0;
	WriteToPort();

	return;
}
// ������ �������� ��� ������ �������� ������
BOOL CRS485::RepeatReadFromPort(void)
{
	if (!ReadFromPort()) WriteToPort();
	else return TRUE;

	if (!ReadFromPort()) WriteToPort();
	else return TRUE;
	
	return ReadFromPort();
}
// ������� ����� ���� �������
void CRS485::SendStart(void)
{
	if (m_CountRS485 == 0) return;
	
	m_TxBuffer[0] = FOR_ALL;
	m_TxBuffer[1] = CMD_PUSK;
	WriteToPort();
	return;
}
// ������� ������� ���� ������� ����
void CRS485::SendStop(void)
{
	if (m_CountRS485 == 0) return;
	m_TxBuffer[0] = FOR_ALL;
	m_TxBuffer[1] = CMD_STOP;
	WriteToPort();
	return;
}
// ��������� ������ ��������� ������, ��������� ��������
void CRS485::SendReset(void)
{
	if (m_CountRS485 == 0) return;

	m_TxBuffer[0] = FOR_ALL;
	m_TxBuffer[1] = CMD_RESET;
	WriteToPort();
	return;
}
// ������� ������� �������� � ������
BOOL CRS485::SendBegin(BYTE SerNumPult)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_BEGIN;
	WriteToPort();
	m_DelayWait = 2000;
	return RepeatReadFromPort();
}


// ��������� �����
BOOL CRS485::ClearLineInPult(BYTE SerNumPult, BYTE NumLine)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	if (NumLine > 32)
	{
		m_TxBuffer[1] = CMD_MOTOR_OFF;
		m_TxBuffer[2] = NumLine-32;
	}
	else	return FALSE;
	WriteToPort();
	m_DelayWait = 20; //������� �������� ������
	//���������� ������ � ���� � ���� ������
	return ReadFromPort();
}


// ��������� ������������ �������� �������
BOOL CRS485::GetPulse(BYTE SerNumPult)
{
	if (m_CountRS485 == 0) return FALSE;

	m_TxBuffer[0] = SerNumPult;
	m_TxBuffer[1] = CMD_GET_PULSE;
	m_TxBuffer[2] = 0;
	m_TxBuffer[3] = 0;
	WriteToPort();
	m_DelayWait = 5000; //������� �������� ������
	return RepeatReadFromPort();
}


// ������� ������� � �����
BOOL CRS485::SendCommand(BYTE SerNumPult, BYTE cmd, DWORD dwWait)
{
		if (m_CountRS485 == 0) return false;

		m_TxBuffer[0] = SerNumPult;
		m_TxBuffer[1] = cmd;
		WriteToPort();
		m_DelayWait = dwWait; //������� �������� ������
		return RepeatReadFromPort();
}
