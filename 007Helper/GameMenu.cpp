#include "GameMenu.h"
#include "Util.h"
#include "Drawing.h"
#include "GameAimBot.h"
#include "GameCFG.h"


#define OFF								"off"
#define ON								"on"

#define DOWN							"down"
#define UP								"up"

GameMenu::GameMenu()
{
	Visible     = false;
	SubMenu     = false;
	M_EntryPos  = 0;
	M_EntrySize = 0;
	S_EntryPos  = 0;
	S_EntrySize = 0;

	Clear();
}

bool GameMenu::vInit()
{
	return true;
}

void GameMenu::vHUD_Redraw(float time, int intermission)
{
	Draw(g_Vars.m_Menu.sk_menu_pos_x->value, g_Vars.m_Menu.sk_menu_pos_y->value);
}

int GameMenu::vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	HUD_Key_Event(keynum);
	return 1;
}

enum MENU_ENTRY
{
	//要排序
	MENU_SETTINGS     ,
	BASICS_MENU       ,
	ESP_MENU          ,
	AIM_MENU          ,
	WALLHACK_MENU     ,
	SCRIPT_MENU       ,
	SPEED_MENU        ,
	DEVELOPER_MENU    ,
	LOAD_SETTINGS_MENU,
	SAVE_SETTINGS_MENU,

};
void GameMenu::Add(char* Name, BYTE Red, BYTE Green, BYTE Blue, bool Space, bool Active)
{

	MenuEntry[M_EntrySize].Name = Name;


	MenuEntry[M_EntrySize].Color[0] = Red;
	MenuEntry[M_EntrySize].Color[1] = Green;
	MenuEntry[M_EntrySize].Color[2] = Blue;

	MenuEntry[M_EntrySize].Space = Space;
	MenuEntry[M_EntrySize].Active = Active;

	M_EntrySize++;
}

void GameMenu::Add(std::string Name, BYTE Red, BYTE Green, BYTE Blue, bool Space, bool Active)
{
	MenuEntry[M_EntrySize].Name = Name;

	MenuEntry[M_EntrySize].Color[0] = Red;
	MenuEntry[M_EntrySize].Color[1] = Green;
	MenuEntry[M_EntrySize].Color[2] = Blue;

	MenuEntry[M_EntrySize].Space = Space;
	MenuEntry[M_EntrySize].Active = Active;

	M_EntrySize++;
}

void GameMenu::AddSub(char* Name, float* Value, float Min, float Max, float Step, bool Float, bool Space)
{
	SubmEntry[S_EntrySize].Name = Name;
	
	SubmEntry[S_EntrySize].Value = Value;
	SubmEntry[S_EntrySize].Min = Min;
	SubmEntry[S_EntrySize].Max = Max;
	SubmEntry[S_EntrySize].Step = Step;
	SubmEntry[S_EntrySize].Float = Float;
	SubmEntry[S_EntrySize].Space = Space;

	S_EntrySize++;
}

void GameMenu::AddSub(std::string Name, float* Value, float Min, float Max, float Step, bool Float, bool Space)
{
	SubmEntry[S_EntrySize].Name = Name;

	SubmEntry[S_EntrySize].Value = Value;
	SubmEntry[S_EntrySize].Min = Min;
	SubmEntry[S_EntrySize].Max = Max;
	SubmEntry[S_EntrySize].Step = Step;
	SubmEntry[S_EntrySize].Float = Float;
	SubmEntry[S_EntrySize].Space = Space;

	S_EntrySize++;
}

void GameMenu::Clear()
{
	for (BYTE i = 0; i < M_EntrySize; ++i)
	{
		MenuEntry[i].Name.clear();
	}
	for (BYTE i = 0; i < S_EntrySize; ++i)
	{
		SubmEntry[i].Name.clear();
		SubmEntry[i].Value = 0;
		SubmEntry[i].Min = 0;
		SubmEntry[i].Max = 0;
		SubmEntry[i].Step = 0;
		SubmEntry[i].Float = false;
	}

	M_EntrySize = 0;
	S_EntrySize = 0;
}




