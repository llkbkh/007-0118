// CBaseDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CBaseDlg.h"
#include "MenuHelper.h"


// CBaseDlg 对话框

IMPLEMENT_DYNAMIC(CBaseDlg, CDialogEx)

CBaseDlg::CBaseDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BASE, pParent)
	, m_Edit_Command(_T(""))
{

}

CBaseDlg::~CBaseDlg()
{

}


void CBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_RPN, m_Check_Rpn);
	DDX_Control(pDX, IDC_HOTKEY_RPN, m_HotKey_Rpn);
	DDX_Control(pDX, IDC_CHECK_REROOM, m_Check_ReRoom);
	DDX_Control(pDX, IDC_HOTKEY_REROOM, m_HotKey_ReRoom);
	DDX_Control(pDX, IDC_CHECK_RETROOM, m_Check_RetRoom);
	DDX_Control(pDX, IDC_HOTKEY_RETROOM, m_HotKey_RetRoom);
	DDX_Control(pDX, IDC_CHECK_NOFOG, m_Check_NoFog);
	DDX_Control(pDX, IDC_HOTKEY_NOFOG, m_HotKey_NoFog);
	DDX_Control(pDX, IDC_CHECK_NOGAMEMENU, m_Check_NoGameMenu);
	DDX_Control(pDX, IDC_HOTKEY_NOGAMEMENU, m_HotKey_NoGameMenu);
	DDX_Control(pDX, IDC_CHECK_CARDLAG, m_Check_PlayerCardLag);
	DDX_Control(pDX, IDC_HOTKEY_CARDLAG, m_HotKey_PlayerCardLag);
	DDX_Control(pDX, IDC_CHECK_ZBSBUY, m_Check_ZbsBuy);
	DDX_Control(pDX, IDC_HOTKEY_ZBSBUG, m_HotKey_ZbsBuy);
	DDX_Control(pDX, IDC_CHECK_THPER, m_Check_ThirdPerson);
	DDX_Control(pDX, IDC_HOTKEY_THPER, m_HotKey_ThirdPerson);
	DDX_Control(pDX, IDC_CHECK_EXITROOM, m_Check_ExitRoom);
	DDX_Control(pDX, IDC_HOTKEY_EXITROOM, m_HotKey_ExitRoom);
	DDX_Control(pDX, IDC_CHECK_ZBSBUY_DRONE, m_Check_ZbsBuy_Drone);
	DDX_Control(pDX, IDC_HOTKEY_ZBSBUY_DRONE, m_HotKey_ZbsBuy_Drone);
	DDX_Control(pDX, IDC_CHECK_NVG, m_Check_Nvg);
	DDX_Control(pDX, IDC_CHECK_MPALIG, m_Check_MapLig);
	DDX_Control(pDX, IDC_LIST_SHOWHP, m_List_ShowHp);
	DDX_Control(pDX, IDC_CHECK_SHOWHP, m_Check_ShowHp);
	DDX_Control(pDX, IDC_CHECK_NOCHAT, m_Check_NoChat);
	DDX_Control(pDX, IDC_CHECK_NOLOTTER, m_Check_NoNottery);
	DDX_Control(pDX, IDC_CHECK_NOALLMSG, m_Check_NoAllMsg);
	DDX_Control(pDX, IDC_CHECK_KICK, m_Check_Kick);

	DDX_Control(pDX, IDC_COMBO_KNIFE, m_Combo_knifeList);
	DDX_Control(pDX, IDC_HOTKEY_KNIFE, m_HotKey_Knfie);
	DDX_Control(pDX, IDC_CHECK_KNIFE, m_Check_Knfie);
	DDX_Control(pDX, IDC_CHECK_SUPSEO, m_Check_SupSeo);
	DDX_Control(pDX, IDC_CHECK_FRAMSEO, m_Check_FramSeo);
	DDX_Control(pDX, IDC_CHECK_ALTERTEAM, m_Check_AlterTeam);
	DDX_Control(pDX, IDC_COMBO_TEAM, m_Combo_AlterTeam);
	DDX_Control(pDX, IDC_CHECK_PROGRESS_UNLOCK, m_Check_ProgressUnLock);
	DDX_Control(pDX, IDC_CHECK_ANTI_ROOMKICK, m_Check_AntiRoomKick);
	DDX_Control(pDX, IDC_CHECK_FONT, m_Check_Font);
	DDX_Control(pDX, IDC_CHECK_FLYING, m_Check_Flying);
	DDX_Control(pDX, IDC_HOTKEY_FLYING, m_HotKey_Flying);
	DDX_Control(pDX, IDC_CHECK_FLY2, m_Check_Flying2);
	DDX_Control(pDX, IDC_HOTKEY_FLYING2, m_HotKey_Flying2);
	DDX_Control(pDX, IDC_CHECK_ALTERRULE, m_Check_AlterRole);
	DDX_Control(pDX, IDC_CHECK_ALTERSPRAY, m_Check_AlterSpray);
	DDX_Control(pDX, IDC_CHECK_ALTERWEAPON, m_Check_AlterWeapon);
	//DDX_Control(pDX, IDC_COMBO_ZBSRPNOFFSET, m_Check_zbRespawnEffect);
	//  DDX_Control(pDX, IDC_CHECK_ZSBRPN, m_Check_zbRespawnEffect);
	DDX_Control(pDX, IDC_COMBO_ZBSRPNOFFSET, m_Combo_zbRespawnEffect);
	DDX_Control(pDX, IDC_CHECK_ZSBRPN, m_Check_zbRespawnEffect);
	DDX_Control(pDX, IDC_CHECK_QUEST, m_Check_Quest);
	DDX_Control(pDX, IDC_CHECK_DEFAulTCONSOLE, m_Check_DefaultConsole);
	DDX_Control(pDX, IDC_CHECK_BADWORD, m_Check_BadWord);
	DDX_Control(pDX, IDC_EDIT_CHAR, m_Edit_Chat);
	DDX_Control(pDX, IDC_CHECK_CHAT, m_Check_Chat);
	//  DDX_Control(pDX, IDC_EDIT_COMMAND, m_Edit_Command);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_Edit_Command);
	DDX_Control(pDX, IDC_HOTKEY_VXL_DESTROY, m_HotKey_Destroy);
	DDX_Control(pDX, IDC_CHECK_VXL_DESTROY, m_Check_Destroy);
	DDX_Control(pDX, IDC_CHECK_VXL_WEATHER, m_Check_VxlWeather);
	DDX_Control(pDX, IDC_CHECK_VXL_SKY, m_Check_VxlSky);
	DDX_Control(pDX, IDC_CHECK_VXL_LIGHT, m_Check_Vxl_Light);
	DDX_Control(pDX, IDC_CHECK_ZBSKICK, m_Check_ZbsKick);
	DDX_Control(pDX, IDC_CHECK_SET_NET_STATUS, m_Check_NetStatus);
	DDX_Control(pDX, IDC_CHECK_VXL_RESTART, m_Check_Vxl_restart);
	DDX_Control(pDX, IDC_HOTKEY_VXL_RESTART, m_HotKey_VxlRestart);
	DDX_Control(pDX, IDC_CHECK_ALLKICK, m_Check_AllKick);
	DDX_Control(pDX, IDC_CHECK_OBSTP, m_Check_ObsTp);
}


