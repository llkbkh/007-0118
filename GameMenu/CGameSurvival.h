#pragma once
#include "afxdialogex.h"


// CGameSurvival 对话框

class CGameSurvival : public CDialogEx
{
	DECLARE_DYNAMIC(CGameSurvival)

public:
	CGameSurvival(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameSurvival();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SURVIVAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CHotKeyCtrl m_HotKey_BuyHp;
	CButton m_Check_BuyHp;
	afx_msg void OnBnClickedCheckBuyhp();
	CButton m_Check_BuyArmor;
	CButton m_Check_BuyAtk;
	CButton m_Check_BuyBullet;
	CButton m_Check_BuyBullet2;
	afx_msg void OnBnClickedCheckArmor();
	afx_msg void OnBnClickedCheckAtk();
	afx_msg void OnBnClickedCheckBullet();
	afx_msg void OnBnClickedCheckBullet2();
	CHotKeyCtrl m_HotKey_BuyBullet2;
	CHotKeyCtrl m_HotKey_BuyBullet;
	CHotKeyCtrl m_HotKey_BuyAtk;
	CHotKeyCtrl m_HotKey_BuyArmor;
	CComboBox m_Combo_ZsGrenade;
	CButton m_Check_BuyGrenade;
	virtual BOOL OnInitDialog();
	CHotKeyCtrl m_HotKey_ZsBuyGrenade;
	afx_msg void OnBnClickedCheckBuygrenade();
	afx_msg void OnCbnSelchangeComboZsGrenade();
	CButton m_Check_AutoBuyBullet;
	afx_msg void OnBnClickedCheckAutoBuyBullet();
	CButton m_Check_AutoBuyArmor;
	afx_msg void OnBnClickedCheckAutoBuyArmor();
	CButton m_Check_AutoBuyAtk;
	afx_msg void OnBnClickedCheckAutoBuyAtk();
	CButton m_Check_AutoDrug30;
	CButton m_Check_AutoDrug100;
	afx_msg void OnBnClickedCheckAutoDrug30();
	afx_msg void OnBnClickedCheckAutoDrug100();
	CButton m_Check_ZS_AFK;
	afx_msg void OnBnClickedCheckAfk();
	CComboBox m_Combo_ZsRole;
	CButton m_Check_ZsAutoStartGame;
	afx_msg void OnBnClickedCheckRole();
	CButton m_Check_ZsRole;
	afx_msg void OnCbnSelchangeComboZsRole();
	afx_msg void OnBnClickedCheckAutoStartGame();
};