void  GameMenu::Initialize()
{
	Clear();

	Add(GetInMenuSubItem("#menu_Settings"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_Basis"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_ESP"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_AimBot"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_WallHack"), 255, 255, 255, false, true);
	//Add(GetInMenuSubItem("#menu_ScreenInfo"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_Script"), 255, 255, 255, false, true);
	Add(GetInMenuSubItem("#menu_Speed"), 255, 255, 255, false, true);
	//Add(GetInMenuSubItem("#menu_Developer"), 255, 255, 255, false, true);
	//Add(GetInMenuSubItem("#menu_Load Settings"), 255, 255, 255, false, true);
	//Add(GetInMenuSubItem("#menu_Save Settings"), 255, 255, 255, false, true);



	if (M_EntryPos == (int)MENU_SETTINGS)
	{

		AddSub(GetInMenuSubItem("#menu_sub_Language"), &g_Vars.m_Menu.sk_menu_language->value, 1, 3, 1, false, true);
		AddSub(GetInMenuSubItem("#menu_sub_PosX"), &g_Vars.m_Menu.sk_menu_pos_x->value, 1, Engine::g_Screen.iWidth, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_PosY"), &g_Vars.m_Menu.sk_menu_pos_y->value, 1, Engine::g_Screen.iHeight, 1, false, true);
		AddSub(GetInMenuSubItem("#menu_sub_Height"), &g_Vars.m_Menu.sk_menu_boxheight->value, 13, 39, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Width"), &g_Vars.m_Menu.sk_menu_boxwidth->value, 170, 220, 1, false, true);
	}
	else if (M_EntryPos == (int)BASICS_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_Basics_Enable"), &g_Vars.m_Basi.sk_basics_enable->value, 0, 1, 1, false, true);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_Revive"), &g_Vars.m_Basi.sk_basics_rpn->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_Reconnect"), &g_Vars.m_Basi.sk_basics_reconnect->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_RetRoom"), &g_Vars.m_Basi.sk_basics_retroom->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_NightVision"), &g_Vars.m_Basi.sk_basics_nightvision->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_HealthMode"), &g_Vars.m_Basi.sk_basics_healthmode->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_ThirdPerson"), &g_Vars.m_Basi.sk_basics_thirdperson->value, 0, 2, 1, false, false);
		if (g_Vars.m_Basi.sk_basics_thirdperson->value)
		{
			switch ((int)g_Vars.m_Basi.sk_basics_thirdperson->value)
			{
			case 1:
				g_Vars.m_Basi.sk_basics_thirdperson->flags &= ~(1 << 0);
				break;
			case 2:
				g_Vars.m_Basi.sk_basics_thirdperson->flags |= (1 << 0);
				break;
			default:
				break;
			}
			AddSub(GetInMenuSubItem("#menu_sub_Basics_ViewDistance"), &g_Vars.m_Basi.sk_basics_viewdistance->value, -1000, 10000, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Basics_ViewHighly"), &g_Vars.m_Basi.sk_basics_viewhighly->value, -1000, 1000, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Basics_Idealyaw"), &g_Vars.m_Basi.sk_basics_idealyaw->value, -1000, 1000, 1, false, true);
		}
		AddSub(GetInMenuSubItem("#menu_sub_Basics_NoblackFog"), &g_Vars.m_Basi.sk_basics_noblackfog->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_Ballistic"), &g_Vars.m_Basi.sk_basics_ballistic->value, 0, 3, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Basics_MapLighten"), &g_Vars.m_Basi.sk_basics_maplighten->value, 0, 1, 1, false, false);
	}
	//显示ESP菜单
	else if (M_EntryPos == (int)ESP_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Enable"), &g_Vars.m_Esp.sk_esp_enable->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Esp_ValidCheck"), &g_Vars.m_Esp.sk_esp_validcheck->value, 0, 1, 1, false, false);//死活检查
		//ShowBox
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Box"), &g_Vars.m_Esp.sk_esp_playerbox->value, 0, 1, 1, false, false);		//方框类型
		if (g_Vars.m_Esp.sk_esp_playerbox->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_Box_Team"), &g_Vars.m_Esp.sk_esp_playerbox_team->value, 1, 3, 1, false, false);		//显示方框阵营
		//ShowName
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Name"), &g_Vars.m_Esp.sk_esp_playername->value, 0, 1, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playername->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Esp_NameTeam"), &g_Vars.m_Esp.sk_esp_playername_team->value, 1, 3, 1, false, false);
			//AddSub(MENUFUNNAME(menu_sub_Esp_NameType), &g_Vars.m_Esp.PlayerNameType, 1, 3, 1, false, false); 显示外挂用户
		}
		//ShowWeapon
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Weapon"), &g_Vars.m_Esp.sk_esp_playerweapon->value, 0, 4, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerweapon->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_WeaponTeam"), &g_Vars.m_Esp.sk_esp_playerweapon_team->value, 1, 3, 1, false, false);
		//显示玩家路径
		AddSub(GetInMenuSubItem("#menu_sub_Esp_playerTraceMove"), &g_Vars.m_Esp.sk_esp_playerTraceMove->value, 0, 1, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerTraceMove->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_TraceMoveTeam"), &g_Vars.m_Esp.sk_esp_playerTrackMove_team->value, 1, 3, 1, false, false);
		//显示瞄准线
		AddSub(GetInMenuSubItem("#menu_sub_Esp_TraceAngleLine"), &g_Vars.m_Esp.sk_esp_playerTraceAngleLine->value, 0, 2, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerTraceAngleLine->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_TraceAngleLineTeam"), &g_Vars.m_Esp.sk_esp_playerTraceAngleLine_team->value, 1, 3, 1, false, false);
		//ShowDistance
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Distance"), &g_Vars.m_Esp.sk_esp_playerdistance->value, 0, 1, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerdistance->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_DistanceTeam"), &g_Vars.m_Esp.sk_esp_playerdistance_team->value, 1, 3, 1, false, false);
		//发光
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Shine"), &g_Vars.m_Esp.sk_esp_playershine->value, 0, 4, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playershine->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_ShineTeam"), &g_Vars.m_Esp.sk_esp_playershine_team->value, 1, 3, 1, false, false);
		//显示世界项
		AddSub(GetInMenuSubItem("#menu_sub_Esp_WorldItems"), &g_Vars.m_Esp.sk_esp_drawitems->value, 0, 1, 1, false, false);
		//显示声音雷达
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Souds"), &g_Vars.m_Esp.sk_esp_souds->value, 0, 1, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_souds->value)
			AddSub(GetInMenuSubItem("#menu_sub_Esp_Souds_Team"), &g_Vars.m_Esp.sk_esp_souds_team->value, 1, 3, 1, false, false);
		//迷你雷达
		AddSub(GetInMenuSubItem("#menu_sub_Esp_MiniRadar"), &g_Vars.m_Esp.sk_esp_playerminiradar->value, 0, 7, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerminiradar->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Esp_MiniRadarTeam"), &g_Vars.m_Esp.sk_esp_playerminiradar_team->value, 1, 3, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Esp_MiniRadarSize"), &g_Vars.m_Esp.sk_esp_playerminiradar_size->value, 1, 100, 1, false, false);
		}
		//敌人预警
		AddSub(GetInMenuSubItem("#menu_sub_Esp_HintWarning"), &g_Vars.m_Esp.sk_esp_playerhintwarning->value, 0, 3, 1, false, false);
		if (g_Vars.m_Esp.sk_esp_playerhintwarning->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Esp_HintWarningTeam"), &g_Vars.m_Esp.sk_esp_playerhintwarning_team->value, 1, 3, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Esp_HintWarningDistance"), &g_Vars.m_Esp.sk_esp_playerhintwarning_distance->value, 1, 99999, 1, false, false);
		}
		AddSub(GetInMenuSubItem("#menu_sub_Esp_Grenades"), &g_Vars.m_Esp.sk_esp_grenades_parabola->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Esp_C4"), &g_Vars.m_Esp.sk_esp_c4timer->value, 0, 1, 1, false, false);
		//躲猫猫透视
		AddSub(GetInMenuSubItem("#menu_sub_Esp_PropHunt"), &g_Vars.m_Esp.sk_esp_prophunt->value, 0, 1, 1, false, false);
		// 显示无人机信息
		AddSub(GetInMenuSubItem("#menu_sub_Esp_PatrolInfo"), &g_Vars.m_Esp.sk_esp_patrol_info->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Esp_ZombieTrap"), &g_Vars.m_Esp.sk_esp_zombie_trap->value, 0, 1, 1, false, false);

	}
	//	自瞄菜单
	else if (M_EntryPos == (int)AIM_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Enable"), &g_Vars.m_Aimbot.sk_aimbot_enable->value, 0, 1, 1, false, true);

		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Key"), &g_Vars.m_Aimbot.sk_aimbot_key->value, 1, 6, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Camp"), &g_Vars.m_Aimbot.sk_aimbot_camp->value, 1, 3, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Angle"), &g_Vars.m_Aimbot.sk_aimbot_angle->value, 0, 360, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Type"), &g_Vars.m_Aimbot.sk_aimbot_aimtype->value, 1, 3, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Position"), &g_Vars.m_Aimbot.sk_aimbot_position->value, 1, 6, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Mode"), &g_Vars.m_Aimbot.sk_aimbot_aimmode->value, 1, 2, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Barrier"), &g_Vars.m_Aimbot.sk_aimbot_barrier->value, 0, 1, 1, false, false);

		if (g_Vars.m_Aimbot.sk_aimbot_aimmode->value == 2)
		{
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_MouseMoveSpeed"), &g_Vars.m_Aimbot.sk_aimbot_MouseMoveSpeed->value, 0, 1000, 1, false, false);
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_MouseMoveTrack"), (float*)&g_Vars.m_Aimbot.sk_aimbot_MouseMoveSpeed->flags, 0, 1, 1, false, false);
		}
		//AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_NoSpread"), &g_Vars.m_Aimbot.sk_aimbot_NoRecoil->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_NoSpreadVisual"), &g_Vars.m_Aimbot.sk_aimbot_NoRecoil_Visual->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationMode"), &g_Vars.m_Aimbot.sk_aimbot_RecoilCompensationMode->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationMode_DEBUG"), &g_Vars.m_Aimbot.sk_aimbot_RecoilCompensationMode_DEBUG->value, 0, 1, 1, false, false);

		if (g_Vars.m_Aimbot.sk_aimbot_RecoilCompensationMode->value == 0)
		{
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationPitch"), &g_Vars.m_Aimbot.sk_aimbot_RecoilCompensationPitch->value, -1000, 1000, 0.1f, true, false);
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationYaw"), &g_Vars.m_Aimbot.sk_aimbot_RecoilCompensationYaw->value, -1000, 1000, 0.1f, true, false);
		}
		else
		{
			auto pRecoil = GameAimBot::GetInstance()->GetCurWeaponRecoilCompensation();
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationPitch"), &pRecoil->x, -1000, 1000, 0.1f, true, false);
			AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_RecoilCompensationYaw"), &pRecoil->y, -1000, 1000, 0.1f, true, false);
		}

		AddSub(GetInMenuSubItem(u8"#menu_sub_AimBot_MatrixAdjustMode"), &g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust->value, 0, 3, 1, false, false);

		if (g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust->value)
		{
			AddSub(GetInMenuSubItem("#X"), &g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust_X->value, -1000, 1000, 0.1f, true, false);
			AddSub(GetInMenuSubItem("#Y"), &g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust_Y->value, -1000, 1000, 0.1f, true, false);
			if (g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust->value == 1)
			{
				AddSub(GetInMenuSubItem("#Z"), &g_Vars.m_Aimbot.sk_aimbot_MatrixAdjust_Z->value, -1000, 1000, 0.1f, true, false);
			}
		}


		AddSub(GetInMenuSubItem("#menu_sub_AimBot_Shoot"), &g_Vars.m_Aimbot.sk_aimbot_shoot->value, 0, 1, 1, false, false);
		if (g_Vars.m_Aimbot.sk_aimbot_shoot->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_AimBot_ShootFov"), &g_Vars.m_Aimbot.sk_aimbot_shoot_fov->value, 0, 360, 1, false, false);
		}

		AddSub(GetInMenuSubItem("#menu_sub_AimBot_ShowAimPlayer"), &g_Vars.m_Aimbot.sk_aimbot_showplayer->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_ShowPosition"), &g_Vars.m_Aimbot.sk_aimbot_showposition->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_ShowRecoil"), &g_Vars.m_Aimbot.sk_aimbot_showrecoil->value, 0, 1, 1, false, false);
		

		AddSub(GetInMenuSubItem("#menu_sub_AimBot_ZbNoAim"), &g_Vars.m_Aimbot.sk_aimbot_zb_notaim->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_NoGrenade"), &g_Vars.m_Aimbot.sk_aimbot_no_grenade->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_AimBot_WeaponCanAttack"), &g_Vars.m_Aimbot.sk_aimbot_WeaponCanAttack->value, 0, 1, 1, false, false);
		


	}
	//透视菜单
	else if (M_EntryPos == (int)WALLHACK_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_Wall_Enable"), &g_Vars.m_WallHack.sk_wall_enable->value, 0, 1, 1, false, true);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_Type"), &g_Vars.m_WallHack.sk_wall_wallhacktype->value, 0, 3, 1, false, false);
		if (g_Vars.m_WallHack.sk_wall_wallhacktype->value == 1)
			AddSub(GetInMenuSubItem("#menu_sub_Wall_Type_Team"), &g_Vars.m_WallHack.sk_wall_wallhackteam->value, 1, 3, 1, false, true);
		if (g_Vars.m_WallHack.sk_wall_wallhacktype->value == 3)
			AddSub(GetInMenuSubItem("#menu_sub_Wall_Type_Alpha"), &g_Vars.m_WallHack.sk_wall_halfwallhackalpha->value, 0.01f, 1, 0.01f, true, true);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_Bleed"), &g_Vars.m_WallHack.sk_wall_bleeding->value, 0, 2, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_Player_Lighten"), &g_Vars.m_WallHack.sk_wall_playerlighten->value, 0, 1, 1, false, false);

		AddSub(GetInMenuSubItem("#menu_sub_Wall_SightBead"), &g_Vars.m_WallHack.sk_wall_sightbead->value, 0, 2, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_ShowGhost"), &g_Vars.m_WallHack.sk_wall_showghost->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_NoSmoke"), &g_Vars.m_WallHack.sk_wall_nosmoke->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Wall_NoFlash"), &g_Vars.m_WallHack.sk_wall_noflash->value, 0, 1, 1, false, false);

		//描边透视
		AddSub(GetInMenuSubItem("#menu_sub_Wall_Outline"), &g_Vars.m_WallHack.sk_wall_outline->value, 0, 1, 1, false, false);
		if (g_Vars.m_WallHack.sk_wall_outline->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Wall_OutlineTeam"), &g_Vars.m_WallHack.sk_wall_outline_team->value, 0, 3, 1, false, false);

			AddSub(GetInMenuSubItem("#menu_sub_Wall_CT_R"), &g_Vars.m_WallHack.sk_wall_outline_ct_r->value, 0, 255, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Wall_CT_G"), &g_Vars.m_WallHack.sk_wall_outline_ct_g->value, 0, 255, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Wall_CT_B"), &g_Vars.m_WallHack.sk_wall_outline_ct_b->value, 0, 255, 1, false, false);

			AddSub(GetInMenuSubItem("#menu_sub_Wall_T_R"), &g_Vars.m_WallHack.sk_wall_outline_t_r->value, 0, 255, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Wall_T_G"), &g_Vars.m_WallHack.sk_wall_outline_t_g->value, 0, 255, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Wall_T_B"), &g_Vars.m_WallHack.sk_wall_outline_t_b->value, 0, 255, 1, false, false);
		}

	}
	//脚本显示功能
	else if (M_EntryPos == (int)SCRIPT_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_Enable"), &g_Vars.m_Kz.sk_kreedz_enable->value, 0, 1, 1, false, true);
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_PlayerSpeed"), &g_Vars.m_Kz.sk_kreedz_groundspeed->value, 0, 1, 1, false, false);



		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_Mode"), &g_Vars.m_Kz.sk_kreedz_mode->value, 0, 2, 1, false, true);


		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_AutoStrafe"), &g_Vars.m_Kz.sk_kreedz_autostrafe->value, 0, 1, 1, false, false);
		if (g_Vars.m_Kz.sk_kreedz_autostrafe->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_StrafeDi"), &g_Vars.m_Kz.sk_kreedz_autostrafe_strafedi->value, 0, 4, 1, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_AutoDir"), &g_Vars.m_Kz.sk_kreedz_autostrafe_autodir->value, 0, 1, 1, false, true);

		}
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_StrafeHelper"), &g_Vars.m_Kz.sk_kreedz_strafehelper->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_GroundStrafe"), &g_Vars.m_Kz.sk_kreedz_groundstrafe->value, 0, 3, 1, false, false);
		if (g_Vars.m_Kz.sk_kreedz_groundstrafe->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_KzSpeed"), &g_Vars.m_Kz.sk_kreedz_groundstrafea_speed->value, 450, 15000, 50, false, false);
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_Standup"), &g_Vars.m_Kz.sk_kreedz_groundstrafe_mscj->value, 0, 1, 1, false, false);

		}
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_BunnyHop"), &g_Vars.m_Kz.sk_kreedz_bunnyhop->value, 0, 2, 1, false, false);
		if (g_Vars.m_Kz.sk_kreedz_bunnyhop->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_BunnyHop_Duck"), &g_Vars.m_Kz.sk_kreedz_bunnyhop_autoduck->value, 0, 1, 1, false, false);
		}


		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_SpinHack"), &g_Vars.m_Kz.sk_kreedz_spinhack->value, 0, 1, 1, false, false);
		if (g_Vars.m_Kz.sk_kreedz_spinhack->value)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Kreedz_SpinHackSpeed"), &g_Vars.m_Kz.sk_kreedz_spinhackSpeed->value, 0, 10, 1, false, false);
		}


		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_JmupBug"), &g_Vars.m_Kz.sk_kreedz_jumpbug->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Kreedz_Info"), &g_Vars.m_Kz.sk_kreedz_info->value, 0, 1, 1, false, false);
		
	}
	else if (M_EntryPos == (int)SPEED_MENU)
	{
		AddSub(GetInMenuSubItem("#menu_sub_Speed_Enable"), &g_Vars.m_Speed.sk_speed_enable->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#menu_sub_Speed_Mode"), &g_Vars.m_Speed.sk_speed_mode->value, 0, 3, 1, false, true);
		if (g_Vars.m_Speed.sk_speed_mode->value == 3)
		{
			AddSub(GetInMenuSubItem("#menu_sub_Speed_Value"), &g_Vars.m_Speed.sk_speed_value->value, 1.0f, 1000, 1.0f, true, false);
		}
		else
		{
			AddSub(GetInMenuSubItem("#menu_sub_Speed_Value"), &g_Vars.m_Speed.sk_speed_value->value, 0.1f, 1000, 0.1f, true, false);

		}
		AddSub(GetInMenuSubItem("#menu_sub_Speed_Key"), &g_Vars.m_Speed.sk_speed_key->value, 0, 6, 1, false, false);

		AddSub(GetInMenuSubItem("#menu_sub_Speed_Reconnect"), &g_Vars.m_Speed.sk_speed_reconnect->value, 0, 1, 1, false, false);
		if (g_Vars.m_Speed.sk_speed_reconnect->value)
			AddSub(GetInMenuSubItem("#menu_sub_Speed_ReTimer"), &g_Vars.m_Speed.sk_speed_reconnect->value, 1, 10, 1, false, false);
	}
#if 0
	//显示开发者菜单
	else if (M_EntryPos == (int)DEVELOPER_MENU)
	{
		//AddSub(MENUFUNNAME(MENU_DEVELOPER_BONEPOINT), &g_Vars.developer.sk_developer_adjust->value, -100, 100, 1, false, false);
#if ANTI_DEBUG
		AddSub(u8"bone", &g_Vars.m_Dev.sk_developer_bone->value, 0, 1, 1, false, false);
		AddSub(u8"modelname", &g_Vars.m_Dev.sk_developer_modelname->value, 0, 1, 1, false, false);
#endif // ANTI_DEBUG


		//AddSub(MENUFUNNAME(MENU_DEVELOPER_AIMINGWAY), &g_Vars.developer.AimingWay, 0, 1, 1, false, false);
		//AddSub(MENUFUNNAME(MENU_DEVELOPER_ZOMBIETOGGLE), &g_Vars.developer.ZombieToggle, 0, 1, 1, false, false);

		AddSub(GetInMenuSubItem("#Dev_KillIcon"), &g_Vars.m_Dev.sk_developer_kill_icon->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#Dev_Fps"), &g_Vars.m_Dev.sk_developer_fps_max->value, 0, 5000, 1, false, false);

		AddSub(GetInMenuSubItem("#Dev_Shielding_effects"), &g_Vars.m_Dev.sk_developer_wondercannon_enable->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#Dev_Shielding_AllEffects"), &g_Vars.m_Dev.sk_developer_alleffects_enable->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#Dev_View_Mode"), &g_Vars.m_Dev.sk_developer_mxtn_mode->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#Dev_Game_Developer_Enable"), &g_Vars.m_Dev.sk_developer_game->value, 0, 1, 1, false, false);
		AddSub(GetInMenuSubItem("#Dev_Shielding_All"), &g_Vars.m_Dev.sk_developer_all_enable->value, 0, 1, 1, false, false);

		//AddSub(GetInMenuSubItem("#Dev_KillIcon"), &g_Vars.m_Dev.sk_developer_hg_notak->value, 0, 1, 1, false, false);


		//AddSub(MENUFUNNAME(MENU_DEVELOPER_ZOMBIEAIM), &g_Vars.m_Dev.aa_yaw_while_running->value, 0, 10, 1, false, false);
		//AddSub(MENUFUNNAME(MENU_DEVELOPER_ZOMBIEAIM), &g_Vars.m_Dev.aa_yaw_static->value, 0, 10, 1, false, false);
		//AddSub(MENUFUNNAME(MENU_DEVELOPER_ZOMBIEAIM), &g_Vars.m_Dev.aa_yaw->value, 0, 10, 1, false, false);
		//AddSub(MENUFUNNAME(MENU_DEVELOPER_ZOMBIEAIM), &g_Vars.m_Dev.aa_pitch->value, 0, 10, 1, false, false);

	}

	

#endif

}

void  GameMenu::HUD_Key_Event(int keynum)
{

	if (keynum == g_Vars.m_Menu.sk_menu_key->value)
	{
		Visible = !Visible;
	
	}
	if (!Visible)
	{
		return;
	}
#if 1
	if ((keynum == K_RIGHTARROW) && !SubMenu)
	{
		switch (M_EntryPos)
		{
		case LOAD_SETTINGS_MENU:
			if (keynum != K_END)
				//载入配置
				//g_GameCFG.ReadCvarInfo();
			break;
		case SAVE_SETTINGS_MENU:
			if (keynum != K_END)
				//保存配置
				//g_GameCFG.SavaCvarInfo();

			break;
		default:
			if (MenuEntry[M_EntryPos].Active)
			{
				S_EntryPos = 0;

				SubMenu = true;
			}

			break;
		}
	}
#endif
	
	else if (keynum == K_BACKSPACE && !SubMenu) //8 
	{
		Visible = false;
	}
	else if (keynum == K_BACKSPACE && SubMenu) //7 
	{
		SubMenu = false;
	}
	else if (keynum == K_UPARROW && !SubMenu)  //6
	{
		if (M_EntryPos > 0)
		{
			--M_EntryPos;
		}
	}
	else if (keynum == K_DOWNARROW && !SubMenu) // 5
	{
		if (M_EntryPos < (M_EntrySize - 1))
		{
			++M_EntryPos;
		}
	}
	else if (keynum == K_UPARROW && SubMenu) //4
	{
		if (S_EntryPos > 0)
		{
			--S_EntryPos;
		}
		else
		{
			SubMenu = false;
		}
	}
	else if (keynum == K_DOWNARROW && SubMenu) //3 
	{
		if (S_EntryPos < (S_EntrySize - 1))
		{
			++S_EntryPos;
		}
	}
	else if (keynum == K_LEFTARROW && SubMenu) //2
	{
		*SubmEntry[S_EntryPos].Value -= SubmEntry[S_EntryPos].Step;

		if (*SubmEntry[S_EntryPos].Value < SubmEntry[S_EntryPos].Min)
		{
			*SubmEntry[S_EntryPos].Value = SubmEntry[S_EntryPos].Min;
		}

	}
	else if (keynum == K_RIGHTARROW && SubMenu)  // 1
	{
		*SubmEntry[S_EntryPos].Value += SubmEntry[S_EntryPos].Step;
		if (*SubmEntry[S_EntryPos].Value > SubmEntry[S_EntryPos].Max)
		{
			*SubmEntry[S_EntryPos].Value = SubmEntry[S_EntryPos].Max;
		}

	}
}



std::string GameMenu::GetInMenuSubItem(std::string Item)
{
	return g_GameCFG.GetInMenuSubItem(Item);
}

void GameMenu::Draw(int Pos_x, int Pos_y)
{
	if (!Visible)
	{

		return;
	}
	Initialize();

	g_Drawing.Box(Pos_x, Pos_y, g_Vars.m_Menu.sk_menu_boxwidth->value, g_Vars.m_Menu.sk_menu_boxheight->value, 1, g_Vars.m_Menu.BoxColor[0],
		g_Vars.m_Menu.BoxColor[1], g_Vars.m_Menu.BoxColor[2], g_Vars.m_Menu.BoxColor[3]);

	short HeadPos_y = Pos_y;

	short HeadFillWidth = g_Vars.m_Menu.sk_menu_boxwidth->value - 2;
	short HeadFillHeight = g_Vars.m_Menu.sk_menu_boxheight->value - 2;

	short HeadMenuFillPos_x = Pos_x + 1;
	short HeadMenuFillPos_y = Pos_y + 1;

	g_Drawing.FillArea(HeadMenuFillPos_x, HeadMenuFillPos_y, HeadFillWidth, HeadFillHeight,
		g_Vars.m_Menu.BoxFillColor[0], g_Vars.m_Menu.BoxFillColor[1], g_Vars.m_Menu.BoxFillColor[2], g_Vars.m_Menu.BoxFillColor[3]);

	short HeadMenuTextPos_x = HeadMenuFillPos_x + (HeadFillWidth / 2);
	short HeadMenuTextPos_y = HeadMenuFillPos_y + (HeadFillHeight / 2) + 4;

	g_Verdana.Print(HeadMenuTextPos_x - 50, HeadMenuTextPos_y, g_Vars.m_Menu.TextColor[0], g_Vars.m_Menu.TextColor[1],
		g_Vars.m_Menu.TextColor[2], g_Vars.m_Menu.TextColor[3], FL_OUTLINE | FL_CENTER, xorstr_("CSOL (dev version)"));

	Pos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 2;

	short MenuFillPos_x = Pos_x + 1;
	short MenuFillPos_y;

	short MenuTextPos_x = MenuFillPos_x + 6;
	short MenuTextPos_y;

	for (short i = 0; i < M_EntrySize; ++i)
	{
		MenuFillPos_y = Pos_y + 1;

		MenuTextPos_y = MenuFillPos_y + (HeadFillHeight / 2) + 4;

		g_Drawing.Box(Pos_x, Pos_y, g_Vars.m_Menu.sk_menu_boxwidth->value, g_Vars.m_Menu.sk_menu_boxheight->value, 1,
			g_Vars.m_Menu.BoxColor[0], g_Vars.m_Menu.BoxColor[1], g_Vars.m_Menu.BoxColor[2], g_Vars.m_Menu.BoxColor[3]);

		g_Drawing.FillArea(MenuFillPos_x, MenuFillPos_y, HeadFillWidth, HeadFillHeight,
			g_Vars.m_Menu.BoxFillColor[0], g_Vars.m_Menu.BoxFillColor[1], g_Vars.m_Menu.BoxFillColor[2], g_Vars.m_Menu.BoxFillColor[3]);

		if (i == M_EntryPos)
		{
			if (!SubMenu)
			{
				g_Drawing.FillArea(MenuFillPos_x, MenuFillPos_y, HeadFillWidth, HeadFillHeight, g_Vars.m_Menu.BoxSelectColor[0],
					g_Vars.m_Menu.BoxSelectColor[1], g_Vars.m_Menu.BoxSelectColor[2], g_Vars.m_Menu.BoxSelectColor[3]);

				g_Drawing.FillArea(MenuFillPos_x, MenuFillPos_y, 2, HeadFillHeight,
					g_Vars.m_Menu.BoxSelectColor[0], g_Vars.m_Menu.BoxSelectColor[1], g_Vars.m_Menu.BoxSelectColor[2], 200);
			}
			else
			{
				g_Drawing.FillArea(MenuFillPos_x, MenuFillPos_y, 2, HeadFillHeight, g_Vars.m_Menu.BoxSelectColor[0],
					g_Vars.m_Menu.BoxSelectColor[1], g_Vars.m_Menu.BoxSelectColor[2], 200);
			}
		}

	

		g_Verdana.Print(MenuTextPos_x, MenuTextPos_y, MenuEntry[i].Color[0],
			MenuEntry[i].Color[1], MenuEntry[i].Color[2], 255, FL_OUTLINE, MenuEntry[i].Name.data());

		Pos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;

		if (MenuEntry[i].Space)
		{
			Pos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
		}
	}

	Pos_x += g_Vars.m_Menu.sk_menu_boxwidth->value + 1;

	short HeadSubMenuFillPos_x = Pos_x + 1;
	short HeadSubMenuFillPos_y = Pos_y + 1;

	if (SubMenu)
	{
		short SubMenuBoxPos_y = HeadPos_y + g_Vars.m_Menu.sk_menu_boxheight->value + 2;

		short SubMenuFillPos_x = Pos_x + 1;
		short SubMenuFillPos_y = SubMenuBoxPos_y + 1;

		short SubMenuTextPos_x = SubMenuFillPos_x + 6;
		short SubMenuTextPos_y = SubMenuFillPos_y + (HeadFillHeight / 2) + 4;

		for (BYTE i = 0; i < S_EntrySize; ++i)
		{
			g_Drawing.Box(Pos_x, SubMenuBoxPos_y, g_Vars.m_Menu.sk_menu_boxwidth->value, g_Vars.m_Menu.sk_menu_boxheight->value, 1,
				g_Vars.m_Menu.BoxColor[0], g_Vars.m_Menu.BoxColor[1], g_Vars.m_Menu.BoxColor[2], g_Vars.m_Menu.BoxColor[3]);

			g_Drawing.FillArea(SubMenuFillPos_x, SubMenuFillPos_y, HeadFillWidth, HeadFillHeight, g_Vars.m_Menu.BoxFillColor[0],
				g_Vars.m_Menu.BoxFillColor[1], g_Vars.m_Menu.BoxFillColor[2], g_Vars.m_Menu.BoxFillColor[3]);

			if (i == S_EntryPos)
			{
				g_Drawing.FillArea(SubMenuFillPos_x, SubMenuFillPos_y, HeadFillWidth, HeadFillHeight, g_Vars.m_Menu.BoxSelectColor[0],
					g_Vars.m_Menu.BoxSelectColor[1], g_Vars.m_Menu.BoxSelectColor[2], g_Vars.m_Menu.BoxSelectColor[3]);

				g_Drawing.FillArea(SubMenuFillPos_x, SubMenuFillPos_y, 2, HeadFillHeight,
					g_Vars.m_Menu.BoxSelectColor[0], g_Vars.m_Menu.BoxSelectColor[1], g_Vars.m_Menu.BoxSelectColor[2], 200);
			}

			g_Verdana.Print(SubMenuTextPos_x, SubMenuTextPos_y, g_Vars.m_Menu.TextColor[0], g_Vars.m_Menu.TextColor[1],
				g_Vars.m_Menu.TextColor[2], g_Vars.m_Menu.TextColor[3], FL_OUTLINE, SubmEntry[i].Name.data());

			if (!SubmEntry[i].Min && SubmEntry[i].Max < 3)
			{
				if (SubmEntry[i].Max == 1)
				{
					*SubmEntry[i].Value ?
						g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
							SubMenuTextPos_y, 80, 255, 80, 255, FL_OUTLINE, ON) :

						g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
							SubMenuTextPos_y, 255, 80, 80, 255, FL_OUTLINE, (OFF));
				}
				else if (SubmEntry[i].Max == 2)
				{
					*SubmEntry[i].Value ?
						g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
							SubMenuTextPos_y, 80, 255, 80, 255, FL_OUTLINE, "%d", (int)*SubmEntry[i].Value) :

						g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
							SubMenuTextPos_y, 255, 80, 80, 255, FL_OUTLINE, (OFF));
				}
			}
			else if (SubmEntry[i].Min == 1000 && SubmEntry[i].Max == 1001)
			{
				if (*SubmEntry[i].Value == 1000)
				{
					g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
						SubMenuTextPos_y, 255, 255, 255, 255, FL_OUTLINE, (DOWN));
				}
				else if (*SubmEntry[i].Value == 1001)
				{
					g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40),
						SubMenuTextPos_y, 255, 255, 255, 255, FL_OUTLINE, (UP));
				}
			}
			else
			{
				if (SubmEntry[i].Float)
				{
					g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40), SubMenuTextPos_y,
						255, 255, 255, 255, FL_OUTLINE, "%.1f", *SubmEntry[i].Value);
				}
				else
				{
					g_Verdana.Print(SubMenuTextPos_x + (g_Vars.m_Menu.sk_menu_boxwidth->value - 40), SubMenuTextPos_y, 255,
						255, 255, 255, FL_OUTLINE, "%d", (int)*SubmEntry[i].Value);
				}
			}

			SubMenuBoxPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
			SubMenuFillPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
			SubMenuTextPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;

			if (SubmEntry[i].Space)
			{
				SubMenuBoxPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
				SubMenuFillPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
				SubMenuTextPos_y += g_Vars.m_Menu.sk_menu_boxheight->value + 1;
			}
		}
	}
}
