// CMPDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CMPDlg.h"
#include "MenuHelper.h"


// CMPDlg 对话框

IMPLEMENT_DYNAMIC(CMPDlg, CDialogEx)

CMPDlg::CMPDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MP, pParent)
{

}

CMPDlg::~CMPDlg()
{
}

void CMPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MP_MODE, m_Check_ModeMp);
	DDX_Control(pDX, IDC_COMBO_MAP, m_Combo_Map);
	DDX_Control(pDX, IDC_COMBO_GAMEMODE, m_Combo_GameMode);
	DDX_Control(pDX, IDC_CHECK_MP_GAMEMODE, m_Check_GameMode);
	DDX_Control(pDX, IDC_CHECK_MP_MAP, m_Check_Map);
	DDX_Control(pDX, IDC_CHECK_MP_GAINMUT, m_Check_GainMut);
	DDX_Control(pDX, IDC_CHECK_MP_HP, m_Check_Health);
	DDX_Control(pDX, IDC_CHECK_MP_GOD, m_Check_God);
	DDX_Control(pDX, IDC_CHECK_MP_SPEED, m_Check_Speed);
	DDX_Control(pDX, IDC_CHECK_MP_AMMO, m_Check_AmmpMax);
	DDX_Control(pDX, IDC_CHECK_MP_AMMOSPEED, m_Check_AmmoSpeed);
	DDX_Control(pDX, IDC_CHECK_MP_RECOIL, m_Check_Recoil);
	DDX_Control(pDX, IDC_CHECK_MP_JUMP, m_Check_hHghtJump);
	DDX_Control(pDX, IDC_CHECK_MP_SOUL, m_Check_Soul);
	DDX_Control(pDX, IDC_HOTKEY_SOUL, m_HotKey_Soul);
	DDX_Control(pDX, IDC_HOTKEY_SOP, m_HotKey_Sop);
	DDX_Control(pDX, IDC_CHECK_MP_SOP, m_Check_Sop);
	DDX_Control(pDX, IDC_CHECK_MP_RPN, m_Check_Rpn);
	DDX_Control(pDX, IDC_HOTKEY_RPN, m_HotKey_Rpn);
}


BEGIN_MESSAGE_MAP(CMPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_MP_MODE, &CMPDlg::OnBnClickedCheckMpMode)

	ON_BN_CLICKED(IDC_BUTTON_CUTMAP, &CMPDlg::OnBnClickedButtonCutmap)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP, &CMPDlg::OnCbnSelchangeComboMap)
	ON_BN_CLICKED(IDC_CHECK_MP_MAP, &CMPDlg::OnBnClickedCheckMpMap)
	ON_CBN_SELCHANGE(IDC_COMBO_GAMEMODE, &CMPDlg::OnCbnSelchangeComboGamemode)
	ON_BN_CLICKED(IDC_CHECK_MP_GAMEMODE, &CMPDlg::OnBnClickedCheckMpGamemode)

	ON_BN_CLICKED(IDC_CHECK_MP_GAINMUT, &CMPDlg::OnBnClickedCheckMpGainmut)
	ON_BN_CLICKED(IDC_CHECK_MP_HP, &CMPDlg::OnBnClickedCheckMpHp)
	ON_BN_CLICKED(IDC_CHECK_MP_GOD, &CMPDlg::OnBnClickedCheckMpGod)
	ON_BN_CLICKED(IDC_CHECK_MP_SPEED, &CMPDlg::OnBnClickedCheckMpSpeed)
	ON_BN_CLICKED(IDC_CHECK_MP_AMMO, &CMPDlg::OnBnClickedCheckMpAmmo)
	ON_BN_CLICKED(IDC_CHECK_MP_AMMOSPEED, &CMPDlg::OnBnClickedCheckMpAmmospeed)
	ON_BN_CLICKED(IDC_CHECK_MP_RECOIL, &CMPDlg::OnBnClickedCheckMpRecoil)
	ON_BN_CLICKED(IDC_CHECK_MP_JUMP, &CMPDlg::OnBnClickedCheckMpJump)
	ON_BN_CLICKED(IDC_CHECK_MP_SOUL, &CMPDlg::OnBnClickedCheckMpSoul)
	ON_BN_CLICKED(IDC_CHECK_MP_SOP, &CMPDlg::OnBnClickedCheckMpSop)
	ON_BN_CLICKED(IDC_CHECK_MP_RPN, &CMPDlg::OnBnClickedCheckMpRpn)
END_MESSAGE_MAP()


// CMPDlg 消息处理程序


void CMPDlg::OnBnClickedCheckMpMode()
{
	int Status = m_Check_ModeMp.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_mp_mode"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Mp_Notice_StartMode"));
	}
	Var->value = Status;
}


