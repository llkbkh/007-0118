#pragma once
#include "007Helper.h"
#include "GameDef.h"
#include "IGameClient.h"
#include <deque>
#include <vector>
#include <set>

struct LocalStruct
{
	KZStruct Kz;

	LocalPlayerStruct Player;
	cl_entity_s* Entity;

	QVector Angles, Forward, Right, Up;

	float	punchangle[3];

	double* g_Net;

	float FPS, FrameTime, EdgeDistance;

	float GroundAngle, FallDamage, AngleSpeed;

	float HeightGround, HeightPlane, Height;

	float fSpeed;

	float fOnLadder;

	float flDuckTimeMax;

	Vector vPunchangle;

	Vector vViewAngles;

	spread_info spread;

	Vector vNoSpreadAng, vNoSpreadAngScr;

	float FreeLookOffset[3];

	float FreeLookAngles[3];

	int  iWeaponID;

	int  iClipAmmo;

	int  iClipAmmo2;

	int  iClip;

	net_status_s Status;

	CBasePlayerWeapon weapon;

	bool Trigger[MAX_PALYER];
	// 技能栏 
	bool	skill1, skill2, skill3, skill4;
};

struct PlayerStruct
{
	hud_player_info_t Info;
	cl_entity_s* Entity;
	QVector Bone[1024],vOrigin, vVelocity;
	float fBoneFOV[1024], fAimbotFOV, fFrametime;
	Vector Angles;
	int Team;
	//活体,可见,下蹲
	bool Valid, Visible, Ducked;
	bool IsAimed;//是否正在瞄准
	bool Dormant; //休眠
	bool bGotHead;
	float fDist;
	bool bVisible;//是否在隐身?
	int Aiming;	//瞄准点

	//坐标点是否可见
	bool vOriginVisible;

	//命中框
	QVector vHitbox[1024];
	QVector vHitboxMin[1024];
	QVector vHitboxMax[1024];
	QVector vHitboxPoints[1024][8];
	//
	ULONG Numhitboxes;
	ULONG NumhitBones;
	//可见的命中
	std::vector<int> VecHitboxVisible;

	float fHitboxFOV[1024];




};



class GamePlayer:public IGameClient
{
public:
	GamePlayer();
	static GamePlayer* GetInstance();

	virtual bool vInit();
	virtual void vV_CalcRefdef(ref_params_s* pparams);
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname);
	virtual void vHUD_Redraw(float time, int intermission);
	virtual void vHUD_PlayerMove(struct playermove_s* ppmove, int server);
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);




	void  StudioEntityLight();
	LocalStruct* GetLocal();
	PlayerStruct* GetPlayer(int Index);
	//@获取队友关系 1 队友 0 敌军
	int GetTeamRelationship(int Index);
	/*
	*	@取屏幕信息转换
	*	@Param:玩家索引
	*	@Param:输出
	*	@Param:Type
	*	@Param:是否是骨骼
	*	@Param:骨骼点
	*/
	BOOL  GetScreenInfo(int nIndex, float out[3], BYTE Type, bool isBone, int Bone);

	BOOL  GetScreenInfo(int nIndex, float out[3],QVector hitBoxs);


	std::tuple<bool, Vector>  GetScreenXy(int nIndex, int Bone);
	//@获取骨骼部位索引
	std::deque<int> GetBonePartIndex(int Index, HITBOX_TYPE Type);
	//通过模型获取骨骼
	ULONG GetBoneByModel(char* Model);
	//检查玩家阵营
	bool IsSetTeam(int nIndex, int nTeamType);
	//判断是否可见
	bool PathFree(float* pflFrom, float* pflTo);
	//@获取玩家之间的距离
	float GetPlayerDistance(uint16_t Index, bool Meters);
	//@取骨骼与人物坐标的距离
	float GetBoneCompareOriginDistance(Vector Bone, Vector Origin);
	//@返回武器信息
	weapon_data_t* GetWeaponInfo();

	//@获取骨骼部位索引
	std::deque<int> GetHitBoxPartIndex(int Index, HITBOX_TYPE Type);

	mstudioseqdesc_t* GetSequence(int PlayerID, int Sequence);
	BOOL ZombiIsAttack(int PlayerID, int Sequence);


	bool GetPlayerValid(int Index);

	bool CheckHitBoxValid(QVector HitBoxs, Vector Origin);

	//@取玩家数量(排除BOT)
	ULONG GetPlayerCount();
private:
	//@更新自己的信息
	inline void UpdateLocalInfo();
	//@更新自己死活
	inline void isAliveEntity();
	inline void GetGroundAngle();
	inline void GetHeightGround();
	inline void GetEdgeDistance();
	inline void GetFallDamage();	//获取掉落伤害
	inline void UpdateInfoByIndex(uint16_t Index);
	inline void isValidEntity(uint16_t Index);


	inline void isDuckedEntity(uint16_t Index);
	inline void isTeamEntity(int PlayerId, uint16_t Index);
	inline void isDormantEntity(uint16_t Index);
	float EndSpeed(float StartSpeed, float Gravity, float FrameTime, float Distance);
	void  GetBoneOrigin(cl_entity_s* Entity);
private:
	LocalStruct                 m_Local;
	PlayerStruct	            m_Player[MAX_PALYER];
	std::shared_ptr<CACEMemory> m_Memory;
	static inline GamePlayer*   m_pInstance = nullptr;
	typedef int(*PfnHUD_GetPlayerTeam)(int nTeam);
	static inline PfnHUD_GetPlayerTeam HUD_GetPlayerTeam = nullptr;
	weapon_data_t               m_weaponInfo;
	std::set<std::string>       m_Sequence;


#define TraceEdge(x,y){\
	Vector Start =  m_Local.Entity->origin;\
	Vector AirTemp = Start;\
	AirTemp[2] = -4096;\
	pmtrace_t *Trace = Engine::g_Engine.PM_TraceLine ( Start, AirTemp, 1.0f, ( Engine::g_pPlayerMove->flags & FL_DUCKING ) ? 1 : 0, -1 );\
	Vector Dis = ( AirTemp -  m_Local.Entity->origin ) * Trace->fraction;\
	Start[2] -= 0.1f;\
	Start[2] -= -Dis[2];\
	Vector End = Start;\
	End[1] += x * mind;\
	End[0] += y * mind;\
	pmtrace_s* Trace_s = Engine::g_Engine.PM_TraceLine(End, Start, 1.0f, ( Engine::g_pPlayerMove->flags & FL_DUCKING ) ? 1 : 0, -1);\
	if ( !( Trace_s->startsolid ) )\
	{\
		mind = ( Trace_s->endpos - Start ).Length2D ( );\
	}\
}
};

