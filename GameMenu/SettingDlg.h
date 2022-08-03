#pragma once
#include "afxdialogex.h"



// SettingDlg 对话框

class CSettingDlg : public CDialogEx 
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSettingDlg();


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()



public:
	virtual BOOL OnInitDialog();
	BOOL SetDlgItemTextC(int nID, CString Str);
	afx_msg void OnBnClickedStaticCfg();
	CButton m_EnalbeMenuTips;
	afx_msg void OnBnClickedCheckShowmenutip();
//	CHotKeyCtrl m_MenuHotkey;

	afx_msg void OnBnClickedButtonLoadcfg();
	afx_msg void OnBnClickedButtonSavacfg();
	afx_msg void OnBnClickedButtonDefaultcfg();


	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CButton m_Check_BaseFun;

	CButton m_Check_EspFun;
	CButton m_Check_AimFun;
	CButton m_Check_WallFun;
	CButton m_Check_KZFun;
	CButton m_Check_ScriptFun;
	CButton m_Check_MpFun;
	afx_msg void OnBnClickedCheckBasefun();
	CHotKeyCtrl m_HotKey_Menu;
	afx_msg void OnBnClickedCheckEspfun();
	afx_msg void OnBnClickedCheckAimfun();
	afx_msg void OnBnClickedCheckWallfun();
	afx_msg void OnBnClickedCheckKzfun();
	CButton m_Check_Survival;
	afx_msg void OnBnClickedCheckZsfun();
	CButton m_Check_Soccer;
	CButton m_Check_DeathBox;
	CButton m_Check_KillIco;
	afx_msg void OnBnClickedCheckKillico();
	afx_msg void OnBnClickedCheckDeathbox();
	afx_msg void OnBnClickedCheckSoccer();
	CButton m_Check_GameFps;
	CString m_Edit_GameFps;
	afx_msg void OnBnClickedCheckGameFps();
	afx_msg void OnEnChangeEditGameFps();
	CButton m_Check_ActvateWindows;
	afx_msg void OnBnClickedCheckActivateWindow();
	afx_msg void OnBnClickedCheckNight();
	CButton m_Check_Zsht;
	afx_msg void OnBnClickedCheckMpfun();
	CButton m_Check_Like;
	afx_msg void OnBnClickedCheckLike();
};
