
// MFCDlg.h : ���� ���������
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


//������� ���������� ���������
#define END_PROGRAM				WM_APP+1
//������� �������
#define TIMER_MAIN				WM_APP+2
//������ �������� ����� � ������
#define TIMER_WAIT_START		WM_APP+3
//������� ������� �������� ������ �������
#define TIMER_ECHO				WM_APP+4
//������� ����� ���������
#define	PAUSE_PROGRAM			WM_APP+5
//������� ����������� ������ ���������
#define	START_PROGRAM			WM_APP+6








typedef struct _LinePrg{
	int NumPP; //���������� �����
	DWORD TimePusk; //����� ����� � 10 �� ����������
	BYTE NumPult; // ����� ������
	BYTE NumLine; // ����� ����� �� ������
	CString Description; //�������� �������
} LinePrg, *pLinePrg;

// ���������� ���� CMFCDlg
class CMFCDlg : public CDialog
{
// ��������
public:
	CMFCDlg(CWnd* pParent = NULL);	// ����������� �����������

	#define COLUMNS_m_ColListCtrl 5

	//����������� ������ ��� ���������� ������� ������ ������
	CCriticalSection cs;

	// ������ ����������� ����
	enum { IDD = IDD_MFC_DIALOG };

// ����������
protected:
	HICON m_hIcon;
	//������������ ����������� - ���������� RS485 
	HDEVNOTIFY NotificationHandle;
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV
	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnDeviceChange(UINT EventType,DWORD dwData);
	DECLARE_MESSAGE_MAP()

protected:
	// ������� ������������� ��� �����
	HACCEL hAccel;
	// ����� � ��������� ���������
	CFont m_FontPrg;
	LRESULT OnPowerChange(WPARAM, LPARAM);
private:
	//������������� ����������
	CLibMP3DLL m_Player;
	//��������� ������������
	bool playing;
	//��������� ����� 
	bool pause;
//**************************************************
//             ������
//**************************************************
	// ������ ��������� ������
	CColorButton m_btnTest;
	// ������ �������� ������
	CColorButton m_btnWork;
	// ������ ���������� � ����������
	CColorButton m_btnComp;
	// ������ ���������� � ������
	CColorButton m_btnPult;
	// ������ ������ ���������
	CColorButton m_btnStart;
	// ������ ��������� ���������
	CColorButton m_btnStop;
	// ������ � ������ ���������
	CColorButton m_btnBegin;
	// ������ ������ ���������
	CColorButton m_btnWrite;
	// ������ �������� ���������
	CColorButton m_btnCheck;
	// ������ ��������������� RS-485
	CColorButton m_btnRS485;
	// ������ ������ ���������
	CColorButton m_btnPrg;
	// ������ ������ ����������
	CColorButton m_btnSound;
	// ������ ��������� � ������
	CColorButton m_btnPrgInPult;
	// ������ ��������� � ����������
	CColorButton m_btnPrgInComp;
	
//*******************************************
//    �������
//*******************************************
	// ���������� ������ ������� � ���������
	CProgressCtrl m_ProgressPower;
	// ������� �� ���� ��� �������
	CString m_AccPower;
	// ��������� ���� �������� ������ ������������
	CString m_AccPercent;
	// ���������� ��������� �������
	void RefreshPower(void);
public:
	// ������ �������
	CArray <CPult,CPult&> m_PultArray;

	// ������ ��������
	CArray <CPrg,CPrg&> m_PrgArray;




//**************************************************
//����� ������ ������ ���������
//**************************************************
	// ����, ��������� ��������� � ����������
	BOOL m_PrgInComp;
	//���� ����� ���������
	bool m_bPause;
	// ���� �������� ����������
	bool m_SoundLoaded;
	// ���� �������� ���������
	bool m_PrgLoaded;
	// ���� �������� ������ (������� ������ ������� � �� ������) 
	BOOL m_WorkReg;
	// ������ ���������� � ���������� 
	bool m_CompControl;
	// ���� ������� ������ ������
	bool m_BlinkWork;
	// ���� ����, ��� ������ ������ �������� �������
	bool m_WorkGreen;





