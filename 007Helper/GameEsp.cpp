#include "GameEsp.h"
#include "Drawing.h"
#include <regex>
#include "Util.h"
#include "cSprites.h"
#include "GameOffset.h"
#include "GameCFG.h"

void (*PfnPreS_DynamicSoud)(int entid, DWORD entchannel, char* szSoundFile, float* fOrigin, float fVolume, float fAttenuation, int iTimeOff, int iPitch) = nullptr;


GameEsp::GameEsp()
{
	m_GamePlayer = nullptr;
	m_Local      = nullptr;
}

GameEsp* GameEsp::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new GameEsp();
	}

	return m_Instance;
}

bool GameEsp::vInit()
{
	m_GamePlayer = GamePlayer::GetInstance();
	m_Local      = m_GamePlayer->GetLocal();
	
	//->不底层
	//Engine::g_pEngine->pEventAPI->EV_PlaySound = EV_PlaySound;
	g_pUtil->GetAceHook()->InlineHook(g_Offset.GetGameAddresInfo()->PreS_DynamicSoundCall,0, PreS_DynamicSound,&PfnPreS_DynamicSoud);


	return true;
}

void GameEsp::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_Esp.sk_esp_enable->value)
		return;

	m_PlayerNumer = 0;

	m_CurrentBackDistanceUnits = g_Vars.m_Esp.sk_esp_playerhintwarning_distance->value;

	if (g_Vars.m_Esp.sk_esp_souds->value)
	{
		//如果超过了指定时间 则弹出
		while (m_Sounds.size() && GetTickCount() - m_Sounds.front().timestamp >= 300)
			m_Sounds.pop_front();

		SoundESP();
	}




	for (int Index = 1; Index <= Engine::g_Engine.GetMaxClients(); ++Index)
	{
		if (m_GamePlayer->GetPlayer(Index)->Valid && Index != m_Local->Player.Index)
		{
			DrawPlayer(Index);
		}
	}

	// * 显示预警信息
	if (g_Vars.m_Esp.sk_esp_playerhintwarning->value)
		DrawWarning();
	if (g_Vars.m_Esp.sk_esp_patrol_info->value && g_Vars.m_Esp.sk_esp_patrol_info->flags)
		//显示 无人机信息
		DrawPatrolDrone();

	// * 显示C4爆炸时间
	if (g_Vars.m_Esp.sk_esp_c4timer->value && m_Local->Player.BombDrop)
	{
		//if (!(g_Util.CheckGameModeIsZombi() && Info::g_Local.Player.Team == TERRORIST))
		{
			DrawC4Info();
		}
	}

	DrawWorld();
}

void GameEsp::vV_CalcRefdef(ref_params_s* pparams)
{
	if (pparams->nextView)
	{
		VectorCopy(pparams->viewangles, m_ViewAngles);
		VectorCopy(pparams->vieworg, m_ViewOrigin);
	}
}

void GameEsp::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	if (!g_Vars.m_Esp.sk_esp_enable->value)
		return;

	// * 显示预测手雷轨迹
	if ( g_Vars.m_Esp.sk_esp_grenades_parabola->value)
		GranadeTrailFunction(cmd);


}

void GameEsp::vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{
	if (!g_Vars.m_Esp.sk_esp_enable->value || !ent)
		return;

	AddWorld(ent, modelname);


	// * 显示追踪视角瞄准线
	if (g_Vars.m_Esp.sk_esp_playerTraceAngleLine->value)
		DrawTraceAngleLine(ent);
	if (g_Vars.m_Esp.sk_esp_playerTraceMove->value)
		DrawTraceMove(ent);


}

int GameEsp::vBombDrop(const char* pszName, int iSize, void* pbuf)
{
	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);

	for (size_t i = 0; i < 3; i++)
	{
		g_Vars.m_Esp.m_C4Coord[i] = lReader->ReadCoord();

	}

	m_Local->Player.BombDrop = lReader->ReadChar();

	if (m_Local->Player.BombDrop)
	{
		g_Vars.m_Esp.m_C4BombDropTick = GetTickCount();
	}

	return 1;
}

int GameEsp::vResetHUD(const char* pszName, int iSize, void* pbuf)
{
	m_Local->Player.BombDrop = 0;
	m_Entity.clear();
	g_Vars.m_Esp.sk_esp_patrol_info->flags = 0;

	return 1;
}

int GameEsp::vInitHUD(const char* pszName, int iSize, void* pbuf)
{
	g_Sprite.SpriteInit();
	m_Entity.clear();
	g_Vars.m_Esp.sk_esp_patrol_info->flags = 0;

	return 1;
}

int GameEsp::vPatrolDrone(const char* pszName, int iSize, void* pbuf)
{
	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);
	int Type            = lReader->ReadChar();
	BOOL bIsLocal       = FALSE;

	if (!m_Local->Entity)
		return 1;

	switch (Type)
	{
	case 0:		//手动状态
	{
		int Index = lReader->ReadInt();
		int State = lReader->ReadChar();		// 1 巡逻中   2 轰炸中
	}
	break;
	case 1:		//巡逻中
	{
		int Index                  = lReader->ReadInt();
		int Core                   = lReader->ReadChar();
		if (m_Local->Entity->index == Index)
		{
			m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
			m_PatrolDrone.Core[Core].State       = State_Patrol;
			bIsLocal                             = TRUE;
		}
	}
	break;
	case 2:		//待机中
	{
		int Index                  = lReader->ReadInt();
		int Core                   = lReader->ReadChar();
		if (m_Local->Entity->index == Index)
		{
			m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
			m_PatrolDrone.Core[Core].State       = State_Standby;
			bIsLocal                             = TRUE;
		}
	}
	break;
	case 3:		//已被破坏
	{
		int Index                  = lReader->ReadInt();
		int Core                   = lReader->ReadChar();
		if (m_Local->Entity->index == Index)
		{
			m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
			m_PatrolDrone.Core[Core].State       = State_Destroy;
			bIsLocal                             = TRUE;
		}

	}
	break;
	case 4:
	{
		int Index                  = lReader->ReadInt();
		int Core                   = lReader->ReadChar();
		int State                  = lReader->ReadInt();
		if (m_Local->Entity->index == Index)
		{
			m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
			// 状态 > 0 等于 轰炸中 =  0 巡逻中
			m_PatrolDrone.Core[Core].State       = State > 0 ? State_Bomb : State_Patrol;
			bIsLocal                             = TRUE;
		}
	}
	break;
	case 5:	// 持续时间
	{
		int Core                             = lReader->ReadChar();
		int CurTimer                         = lReader->ReadInt();
		int v3                               = lReader->ReadInt();
		int v4                               = lReader->ReadInt();
		m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
		m_PatrolDrone.Core[Core].Duration    = CurTimer;
		bIsLocal                             = TRUE;

		break;
	}
	case 6:		//初始化
	{
		int Index = lReader->ReadInt();
		int Core  = lReader->ReadChar();
		int State = lReader->ReadInt();

		if (m_Local->Entity->index == Index)
		{
			m_PatrolDrone.Core[Core].CurrentCore = Core + 1;
			// 状态 > 0 等于 轰炸中  =  0 巡逻中
			m_PatrolDrone.Core[Core].State       = State == 0 ? State_Standby : State_Destroy;
			bIsLocal                             = TRUE;
		}
		break;
	}

	}

	if (bIsLocal)
		g_Vars.m_Esp.sk_esp_patrol_info->flags = 1;

	return 1;
}

