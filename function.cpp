#include "stdafx.h"
#include "function.h"
#include "PultConst.h"

#define ON_BATTERY 0
//Перевод буквы мотора в номер линии
BYTE StrToNumLine(CString NumLine)
{
	if (NumLine.Compare(_T("A")) == 0)
		return 33;
	if (NumLine.Compare(_T("B")) == 0) 
		return 34;
	if (NumLine.Compare(_T("C")) == 0) 
		return 35;
	if (NumLine.Compare(_T("D")) == 0) 
		return 36;
	if (NumLine.Compare(_T("E")) == 0) 
		return 37;
	if (NumLine.Compare(_T("F")) == 0) 
		return 38;
	if (NumLine.Compare(_T("G")) == 0) 
		return 39;
	if (NumLine.Compare(_T("H")) == 0) 
		return 40;
	return 
		StrToInt(NumLine);
}
//Переводит строку из программы в количество по 10 миллисекунд
DWORD StringToTime(CString TimeStr)
{
CString tempString;
DWORD hh,mm,ss,ms;

tempString = TimeStr.Mid(0,1);
hh = StrToInt(tempString);

tempString = TimeStr.Mid(2,2);
mm = StrToInt(tempString);

tempString = TimeStr.Mid(5,2);
ss = StrToInt(tempString);

// Десятки мс
tempString = TimeStr.Mid(8,2);
ms = StrToInt(tempString);

return (ms + ss*100 + mm*60*100 + hh*60*60*100);

}

CString TimeToString(DWORD TimeDword)
{
	//часы
	BYTE h = static_cast<BYTE>(TimeDword/360000);
	//минуты
	BYTE m = static_cast<BYTE>((TimeDword%360000)/6000);
	//секунды
	BYTE s = static_cast<BYTE>(((TimeDword%36000)%6000)/100);
	//десятки мс
	BYTE z = static_cast<BYTE>(((TimeDword%36000)%6000)%100);
	CString tmp;
	tmp.Format(_T("%01d:%02d:%02d.%02d"),h,m,s,z);
	return tmp;

}

// Процент заряда батареи
int PowerAcc(void)
{
	SYSTEM_POWER_STATUS sps;
	if (GetSystemPowerStatus(&sps))
	{
		return (int)sps.BatteryLifePercent;
	} 
	else
	{
		return 0;
	}
}
//Функция определения питания сеть/батарея
bool IsAccPower(void)
{
	SYSTEM_POWER_STATUS PowerStatus;
	GetSystemPowerStatus(&PowerStatus);
	return PowerStatus.ACLineStatus != ON_BATTERY;

}
//Функция вычисления контрольной суммы
//unsigned char *puchMsg ; /* message to calculate CRC upon */
//unsigned short usDataLen ; /* quantity of bytes in message */

/* Функция возвращает CRC WORD */
WORD  CRC16 ( BYTE* puchMsg, WORD usDataLen )
{
	BYTE uchCRCHi = 0xFF ; /* Старший байт инициализации */
	BYTE uchCRCLo = 0xFF ; /* Младший байт инициализации */
	unsigned uIndex ; /* Индекс таблицы CRC */

	while (usDataLen--) /* Шаг по буферу */
	{
		uIndex = uchCRCLo ^ *puchMsg++ ; /* вычисление CRC */
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex] ;
		uchCRCHi = auchCRCLo[uIndex] ;
	}
	return (uchCRCLo << 8 | uchCRCHi) ;
}
