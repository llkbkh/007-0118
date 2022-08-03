#include "GameKreedz.h"
#include "Drawing.h"
#include "GameOffset.h"

GameKreedz::GameKreedz()
{

}

GameKreedz* GameKreedz::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new GameKreedz();
	}
	return m_Instance; 
}

bool GameKreedz::vInit()
{

	m_pGamePlayer = GamePlayer::GetInstance();
	m_dwSpeedptr  = 0;
	m_Local       = m_pGamePlayer->GetLocal();
	m_bJumped     = false;
	m_balance_bh  = false;
	m_InterpFps   = 0.0f;
	m_FrameCnt    = 0.0f;
	m_FpsCnt      = 0.0f;
	m_PreStrafe   = 0.0f;
	m_JumpOff     = 0.0f;

	return true;
}



void GameKreedz::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	if (!g_Vars.m_Kz.sk_kreedz_enable->value)
		return;

	m_Local->FrameTime = frametime;
	Forward            = cmd->buttons & IN_FORWARD ? true : false;
	Backward           = cmd->buttons & IN_BACK ? true : false;
	MoveLeft           = cmd->buttons & IN_MOVELEFT ? true : false;
	MoveRight          = cmd->buttons & IN_MOVERIGHT ? true : false;
	Duck               = cmd->buttons & IN_DUCK ? true : false;
	Jump               = cmd->buttons & IN_JUMP ? true : false;


	if (active)
		AdjustSpeed(1);

	cl_entity_s* pEntry = Engine::g_Engine.GetLocalPlayer();
	if (!pEntry || !pEntry->index || !m_Local->Player.isAlive)
		return;

	if ((Engine:: g_Engine.GetClientTime() - m_Local->Kz.flDuckTimeMax) >= 0.8 && m_balance_bh == true)
	{
		Engine::g_Engine.pfnClientCmd("-duck");
		m_balance_bh = false;
	}
	if (g_Vars.m_Kz.sk_kreedz_jumpbug->value)
		JumpBug(frametime, cmd);

	if (g_Vars.m_Kz.sk_kreedz_groundspeed->value)
		FastRun(frametime, cmd);

	if (g_Vars.m_Kz.sk_kreedz_mode->value == 2)
	{

		if (g_Vars.m_Kz.sk_kreedz_bunnyhop->value)
			Bhop(frametime, cmd);
		if (g_Vars.m_Kz.sk_kreedz_groundstrafe->value)
			Gstrafe(cmd);
		if (g_Vars.m_Kz.sk_kreedz_autostrafe->value)
			StrafeHack(frametime, cmd);
		if (g_Vars.m_Kz.sk_kreedz_strafehelper->value)
			StrafeHelper(frametime, cmd);
	}
	else if (g_Vars.m_Kz.sk_kreedz_mode->value == 1)
	{

		KzStrafe(cmd);


	}



	if (g_Vars.m_Kz.sk_kreedz_spinhack->value)
		SpinHack(cmd);


	ShowLongJump(cmd);





}

void GameKreedz::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_Kz.sk_kreedz_enable->value || !g_Vars.m_Kz.sk_kreedz_info->value)
	{
		return;
	}
	if (m_bJumped)
	{
		m_FrameCnt += 1;
		m_FpsCnt += (1 / m_Local->Kz.flFrametime);
	}
	DrawKreedzInfo();
	DrawShowKeys();

	//Engine::g_Engine.Con_NPrintf(0, "flags: %d  \n movetype:%d", Engine::g_pPlayerMove->flags, Engine::g_pPlayerMove->movetype);
}