void GameEsp::DrawPlayer(int Index)
{
	//开启检查且 自己死亡 则返回 否则正常

	if (g_Vars.m_Esp.sk_esp_validcheck->value)
	{
		cl_entity_s* pEntry = Engine::g_Engine.GetLocalPlayer();

		if (!pEntry || !pEntry->index || !m_Local->Player.isAlive)
		{
			return;
		}
	}

	auto Player = m_GamePlayer->GetPlayer(Index);


	if (Player->Entity == NULL || Player->Dormant || !Player->Valid)
		return;
	if (Player->Info.name == NULL)
		return;

	//获取颜色
	GetColor(Index);
	// * 取出画出ESP骨骼位置
	int nEspBones = m_GamePlayer->GetBonePartIndex(Index, HITBOX_TYPE::HEAD).at(0);

	//地图雷达
	if (g_Vars.m_Esp.sk_esp_playerminiradar->value && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerminiradar_team->value))
	{
		drawMiniRadarPoint(Player->Entity->origin, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], true, g_Vars.m_Esp.sk_esp_playerminiradar_size->value);
	}
	//显示敌人预警
	if (g_Vars.m_Esp.sk_esp_playerhintwarning->value && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerhintwarning_team->value))
	{
		if ((g_Vars.m_Esp.sk_esp_playerhintwarning->value == 1 && !IsScreenIn(Index)) || (g_Vars.m_Esp.sk_esp_playerhintwarning->value == 3 && IsScreenIn(Index) || g_Vars.m_Esp.sk_esp_playerhintwarning->value == 2))
		{
			m_PlayerNumer++;

			int  BackDistanceUnits = int(m_GamePlayer->GetPlayerDistance(Index, false));
			if (BackDistanceUnits < m_CurrentBackDistanceUnits)
			{
				m_CurrentBackDistanceUnits = BackDistanceUnits;
				m_CurrentPlayerIndex = Index;
			}
		}
	}


	// * 是否在屏幕内 > 45
	Vector out;
	if (!IsScreenIn(Index)) return;
	float  ScreenTop[3], ScreenBot[3];

	//判断手枪隐身  真 则用坐标显示 
	if (!(m_GamePlayer->GetScreenInfo(Index, ScreenTop, 1, !Player->bVisible, nEspBones) && m_GamePlayer->GetScreenInfo(Index, ScreenBot, 2, !Player->bVisible, nEspBones)))
	{
		return;
	}

	float Height = ScreenBot[1] - ScreenTop[1];
	float PlayerBoxHeight = Player->Ducked ? Height : Height * 0.9f;

	// * 显示名字
	if (g_Vars.m_Esp.sk_esp_playername->value) { DrawPlayerName(Index, ScreenTop, ScreenBot, Height, PlayerBoxHeight); }
	// * 显示方框
	if (g_Vars.m_Esp.sk_esp_playerbox->value) { DrawPlayerBox(Index, ScreenTop, ScreenBot, !Player->bVisible, nEspBones); }
	// * 显示武器模型和代码
	if (g_Vars.m_Esp.sk_esp_playerweapon->value && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerweapon_team->value))
	{
		DrawWeapon(Index, ScreenTop, ScreenBot);
	}
	// * 显示距离
	if (g_Vars.m_Esp.sk_esp_playerdistance->value) { DrawDistance(Index, ScreenTop, ScreenBot); }
	// * 人物发光
	if (g_Vars.m_Esp.sk_esp_playershine->value == 1 && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playershine_team->value))
	{
		ColorEntry color1;
		color1.r = m_BoxColor[0], color1.g = m_BoxColor[1], color1.b = m_BoxColor[2];
		DoExtraRendering(Player->Entity, color1);
	}


}

void GameEsp::DrawPlayerName(int Index, float ScreenTop[3], float ScreenBot[3], float Height, float PlayerBoxHeight)
{
	if ((g_Vars.m_Esp.sk_esp_playername->value == 1) && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playername_team->value))
	{

		g_Font.FmtDrawString(int(ScreenTop[0]) - 10, int(PlayerBoxHeight + ScreenTop[1] - 5),
			m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], "{}", m_GamePlayer->GetPlayer(Index)->Info.name);
	}
	// 显示 外挂用户  ___
}

void GameEsp::DrawPlayerBox(int Index, float ScreenTop[3], float ScreenBot[3], bool bVisible, int Bones)
{

	if (m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerbox_team->value) && g_Vars.m_Esp.sk_esp_playerbox->value > 0)
	{
		if (g_Vars.m_Esp.sk_esp_playerbox->value == 1)
		{
			//正常的.
			Vector out;
			m_GamePlayer->GetScreenInfo(Index, out, 0, bVisible, Bones);

			g_Drawing.DrawBox(out.x, out.y, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], 255, 8, 2);
		}
	}
}

