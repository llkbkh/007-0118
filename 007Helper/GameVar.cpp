#include "GameVar.h"
#include "GameDef.h"
#include "GameOffset.h"
GameVar g_Vars;
cvar7_s g_InitVars;

GameVar::GameVar()
{
	m_GameHwnd = nullptr;
}

bool GameVar::RegisterVariable()
{
	g_InitVars.name   = "007Init";
	g_InitVars.string = "";
	g_InitVars.value  = 0;
	g_InitVars.flags  = 0;
	g_InitVars.hotkey = 0;

	DefaultVariable();

	return true;
}

bool GameVar::DefaultVariable()
{

	//注册菜单
	RegMenu();
	//注册基础功能
	RegBasics();

	RegEsp();

	RegAimBot();

	RegWallHack();

	RegKz();

	RegScript();


	RegMFCMenu();


	RegMpMenu();

	RegVxl();

	RegSpeed();

	RegSurvival();

	RegZsht();
	return true;
}

cvar7_s* GameVar::GetInitCvar()
{
	return &g_InitVars;
}

void GameVar::RegBasics()
{
	//设置基础功能 1
	SetVarsValue(&m_Basi.sk_basics_enable        , ("sk_basics_enable"), 1);
	SetVarsValue(&m_Basi.sk_basics_reconnect     , ("sk_basics_reconnect"), 0, K_0);
	SetVarsValue(&m_Basi.sk_basics_retroom       , ("sk_basics_retroom"), 0, VK_OEM_MINUS);	//-键
	SetVarsValue(&m_Basi.sk_basics_maskgamemenu  , ("sk_basics_maskgamemenu"), 0, VK_F4);
	SetVarsValue(&m_Basi.sk_basics_rpn           , ("sk_basics_rpn"), 1, K_9);
	SetVarsValue(&m_Basi.sk_basics_noblackfog    , ("sk_basics_noblackfog"), 1, K_7);
	SetVarsValue(&m_Basi.sk_basics_cardenable    , ("sk_basics_cardenable"), 0, VK_F1);
	SetVarsValue(&m_Basi.sk_basics_itemknifeid   , ("sk_basics_itemknifeid"), 0, VK_F2);
	SetVarsValue(&m_Basi.sk_basics_buymenu       , ("sk_basics_buymenu"), 0, 'T');
	SetVarsValue(&m_Basi.sk_basics_vxllight      , ("sk_basics_vxllight"), 0, 'L');
	SetVarsValue(&m_Basi.sk_basics_drone         , ("sk_basics_drone"), 0, K_4);
	SetVarsValue(&m_Basi.sk_basics_antikick      , ("sk_basics_antikick"), 0);
	SetVarsValue(&m_Basi.sk_basics_thirdperson   , ("sk_basics_thirdperson"), 0, K_8,~(1 << 0)); //默认是第三人称
	
	SetVarsValue(&m_Basi.sk_basics_viewdistance  , ("sk_basics_viewdistance"), 180);
	SetVarsValue(&m_Basi.sk_basics_viewhighly    , ("sk_basics_viewhighly"), 15);
	SetVarsValue(&m_Basi.sk_basics_idealyaw      , ("sk_basics_idealyaw"), 0);

	SetVarsValue(&m_Basi.sk_basics_healthmode    , ("sk_basics_healthmode"), 0,0, (1 << 0)| (1 << 1)/*| (1 << 2)*/); //默认就是固定方框显血


	SetVarsValue(&m_Basi.sk_basics_nightvision   , ("sk_basics_nightvision"), 0);
	SetVarsValue(&m_Basi.sk_basics_ballistic     , ("sk_basics_ballistic"), 0);
	SetVarsValue(&m_Basi.sk_basics_bighead       , ("sk_basics_bighead"), 0);
	SetVarsValue(&m_Basi.sk_basics_bigheadvalue  , ("sk_basics_bigheadvalue"), 3.0);
	SetVarsValue(&m_Basi.sk_basics_play_card     , ("sk_basics_play_card"), 0);
	SetVarsValue(&m_Basi.sk_basics_superop       , ("sk_basics_superop"), 0);
	SetVarsValue(&m_Basi.sk_basics_pictureop     , ("sk_basics_pictureop"), 0);

	SetVarsValue(&m_Basi.sk_basics_maplighten    , ("sk_basics_maplighten"), 0);
	SetVarsValue(&m_Basi.sk_basics_maskwords     , ("sk_basics_maskwords"), 0);
	SetVarsValue(&m_Basi.sk_basics_masklottery   , ("sk_basics_masklottery"), 0);
	SetVarsValue(&m_Basi.sk_basics_masknotice    , ("sk_basics_masknotice"), 0);
	SetVarsValue(&m_Basi.sk_basics_maskallmsg    , ("sk_basics_maskallmsg"), 0);
	SetVarsValue(&m_Basi.sk_basics_fly           , ("sk_basics_fly"), 0,'F',5000);
	SetVarsValue(&m_Basi.sk_basics_fly2			 , ("sk_basics_fly2"), 0,'N',0);


	SetVarsValue(&m_Basi.sk_basics_league          , ("sk_basics_league"), 0);

	SetVarsValue(&m_Basi.sk_basics_console_option   , ("sk_basics_console_option"), 0);
	SetVarsValue(&m_Basi.sk_basics_zbsunlock        , ("sk_basics_zbsunlock"), 0);
	SetVarsValue(&m_Basi.sk_basics_quest            , ("sk_basics_quest"), 0);
	SetVarsValue(&m_Basi.sk_basics_superretroom     , ("sk_basics_superretroom"), 0,'K');
	SetVarsValue(&m_Basi.sk_basics_displaykick      , ("sk_basics_displaykick"), 0,0,1 << 0);		// 默认自动反踢 flags & 1
	SetVarsValue(&m_Basi.sk_basics_roomkick		    , ("sk_basics_roomkick"), 0);
	SetVarsValue(&m_Basi.sk_basics_inventory_role   , ("sk_basics_inventory_role"), 0);
	SetVarsValue(&m_Basi.sk_basics_inventory_spray  , ("sk_basics_inventory_spray"), 0);
	SetVarsValue(&m_Basi.sk_basics_inventory_weapon , ("sk_basics_inventory_weapon"), 0);
	SetVarsValue(&m_Basi.sk_basics_damageskin       , ("sk_basics_damageskin"), 0);
	SetVarsValue(&m_Basi.sk_basics_respawneffect    , ("sk_basics_respawneffect"), 0);
	SetVarsValue(&m_Basi.sk_basics_badword          , ("sk_basics_badword"), 0);
	SetVarsValue(&m_Basi.sk_basics_deathbox         , ("sk_basics_deathbox"), 1);
	SetVarsValue(&m_Basi.sk_basics_deathico         , ("sk_basics_deathico"), 1);
	SetVarsValue(&m_Basi.sk_basics_soccer			, xorstr_("sk_basics_soccer"), 0);
	SetVarsValue(&m_Basi.sk_basics_fps              , xorstr_("sk_basics_fps"), 0,0,99);
	SetVarsValue(&m_Basi.sk_basics_activate_window	, xorstr_("sk_basics_activate_window"), 0);
	SetVarsValue(&m_Basi.sk_basics_item8530			, xorstr_("sk_basics_item8530"), 0,0,0);
	SetVarsValue(&m_Basi.sk_basics_item8133			, xorstr_("sk_basics_item8133"), 0, 0, 0);
	SetVarsValue(&m_Basi.sk_basics_discipline		, xorstr_("sk_basics_discipline"), 0, 0, 0);
	SetVarsValue(&m_Basi.sk_basics_obstp			, xorstr_("sk_basics_obstp"), 0, 0, 0);

	
}