void GameKreedz::Bhop(float frametime, struct usercmd_s* cmd)
{
	static bool bhop_standup_state = false;
	static bool lastFramePressedJump = false;
	static bool JumpInNextFrame = false;
	int curFramePressedJump = cmd->buttons & IN_JUMP;



	// * 在水里且不在地面 水中加速
	if (!(Engine::g_pPlayerMove->flags & FL_ONGROUND) && (Engine::g_pPlayerMove->flags & FL_INWATER) && Engine::g_pPlayerMove->movetype != 5)
	{
	


		return;
	}


	if (g_Vars.m_Kz.sk_kreedz_bunnyhop->value == 2 && Engine::g_pPlayerMove->flFallVelocity > 0)
	{
		if (m_Local->Kz.flGroundAngle < 5.0f && ((Engine::g_pPlayerMove->flags & FL_ONGROUND) || m_Local->Kz.flHeight < 0.00001f))
			AdjustSpeed(0.0001);

		if (Engine::g_pPlayerMove->flFallVelocity >= 140)
			if (m_Local->Kz.flHeight <= 30)
				cmd->buttons |= IN_DUCK;
	}



	if (m_Local->Kz.flHeight <= 20 && bhop_standup_state == true && g_Vars.m_Kz.sk_kreedz_bunnyhop->value != 0 && g_Vars.m_Kz.sk_kreedz_bunnyhop_autoduck->value)
	{
		bhop_standup_state = false;
		Engine::g_Engine.pfnClientCmd(xorstr_("-duck"));
	}

	if (bhop_standup_state == true && m_Local->Kz.fOnLadder == 1)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("-duck"));
	}

	if (JumpInNextFrame)
	{
		JumpInNextFrame = false;
		cmd->buttons |= IN_JUMP;
		goto bhopfuncend;
	}
	static int inAirBhopCnt = 0; bool isJumped = false;

	if (g_Vars.m_Kz.sk_kreedz_bunnyhop->value != 0 && curFramePressedJump)
	{
		if (g_Vars.m_Kz.sk_kreedz_bunnyhop_autoduck->value)
		{
			if (m_Local->Kz.flHeight >= 30 && m_Local->Kz.flHeight <= 64 && bhop_standup_state == false && !m_Local->Kz.fOnLadder)
			{
				Engine::g_Engine.pfnClientCmd("+duck");
				bhop_standup_state = true;
				m_Local->Kz.flDuckTimeMax = Engine::g_Engine.GetClientTime();
				m_balance_bh = true;
			}
		}

		cmd->buttons &= ~IN_JUMP;
		if (((!lastFramePressedJump) || m_Local->Kz.iFlags & FL_ONGROUND || m_Local->Kz.iWaterLevel >= 2 || m_Local->Kz.fOnLadder == 1 || m_Local->Kz.flHeight <= 2))
		{
			if (true)
			{
				
				static int bhop_jump_number = 0;
				bhop_jump_number++;
				if (bhop_jump_number >= 2)
				{
					bhop_jump_number = 0;
					JumpInNextFrame = true;
					goto bhopfuncend;
				}
			}

			inAirBhopCnt = (int)Engine::g_Engine.pfnRandomFloat((int)1, (int)10);
			isJumped = true;
			cmd->buttons |= IN_JUMP;
		}
	}
	if (!isJumped)
	{
		if (inAirBhopCnt > 0)
		{
			if (inAirBhopCnt % 2 == 0) { cmd->buttons |= IN_JUMP; }
			else cmd->buttons &= ~IN_JUMP;
			inAirBhopCnt--;
		}
	}
bhopfuncend:
	lastFramePressedJump = curFramePressedJump;
}

void GameKreedz::AdjustSpeed(double x)
{
	if (m_dwSpeedptr == 0)
	{
		m_dwSpeedptr = g_Offset.GetGameAddresInfo()->dwAdSpeed;
	}
	static double LastSpeed = 1;
	if (x != LastSpeed)
	{
		*(double*)m_dwSpeedptr = (x * 1000);
		LastSpeed = x;
	}
}
bool GameKreedz::CheckKzKeyStatus(usercmd_s* cmd)
{
	bool bResult = false;

	if (g_Vars.m_Kz.sk_kreedz_groundstrafe->value == 1)
	{
		if (!(cmd->buttons & IN_JUMP))
		{
			return bResult;
		}
	}
	else if (g_Vars.m_Kz.sk_kreedz_groundstrafe->value == 3)
	{
		if (!(GetKeyState(VK_SHIFT) < 0))
		{

			return bResult;
		}
		else
		{
			RemoveShiftKey();
		}

	}
	else if (g_Vars.m_Kz.sk_kreedz_groundstrafe->value == 2)
	{
		if (!(GetKeyState(VK_MENU) < 0))
		{
			return bResult;
		}
		else
		{
			RemoveAltKey();
		}
	}
	else
	{
		RemoveAltKey();
	}

	bResult = true;
	return bResult;
}
void KzUnknownFunctions()
{

}
void GameKreedz::RemoveShiftKey()
{
	static bool InitShiftKey = false;
	if (!InitShiftKey)
	{
		pcmd_t pCmd = Engine::g_Engine.pfnGetCmdList();
		while (pCmd)
		{
			if (lstrcmpA(pCmd->name,xorstr_("+speed")) == 0)
			{
				InitShiftKey = true;
				*(PDWORD)(&pCmd->function) = (DWORD)KzUnknownFunctions;
				break;
			}
			pCmd = pCmd->next;
		}
	}
}