void GameEsp::DrawWeapon(int Index, float ScreenTop[3], float ScreenBot[3])
{
	

	float Height = ScreenBot[1] - ScreenTop[1];

	float PlayerBoxHeight = m_GamePlayer->GetPlayer(Index)->Ducked ? Height : Height * 0.9f;

	model_s* Model = Engine::g_Studio.GetModelByIndex(m_GamePlayer->GetPlayer(Index)->Entity->curstate.weaponmodel);

	float Weapon = ScreenBot[1] + (12 - (ScreenBot[1] - ScreenTop[1]));

	float DistanceWeapon = ScreenBot[1] + (24 - (ScreenBot[1] - ScreenTop[1]));

	float PlayerWeaponPos_y = g_Vars.m_Esp.sk_esp_playerdistance->value ? DistanceWeapon : Weapon;

	if (Model && Model->name)
	{
		
		std::string Name = Model->name;

		//取中间匹配的字符串.(任意字符必须出现1到多次)
		std::regex r(xorstr_("models/p_(.+)\.mdl"));

		std::sregex_iterator pos(Name.cbegin(), Name.cend(), r);
		std::sregex_iterator end;

		for (; pos != end; ++pos)
		{
			if (pos->size() < 2)
			{
				continue;;
			}
			std::string WeaponCode = pos->str(1);

			//显示武器翻译的名称
			if (g_Vars.m_Esp.sk_esp_playerweapon->flags & (1 << 0))
			{
				auto Utf8_WeaponName =  g_pUtil->GetCSOItemName(WeaponCode.data());
				//找到了.证明失败
				bool  bFail = Utf8_WeaponName.find(xorstr_("CSO_Item_Name_")) != Utf8_WeaponName.npos;

				g_Font.FmtDrawString(int(ScreenTop[0]) - 10, int(PlayerBoxHeight+ ScreenTop[1]) + 10, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], bFail ? WeaponCode: Utf8_WeaponName);

			}
			//显示武器模型
			if (g_Vars.m_Esp.sk_esp_playerweapon->flags & (1 << 1))
			{
				// * 画出武器模型

				std::string Text = fmt::format(xorstr_("d_{}"), WeaponCode);
				int index        = g_Sprite.GetSpriteIndex(Text.data());
				if (index != -1)
				{

					Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2]); // Get sprite ready
					Engine::g_Engine.pfnSPR_DrawAdditive(0, int(ScreenTop[0]) - 10, int(PlayerBoxHeight - 5 + ScreenTop[1]) - 10, &(spritedata.spriteinfo[index].rc)); // Draw sprite
				}

			}



		}

	}
}
float  Interp(float s1, float s2, float s3, float f1, float f3)
{
	if (s2 == s1)
	{
		return f1;
	}

	if (s2 == s3)
	{
		return f3;
	}

	if (s3 == s1)
	{
		return f1;
	}

	return f1 + ((s2 - s1) / (s3 - s1)) * ((f3 - f1));
}

void GameEsp::DrawDistance(int Index, float ScreenTop[3], float ScreenBot[3])
{
	float DistancePos_y = ScreenBot[1] + (12 - (ScreenBot[1] - ScreenTop[1]));
	// 显示米单位
	short DistanceUnits = int(m_GamePlayer->GetPlayerDistance(Index, false));

	short DistanceMeters = int(m_GamePlayer->GetPlayerDistance(Index, true));

	BYTE PlayerDistanceColor[3];

	if (DistanceUnits >= 0 && DistanceUnits <= 1000)
	{
		PlayerDistanceColor[0] = 255;
		PlayerDistanceColor[1] = DistanceUnits >= 500 ? Interp(500, DistanceUnits, 1000, 255, 80) : 255;
		PlayerDistanceColor[2] = DistanceUnits <= 500 ? Interp(0, DistanceUnits, 500, 255, 80) : 80;
	}
	else if (DistanceUnits > 1000)
	{
		PlayerDistanceColor[0] = 255;
		PlayerDistanceColor[1] = 80;
		PlayerDistanceColor[2] = 80;
	}

/*	if (g_Vars.m_Esp.sk_esp_playerdistance->value == 2 && IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerdistance_team->value))
	{
		g_Font.DrawString(int(ScreenTop[0]), int(DistancePos_y), PlayerDistanceColor[0], PlayerDistanceColor[1],
			PlayerDistanceColor[2], "%d ", DistanceUnits);
	}
	else */
	if (g_Vars.m_Esp.sk_esp_playerdistance->value == 1 && m_GamePlayer->IsSetTeam(Index, g_Vars.m_Esp.sk_esp_playerdistance_team->value))
	{
		g_Font.DrawString(int(ScreenTop[0]), int(DistancePos_y), PlayerDistanceColor[0], PlayerDistanceColor[1],
			PlayerDistanceColor[2], "%d m", DistanceMeters);
	}
}

void GameEsp::GetColor(uint16_t Index)
{
	for (uint16_t i = 0; i < sizeof(m_BoxColor); ++i)
	{
		int Bones = m_GamePlayer->GetBonePartIndex(Index, HITBOX_TYPE::HEAD).at(0);

		if (!m_GamePlayer->PathFree(m_Local->Player.vEye, m_GamePlayer->GetPlayer(Index)->Bone[Bones]))
		{
			m_GamePlayer->GetPlayer(Index)->Visible = false;
			//不可见
			if (m_GamePlayer->GetPlayer(Index)->Team == TERRORIST)
			{
				//黄色
				m_BoxColor[0] = 255;
				m_BoxColor[1] = 255;
				m_BoxColor[2] = 0;
			}
			else if (m_GamePlayer->GetPlayer(Index)->Team == CT)
			{
				//绿色方框
				m_BoxColor[0] = 0;
				m_BoxColor[1] = 255;
				m_BoxColor[2] = 255;
			}
		}
		else
		{
			m_GamePlayer->GetPlayer(Index)->Visible = true;
			//可见
			if (m_GamePlayer->GetPlayer(Index)->Team == TERRORIST)
			{
				//红色
				m_BoxColor[0] = 255;
				m_BoxColor[1] = 0;
				m_BoxColor[2] = 0;
			}
			else if (m_GamePlayer->GetPlayer(Index)->Team == CT)
			{
				//蓝色方框
				m_BoxColor[0] = 0;
				m_BoxColor[1] = 0;
				m_BoxColor[2] = 255;
			}

		}
	}





}

