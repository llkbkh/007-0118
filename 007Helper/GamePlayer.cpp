#include "GamePlayer.h"
#include "GameOffset.h"
#include "Util.h"
#include "Drawing.h"


GamePlayer::GamePlayer()
{

}

GamePlayer* GamePlayer::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GamePlayer();
	}
	return m_pInstance;
}

bool GamePlayer::vInit()
{
	VMProtectBegin(__FUNCTION__);

	m_Memory = g_pUtil->GetAceMemory();

	HUD_GetPlayerTeam = (PfnHUD_GetPlayerTeam)GetProcAddress(g_Offset.GetClientModuleInfo()->hModule, xorstr_("HUD_GetPlayerTeam"));

	m_Sequence.insert("ref_shoot_knife_idle1");
	m_Sequence.insert("ref_shoot_knife");
	m_Sequence.insert("ref_shoot_knife_walk");
	m_Sequence.insert("ref_shoot_knife_run");
	m_Sequence.insert("ref_shoot_knife_jump");
	m_Sequence.insert("ref_shoot_knife_longjump");
	m_Sequence.insert("zbs_attack1_run");
	m_Sequence.insert("zbs_attack2_run");
	m_Sequence.insert("zbs_attack_walk");
	m_Sequence.insert("crouch_shoot_knife");
	m_Sequence.insert("crouch_shoot_knife_crouch_idle");
	m_Sequence.insert("crouch_shoot_knife_crouchrun");
	m_Sequence.insert("fly_attack");

	VMProtectEnd();
	return true;
}




void GamePlayer::vV_CalcRefdef(ref_params_s* pparams)
{
	VectorCopy(pparams->forward, m_Local.Forward);
	VectorCopy(pparams->right, m_Local.Right);
	VectorCopy(pparams->up, m_Local.Up);
	m_Local.vPunchangle = pparams->punchangle;
}



void GamePlayer::vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{
	if (ent && ent->player && ent->index && ent->index <= Engine::g_Engine.GetMaxClients() && ent->index != m_Local.Player.Index)
	{
		m_Player[ent->index].fFrametime = ent->curstate.animtime - ent->prevstate.animtime;

		if (m_Player[ent->index].fFrametime == 0)
			m_Player[ent->index].fFrametime = m_Local.FrameTime;
		m_Player[ent->index].vVelocity = ent->curstate.origin - ent->prevstate.origin;

		m_Player[ent->index].vOrigin = ent->curstate.origin;//(g_Player[Entity->index].vVelocity * g_Player[Entity->index].fFrametime) * 0 /*cvar.prediction->value*/;

		m_Player[ent->index].fDist = VectorDistance(m_Local.Player.vEye, m_Player[ent->index].vOrigin);

		m_Player[ent->index].fAimbotFOV = ((POW(9999999.9f) * 10) / (m_Player[ent->index].fDist * m_Local.Player.iFOV));


	}
	//判断人物隐身
	if (ent && ent->index >= 1 && ent->index <= Engine::g_Engine.GetMaxClients() && ent->index != m_Local.Player.Index)
	{
		if (ent->curstate.rendermode == 4)
		{
			m_Player[ent->index].bVisible = true;
			if (m_Player[ent->index].Entity)
			{
				Vector EntityViewOrg = ent->origin;
				if (ent->curstate.usehull == 0)
					EntityViewOrg[2] += 17;
				else if (ent->curstate.usehull == 1)
					EntityViewOrg[2] += 12;
				m_Player[ent->index].Entity->origin = EntityViewOrg;
			}

		}
		else
		{
			m_Player[ent->index].bVisible = false;
		}
	}
}


void GamePlayer::vHUD_Redraw(float time, int intermission)
{
	UpdateLocalInfo();

	for (int Index = 1; Index <= Engine::g_Engine.GetMaxClients(); ++Index)
	{

		if (Index != m_Local.Player.Index)
		{
			UpdateInfoByIndex(Index);
		}
	}

#if 0
	//测试
	short DrawKreedzPos_x = Engine::g_Screen.iWidth - 150;
	short DrawKreedzHSpeedPos_y = Engine::g_Screen.iHeight / 1.45f - 15;
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y, 255, 0, 0, "TimeWeaponIdle:%f", m_weaponInfo.m_flTimeWeaponIdle);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 1, 255, 0, 0, "InReload:%d", m_weaponInfo.m_fInReload);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 2, 255, 0, 0, "m_fInSpecialReload:%d", m_weaponInfo.m_fInSpecialReload);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 3, 255, 0, 0, "NextInReload:%f", m_weaponInfo.m_flNextReload);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 4, 255, 0, 0, "ReloadTime:%f", m_weaponInfo.m_fReloadTime);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 5, 255, 0, 0, "inZoom:%d", m_weaponInfo.m_fInZoom);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 6, 255, 0, 0, "weaponState:%d", m_weaponInfo.m_iWeaponState);
	g_Font.DrawString(DrawKreedzPos_x, DrawKreedzHSpeedPos_y + 15 * 7, 255, 0, 0, "m_fun1:%f", m_weaponInfo.m_fun1);
#endif

}