void GameKreedz::RemoveAltKey()
{
	static bool InitAltKey = false;
	if (!InitAltKey)
	{
		pcmd_t pCmd = Engine::g_Engine.pfnGetCmdList();
		while (pCmd)
		{
			if (lstrcmpA(pCmd->name, xorstr_("+strafe")) == 0)
			{
				InitAltKey = true;
				*(PDWORD)(&pCmd->function) = (DWORD)KzUnknownFunctions;
				break;
			}
			pCmd = pCmd->next;
		}
	}
}

void GameKreedz::Gstrafe(struct usercmd_s* cmd)
{
	if (!CheckKzKeyStatus(cmd))
	{
		return;
	}

	if (/*gstrafe && !jumpbug*/1)
	{
		static int gs_state = 0;

		if (g_Vars.m_Kz.sk_kreedz_groundstrafe_mscj->value)
		{
			if (g_Vars.m_Kz.sk_kreedz_groundstrafe_noslowdown->value && (m_Local->Kz.flGroundAngle < 5) && (m_Local->Kz.flHeight <= 0.000001f || m_Local->Kz.iFlags & FL_ONGROUND)) { AdjustSpeed(0.0001); }

			if (Engine::g_pPlayerMove->flFallVelocity >= 140)
				if (m_Local->Kz.flHeight <= 30)
					cmd->buttons |= IN_DUCK;

			//cmd->buttons |= IN_DUCK;
		}
		if (gs_state == 0 && (m_Local->Kz.iFlags & FL_ONGROUND))
		{
			if ((m_Local->Kz.flGroundAngle < 5.0f) && g_Vars.m_Kz.sk_kreedz_groundstrafe_noslowdown->value && (m_Local->Kz.flHeight <= 0.000001f || (m_Local->Kz.iFlags & FL_ONGROUND))) { AdjustSpeed(0.0001); }
			cmd->buttons |= IN_DUCK;
			gs_state = 1;
		}
		else if (gs_state == 1)
		{
			if ((m_Local->Kz.flGroundAngle < 5) && g_Vars.m_Kz.sk_kreedz_groundstrafe_noslowdown->value && (m_Local->Kz.flHeight <= 0.000001f || (m_Local->Kz.iFlags & FL_ONGROUND))) { AdjustSpeed(0.0001); }
			if (g_Vars.m_Kz.sk_kreedz_bunnyhop->value && m_Local->Kz.iUseHull == 0)
			{ 
				cmd->buttons |= IN_JUMP; 
			}
			cmd->buttons &= ~IN_DUCK;
			gs_state = 0;
		}

		if (Engine::g_pPlayerMove->velocity.Length2D() > g_Vars.m_Kz.sk_kreedz_groundstrafea_speed->value)
			AdjustSpeed(1);
	}
}