BEGIN_MESSAGE_MAP(CBaseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_SHOWHP, &CBaseDlg::OnBnClickedCheckShowhp)
	ON_LBN_SELCHANGE(IDC_LIST_SHOWHP, &CBaseDlg::OnLbnSelchangeListShowhp)
	ON_BN_CLICKED(IDC_CHECK_RPN, &CBaseDlg::OnBnClickedCheckRpn)
	ON_BN_CLICKED(IDC_CHECK_REROOM, &CBaseDlg::OnBnClickedCheckReroom)
	ON_BN_CLICKED(IDC_CHECK_RETROOM, &CBaseDlg::OnBnClickedCheckRetroom)
	ON_BN_CLICKED(IDC_CHECK_NOFOG, &CBaseDlg::OnBnClickedCheckNofog)
	ON_BN_CLICKED(IDC_CHECK_NOGAMEMENU, &CBaseDlg::OnBnClickedCheckNogamemenu)
	ON_BN_CLICKED(IDC_CHECK_CARDLAG, &CBaseDlg::OnBnClickedCheckCardlag)
	ON_BN_CLICKED(IDC_CHECK_ZBSBUY, &CBaseDlg::OnBnClickedCheckZbsbuy)
	ON_BN_CLICKED(IDC_CHECK_THPER, &CBaseDlg::OnBnClickedCheckThper)
	ON_BN_CLICKED(IDC_CHECK_EXITROOM, &CBaseDlg::OnBnClickedCheckExitroom)
	ON_BN_CLICKED(IDC_CHECK_ZBSBUY_DRONE, &CBaseDlg::OnBnClickedCheckZbsbuyDrone)
	ON_BN_CLICKED(IDC_CHECK_NVG, &CBaseDlg::OnBnClickedCheckNvg)
	ON_BN_CLICKED(IDC_CHECK_NOCHAT, &CBaseDlg::OnBnClickedCheckNochat)
	ON_BN_CLICKED(IDC_CHECK_NOLOTTER, &CBaseDlg::OnBnClickedCheckNolotter)
	ON_BN_CLICKED(IDC_CHECK_NOALLMSG, &CBaseDlg::OnBnClickedCheckNoallmsg)
	ON_BN_CLICKED(IDC_CHECK_MPALIG, &CBaseDlg::OnBnClickedCheckMpalig)
	ON_BN_CLICKED(IDC_CHECK_KICK, &CBaseDlg::OnBnClickedCheckKick)

	ON_BN_CLICKED(IDC_CHECK_KNIFE, &CBaseDlg::OnBnClickedCheckKnife)
	ON_CBN_SELCHANGE(IDC_COMBO_KNIFE, &CBaseDlg::OnCbnSelchangeComboKnife)
	ON_BN_CLICKED(IDC_CHECK_SUPSEO, &CBaseDlg::OnBnClickedCheckSupseo)
	ON_BN_CLICKED(IDC_CHECK_FRAMSEO, &CBaseDlg::OnBnClickedCheckFramseo)
	ON_BN_CLICKED(IDC_CHECK_ALTERTEAM, &CBaseDlg::OnBnClickedCheckAlterteam)
	ON_CBN_SELCHANGE(IDC_COMBO_TEAM, &CBaseDlg::OnCbnSelchangeComboTeam)
	ON_BN_CLICKED(IDC_CHECK_PROGRESS_UNLOCK, &CBaseDlg::OnBnClickedCheckProgressUnlock)
	ON_BN_CLICKED(IDC_CHECK_ANTI_ROOMKICK, &CBaseDlg::OnBnClickedCheckAntiRoomkick)
	ON_BN_CLICKED(IDC_CHECK_FONT, &CBaseDlg::OnBnClickedCheckFont)
	ON_BN_CLICKED(IDC_CHECK_FLYING, &CBaseDlg::OnBnClickedCheckFlying)
	ON_BN_CLICKED(IDC_CHECK_FLY2, &CBaseDlg::OnBnClickedCheckFly2)
	ON_BN_CLICKED(IDC_CHECK_ALTERRULE, &CBaseDlg::OnBnClickedCheckAlterrule)
	ON_BN_CLICKED(IDC_CHECK_ALTERSPRAY, &CBaseDlg::OnBnClickedCheckAlterspray)
	ON_BN_CLICKED(IDC_CHECK_ALTERWEAPON, &CBaseDlg::OnBnClickedCheckAlterweapon)
	ON_BN_CLICKED(IDC_CHECK_ZSBRPN, &CBaseDlg::OnBnClickedCheckZsbrpn)
	ON_CBN_SELCHANGE(IDC_COMBO_ZBSRPNOFFSET, &CBaseDlg::OnCbnSelchangeComboZbsrpnoffset)

	ON_BN_CLICKED(IDC_CHECK_QUEST, &CBaseDlg::OnBnClickedCheckQuest)
	ON_BN_CLICKED(IDC_CHECK_DEFAulTCONSOLE, &CBaseDlg::OnBnClickedCheckDefaultconsole)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND, &CBaseDlg::OnBnClickedButtonCommand)
	ON_BN_CLICKED(IDC_CHECK_VXL_DESTROY, &CBaseDlg::OnBnClickedCheckVxlDestroy)
	ON_BN_CLICKED(IDC_CHECK_VXL_WEATHER, &CBaseDlg::OnBnClickedCheckVxlWeather)
	ON_BN_CLICKED(IDC_CHECK_VXL_SKY, &CBaseDlg::OnBnClickedCheckVxlSky)
	ON_BN_CLICKED(IDC_CHECK_VXL_LIGHT, &CBaseDlg::OnBnClickedCheckVxlLight)
	ON_BN_CLICKED(IDC_CHECK_ZBSKICK, &CBaseDlg::OnBnClickedCheckZbskick)
	ON_BN_CLICKED(IDC_CHECK_SET_NET_STATUS, &CBaseDlg::OnBnClickedCheckSetNetStatus)
	ON_BN_CLICKED(IDC_CHECK_VXL_RESTART, &CBaseDlg::OnBnClickedCheckVxlRestart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_CHAT, &CBaseDlg::OnBnClickedCheckChat)
	ON_BN_CLICKED(IDC_CHECK_ALLKICK, &CBaseDlg::OnBnClickedCheckAllkick)
	ON_BN_CLICKED(IDC_CHECK_OBSTP, &CBaseDlg::OnBnClickedCheckObstp)
