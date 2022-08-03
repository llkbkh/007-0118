#include "GameScript.h"
#include "GameOffset.h"
#include "Util.h"

GameScript::GameScript()
{

}

GameScript* GameScript::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameScript();
	}
	return m_pInstance;
}

void GameScript::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	if (!g_Vars.m_Script.sk_script_enable->value)
	{
		return;
	}

	if (g_Vars.m_Script.sk_script_zbs_autogank_ct->value)
	{
		ZombiAutoGankCT(cmd);
	}

	if (g_Vars.m_Script.sk_script_zbs_counter->value)
	{
		ZbsCounter(cmd);
	}

}

bool GameScript::vInit()
{
	m_pGamePlayer = GamePlayer::GetInstance();
	m_Local       = m_pGamePlayer->GetLocal();
	m_Memory      = g_pUtil->GetAceMemory();


	return true;
}

void GameScript::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_Script.sk_script_enable->value)
		return;
	//玩家坐标点可见
	PlayerOriginVisible();

	if (g_Vars.m_Script.sk_script_zb5_auto_skill->value)
	{

		if (m_Memory->Read<short>(g_Offset.GetGameAddresInfo()->dwRoomMode) == CSO_GameMode_Zombi_5)
		{
			if (m_Local->skill3)
			{
				Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill2\""));
			}
			if (m_Local->skill4)
			{
				Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill3\""));
			}
		}
	}

	
	//if (m_Local->Entity)
	//{
	//	Engine::g_pEngine->Con_NPrintf(0, "sequence:%d", m_Local->Entity->curstate.sequence);

	//}
#if 0
	int Index = 0;
	GAME_PLAYER_WEAPON_INFO* Info = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;
	if (Info && Info->KnifeData != 0)
	{
		if (m_Local && m_Local->iWeaponID == 0x1D)
		{
			Index = m_Memory->Read<DWORD>(((ULONG)Info->KnifeData) + 0xB8);
		}
		ULONG Sequence = m_Memory->Read<ULONG>(g_Offset.GetGameAddresInfo()->dwPlayerSequence + 0x1D8);
		Engine::g_pEngine->Con_NPrintf(0, "Index:%d sequence:%d seq:%d", Index, m_Local->Entity->curstate.sequence, Sequence);
	}
#endif
	
	
	



}

int GameScript::vMPToCL(const char* pszName, int iSize, void* pbuf)
{
	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);

	int Type = lReader->ReadChar();

	if (Type == 43)
	{
		int Flag = lReader->ReadChar();

		//获取当前技能状态
		m_Local->skill1 = Flag & 1;
		m_Local->skill2 = Flag & 2;
		m_Local->skill3 = Flag & 4;
		m_Local->skill4 = Flag & 8;

	}



	

	return 1;
}

int GameScript::vInitHUD(const char* pszName, int iSize, void* pbuf)
{
	m_Local->skill1 = 0;
	m_Local->skill2 = 0;
	m_Local->skill3 = 0;
	m_Local->skill4 = 0;




	return 1;
}