void GameKreedz::StrafeHack(float frametime, struct usercmd_s* cmd)
{
	if (!m_Local->Player.isAlive)
		return;





	if (!(Engine::g_pPlayerMove->flags & FL_ONGROUND) && Engine::g_pPlayerMove->movetype != 5)
	{
		float dir = 0.0f;
		int dir_value = g_Vars.m_Kz.sk_kreedz_autostrafe_strafedi->value;	//同步方向 (1正向AD 2向左WS 3倒序AD 4向右WS)

		if (g_Vars.m_Kz.sk_kreedz_autostrafe_autodir->value)
		{
			if (cmd->buttons & IN_FORWARD)
			{
				dir_value = 1;
			}
			else if (cmd->buttons & IN_BACK)
			{
				dir_value = 3;
			}
			else if (cmd->buttons & IN_MOVELEFT)
			{
				dir_value = 4;
			}
			else if (cmd->buttons & IN_MOVERIGHT)
			{
				dir_value = 2;
			}
		}
		if (dir_value == 1)dir = 0 * M_PI / 180.0f;
		else if (dir_value == 2)	dir = 90 * M_PI / 180.0f;
		else if (dir_value == 3)	dir = 180 * M_PI / 180.0f;
		else if (dir_value == 4)	dir = -90 * M_PI / 180.0f;


		//测试水里加速
		if (m_Local->fSpeed < 15.0f /*|| (m_Local->Kz.iFlags & FL_INWATER)*/)
		{
			if (cmd->buttons & IN_FORWARD)
			{
				if (cmd->buttons & IN_MOVELEFT)
				{
					cmd->forwardmove = 900;
					cmd->sidemove = -900;
				}
				else if (cmd->buttons & IN_MOVERIGHT)
				{
					cmd->forwardmove = 900;
					cmd->sidemove = 900;
				}
				else
					cmd->forwardmove = 900;
			}
			else if (cmd->buttons & IN_BACK)
			{
				if (cmd->buttons & IN_MOVELEFT)
				{
					cmd->forwardmove = -900;
					cmd->sidemove = -900;
				}
				else if (cmd->buttons & IN_MOVERIGHT)
				{
					cmd->forwardmove = -900;
					cmd->sidemove = 900;
				}
				else
					cmd->forwardmove = -900;
			}
			else if (cmd->buttons & IN_MOVELEFT)
				cmd->sidemove = -900;
			else if (cmd->buttons & IN_MOVERIGHT)
				cmd->sidemove = 900;
			else
				cmd->forwardmove = 900;
		}
		else
		{
			float va_speed = atan2(Engine::g_pPlayerMove->velocity.y, Engine::g_pPlayerMove->velocity.x);

			float va[3] = {};
			Engine::g_Engine.GetViewAngles(va);

			float adif = va_speed - va[1] * M_PI / 180.0f - dir;

			adif = sin(adif);
			adif = atan2(adif, sqrt(1 - adif * adif));

			cmd->sidemove = (g_Vars.m_Kz.sk_kreedz_strafe_sidemove->value) * (adif > 0 ? 1 : -1);
			cmd->forwardmove = 0;

			float angle;
			float osin, ocos, nsin, ncos;

			angle = cmd->viewangles.y * M_PI / 180.0f;
			osin = sin(angle);
			ocos = cos(angle);

			angle = 2.0f * cmd->viewangles.y * M_PI / 180.0f - va_speed + dir;
			nsin = sin(angle);
			ncos = cos(angle);

			cmd->forwardmove = cmd->sidemove * (osin * ncos - ocos * nsin);
			cmd->sidemove *= osin * nsin + ocos * ncos;

			float fs = 0;

			if (atan2(g_Vars.m_Kz.sk_kreedz_strafe_angle->value / m_Local->fSpeed, 1.0f) >= abs(adif))
			{
				QVector vBodyDirection;

				if (dir_value & 1)
					vBodyDirection = m_Local->Forward;
				else
					vBodyDirection = m_Local->Right;

				vBodyDirection[2] = 0;
				vBodyDirection = vBodyDirection.Normalize();
	
				float vel = POW(vBodyDirection[0] * Engine::g_pPlayerMove->velocity[0]) + POW(vBodyDirection[1] * Engine::g_pPlayerMove->velocity[1]);
				fs = sqrt(g_Vars.m_Kz.sk_kreedz_strafe_speed->value * 100000 / vel);
			}
			cmd->forwardmove += fs;
		}

		float sdmw = cmd->sidemove;
		float fdmw = cmd->forwardmove;

		switch ((int)dir_value)
		{
		case 1:
			cmd->forwardmove = fdmw;
			cmd->sidemove = sdmw;
			break;
		case 2:
			cmd->forwardmove = -sdmw;
			cmd->sidemove = fdmw;
			break;
		case 3:
			cmd->forwardmove = -fdmw;
			cmd->sidemove = -sdmw;
			break;
		case 4:
			cmd->forwardmove = sdmw;
			cmd->sidemove = -fdmw;
			break;
		}

	}
}

