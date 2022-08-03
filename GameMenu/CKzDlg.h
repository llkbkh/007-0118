#pragma once
#include "afxdialogex.h"


// CKzDlg 对话框

class CKzDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKzDlg)

public:
	CKzDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CKzDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_KZ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_278;
	CButton m_Check_SpinHack;
	CButton m_Check_JumpBug;
	CButton m_Check_KzInfo;
	CButton m_Check_BunnyHop;
	CButton m_Check_AutoStrafe;
	CButton m_Check_Mcj;
	CComboBox m_Combo_McjKey;
	afx_msg void OnBnClickedCheckKz278();
	afx_msg void OnBnClickedCheckKzSpinhack();
	afx_msg void OnBnClickedCheckKzJumpbug();
	afx_msg void OnBnClickedCheckKzInfo();
	afx_msg void OnBnClickedCheckKzHop();
	afx_msg void OnBnClickedCheckKzAutoStrafe();
	afx_msg void OnBnClickedCheckMcj();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboKzMcjKey();
	CListBox m_List_ZbsAutoGank;
	CButton m_Check_ZbsAutoGank;
	CListBox m_List_ZbsAutoCounter;
	afx_msg void OnBnClickedCheckZbsGank();
	afx_msg void OnLbnSelchangeListZbsAutoGank();
	CComboBox m_Combo_CounterMode;
	afx_msg void OnBnClickedCheckZbsCounter();
	CButton m_Check_ZbsCounter;
	afx_msg void OnCbnSelchangeComboCounterMode();
	afx_msg void OnLbnSelchangeListZbsAutoCounter();
	CButton m_Check_ZBS_AutoSkill;
	afx_msg void OnBnClickedCheckZbsSAutoSkill();
	CComboBox m_Combo_GameSpeed;
	CButton m_Check_SpeedEnable;
	afx_msg void OnCbnSelchangeComboGameSpeed();
	CComboBox m_Combo_GameSpeedKey;
	afx_msg void OnBnClickedCheckSpeedEnable();
	afx_msg void OnEnChangeEditSpeedValue();
	CString m_Edit_SpeedValue;
	afx_msg void OnCbnSelchangeComboGameKey();
	CString m_Edit_SpeedReConnectTime;
	CButton m_Check_SpeedReConnectEnable;
	afx_msg void OnBnClickedCheckSpeedRecon();
	afx_msg void OnEnChangeEditSpeedReconnectValue();
	CButton m_Check_KZMode;
	CComboBox m_Combo_KZMode;
	afx_msg void OnCbnSelchangeComboKzMode();
};
