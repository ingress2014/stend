#pragma once
#include "rs485.h"



class CPult
{
public:
	CPult(void);
	//����������� �����������
	CPult(const CPult &pult); 
	~CPult(void);
	
	CPult operator=( CPult &pult )
	{
		m_SerNumPult = pult.m_SerNumPult;
		m_NumPult = pult.m_NumPult;
		m_Power12 = pult.m_Power12;
		m_Power24 = pult.m_Power24;
		m_Temper = pult.m_Temper;
		m_WorkMode = pult.m_WorkMode;
		m_Pulse = pult.m_Pulse;

		memcpy(m_ResLine,pult.m_ResLine,sizeof(pult.m_ResLine));
		memcpy(m_CapLine,pult.m_CapLine,sizeof(pult.m_CapLine));
		memcpy(m_CountLine,pult.m_CountLine,sizeof(pult.m_CountLine));
		return( *this );
	}
	
	// �������� ����� ������
	BYTE m_SerNumPult;
	// ����� ������
	BYTE m_NumPult;
	// ���� ��������� ������ ������ ������
	BYTE m_WorkMode;
	// ������� 12 �����
	WORD m_Power12;
	// ������� 24 ������
	WORD m_Power24;
	// ����������� ������
	WORD m_Temper;
	// ������������ �������� � ������
	WORD m_Pulse;
	// ������������� �����
	WORD* m_ResLine;
	// ������� �����
	WORD* m_CapLine;

	// ������ ���������� ������� �� ������ �����
	int* m_CountLine;
	// ���������� �������
	static int m_PultCount;

	// ���� ����� �� �����������
	bool m_fNotConnected;
	// ���� ������� 12 ����� �� � �����
	bool m_f12VNotReady;
	// ���� ������� 24 �� � �����
	bool m_f24VNotReady;
	// ���� ����������� �� � �����
	bool m_fTemperNotReady;

};