void GameKreedz::StrafeHelper(float frametime, struct usercmd_s* cmd)
{
	static int lastdir = 0;
	static int strafe_counter = 0;
	static int strafe_num = 0;
	int strafe_dir = 0;

	if (m_Local->Kz.iFlags & FL_ONGROUND || m_Local->Kz.fOnLadder == 1 || !(m_Local->Kz.iWaterLevel < 2) || (m_Local->Kz.flGroundAngle > 45.9 && m_Local->Kz.flHeight < 50))
	{
		strafe_counter = 0;
		lastdir = 0;

	}
	else
	{


		cmd->buttons &= ~IN_BACK;
		cmd->buttons &= ~IN_FORWARD;

		if (!(cmd->buttons & IN_MOVERIGHT))
			cmd->buttons &= ~IN_MOVERIGHT;

		if (!(cmd->buttons & IN_MOVELEFT))
			cmd->buttons &= ~IN_MOVELEFT;

		static int nobuttons = 0;
		if (lastdir != -1 && m_Local->Kz.fAngleSpeed < 0)
		{
			lastdir = -1;
			strafe_counter++;
			strafe_num = 0;
			nobuttons = 3;
		}
		if (lastdir != 1 && m_Local->Kz.fAngleSpeed > 0)
		{
			lastdir = 1;
			strafe_counter++;
			strafe_num = 0;
			nobuttons = 3;
		}
		strafe_num++;
		{
			switch (strafe_dir)
			{
			case 1://forward
				if (m_Local->Kz.fAngleSpeed < 0) { cmd->buttons |= IN_MOVELEFT; cmd->buttons &= ~IN_MOVERIGHT; }//mouse left - left
				if (m_Local->Kz.fAngleSpeed > 0) { cmd->buttons |= IN_MOVERIGHT; cmd->buttons &= ~IN_MOVELEFT; }//mouse right - right
				break;
			case 3: // back
				if (m_Local->Kz.fAngleSpeed < 0) { cmd->buttons |= IN_MOVERIGHT; cmd->buttons &= ~IN_MOVELEFT; }//mouse left - right
				if (m_Local->Kz.fAngleSpeed > 0) { cmd->buttons |= IN_MOVELEFT; cmd->buttons &= ~IN_MOVERIGHT; }//mouse right - left
				break;
			case 4://left
				if (m_Local->Kz.fAngleSpeed < 0) { cmd->buttons |= IN_BACK; cmd->buttons &= ~IN_FORWARD; }//mouse left - back
				if (m_Local->Kz.fAngleSpeed > 0) { cmd->buttons |= IN_FORWARD; cmd->buttons &= ~IN_BACK; }//mouse right - forward
				break;
			case 2://right
				if (m_Local->Kz.fAngleSpeed < 0) { cmd->buttons |= IN_FORWARD; cmd->buttons &= ~IN_BACK; }//mouse left - forward
				if (m_Local->Kz.fAngleSpeed > 0) { cmd->buttons |= IN_BACK; cmd->buttons &= ~IN_FORWARD; }//mouse right - back
				break;
			}
		}
		float sidespeed = 0;
		float forwardspeed = 0;

		double ms = m_Local->Kz.flXYspeed / m_Local->Kz.flMaxSpeed;
		sidespeed += 400 * (m_Local->Kz.fAngleSpeed < 0 ? -1 : 1);
		forwardspeed = 30 / ms;

		if (m_Local->Kz.fAngleSpeed > 0.0f || m_Local->Kz.fAngleSpeed < 0.0f)
		{
			switch (strafe_dir)
			{
			case 1:
				cmd->forwardmove = forwardspeed;
				cmd->sidemove = sidespeed;
				break;
			case 2:
				cmd->forwardmove = -sidespeed;
				cmd->sidemove = forwardspeed;
				break;
			case 3:
				cmd->forwardmove = -forwardspeed;
				cmd->sidemove = -sidespeed;
				break;
			case 4:
				cmd->forwardmove = sidespeed;
				cmd->sidemove = -forwardspeed;
				break;
			}
		}
	}
}