void GameEsp::DoExtraRendering(cl_entity_s* ent, ColorEntry color)
{
	typedef dlight_t* (*CL_AllocDlight)			(int key);
	static CL_AllocDlight AllocDlight = nullptr;
	ULONG ulBeamPointsEntry = g_Offset.GetGameAddresInfo()->dwBeamPoints;
	if (!AllocDlight)
	{
		__asm
		{
			pushad
			mov eax, ulBeamPointsEntry
			mov ecx, dword ptr ds : [eax]
			mov edx, dword ptr ds : [ecx + 00000114h]
			mov AllocDlight, edx
			popad
		}
	}
	dlight_t* pDl = AllocDlight(0);
	pDl->color.r = color.r;
	pDl->color.g = color.g;
	pDl->color.b = color.b;
	pDl->origin = ent->origin;
	pDl->die = Engine::g_Engine.GetClientTime() + 0.1;
	pDl->radius = 100;
}

void GameEsp::drawMiniRadarPoint(const float* origin, int r, int g, int b, bool addbox, int boxsize)
{
	int screenx, screeny;
	calcRadarPoint(origin, screenx, screeny);

	Engine::g_Engine.pfnFillRGBA(screenx - 1, screeny - 1, boxsize, boxsize, r, g, b, 255);

	if (addbox)g_Drawing.blackBorder(screenx - 1, screeny - 1, boxsize, boxsize + 1);
}

void GameEsp::calcRadarPoint(const float* origin, int& screenx, int& screeny)
{
	float dx = origin[0] - m_Local->Player.vEye[0];
	float dy = origin[1] - m_Local->Player.vEye[1];

	// rotate
	float yaw           = m_ViewAngles[1] * (M_PI / 180.0);
	float sin_yaw       = sin(yaw);
	float minus_cos_yaw = -cos(yaw);
	float x             = dy * minus_cos_yaw + dx * sin_yaw;
	float y             = dx * minus_cos_yaw - dy * sin_yaw;
	float range         = 2500.0f;//float& range = cvar.radar_range;
	if (fabs(x) > range || fabs(y) > range)
	{
		// clipping
		if (y > x)
		{
			if (y > -x)
				x = range * x / y, y = range;
			else
				y = -range * y / x, x = -range;
		}
		else
		{
			if (y > -x)
				y = range * y / x, x = range;
			else
				x = -range * x / y, y = -range;
		}
	}
	int displayCenterX = 90;
	int displayCenterY = 90;
	switch ((int)g_Vars.m_Esp.sk_esp_playerminiradar->value)
	{
	case 1:		//屏幕中间
		displayCenterX = Engine::g_Screen.iWidth / 2;
		displayCenterY = Engine::g_Screen.iHeight / 2;
		break;
	case 2:		//左上角
		displayCenterX = 90;
		displayCenterY = 90;
		break;
	case 3:		//右上角
		displayCenterX = Engine::g_Screen.iWidth - 90;
		displayCenterY = 90;
		break;
	case 4:		//中间靠左
		displayCenterX = 90;
		displayCenterY = Engine::g_Screen.iHeight / 2;
		break;
	case 5:		//中间靠右
		displayCenterX = Engine::g_Screen.iWidth - 90;
		displayCenterY = Engine::g_Screen.iHeight / 2;
		break;
	case 6:		//左下角
		displayCenterX = 90;
		displayCenterY = Engine::g_Screen.iHeight - 90;
		break;
	case 7:		//右下角
		displayCenterX = Engine::g_Screen.iWidth - 90;
		displayCenterY = Engine::g_Screen.iHeight - 90;
		break;

	default:
		break;
	}

	screenx = displayCenterX + int(x / range * float(100));
	screeny = displayCenterY + int(y / range * float(100));
}

BOOL GameEsp::IsScreenIn(int Index)
{
	//按照人物坐标来计算的
	Vector out;
	return WorldToScreen( m_GamePlayer->GetPlayer(Index)->Entity->origin, out);
}

void GameEsp::DrawWarning()
{
	if (m_PlayerNumer == 0) return;

	std::string Text =  fmt::format(g_Vars.m_Esp.sk_esp_playerhintwarning->value == 1 ? GetInMenuSubItem(xorstr_("#Hint_Warnig_Enemy_Behind")) : g_Vars.m_Esp.sk_esp_playerhintwarning->value == 2 ? GetInMenuSubItem(xorstr_("#Hint_Warnig_Around_Behind")) : GetInMenuSubItem(xorstr_("#Hint_Warnig_FOV_Behind")), m_PlayerNumer);

	g_Drawing.Print(Engine::g_Screen.iWidth / 2, Engine::g_Screen.iHeight / 2 - 200, 255, 0, 0, 255, FL_CENTER, Text.data());

	if (m_CurrentBackDistanceUnits <= g_Vars.m_Esp.sk_esp_playerhintwarning_distance->value - 1)
	{
		std::string Info = fmt::format(GetInMenuSubItem(xorstr_("#Hint_Warnig_Enemy_Src")), m_CurrentBackDistanceUnits, m_GamePlayer->GetPlayer(m_CurrentPlayerIndex)->Info.name != NULL ? m_GamePlayer->GetPlayer(m_CurrentPlayerIndex)->Info.name : xorstr_("unknown"));

		g_Drawing.Print(Engine::g_Screen.iWidth / 2, Engine::g_Screen.iHeight / 2 - 170, 255, 0, 0, 255, FL_CENTER, Info.data());
	}
}

std::string GameEsp::GetInMenuSubItem(char* Item)
{
	return g_GameCFG.GetInMenuSubItem(Item);
}

