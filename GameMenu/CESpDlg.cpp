// CESpDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CESpDlg.h"
#include "MenuHelper.h"


// CESpDlg 对话框

IMPLEMENT_DYNAMIC(CESpDlg, CDialogEx)

CESpDlg::CESpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ESP, pParent)
{

}

CESpDlg::~CESpDlg()
{
}

void CESpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SHOWNAME, m_Check_ShowName);
	DDX_Control(pDX, IDC_CHECK_SHOWBOX, m_Check_ShowBox);
	DDX_Control(pDX, IDC_CHECK_SHOWWEAPON, m_Check_ShowWeapon);
	DDX_Control(pDX, IDC_CHECK_SHOWKM, m_Check_ShowDistance);
	DDX_Control(pDX, IDC_CHECK_SHINE, m_Check_ShowShine);
	DDX_Control(pDX, IDC_CHECK_SHOWTRACEMOVE, m_Check_TraceMove);
	DDX_Control(pDX, IDC_CHECK_TRACEANGLE, m_Check_TraceAngleLine);
	DDX_Control(pDX, IDC_CHECK_SHOWITEM, m_Check_Item);
	DDX_Control(pDX, IDC_CHECK_C4, m_Check_C4info);
	DDX_Control(pDX, IDC_CHECK_CHECKSTATUS, m_Check_PlayerStatus);
	//  DDX_Control(pDX, IDC_COMBO_ESPTEAM, m_Check_EspTeam);
	DDX_Control(pDX, IDC_COMBO_ESPTEAM, m_Combo_EspTeam);
	DDX_Control(pDX, IDC_CHECK_PLAYERRADAR, m_Check_PlayerRadar);
	DDX_Control(pDX, IDC_COMBO_RADAR_POS, m_Combo_Radar_pos);
	DDX_Control(pDX, IDC_COMBO_PLAYERRADARTEAM, m_Combo_RadarTeam);
	DDX_Control(pDX, IDC_CHECK_WARING, m_Check_Waring);
	DDX_Control(pDX, IDC_COMBO_WARING_POS, m_Combo_Waring_Pos);
	DDX_Control(pDX, IDC_COMBO_WARING_TEAM, m_Combo_Waring_Team);
	DDX_Control(pDX, IDC_CHECK_SHOWDRONE, m_Check_ShowDrone);
	DDX_Control(pDX, IDC_CHECK_SOUDSRADAR, m_Check_SoudsRadar);
	//  DDX_Control(pDX, IDC_COMBO_SOUDSRADAR, m_Combo_SoudsRadar);
	DDX_Control(pDX, IDC_COMBO_SOUDSRADAR, m_Combo_SoudsRadarTeam);
	DDX_Control(pDX, IDC_CHECK_PARABOLA, m_Check_Gremades_Parabola);
	DDX_Control(pDX, IDC_CHECK_PROPHUNT, m_Check_Prophunt);
	DDX_Control(pDX, IDC_CHECK_SUPPLY, m_Check_Supply);
	DDX_Control(pDX, IDC_CHECK_TRAP, m_Check_ZBtRAP);
	//  DDX_Control(pDX, IDC_COMBO_WALL_TEAM, m_Combo_Wall_Type);
	//  DDX_Control(pDX, IDC_COMBO_WALL_TYPE, m_Combo_Wall_Type);
	DDX_Control(pDX, IDC_CHECK_WALLHACK, m_Check_Wall_Hack);
	DDX_Control(pDX, IDC_COMBO_WALL_TEAM, m_Combo_Wall_Team);
	DDX_Control(pDX, IDC_COMBO_WALL_TYPE, m_Combo_Wall_Type);
	DDX_Control(pDX, IDC_CHECK_OUTLINE, m_Check_OutLine);
	DDX_Control(pDX, IDC_COMBO_OUTLINE_TEAM, m_Combo_OutLine_Team);
	DDX_Control(pDX, IDC_CHECK_SIGHTBEAD, m_Check_Sightbead);
	DDX_Control(pDX, IDC_COMBO_SIGHTBEAD_TYPE, m_Combo_Sightbead_Type);
	DDX_Control(pDX, IDC_CHECK_NOFLASH, m_Check_NoFlash);
	DDX_Control(pDX, IDC_CHECK_NOSMOKE, m_Check_NoSmoke);
	DDX_Control(pDX, IDC_CHECK_PLAYER_LIGHT, m_Check_Player_Light);
	DDX_Control(pDX, IDC_CHECK_GHOST, m_Check_Ghost);
	DDX_Control(pDX, IDC_CHECK_BLEEDING, m_Check_Bleeding);
	DDX_Control(pDX, IDC_LIST_SOUND, m_List_Sound);
	DDX_Control(pDX, IDC_CHECK_ALL_ITEM, m_Check_AllItem);
}