void GameKreedz::ShowLongJump(usercmd_s* cmd)
{
	if (m_bJumped && (m_Local->Kz.iFlags & FL_ONGROUND || m_Local->Kz.fOnLadder == 1))
	{
		Vector endpos = m_Local->Kz.vOrigin;
		endpos.z -= m_Local->Kz.iUseHull == 0 ? 36.0 : 18.0;
		m_Local->Kz.vt2 = endpos;
		if (endpos.z == m_Local->Kz.StartJumpPos.z)
		{
			Vector lj = endpos - m_Local->Kz.StartJumpPos;
			float dist = lj.Length() + 32.0625f + 0.003613;
			if (dist >= 200)
			{
				m_InterpFps = m_FpsCnt / m_FrameCnt;
				m_FpsCnt = 0;
				m_FrameCnt = 0;
				m_Local->Kz.flJumpDist = dist;
				m_Local->Kz.flJumpMessTime = Engine::g_Engine.GetClientTime() + 7.0;


				/*if (cvar.lj_stats_log->value)
					add_lj_log("%s-> distance: %.3f, prestrafe: %.3f, jumpoff: %.3f, strafe_aa: %.0f, fps: %.3f", timeStr, dist, PreStrafe, JumpOff, (float)cvar.strafe_aa->value, InterpFps);*/
			}
		}
		m_bJumped = false;
	}

	//
	if (!m_bJumped && m_Local->Kz.iFlags & FL_ONGROUND && cmd->buttons & IN_JUMP)
	{
		m_bJumped = true;
		m_PreStrafe = m_Local->Kz.flXYspeed;
		if (m_Local->EdgeDistance != 250)
			m_JumpOff = m_Local->EdgeDistance;
		else m_JumpOff = 0;
		m_Local->Kz.StartJumpPos = m_Local->Kz.vOrigin;
		m_Local->Kz.StartJumpPos.z -= m_Local->Kz.iUseHull == 0 ? 36.0 : 18.0;
		m_Local->Kz.vt1 = m_Local->Kz.StartJumpPos;
	}
}
double _my_abs(double n)
{
	if (n >= 0)return n; //if positive, return without ant change
	else return 0 - n; //if negative, return a positive version
}
int GameKreedz::JumpBug(float frametime, usercmd_s* cmd)
{
	static int state = 0;

	bool autojb = false;
	if (Engine::g_pPlayerMove->flFallVelocity >= PLAYER_MAX_SAFE_FALL_SPEED)
		if (m_Local->Height - (Engine::g_pPlayerMove->flFallVelocity * frametime / 1 * 15) <= 0)
			autojb = true;

	if ((autojb) && Engine::g_pPlayerMove->flFallVelocity > 0)
	{
		bool curveang = false;
		float fpheight = 0;
		if (m_Local->GroundAngle > 1)
		{
			curveang = true;
			Vector vTemp = Engine::g_pPlayerMove->origin;
			vTemp[2] -= 8192;
			pmtrace_t* trace = Engine::g_Engine.PM_TraceLine(Engine::g_pPlayerMove->origin, vTemp, 1, 2, -1);
			fpheight = fabs(Engine::g_pPlayerMove->origin.z - trace->endpos.z - (Engine::g_pPlayerMove->usehull == 1 ? 18.0f : 36.0f));
		}
		else fpheight = m_Local->Height;


		static float last_h = 0.0f;
		float cur_frame_zdist = fabs((Engine::g_pPlayerMove->flFallVelocity + (800 * frametime)) * frametime);
		cmd->buttons |= IN_DUCK;
		cmd->buttons &= ~IN_JUMP;
		switch (state)
		{
		case 1:
			cmd->buttons &= ~IN_DUCK;
			cmd->buttons |= IN_JUMP;
			state = 2;
			break;
		case 2:
			state = 0;
			break;
		default:
			if (_my_abs(fpheight - cur_frame_zdist * 1.5) <= (20.0) && cur_frame_zdist > 0.0f)
			{
				float needspd = _my_abs(fpheight - (19.0));
				float scale = fabs(needspd / cur_frame_zdist);
				AdjustSpeed(scale);
				state = 1;

			}
			break;
		}
		last_h = fpheight;
	}
	else
	{
		state = 0;
	}

	return  state;
}

void GameKreedz::SpinHack(usercmd_s* cmd)
{
	using namespace Engine;

	//if (cvar.SpinHack->value)
	{
		cl_entity_s* pLocal;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
		float newforward, newright, newup, fTime;
		float forward = cmd->forwardmove;
		float right = cmd->sidemove;
		float up = cmd->upmove;
		pLocal = g_Engine.GetLocalPlayer();
		if (!pLocal)   return;
		if (pLocal->curstate.movetype == MOVETYPE_WALK)
		{
			g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup);
		}
		else
		{
			g_Engine.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup);
		}
	
		if (pLocal->curstate.movetype == MOVETYPE_WALK && !(cmd->buttons & IN_ATTACK) && !(cmd->buttons & IN_ATTACK2) && !(cmd->buttons & IN_USE))
		{
			fTime = g_Engine.GetClientTime();
			cmd->viewangles.y = fmod(fTime * g_Vars.m_Kz.sk_kreedz_spinhackSpeed->value * 360.0f, 360.0f);
		}
		if (pLocal->curstate.movetype == MOVETYPE_WALK)
		{
			g_Engine.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup);
		}
		else
		{
			g_Engine.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup);
		}
		newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
		newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
		newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);
		cmd->forwardmove = newforward;
		cmd->sidemove = newright;
		cmd->upmove = newup;
	}
}

void GameKreedz::FastRun(float frametime, struct usercmd_s* cmd)
{
	if (Engine::g_pPlayerMove->flags & FL_ONGROUND)
	{
		static bool _FastRun = false;

		if ((cmd->buttons & IN_FORWARD && cmd->buttons & IN_MOVELEFT) || (cmd->buttons & IN_BACK && cmd->buttons & IN_MOVERIGHT))
		{
			if (_FastRun) { _FastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove -= 89.6f; }
			else { _FastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove += 89.6f; }
		}
		else if ((cmd->buttons & IN_FORWARD && cmd->buttons & IN_MOVERIGHT) || (cmd->buttons & IN_BACK && cmd->buttons & IN_MOVELEFT))
		{
			if (_FastRun) { _FastRun = false; cmd->sidemove -= 89.6f; cmd->forwardmove += 89.6f; }
			else { _FastRun = true;  cmd->sidemove += 89.6f; cmd->forwardmove -= 89.6f; }
		}
		else if (cmd->buttons & IN_FORWARD || cmd->buttons & IN_BACK)
		{
			if (_FastRun) { _FastRun = false; cmd->sidemove -= 126.6f; }
			else { _FastRun = true;  cmd->sidemove += 126.6f; }
		}
		else if (cmd->buttons & IN_MOVELEFT || cmd->buttons & IN_MOVERIGHT)
		{
			if (_FastRun) { _FastRun = false; cmd->forwardmove -= 126.6f; }
			else { _FastRun = true;  cmd->forwardmove += 126.6f; }
		}
	}
}