void GameEsp::DrawC4Info()
{
	int value = GetTickCount() - g_Vars.m_Esp.m_C4BombDropTick;

	std::string info = fmt::format("{}", 45 - (value / 1000));

	//显示C4安装位置
	float out[3] = { 0 };

	if (WorldToScreen(g_Vars.m_Esp.m_C4Coord, out))
	{

		g_Sprite.DrawNumber(info.data(), out[0], out[1], m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], 1);

		int index = g_Sprite.GetSpriteIndex(xorstr_("c4_on_new"));

		if (index != -1)
		{
			Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, 0, 255, 0);
			Engine::g_Engine.pfnSPR_DrawAdditive(0, out[0] - 50, out[1], &(spritedata.spriteinfo[index].rc)); 
		}
	}
}

void GameEsp::GranadeTrailFunction(usercmd_s* cmd)
{
	if (cmd->buttons & IN_ATTACK2 && g_pUtil->IsGranade(m_Local->iWeaponID))
	{
		float grenorigin[3];
		TraceGrenade(grenorigin);
	}
}
void GameEsp::TraceGrenade(float* dest)
{
#define NUMBLOOPS 50.0f //cvar.test7
#define TIMEALIVE 3.00f //cvar.test8
#define OLDDAMPER 1.75f //cvar.test6
#define NEWDAMPER 0.75f //cvar.test5
#define SVGRAVITY 3.75f //cvar.test3
#define FLOORSTOP 0.20f //cvar.test4

	float throwangles[3], throwvector[3], startpos[3], endpos[3];
	float viewAngles[3];
	pmtrace_t pmtrace;

	float gravity = 800 / SVGRAVITY;

	Engine:: g_Engine.GetViewAngles(viewAngles);
	throwangles[0] = viewAngles[0];
	throwangles[1] = viewAngles[1];
	throwangles[2] = viewAngles[2];

	if (throwangles[0] < 0)
		throwangles[0] = -10 + throwangles[0] * ((90 - 10) / 90.0);
	else
		throwangles[0] = -10 + throwangles[0] * ((90 + 10) / 90.0);

	float flVel = (90 - throwangles[0]) * 4;
	if (flVel > 500)
		flVel = 500;

	Engine::g_Engine.pfnAngleVectors(throwangles, throwvector, NULL, NULL);

	startpos[0] = m_Local->Player.ViewOrg[0] + throwvector[0] * 16;
	startpos[1] = m_Local->Player.ViewOrg[1] + throwvector[1] * 16;
	startpos[2] = m_Local->Player.ViewOrg[2] + throwvector[2] * 16;

	throwvector[0] = (throwvector[0] * flVel) + m_Local->Kz.vVelocity[0];
	throwvector[1] = (throwvector[1] * flVel) + m_Local->Kz.vVelocity[1];
	throwvector[2] = (throwvector[2] * flVel) + m_Local->Kz.vVelocity[2];

	int collisions = 0;
	float timelive;
	float step = (TIMEALIVE / NUMBLOOPS);

	for (timelive = 0; timelive < TIMEALIVE; timelive += step)
	{
		endpos[0] = startpos[0] + throwvector[0] * step;
		endpos[1] = startpos[1] + throwvector[1] * step;
		endpos[2] = startpos[2] + throwvector[2] * step; //move

		Engine::g_Engine.pEventAPI->EV_SetTraceHull(2);
		Engine::g_Engine.pEventAPI->EV_PlayerTrace(startpos, endpos, PM_STUDIO_BOX, -1, &pmtrace);

		if (pmtrace.ent != m_Local->Player.Index && pmtrace.fraction < 1.0) //hits a wall
		{
			//hitpoint
			endpos[0] = startpos[0] + throwvector[0] * pmtrace.fraction * step;
			endpos[1] = startpos[1] + throwvector[1] * pmtrace.fraction * step;
			endpos[2] = startpos[2] + throwvector[2] * pmtrace.fraction * step;

			if (pmtrace.plane.normal[2] > 0.9 && throwvector[2] <= 0 && throwvector[2] >= -gravity * FLOORSTOP)
			{
				dest[0] = endpos[0];
				dest[1] = endpos[1];
				dest[2] = endpos[2];
				return;
			}

			float proj = DotProduct(throwvector, pmtrace.plane.normal);

			throwvector[0] = (throwvector[0] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[0]) * NEWDAMPER; //reflection off the wall
			throwvector[1] = (throwvector[1] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[1]) * NEWDAMPER;
			throwvector[2] = (throwvector[2] * OLDDAMPER - proj * 2 * pmtrace.plane.normal[2]) * NEWDAMPER;

			collisions++;
			if (collisions > 30) break;

			timelive -= (step * (1 - pmtrace.fraction));
		}

		int beamindex = Engine::g_Engine.pEventAPI->EV_FindModelIndex(xorstr_("sprites/laserbeam.spr"));

		DrawLinePoints(startpos, endpos, beamindex, 0.05f, 0.6f, 0, 127, 0, 0, 0, 0, 255, 0);

		startpos[0] = endpos[0];
		startpos[1] = endpos[1];
		startpos[2] = endpos[2];

		throwvector[2] -= gravity * pmtrace.fraction * step; //gravity
	}

	dest[0] = startpos[0];
	dest[1] = startpos[1];
	dest[2] = startpos[2];
}

void GameEsp::DrawLinePoints(float* start, float* end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b)
{
	typedef BEAM* (*R_BeamPoints)				(float* start, float* end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b);
	static R_BeamPoints BeamPoints = nullptr;

	ULONG ulBeamPointsEntry = g_Offset.GetGameAddresInfo()->dwBeamPoints;

	if (!BeamPoints)
	{
		__asm
		{
			pushad
			mov eax, ulBeamPointsEntry
			mov ecx, dword ptr ds : [eax]
			mov edx, dword ptr ds : [ecx + 0000010Ch]
			mov BeamPoints, edx
			popad
		}
	}
	BeamPoints(start, end, modelIndex, life, width, amplitude, brightness, speed, startFrame, framerate, r, g, b);
}

