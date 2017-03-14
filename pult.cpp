#include "StdAfx.h"
#include "pult.h"
#include "PultConst.h"

int CPult::m_PultCount=0;

CPult::CPult(void)
	: m_SerNumPult(0)
	, m_NumPult(0)
	, m_WorkMode(0)
	, m_Pulse(0)
	, m_ResLine(nullptr)
	, m_CapLine(nullptr)
	, m_CountLine(nullptr)
	, m_fNotConnected(false)
	, m_f12VNotReady(false)
	, m_f24VNotReady(false)
	, m_fTemperNotReady(false)
{
	//Выделяем память под массивы сопротивлений, емкостей и выходов линий

	m_ResLine = new WORD[MAX_LINE];
	m_CapLine = new WORD[MAX_LINE];
	m_CountLine = new int[MAX_LINE];
	m_PultCount++;
}

//Конструктор копирования
CPult::CPult(const CPult &pult)
{
	m_SerNumPult = pult.m_SerNumPult;
	m_NumPult = pult.m_NumPult;
	m_Power12 = pult.m_Power12;
	m_Power24 = pult.m_Power24;
	m_Temper = pult.m_Temper;
	m_WorkMode = pult.m_WorkMode;
	m_Pulse = pult.m_Pulse;

	m_ResLine = new WORD[MAX_LINE];
	m_CapLine = new WORD[MAX_LINE];
	m_CountLine = new int[MAX_LINE];

	memcpy(m_ResLine,pult.m_ResLine,sizeof(WORD)*MAX_LINE);
	memcpy(m_CapLine,pult.m_CapLine,sizeof(WORD)*MAX_LINE);
	memcpy(m_CountLine,pult.m_CountLine,sizeof(int)*MAX_LINE);
	m_PultCount++;
}





CPult::~CPult(void)
{
	delete  [] m_ResLine;
	delete  [] m_CapLine;
	delete  [] m_CountLine;
	m_ResLine = m_CapLine = nullptr;
	m_CountLine = nullptr;
	m_PultCount--;
}

