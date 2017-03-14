#pragma once
#include "lineinfo.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "ColorButton.h"



// CLineDlg dialog
class CMFCDlg;

class CLineDlg : public CDialog
{
	DECLARE_DYNAMIC(CLineDlg)

public:
	CLineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLineDlg();

// Dialog Data
	enum { IDD = IDD_INFO_LINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CMFCDlg* m_pParent;
	CLineInfo m_Line1;
	CLineInfo m_Line2;
	CLineInfo m_Line3;
	CLineInfo m_Line4;
	CLineInfo m_Line5;
	CLineInfo m_Line6;
	CLineInfo m_Line7;
	CLineInfo m_Line8;
	CLineInfo m_Line9;
	CLineInfo m_Line10;
	CLineInfo m_Line11;
	CLineInfo m_Line12;
	CLineInfo m_Line13;
	CLineInfo m_Line14;
	CLineInfo m_Line15;
	CLineInfo m_Line16;
	CLineInfo m_Line17;
	CLineInfo m_Line18;
	CLineInfo m_Line19;
	CLineInfo m_Line20;
	CLineInfo m_Line21;
	CLineInfo m_Line22;
	CLineInfo m_Line23;
	CLineInfo m_Line24;
	CLineInfo m_Line25;
	CLineInfo m_Line26;
	CLineInfo m_Line27;
	CLineInfo m_Line28;
	CLineInfo m_Line29;
	CLineInfo m_Line30;
	CLineInfo m_Line31;
	CLineInfo m_Line32;
	CLineInfo m_Line33;
	CLineInfo m_Line34;
	CLineInfo m_Line35;
	CLineInfo m_Line36;
	CLineInfo m_Line37;
	CLineInfo m_Line38;
	CLineInfo m_Line39;
	CLineInfo m_Line40;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButtonSyncTime();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// Текущий пульт
	CStatic m_CurrentPult;
	// Протокол работы 
	CStatic m_stProtocol;
	// Установить запись протокола работы
	void SetProtocol(CString str);
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonWritePrg();
	// Прогресс-бар для операция формы настройки пульта
	CProgressCtrl m_ProgressRpt;
	afx_msg void OnBnClickedButtonCheckPrg();
	// Кнопка опроса пульта
	CColorButton m_btnScan;
	// Кнопка записи программы
	CColorButton m_btnWrite;
	// Кнопка проверки программы
	CColorButton m_btnCheck;
	// Кнопка изменения номера пульта
	CColorButton m_btnChange;
	// Кнопка синхронизации времени
	CColorButton m_SyncTime;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonChangeNumpult();
	// Кнопка изменения длительности импульса запуска
	CColorButton m_btnPulse;
	afx_msg void OnBnClickedTimeImp();
	// Пояснение к панели
	CLineInfo m_LineInfo;
	afx_msg void OnBnClickedRescan(UINT nID);

};