int GameScript::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (!g_Vars.m_Script.sk_script_enable->value)
		return 1;


	if (g_Vars.m_Script.sk_script_god->value && g_Vars.m_Script.sk_script_god->hotkey == wParam)
	{
		auto Script_Start = [&]()
		{
			while (g_Vars.m_Script.sk_script_god->flags)
			{
				int Index = 0;
				GAME_PLAYER_WEAPON_INFO* Info = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;

				if (Info && Info->KnifeData != 0)
				{
					if (m_Local && m_Local->iWeaponID == 0x1D)
					{
						Index = m_Memory->Read<DWORD>(((ULONG)Info->KnifeData) + 0xB8);
					}
				}
				if (Index == 181 || Index == 137)
				{
					Engine::g_pEngine->pfnClientCmd(xorstr_("+attack2"));
					Sleep(200);
					Engine::g_pEngine->pfnClientCmd(xorstr_("-attack2"));
					Sleep(100);
					Engine::g_pEngine->pfnClientCmd(xorstr_("+attack"));

					Sleep(1100);
					g_Vars.m_Basi.sk_basics_item8530->flags = 1;
					g_Vars.m_Basi.sk_basics_item8530->value = 1;
					Engine::g_pEngine->pfnClientCmd(xorstr_("-attack"));
					Engine::g_pEngine->pfnClientCmd(xorstr_("vxl_BuyZone"));
					Sleep(300);
				}




			}

		};

		g_Vars.m_Script.sk_script_god->flags = !g_Vars.m_Script.sk_script_god->flags;

		if (g_Vars.m_Script.sk_script_god->flags)
		{
			std::thread t(Script_Start);
			t.detach();
		}
	}
	if (g_Vars.m_Script.sk_script_excalibur->value && g_Vars.m_Script.sk_script_excalibur->hotkey == wParam)
	{
		auto Script_Start = [&]()
		{
			while (g_Vars.m_Script.sk_script_excalibur->flags)
			{
				int Index = 0;

				GAME_PLAYER_WEAPON_INFO* Info = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;

				if (Info && Info->KnifeData != 0)
				{
					if (m_Local && m_Local->iWeaponID == 0x1D)
					{
						Index = m_Memory->Read<DWORD>(((ULONG)Info->KnifeData) + 0xB8);
					}
				}
				static BOOL v1 = TRUE;
				if ((Index == 0x81 || Index == 0xF8) && m_pGamePlayer->GetWeaponInfo()->m_flTimeWeaponIdle <= 1.3f && m_pGamePlayer->GetWeaponInfo()->m_flTimeWeaponIdle >= 1.2f)
				{
					v1 = FALSE;
					g_Vars.m_Basi.sk_basics_item8133->flags = 1;
					g_Vars.m_Basi.sk_basics_item8133->value = 1;
					Engine::g_pEngine->pfnClientCmd(xorstr_("vxl_BuyZone"));
					Engine::g_pEngine->pfnClientCmd(xorstr_("+attack2"));
					Sleep(300);	//400
				}
				else
				{
					v1 = TRUE;
				}

			}
			Engine::g_pEngine->pfnClientCmd(xorstr_("-attack2"));
		};

		g_Vars.m_Script.sk_script_excalibur->flags = !g_Vars.m_Script.sk_script_excalibur->flags;

		if (g_Vars.m_Script.sk_script_excalibur->flags)
		{
			std::thread t(Script_Start);
			t.detach();
		}
	}



	return 1;
}

