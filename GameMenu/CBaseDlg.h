#pragma once
#include "afxdialogex.h"


// CBaseDlg 对话框

class CBaseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBaseDlg)

public:
	CBaseDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBaseDlg();
	
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BASE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_Rpn;
	CHotKeyCtrl m_HotKey_Rpn;
	CButton m_Check_ReRoom;
	CHotKeyCtrl m_HotKey_ReRoom;
	CButton m_Check_RetRoom;
	CHotKeyCtrl m_HotKey_RetRoom;
	CButton m_Check_NoFog;
	CHotKeyCtrl m_HotKey_NoFog;
	CButton m_Check_NoGameMenu;
	CHotKeyCtrl m_HotKey_NoGameMenu;
	CButton m_Check_PlayerCardLag;
	CHotKeyCtrl m_HotKey_PlayerCardLag;
	CButton m_Check_ZbsBuy;
	CHotKeyCtrl m_HotKey_ZbsBuy;
	CButton m_Check_ThirdPerson;
	CHotKeyCtrl m_HotKey_ThirdPerson;
	CButton m_Check_ExitRoom;
	CHotKeyCtrl m_HotKey_ExitRoom;
	CButton m_Check_ZbsBuy_Drone;
	CHotKeyCtrl m_HotKey_ZbsBuy_Drone;
	CButton m_Check_Nvg;
	CButton m_Check_MapLig;
	virtual BOOL OnInitDialog();
	CListBox m_List_ShowHp;
	CButton m_Check_ShowHp;
	afx_msg void OnBnClickedCheckShowhp();
	afx_msg void OnLbnSelchangeListShowhp();
	afx_msg void OnBnClickedCheckRpn();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckReroom();
	afx_msg void OnBnClickedCheckRetroom();
	afx_msg void OnBnClickedCheckNofog();
	afx_msg void OnBnClickedCheckNogamemenu();
	afx_msg void OnBnClickedCheckCardlag();
	afx_msg void OnBnClickedCheckZbsbuy();
	afx_msg void OnBnClickedCheckThper();
	afx_msg void OnBnClickedCheckExitroom();
	afx_msg void OnBnClickedCheckZbsbuyDrone();
	afx_msg void OnBnClickedCheckNvg();
	CButton m_Check_NoChat;
	afx_msg void OnBnClickedCheckNochat();
	CButton m_Check_NoNottery;
	afx_msg void OnBnClickedCheckNolotter();
	CButton m_Check_NoAllMsg;
	afx_msg void OnBnClickedCheckNoallmsg();
	afx_msg void OnBnClickedCheckMpalig();
	CButton m_Check_Kick;
	afx_msg void OnBnClickedCheckKick();
	afx_msg void OnBnClickedCheckKnife();
	CComboBox m_Combo_knifeList;
	CHotKeyCtrl m_HotKey_Knfie;
	CButton m_Check_Knfie;

	BOOL SetDlgItemTextC(int nID, CString Str);

	afx_msg void OnCbnSelchangeComboKnife();
	CButton m_Check_SupSeo;
	CButton m_Check_FramSeo;
	afx_msg void OnBnClickedCheckSupseo();
	afx_msg void OnBnClickedCheckFramseo();
	CButton m_Check_AlterTeam;
	CComboBox m_Combo_AlterTeam;
	afx_msg void OnBnClickedCheckAlterteam();
	afx_msg void OnCbnSelchangeComboTeam();
	CButton m_Check_ProgressUnLock;
	afx_msg void OnBnClickedCheckProgressUnlock();
	CButton m_Check_AntiRoomKick;
	afx_msg void OnBnClickedCheckAntiRoomkick();
	CButton m_Check_Font;
	afx_msg void OnBnClickedCheckFont();
	afx_msg void OnBnClickedCheckFlying();
	CButton m_Check_Flying;
	CHotKeyCtrl m_HotKey_Flying;
	CButton m_Check_Flying2;
	CHotKeyCtrl m_HotKey_Flying2;
	afx_msg void OnBnClickedCheckFly2();
	CButton m_Check_AlterRole;
	afx_msg void OnBnClickedCheckAlterrule();
	CButton m_Check_AlterSpray;
	CButton m_Check_AlterWeapon;
	afx_msg void OnBnClickedCheckAlterspray();
	afx_msg void OnBnClickedCheckAlterweapon();
//	CComboBox m_Check_zbRespawnEffect;

	CComboBox m_Combo_zbRespawnEffect;
	afx_msg void OnBnClickedCheckZsbrpn();
	CButton m_Check_zbRespawnEffect;
	afx_msg void OnCbnSelchangeComboZbsrpnoffset();
//	afx_msg void OnBnClickedCheckNvg13();
	CButton m_Check_Quest;
	afx_msg void OnBnClickedCheckQuest();
	CButton m_Check_DefaultConsole;
	afx_msg void OnBnClickedCheckDefaultconsole();
	CButton m_Check_BadWord;
	CEdit m_Edit_Chat;
	CButton m_Check_Chat;
	afx_msg void OnBnClickedButtonCommand();
//	CEdit m_Edit_Command;
	CString m_Edit_Command;
	CHotKeyCtrl m_HotKey_Destroy;
	afx_msg void OnBnClickedCheckVxlDestroy();
	CButton m_Check_Destroy;
	CButton m_Check_VxlWeather;
	CButton m_Check_VxlSky;
	CButton m_Check_Vxl_Light;
	afx_msg void OnBnClickedCheckVxlWeather();
	afx_msg void OnBnClickedCheckVxlSky();
	afx_msg void OnBnClickedCheckVxlLight();
	CButton m_Check_ZbsKick;
	afx_msg void OnBnClickedCheckZbskick();
	CButton m_Check_NetStatus;
	afx_msg void OnBnClickedCheckSetNetStatus();
	CButton m_Check_Vxl_restart;
	afx_msg void OnBnClickedCheckVxlRestart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CHotKeyCtrl m_HotKey_VxlRestart;
	afx_msg void OnBnClickedCheckChat();

	static void CALLBACK TimerProcChat(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);

	CButton m_Check_AllKick;
	afx_msg void OnBnClickedCheckAllkick();
	CButton m_Check_ObsTp;
	afx_msg void OnBnClickedCheckObstp();
};