END_MESSAGE_MAP()


// CBaseDlg 消息处理程序


BOOL CBaseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_List_ShowHp.InsertString(0, xorstr_(L"左上方框显血"));
	m_List_ShowHp.InsertString(1, xorstr_(L"游戏文本显血"));
	m_List_ShowHp.InsertString(2, xorstr_(L"人物头顶显血"));


	//添加近身武器
	
	auto Item = g_pTool->GetGameItem();

	std::map<int, PGAME_ITEMMAP>::iterator it ;

	ULONG CurSel = 0;

	for (it = Item->begin(); it != Item->end(); it++)
	{
		if ((it)->second->Category == 11)
		{
			switch ((it)->first)
			{
			case 757:	//Index=757 Src 空手
			case 758:	//Index=758 Src CSO_Item_Name_STANDALONEKNIFE
			case 795:	//Index = 795 Src 空手
			case 8305:	//Index=8305 Src 大型急救包
			case 8307:	//Index=8307 Src 空手
				break;
			default:
				m_Combo_knifeList.InsertString(CurSel++, CString(it->second->ItemSrc));
				break;
			}

		}

		
	}
	m_Combo_knifeList.SetCurSel(CurSel - 1);

	// 添加复活特效
	ULONG RespawnCurSel = 0;
	for (it = Item->begin(); it != Item->end(); it++)
	{
		if ((it)->second->RecourceType == ClassType_zbRespawnEffect)
		{
			m_Combo_zbRespawnEffect.InsertString(RespawnCurSel++, CString(it->second->ItemSrc));
		}
	}
	m_Combo_zbRespawnEffect.SetCurSel(RespawnCurSel - 1);



	m_Combo_AlterTeam.InsertString(0, L"T");
	m_Combo_AlterTeam.InsertString(1, L"CT");
	m_Combo_AlterTeam.SetCurSel(1);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBaseDlg::OnBnClickedCheckShowhp()
{
	int Status = m_Check_ShowHp.GetCheck();

	m_List_ShowHp.EnableWindow(Status);

	auto pVar = g_pTool->GetVar(xorstr_("sk_basics_healthmode"));

	if (Status)
	{
		int nSel = m_List_ShowHp.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_List_ShowHp.SetSel(i, pVar->flags & (1 << i));
		}

	}
	if (pVar)
	{
		pVar->value = Status;
	}
	
}


