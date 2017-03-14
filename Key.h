#pragma once
#include "handbtn.h"
#include "afxwin.h"



// CKey dialog

class CMFCDlg;

class CKey : public CDialog
{
	DECLARE_DYNAMIC(CKey)

public:
	CKey(CWnd* pParent=NULL);   // standard constructor
	virtual ~CKey();
	CMFCDlg* m_pParent;
// Dialog Data
	enum { IDD = IDD_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CHandBtn m_key1;
	CHandBtn m_key2;
	CHandBtn m_key3;
	CHandBtn m_key4;
	CHandBtn m_key5;
	CHandBtn m_key6;
	CHandBtn m_key7;
	CHandBtn m_key8;
	CHandBtn m_key9;
	CHandBtn m_key10;
	CHandBtn m_key11;
	CHandBtn m_key12;
	CHandBtn m_key13;
	CHandBtn m_key14;
	CHandBtn m_key15;
	CHandBtn m_key16;
	CHandBtn m_key17;
	CHandBtn m_key18;
	CHandBtn m_key19;
	CHandBtn m_key20;
	CHandBtn m_key21;
	CHandBtn m_key22;
	CHandBtn m_key23;
	CHandBtn m_key24;
	CHandBtn m_key25;
	CHandBtn m_key26;
	CHandBtn m_key27;
	CHandBtn m_key28;
	CHandBtn m_key29;
	CHandBtn m_key30;
	CHandBtn m_key31;
	CHandBtn m_key32;
	CHandBtn m_key33;
	CHandBtn m_key34;
	CHandBtn m_key35;
	CHandBtn m_key36;
	CHandBtn m_key37;
	CHandBtn m_key38;
	CHandBtn m_key39;
	CHandBtn m_key40;
	afx_msg void OnBnClickedGroup();
	afx_msg void OnBnClickedResetGroup();
	// Режим группировки
	BOOL m_Group;
	afx_msg void OnBnClickedGroupPusk();
	// Кнопка запуска группы
	CButton m_GroupPusk;
	// Кнопка управления группировкой
	CButton m_btnGroup;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedStep();
	// Вариант1 группировки кнопок
	CButton m_stGrp1;
	// Вариант 2 группировки кнопок
	CButton m_stGrp2;
	// Вариант3 группировки кнопок
	CButton m_stGrp3;
	// Значение первой группировки кнопок
	DWORD m_SaveGrp1;
	// Вариант 2 группировки кнопок
	DWORD m_SaveGrp2;
	// Вариант 3 группировки кнопок
	DWORD m_SaveGrp3;
	afx_msg void OnBnClickedGrpSave1();
	void OnBnClickedGrpSave(int NumGrp);
	afx_msg void OnBnClickedGrpSave2();
	afx_msg void OnBnClickedGrpSave3();
};