float GameScript::GetAngledeviation(int nIndex)
{

	QVector Forward = m_Local->Forward;

	return  Forward.AngleBetween((QVector)m_pGamePlayer->GetPlayer(nIndex)->Entity->origin - m_Local->Player.vEye);
}
void GameScript::ZombiAutoGankCT(struct usercmd_s* cmd)
{
	//可以加个判断模式
	if (m_Local->Player.Team == 1)
	{
		int id       = 0;
		float flDist = 8192.f;

		for (int i = 1; i <= Engine::g_Engine.GetMaxClients(); i++)
		{
			if (i == m_Local->Player.Index || !m_Local->Player.isAlive)
				continue;

			auto pEnt =  m_pGamePlayer->GetPlayer(i);

			if (pEnt->Entity == NULL || pEnt->Dormant || !pEnt->Valid)
				continue;
			if (pEnt->Info.name == NULL)
				continue;
			if (pEnt->Team == m_Local->Player.Team)
				continue;

			//是否在视角内
			if (g_Vars.m_Script.sk_script_zbs_autogank_ct->flags & (1 << 3))
			{
				if (GetAngledeviation(i) > 45)
					continue;
			}
			//圣剑盾反
			if (g_Vars.m_Script.sk_script_zbs_autogank_ct->flags & (1 << 1))
			{
				if (pEnt->Entity->curstate.sequence == 0x141)
					continue;
			}
			//隐身检查
			if (g_Vars.m_Script.sk_script_zbs_autogank_ct->flags & (1 << 0))
			{
				if (pEnt->Entity->curstate.rendermode == 4)
					continue;
			}
			//是否可见状态
			if (g_Vars.m_Script.sk_script_zbs_autogank_ct->flags & (1 << 2))
			{
				if (pEnt->vOriginVisible == false)
				{
					continue;
				}
			}

			float fCurDist = m_pGamePlayer->GetPlayerDistance(i, false);

			if (fCurDist < flDist || id == 0)
			{
				flDist = fCurDist;
				id = i;
			}

		}

		if (id != 0)
		{
			auto pEnt = m_pGamePlayer->GetPlayer(id);


			int fDidHit = FALSE;

			Vector vecPlayerOrigin = pEnt->Entity->origin + (Vector)pEnt->vVelocity * pEnt->fFrametime;

			float fOrigin = (Engine::g_pPlayerMove->origin - vecPlayerOrigin).Length();


			//78  重击  66 左键
			if (fOrigin <= 78)
			{
				// hit
				fDidHit = TRUE;

				Vector vecForwardPlayer, vecForward;

				Vector vecSrc = m_Local->Player.vEye;

				QAngle QAimAngle;

				VectorAngles(vecPlayerOrigin - vecSrc, QAimAngle);

				QAimAngle.Normalize();

				Engine::g_Engine.pfnAngleVectors(QAimAngle, vecForward, 0, 0);
				Engine::g_Engine.pfnAngleVectors(pEnt->Entity->angles, vecForwardPlayer, 0, 0);

				vecForwardPlayer.Normalize();
				vecForward.Normalize();


				float flDot = vecForward.x * vecForwardPlayer.x + vecForward.y * vecForwardPlayer.y;

				if (fDidHit)
				{
					MakeAngle(false, QAimAngle, cmd);

					if (fOrigin <= 66)
						cmd->buttons |= IN_ATTACK;
					else
						cmd->buttons |= IN_ATTACK2;

				}
			}
		}
	}
}
void GameScript::VectorAngles(const float* forward, float* angles)
{
	float tmp, yaw, pitch;

	if (forward[1] == 0 && forward[0] == 0)
	{
		yaw = 0;
		if (forward[2] > 0)
			pitch = 270;
		else
			pitch = 90;
	}
	else
	{
		yaw = (atan2(forward[1], forward[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;

		tmp = sqrt(forward[0] * forward[0] + forward[1] * forward[1]);
		pitch = (atan2(-forward[2], tmp) * 180 / M_PI);
		if (pitch < 0)
			pitch += 360;
	}

	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;

	while (angles[0] < -89) { angles[0] += 180; angles[1] += 180; }
	while (angles[0] > 89) { angles[0] -= 180; angles[1] += 180; }
	while (angles[1] < -180) { angles[1] += 360; }
	while (angles[1] > 180) { angles[1] -= 360; }
}
void GameScript::MakeAngle(bool addangle, float* angles, struct usercmd_s* cmd)
{
	cl_entity_s* pLocal = Engine::g_Engine.GetLocalPlayer();

	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;

	float newforward, newright, newup;
	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;


	if (!pLocal)
		return;

	if (pLocal->curstate.movetype == MOVETYPE_WALK) { Engine::g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup); }
	else { Engine::g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup); }

	if (pLocal->curstate.movetype == MOVETYPE_WALK && !(Engine::g_pPlayerMove->movetype == 5))
	{

		if (addangle == 0)
		{
			cmd->viewangles.x = angles[0];
			cmd->viewangles.y = angles[1];
		}
		else if (addangle == 1)
		{
			cmd->viewangles.x += angles[0];
			cmd->viewangles.y += angles[1];
		}
	}


	if (pLocal->curstate.movetype == MOVETYPE_WALK) { Engine::g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup); }
	else { Engine::g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup); }

	newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
	newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
	newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

	if (angles[0] > 81) { cmd->forwardmove = -newforward; }
	else { cmd->forwardmove = newforward; }

	cmd->sidemove = newright;
	cmd->upmove = newup;
}

