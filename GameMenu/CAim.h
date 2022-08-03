#pragma once
#include "afxdialogex.h"


// CAim 对话框

class CAim : public CDialogEx
{
	DECLARE_DYNAMIC(CAim)

public:
	CAim(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAim();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AIM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_Aim_Key;
	afx_msg void OnBnClickedCheckAimKey();
	afx_msg void OnCbnSelchangeComboAimKey();
	CComboBox m_Combo_Aim_Key;
	virtual BOOL OnInitDialog();
	CComboBox m_Combo_Aim_Team;
	CComboBox m_Combo_Aim_Pos;
	CComboBox m_Combo_Aim_Type;
	CComboBox m_Combo_Aim_Mode;
//	CEdit m_Edit_Aim_Angle;
	afx_msg void OnCbnSelchangeComboAimType();
	afx_msg void OnCbnSelchangeComboAimTeam();
	afx_msg void OnCbnSelchangeComboAimPos();
	afx_msg void OnCbnSelchangeComboAimMode();
	afx_msg void OnEnChangeEditAimAngle();
	CString m_Edit_Aim_Angle;
	CString m_Edit_Aim_Pitch;
	afx_msg void OnEnChangeEditAimPitch();
	CString m_Edit_Aim_Yaw;
	afx_msg void OnEnChangeEditAimYaw();
	CComboBox m_Combo_Aim_RecoilMode;
	afx_msg void OnCbnSelchangeComboAimRecoilmode();
	CButton m_Check_Aim_NoRecoil;
	CButton m_Check_NoRecoil_Visual;
	afx_msg void OnBnClickedCheckAimNorecoil();
	afx_msg void OnBnClickedCheckAimNorecoilVl();
	CSliderCtrl m_Slider;
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	CComboBox m_Combo_Aim_MouseType;
	CComboBox m_Combo_Aim_HixBox;
	CComboBox m_Combo_Aim_HixBoxType;
	afx_msg void OnCbnSelchangeComboAimHixbox();
	CButton m_Check_Aim_ShowPlayer;
	CButton m_Check_Aim_ShowPos;
	CButton m_Check_Aim_ShowRecoil;
	afx_msg void OnBnClickedCheckAimShowplayer();
	afx_msg void OnBnClickedCheckAimShowSition();
	afx_msg void OnBnClickedCheckAimShowRecoil();
	CButton m_Check_Aim_WeaponCan;
	afx_msg void OnBnClickedCheckAimCanWeapon();
	CButton m_Check_Aim_ZbsNoAim;
	CButton m_Check_Aim_NoGrenade;
	CButton m_Check_Aim_Barrier;
	afx_msg void OnBnClickedCheckAimZbsNoaim();
	afx_msg void OnBnClickedCheckAimNo4();
	afx_msg void OnBnClickedCheckAimBarrier();
	afx_msg void OnCbnSelchangeComboAimMouseType();
	CString m_Edit_ShootAngle;
	CButton m_Check_Aim_Shoot;
	afx_msg void OnBnClickedCheckAimShoot();
	afx_msg void OnEnChangeEditAimShootAngle();
	afx_msg void OnBnClickedButton1();
};
