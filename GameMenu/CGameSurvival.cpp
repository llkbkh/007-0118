// CGameSurvival.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CGameSurvival.h"
#include "MenuHelper.h"


// CGameSurvival 对话框

IMPLEMENT_DYNAMIC(CGameSurvival, CDialogEx)

CGameSurvival::CGameSurvival(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SURVIVAL, pParent)
{

}

CGameSurvival::~CGameSurvival()
{
}

void CGameSurvival::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_BUYHP, m_HotKey_BuyHp);
	DDX_Control(pDX, IDC_CHECK_BUYHP, m_Check_BuyHp);
	DDX_Control(pDX, IDC_CHECK_ARMOR, m_Check_BuyArmor);
	DDX_Control(pDX, IDC_CHECK_ATK, m_Check_BuyAtk);
	DDX_Control(pDX, IDC_CHECK_BULLET, m_Check_BuyBullet);
	DDX_Control(pDX, IDC_CHECK_BULLET2, m_Check_BuyBullet2);
	DDX_Control(pDX, IDC_HOTKEY_BULLET2, m_HotKey_BuyBullet2);
	DDX_Control(pDX, IDC_HOTKEY_BULLET, m_HotKey_BuyBullet);
	DDX_Control(pDX, IDC_HOTKEY_ATK, m_HotKey_BuyAtk);
	DDX_Control(pDX, IDC_HOTKEY_ARMOR, m_HotKey_BuyArmor);
	DDX_Control(pDX, IDC_COMBO_ZS_GRENADE, m_Combo_ZsGrenade);
	DDX_Control(pDX, IDC_CHECK_BUYGRENADE, m_Check_BuyGrenade);
	DDX_Control(pDX, IDC_HOTKEY_ZS_GRENADE, m_HotKey_ZsBuyGrenade);
	DDX_Control(pDX, IDC_CHECK_AUTO_BUY_BULLET, m_Check_AutoBuyBullet);
	DDX_Control(pDX, IDC_CHECK_AUTO_BUY_ARMOR, m_Check_AutoBuyArmor);
	DDX_Control(pDX, IDC_CHECK_AUTO_BUY_ATK, m_Check_AutoBuyAtk);
	DDX_Control(pDX, IDC_CHECK_AUTO_DRUG30, m_Check_AutoDrug30);
	DDX_Control(pDX, IDC_CHECK_AUTO_DRUG100, m_Check_AutoDrug100);
	DDX_Control(pDX, IDC_CHECK_AFK, m_Check_ZS_AFK);
	DDX_Control(pDX, IDC_COMBO_ZS_ROLE, m_Combo_ZsRole);
	DDX_Control(pDX, IDC_CHECK_AUTO_START_GAME, m_Check_ZsAutoStartGame);
	DDX_Control(pDX, IDC_CHECK_ROLE, m_Check_ZsRole);
}


BEGIN_MESSAGE_MAP(CGameSurvival, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_BUYHP, &CGameSurvival::OnBnClickedCheckBuyhp)
	ON_BN_CLICKED(IDC_CHECK_ARMOR, &CGameSurvival::OnBnClickedCheckArmor)
	ON_BN_CLICKED(IDC_CHECK_ATK, &CGameSurvival::OnBnClickedCheckAtk)
	ON_BN_CLICKED(IDC_CHECK_BULLET, &CGameSurvival::OnBnClickedCheckBullet)
	ON_BN_CLICKED(IDC_CHECK_BULLET2, &CGameSurvival::OnBnClickedCheckBullet2)
	ON_BN_CLICKED(IDC_CHECK_BUYGRENADE, &CGameSurvival::OnBnClickedCheckBuygrenade)
	ON_CBN_SELCHANGE(IDC_COMBO_ZS_GRENADE, &CGameSurvival::OnCbnSelchangeComboZsGrenade)
	ON_BN_CLICKED(IDC_CHECK_AUTO_BUY_BULLET, &CGameSurvival::OnBnClickedCheckAutoBuyBullet)
	ON_BN_CLICKED(IDC_CHECK_AUTO_BUY_ARMOR, &CGameSurvival::OnBnClickedCheckAutoBuyArmor)
	ON_BN_CLICKED(IDC_CHECK_AUTO_BUY_ATK, &CGameSurvival::OnBnClickedCheckAutoBuyAtk)
	ON_BN_CLICKED(IDC_CHECK_AUTO_DRUG30, &CGameSurvival::OnBnClickedCheckAutoDrug30)
	ON_BN_CLICKED(IDC_CHECK_AUTO_DRUG100, &CGameSurvival::OnBnClickedCheckAutoDrug100)
	ON_BN_CLICKED(IDC_CHECK_AFK, &CGameSurvival::OnBnClickedCheckAfk)
	ON_BN_CLICKED(IDC_CHECK_ROLE, &CGameSurvival::OnBnClickedCheckRole)
	ON_CBN_SELCHANGE(IDC_COMBO_ZS_ROLE, &CGameSurvival::OnCbnSelchangeComboZsRole)
	ON_BN_CLICKED(IDC_CHECK_AUTO_START_GAME, &CGameSurvival::OnBnClickedCheckAutoStartGame)