void GamePlayer::vHUD_PlayerMove(struct playermove_s* ppmove, int server)
{
	Engine::g_Engine.pEventAPI->EV_LocalPlayerViewheight(m_Local.Kz.vEye);
	m_Local.Kz.vEye        = m_Local.Player.vEye + ppmove->origin;
	m_Local.Kz.iFlags      = ppmove->flags;
	m_Local.Kz.vOrigin     = ppmove->origin;
	m_Local.Kz.flXYspeed   = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));
	m_Local.Kz.vVelocity   = ppmove->velocity;
	m_Local.Kz.iWaterLevel = ppmove->waterlevel;
	m_Local.Kz.flMaxSpeed  = ppmove->maxspeed;
	m_Local.Kz.iUseHull    = ppmove->usehull;
	m_Local.Kz.flZspeed    = (ppmove->velocity[2] != 0) ? -ppmove->velocity[2] : 0;
	m_Local.Kz.flFallSpeed = ppmove->flFallVelocity;
	m_Local.Kz.fOnLadder   = ppmove->movetype == 5;
	m_Local.Kz.fAngleSpeed = (m_Local.Kz.vAngles)[1] - (ppmove->angles)[1]; while (m_Local.Kz.fAngleSpeed < -180) { m_Local.Kz.fAngleSpeed += 360; }while (m_Local.Kz.fAngleSpeed > 180) { m_Local.Kz.fAngleSpeed -= 360; }
	m_Local.Kz.vAngles     = ppmove->angles;
	Vector vel             = ppmove->velocity;
	m_Local.Kz.fVSpeed     = vel.Length();

	float fYaw                = ppmove->angles[1] * (3.1415 / 180.0);
	m_Local.Kz.fsin_yaw       = sin(fYaw);
	m_Local.Kz.fminus_cos_yaw = -cos(fYaw);

	Vector vTemp1       = m_Local.Kz.vOrigin;
	vTemp1[2] -= 8192;
	pmtrace_t* trace    = Engine::g_Engine.PM_TraceLine(m_Local.Kz.vOrigin, vTemp1, 1, ppmove->usehull, -1);
	m_Local.Kz.flHeight = fabs(trace->endpos.z - m_Local.Kz.vOrigin.z);

	if (m_Local.Kz.flHeight <= 60)  m_Local.Kz.flGroundAngle = acos(trace->plane.normal[2]) / M_PI * 180;
	else  m_Local.Kz.flGroundAngle = 0;

	Vector vTemp2 = trace->endpos;
	pmtrace_t pTrace;
	Engine::g_Engine.pEventAPI->EV_SetTraceHull(ppmove->usehull);
	Engine::g_Engine.pEventAPI->EV_PlayerTrace(m_Local.Kz.vOrigin, vTemp2, PM_GLASS_IGNORE | PM_STUDIO_BOX, m_Local.Player.Index, &pTrace);
	if (pTrace.fraction < 1.0f)
	{
		m_Local.Kz.flHeight = fabs(pTrace.endpos.z - m_Local.Kz.vOrigin.z);
		int ind = Engine::g_Engine.pEventAPI->EV_IndexFromTrace(&pTrace);
		if (ind > 0 && ind <= Engine::g_Engine.GetMaxClients() && m_Player[ind].Valid)
		{
			if (m_Player[ind].Entity)
			{
				float dst = m_Local.Kz.vOrigin.z - (m_Local.Kz.iUseHull == 0 ? 32 : 18) - m_Player[ind].Entity->origin.z - m_Local.Kz.flHeight;
				if (dst < 30)
				{
					m_Local.Kz.flHeight -= 14.0;
				}
			}

		}
	}
}



void GamePlayer::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	m_Local.FrameTime = frametime;

}

void GamePlayer::vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{
	if (runfuncs)
	{
		m_Local.iWeaponID = to->client.m_iId;
		m_Local.Player.iFOV = to->client.fov;


		memcpy(&m_weaponInfo, &to->weapondata[0],sizeof(weapon_data_t));



	}
}

void GamePlayer::StudioEntityLight()
{
	cl_entity_s* Entity = Engine::g_Studio.GetCurrentEntity();

	if (Entity && Entity->index >= 0 && Entity->index <= Engine::g_Engine.GetMaxClients())
	{
		if (Entity->index != m_Local.Player.Index && m_Player[Entity->index].Valid)
		{
			GetBoneOrigin(Entity);

		}
	}



}

LocalStruct* GamePlayer::GetLocal()
{
	return &m_Local;
}

PlayerStruct* GamePlayer::GetPlayer(int Index)
{
	return &m_Player[Index];
}

int GamePlayer::GetTeamRelationship(int Index)
{
	m_Local.Player.Team = HUD_GetPlayerTeam(m_Local.Player.Index);

	return HUD_GetPlayerTeam(Index) == m_Local.Player.Team ? 1:0;
}