cvar7_s* CESpDlg::GetVar(char* Name)
{
	return g_pTool->GetVar(Name);
}

BEGIN_MESSAGE_MAP(CESpDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_SHOWNAME, &CESpDlg::OnBnClickedCheckShowname)
	ON_BN_CLICKED(IDC_CHECK_SHOWBOX, &CESpDlg::OnBnClickedCheckShowbox)
	ON_BN_CLICKED(IDC_CHECK_SHOWWEAPON, &CESpDlg::OnBnClickedCheckShowweapon)
	ON_BN_CLICKED(IDC_CHECK_SHOWKM, &CESpDlg::OnBnClickedCheckShowkm)
	ON_BN_CLICKED(IDC_CHECK_SHINE, &CESpDlg::OnBnClickedCheckShine)
	ON_BN_CLICKED(IDC_CHECK_SHOWTRACEMOVE, &CESpDlg::OnBnClickedCheckShowtracemove)
	ON_BN_CLICKED(IDC_CHECK_TRACEANGLE, &CESpDlg::OnBnClickedCheckTraceangle)
	ON_BN_CLICKED(IDC_CHECK_SHOWITEM, &CESpDlg::OnBnClickedCheckShowitem)
	ON_BN_CLICKED(IDC_CHECK_C4, &CESpDlg::OnBnClickedCheckC4)
	ON_BN_CLICKED(IDC_CHECK_CHECKSTATUS, &CESpDlg::OnBnClickedCheckCheckstatus)
	ON_CBN_SELCHANGE(IDC_COMBO_ESPTEAM, &CESpDlg::OnCbnSelchangeComboEspteam)
	ON_BN_CLICKED(IDC_CHECK_PLAYERRADAR, &CESpDlg::OnBnClickedCheckPlayerradar)
	ON_CBN_SELCHANGE(IDC_COMBO_RADAR_POS, &CESpDlg::OnCbnSelchangeComboRadarPos)
	ON_CBN_SELCHANGE(IDC_COMBO_PLAYERRADARTEAM, &CESpDlg::OnCbnSelchangeComboPlayerradarteam)
	ON_BN_CLICKED(IDC_CHECK_WARING, &CESpDlg::OnBnClickedCheckWaring)
	ON_CBN_SELCHANGE(IDC_COMBO_WARING_POS, &CESpDlg::OnCbnSelchangeComboWaringPos)
	ON_CBN_SELCHANGE(IDC_COMBO_WARING_TEAM, &CESpDlg::OnCbnSelchangeComboWaringTeam)
	ON_BN_CLICKED(IDC_CHECK_SHOWDRONE, &CESpDlg::OnBnClickedCheckShowdrone)
	ON_BN_CLICKED(IDC_CHECK_SOUDSRADAR, &CESpDlg::OnBnClickedCheckSoudsradar)
	ON_CBN_SELCHANGE(IDC_COMBO_SOUDSRADAR, &CESpDlg::OnCbnSelchangeComboSoudsradar)
	ON_BN_CLICKED(IDC_CHECK_PARABOLA, &CESpDlg::OnBnClickedCheckParabola)
	ON_BN_CLICKED(IDC_CHECK_PROPHUNT, &CESpDlg::OnBnClickedCheckProphunt)
	ON_BN_CLICKED(IDC_CHECK_SUPPLY, &CESpDlg::OnBnClickedCheckSupply)
	ON_BN_CLICKED(IDC_CHECK_TRAP, &CESpDlg::OnBnClickedCheckTrap)
	ON_BN_CLICKED(IDC_CHECK_WALLHACK, &CESpDlg::OnBnClickedCheckWallhack)
	ON_BN_CLICKED(IDC_CHECK_OUTLINE, &CESpDlg::OnBnClickedCheckOutline)
	ON_CBN_SELCHANGE(IDC_COMBO_WALL_TYPE, &CESpDlg::OnCbnSelchangeComboWallType)
	ON_CBN_SELCHANGE(IDC_COMBO_WALL_TEAM, &CESpDlg::OnCbnSelchangeComboWallTeam)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTLINE_TEAM, &CESpDlg::OnCbnSelchangeComboOutlineTeam)
	ON_BN_CLICKED(IDC_CHECK_SIGHTBEAD, &CESpDlg::OnBnClickedCheckSightbead)
	ON_CBN_SELCHANGE(IDC_COMBO_SIGHTBEAD_TYPE, &CESpDlg::OnCbnSelchangeComboSightbeadType)
	ON_BN_CLICKED(IDC_CHECK_BLEEDING, &CESpDlg::OnBnClickedCheckBleeding)
	ON_BN_CLICKED(IDC_CHECK_GHOST, &CESpDlg::OnBnClickedCheckGhost)
	ON_BN_CLICKED(IDC_CHECK_PLAYER_LIGHT, &CESpDlg::OnBnClickedCheckPlayerLight)
	ON_BN_CLICKED(IDC_CHECK_NOSMOKE, &CESpDlg::OnBnClickedCheckNosmoke)
	ON_BN_CLICKED(IDC_CHECK_NOFLASH, &CESpDlg::OnBnClickedCheckNoflash)
	ON_LBN_SELCHANGE(IDC_LIST_SOUND, &CESpDlg::OnLbnSelchangeListSound)
	ON_BN_CLICKED(IDC_CHECK_ALL_ITEM, &CESpDlg::OnBnClickedCheckAllItem)