BOOL CMPDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_pGameMap = g_pTool->GetGameMap();
	m_pGameMode = g_pTool->GetGameMode();


	ULONG ulCurSel = 0;
	for (auto & iter : *m_pGameMap)
	{
		m_Combo_Map.InsertString(ulCurSel++, iter.second->map_translation.data());
	}
	m_Combo_Map.SetCurSel(ulCurSel - 1);
	ulCurSel = 0;
	for (auto& iter : *m_pGameMode)
	{
		m_Combo_GameMode.InsertString(ulCurSel++, iter.second.data());
	}
	m_Combo_GameMode.SetCurSel(ulCurSel - 1);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMPDlg::OnBnClickedButtonCutmap()
{
	OnCbnSelchangeComboMap();

	auto Text = fmt::format(xorstr_("map {}"), m_MapCode);

	g_pTool->GameCommand(Text.data());
}


void CMPDlg::OnCbnSelchangeComboMap()
{

	ULONG Index = m_Combo_Map.GetCurSel();

	CString Str;

	m_Combo_Map.GetLBText(Index, Str);

	for (auto& iter : *m_pGameMap)
	{
		if (lstrcmpiW(Str, iter.second->map_translation.data()) == 0)
		{
			m_MapCode = iter.second->map_code;

			g_pTool->GetVar(xorstr_("sk_mp_gamemap"))->flags = iter.second->map_id;
			break;
		}
	}



}


void CMPDlg::OnBnClickedCheckMpMap()
{
	OnCbnSelchangeComboMap();
	g_pTool->GetVar(xorstr_("sk_mp_gamemap"))->value = m_Check_Map.GetCheck();
}


void CMPDlg::OnCbnSelchangeComboGamemode()
{
	ULONG Index = m_Combo_GameMode.GetCurSel();

	CString Str;

	m_Combo_GameMode.GetLBText(Index, Str);

	for (auto& iter : *m_pGameMode)
	{
		if (lstrcmpiW(Str, iter.second.data()) == 0)
		{
			g_pTool->GetVar(xorstr_("sk_mp_gamemode"))->flags = iter.first;
			break;
		}
	}
}


void CMPDlg::OnBnClickedCheckMpGamemode()
{
	OnCbnSelchangeComboGamemode();
	g_pTool->GetVar(xorstr_("sk_mp_gamemode"))->value = m_Check_GameMode.GetCheck();
}




void CMPDlg::OnBnClickedCheckMpGainmut()
{
	ULONG ulIndex = m_Check_GainMut.GetCheck();

	if (ulIndex)
	{
		g_pTool->ShowMsg(xorstr_("#Mp_Notice_GainMut"));
	}
	g_pTool->GetVar(xorstr_("sk_mp_GainMut"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpHp()
{
	ULONG ulIndex = m_Check_Health.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_health"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpGod()
{
	ULONG ulIndex = m_Check_God.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_god"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpSpeed()
{
	ULONG ulIndex = m_Check_Speed.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_speed"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpAmmo()
{
	ULONG ulIndex = m_Check_AmmpMax.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_ammo_max"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpAmmospeed()
{
	ULONG ulIndex = m_Check_AmmoSpeed.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_ammo_speed"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpRecoil()
{
	ULONG ulIndex = m_Check_Recoil.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_weapon_recoil"))->value = ulIndex;
}


void CMPDlg::OnBnClickedCheckMpJump()
{
	ULONG ulIndex = m_Check_hHghtJump.GetCheck();

	g_pTool->GetVar(xorstr_("sk_mp_highjump"))->value = ulIndex;
	
}


void CMPDlg::OnBnClickedCheckMpSoul()
{
	// A-Soul

	int Status = m_Check_Soul.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_mp_soul"));

	Var->value = Status;
}


BOOL CMPDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_SOUL:
		g_pTool->GetVar(xorstr_("sk_mp_soul"))->hotkey = m_HotKey_Soul.GetHotKey();
		break;
	case IDC_HOTKEY_SOP:
		g_pTool->GetVar(xorstr_("sk_mp_sop"))->hotkey = m_HotKey_Sop.GetHotKey();
		break;
	case IDC_HOTKEY_RPN:
		g_pTool->GetVar(xorstr_("sk_mp_rpn"))->hotkey = m_HotKey_Rpn.GetHotKey();
		break;
		

		
	default:
		break;
	}





	return CDialogEx::OnCommand(wParam, lParam);
}


void CMPDlg::OnBnClickedCheckMpSop()
{
	int Status = m_Check_Sop.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_mp_sop"));

	Var->value = Status;
}


void CMPDlg::OnBnClickedCheckMpRpn()
{
	int Status = m_Check_Rpn.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_mp_rpn"));

	Var->value = Status;
}