END_MESSAGE_MAP()


// CGameSurvival 消息处理程序


BOOL CGameSurvival::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_BUYHP:
		g_pTool->GetVar(xorstr_("sk_cata_hp"))->hotkey = m_HotKey_BuyHp.GetHotKey();
		break;
	case IDC_HOTKEY_ARMOR:
		g_pTool->GetVar(xorstr_("sk_cata_armor"))->hotkey = m_HotKey_BuyArmor.GetHotKey();
		break;
	case IDC_HOTKEY_ATK:
		g_pTool->GetVar(xorstr_("sk_cata_attack"))->hotkey = m_HotKey_BuyAtk.GetHotKey();
		break;
	case IDC_HOTKEY_BULLET:
		g_pTool->GetVar(xorstr_("sk_cata_attack"))->hotkey = m_HotKey_BuyBullet.GetHotKey();
		break;
	case IDC_HOTKEY_BULLET2:
		g_pTool->GetVar(xorstr_("sk_cata_bullet2"))->hotkey = m_HotKey_BuyBullet2.GetHotKey();
		break;
	case IDC_HOTKEY_ZS_GRENADE:
		g_pTool->GetVar(xorstr_("sk_cata_grenades"))->hotkey = m_HotKey_ZsBuyGrenade.GetHotKey();
		break;
		
	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


void CGameSurvival::OnBnClickedCheckBuyhp()
{
	int Status = m_Check_BuyHp.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_cata_hp"));

	Var->value = Status;
}


void CGameSurvival::OnBnClickedCheckArmor()
{
	int Status = m_Check_BuyArmor.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_cata_armor"));

	Var->value = Status;
}


void CGameSurvival::OnBnClickedCheckAtk()
{
	int Status = m_Check_BuyAtk.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_cata_attack"));

	Var->value = Status;
}


void CGameSurvival::OnBnClickedCheckBullet()
{

	int Status = m_Check_BuyBullet.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_cata_bullet"));

	Var->value = Status;

}


void CGameSurvival::OnBnClickedCheckBullet2()
{
	int Status = m_Check_BuyBullet2.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_cata_bullet2"));

	Var->value = Status;
}


BOOL CGameSurvival::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	auto FilterGrenades = [&](ULONG ulIndex)->BOOL
	{
		switch (ulIndex)
		{
		case 25:	//夜视仪
		case 27:
		case 28:
		case 29:
		case 754:
		case 755:
		case 756:
			return FALSE;
		default:
			break;
		}
		return TRUE;
	};


	auto items       = g_pTool->GetGameItem();
	ULONG CurSelBom  = 0;
	ULONG CurSelRole = 0;
	for (auto & ite :*items)
	{
		if (ite.second->Category == 6 && ite.second->SortingIndex == 9)
		{
			if (FilterGrenades(ite.first))
			{
				m_Combo_ZsGrenade.InsertString(CurSelBom++, ite.second->ItemSrc);
			}
		}
		else if (ite.second->RecourceType == ClassType_Class)
		{
			m_Combo_ZsRole.InsertString(CurSelRole++, ite.second->ItemSrc);
		}

	}

	m_Combo_ZsGrenade.SetCurSel(CurSelBom - 1);
	m_Combo_ZsRole.SetCurSel(CurSelRole - 1);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CGameSurvival::OnBnClickedCheckBuygrenade()
{
	OnCbnSelchangeComboZsGrenade();
}