void GameVar::RegMenu()
{
	//设置内嵌菜单属性

	SetVarsValue(&m_Menu.sk_menu_language , ("sk_menu_language"), g_Offset.GetGameServer() == Chinese ? 1 : g_Offset.GetGameServer() == TaiWan ? 2 : 3);
	SetVarsValue(&m_Menu.sk_menu_key      , ("sk_menu_key"), K_END);
	SetVarsValue(&m_Menu.sk_menu_pos_x    , ("sk_menu_pos_x"), 50);
	SetVarsValue(&m_Menu.sk_menu_pos_y    , ("sk_menu_pos_y"), 138);
	SetVarsValue(&m_Menu.sk_menu_boxheight, ("sk_menu_boxheight"), 20);
	SetVarsValue(&m_Menu.sk_menu_boxwidth , ("sk_menu_boxwidth"), 190);

	//设置菜单颜色
	m_Menu.BoxColor[0]       = 20.0;
	m_Menu.BoxColor[1]       = 20.0;
	m_Menu.BoxColor[2]       = 20.0;
	m_Menu.BoxColor[3]       = 40.0;
	m_Menu.BoxFillColor[0]   = 20.0;
	m_Menu.BoxFillColor[1]   = 20.0;
	m_Menu.BoxFillColor[2]   = 50.0;
	m_Menu.BoxFillColor[3]   = 130.0;
	m_Menu.BoxSelectColor[0] = 00.0;
	m_Menu.BoxSelectColor[1] = 00.0;
	m_Menu.BoxSelectColor[2] = 255.0;
	m_Menu.BoxSelectColor[3] = 100.0;
	m_Menu.TextColor[0]      = 255.0;
	m_Menu.TextColor[1]      = 255.0;
	m_Menu.TextColor[2]      = 255.0;
	m_Menu.TextColor[3]      = 255.0;
}


