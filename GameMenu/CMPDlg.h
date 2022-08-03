#pragma once
#include "afxdialogex.h"


// CMPDlg 对话框

class CMPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMPDlg)

public:
	CMPDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMPDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckMpMode();
	CButton m_Check_ModeMp;
	CComboBox m_Combo_Map;
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonCutmap();



private:
	std::string m_MapCode;
	std::map<int, PGAME_MAPINFO>* m_pGameMap;
	std::map<int, std::wstring>* m_pGameMode;


public:
	afx_msg void OnCbnSelchangeComboMap();
	CComboBox m_Combo_GameMode;
	CButton m_Check_GameMode;
	afx_msg void OnBnClickedCheckMpMap();
	CButton m_Check_Map;
	afx_msg void OnCbnSelchangeComboGamemode();
	afx_msg void OnBnClickedCheckMpGamemode();

	CButton m_Check_GainMut;
	afx_msg void OnBnClickedCheckMpGainmut();
	CButton m_Check_Health;
	afx_msg void OnBnClickedCheckMpHp();
	CButton m_Check_God;
	CButton m_Check_Speed;
	CButton m_Check_AmmpMax;
	afx_msg void OnBnClickedCheckMpGod();
	afx_msg void OnBnClickedCheckMpSpeed();
	afx_msg void OnBnClickedCheckMpAmmo();
	CButton m_Check_AmmoSpeed;
	afx_msg void OnBnClickedCheckMpAmmospeed();
	CButton m_Check_Recoil;
	afx_msg void OnBnClickedCheckMpRecoil();
	CButton m_Check_hHghtJump;
	afx_msg void OnBnClickedCheckMpJump();
	CButton m_Check_Soul;
	afx_msg void OnBnClickedCheckMpSoul();
	CHotKeyCtrl m_HotKey_Soul;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CHotKeyCtrl m_HotKey_Sop;
	CButton m_Check_Sop;
	afx_msg void OnBnClickedCheckMpSop();
	CButton m_Check_Rpn;
	CHotKeyCtrl m_HotKey_Rpn;
	afx_msg void OnBnClickedCheckMpRpn();
};
