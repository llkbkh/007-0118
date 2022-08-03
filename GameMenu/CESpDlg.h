#pragma once
#include "afxdialogex.h"


// CESpDlg 对话框

class CESpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CESpDlg)

public:
	CESpDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CESpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ESP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	cvar7_s* GetVar(char* Name);

public:
	CButton m_Check_ShowName;
	afx_msg void OnBnClickedCheckShowname();




	CButton m_Check_ShowBox;
	afx_msg void OnBnClickedCheckShowbox();
	CButton m_Check_ShowWeapon;
	afx_msg void OnBnClickedCheckShowweapon();
	CButton m_Check_ShowDistance;
	afx_msg void OnBnClickedCheckShowkm();
	CButton m_Check_ShowShine;
	afx_msg void OnBnClickedCheckShine();
	CButton m_Check_TraceMove;
	afx_msg void OnBnClickedCheckShowtracemove();
	CButton m_Check_TraceAngleLine;
	afx_msg void OnBnClickedCheckTraceangle();
	CButton m_Check_Item;
	afx_msg void OnBnClickedCheckShowitem();
	CButton m_Check_C4info;
	afx_msg void OnBnClickedCheckC4();
	CButton m_Check_PlayerStatus;
	afx_msg void OnBnClickedCheckCheckstatus();
//	CComboBox m_Check_EspTeam;
	virtual BOOL OnInitDialog();
	CComboBox m_Combo_EspTeam;
	afx_msg void OnCbnSelchangeComboEspteam();
	CButton m_Check_PlayerRadar;
	CComboBox m_Combo_Radar_pos;
	CComboBox m_Combo_RadarTeam;
	afx_msg void OnBnClickedCheckPlayerradar();
	afx_msg void OnCbnSelchangeComboRadarPos();
	afx_msg void OnCbnSelchangeComboPlayerradarteam();
	CButton m_Check_Waring;
	CComboBox m_Combo_Waring_Pos;
	CComboBox m_Combo_Waring_Team;
	afx_msg void OnBnClickedCheckWaring();
	afx_msg void OnCbnSelchangeComboWaringPos();
	afx_msg void OnCbnSelchangeComboWaringTeam();
	CButton m_Check_ShowDrone;
	afx_msg void OnBnClickedCheckShowdrone();
	CButton m_Check_SoudsRadar;
//	CComboBox m_Combo_SoudsRadar;
	afx_msg void OnBnClickedCheckSoudsradar();
	CComboBox m_Combo_SoudsRadarTeam;
	afx_msg void OnCbnSelchangeComboSoudsradar();
	CButton m_Check_Gremades_Parabola;
	afx_msg void OnBnClickedCheckParabola();
	CButton m_Check_Prophunt;
	afx_msg void OnBnClickedCheckProphunt();
	CButton m_Check_Supply;
	afx_msg void OnBnClickedCheckSupply();
	CButton m_Check_ZBtRAP;
	afx_msg void OnBnClickedCheckTrap();
//	CComboBox m_Combo_Wall_Type;
//	CComboBox m_Combo_Wall_Type;
	CButton m_Check_Wall_Hack;
	CComboBox m_Combo_Wall_Team;
	CComboBox m_Combo_Wall_Type;
	afx_msg void OnBnClickedCheckWallhack();
	CButton m_Check_OutLine;
	CComboBox m_Combo_OutLine_Team;
	afx_msg void OnBnClickedCheckOutline();
	afx_msg void OnCbnSelchangeComboWallType();
	afx_msg void OnCbnSelchangeComboWallTeam();
	afx_msg void OnCbnSelchangeComboOutlineTeam();
	CButton m_Check_Sightbead;
	CComboBox m_Combo_Sightbead_Type;
	afx_msg void OnBnClickedCheckSightbead();
	afx_msg void OnCbnSelchangeComboSightbeadType();
	CButton m_Check_NoFlash;
	CButton m_Check_NoSmoke;
	CButton m_Check_Player_Light;
	CButton m_Check_Ghost;
	CButton m_Check_Bleeding;
	afx_msg void OnBnClickedCheckBleeding();
	afx_msg void OnBnClickedCheckGhost();
	afx_msg void OnBnClickedCheckPlayerLight();
	afx_msg void OnBnClickedCheckNosmoke();
	afx_msg void OnBnClickedCheckNoflash();
	CListBox m_List_Sound;
	afx_msg void OnLbnSelchangeListSound();
	CButton m_Check_AllItem;
	afx_msg void OnBnClickedCheckAllItem();
};