void GameScript::PlayerOriginVisible()
{
	for (int i = 1; i <= Engine::g_Engine.GetMaxClients(); i++)
	{
		auto pEnt              = m_pGamePlayer->GetPlayer(i);
		pEnt->vOriginVisible   = false;
		if (i                  == m_Local->Player.Index || !m_Local->Player.isAlive)
			continue;
		if (pEnt->Entity       == NULL || pEnt->Dormant || !pEnt->Valid)
			continue;
		if (pEnt->Info.name    == NULL)
			continue;
		Vector vecPlayerOrigin = pEnt->Entity->origin + (Vector)pEnt->vVelocity * pEnt->fFrametime;
		pEnt->vOriginVisible   =  m_pGamePlayer->PathFree(m_Local->Player.vEye, vecPlayerOrigin);

	}

	//auto me =  m_pGamePlayer->GetSequence(Engine::g_Engine.GetLocalPlayer()->index, Engine::g_Engine.GetLocalPlayer()->curstate.sequence);

	//auto attack =  m_pGamePlayer->ZombiIsAttack(Engine::g_Engine.GetLocalPlayer()->index, Engine::g_Engine.GetLocalPlayer()->curstate.sequence);

	//if (me)
	//{
	//	Engine::g_Engine.Con_NPrintf(2, "%s - %d", me->label, attack);
	//}

}