END_MESSAGE_MAP()


// CESpDlg 消息处理程序


void CESpDlg::OnBnClickedCheckShowname()
{
	int Status = m_Check_ShowName.GetCheck();

	GetVar(xorstr_("sk_esp_playername"))->value = Status;

}


void CESpDlg::OnBnClickedCheckShowbox()
{
	int Status = m_Check_ShowBox.GetCheck();

	GetVar(xorstr_("sk_esp_playerbox"))->value = Status;
}


void CESpDlg::OnBnClickedCheckShowweapon()
{
	int Status = m_Check_ShowWeapon.GetCheck();

	GetVar(xorstr_("sk_esp_playerweapon"))->value = Status;
}


void CESpDlg::OnBnClickedCheckShowkm()
{
	int Status = m_Check_ShowDistance.GetCheck();

	GetVar(xorstr_("sk_esp_playerdistance"))->value = Status;
}


void CESpDlg::OnBnClickedCheckShine()
{
	int Status = m_Check_ShowShine.GetCheck();

	GetVar(xorstr_("sk_esp_playershine"))->value = Status;
}


void CESpDlg::OnBnClickedCheckShowtracemove()
{
	int Status = m_Check_TraceMove.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_TraceMove"));
	}

	GetVar(xorstr_("sk_esp_playerTraceMove"))->value = Status;
}


void CESpDlg::OnBnClickedCheckTraceangle()
{
	int Status = m_Check_TraceAngleLine.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_TraceAngleLine"));
	}

	GetVar(xorstr_("sk_esp_playerTraceAngleLine"))->value = Status;
}


void CESpDlg::OnBnClickedCheckShowitem()
{
	int Status = m_Check_Item.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_ShowItem"));
	}

	GetVar(xorstr_("sk_esp_drawitems"))->value = Status;
}


void CESpDlg::OnBnClickedCheckC4()
{
	int Status = m_Check_C4info.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_C4"));
	}

	GetVar(xorstr_("sk_esp_c4timer"))->value = Status;
}


void CESpDlg::OnBnClickedCheckCheckstatus()
{
	int Status = m_Check_PlayerStatus.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_ValidCheck"));
	}

	GetVar(xorstr_("sk_esp_validcheck"))->value = Status;
}


