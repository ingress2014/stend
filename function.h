#pragma once
#include "afxwin.h"

//��������� ������ � ����� �����
BYTE StrToNumLine(CString NumLine);
//��������� ������ �� ��������� � ���������� �� 10 �����������
DWORD StringToTime(CString TimeStr);
//��������� ����� � 10�� ���������� � ������
CString TimeToString(DWORD TimeDword);
//���������� ������� ������ ������������
//� ������ ������ �� ���� ��� �������
int PowerAcc(void);
bool IsAccPower(void);
WORD  CRC16 ( unsigned char* puchMsg, unsigned short usDataLen );