void CBaseDlg::OnLbnSelchangeListShowhp()
{
	// TODO: 在此添加控件通知处理程序代码

	int nSel = m_List_ShowHp.GetSelCount();

	auto pVar = g_pTool->GetVar(xorstr_("sk_basics_healthmode"));

	if (pVar)
	{
		CArray< int, int& > arrayListSel;

		arrayListSel.SetSize(nSel);

		m_List_ShowHp.GetSelItems(nSel, arrayListSel.GetData());

		pVar->flags   = 0;

		for (int i = 0; i < nSel; i++)
		{
			int Index = arrayListSel[i];

			pVar->flags |=  (1 << Index);

			ACEDebugFileLog("{} Index:{}",__FUNCTION__, Index);
		}

		pVar->value = m_Check_ShowHp.GetCheck();
	}
	

}


void CBaseDlg::OnBnClickedCheckRpn()
{
	int Status = m_Check_Rpn.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_rpn"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_Rpn"), m_HotKey_Rpn.GetHotKeyName());
	}

	Var->value = Status;
}


BOOL CBaseDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_RPN:
		g_pTool->GetVar(xorstr_("sk_basics_rpn"))->hotkey          = m_HotKey_Rpn.GetHotKey();
		break;
	case IDC_HOTKEY_REROOM:
		g_pTool->GetVar(xorstr_("sk_basics_reconnect"))->hotkey    = m_HotKey_ReRoom.GetHotKey();
		break;
	case IDC_HOTKEY_RETROOM:
		g_pTool->GetVar(xorstr_("sk_basics_retroom"))->hotkey      = m_HotKey_RetRoom.GetHotKey();
		break;
	case IDC_HOTKEY_NOFOG:
		g_pTool->GetVar(xorstr_("sk_basics_noblackfog"))->hotkey   = m_HotKey_NoFog.GetHotKey();
		break;
	case IDC_HOTKEY_NOGAMEMENU:
		g_pTool->GetVar(xorstr_("sk_basics_maskgamemenu"))->hotkey = m_HotKey_NoGameMenu.GetHotKey();
		break;
	case IDC_HOTKEY_CARDLAG:
		g_pTool->GetVar(xorstr_("sk_basics_cardenable"))->hotkey   = m_HotKey_PlayerCardLag.GetHotKey();
		break;
	case IDC_HOTKEY_ZBSBUG:
		g_pTool->GetVar(xorstr_("sk_basics_buymenu"))->hotkey      = m_HotKey_ZbsBuy.GetHotKey();
		break;
	case IDC_HOTKEY_THPER:
		g_pTool->GetVar(xorstr_("sk_basics_thirdperson"))->hotkey  = m_HotKey_ThirdPerson.GetHotKey();
		break;
	case IDC_HOTKEY_EXITROOM:
		g_pTool->GetVar(xorstr_("sk_basics_superretroom"))->hotkey = m_HotKey_ExitRoom.GetHotKey();
		break;
	case IDC_HOTKEY_ZBSBUY_DRONE:
		g_pTool->GetVar(xorstr_("sk_basics_drone"))->hotkey        = m_HotKey_ZbsBuy_Drone.GetHotKey();
		break;
	case IDC_HOTKEY_KNIFE:
		g_pTool->GetVar(xorstr_("sk_basics_itemknifeid"))->hotkey  = m_HotKey_Knfie.GetHotKey();
		break;
	case IDC_HOTKEY_FLYING:
		g_pTool->GetVar(xorstr_("sk_basics_fly"))->hotkey          = m_HotKey_Flying.GetHotKey();
		break;
	case IDC_HOTKEY_FLYING2:
		g_pTool->GetVar(xorstr_("sk_basics_fly2"))->hotkey = m_HotKey_Flying2.GetHotKey();
		break;
	case IDC_HOTKEY_VXL_DESTROY:
		g_pTool->GetVar(xorstr_("sk_vxl_destroy"))->hotkey = m_HotKey_Destroy.GetHotKey();
		break;
	case IDC_HOTKEY_VXL_RESTART:
		g_pTool->GetVar(xorstr_("sk_vxl_restart"))->hotkey = m_HotKey_VxlRestart.GetHotKey();
		break;

		
	default:
		break;
	}

	



	return CDialogEx::OnCommand(wParam, lParam);
}