void GamePlayer::UpdateLocalInfo()
{
	// 更新自己
	m_Local .Entity = Engine::g_Engine.GetLocalPlayer();

	m_Local.AngleSpeed = (m_Local.Angles)[1] - (Engine::g_pPlayerMove->angles)[1];
	while (m_Local.AngleSpeed < -180)
	{
		m_Local.AngleSpeed += 360;
	}
	while (m_Local.AngleSpeed > 180)
	{
		m_Local.AngleSpeed -= 360;
	}
	m_Local.Angles       = Engine::g_pPlayerMove->angles;
	m_Local.fOnLadder    = Engine::g_pPlayerMove->movetype == 5;
	m_Local.Player.Index = m_Local.Entity->index;
	m_Local.FPS          = 1 / m_Local.FrameTime;

	//读取自己的死活
	isAliveEntity();
	//获取地面角度?
	GetGroundAngle();
	//获取地面高度
	GetHeightGround();
	//获取边界距离?
	GetEdgeDistance();
	//跌落的损伤
	GetFallDamage();

	m_Local.Player.OnSurf = m_Local.GroundAngle > 45 && m_Local.HeightGround < 1;

	m_Local.fSpeed = Engine::g_pPlayerMove->velocity.Length2D();

	Engine::g_Engine.pEventAPI->EV_LocalPlayerViewheight(m_Local.Player.ViewOrg);

	VectorAdd(m_Local.Entity->origin, m_Local.Player.ViewOrg, m_Local.Player.ViewOrg);

	Engine::g_Engine.pEventAPI->EV_LocalPlayerViewheight(m_Local.Player.vEye);
	m_Local.Player.vEye = m_Local.Player.vEye + Engine::g_pPlayerMove->origin + (Engine::g_pPlayerMove->velocity * Engine::g_pPlayerMove->frametime);
}

void GamePlayer::isAliveEntity()
{
	//活着 3 死了 6  看队友视角 0 
	m_Local.Player.isAlive = m_Memory->Read<BYTE>(g_Offset.GetGameAddresInfo()->dwValid + (PLAYER_OFFSET * (m_Local.Player.Index - 1))) == 3 ? true : false;
}

void GamePlayer::GetGroundAngle()
{
	Vector Origin = Engine::g_pPlayerMove->origin;
	Origin[2] = -4096;
	pmtrace_t* Trace = Engine::g_Engine.PM_TraceLine(Engine::g_pPlayerMove->origin, Origin, 1, Engine::g_pPlayerMove->flags & FL_DUCKING ? 1 : 0, -1);
	m_Local.GroundAngle = acos(Trace->plane.normal[2]) / M_PI * 180;
}

void GamePlayer::GetHeightGround()
{
	Vector Origin = Engine::g_pPlayerMove->origin;

	Origin[2] = -4096;

	pmtrace_t* Trace = Engine::g_Engine.PM_TraceLine(Engine::g_pPlayerMove->origin, Origin, 1,
		(Engine::g_pPlayerMove->flags & FL_DUCKING) ? 1 : 0, -1);

	m_Local.HeightGround = Engine::g_pPlayerMove->origin.z - Trace->endpos.z;

	pmtrace_t PlayerTrace;

	Engine::g_Engine.pEventAPI->EV_SetTraceHull(Engine::g_pPlayerMove->flags & FL_DUCKING ? 1 : 0);

	Engine::g_Engine.pEventAPI->EV_PlayerTrace(Engine::g_pPlayerMove->origin, Trace->endpos,
		PM_GLASS_IGNORE | PM_STUDIO_BOX, m_Local.Player.Index, &PlayerTrace);

	if (PlayerTrace.fraction < 1)
	{
		m_Local.HeightGround = PlayerTrace.endpos.z - Engine::g_pPlayerMove->origin.z;

		uint16_t Index = Engine::g_Engine.pEventAPI->EV_IndexFromTrace(&PlayerTrace);

		//一直都是0,无法执行下面这段
 		if (Index >= 1 && Index <= Engine::g_Engine.GetMaxClients())
		{

			if (m_Player[Index].Entity)
			{
				float Distance = Engine::g_pPlayerMove->origin.z - ((Engine::g_pPlayerMove->flags & FL_DUCKING ?
					1 : 0) == 0 ? 32 : 18) - m_Player[Index].Entity->origin.z - m_Local.HeightGround;

				if (Distance < 30)
				{
					m_Local.HeightGround -= 14;
				}
			}


			
		}
	}

	Vector OriginHull = Engine::g_pPlayerMove->origin;

	OriginHull[2] = -4096;

	pmtrace_t* TraceHull = Engine::g_Engine.PM_TraceLine(Engine::g_pPlayerMove->origin, OriginHull, 1, 2, -1);

	m_Local.HeightPlane = Engine::g_pPlayerMove->origin.z - TraceHull->endpos.z - (Engine::g_pPlayerMove->usehull == 1 ? 18 : 36);
	m_Local.Height = m_Local.HeightGround;

	if (m_Local.GroundAngle >= 0.1f)
	{
		if ((m_Local.HeightPlane - m_Local.HeightGround) <= 20)
		{
			m_Local.Height = m_Local.HeightPlane;
		}
	}
}

void GamePlayer::GetEdgeDistance()
{
	float mind = 250;

	TraceEdge(-1, 0);
	TraceEdge(1, 0);
	TraceEdge(0, 1);
	TraceEdge(0, -1);
	TraceEdge(-1, -1);
	TraceEdge(1, 1);
	TraceEdge(1, -1);
	TraceEdge(-1, 1);
	m_Local.EdgeDistance = mind;
}

void GamePlayer::GetFallDamage()
{
	float FallDamage = EndSpeed(Engine::g_pPlayerMove->flFallVelocity,

	Engine::g_Engine.pfnGetGravity(), 0.01f, m_Local.Height) - PLAYER_MAX_SAFE_FALL_SPEED;
	// 掉落的损伤
	m_Local.FallDamage = FallDamage * DAMAGE_FOR_FALL_SPEED < 0 ? 0 : FallDamage * DAMAGE_FOR_FALL_SPEED;
}