void GameEsp::PreS_DynamicSound(int entid, DWORD entchannel, char* szSoundFile, float* fOrigin, float fVolume, float fAttenuation, int iTimeOff, int iPitch)
{
	if (g_Vars.m_Esp.sk_esp_enable->value && g_Vars.m_Esp.sk_esp_souds->value)
	{
		if (entid > 0 && entid <= Engine::g_Engine.GetMaxClients() && entid != GameEsp::GetInstance()->m_Local->Player.Index)
		{

			sound_t sound;
			sound.origin    = fOrigin;
			sound.name      = szSoundFile;
			sound.timestamp = GetTickCount();
			sound.id        = entid;
			GameEsp::GetInstance()->m_Sounds.push_back(sound);
		}

	}

	PfnPreS_DynamicSoud(entid, entchannel, szSoundFile, fOrigin, fVolume, fAttenuation, iTimeOff, iPitch);
}

void GameEsp::SoundESP()
{

	for (sound_t sound : m_Sounds)
	{
		float screen[2] = {};
		float size = 10.0f * (1000 - (GetTickCount() - sound.timestamp)) / 1000.0f;

		if (WorldToScreen(sound.origin, screen) && m_GamePlayer->IsSetTeam(sound.id,g_Vars.m_Esp.sk_esp_souds_team->value))
		{

			GetColor(sound.id);

			if (g_Vars.m_Esp.sk_esp_souds->flags & (1 << 0))
			{
				//hostage4a	 劫持人质
				std::string Text = xorstr_("hostage1a");
				int index = g_Sprite.GetSpriteIndex(Text.data());
				if (index != -1)
				{
					Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2]); // Get sprite ready
					Engine::g_Engine.pfnSPR_DrawAdditive(0, screen[0] - size, screen[1] - size, &(spritedata.spriteinfo[index].rc)); // Draw sprite
				}
			}
			if (g_Vars.m_Esp.sk_esp_souds->flags & (1 << 1))
			{
				//显示名字
				if (m_GamePlayer->GetPlayer(sound.id)->Info.name)
				{
					g_Font.FmtDrawString(screen[0] - size, screen[1] - size - 15,m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], "{}", m_GamePlayer->GetPlayer(sound.id)->Info.name);
				}
			}
			if (g_Vars.m_Esp.sk_esp_souds->flags & (1 << 2))
			{
				//显示距离
				short DistanceUnits = int(m_GamePlayer->GetBoneCompareOriginDistance(sound.origin, m_Local->Entity->origin));
				g_Font.FmtDrawString(screen[0] - size, screen[1] - size + 15, m_BoxColor[0], m_BoxColor[1], m_BoxColor[2], "[{}]", DistanceUnits);
			}

		}
	}

}

void GameEsp::DrawTraceAngleLine(cl_entity_s* ent)
{
	if (ent && ent->index != m_Local->Player.Index && (ent->index <= Engine::g_Engine.GetMaxClients()) && m_GamePlayer->GetPlayer(ent->index)->Valid && m_GamePlayer->IsSetTeam(ent->index, g_Vars.m_Esp.sk_esp_playerTraceAngleLine_team->value))
	{
		Vector End, Up, Right, Forward;
		Vector EntityViewOrg = ent->origin;
		if (ent->curstate.usehull == 0)
			EntityViewOrg[2] += 17;
		else if (ent->curstate.usehull == 1)
			EntityViewOrg[2] += 12;

		Engine::g_Engine.pfnAngleVectors(ent->angles, Forward, Right, Up);

		Forward[2] = -Forward[2];

		Vector Begin = EntityViewOrg;

		BYTE PlayerBarrelColor[3] = { 0,0,0 };

		if (m_GamePlayer->GetPlayer(ent->index)->Team == TERRORIST)
		{
			PlayerBarrelColor[0] = 255;
			PlayerBarrelColor[1] = 0;
			PlayerBarrelColor[2] = 0;
		}
		else if (m_GamePlayer->GetPlayer(ent->index)->Team == CT)
		{
			PlayerBarrelColor[0] = 0;
			PlayerBarrelColor[1] = 0;
			PlayerBarrelColor[2] = 255;
		}

		End = Begin + Forward * 8000;
		int beamindex = Engine::g_Engine.pEventAPI->EV_FindModelIndex(xorstr_("sprites/laserbeam.spr"));
		this->DrawLinePoints(Begin, End, beamindex, 0.001f, 0.9f, 0, 32, 2, 0, 0, PlayerBarrelColor[0], PlayerBarrelColor[1], PlayerBarrelColor[2]);

	}
}

void GameEsp::DrawTraceMove(cl_entity_s* ent)
{
	if (ent && ent->index != m_Local->Player.Index && (ent->index <= Engine::g_Engine.GetMaxClients()) && m_GamePlayer->GetPlayer(ent->index)->Valid && m_GamePlayer->IsSetTeam(ent->index, g_Vars.m_Esp.sk_esp_playerTrackMove_team->value))
	{
		BYTE PlayerBarrelColor[3] ={0,0,0};

		if (m_GamePlayer->GetPlayer(ent->index)->Team == TERRORIST)
		{
			PlayerBarrelColor[0] = 255;
			PlayerBarrelColor[1] = 0;
			PlayerBarrelColor[2] = 0;
		}
		else if (m_GamePlayer->GetPlayer(ent->index)->Team == CT)
		{
			PlayerBarrelColor[0] = 0;
			PlayerBarrelColor[1] = 0;
			PlayerBarrelColor[2] = 255;
		}


		//显示玩家路径
		ColorEntry color1;
		color1.r = PlayerBarrelColor[0];
		color1.g = PlayerBarrelColor[1];
		color1.b = PlayerBarrelColor[2];
		color1.a = 255;

		if (!g_Vars.m_Esp.sk_esp_playerTraceAngleLine->value)
		{
			//如果开了视角线.就不开启追踪线了
			R_BeamFollow(ent, &color1, 5, 1);
		}

	

	}
}