BOOL CESpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	for (size_t i = 0; i < 3; i++)
	{
		auto Text = g_pTool->GetMenuItemValue(fmt::format(xorstr_("#Esp_Combo_EspTeam_0{}"), i + 1));
		m_Combo_EspTeam.InsertString(i, Text);
		m_Combo_RadarTeam.InsertString(i, Text);
		m_Combo_Waring_Team.InsertString(i, Text);
		m_Combo_SoudsRadarTeam.InsertString(i, Text);
		m_Combo_Wall_Team.InsertString(i, Text);
		m_Combo_OutLine_Team.InsertString(i, Text);

	}
	m_Combo_EspTeam.SetCurSel(0);
	m_Combo_RadarTeam.SetCurSel(0);
	m_Combo_Waring_Team.SetCurSel(0);
	m_Combo_SoudsRadarTeam.SetCurSel(0);
	m_Combo_Wall_Team.SetCurSel(2);
	m_Combo_OutLine_Team.SetCurSel(0);


	for (size_t i = 0; i < 7; i++)
	{
		m_Combo_Radar_pos.InsertString(i, g_pTool->GetMenuItemValue(fmt::format(xorstr_("#Esp_Combo_Radar_pos_0{}"), i + 1)));
	}
	m_Combo_Radar_pos.SetCurSel(0);


	m_Combo_Waring_Pos.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Esp_Combo_Waring_Type_01")));
	m_Combo_Waring_Pos.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Esp_Combo_Waring_Type_02")));
	m_Combo_Waring_Pos.InsertString(2, g_pTool->GetMenuItemValue(xorstr_("#Esp_Combo_Waring_Type_03")));
	m_Combo_Waring_Pos.SetCurSel(0);

	m_Combo_Wall_Type.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Wall_Combo_Type_01")));
	m_Combo_Wall_Type.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Wall_Combo_Type_02")));
	m_Combo_Wall_Type.InsertString(2, g_pTool->GetMenuItemValue(xorstr_("#Wall_Combo_Type_03")));
	m_Combo_Wall_Type.SetCurSel(0);


	m_Combo_Sightbead_Type.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Wall_Combo_SightBead_01")));
	m_Combo_Sightbead_Type.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Wall_Combo_SightBead_02")));
	m_Combo_Sightbead_Type.SetCurSel(0);

	m_List_Sound.InsertString(0, xorstr_(L"显示标志"));
	m_List_Sound.InsertString(1, xorstr_(L"显示名字"));
	m_List_Sound.InsertString(2, xorstr_(L"显示距离"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CESpDlg::OnCbnSelchangeComboEspteam()
{
	// TODO: 在此添加控件通知处理程序代码

	ULONG uIndex = m_Combo_EspTeam.GetCurSel() + 1;

	GetVar(xorstr_("sk_esp_playername_team"))->value           = uIndex;
	GetVar(xorstr_("sk_esp_playerbox_team"))->value            = uIndex;
	GetVar(xorstr_("sk_esp_playerweapon_team"))->value         = uIndex;
	GetVar(xorstr_("sk_esp_playerdistance_team"))->value       = uIndex;
	GetVar(xorstr_("sk_esp_playershine_team"))->value          = uIndex;
	GetVar(xorstr_("sk_esp_playerTrackMove_team"))->value      = uIndex;
	GetVar(xorstr_("sk_esp_playerTraceAngleLine_team"))->value = uIndex;

}


void CESpDlg::OnBnClickedCheckPlayerradar()
{
	OnCbnSelchangeComboRadarPos();
}


void CESpDlg::OnCbnSelchangeComboRadarPos()
{
	// TODO: 在此添加控件通知处理程序代码
	int Status = m_Check_PlayerRadar.GetCheck();

	if (Status)
	{
		Status = m_Combo_Radar_pos.GetCurSel() + 1;
	}

	GetVar(xorstr_("sk_esp_playerminiradar"))->value = Status;
}


void CESpDlg::OnCbnSelchangeComboPlayerradarteam()
{
	GetVar(xorstr_("sk_esp_playerminiradar_team"))->value = m_Combo_RadarTeam.GetCurSel() + 1;
}


void CESpDlg::OnBnClickedCheckWaring()
{
	OnCbnSelchangeComboWaringPos();
}


void CESpDlg::OnCbnSelchangeComboWaringPos()
{
	int Status = m_Check_Waring.GetCheck();

	if (Status)
	{
		Status = m_Combo_Waring_Pos.GetCurSel() + 1;
	}

	GetVar(xorstr_("sk_esp_playerhintwarning"))->value = Status;
}


void CESpDlg::OnCbnSelchangeComboWaringTeam()
{
	GetVar(xorstr_("sk_esp_playerhintwarning_team"))->value = m_Combo_Waring_Team.GetCurSel() + 1;
}


void CESpDlg::OnBnClickedCheckShowdrone()
{
	int Status = m_Check_ShowDrone.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_Drone"));
	}

	GetVar(xorstr_("sk_esp_patrol_info"))->value = Status;
}


void CESpDlg::OnBnClickedCheckSoudsradar()
{

	auto pVar = GetVar(xorstr_("sk_esp_souds"));

	int Status = m_Check_SoudsRadar.GetCheck();

	if (Status)
	{
		int nSel = m_List_Sound.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_List_Sound.SetSel(i, pVar->flags & (1 << i));
		}

		g_pTool->ShowMsg(xorstr_("#Esp_Notice_SoudsRadar"));
	}

	if (pVar)
	{
		pVar->value = Status;
	}
}


void CESpDlg::OnCbnSelchangeComboSoudsradar()
{
	GetVar(xorstr_("sk_esp_souds_team"))->value = m_Combo_SoudsRadarTeam.GetCurSel()+1;
}