void CBaseDlg::OnBnClickedCheckReroom()
{
	// TODO: 在此添加控件通知处理程序代码

	int Status = m_Check_ReRoom.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_reconnect"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_Reconnect"), m_HotKey_ReRoom.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckRetroom()
{
	int Status = m_Check_RetRoom.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_retroom"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_RetRoom"), m_HotKey_RetRoom.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckNofog()
{
	int Status = m_Check_NoFog.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_noblackfog"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_Noblackfog"), m_HotKey_NoFog.GetHotKeyName());
	}

	Var->value = Status;

}


void CBaseDlg::OnBnClickedCheckNogamemenu()
{
	int Status = m_Check_NoGameMenu.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_maskgamemenu"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_NoGameBuyMenu"), m_HotKey_NoGameMenu.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckCardlag()
{
	int Status = m_Check_PlayerCardLag.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_cardenable"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_CardLag"), m_HotKey_PlayerCardLag.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckZbsbuy()
{
	int Status = m_Check_ZbsBuy.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_buymenu"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_ZbsBuy"), m_HotKey_ZbsBuy.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckThper()
{
	int Status = m_Check_ThirdPerson.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_thirdperson"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_Thirdperson"), m_HotKey_ThirdPerson.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckExitroom()
{
	int Status = m_Check_ExitRoom.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_superretroom"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_SuperRetRoom"), m_HotKey_ExitRoom.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckZbsbuyDrone()
{
	int Status = m_Check_ZbsBuy_Drone.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_drone"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_ZbsBuy_Drone"), m_HotKey_ZbsBuy_Drone.GetHotKeyName());
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckNvg()
{
	int Status = m_Check_Nvg.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_nightvision"));

	if (Status)
	{
		if (g_pTool->ShowMsgOK(xorstr_("#Base_Tip_Nightvision")) == IDYES)
		{
			Var->flags |= (1 << 0);
		}
		else
		{
			//最后一位清零
			Var->flags &= ~(1 << 0);
		}
		
	}

	Var->value = Status;
}