void GamePlayer::UpdateInfoByIndex(uint16_t Index)
{
	m_Player[Index].Entity = Engine::g_Engine.GetEntityByIndex(Index);

	if (m_Player[Index].Entity== nullptr)
	{
		return;
	}
	Engine::g_Engine.pfnGetPlayerInfo(Index, &m_Player[Index].Info);

	isValidEntity(Index);
	isDuckedEntity(Index);
	isTeamEntity(Index, m_Player[Index].Entity->index);
	isDormantEntity(Index);
}

void GamePlayer::isValidEntity(uint16_t Index)
{
	m_Player[Index].Valid = m_Memory->Read<BYTE>(g_Offset.GetGameAddresInfo()->dwValid + (PLAYER_OFFSET * (m_Player[Index].Entity->index - 1))) == 3 ? true : false;
	//更新自己活死
	isAliveEntity();
}

bool GamePlayer::GetPlayerValid(int Index)
{
	return m_Memory->Read<BYTE>(g_Offset.GetGameAddresInfo()->dwValid + (PLAYER_OFFSET * Index)) == 3 ? true : false;
}

bool GamePlayer::CheckHitBoxValid(QVector HitBoxs, Vector Origin)
{
	auto Value = GetBoneCompareOriginDistance((Vector)HitBoxs, Origin);

	return Value < 60;
}

ULONG GamePlayer::GetPlayerCount()
{
	ULONG Count = 0;

	for (int Index = 1; Index <= Engine::g_Engine.GetMaxClients(); ++Index)
	{
		if (Index != m_Local.Player.Index)
		{
			 auto pEntity = Engine::g_Engine.GetEntityByIndex(Index);

			 if (pEntity && pEntity->player && pEntity->player_mark)
			 {
				 Count++;
			 }

		}
	}

	return Count;
}

void GamePlayer::isDuckedEntity(uint16_t Index)
{
	m_Player[Index].Ducked = m_Player[Index].Entity->curstate.maxs[2] - m_Player[Index].Entity->curstate.mins[2] < 54;
}

void GamePlayer::isTeamEntity(int PlayerId, uint16_t Index)
{
	m_Player[PlayerId].Team = HUD_GetPlayerTeam(Index);
	//更新自己阵营
	m_Local.Player.Team = HUD_GetPlayerTeam(m_Local.Player.Index);
}

void GamePlayer::isDormantEntity(uint16_t Index)
{
	m_Player[Index].Dormant = m_Memory->Read<BYTE>(g_Offset.GetGameAddresInfo()->dwDormant + (PLAYER_OFFSET * (m_Player[Index].Entity->index - 1))) == 0 ? true : false;
}

float GamePlayer::EndSpeed(float StartSpeed, float Gravity, float FrameTime, float Distance)
{
	while (Distance > 2)
	{
		StartSpeed += Gravity * FrameTime;

		float dist = StartSpeed * FrameTime;

		Distance -= dist;
	}

	return StartSpeed;
}
float fCalcFOV(Vector a, Vector b, float lena, float lenb)
{
	float l1 = 0.0f, l2 = 0.0f;

	if (lena > 0)l1 = lena; else l1 = a.Length();
	if (lenb > 0)l2 = lenb; else l2 = b.Length();

	float sc = a.x * b.x + a.y * b.y + a.z * b.z;

	return acos(sc / (l1 * l2)) * (180.0 / M_PI);
}

void MyVectorTransform(Vector in1, float in2[3][4], float* out, int xyz, float multi)
{
	in1[xyz] *= multi;

	out[0] = in1.Dot(in2[0]) + in2[0][3];
	out[1] = in1.Dot(in2[1]) + in2[1][3];
	out[2] = in1.Dot(in2[2]) + in2[2][3];
}