void GameEsp::R_BeamFollow(cl_entity_s* pEnt, ColorEntry* pClr, float life, float width)
{
	typedef BEAM* (*R_BeamFollow)				(int startEnt, int modelIndex, float life, float width, float r, float g, float b, float brightness);
	static R_BeamFollow BeamFollow = nullptr;
	ULONG ulBeamPointsEntry = g_Offset.GetGameAddresInfo()->dwBeamPoints;
	if (!BeamFollow)
	{
		__asm
		{
			pushad
			mov eax, ulBeamPointsEntry
			mov ecx, dword ptr ds : [eax]
			mov eax, dword ptr ds : [ecx + 00000100h]
			mov BeamFollow, eax
			popad
		}
	}
	int beamindex = Engine::g_Engine.pEventAPI->EV_FindModelIndex(xorstr_("sprites/laserbeam.spr"));
	BeamFollow(pEnt->index, beamindex, life, width, pClr->r, pClr->g, pClr->b, 1);
}

void GameEsp::AddWorld(cl_entity_s* Entity, const char* modelname)
{

	//ACEDebugLog("[{}]  {}",__FUNCTION__, modelname);


	if (g_Vars.m_Esp.sk_esp_drawitems->value && modelname != NULL && Entity->index > Engine::g_Engine.GetMaxClients())
	{
		std::string StrModel = modelname;

		if (StrModel.find(xorstr_("models/w_"))!=StrModel.npos)
		{

			//取中间匹配的字符串.(任意字符必须出现1到多次)
			std::regex r(xorstr_("models/w_(.+)\.mdl"));

			std::sregex_iterator pos(StrModel.cbegin(), StrModel.cend(), r);
			std::sregex_iterator end;

			for (; pos != end; ++pos)
			{
				if (pos->size() < 2)
				{
					continue;;
				}
				std::string WeaponCode = pos->str(1);

				PushEntity(Entity->index, WeaponCode, Entity->origin, ADD_ENTITY_TYPE::WEAPON);

				Entity->curstate.renderamt     = 100;
				Entity->curstate.rendercolor.r = 255;
				Entity->curstate.rendercolor.b = 0;
				Entity->curstate.rendercolor.g = 0;
				ColorEntry color1;
				color1.r                       = 75;
				color1.g                       = 0;
				color1.b                       = 130;
				DoExtraRendering(Entity, color1);
				break;
			}

		}
	}
	if (g_Vars.m_Esp.sk_esp_zombie_trap->value && modelname != NULL && Entity->index > Engine::g_Engine.GetMaxClients())
	{
		std::string StrModel = modelname;

		if (StrModel.find(xorstr_("models/zombitrap.mdl")) != StrModel.npos)
		{
			PushEntity(Entity->index, StrModel, Entity->origin, ADD_ENTITY_TYPE::ZOMBITRAP);
		}
	}
	if (g_Vars.m_Esp.sk_esp_supply->value && modelname != NULL && Entity->index > Engine::g_Engine.GetMaxClients())
	{
		std::string StrModel = modelname;

		if (StrModel.find(xorstr_("models/supply")) != StrModel.npos)
		{
			PushEntity(Entity->index, StrModel, Entity->origin, ADD_ENTITY_TYPE::SUPPLY);
		}
	}
	if (g_Vars.m_Esp.sk_esp_prophunt->value && modelname != NULL)
	{
		std::string StrModel = modelname;

		if (StrModel.find(xorstr_("models/voxel")) != StrModel.npos)
		{

			std::regex r(xorstr_("models/voxel(.+)/(.+)\.mdl"));

			std::sregex_iterator pos(StrModel.cbegin(), StrModel.cend(), r);
			std::sregex_iterator end;

			for (; pos != end; ++pos)
			{
				if (pos->size() < 3)
				{
					continue;;
				}

				//PushEntity(Entity->index, pos->str(2), Entity->origin, ADD_ENTITY_TYPE::VXL);

				//Entity->curstate.renderamt = 100;
				//Entity->curstate.rendercolor.r = 255;
				//Entity->curstate.rendercolor.b = 0;
				//Entity->curstate.rendercolor.g = 0;

				//ColorEntry color1;
				//color1.r = 255;
				//color1.g = 255;
				//color1.b = 0;
				//DoExtraRendering(Entity, color1);

				Entity->outlineType = 7;
				Entity->outline_R = 255;
				Entity->outline_G = 0;
				Entity->outline_B = 0;

				break;
			}
	
		}
	}
	if (g_Vars.m_Esp.sk_esp_allitem->value &&  modelname != NULL && Entity->index > Engine::g_Engine.GetMaxClients())
	{
		Entity->outlineType = 7;
		Entity->outline_R   = 255;
		Entity->outline_G   = 165;
		Entity->outline_B   = 0;
	}
}

