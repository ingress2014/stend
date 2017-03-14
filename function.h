#pragma once
#include "afxwin.h"

//Переводит строку в номер линии
BYTE StrToNumLine(CString NumLine);
//Переводит строку из программы в количество по 10 миллисекунд
DWORD StringToTime(CString TimeStr);
//Переводит время в 10мс интервалах в строку
CString TimeToString(DWORD TimeDword);
//Возвращает процент заряда аккумулятора
//и статус работы от сети или батареи
int PowerAcc(void);
bool IsAccPower(void);
WORD  CRC16 ( unsigned char* puchMsg, unsigned short usDataLen );