void CBaseDlg::OnBnClickedCheckNochat()
{
	int Status = m_Check_NoChat.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_maskwords"));

	if (Var)
	{
		Var->value = Status;
	}

}


void CBaseDlg::OnBnClickedCheckNolotter()
{
	int Status = m_Check_NoNottery.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_masklottery"));

	if (Var)
	{
		Var->value = Status;
	}
}


void CBaseDlg::OnBnClickedCheckNoallmsg()
{
	int Status = m_Check_NoAllMsg.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_maskallmsg"));

	if (Var)
	{
		Var->value = Status;
	}
}


void CBaseDlg::OnBnClickedCheckMpalig()
{
	int Status = m_Check_MapLig.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_maplighten"));

	if (Var)
	{
		Var->value = Status;
	}
}


void CBaseDlg::OnBnClickedCheckKick()
{
	int Status = m_Check_Kick.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_basics_displaykick"));

	if (Status)
	{
		if (g_pTool->ShowMsgOK(xorstr_("#Base_Tip_AutoKick")) == IDYES)
		{
			Var->flags |= (1 << 0);
		}
		else
		{
			//最后一位清零
			Var->flags &= ~(1 << 0);
		}

	}

	Var->value = Status;
}





void CBaseDlg::OnBnClickedCheckKnife()
{
	UpdateData(TRUE);

	ULONG State =  m_Check_Knfie.GetCheck();

	if (State)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Tip_AlterKnfie"), m_HotKey_Knfie.GetHotKeyName());
	}

	OnCbnSelchangeComboKnife();
}


void CBaseDlg::OnCbnSelchangeComboKnife()
{
	UpdateData(TRUE);

	int index = m_Combo_knifeList.GetCurSel();
	CString ItemStr;
	m_Combo_knifeList.GetLBText(index, ItemStr);        //根据行号，这个可以得到那行的字符串  

	//字符串比对
	auto Item = g_pTool->GetGameItem();

	std::map<int, PGAME_ITEMMAP>::iterator it;

	ULONG CurSel = 0;

	for (it = Item->begin(); it != Item->end(); it++)
	{
		if ((it)->second->Category == 11)
		{

			if (CString(it->second->ItemSrc).CompareNoCase(ItemStr) == 0)
			{
				auto pVar    = g_pTool->GetVar(xorstr_("sk_basics_itemknifeid"));
				pVar->flags  = it->first;
				pVar->value  = m_Check_Knfie.GetCheck();
				pVar->hotkey = m_HotKey_Knfie.GetHotKey();

			}

		}

	}
}


void CBaseDlg::OnBnClickedCheckSupseo()
{
	int Status = m_Check_SupSeo.GetCheck();

	if (Status)
	{
		g_pTool->GameCommand(xorstr_("cl_cmdrate 101;cl_updaterate 101;rate 25000;fps_max 101;net_graph 1;cl_showfps 1;gl_cull 1;"));
		g_pTool->GameCommand(xorstr_("gl_max_size 128;gl_playermip 2;mp_decals 0;gl_picmip 0;max_shells 0;max_smokepuffs 0;r_dynamic 0;gl_smoothmodels 0;violence_ablood 0;violence_hblood 0;"));

		g_pTool->ShowMsgOK(xorstr_("#Base_Tip_optimization"));
	
	}


}