void GamePlayer::GetBoneOrigin(cl_entity_s* Entity)
{
	typedef float TransformMatrix[MAXSTUDIOBONES][3][4];

	/*model_s* Model = Engine::g_Studio.GetModelByIndex(m_Player[Entity->index].Entity->curstate.modelindex);
	model_s* Model1 = Engine::g_Studio.GetModelByIndex(m_Player[Entity->index].Entity->baseline.modelindex);*/

	//只会分辨僵尸模型.如果是玩家模型默认则是urban 或是 jennifer 杰西卡？不确定  (女角色默认是杰西卡 男角色是海豹突击队 .测试是这样的)
	//model_s* Model = Engine::g_Studio.GetModelByIndex(m_Player[Entity->index].Entity->prevstate.modelindex);

	//ACEDebugFileLog("{} {} {}", Model->name, Model1->name, Model2->name);

	// 索引 - 1 -> 默认是 0 开始计算的
	model_t* Model = Engine::g_Studio.SetupPlayerModel(Entity->index - 1);
	if (Model == nullptr)
	{
		return;
	}
	//判断是否是模型
	//ACEDebugFileLog("Index:{} {}", Entity->index, Model->name);

	studiohdr_t* pStudioHeader = (studiohdr_t*)Engine::g_Studio.Mod_Extradata(Model);

	TransformMatrix* pBoneMatrix = (TransformMatrix*)Engine::g_Studio.StudioGetBoneTransform();

	mstudiobbox_t* pHitbox = (mstudiobbox_t*)((byte*)pStudioHeader + pStudioHeader->hitboxindex);

	Vector vBBMin, vBBMax, vTransform, vMultiPoint;

	m_Player[Entity->index].Numhitboxes = pStudioHeader->numhitboxes - 1;
	m_Player[Entity->index].NumhitBones = pStudioHeader->numbones - 1;
	for (int i = 0; i < pStudioHeader->numbones; ++i)
	{

		m_Player[Entity->index].Bone[i][0] = (*pBoneMatrix)[i][0][3];
		m_Player[Entity->index].Bone[i][1] = (*pBoneMatrix)[i][1][3];
		m_Player[Entity->index].Bone[i][2] = (*pBoneMatrix)[i][2][3];  // 3*4 

		m_Player[Entity->index].fBoneFOV[i] = fCalcFOV((Vector)m_Local.Forward, ((Vector)m_Player[Entity->index].Bone[i] - m_Local.Player.vEye), 0, 0);

		if (CheckHitBoxValid(m_Player[Entity->index].Bone[i], m_Player[Entity->index].Entity->origin) == false)
		{
			m_Player[Entity->index].Bone->Clear();
		}

		//骨骼点肯定比命中框要多的
		if (i >= pStudioHeader->numhitboxes)
		{
			continue;
		}
		
		VectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vBBMin);
		VectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vBBMax);

		m_Player[Entity->index].vHitboxMin[i] = vBBMin;
		m_Player[Entity->index].vHitboxMax[i] = vBBMax;
		m_Player[Entity->index].vHitbox[i] = (vBBMax + vBBMin) * 0.5f + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;

		m_Player[Entity->index].fHitboxFOV[i] = m_Local.Forward.AngleBetween(m_Player[Entity->index].vHitbox[i] - m_Local.Player.vEye);

		if (CheckHitBoxValid(m_Player[Entity->index].vHitbox[i], m_Player[Entity->index].Entity->origin) == false)
		{
			m_Player[Entity->index].vHitbox->Clear();
		}

		m_Player[Entity->index].vHitboxPoints[i][0] = vBBMax + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;
		m_Player[Entity->index].vHitboxPoints[i][1] = vBBMin + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;

		vMultiPoint[0] = pHitbox[i].bbmin[0] / pHitbox[i].bbmax[0];
		vMultiPoint[1] = pHitbox[i].bbmin[1] / pHitbox[i].bbmax[1];
		vMultiPoint[2] = pHitbox[i].bbmin[2] / pHitbox[i].bbmax[2];

		MyVectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 0, vMultiPoint[0]);
		m_Player[Entity->index].vHitboxPoints[i][2] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;
		MyVectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 1, vMultiPoint[1]);
		m_Player[Entity->index].vHitboxPoints[i][3] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;
		MyVectorTransform(pHitbox[i].bbmax, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 2, vMultiPoint[2]);
		m_Player[Entity->index].vHitboxPoints[i][4] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;

		vMultiPoint[0] = pHitbox[i].bbmax[0] / pHitbox[i].bbmin[0];
		vMultiPoint[1] = pHitbox[i].bbmax[1] / pHitbox[i].bbmin[1];
		vMultiPoint[2] = pHitbox[i].bbmax[2] / pHitbox[i].bbmin[2];

		MyVectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 0, vMultiPoint[0]);
		m_Player[Entity->index].vHitboxPoints[i][5] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;
		MyVectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 1, vMultiPoint[1]);
		m_Player[Entity->index].vHitboxPoints[i][6] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;
		MyVectorTransform(pHitbox[i].bbmin, (*pBoneMatrix)[pHitbox[i].bone], vTransform, 2, vMultiPoint[2]);
		m_Player[Entity->index].vHitboxPoints[i][7] = vTransform + (Vector)m_Player[Entity->index].vVelocity * m_Player[Entity->index].fFrametime;


	}


}

BOOL GamePlayer::GetScreenInfo(int nIndex, float out[3], BYTE Type, bool isBone, int Bone)
{
	Vector TagCoorDinateobj;

	// 是否是骨骼点判断
	if (m_Player[nIndex].Entity == nullptr)
	{
		return FALSE;
	}

	TagCoorDinateobj = isBone ? (Vector)m_Player[nIndex].Bone[Bone] : m_Player[nIndex].Entity->origin;

	//判断骨骼与人物坐标之间距离 是否过大.如果过大说明.
	if (isBone)
	{
		float value =  GetBoneCompareOriginDistance((Vector)m_Player[nIndex].Bone[Bone], m_Player[nIndex].Entity->origin);

		if (value > 200)
		{
			return FALSE;
		}
	}

	
	if (Type == 1)//top
		TagCoorDinateobj.z = m_Player[nIndex].Bone[Bone].z + m_Player[nIndex].Entity->curstate.mins.z;
	else if (Type == 2) //bot
		TagCoorDinateobj.z = m_Player[nIndex].Bone[Bone].z + m_Player[nIndex].Entity->curstate.maxs.z;

	return WorldToScreen(TagCoorDinateobj, out);
}

BOOL GamePlayer::GetScreenInfo(int nIndex, float out[3], QVector hitBoxs)
{
	if (m_Player[nIndex].Entity == nullptr)
	{
		return FALSE;
	}
	float value = GetBoneCompareOriginDistance((Vector)hitBoxs, m_Player[nIndex].Entity->origin);

	if (value > 100)
	{
		return FALSE;
	}
	return WorldToScreen(hitBoxs, out);
}