void CESpDlg::OnBnClickedCheckParabola()
{
	int Status = m_Check_Gremades_Parabola.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Esp_Notice_Gremades_Parabola"));
	}

	GetVar(xorstr_("sk_esp_grenades_parabola"))->value = Status;
}


void CESpDlg::OnBnClickedCheckProphunt()
{
	int Status = m_Check_Prophunt.GetCheck();

	GetVar(xorstr_("sk_esp_prophunt"))->value = Status;
}


void CESpDlg::OnBnClickedCheckSupply()
{
	int Status = m_Check_Supply.GetCheck();

	GetVar(xorstr_("sk_esp_supply"))->value = Status;
}


void CESpDlg::OnBnClickedCheckTrap()
{
	int Status = m_Check_ZBtRAP.GetCheck();

	GetVar(xorstr_("sk_esp_zombie_trap"))->value = Status;
}


void CESpDlg::OnBnClickedCheckWallhack()
{
	int Status = m_Check_Wall_Hack.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Wall_Notice_Prompt"));
	}
	OnCbnSelchangeComboWallType();
}


void CESpDlg::OnBnClickedCheckOutline()
{
	int Status = m_Check_OutLine.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Wall_Notice_OutLine"));
	}
	GetVar(xorstr_("sk_wall_outline"))->value = Status;
}


void CESpDlg::OnCbnSelchangeComboWallType()
{
	int Status = m_Check_Wall_Hack.GetCheck();

	if (Status)
	{
		Status = m_Combo_Wall_Type.GetCurSel() + 1;
	}
	GetVar(xorstr_("sk_wall_wallhacktype"))->value = Status;
}


void CESpDlg::OnCbnSelchangeComboWallTeam()
{
	GetVar(xorstr_("sk_wall_wallhackteam"))->value = m_Combo_Wall_Team.GetCurSel() + 1;
}


void CESpDlg::OnCbnSelchangeComboOutlineTeam()
{
	GetVar(xorstr_("sk_wall_outline_team"))->value = m_Combo_OutLine_Team.GetCurSel() + 1;
}


void CESpDlg::OnBnClickedCheckSightbead()
{
	OnCbnSelchangeComboSightbeadType();
}


void CESpDlg::OnCbnSelchangeComboSightbeadType()
{
	int Status = m_Check_Sightbead.GetCheck();

	if (Status)
	{
		Status = m_Combo_Sightbead_Type.GetCurSel() + 1;
	}
	GetVar(xorstr_("sk_wall_sightbead"))->value = Status;
}


void CESpDlg::OnBnClickedCheckBleeding()
{
	int Status = m_Check_Bleeding.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Wall_Notice_Bleeding"));
	}
	GetVar(xorstr_("sk_wall_bleeding"))->value = Status;
}


void CESpDlg::OnBnClickedCheckGhost()
{
	int Status = m_Check_Ghost.GetCheck();

	if (Status)
	{
		
	}
	GetVar(xorstr_("sk_wall_showghost"))->value = Status;
}


void CESpDlg::OnBnClickedCheckPlayerLight()
{
	int Status = m_Check_Player_Light.GetCheck();

	if (Status)
	{
		
	}
	GetVar(xorstr_("sk_wall_playerlighten"))->value = Status;
}


void CESpDlg::OnBnClickedCheckNosmoke()
{
	int Status = m_Check_NoSmoke.GetCheck();

	if (Status)
	{

	}
	GetVar(xorstr_("sk_wall_nosmoke"))->value = Status;
}


void CESpDlg::OnBnClickedCheckNoflash()
{
	int Status = m_Check_NoFlash.GetCheck();

	if (Status)
	{

	}
	GetVar(xorstr_("sk_wall_noflash"))->value = Status;
}


void CESpDlg::OnLbnSelchangeListSound()
{
	int nSel = m_List_Sound.GetSelCount();

	auto pVar = g_pTool->GetVar(xorstr_("sk_esp_souds"));

	if (pVar)
	{
		CArray< int, int& > arrayListSel;

		arrayListSel.SetSize(nSel);

		m_List_Sound.GetSelItems(nSel, arrayListSel.GetData());

		pVar->flags = 0;

		for (int i = 0; i < nSel; i++)
		{
			int Index = arrayListSel[i];

			pVar->flags |= (1 << Index);

			ACEDebugFileLog("{} Index:{}", __FUNCTION__, Index);
		}

		pVar->value = m_Check_SoudsRadar.GetCheck();
	}
}


void CESpDlg::OnBnClickedCheckAllItem()
{
	int Status = m_Check_AllItem.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#ESP_Notice_AllItem"));
	}
	GetVar(xorstr_("sk_esp_allitem"))->value = Status;
}