void CBaseDlg::OnBnClickedCheckFramseo()
{
	int Status = m_Check_FramSeo.GetCheck();

	if (Status)
	{
		g_pTool->GameCommand(xorstr_("gl_fog 0;cl_showfps 1;cl_weather 0;fastsprites 2;MAX_shells 0;MAX_smokepuffs 0;cl_corpsestay 0;fastsprites 2;fps_max 101;"));

		g_pTool->ShowMsgOK(xorstr_("#Base_Tip_optimization"));

	}
	
}


void CBaseDlg::OnBnClickedCheckAlterteam()
{
	if (m_Check_AlterTeam.GetCheck())
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Tip_AlterTeam"));
	}
}


void CBaseDlg::OnCbnSelchangeComboTeam()
{
	UpdateData(TRUE);

	if (m_Check_AlterTeam.GetCheck())
	{
		g_pTool->AlterTeam(m_Combo_AlterTeam.GetCurSel() + 1);

	}

}


void CBaseDlg::OnBnClickedCheckProgressUnlock()
{
	if (m_Check_ProgressUnLock.GetCheck())
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_ProgressUnLock"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_zbsunlock"))->value = m_Check_ProgressUnLock.GetCheck();

}


void CBaseDlg::OnBnClickedCheckAntiRoomkick()
{
	ULONG Status = m_Check_AntiRoomKick.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_AntiRoomKick"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_roomkick"))->value = Status;

}


void CBaseDlg::OnBnClickedCheckFont()
{

	ULONG Status = m_Check_Font.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_Font"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_damageskin"))->value = Status;

}


void CBaseDlg::OnBnClickedCheckFlying()
{
	// TODO: 在此添加控件通知处理程序代码  Base_Notice_Fly

	ULONG Status = m_Check_Flying.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Notice_Fly"), m_HotKey_Flying.GetHotKeyName());
	}

	g_pTool->GetVar(xorstr_("sk_basics_fly"))->value = Status;

}


void CBaseDlg::OnBnClickedCheckFly2()
{
	ULONG Status = m_Check_Flying2.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Notice_Fly2"),  m_HotKey_Flying2.GetHotKeyName());
	}

	g_pTool->GetVar(xorstr_("sk_basics_fly2"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckAlterrule()
{
	
	ULONG Status = m_Check_AlterRole.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_AlterRule"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_inventory_role"))->value = Status;

}


void CBaseDlg::OnBnClickedCheckAlterspray()
{
	ULONG Status = m_Check_AlterSpray.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_AlterSpray"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_inventory_spray"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckAlterweapon()
{
	ULONG Status = m_Check_AlterWeapon.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_AlterWeapon"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_inventory_weapon"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckZsbrpn()
{
	ULONG Status = m_Check_zbRespawnEffect.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_AlterRespawneffect"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_respawneffect"))->value = Status;

	OnCbnSelchangeComboZbsrpnoffset();
}


void CBaseDlg::OnCbnSelchangeComboZbsrpnoffset()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int index = m_Combo_zbRespawnEffect.GetCurSel();
	CString ItemStr;
	m_Combo_zbRespawnEffect.GetLBText(index, ItemStr);   

	auto Item = g_pTool->GetGameItem();

	std::map<int, PGAME_ITEMMAP>::iterator it;

	ULONG CurSel = 0;

	for (it = Item->begin(); it != Item->end(); it++)
	{
		if ((it)->second->RecourceType == ClassType_zbRespawnEffect)
		{
			if (CString(it->second->ItemSrc).CompareNoCase(ItemStr) == 0)
			{
				auto pVar   = g_pTool->GetVar(xorstr_("sk_basics_respawneffect"));
				pVar->flags = it->first;

			}

		}

	}


}


//void CBaseDlg::OnBnClickedCheckNvg13()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CBaseDlg::OnBnClickedCheckQuest()
{
	ULONG Status = m_Check_Quest.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_Quest"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_quest"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckDefaultconsole()
{

	ULONG Status = m_Check_DefaultConsole.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_DefaultConsole"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_console_option"))->value = Status;
	
}


void CBaseDlg::OnBnClickedButtonCommand()
{
	UpdateData(TRUE);

	auto AceUtil = std::make_shared<CACEUtil>();

	auto Command =  AceUtil->WstringToString(m_Edit_Command.GetBuffer());

	g_pTool->GameCommand(Command.data());

	
	

}