void CGameSurvival::OnCbnSelchangeComboZsGrenade()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	int index = m_Combo_ZsGrenade.GetCurSel();

	CString ItemStr;

	m_Combo_ZsGrenade.GetLBText(index, ItemStr);

	auto Item = g_pTool->GetGameItem();

	std::map<int, PGAME_ITEMMAP>::iterator it;

	ULONG CurSel = 0;

	auto pVar = g_pTool->GetVar(xorstr_("sk_cata_grenades"));

	for (it = Item->begin(); it != Item->end(); it++)
	{

		if (it->second->Category == 6 && it->second->SortingIndex == 9)
		{
			if (CString(it->second->ItemSrc).CompareNoCase(ItemStr) == 0)
			{
				
				pVar->flags = it->first;

			}
		}

	}

	pVar->value = m_Check_BuyGrenade.GetCheck();
}


void CGameSurvival::OnBnClickedCheckAutoBuyBullet()
{
	g_pTool->GetVar(xorstr_("sk_cata_autobullet"))->value = m_Check_AutoBuyBullet.GetCheck();

}


void CGameSurvival::OnBnClickedCheckAutoBuyArmor()
{
	g_pTool->GetVar(xorstr_("sk_cata_autoarmor"))->value = m_Check_AutoBuyArmor.GetCheck();
}


void CGameSurvival::OnBnClickedCheckAutoBuyAtk()
{
	g_pTool->GetVar(xorstr_("sk_cata_atck_up"))->value = m_Check_AutoBuyAtk.GetCheck();
}


void CGameSurvival::OnBnClickedCheckAutoDrug30()
{
	g_pTool->GetVar(xorstr_("sk_cata_drug_30"))->value = m_Check_AutoDrug30.GetCheck();
}


void CGameSurvival::OnBnClickedCheckAutoDrug100()
{
	g_pTool->GetVar(xorstr_("sk_cata_drug_100"))->value = m_Check_AutoDrug100.GetCheck();
}


void CGameSurvival::OnBnClickedCheckAfk()
{
	g_pTool->GetVar(xorstr_("sk_cata_afk"))->value = m_Check_ZS_AFK.GetCheck();

	if (m_Check_ZS_AFK.GetCheck())
	{
		g_pTool->ShowMsgOK(xorstr_("#Survival_Notice_AFK"));
	}

}


void CGameSurvival::OnBnClickedCheckRole()
{
	ULONG Status = m_Check_ZsRole.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsgOK(xorstr_("#Survival_Notice_Role"));
	}


	OnCbnSelchangeComboZsRole();
}


void CGameSurvival::OnCbnSelchangeComboZsRole()
{
	UpdateData(TRUE);

	ULONG Status = m_Check_ZsRole.GetCheck();

	int index = m_Combo_ZsRole.GetCurSel();

	CString ItemStr;

	m_Combo_ZsRole.GetLBText(index, ItemStr);

	auto Item = g_pTool->GetGameItem();

	std::map<int, PGAME_ITEMMAP>::iterator it;

	ULONG CurSel = 0;

	auto pVar = g_pTool->GetVar(xorstr_("sk_cata_role"));

	for (it = Item->begin(); it != Item->end(); it++)
	{
		if (it->second->RecourceType == ClassType_Class)
		{
			if (CString(it->second->ItemSrc).CompareNoCase(ItemStr) == 0)
			{

				pVar->flags = it->second->InGameID;
				CurSel = it->first;
			}
		}

	}
	if (Status)
	{
		Status = CurSel;
	}
	pVar->value = Status;
}


void CGameSurvival::OnBnClickedCheckAutoStartGame()
{
	g_pTool->GetVar(xorstr_("sk_cata_start_game"))->value = m_Check_ZsAutoStartGame.GetCheck();

}