void GameEsp::DrawWorld()
{
	//ACEDebugLog("size:{}", m_Entity.size());
	for (size_t i = 0; i < m_Entity.size(); i++)
	{
		float EntityScreen[3];

		EntityStruct & Entity =  m_Entity.back();

		if (WorldToScreen(Entity.Origin, EntityScreen))
		{
			float Distance = floor(sqrt(POW(fabs(Entity.Origin.x - m_Local->Entity->origin.x)) +
				POW(fabs(Entity.Origin.y - m_Local->Entity->origin.y)) +
				POW(fabs(Entity.Origin.z - m_Local->Entity->origin.z))) - 32);

			BYTE Color[3] = { 0,0,0 };


			if (Distance >= 0 && Distance <= 1000)
			{
				Color[0] = 255;
				Color[1] = Distance >= 500 ? Interp(500, Distance, 1000, 255, 80) : 255;
				Color[2] = Distance <= 500 ? Interp(0, Distance, 500, 255, 80) : 80;
			}
			else if (Distance > 1000)
			{
				Color[0] = 255;
				Color[1] = 80;
				Color[2] = 80;
			}

			if (Entity.Type == ADD_ENTITY_TYPE::WEAPON)
			{

				//显示武器翻译的名称
				if (g_Vars.m_Esp.sk_esp_drawitems->flags & (1 << 0))
				{
					auto Utf8_WeaponName = g_pUtil->GetCSOItemName(Entity.Name.data());
					//找到了.证明失败
					bool  bFail = Utf8_WeaponName.find(xorstr_("CSO_Item_Name_")) != Utf8_WeaponName.npos;

					g_Font.FmtDrawString(int(EntityScreen[0]), int(EntityScreen[1] + 15), Color[0], Color[1], Color[2], bFail ? Entity.Name : Utf8_WeaponName);

				}
				//显示武器模型
				if (g_Vars.m_Esp.sk_esp_drawitems->flags & (1 << 1))
				{
					// * 画出武器模型
					std::string Text = fmt::format(xorstr_("d_{}"), Entity.Name);
					int index = g_Sprite.GetSpriteIndex(Text.data());
					if (index != -1)
					{

						Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, 255, 120,50);
						Engine::g_Engine.pfnSPR_DrawAdditive(0, int(EntityScreen[0]), int(EntityScreen[1]), &(spritedata.spriteinfo[index].rc));
					}

				}

				std::string Text = fmt::format("{}", (int)Distance);
				g_Sprite.DrawNumber(Text.data(), int(EntityScreen[0]), int(EntityScreen[1]) + 30, Color[0], Color[1], Color[2], 1);
			}
			else if (Entity.Type == ADD_ENTITY_TYPE::ZOMBITRAP)
			{
				std::string Text = xorstr_("zombitrap");
				int index = g_Sprite.GetSpriteIndex(Text.data());
				if (index != -1)
				{
					Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, 255, 0, 0);
					Engine::g_Engine.pfnSPR_DrawAdditive(0, int(EntityScreen[0]), int(EntityScreen[1]), &(spritedata.spriteinfo[index].rc));
				}

			}
			else if (Entity.Type == ADD_ENTITY_TYPE::SUPPLY)
			{
				//南瓜
				std::string Text = xorstr_("d_pumpkin");
				int index = g_Sprite.GetSpriteIndex(Text.data());
				if (index != -1)
				{
					Engine::g_Engine.pfnSPR_Set((HSPRITE)spritedata.spriteinfo[index].hspr, 255, 0, 0);
					Engine::g_Engine.pfnSPR_DrawAdditive(0, int(EntityScreen[0]), int(EntityScreen[1]), &(spritedata.spriteinfo[index].rc));
				}

				//显示距离
				Text = fmt::format("{}", (int)Distance);
				g_Sprite.DrawNumber(Text.data(), int(EntityScreen[0]), int(EntityScreen[1]) + 10, Color[0], Color[1], Color[2], 1);
			}
			else if (Entity.Type == ADD_ENTITY_TYPE::VXL)
			{
				//不显示.直接用描边透视算了
			}

		}

		m_Entity.pop_back();
	}

}

void GameEsp::PushEntity(ULONG Index, std::string Name, vec3_t origin, ADD_ENTITY_TYPE Type)
{
	EntityStruct Info;
	Info.Index  = Index;
	Info.Name   = Name;
	Info.Origin = origin;
	Info.Type   = Type;
	m_Entity.push_back(Info);
}

void GameEsp::DrawPatrolDrone()
{
	auto  pWeaponInfo = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;

	if (pWeaponInfo && pWeaponInfo->GRENADES && *(PDWORD)(pWeaponInfo->GRENADES + 0x9c) == 458)
	{
		for (int i = 0; i < 4; i++)
		{
			auto State = GetPatrolState(m_PatrolDrone.Core[i].State);

			auto Text = fmt::format(GetInMenuSubItem("#Esp_Patrol_Status_Core"), m_PatrolDrone.Core[i].CurrentCore, State, m_PatrolDrone.Core[i].Duration);

			TRIVERTEX* pColor = GetPatrolColor(&m_PatrolDrone, i);

			g_Drawing.Print(Engine::g_Screen.iWidth / 2, Engine::g_Screen.iHeight / 2 - 150 + (i * 18), pColor->Red, pColor->Green, pColor->Blue, 255, FL_CENTER, Text.data());

		}
	}
}

std::string GameEsp::GetPatrolState(Patrol_State Type)
{
	std::string PatrolStrStatus;

	switch (Type)
	{
	case State_Destroy:
		PatrolStrStatus = GetInMenuSubItem("#Esp_Patrol_Status_0");
		break;
	case State_Standby:
		PatrolStrStatus = GetInMenuSubItem("#Esp_Patrol_Status_1");
		break;
	case State_Patrol:
		PatrolStrStatus = GetInMenuSubItem("#Esp_Patrol_Status_2");
		break;
	case State_Bomb:
		PatrolStrStatus = GetInMenuSubItem("#Esp_Patrol_Status_3");
		break;
	default:
		PatrolStrStatus = GetInMenuSubItem("#Esp_Patrol_Status_0");
		break;
	}

	return PatrolStrStatus;
}

TRIVERTEX* GameEsp::GetPatrolColor(GAME_PATROL_MSG* pInfo, int Index)
{
	TRIVERTEX* pColor = &m_PatrolColor;

	if (pInfo->Core[Index].State == State_Destroy)
	{
		// 如果核心已损坏,显示为   白色
		pColor->Red = 255, pColor->Green = 255, pColor->Blue = 255;
		return pColor;
	}
	if (pInfo->Core[Index].Duration <= 200)
		pColor->Red = 255, pColor->Green = 0, pColor->Blue = 0;

	else if (pInfo->Core[Index].Duration > 200 && pInfo->Core[Index].Duration <= 400)
		pColor->Red = 255, pColor->Green = 255, pColor->Blue = 0;

	else if (pInfo->Core[Index].Duration > 400 && pInfo->Core[Index].Duration <= 600)
		pColor->Red = 0, pColor->Green = 0, pColor->Blue = 255;

	else if (pInfo->Core[Index].Duration > 600 && pInfo->Core[Index].Duration <= 900)
		pColor->Red = 0, pColor->Green = 255, pColor->Blue = 255;
	else
		pColor->Red = 0, pColor->Green = 0, pColor->Blue = 0;

	return pColor;
}