void GameScript::ZbsCounter(usercmd_s* cmd)
{
#if 1
	if (m_Local->Player.Team == CT)
	{
		int id       = 0;
		float flDist = 100.f;

		for (int i = 1; i <= Engine::g_Engine.GetMaxClients(); i++)
		{
			auto pEnt            = m_pGamePlayer->GetPlayer(i);
			pEnt->vOriginVisible = false;
			if (i                == m_Local->Player.Index || !m_Local->Player.isAlive)
				continue;
			if (pEnt->Entity     == nullptr || pEnt->Dormant || !pEnt->Valid)
				continue;
			if (pEnt->Info.name  == nullptr)
				continue;

			if (pEnt->Team == m_Local->Player.Team)
				continue;

			float fCurDist = m_pGamePlayer->GetPlayerDistance(i, false);

			if (fCurDist < flDist)
			{
			
				if (g_Vars.m_Script.sk_script_zbs_counter->value == 1 || (g_Vars.m_Script.sk_script_zbs_counter->value == 2 && m_pGamePlayer->ZombiIsAttack(i, pEnt->Entity->curstate.sequence)))
				{
					id = i;
					//切换圣剑
					GAME_PLAYER_WEAPON_INFO* Infp = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;
					if (Infp && Infp->KnifeData != 0)
					{
						// 近身武器 = F8 切 Id = 0x1D 就能判断出 当前武器 不是圣剑 且装备栏有圣剑 切换到圣剑
						if ((g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_RAIER) && m_Memory->Read<ULONG>(Infp->KnifeData + 0xB8) == 0xF8 && m_Local->iWeaponID != 0x1D)
						{
							Engine::g_pEngine->pfnClientCmd(xorstr_("slot3"));
						}
						
						if ((g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_SMAILLRAIER) /*&& m_Memory->Read<ULONG>(Infp->KnifeData + 0xB8) == 284 && m_Local->iWeaponID != 0x1D*/)
						{
							if (m_pGamePlayer->GetWeaponInfo()->m_iId == 0x1D)
							{
								ULONG Sequence = m_Memory->Read<ULONG>(g_Offset.GetGameAddresInfo()->dwPlayerSequence+0x1D8);
								if (Sequence == 9)
								{
									// 起手状态.不要切换武器
									goto Lab_next;
								}
								else if (Sequence == 11 /*&& !(m_pGamePlayer->GetWeaponInfo()->m_flTimeWeaponIdle <= 1.3f && m_pGamePlayer->GetWeaponInfo()->m_flTimeWeaponIdle >= 1.2f)*/)
								{
									goto Lab_next;
								}
								if (m_pGamePlayer->GetWeaponInfo()->m_iClip >= 50)
								{
									goto Lab_next;
								}
								g_Vars.m_Script.m_buy_excalibur_ex = 1;
								//先购买弑神.在购买小圣剑
								Engine::g_pEngine->pfnClientCmd(xorstr_("vxl_BuyZone"));
								
								
							}
							else
							{
								g_Vars.m_Script.m_buy_excalibur_ex = 1;
								//先购买弑神.在购买小圣剑
								Engine::g_pEngine->pfnClientCmd(xorstr_("slot3;vxl_BuyZone"));
							}

						}
						
Lab_next:
						//开6
						if ((g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_HM_SKILL02) && m_Local->skill2)
						{
							//如果开了切换圣剑 且开6
							if ((g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_RAIER )|| (g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_SMAILLRAIER))
							{
				

								if ((*(PULONG)(Infp->KnifeData + 0xB8) == 0xF8 || *(PULONG)(Infp->KnifeData + 0xB8) == 284)&& m_Local->iWeaponID == 0x1D)
								{
									//获取攻击状态 如果是在盾反 则开6
									ULONG Sequence = m_Memory->Read<ULONG>(g_Offset.GetGameAddresInfo()->dwPlayerSequence+0x1D8);
									if (Sequence == 11)
									{
										Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill1\""));
									}
								}
	
							}
							else
							{
								Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill1\""));
							}
						}
					}
					break;
				}
			}

		}

		if (id > 0 && id <= Engine::g_Engine.GetMaxClients())
		{
			auto pEnt = m_pGamePlayer->GetPlayer(id);


			int fDidHit = FALSE;

			Vector vecPlayerOrigin = pEnt->Entity->origin + (Vector)pEnt->vVelocity * pEnt->fFrametime;

			float fOrigin = (Engine::g_pPlayerMove->origin - vecPlayerOrigin).Length();

			//78  重击  66 左键
			if (g_Vars.m_Script.sk_script_zbs_counter->value == 1 || (g_Vars.m_Script.sk_script_zbs_counter->value == 2 && fOrigin <= 85))
			{
				//开5
				if (g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_HM_SKILL01 && m_Local->skill1)
				{
					Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill0\""));
				}

				//生化S 自动按V
				if (g_Vars.m_Script.sk_script_zb5_auto_skill->value && m_Memory->Read<short>(g_Offset.GetGameAddresInfo()->dwRoomMode) == CSO_GameMode_Zombi_5)
				{
					Engine::g_pEngine->pfnClientCmd(xorstr_("zb5_ablt"));
				}

				if (g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_HM_SKILL03 && m_Local->skill3)
				{
					Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill2\""));
				}
				if (g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_HM_SKILL04 && m_Local->skill4)
				{
					Engine::g_pEngine->pfnClientCmd(xorstr_("useitem \"hm_skill3\""));
				}


				// hit
				fDidHit = TRUE;

				Vector vecForwardPlayer, vecForward;

				Vector vecSrc = m_Local->Player.vEye;

				QAngle QAimAngle;

				VectorAngles(vecPlayerOrigin - vecSrc, QAimAngle);

				QAimAngle.Normalize();

				Engine::g_Engine.pfnAngleVectors(QAimAngle, vecForward, 0, 0);
				Engine::g_Engine.pfnAngleVectors(pEnt->Entity->angles, vecForwardPlayer, 0, 0);

				vecForwardPlayer.Normalize();
				vecForward.Normalize();


				float flDot = vecForward.x * vecForwardPlayer.x + vecForward.y * vecForwardPlayer.y;

				if (fDidHit)
				{
					MakeAngle(false, QAimAngle, cmd);


					GAME_PLAYER_WEAPON_INFO* Infp = (PGAME_PLAYER_WEAPON_INFO)g_Offset.GetGameAddresInfo()->dwPlayerWeaponInfo;

					if (Infp && Infp->KnifeData != 0)
					{

						if ((*(PULONG)(Infp->KnifeData + 0xB8) == 0xF8 || *(PULONG)(Infp->KnifeData + 0xB8) == 284) && m_Local->iWeaponID == 0x1D)
						{

							static bool bFire = false;

							if (g_Vars.m_Script.sk_script_zbs_counter->flags & SE_SCRIPT::SE_RAIEREX)
							{

								if (bFire)
								{
									cmd->buttons |= IN_ATTACK2;
									bFire = false;
								}
								else if (!bFire)
								{
									cmd->buttons &= ~IN_ATTACK2;
									bFire = true;
								}

							}
						}
					}

				}
			}
		}
	}
#endif
	
}
