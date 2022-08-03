// CTabDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CTabDlg.h"
#include "MenuHelper.h"


// CTabDlg 对话框

IMPLEMENT_DYNAMIC(CTabDlg, CDialogEx)

CTabDlg::CTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TABDLG, pParent)
{

}

CTabDlg::~CTabDlg()
{
}

void CTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CTabDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CTabDlg 消息处理程序


BOOL CTabDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}





void CTabDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	ACEDebugLog("[{}] Index:{}",__FUNCTION__, m_Tab.GetCurSel());

	switch (m_Tab.GetCurSel())
	{
	case 1:
	{
		if (!g_pTool->GetVar(xorstr_("sk_basics_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er01"));
		
		}
	}
		break;
	case 2:
	{
		if (!g_pTool->GetVar(xorstr_("sk_esp_enable"))->value || !g_pTool->GetVar(xorstr_("sk_wall_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er02"));
			
		}
	}
	break;
	case 3:
	{
		if (!g_pTool->GetVar(xorstr_("sk_aimbot_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er03"));
			
		}
	}
	break;
	case 4:
	{
		if (!g_pTool->GetVar(xorstr_("sk_kreedz_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er04"));
			
		}
	}
	break;
	case 5:
	{
		m_LuaScript.LuaInit();
	}
	break;
	case 6:
	{
		if (!g_pTool->GetVar(xorstr_("sk_cata_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er05"));

		}
	}
	break;
	case 7:
	{
		if (!g_pTool->GetVar(xorstr_("sk_zsht_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er06"));

		}
	}
	break;
	case 8:
	{
		if (!g_pTool->GetVar(xorstr_("sk_mp_enable"))->value)
		{
			m_Tab.SetCurSel(0);
			g_pTool->ShowMsgEr(xorstr_("#Tab_Fun_Enalbe_Er07"));

		}
	}
	break;
	
	default:
		break;
	}



	*pResult = 0;
}

void CTabDlg::Init()
{
	m_Page_Title[0] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Setting"));
	m_Page_Title[1] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Base"));
	m_Page_Title[2] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Esp"));
	m_Page_Title[3] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Aim"));
	m_Page_Title[4] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Kz"));
	m_Page_Title[5] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_LuaScript"));
	m_Page_Title[6] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Survival"));
	m_Page_Title[7] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Zsht"));
	m_Page_Title[8] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_MP"));
	m_Page_Title[9] = g_pTool->GetMenuItemValue(xorstr_("#Page_Sort_Weapon"));
	
	m_Tab.AddPage(m_Page_Title[0], &m_SettingPage, IDD_DIALOG_SETTING);
	m_Tab.AddPage(m_Page_Title[1], &m_BasePage, IDD_DIALOG_BASE);
	m_Tab.AddPage(m_Page_Title[2], &m_EspPage, IDD_DIALOG_ESP);
	m_Tab.AddPage(m_Page_Title[3], &m_AimPage, IDD_DIALOG_AIM);
	m_Tab.AddPage(m_Page_Title[4], &m_KzPage, IDD_DIALOG_KZ);
	m_Tab.AddPage(m_Page_Title[5], &m_LuaScript, IDD_DIALOG_LUA);
	m_Tab.AddPage(m_Page_Title[6], &m_Survival, IDD_DIALOG_SURVIVAL);
	m_Tab.AddPage(m_Page_Title[7], &m_ZshtPage, IDD_DIALOG_ZSHT);
	m_Tab.AddPage(m_Page_Title[8], &m_MpPage, IDD_DIALOG_MP);
	m_Tab.AddPage(m_Page_Title[9], &m_WeaponPage, IDD_DIALOG_WEAPON);
	
	m_Tab.Show();

	//配置显示到菜单
	LoadCFGGui();
}

