#pragma once
#include "rs485.h"



class CPult
{
public:
	CPult(void);
	//Конструктор копирования
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
	
	// Серийный номер пульта
	BYTE m_SerNumPult;
	// Номер пульта
	BYTE m_NumPult;
	// Байт состояния режима работы пульта
	BYTE m_WorkMode;
	// Питание 12 вольт
	WORD m_Power12;
	// Питание 24 вольта
	WORD m_Power24;
	// Температура пульта
	WORD m_Temper;
	// Длительность импульса в пульте
	WORD m_Pulse;
	// Сопротивление линий
	WORD* m_ResLine;
	// Емкость линий
	WORD* m_CapLine;

	// Массив количества выходов на каждой линии
	int* m_CountLine;
	// Количество пультов
	static int m_PultCount;

	// Флаг пульт не подсоединен
	bool m_fNotConnected;
	// Флаг питание 12 вольт не в норме
	bool m_f12VNotReady;
	// Флаг питание 24 не в норме
	bool m_f24VNotReady;
	// Флаг температура не в норме
	bool m_fTemperNotReady;

};