std::tuple<bool, Vector> GamePlayer::GetScreenXy(int nIndex, int Bone)
{
	std::tuple<bool, Vector> result = std::make_tuple<bool, Vector>(false, Vector(0,0,0));

	Vector  ScreenTop, ScreenBot;

	if (!(GetScreenInfo(nIndex, ScreenTop, 1, true, Bone) && GetScreenInfo(nIndex, ScreenBot, 2, true, Bone)))
	{
		return result;
	}

	float Height = ScreenBot[1] - ScreenTop[1];
	float PlayerBoxHeight = m_Player[nIndex].Ducked ? Height : Height * 0.9f;

	std::get<bool>(result)     = true;
	std::get<Vector>(result).x = ScreenTop[0];
	std::get<Vector>(result).y = int(PlayerBoxHeight + ScreenTop[1]);

	return result;
}

std::deque<int> GamePlayer::GetBonePartIndex(int Index, HITBOX_TYPE Type)
{
	std::deque<int> Result;

	if (!m_Player[Index].Entity)
	{
		Result.push_back(0x7);
		return Result;
	}
		

	isValidEntity(Index);

	if (!m_Player[Index].Valid)
	{
		Result.push_back(0x7);
		return Result;
	}

	Engine::g_Engine.pfnGetPlayerInfo(Index, &m_Player[Index].Info);

	if (m_Player[Index].Info.model == NULL || m_Player[Index].Info.model[0] == '\0')
	{
		Result.push_back(0x7);
		return Result;
	}

	std::string model = m_Player[Index].Info.model;

	if (model.compare(xorstr_("zombi_host"))==0 || model.compare(xorstr_("zombi_origin")) == 0)
	{
		//小奥
		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(6);
			Result.push_back(5);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("speed_zombi_host")) == 0 || model.compare(xorstr_("speed_zombi_origin")) == 0)
	{
		//芭比
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("heavy_zombi_host")) == 0 || model.compare(xorstr_("heavy_zombi_origin")) == 0)
	{
		//屠夫
		switch (Type)
		{
		case HEAD:
			Result.push_back(13);
			break;
		case NECK:
			Result.push_back(12);
			break;
		case CHEAST:
			Result.push_back(11);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(13);
			Result.push_back(12);
			Result.push_back(11);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("heal_zombi_host")) == 0 || model.compare(xorstr_("heal_zombi_origin")) == 0)
	{
		//雾中术士

		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(6);
			Result.push_back(5);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}


	}
	else if (model.compare(xorstr_("pc_zombi_host")) == 0 || model.compare(xorstr_("pc_zombi_origin")) == 0)
	{
		//PC

		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(6);
			Result.push_back(5);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("deimos_zombi_host")) == 0 || model.compare(xorstr_("deimos_zombi_origin")) == 0)
	{
		//恶魔之子
		switch (Type)
		{
		case HEAD:
			Result.push_back(8);
			break;
		case NECK:
			Result.push_back(7);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(8);
			Result.push_back(7);
			Result.push_back(3);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("deimos2_zombi_origin")) == 0)
	{
		//恶魔猎手母体 次级模型和恶魔之子一个模型
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("witch_zombi_host")) == 0 || model.compare(xorstr_("witch_zombi_origin")) == 0)
	{
		//女妖
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(4);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(4);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("stamper_zombi_host")) == 0 || model.compare(xorstr_("stamper_zombi_origin")) == 0)
	{
		//棺材男
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("china_zombi_host")) == 0 || model.compare(xorstr_("china_zombi_origin")) == 0)
	{
		//CN僵尸
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(4);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(4);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("boomer_zombi_host")) == 0 || model.compare(xorstr_("boomer_zombi_origin")) == 0)
	{
		//暴君
		switch (Type)
		{
		case HEAD:
			Result.push_back(15);
			break;
		case NECK:
			Result.push_back(14);
			break;
		case CHEAST:
			Result.push_back(11);
			break;
		case STOMACH:
			Result.push_back(10);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(15);
			Result.push_back(14);
			Result.push_back(11);
			Result.push_back(10);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("resident_zombi_host")) == 0 || model.compare(xorstr_("resident_zombi_origin")) == 0)
	{
		//痛苦女王
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(3);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("booster_zombi_host")) == 0 || model.compare(xorstr_("booster_zombi_origin")) == 0)
	{
		//邓紫棋
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(4);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(4);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("booster_zombi_host")) == 0 || model.compare(xorstr_("booster_zombi_origin")) == 0)
	{
		//蔡徐坤
		switch (Type)
		{
		case HEAD:
			Result.push_back(14);
			break;
		case NECK:
			Result.push_back(13);
			break;
		case CHEAST:
			Result.push_back(10);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(14);
			Result.push_back(13);
			Result.push_back(10);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("flyingzombi_origin")) == 0 || model.compare(xorstr_("flyingzombi_host")) == 0)
	{
		//断翼恶灵
		switch (Type)
		{
		case HEAD:
			Result.push_back(54);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(54);
			Result.push_back(5);
			Result.push_back(3);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("akshazombi_origin")) == 0 || model.compare(xorstr_("akshazombi_host")) == 0)
	{
		//赤炎恶鬼
		switch (Type)
		{
		case HEAD:
			Result.push_back(52);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(1);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(52);
			Result.push_back(5);
			Result.push_back(2);
			Result.push_back(1);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("revival_zombi_host")) == 0 || model.compare(xorstr_("revival_zombi_origin")) == 0)
	{
		//男英雄
		switch (Type)
		{
		case HEAD:
			Result.push_back(8);
			break;
		case NECK:
			Result.push_back(7);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(8);
			Result.push_back(7);
			Result.push_back(3);
			Result.push_back(2);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("teleport_zombi_host")) == 0 || model.compare(xorstr_("teleport_zombi_origin")) == 0)
	{
		//女英雄
		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(4);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(8);
			Result.push_back(7);
			Result.push_back(3);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}
	else
	{
		//其他模型

		//ACEWarningFileLog("{}", model);

		switch (Type)
		{
		case HEAD:
			Result.push_back(7);
			break;
		case NECK:
			Result.push_back(6);
			break;
		case CHEAST:
			Result.push_back(4);
			break;
		case STOMACH:
			Result.push_back(2);
			break;
		case ALL:
			for (size_t i = 0; i < m_Player[Index].NumhitBones; i++)
			{
				Result.push_back(i);
			}
			break;
		case VITAL:
			Result.push_back(7);
			Result.push_back(6);
			Result.push_back(4);
			Result.push_back(2);
			break;
		default:
			break;
		}

	}


	return Result;
}
std::deque<int>  GamePlayer::GetHitBoxPartIndex(int Index, HITBOX_TYPE Type)
{
	std::deque<int> Result;

	if (!m_Player[Index].Entity)
	{
		Result.push_back(11);
		return Result;
	}
		

	isValidEntity(Index);

	if (!m_Player[Index].Valid)
	{
		Result.push_back(11);
		return Result;
	}

	Engine::g_Engine.pfnGetPlayerInfo(Index, &m_Player[Index].Info);

	if (m_Player[Index].Info.model == NULL || m_Player[Index].Info.model[0] == '\0')
	{
		Result.push_back(11);
		return Result;
	}

	std::string model = m_Player[Index].Info.model;


	if (model.compare(xorstr_("zombi_host")) == 0 || model.compare(xorstr_("zombi_origin")) == 0)
	{
		//小奥
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
			break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("speed_zombi_host")) == 0 )
	{
		//芭比
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(7);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(7);
			Result.push_back(0);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("speed_zombi_origin")) == 0)
	{
		//芭比母体
		switch (Type)
		{
		case HEAD:
			Result.push_back(10);
			break;
		case NECK:
			Result.push_back(9);
			break;
		case CHEAST:
			Result.push_back(8);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(10);
			Result.push_back(9);
			Result.push_back(8);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}

	else if (model.compare(xorstr_("heavy_zombi_host")) == 0 || model.compare(xorstr_("heavy_zombi_origin")) == 0)
	{
		//屠夫
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(7);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(7);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("heal_zombi_host")) == 0 || model.compare(xorstr_("heal_zombi_origin")) == 0)
	{
		//雾中术士
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(7);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(7);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("pc_zombi_host")) == 0 || model.compare(xorstr_("pc_zombi_origin")) == 0)
	{
		//PC
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("deimos_zombi_host")) == 0 || model.compare(xorstr_("deimos_zombi_origin")) == 0)
	{
		//恶魔之子
		switch (Type)
		{
		case HEAD:
			Result.push_back(0);
			break;
		case NECK:
			Result.push_back(1);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(4);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(0);
			Result.push_back(1);
			Result.push_back(3);
			Result.push_back(4);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("deimos2_zombi_origin")) == 0 || model.compare(xorstr_("deimos2_zombi_host")) == 0)
	{
		//恶魔猎手母体 次级模型和恶魔之子一个模型
		switch (Type)
		{
		case HEAD:
			Result.push_back(0);
			break;
		case NECK:
			Result.push_back(1);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(4);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(0);
			Result.push_back(1);
			Result.push_back(3);
			Result.push_back(4);
			break;
		default:
			break;
		}
	}
	else if (model.compare(xorstr_("witch_zombi_host")) == 0 || model.compare(xorstr_("witch_zombi_origin")) == 0)
	{
		//女妖
		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(3);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(6);
			Result.push_back(5);
			Result.push_back(3);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("stamper_zombi_host")) == 0 || model.compare(xorstr_("stamper_zombi_origin")) == 0)
	{
		//棺材男
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(8);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(8);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("china_zombi_host")) == 0 || model.compare(xorstr_("china_zombi_origin")) == 0)
	{
		//CN僵尸
		switch (Type)
		{
		case HEAD:
			Result.push_back(5);
			break;
		case NECK:
			Result.push_back(4);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(5);
			Result.push_back(4);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("boomer_zombi_host")) == 0 || model.compare(xorstr_("boomer_zombi_origin")) == 0)
	{
		//暴君
		switch (Type)
		{
		case HEAD:
			Result.push_back(3);
			break;
		case NECK:
			Result.push_back(2);
			break;
		case CHEAST:
			Result.push_back(1);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(3);
			Result.push_back(2);
			Result.push_back(1);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("resident_zombi_host")) == 0 || model.compare(xorstr_("resident_zombi_origin")) == 0)
	{
		//痛苦女王
		switch (Type)
		{
		case HEAD:
			Result.push_back(5);
			break;
		case NECK:
			Result.push_back(4);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(5);
			Result.push_back(4);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("booster_zombi_host")) == 0 || model.compare(xorstr_("booster_zombi_origin")) == 0)
	{
		//邓紫棋
		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(5);
			Result.push_back(4);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("booster_zombi_host")) == 0 || model.compare(xorstr_("booster_zombi_origin")) == 0)
	{
		//蔡徐坤
		switch (Type)
		{
		case HEAD:
			Result.push_back(11);
			break;
		case NECK:
			Result.push_back(10);
			break;
		case CHEAST:
			Result.push_back(9);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(11);
			Result.push_back(10);
			Result.push_back(9);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("flyingzombi_origin")) == 0 || model.compare(xorstr_("flyingzombi_host")) == 0)
	{
		//断翼恶灵
		switch (Type)
		{
		case HEAD:
			Result.push_back(17);
			break;
		case NECK:
			Result.push_back(4);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(17);
			Result.push_back(4);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("akshazombi_origin")) == 0 || model.compare(xorstr_("akshazombi_host")) == 0)
	{
		//赤炎恶鬼
		switch (Type)
		{
		case HEAD:
			Result.push_back(15);
			break;
		case NECK:
			Result.push_back(4);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(15);
			Result.push_back(4);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("revival_zombi_host")) == 0 || model.compare(xorstr_("revival_zombi_origin")) == 0)
	{
		//男英雄
		switch (Type)
		{
		case HEAD:
			Result.push_back(0);
			break;
		case NECK:
			Result.push_back(1);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(4);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(0);
			Result.push_back(1);
			Result.push_back(2);
			Result.push_back(4);
			break;
		default:
			break;
		}

	}
	else if (model.compare(xorstr_("teleport_zombi_host")) == 0 || model.compare(xorstr_("teleport_zombi_origin")) == 0)
	{
		//女英雄
		switch (Type)
		{
		case HEAD:
			Result.push_back(6);
			break;
		case NECK:
			Result.push_back(5);
			break;
		case CHEAST:
			Result.push_back(2);
			break;
		case STOMACH:
			Result.push_back(0);
			break;
		case ALL:
		{
			for (size_t i = 0; i < m_Player[Index].Numhitboxes; i++)
			{
				Result.push_back(i);
			}
		}
		break;
		case VITAL:
			Result.push_back(6);
			Result.push_back(5);
			Result.push_back(2);
			Result.push_back(0);
			break;
		default:
			break;
		}

	}
	else
	{
		//其他模型
		//ACEWarningFileLog("{}", model);

		Result.push_back(11);
		Result.push_back(10);
		Result.push_back(2);
		Result.push_back(0);




	}

	return Result;
}