void CTabDlg::LoadCFGGui()
{
	//获取全部参数
	cvar7_s* pVar = g_pTool->GetVarList();

	if (!pVar)
	{
		g_pTool->ShowMsgEr(xorstr_("#Setting_GetVarList_Error"));
		return;
	}
	while (pVar)
	{
		ACEDebugLog("[{}] {} {} {} {}", __FUNCTION__, pVar->name.data(), pVar->value, pVar->flags, pVar->hotkey);

		if (!SettingVar(pVar))
		{
			if (!BaseVar(pVar))
			{
				if (!EspVar(pVar))
				{
					if (!AimVar(pVar))
					{
						if (!KzVar(pVar))
						{
							if (!SurvivalVar(pVar))
							{
								if (!ZshtVar(pVar))
								{
									if (!WeaponVar(pVar))
									{
										if (!MpVar(pVar))
										{


										}

									}

								}
							}
							

						}
					}
				}

			}
		}







		pVar = pVar->next;
	}
}

CWeaponDlg* CTabDlg::GetWeaponDlg()
{
	return &m_WeaponPage;
}

BOOL CTabDlg::SettingVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mfcMenu_hotkey")) == 0)
	{
		// 低八位是 key 8-16位 = 组合键 (可能是)
		m_SettingPage.m_HotKey_Menu.SetHotKey(pVar->hotkey & 0xFF , (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mfcMenu_enable_tips")) == 0)
	{
		m_SettingPage.m_EnalbeMenuTips.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_enable")) == 0)
	{
		m_SettingPage.m_Check_BaseFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_enable")) == 0)
	{
		m_SettingPage.m_Check_EspFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_enable")) == 0)
	{
		m_SettingPage.m_Check_AimFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_enable")) == 0)
	{
		m_SettingPage.m_Check_WallFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_enable")) == 0)
	{
		m_SettingPage.m_Check_KZFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_enable")) == 0)
	{
		m_SettingPage.m_Check_ScriptFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_enable")) == 0)
	{
		m_SettingPage.m_Check_MpFun.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_deathbox")) == 0)
	{
		m_SettingPage.m_Check_DeathBox.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_deathico")) == 0)
	{
		m_SettingPage.m_Check_KillIco.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_soccer")) == 0)
	{
		m_SettingPage.m_Check_Soccer.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_fps")) == 0)
	{
		m_SettingPage.m_Check_GameFps.SetCheck((int)pVar->value);
		CString Text;
		Text.Format(L"%d", pVar->flags);
		m_SettingPage.SetDlgItemTextW(IDC_EDIT_GAME_FPS, Text);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_activate_window")) == 0)
	{
		m_SettingPage.m_Check_ActvateWindows.SetCheck((int)pVar->value);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_enable")) == 0)
	{
		m_SettingPage.m_Check_Zsht.SetCheck((int)pVar->value);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_enable")) == 0)
	{
		m_SettingPage.m_Check_Survival.SetCheck((int)pVar->value);

		Result = TRUE;
	}

	return Result;
}

BOOL CTabDlg::BaseVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_rpn")) == 0)
	{
		m_BasePage.m_Check_Rpn.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_Rpn.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_reconnect")) == 0)
	{
		m_BasePage.m_Check_ReRoom.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_ReRoom.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_retroom")) == 0)
	{
		m_BasePage.m_Check_RetRoom.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_RetRoom.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_noblackfog")) == 0)
	{
		m_BasePage.m_Check_NoFog.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_NoFog.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_maskgamemenu")) == 0)
	{
		m_BasePage.m_Check_NoGameMenu.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_NoGameMenu.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_cardenable")) == 0)
	{
		m_BasePage.m_Check_PlayerCardLag.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_PlayerCardLag.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_buymenu")) == 0)
	{
		m_BasePage.m_Check_ZbsBuy.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_ZbsBuy.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_thirdperson")) == 0)
	{
		m_BasePage.m_Check_ThirdPerson.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_ThirdPerson.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_superretroom")) == 0)
	{
		m_BasePage.m_Check_ExitRoom.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_ExitRoom.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_drone")) == 0)
	{
		m_BasePage.m_Check_ZbsBuy_Drone.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_ZbsBuy_Drone.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_nightvision")) == 0)
	{
		m_BasePage.m_Check_Nvg.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_maplighten")) == 0)
	{
		m_BasePage.m_Check_MapLig.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_healthmode")) == 0)
	{
		m_BasePage.m_Check_ShowHp.SetCheck((int)pVar->value);
		m_BasePage.m_List_ShowHp.EnableWindow((bool)pVar->value);

		if (pVar->value)	//开启的时候 才会显示选择的哪些功能.否则禁用状态下会显得很难看
		{
			int nSel = m_BasePage.m_List_ShowHp.GetCount();
			for (int i = 0; i < nSel; i++)
			{
				if (pVar->flags & (1 << i))
				{
					m_BasePage.m_List_ShowHp.SetSel(i, TRUE);
				}

			}
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_itemknifeid")) == 0)
	{
		m_BasePage.m_Check_Knfie.SetCheck((int)pVar->value);

		m_BasePage.m_HotKey_Knfie.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);

		/*
		*	item index -> itemName
		*	
		*/
		if (pVar->flags)
		{
			auto Item = g_pTool->GetGameItem();

			if (Item->count(pVar->flags))
			{
				int nIndex = m_BasePage.m_Combo_knifeList.FindStringExact(0, Item->at(pVar->flags)->ItemSrc);

				if (nIndex != CB_ERR)
				{
					m_BasePage.m_Combo_knifeList.SetCurSel(nIndex);
				}

			}
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_maskwords")) == 0)
	{
		m_BasePage.m_Check_NoChat.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_displaykick")) == 0)
	{
		m_BasePage.m_Check_Kick.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_zbsunlock")) == 0)
	{
		m_BasePage.m_Check_ProgressUnLock.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_roomkick")) == 0)
	{
		m_BasePage.m_Check_AntiRoomKick.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_damageskin")) == 0)
	{
		m_BasePage.m_Check_Font.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_fly")) == 0)
	{
		m_BasePage.m_Check_Flying.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_Flying.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_fly2")) == 0)
	{
		m_BasePage.m_Check_Flying2.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_Flying2.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_inventory_role")) == 0)
	{
		m_BasePage.m_Check_AlterRole.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_inventory_spray")) == 0)
	{
		m_BasePage.m_Check_AlterSpray.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_inventory_weapon")) == 0)
	{
		m_BasePage.m_Check_AlterWeapon.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_respawneffect")) == 0)
	{
		m_BasePage.m_Check_zbRespawnEffect.SetCheck((int)pVar->value);
		Result = TRUE;

		if (pVar->flags)
		{
			auto Item = g_pTool->GetGameItem();

			if (Item->count(pVar->flags))
			{
				int nIndex = m_BasePage.m_Combo_zbRespawnEffect.FindStringExact(0, Item->at(pVar->flags)->ItemSrc);

				if (nIndex != CB_ERR)
				{
					m_BasePage.m_Combo_zbRespawnEffect.SetCurSel(nIndex);
				}

			}
		}



	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_quest")) == 0)
	{
		m_BasePage.m_Check_Quest.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_console_option")) == 0)
	{
		m_BasePage.m_Check_DefaultConsole.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_vxl_destroy")) == 0)
	{
		m_BasePage.m_Check_Destroy.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_Destroy.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_vxl_weather")) == 0)
	{
		m_BasePage.m_Check_VxlWeather.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_vxl_sky")) == 0)
	{
		m_BasePage.m_Check_VxlSky.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_vxl_light")) == 0)
	{
		m_BasePage.m_Check_Vxl_Light.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_vxl_restart")) == 0)
	{
		m_BasePage.m_Check_Vxl_restart.SetCheck((int)pVar->value);
		m_BasePage.m_HotKey_VxlRestart.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_discipline")) == 0)
	{
		m_BasePage.m_Check_AllKick.SetCheck((int)pVar->value);	
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_basics_obstp")) == 0)
	{
		m_BasePage.m_Check_ObsTp.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	return Result;
}

BOOL CTabDlg::EspVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_validcheck")) == 0)
	{
		m_EspPage.m_Check_PlayerStatus.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playername")) == 0)
	{
		m_EspPage.m_Check_ShowName.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerbox")) == 0)
	{
		m_EspPage.m_Check_ShowBox.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerweapon")) == 0)
	{
		m_EspPage.m_Check_ShowWeapon.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerdistance")) == 0)
	{
		m_EspPage.m_Check_ShowDistance.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playershine")) == 0)
	{
		m_EspPage.m_Check_ShowShine.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerTraceMove")) == 0)
	{
		m_EspPage.m_Check_TraceMove.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerTraceAngleLine")) == 0)
	{
		m_EspPage.m_Check_TraceAngleLine.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_drawitems")) == 0)
	{
		m_EspPage.m_Check_Item.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_c4timer")) == 0)
	{
		m_EspPage.m_Check_C4info.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playername_team")) == 0)
	{
		m_EspPage.m_Combo_EspTeam.SetCurSel(pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerminiradar")) == 0)
	{
		ULONG uIndex = pVar->value;
		if (uIndex)
		{
			m_EspPage.m_Combo_Radar_pos.SetCurSel(uIndex  - 1);
		}
		m_EspPage.m_Check_PlayerRadar.SetCheck(uIndex);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerminiradar_team")) == 0)
	{
		m_EspPage.m_Combo_RadarTeam.SetCurSel(pVar->value - 1);
		
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerhintwarning")) == 0)
	{
		m_EspPage.m_Check_Waring.SetCheck((int)pVar->value);

		if ((int)pVar->value)
		{
			m_EspPage.m_Combo_Waring_Pos.SetCurSel(pVar->value - 1);
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_playerhintwarning_team")) == 0)
	{
		m_EspPage.m_Combo_Waring_Team.SetCurSel(pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_patrol_info")) == 0)
	{
		m_EspPage.m_Check_ShowDrone.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_souds")) == 0)
	{
		m_EspPage.m_Check_SoudsRadar.SetCheck((int)pVar->value);


		if (pVar->value)	//开启的时候 才会显示选择的哪些功能.否则禁用状态下会显得很难看
		{
			int nSel = m_EspPage.m_List_Sound.GetCount();
			for (int i = 0; i < nSel; i++)
			{
				if (pVar->flags & (1 << i))
				{
					m_EspPage.m_List_Sound.SetSel(i, TRUE);
				}

			}
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_souds_team")) == 0)
	{
		m_EspPage.m_Combo_SoudsRadarTeam.SetCurSel(pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_supply")) == 0)
	{
		m_EspPage.m_Check_Supply.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_zombie_trap")) == 0)
	{
		m_EspPage.m_Check_ZBtRAP.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_esp_prophunt")) == 0)
	{
		m_EspPage.m_Check_Prophunt.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	// 视觉功能
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_wallhacktype")) == 0)
	{
		ULONG uIndex = (int)pVar->value;

		if (uIndex)
		{
			m_EspPage.m_Combo_Wall_Type.SetCurSel(uIndex - 1);

		}
		m_EspPage.m_Check_Wall_Hack.SetCheck(uIndex);
		


		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_wallhackteam")) == 0)
	{
		m_EspPage.m_Combo_Wall_Team.SetCurSel((int)pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_showghost")) == 0)
	{
		m_EspPage.m_Check_Ghost.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_playerlighten")) == 0)
	{
		m_EspPage.m_Check_Player_Light.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_sightbead")) == 0)
	{
		ULONG uIndex = (int)pVar->value;

		if (uIndex)
		{
			m_EspPage.m_Combo_Sightbead_Type.SetCurSel(uIndex - 1);
		}

		m_EspPage.m_Check_Sightbead.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_nosmoke")) == 0)
	{
		m_EspPage.m_Check_NoSmoke.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_noflash")) == 0)
	{
		m_EspPage.m_Check_NoFlash.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_bleeding")) == 0)
	{
		m_EspPage.m_Check_Bleeding.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_outline")) == 0)
	{
		m_EspPage.m_Check_OutLine.SetCheck((int)pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_wall_outline_team")) == 0)
	{
		m_EspPage.m_Combo_OutLine_Team.SetCurSel((int)pVar->value);
		Result = TRUE;
	}






	return Result;
}

BOOL CTabDlg::AimVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_position")) == 0)
	{
		m_AimPage.m_Combo_Aim_Pos.SetCurSel((int)pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_camp")) == 0)
	{
		m_AimPage.m_Combo_Aim_Team.SetCurSel((int)pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_key")) == 0)
	{
		ULONG uIndex = pVar->value;

		if (uIndex)
		{
			m_AimPage.m_Combo_Aim_Key.SetCurSel(uIndex -1);
		}
		m_AimPage.m_Check_Aim_Key.SetCheck((BOOL)uIndex);
		
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_angle")) == 0)
	{
		ULONG uIndex = pVar->value;

		CString Text;
		Text.Format(L"%d", uIndex);
		m_AimPage.m_Edit_Aim_Angle = Text;
		UpdateData(FALSE);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_barrier")) == 0)
	{
		m_AimPage.m_Check_Aim_Barrier.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_aimtype")) == 0)
	{
		ULONG uIndex = pVar->value;

		m_AimPage.m_Combo_Aim_Type.SetCurSel(uIndex - 1);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_aimmode")) == 0)
	{
		ULONG uIndex = pVar->value;

		m_AimPage.m_Combo_Aim_Mode.SetCurSel(uIndex - 1);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_showplayer")) == 0)
	{
		m_AimPage.m_Check_Aim_ShowPlayer.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_showposition")) == 0)
	{
		m_AimPage.m_Check_Aim_ShowPos.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_showrecoil")) == 0)
	{
		m_AimPage.m_Check_Aim_ShowRecoil.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_shoot")) == 0)
	{
		m_AimPage.m_Check_Aim_Shoot.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_shoot_fov")) == 0)
	{
		ULONG uIndex = pVar->value;

		CString Text;
		Text.Format(L"%d", uIndex);
		m_AimPage.m_Edit_ShootAngle = Text;
		UpdateData(FALSE);

	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_zb_notaim")) == 0)
	{
		m_AimPage.m_Check_Aim_ZbsNoAim.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_no_grenade")) == 0)
	{
		m_AimPage.m_Check_Aim_NoGrenade.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_NoRecoil")) == 0)
	{
		m_AimPage.m_Check_Aim_NoRecoil.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_NoRecoil_Visual")) == 0)
	{
		m_AimPage.m_Check_NoRecoil_Visual.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_MouseMoveSpeed")) == 0)
	{
		m_AimPage.m_Slider.SetPos(pVar->value);

		m_AimPage.m_Combo_Aim_MouseType.SetCurSel(pVar->flags);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_RecoilCompensationPitch")) == 0)
	{
		CString Text;
		Text.Format(L"%0.2f", pVar->value);
		m_AimPage.m_Edit_Aim_Pitch = Text;
		UpdateData(FALSE);

	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_RecoilCompensationYaw")) == 0)
	{
		CString Text;
		Text.Format(L"%0.2f", pVar->value);
		m_AimPage.m_Edit_Aim_Yaw = Text;
		UpdateData(FALSE);

	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_RecoilCompensationMode")) == 0)
	{

		m_AimPage.m_Combo_Aim_RecoilMode.SetCurSel(pVar->value);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_aimbot_WeaponCanAttack")) == 0)
	{

		m_AimPage.m_Check_Aim_WeaponCan.SetCheck(pVar->value);

		Result = TRUE;
	}
	


	return Result;
}

BOOL CTabDlg::KzVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_groundspeed")) == 0)
	{
		m_KzPage.m_Check_278.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_spinhack")) == 0)
	{
		m_KzPage.m_Check_SpinHack.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_jumpbug")) == 0)
	{
		m_KzPage.m_Check_JumpBug.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_info")) == 0)
	{
		m_KzPage.m_Check_KzInfo.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_bunnyhop")) == 0)
	{
		m_KzPage.m_Check_BunnyHop.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_groundstrafe")) == 0)
	{
		ULONG uIndex = pVar->value;

		if (uIndex)
		{
			m_KzPage.m_Combo_McjKey.SetCurSel(uIndex - 1);
		}
		m_KzPage.m_Check_Mcj.SetCheck(BOOL(uIndex));
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_zbs_autogank_ct")) == 0)
	{
		int nSel = m_KzPage.m_List_ZbsAutoGank.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_KzPage.m_List_ZbsAutoGank.SetSel(i, (pVar->flags & (1 << i)));

		}
		m_KzPage.m_Check_ZbsAutoGank.SetCheck(pVar->value);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_zbs_counter")) == 0)
	{
		int nSel = m_KzPage.m_List_ZbsAutoCounter.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_KzPage.m_List_ZbsAutoCounter.SetSel(i, (pVar->flags & (1 << i)));

		}
		m_KzPage.m_Check_ZbsCounter.SetCheck(pVar->value);

		if (pVar->value)
		{
			m_KzPage.m_Combo_CounterMode.SetCurSel(pVar->value - 1);
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_zb5_auto_skill")) == 0)
	{
		m_KzPage.m_Check_ZBS_AutoSkill.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_enable")) == 0)
	{
		m_KzPage.m_Check_SpeedEnable.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_mode")) == 0)
	{
		if (pVar->value)
		{
			m_KzPage.m_Combo_GameSpeed.SetCurSel(pVar->value - 1);
		}
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_key")) == 0)
	{
		if (pVar->value)
		{
			m_KzPage.m_Combo_GameSpeedKey.SetCurSel(pVar->value - 1);
		}
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_value")) == 0)
	{
		CString Text;
		Text.Format(L"%.1f", pVar->value);
		m_KzPage.SetDlgItemTextW(IDC_EDIT_SPEED_VALUE, Text);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_reconnect")) == 0)
	{
		m_KzPage.m_Check_SpeedReConnectEnable.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_speed_reconnect_timer")) == 0)
	{
		CString Text;
		Text.Format(L"%d", (int)pVar->value);
		m_KzPage.SetDlgItemTextW(IDC_EDIT_SPEED_RECONNECT_VALUE, Text);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_kreedz_mode")) == 0)
	{
		m_KzPage.m_Combo_KZMode.SetCurSel(pVar->value - 1);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_god")) == 0)
	{
		m_LuaScript.m_Check_LuaGod.SetCheck(pVar->value);
		m_LuaScript.m_HotKey_God.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_script_excalibur")) == 0)
	{
		m_LuaScript.m_Check_God8133.SetCheck(pVar->value);
		m_LuaScript.m_HotKey_God8133.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	
	return Result;
}

BOOL CTabDlg::SurvivalVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_armor")) == 0)
	{
		m_Survival.m_Check_BuyArmor.SetCheck(pVar->value);
		m_Survival.m_HotKey_BuyArmor.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_attack")) == 0)
	{
		m_Survival.m_Check_BuyAtk.SetCheck(pVar->value);
		m_Survival.m_HotKey_BuyAtk.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_hp")) == 0)
	{
		m_Survival.m_Check_BuyHp.SetCheck(pVar->value);
		m_Survival.m_HotKey_BuyHp.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_bullet")) == 0)
	{
		m_Survival.m_Check_BuyBullet.SetCheck(pVar->value);
		m_Survival.m_HotKey_BuyBullet.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_bullet2")) == 0)
	{
		m_Survival.m_Check_BuyBullet2.SetCheck(pVar->value);
		m_Survival.m_HotKey_BuyBullet2.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_grenades")) == 0)
	{
		m_Survival.m_Check_BuyGrenade.SetCheck(pVar->value);
		m_Survival.m_HotKey_ZsBuyGrenade.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);


		if (pVar->flags)
		{
			auto Item = g_pTool->GetGameItem();

			if (Item->count(pVar->flags))
			{
				int nIndex = m_Survival.m_Combo_ZsGrenade.FindStringExact(0, Item->at(pVar->flags)->ItemSrc);

				if (nIndex != CB_ERR)
				{
					m_Survival.m_Combo_ZsGrenade.SetCurSel(nIndex);
				}

			}
		}

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_autobullet")) == 0)
	{
		m_Survival.m_Check_AutoBuyBullet.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_autoarmor")) == 0)
	{
		m_Survival.m_Check_AutoBuyArmor.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_drug_30")) == 0)
	{
		m_Survival.m_Check_AutoDrug30.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_drug_100")) == 0)
	{
		m_Survival.m_Check_AutoDrug100.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_afk")) == 0)
	{
		m_Survival.m_Check_ZS_AFK.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_atck_up")) == 0)
	{
		m_Survival.m_Check_AutoBuyAtk.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_role")) == 0)
	{
		m_Survival.m_Check_ZsRole.SetCheck(pVar->value);

		if (pVar->flags)
		{
			auto Item = g_pTool->GetGameItem();

			if (Item->count(pVar->value))
			{
				int nIndex = m_Survival.m_Combo_ZsRole.FindStringExact(0, Item->at(pVar->value)->ItemSrc);

				if (nIndex != CB_ERR)
				{
					m_Survival.m_Combo_ZsRole.SetCurSel(nIndex);
				}

			}
		}
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_cata_start_game")) == 0)
	{
		m_Survival.m_Check_ZsAutoStartGame.SetCheck(pVar->value);
		Result = TRUE;
	}


	return Result;
}

BOOL CTabDlg::ZshtVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_transfer")) == 0)
	{
		m_ZshtPage.m_Check_ZshtTransfer.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_contribute")) == 0)
	{
		m_ZshtPage.m_Check_ZshtContribute.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_resource")) == 0)
	{
		m_ZshtPage.m_Check_ZshtReSoures.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_loopbuild")) == 0)
	{
		m_ZshtPage.m_Combo_ZshtBuild.SetCurSel(pVar->flags);
		m_ZshtPage.m_Check_Build.SetCheck(pVar->value);
		CString Text;
		Text.Format(L"%d", (int)pVar->value);
		m_ZshtPage.SetDlgItemTextW(IDC_EDIT_ZSHT_COUNT, Text);

		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_loopbuild_x")) == 0)
	{
		CString Text;
		Text.Format(L"%d", (int)pVar->value);
		m_ZshtPage.SetDlgItemTextW(IDC_EDIT_ZSHT_X, Text);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_loopbuild_y")) == 0)
	{
		CString Text;
		Text.Format(L"%d", (int)pVar->value);
		m_ZshtPage.SetDlgItemTextW(IDC_EDIT_ZSHT_Y, Text);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_zsht_loopbuild_z")) == 0)
	{
		CString Text;
		Text.Format(L"%d", (int)pVar->value);
		m_ZshtPage.SetDlgItemTextW(IDC_EDIT_ZSHT_Z, Text);
		Result = TRUE;
	}
	return Result;
}

BOOL CTabDlg::WeaponVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_weapon")) == 0)
	{
		m_WeaponPage.m_Check_Weapon.SetCheck(pVar->value);
		m_WeaponPage.m_HotKey_Weapon.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}

	return Result;
}

BOOL CTabDlg::MpVar(cvar7_s* pVar)
{
	BOOL Result = FALSE;

	if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_mode")) == 0)
	{
		m_MpPage.m_Check_ModeMp.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_soul")) == 0)
	{
		m_MpPage.m_Check_Soul.SetCheck(pVar->value);
		m_MpPage.m_HotKey_Soul.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_sop")) == 0)
	{
		m_MpPage.m_Check_Sop.SetCheck(pVar->value);
		m_MpPage.m_HotKey_Sop.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_rpn")) == 0)
	{
		m_MpPage.m_Check_Rpn.SetCheck(pVar->value);
		m_MpPage.m_HotKey_Rpn.SetHotKey(pVar->hotkey & 0xFF, (pVar->hotkey >> 8) & 0xF);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_mode")) == 0)
	{
		m_MpPage.m_Check_ModeMp.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_gamemode")) == 0)
	{
		m_MpPage.m_Check_GameMode.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_gamemap")) == 0)
	{
		m_MpPage.m_Check_Map.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_GainMut")) == 0)
	{
		m_MpPage.m_Check_GainMut.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_health")) == 0)
	{
		m_MpPage.m_Check_Health.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_god")) == 0)
	{
		m_MpPage.m_Check_God.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_speed")) == 0)
	{
		m_MpPage.m_Check_Speed.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_ammo_max")) == 0)
	{
		m_MpPage.m_Check_AmmpMax.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_ammo_speed")) == 0)
	{
		m_MpPage.m_Check_AmmoSpeed.SetCheck(pVar->value);
		Result = TRUE;
	}
	else if (lstrcmpiA(pVar->name.data(), xorstr_("sk_mp_weapon_recoil")) == 0)
	{
		m_MpPage.m_Check_Recoil.SetCheck(pVar->value);
		Result = TRUE;
	}
	return Result;
}
