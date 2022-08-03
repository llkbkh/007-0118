#pragma once

#include "afxdialogex.h"
#include "TabSheet.h"
#include "SettingDlg.h"

#include "../007ConnectDef/Def.h"
#include "CBaseDlg.h"
#include "CESpDlg.h"
#include "CAim.h"
#include "CKzDlg.h"
#include "CLuaScriptDlg.h"
#include "CGameSurvival.h"
#include "CZshtDlg.h"
#include "CMPDlg.h"
#include "CWeaponDlg.h"

// CTabDlg 对话框


class CTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTabDlg)

public:
	CTabDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TABDLG };
#endif




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabSheet m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	
	void Init();

	void LoadCFGGui();
	CWeaponDlg* GetWeaponDlg();



	BOOL SettingVar(cvar7_s* pVar);
	BOOL BaseVar(cvar7_s* pVar);
	BOOL EspVar(cvar7_s* pVar);
	BOOL AimVar(cvar7_s* pVar);
	BOOL KzVar(cvar7_s* pVar);
	BOOL SurvivalVar(cvar7_s* pVar);
	BOOL ZshtVar(cvar7_s* pVar);
	BOOL WeaponVar(cvar7_s* pVar);
	BOOL MpVar(cvar7_s* pVar);

	CBaseDlg	    m_BasePage;
private:
	CDialog*	m_pDialog[20];  //用来保存对话框对象指针
	CString		m_Page_Title[20];

	CSettingDlg	    m_SettingPage;

	CESpDlg		    m_EspPage;
	CAim	        m_AimPage;
	CKzDlg		    m_KzPage;
	CLuaScriptDlg	m_LuaScript;
	CGameSurvival	m_Survival;
	CZshtDlg		m_ZshtPage;
	CMPDlg			m_MpPage;
	CWeaponDlg		m_WeaponPage;
};