mstudioseqdesc_t* GamePlayer::GetSequence(int PlayerID, int Sequence)
{
	model_t* Model = Engine::g_Studio.SetupPlayerModel(PlayerID - 1);
	if (Model == nullptr)
	{
		return nullptr;
	}
	studiohdr_t* pStudioHdr = (studiohdr_t*)Engine::g_pStudio->Mod_Extradata(Model);

	if (!pStudioHdr) return nullptr;

	return &((mstudioseqdesc_t*)((BYTE*)pStudioHdr + pStudioHdr->seqindex))[Sequence];
}
BOOL GamePlayer::ZombiIsAttack(int PlayerID, int Sequence)
{
	mstudioseqdesc_t* Infp = GetSequence(PlayerID, Sequence);

	if (Infp)
	{
		if (m_Sequence.count(Infp->label))
		{
			return TRUE;
		}
	}
	return FALSE;
}




ULONG GamePlayer::GetBoneByModel(char* Model)
{
	return 0;
}

bool GamePlayer::IsSetTeam(int nIndex, int nTeamType)
{
	if (nTeamType == 1 && m_Player[nIndex].Team != m_Local.Player.Team)
		//敌军
		return true;
	else if (nTeamType == 2 && m_Player[nIndex].Team == m_Local.Player.Team)
		//友军
		return true;
	else if (nTeamType == 3)
		//全部
		return true;
	return false;
}