void GameVar::RegEsp()
{
	//设置Esp功能
	SetVarsValue(&m_Esp.sk_esp_enable                    , "sk_esp_enable", 0);
	SetVarsValue(&m_Esp.sk_esp_validcheck                , "sk_esp_validcheck", 1);

	SetVarsValue(&m_Esp.sk_esp_playername                , "sk_esp_playername", 0);
	SetVarsValue(&m_Esp.sk_esp_playername_team           , "sk_esp_playername_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerbox                 , "sk_esp_playerbox", 0);
	SetVarsValue(&m_Esp.sk_esp_playerbox_team            , "sk_esp_playerbox_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerweapon              , "sk_esp_playerweapon", 0,0,(1 << 0) | (1 << 1));
	SetVarsValue(&m_Esp.sk_esp_playerweapon_team         , "sk_esp_playerweapon_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerdistance            , "sk_esp_playerdistance", 0);
	SetVarsValue(&m_Esp.sk_esp_playerdistance_team       , "sk_esp_playerdistance_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playershine               , "sk_esp_playershine", 0);
	SetVarsValue(&m_Esp.sk_esp_playershine_team          , "sk_esp_playershine_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerTraceMove           , "sk_esp_playerTraceMove", 0);
	SetVarsValue(&m_Esp.sk_esp_playerTrackMove_team      , "sk_esp_playerTrackMove_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerTraceAngleLine      , "sk_esp_playerTraceAngleLine", 0);
	SetVarsValue(&m_Esp.sk_esp_playerTraceAngleLine_team , "sk_esp_playerTraceAngleLine_team", 1);
	SetVarsValue(&m_Esp.sk_esp_drawitems                 , "sk_esp_drawitems", 0,0,(1 << 0)|(1 << 1));
	SetVarsValue(&m_Esp.sk_esp_playerminiradar           , "sk_esp_playerminiradar", 0);
	SetVarsValue(&m_Esp.sk_esp_playerminiradar_team      , "sk_esp_playerminiradar_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerminiradar_size      , "sk_esp_playerminiradar_size", 4);
	SetVarsValue(&m_Esp.sk_esp_playerhintwarning         , "sk_esp_playerhintwarning", 0);
	SetVarsValue(&m_Esp.sk_esp_playerhintwarning_team    , "sk_esp_playerhintwarning_team", 1);
	SetVarsValue(&m_Esp.sk_esp_playerhintwarning_distance, "sk_esp_playerhintwarning_distance", 500);
	SetVarsValue(&m_Esp.sk_esp_grenades_parabola         , "sk_esp_grenades_parabola", 0);
	SetVarsValue(&m_Esp.sk_esp_c4timer                   , "sk_esp_c4timer", 0);
	SetVarsValue(&m_Esp.sk_esp_prophunt                  , "sk_esp_prophunt", 0);
	SetVarsValue(&m_Esp.sk_esp_patrol_info               , "sk_esp_patrol_info", 0);
	SetVarsValue(&m_Esp.sk_esp_zombie_trap               , "sk_esp_zombie_trap", 0);
	SetVarsValue(&m_Esp.sk_esp_supply                    , "sk_esp_supply", 0);
	SetVarsValue(&m_Esp.sk_esp_souds					 , "sk_esp_souds", 0,0,1 << 0 | 1<< 1 /*| 1 << 2*/);	//spr -name - distance
	SetVarsValue(&m_Esp.sk_esp_souds_team				 , "sk_esp_souds_team", 1);
	SetVarsValue(&m_Esp.sk_esp_allitem					 , "sk_esp_allitem", 0);
	
	
}

void GameVar::RegAimBot()
{
	//设置自瞄
	SetVarsValue(&m_Aimbot.sk_aimbot_enable                      , "sk_aimbot_enable", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_position                    , "sk_aimbot_position", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_camp                        , "sk_aimbot_camp", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_key                         , "sk_aimbot_key", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_angle                       , "sk_aimbot_angle", 45);
	SetVarsValue(&m_Aimbot.sk_aimbot_barrier                     , "sk_aimbot_barrier", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_aimtype                     , "sk_aimbot_aimtype", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_aimmode                     , "sk_aimbot_aimmode", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_showplayer                  , "sk_aimbot_showplayer", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_showposition                , "sk_aimbot_showposition", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_showrecoil                  , "sk_aimbot_showrecoil", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_shoot                       , "sk_aimbot_shoot", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_shoot_fov                   , "sk_aimbot_shoot_fov", 45);
	SetVarsValue(&m_Aimbot.sk_aimbot_zb_notaim                   , "sk_aimbot_zb_notaim", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_no_grenade                  , "sk_aimbot_no_grenade", 1);
	SetVarsValue(&m_Aimbot.sk_aimbot_NoRecoil                    , "sk_aimbot_NoRecoil", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_NoRecoil_Visual             , "sk_aimbot_NoRecoil_Visual", 0);
	//默认 锁定 - 螺旋移动
	SetVarsValue(&m_Aimbot.sk_aimbot_MouseMoveSpeed              , "sk_aimbot_MouseMoveSpeed", 0,0,1);
	//反冲补偿
	SetVarsValue(&m_Aimbot.sk_aimbot_RecoilCompensationPitch     , "sk_aimbot_RecoilCompensationPitch", 2);
	SetVarsValue(&m_Aimbot.sk_aimbot_RecoilCompensationYaw       , "sk_aimbot_RecoilCompensationYaw", 2);
	//反冲补偿模式
	SetVarsValue(&m_Aimbot.sk_aimbot_RecoilCompensationMode      , "sk_aimbot_RecoilCompensationMode", 0);
	//默认 hitBox
	SetVarsValue(&m_Aimbot.sk_aimbot_HitBoxMode                  , "sk_aimbot_HitBoxMode", 0);
	//武器判断
	SetVarsValue(&m_Aimbot.sk_aimbot_WeaponCanAttack             , "sk_aimbot_WeaponCanAttack", 0);
	//默认矩阵是不调整的
	SetVarsValue(&m_Aimbot.sk_aimbot_MatrixAdjust                , "sk_aimbot_MatrixAdjust", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_MatrixAdjust_X              , "sk_aimbot_MatrixAdjust_X", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_MatrixAdjust_Y              , "sk_aimbot_MatrixAdjust_Y", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_MatrixAdjust_Z              , "sk_aimbot_MatrixAdjust_Z", 0);
	SetVarsValue(&m_Aimbot.sk_aimbot_RecoilCompensationMode_DEBUG, ("sk_aimbot_RecoilCompensationMode_DEBUG"), 0);
	
}

void GameVar::RegWallHack()
{
	//设置透视
	SetVarsValue(&m_WallHack.sk_wall_enable           , "sk_wall_enable", 0);
	SetVarsValue(&m_WallHack.sk_wall_wallhacktype     , "sk_wall_wallhacktype", 0);
	SetVarsValue(&m_WallHack.sk_wall_halfwallhackalpha, "sk_wall_halfwallhackalpha", 0.75);
	SetVarsValue(&m_WallHack.sk_wall_wallhackteam     , "sk_wall_wallhackteam", 1);
	SetVarsValue(&m_WallHack.sk_wall_showghost        , "sk_wall_showghost", 0);
	SetVarsValue(&m_WallHack.sk_wall_playerlighten    , "sk_wall_playerlighten", 0);
	SetVarsValue(&m_WallHack.sk_wall_sightbead        , "sk_wall_sightbead", 0);
	SetVarsValue(&m_WallHack.sk_wall_nosmoke          , "sk_wall_nosmoke", 1);
	SetVarsValue(&m_WallHack.sk_wall_noflash          , "sk_wall_noflash", 1);
	SetVarsValue(&m_WallHack.sk_wall_bleeding         , "sk_wall_bleeding", 0);
	SetVarsValue(&m_WallHack.sk_wall_outline          , "sk_wall_outline", 0);
	SetVarsValue(&m_WallHack.sk_wall_outline_team     , "sk_wall_outline_team", 3);
	SetVarsValue(&m_WallHack.sk_wall_outline_ct_r     , "sk_wall_outline_ct_r", 76);
	SetVarsValue(&m_WallHack.sk_wall_outline_ct_g     , "sk_wall_outline_ct_g", 208);
	SetVarsValue(&m_WallHack.sk_wall_outline_ct_b     , "sk_wall_outline_ct_b", 130);
	SetVarsValue(&m_WallHack.sk_wall_outline_t_r      , "sk_wall_outline_t_r", 255);
	SetVarsValue(&m_WallHack.sk_wall_outline_t_g      , "sk_wall_outline_t_g", 0);
	SetVarsValue(&m_WallHack.sk_wall_outline_t_b      , "sk_wall_outline_t_b", 0);
}

void GameVar::RegKz()
{
	SetVarsValue(&m_Kz.sk_kreedz_enable                 , "sk_kreedz_enable", 0);
	SetVarsValue(&m_Kz.sk_kreedz_mode                   , "sk_kreedz_mode", 2);
	SetVarsValue(&m_Kz.sk_kreedz_bunnyhop               , "sk_kreedz_bunnyhop", 0);
	SetVarsValue(&m_Kz.sk_kreedz_bunnyhop_autoduck      , "sk_kreedz_bunnyhop_autoduck", 0);
	SetVarsValue(&m_Kz.sk_kreedz_autostrafe             , "sk_kreedz_autostrafe", 0);
	SetVarsValue(&m_Kz.sk_kreedz_autostrafe_strafedi    , "sk_kreedz_autostrafe_strafedi", 1);
	SetVarsValue(&m_Kz.sk_kreedz_autostrafe_autodir     , "sk_kreedz_autostrafe_autodir", 1);
	SetVarsValue(&m_Kz.sk_kreedz_groundstrafe           , "sk_kreedz_groundstrafe", 0);
	SetVarsValue(&m_Kz.sk_kreedz_groundstrafe_mscj      , "sk_kreedz_groundstrafe_mscj", 0);
	SetVarsValue(&m_Kz.sk_kreedz_groundstrafe_noslowdown, "sk_kreedz_groundstrafe_noslowdown", 1);
	SetVarsValue(&m_Kz.sk_kreedz_groundstrafea_speed    , "sk_kreedz_groundstrafea_speed", 2000);
	SetVarsValue(&m_Kz.sk_kreedz_strafe_sidemove        , "sk_kreedz_strafe_sidemove", 437.8928);	//好像速度不是越大越好 999
	SetVarsValue(&m_Kz.sk_kreedz_strafe_angle           , "sk_kreedz_strafe_angle", 30);
	SetVarsValue(&m_Kz.sk_kreedz_strafe_speed           , "sk_kreedz_strafe_speed", 69/*25*/);			//这个也一样
	SetVarsValue(&m_Kz.sk_kreedz_strafehelper           , "sk_kreedz_strafehelper", 0);
	SetVarsValue(&m_Kz.sk_kreedz_groundspeed            , "sk_kreedz_groundspeed", 0);
	SetVarsValue(&m_Kz.sk_kreedz_jumpbug                , "sk_kreedz_jumpbug", 0);
	SetVarsValue(&m_Kz.sk_kreedz_spinhack               , "sk_kreedz_spinhack", 0);
	SetVarsValue(&m_Kz.sk_kreedz_spinhackSpeed			, "sk_kreedz_spinhackSpeed", 6);
	SetVarsValue(&m_Kz.sk_kreedz_info					, "sk_kreedz_info", 0);



}


void GameVar::SetVarsValue(cvar7_s** Vars, char* name, float Value, DWORD dwHotKey /*= 0*/, ULONG Flag /*= 0*/)
{
	cvar7_s* pVars = &g_InitVars;

	//添加一个链表
	while (pVars)
	{
		//已有相同的var
		if (lstrcmpi(pVars->name.data(), name) == 0)
		{
			pVars->value  = Value;
			pVars->hotkey = dwHotKey;
			pVars->flags  = Flag;
			break;
		}
		//如果下一项链表 = NULL 则new 一个出来
		if (pVars->next == NULL)
		{
			//赋值
			*Vars           = new cvar7_s;
			
			(*Vars)->name   = name;
			(*Vars)->value  = Value;
			(*Vars)->string = "";
			(*Vars)->hotkey = dwHotKey;
			(*Vars)->flags  = Flag;
			(*Vars)->next   = nullptr;
			// 当前链表下一项      = 此链表
			pVars->next     = *Vars;
			break;
		}
		pVars = pVars->next;
	}
}


void GameVar::RegScript()
{
	SetVarsValue(&m_Script.sk_script_enable         , xorstr_("sk_script_enable"), 1);
	SetVarsValue(&m_Script.sk_script_zbs_autogank_ct, xorstr_("sk_script_zbs_autogank_ct"), 0,0,(1 << 0) | (1 << 1) /*| (1 << 2)*/);	//敌人隐身不抓 | 圣剑盾反状态不抓 | 障碍物判断
	SetVarsValue(&m_Script.sk_script_zbs_counter    , xorstr_("sk_script_zbs_counter"), 0);
	SetVarsValue(&m_Script.sk_script_zb5_auto_skill , xorstr_("sk_script_zb5_auto_skill"), 0);
	SetVarsValue(&m_Script.sk_script_god            , xorstr_("sk_script_god"), 0,'C',0);
	SetVarsValue(&m_Script.sk_script_excalibur		, xorstr_("sk_script_excalibur"), 0, 'X', 0);

}

void GameVar::RegMFCMenu()
{
	SetVarsValue(&m_MFCMenu.sk_mfcMenu_hotkey     , "sk_mfcMenu_hotkey", 1, VK_HOME | 0x800);
	SetVarsValue(&m_MFCMenu.sk_mfcMenu_enable_tips, "sk_mfcMenu_enable_tips", 1);



}

void GameVar::RegMpMenu()
{
	SetVarsValue(&m_Mp.sk_mp_enable       , xorstr_("sk_mp_enable"), 0);
	SetVarsValue(&m_Mp.sk_mp_weapon       , xorstr_("sk_mp_weapon"), 0, VK_NUMPAD0,0);	//小键盘0
	SetVarsValue(&m_Mp.sk_mp_mode         , xorstr_("sk_mp_mode"), 1);
	SetVarsValue(&m_Mp.sk_mp_gamemode     , xorstr_("sk_mp_gamemode"), 0);
	SetVarsValue(&m_Mp.sk_mp_gamemap      , xorstr_("sk_mp_gamemap"), 0);
	SetVarsValue(&m_Mp.sk_mp_GainMut      , xorstr_("sk_mp_GainMut"), 0);
	SetVarsValue(&m_Mp.sk_mp_health       , xorstr_("sk_mp_health"), 0);
	SetVarsValue(&m_Mp.sk_mp_god          , xorstr_("sk_mp_god"), 0);
	SetVarsValue(&m_Mp.sk_mp_speed        , xorstr_("sk_mp_speed"), 0);
	SetVarsValue(&m_Mp.sk_mp_ammo_max     , xorstr_("sk_mp_ammo_max"), 0);
	SetVarsValue(&m_Mp.sk_mp_ammo_speed   , xorstr_("sk_mp_ammo_speed"), 0);
	SetVarsValue(&m_Mp.sk_mp_weapon_recoil, xorstr_("sk_mp_weapon_recoil"), 0);
	SetVarsValue(&m_Mp.sk_mp_highjump     , xorstr_("sk_mp_highjump"), 0);
	SetVarsValue(&m_Mp.sk_mp_soul         , xorstr_("sk_mp_soul"), 0, VK_NUMPAD8,0);	//小键盘8
	SetVarsValue(&m_Mp.sk_mp_sop		  , xorstr_("sk_mp_sop"), 0, VK_NUMPAD1, 0);	//小键盘1
	SetVarsValue(&m_Mp.sk_mp_rpn		  , xorstr_("sk_mp_rpn"), 0, VK_NUMPAD9, 0);	//小键盘9
	
	
}

void GameVar::RegVxl()
{
	SetVarsValue(&m_Vxl.sk_vxl_enable , xorstr_("sk_vxl_enable"), 1);
	SetVarsValue(&m_Vxl.sk_vxl_destroy, xorstr_("sk_vxl_destroy"), 0, VK_OEM_4);	//[键
	SetVarsValue(&m_Vxl.sk_vxl_sky    , xorstr_("sk_vxl_sky"), 0);
	SetVarsValue(&m_Vxl.sk_vxl_weather, xorstr_("sk_vxl_weather"), 0);
	SetVarsValue(&m_Vxl.sk_vxl_light  , xorstr_("sk_vxl_light"), 0);
	SetVarsValue(&m_Vxl.sk_vxl_restart, xorstr_("sk_vxl_restart"), 0, VK_OEM_6);		//]键
	
}

void GameVar::RegSpeed()
{
	SetVarsValue(&m_Speed.sk_speed_enable         , xorstr_("sk_speed_enable"), 0);
	SetVarsValue(&m_Speed.sk_speed_mode           , xorstr_("sk_speed_mode"), 1);
	SetVarsValue(&m_Speed.sk_speed_key            , xorstr_("sk_speed_key"), 0);
	SetVarsValue(&m_Speed.sk_speed_value          , xorstr_("sk_speed_value"), 1);
	SetVarsValue(&m_Speed.sk_speed_reconnect      , xorstr_("sk_speed_reconnect"), 0);
	SetVarsValue(&m_Speed.sk_speed_reconnect_timer, xorstr_("sk_speed_reconnect_timer"), 5);
}

void GameVar::RegSurvival()
{
	SetVarsValue(&m_Survial.sk_cata_enable    , xorstr_("sk_cata_enable"), 0);
	SetVarsValue(&m_Survial.sk_cata_armor     , xorstr_("sk_cata_armor"), 0,'C');
	SetVarsValue(&m_Survial.sk_cata_attack    , xorstr_("sk_cata_attack"),0, 'K');
	SetVarsValue(&m_Survial.sk_cata_hp        , xorstr_("sk_cata_hp"), 0,'Z');
	SetVarsValue(&m_Survial.sk_cata_bullet    , xorstr_("sk_cata_bullet"), 0, VK_OEM_COMMA);
	SetVarsValue(&m_Survial.sk_cata_bullet2   , xorstr_("sk_cata_bullet2"),0, VK_OEM_PERIOD);
	SetVarsValue(&m_Survial.sk_cata_grenades  , xorstr_("sk_cata_grenades"), 0, K_4,0);
	SetVarsValue(&m_Survial.sk_cata_autobullet, xorstr_("sk_cata_autobullet"), 0);
	SetVarsValue(&m_Survial.sk_cata_autoarmor , xorstr_("sk_cata_autoarmor"), 0);
	SetVarsValue(&m_Survial.sk_cata_atck_up   , xorstr_("sk_cata_atck_up"), 0);
	SetVarsValue(&m_Survial.sk_cata_drug_30   , xorstr_("sk_cata_drug_30"), 0,0,20);
	SetVarsValue(&m_Survial.sk_cata_drug_100  , xorstr_("sk_cata_drug_100"), 0,0,15);
	SetVarsValue(&m_Survial.sk_cata_afk       , xorstr_("sk_cata_afk"), 0);
	SetVarsValue(&m_Survial.sk_cata_role	  , xorstr_("sk_cata_role"), 0,0,10);
	SetVarsValue(&m_Survial.sk_cata_start_game, xorstr_("sk_cata_start_game"), 0);
	
}

void GameVar::RegZsht()
{
	SetVarsValue(&m_Zsht.sk_zsht_enable, xorstr_("sk_zsht_enable"), 0);
	SetVarsValue(&m_Zsht.sk_zsht_transfer, xorstr_("sk_zsht_transfer"), 0,'K',0 );
	SetVarsValue(&m_Zsht.sk_zsht_contribute, xorstr_("sk_zsht_contribute"), 0, 'P', 0);
	SetVarsValue(&m_Zsht.sk_zsht_resource, xorstr_("sk_zsht_resource"), 0, 'L', 0);
	SetVarsValue(&m_Zsht.sk_zsht_loopbuild, xorstr_("sk_zsht_loopbuild"), 0, 'R', 1);
	SetVarsValue(&m_Zsht.sk_zsht_loopbuild_x, xorstr_("sk_zsht_loopbuild_x"), 0);
	SetVarsValue(&m_Zsht.sk_zsht_loopbuild_y, xorstr_("sk_zsht_loopbuild_y"), 33);
	SetVarsValue(&m_Zsht.sk_zsht_loopbuild_z, xorstr_("sk_zsht_loopbuild_z"), 20);

}