void CBaseDlg::OnBnClickedCheckVxlDestroy()
{
	ULONG Status = m_Check_Destroy.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Notice_VxlDestroy"), m_HotKey_Destroy.GetHotKeyName());
	}

	g_pTool->GetVar(xorstr_("sk_vxl_destroy"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckVxlWeather()
{
	ULONG Status = m_Check_VxlWeather.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_VxlWeather"));
	}

	g_pTool->GetVar(xorstr_("sk_vxl_weather"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckVxlSky()
{
	ULONG Status = m_Check_VxlSky.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_VxlSky"));
	}

	g_pTool->GetVar(xorstr_("sk_vxl_sky"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckVxlLight()
{
	ULONG Status = m_Check_Vxl_Light.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_VxlLight"));
	}

	g_pTool->GetVar(xorstr_("sk_vxl_light"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckZbskick()
{
	ULONG Status = m_Check_ZbsKick.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Base_Notice_ZbsKick"));
	}

	g_pTool->GetVar(xorstr_("sk_basics_antikick"))->value = Status;
}


void CBaseDlg::OnBnClickedCheckSetNetStatus()
{
	ULONG Status = m_Check_NetStatus.GetCheck();

	static BOOL bValue = FALSE;

	if (Status && !bValue)
	{
		bValue = Status;

		g_pTool->ShowMsgOK(xorstr_("#Base_SetNetWork"));
	}

	g_pTool->SetGameNetWordStatus(m_Check_NetStatus.GetCheck());

}


void CBaseDlg::OnBnClickedCheckVxlRestart()
{

	int Status = m_Check_Vxl_restart.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_vxl_restart"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Base_Notice_VxlRestart"), m_HotKey_VxlRestart.GetHotKeyName());
	}

	Var->value = Status;
}


HBRUSH CBaseDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_CHECK_ZBSBUY:
	case IDC_CHECK_PROGRESS_UNLOCK:
	case IDC_CHECK_KICK:
	case IDC_CHECK_FONT:
		// 设置文本颜色
		pDC->SetTextColor(RGB(255, 0, 0));
		// 设置背景颜色
		//COLORREF bkColor = pDC->SetBkColor(0xF5F5F5);
		break;
	default:
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CBaseDlg::OnBnClickedCheckChat()
{
	if (m_Check_Chat.GetCheck())
	{
		SetTimer(1,1000, TimerProcChat);
	}
	else
	{
		KillTimer(1);
	}
}
void Split(CString source, CStringArray& dest, CString division)
{
	dest.RemoveAll();
	int pos = 0;
	int pre_pos = 0;
	while (-1 != pos) {
		pre_pos = pos;
		pos = source.Find(division, (pos + 1));
		dest.Add(source.Mid(pre_pos, (pos - pre_pos)));
	}
}
int Div(const CString strLine, wchar_t* split, CStringArray& strArray)
{
	strArray.RemoveAll();//自带清空属性
	CString temp = strLine;
	int tag = 0;
	while (1)
	{
		tag = temp.Find(split);
		if (tag >= 0)
		{
			strArray.Add(temp.Left(tag));
			temp = temp.Right(temp.GetLength() - tag - 2);
		}
		else { break; }
	}
	strArray.Add(temp);
	return strArray.GetSize();
}
void CALLBACK CBaseDlg::TimerProcChat(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	CString			TextChat;
	CStringArray    StrArray;
	static ULONG	ulIndex = 0;
	theApp.GetTabDlg()->m_BasePage.GetDlgItemTextW(IDC_EDIT_CHAR, TextChat);

	Div(TextChat, L"\r\n", StrArray);

	auto Util = g_pTool->GetACEUtil();


	if (ulIndex >= StrArray.GetSize())
	{
		ulIndex = 0;
	}
	auto u8Text = fmt::format("say :{}", Util->UnicodeToUtf8(StrArray[ulIndex++].GetBuffer()));


	g_pTool->GameCommand(u8Text.data());
}


void CBaseDlg::OnBnClickedCheckAllkick()
{
	int Status = m_Check_AllKick.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_discipline"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Base_Notice_AllLike"));
	}
}


void CBaseDlg::OnBnClickedCheckObstp()
{
	int Status = m_Check_ObsTp.GetCheck();

	g_pTool->GetVar(xorstr_("sk_basics_obstp"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Base_Notice_obstp"));
	}
}