bool GamePlayer::PathFree(float* pflFrom, float* pflTo)
{
	if (!pflFrom || !pflTo)
		return false;

	pmtrace_t pTrace;

	Engine::g_Engine.pEventAPI->EV_SetTraceHull(2);
	Engine::g_Engine.pEventAPI->EV_PlayerTrace(pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, m_Local.Player.Index, &pTrace);

	return (bool)(pTrace.fraction == 1.0f);
}

float GamePlayer::GetPlayerDistance(uint16_t Index, bool Meters)
{
	if (m_Player[Index].Entity == NULL) return 0.0f;

	float Distance = floor(sqrt(POW(fabs(m_Player[Index].Entity->origin.x - m_Local.Entity->origin.x)) +
		POW(fabs(m_Player[Index].Entity->origin.y - m_Local.Entity->origin.y)) +
		POW(fabs(m_Player[Index].Entity->origin.z - m_Local.Entity->origin.z))) - 32);

	if (Meters)
	{
		return Distance < 0 ? fabs(Distance) : Distance * 0.025f;
	}
	else
	{
		return Distance < 0 ? fabs(Distance) : Distance;
	}
}

float GamePlayer::GetBoneCompareOriginDistance(Vector Bone, Vector Origin)
{

	float Distance = floor(sqrt(POW(fabs(Bone.x - Origin.x)) +
		POW(fabs(Bone.y - Origin.y)) +
		POW(fabs(Bone.z - Origin.z))) - 32);

	return fabs(Distance);
}

weapon_data_t* GamePlayer::GetWeaponInfo()
{
	return &m_weaponInfo;
}