	// ���� ������������� �������� ������
	volatile BOOL m_WorkRunning;
	// ���� ������������� 
	// ������ �������� ������
	volatile BOOL m_WaitStart;
	// ������ ������ �������
	volatile BOOL m_FindStart;

	// ���� ����������� �������� ������
	volatile BOOL fContinue;
	// ���� ����������� ������ �������� ������ ��������� 
	volatile BOOL fContinueWaitStart;


	//������ ���������
	CListBox m_ListPrg;
	// ������ ������������ �������
	CColorListCtrl m_PultList;
	// ������� ��� ���������
	CString m_PrgFileName;
	// ������� ��� ����� ����������
	CString m_SoundFileName;
	afx_msg void OnDestroy();

	//������� ����� ���������� ���������
	afx_msg LRESULT OnEndProgram(WPARAM wParam, LPARAM lParam);
	// ������� �������� �� �����
	afx_msg LRESULT OnPauseProgram(WPARAM wParam, LPARAM lParam);
	//������� ������ ��������� � ������
	afx_msg LRESULT OnStartFromPult(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedLoadPrg();
	afx_msg void OnBnClickedLoadMp3();
	afx_msg void OnBnClickedTestReg();
	afx_msg void OnBnClickedWorkReg();
	afx_msg void OnBnClickedCompReg();
	afx_msg void OnBnClickedPultReg();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPreference();


	// �������� ��������� ������� � �����
	void SendGrpCmdToPult(DWORD dwData);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// ��������������� RS485
	CRS485* m_DevRS485;
	// �������� ������ ������� ���������
	CKey* m_Key;
	// ������ ���������
	CColorButton m_btnHand;
	afx_msg void OnBnClickedHandBtn();
	// �������� ������ ���������
	BOOL m_HandBtnValue;
	afx_msg void OnBnClickedStop();
	// ������ ������ �������������� �������
	CColorButton m_btnFind;
	afx_msg void OnBnClickedFind();
	// ������� ���������� ������ � ������
	void AddPult(BYTE SerNumPult);
	// ���������� ������ ���������� 
	CString m_Protocol;

//*************************************************
//     ������� 
//*************************************************
	// ������������ ������ ������� ������
	CCPUTicker m_TimerWork;


	//�������� ������
	UINT m_Timer;
	// ������ �������� ������ �������
	UINT m_TimerEcho;
	// ������ �������� ����� ���������
	UINT m_TimerWaitStart;


//*************************************************
//              ������
//*************************************************
	// �������� ����� ���������� ���������
	CWinThread* pWorkThread;
	// ����� �������� ������ � ���������� ������
	CWinThread* pWaitThread;
	// ����� ������ �������
	CWinThread* pFindThread;

	// ������ ����� ���������
	CList <LinePrg,LinePrg> m_ListLine;
	// ������� ������� � ���������
	POSITION m_posCurrentPosPrg;
	// ��������� ������� ������ � ���������
	int m_intCurrentPosPrg;
	// ���� ������ ��������� �������
	volatile BOOL m_TimerRun;
	// ������� ������� ������ ���������
	HANDLE m_hEventWork;
	//�������� ������� � ������ ������ � ��������
	volatile double m_StartTime;
	//�������� ������� � ������ ���������
	volatile double m_StopTime;
	//�������� ������� �������� �����
	volatile DWORD m_WaitTime;
	

	// ������� ��������� �����
	int m_CurrentPult;
	// ����� ���������� ������
	CString m_PultInfo;
	afx_msg void OnLvnItemchangedListPult(NMHDR *pNMHDR, LRESULT *pResult);
	// ������� �� ��������� ����� ����� � ������
	void SendSetLineToPult(BYTE NumPult, BYTE NumLine);
	afx_msg void OnBnClickedWrite();
	afx_msg void OnBnClickedBegin();
	afx_msg void OnBnClickedCheck();
	//������ ��������� � ����� (����������, ���������)
	BOOL WritePrgToPult(BYTE NumPult);
	// ������ ����� ��������� � �����
	BOOL WritePrg(BYTE NumPult);
	// ������ � ����� ���������� � ��������� 
	BOOL WriteInfo(BYTE NumPult);
	// ���������� ����� ������ �� ��������� ������
	BYTE GetNumPult(BYTE SerNumPult);
	// ����������� ����������� ���������� ��������� 
	CProgressCtrl m_ProgressRpt;
//*********************************************************************
// ���� ������� � ���������
//*********************************************************************
	// ����������� ������� ��������� �������
	CTablo m_MainTimer;
	// ���������� ���������� �������
	void RefreshTimer(void);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// ��������� ������, �� ������ ������ ���������
	BOOL m_PrefBtnValue;
	// �������� ���������� ���������
	CSliderCtrl m_Delay;
	// ����� �������� ���������
	CString m_DelayTxt;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// ������ ���������
	CColorButton m_PrefBtn;
	afx_msg void OnBnClickedButtonDebug();
	CDebugDlg* m_dlgDebug;
	CLineDlg* m_LineDlg;
	// �������� ������
	BOOL CheckPrg(BYTE NumPult);
	// ���������� ��������� �� ������ �� ���������� ����
	BOOL WritePrgToFlash(BYTE SerNumPult);
	// ������� ���������� ����� � ��������� ��� ������
	int CountLineInPult(BYTE NumPult);
	// ������ ������� �� ����� ���������
	CTablo m_EndPrg;
	// ������ ������� �� ��������� ����������
	CTablo m_EndSound;
	//������������ ���������
	DWORD m_TimePrg;
	//������������ ����������
	DWORD m_TimeSound;
	// �������� ����� ������
	void ScanPult(int NumPult);
	// �������� ����� ����� ������
	void ScanPult(int NumPult, BYTE NumLine);
	virtual void OnOK();
	virtual void OnCancel();
	// ����� ����� ������� �������
	BOOL FindDouble(void);
	// ��������� ������ �������� ������
	BOOL ChangeNumPult(BYTE NewNumPult);
	// ������ ���������
	CMsgDlg* m_MsgDlg;
	//������ ����������� �������
	CDelayStart* m_DelayStart;
	// ��������� ������������ �������� �������
	BOOL SetPulse(int wPulse);
	// ����� ����� � �������
	void CheckPult(void);
	// ���������, ���������� ������ 
	void EnableBtn(BOOL EN_DIS);
	afx_msg void OnBnClickedPrgInPult();
	afx_msg void OnBnClickedPrgInComp();
	// ������� ���������� ��������� �� �����
	int CountLineInPult(BYTE NumPult, BYTE NumLine);
	// ���� ������� �� ������� ���������
	CReport* m_log;
	// ��������� ����� � ������
	void SendClearLineToPult(BYTE NumPult, BYTE NumLine);
	// ������ ������������ �������� �������� ������
	int GetPulse();
	// ������������ �� ����� � ������ �������
	bool PultIsPresent(int NumPult);
	// �������� ������� ������, ��������������� � ���������
	CString CheckPultInPrg();
	// ������� �� ������ ������ ��� F1
	CColorButton m_btnWorkF1;
	afx_msg void OnBnClickedWork();
	// ������ ������� �������� ������
	CColorButton m_btnStepReg;
	afx_msg void OnBnClickedStepReg();
	// ���������� ����� ������
	bool SafeEcho(int NumPultInList);
	// ������� � ��������� ���� ������� � ��������������
	int PauseAllPult(void);
	// ������ ����� �������� ������� � ������
	void ShowWaitStart(void);
	// ���� �������� ������� �� ��������� �����
	bool m_WaitFromClock;
	// ����� ������ �� ����������
	COleDateTime m_TimeStart;

	// ������� ������ ���������
	CColorButton m_Prg1;
	// ������� ������ ���������
	CColorButton m_Prg2;
	// ������� 3 ���������
	CColorButton m_Prg3;
	CColorButton m_Prg4;
	CColorButton m_Prg5;
	CColorButton m_Prg6;
	CColorButton m_Prg7;
	CColorButton m_Prg8;
	// ������� ���������
	int m_CurrentPrg;
	afx_msg void OnBnClickedPrg(UINT nID);
	// ������������ �� ����� � �������� ������� � ������ ������� 
	bool PultInList(BYTE SerNumPult);
	// �������� �������� ������ ������ 
	CStatic m_imgWork;
};