void GameKreedz::DrawShowKeys()
{
	int FontColor[] = {100,100,255};

	short DrawShowKeysForwardPos_x = Forward ? (Engine::g_Screen.iWidth - 150) - 1 : Engine::g_Screen.iWidth - 150; ;
	short DrawShowKeysForwardPos_y = int(Engine::g_Screen.iHeight / 2.35f);

	g_Font.DrawString(DrawShowKeysForwardPos_x, DrawShowKeysForwardPos_y,FontColor[0], FontColor[1],
		FontColor[2], "%s", Forward ? "W" : ".");

	short DrawShowKeysMoveRightPos_x = MoveRight ? (Engine::g_Screen.iWidth - 150) + 15 : (Engine::g_Screen.iWidth - 150) + 16;

	short DrawShowKeysMoveRightPos_y = DrawShowKeysForwardPos_y + 18;

	g_Font.DrawString(DrawShowKeysMoveRightPos_x, DrawShowKeysMoveRightPos_y, FontColor[0], FontColor[1],
		FontColor[2], "%s", MoveRight ? "D" : ".");

	short DrawShowKeysMoveLeftPos_x = MoveLeft ? (Engine::g_Screen.iWidth - 150) - 15 : (Engine::g_Screen.iWidth - 150) - 16;

	short DrawShowKeysMoveLeftPos_y = DrawShowKeysMoveRightPos_y;

	g_Font.DrawString(DrawShowKeysMoveLeftPos_x, DrawShowKeysMoveLeftPos_y, FontColor[0], FontColor[1],
		FontColor[2], "%s", MoveLeft ? "A" : ".");

	short DrawShowKeysBackwardPos_x = Backward ? (Engine::g_Screen.iWidth - 150) - 1 : Engine::g_Screen.iWidth - 150;
	short DrawShowKeysBackwardPos_y = DrawShowKeysMoveRightPos_y;

	g_Font.DrawString(DrawShowKeysBackwardPos_x, DrawShowKeysBackwardPos_y, FontColor[0], FontColor[1],
		FontColor[2], "%s", Backward ? "S" : ".");

	short DrawShowKeysJumpPos_x = (Engine::g_Screen.iWidth - 150) + 45;

	short DrawShowKeysJumpPos_y = Jump ? DrawShowKeysForwardPos_y : DrawShowKeysForwardPos_y + 2;

	g_Font.DrawString(DrawShowKeysJumpPos_x, DrawShowKeysJumpPos_y, FontColor[0], FontColor[1],
		FontColor[2], "%s", Jump ? "jump" : "-");

	short DrawShowKeysDuckPos_x = DrawShowKeysJumpPos_x;
	short DrawShowKeysDuckPos_y = Duck ? DrawShowKeysMoveRightPos_y : DrawShowKeysMoveRightPos_y + 2;

	g_Font.DrawString(DrawShowKeysDuckPos_x, DrawShowKeysDuckPos_y, FontColor[0], FontColor[1],
		FontColor[2], "%s", Duck ? "duck" : "-");
}

void GameKreedz::DrawKreedzInfo()
{
	short DrawKreedzPos_x = Engine::g_Screen.iWidth - 150;
	short DrawKreedzHSpeedPos_y = Engine::g_Screen.iHeight / 1.45f - 15;
	static float fMaxSpeed = 0;
	static float fPreStrafe = 0;

	int KZFontColor[] = { 0,250,154 };


	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y, KZFontColor[0],KZFontColor[1], KZFontColor[2],xorstr_("PreStrafe: %.3f"), Engine::g_pPlayerMove->velocity.Length2D());

	if (Engine::g_pPlayerMove->velocity.Length2D() > fPreStrafe)
		fPreStrafe = Engine::g_pPlayerMove->velocity.Length2D();
	if (Engine::g_pPlayerMove->velocity.Length2D() == 0)
		fPreStrafe = 0;

	short DrawKreedzMaxHSpeedPos_y = DrawKreedzHSpeedPos_y + 15;

	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzMaxHSpeedPos_y, KZFontColor[0], KZFontColor[1], KZFontColor[2],xorstr_("MaxPreStrafe: %.3f"), fPreStrafe);


	short DrawKreedzVSpeedPos_y = DrawKreedzMaxHSpeedPos_y + 15;

	float VSpeed = fabs(Engine::g_pPlayerMove->flFallVelocity);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzVSpeedPos_y, KZFontColor[0], KZFontColor[1], KZFontColor[2],xorstr_("VSpeed: %.3f"), VSpeed);

	if (VSpeed > fMaxSpeed)
		fMaxSpeed = VSpeed;
	if (fPreStrafe == 0)
		fMaxSpeed = 0;

	short DrawKreedzMAXSpeedPos_y = DrawKreedzVSpeedPos_y + 15;
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzMAXSpeedPos_y, KZFontColor[0], KZFontColor[1],KZFontColor[2],
		xorstr_("MaxSpeed: %.3f"), fMaxSpeed);


	short DrawKreedzHeightPos_y = DrawKreedzMAXSpeedPos_y + 15;

	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHeightPos_y, KZFontColor[0], KZFontColor[1],KZFontColor[2],
		xorstr_("Height: %.3f"), m_Local->Kz.flHeight);

	short DrawKreedzFallDamagePos_y = DrawKreedzHeightPos_y + 15;

	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzFallDamagePos_y, KZFontColor[0],KZFontColor[1],KZFontColor[2],
		xorstr_("FallSpeed: %.3f"), m_Local->Kz.flFallSpeed);

	short DrawKreedzLongjumpPos_y = DrawKreedzFallDamagePos_y + 15;
	if (m_Local->Kz.flJumpMessTime > Engine::g_Engine.GetClientTime())
	{
		g_Font.DrawString(DrawKreedzPos_x, DrawKreedzLongjumpPos_y, KZFontColor[0], KZFontColor[1],KZFontColor[2],
			xorstr_("LJ Distance: %.3f"), m_Local->Kz.flJumpDist);

		g_Font.DrawString(DrawKreedzPos_x, DrawKreedzLongjumpPos_y += 15, KZFontColor[0], KZFontColor[1], KZFontColor[2],
			xorstr_("PreStrafe: %.3f"), m_PreStrafe);

		g_Font.DrawString(DrawKreedzPos_x, DrawKreedzLongjumpPos_y += 15, KZFontColor[0], KZFontColor[1], KZFontColor[2],
			xorstr_("jumpoff: %.3f"), m_JumpOff);

		/*g_Font.DrawString(DrawKreedzPos_x, DrawKreedzLongjumpPos_y += 15, KZFontColor[0], KZFontColor[1], KZFontColor[2],
			xorstr_("fps: %.3f"), m_InterpFps);*/

	}
}

void GameKreedz::KzStrafe(usercmd_s* cmd)
{
	BOOL bIsAlt   = (GetKeyState(VK_MENU) < 0);
	BOOL bIsSpace = (cmd->buttons & IN_JUMP);
	BOOL bIsW     = (cmd->buttons & IN_FORWARD);
	BOOL bIsS     = (cmd->buttons & IN_BACK);
	BOOL bIsA     = (cmd->buttons & IN_MOVELEFT);
	BOOL bIsD     = (cmd->buttons & IN_MOVERIGHT);
	BOOL bIsT     = (GetKeyState('T') < 0);

	g_Vars.m_Kz.sk_kreedz_groundstrafe_mscj->value = bIsT;
	g_Vars.m_Kz.sk_kreedz_autostrafe_strafedi->value = bIsW ? 1 : 3;

	if (bIsAlt)
	{
		g_Vars.m_Kz.sk_kreedz_groundstrafe->value = 4;
		Gstrafe(cmd);

		if (bIsW || bIsS)
		{
			StrafeHack(0, cmd);
		}
		else
		{
			StrafeHelper(0, cmd);
		}
	}
	else if (bIsSpace)
	{

		if (bIsW || bIsS)
		{
			StrafeHack(0, cmd);
		}
		else
		{
			StrafeHelper(0, cmd);
		}
		g_Vars.m_Kz.sk_kreedz_bunnyhop->value = 1;
		Bhop(0, cmd);

	}
	if (bIsT)
	{
		Gstrafe(cmd);

		if (bIsW)
		{
			StrafeHack(0, cmd);
		}
	}
}
