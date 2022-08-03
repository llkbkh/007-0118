#pragma once
#include "007Helper.h"
#include "GamePlayer.h"
#include <deque>

class GameEsp:public IGameClient,public IGameEngineMSG
{

public:
	GameEsp();
	static GameEsp* GetInstance();
	virtual bool vInit();
	virtual void vHUD_Redraw(float time, int intermission);
	virtual void vV_CalcRefdef(ref_params_s* pparams);
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname);
	//---------MSG--------
	virtual int vBombDrop(const char* pszName, int iSize, void* pbuf);
	virtual int vResetHUD(const char* pszName, int iSize, void* pbuf);
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);
	virtual int vPatrolDrone(const char* pszName, int iSize, void* pbuf);

private:
	//@画玩家
	void	DrawPlayer(int Index);
	// * 画出姓名
	void DrawPlayerName(int Index, float ScreenTop[3], float ScreenBot[3], float Height,float PlayerBoxHeight);
	// * 画出方框
	void DrawPlayerBox(int Index, float ScreenTop[3], float ScreenBot[3], bool bVisible, int Bones);
	// * 画出武器
	void DrawWeapon(int Index, float ScreenTop[3], float ScreenBot[3]);
	// * 画出距离
	void DrawDistance(int Index, float ScreenTop[3], float ScreenBot[3]);
	// * 获取颜色
	void GetColor(uint16_t Index);
	// * 人物染色
	void DoExtraRendering(cl_entity_s* ent, ColorEntry color);
	// * 迷你雷达
	void drawMiniRadarPoint(const float* origin, int r, int g, int b, bool addbox, int boxsize);
	// * 计算雷达
	void calcRadarPoint(const float* origin, int& screenx, int& screeny);
	// * 人物坐标是否在屏幕内
	BOOL IsScreenIn(int Index);
	// * 显示敌人预警
	void DrawWarning();
	// * 获取菜单子菜单项
	std::string GetInMenuSubItem(char* Item);
	// * 显示C4
	void DrawC4Info();
	// * 显示手雷轨迹(预测)
	void GranadeTrailFunction(usercmd_s* cmd);
	// * 手雷跟踪
	void TraceGrenade(float* dest);
	// * 画线
	void DrawLinePoints(float* start, float* end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b);

	static void PreS_DynamicSound(int entid, DWORD entchannel, char* szSoundFile, float* fOrigin, float fVolume, float fAttenuation, int iTimeOff, int iPitch);
	// * 声音雷达
	void SoundESP();
	// * 画出视角追踪线
	void DrawTraceAngleLine(cl_entity_s* ent);
	// * 画出移动追踪轨迹
	void DrawTraceMove(cl_entity_s* ent);

	void R_BeamFollow(cl_entity_s* pEnt, ColorEntry* pClr, float life, float width);

	// * 添加地图中物品
	void AddWorld(cl_entity_s * Entity, const char* modelname);
	// * 呼出地图中的物品
	void DrawWorld();
	// * Push实体
	void PushEntity(ULONG Index,std::string Name, vec3_t origin, ADD_ENTITY_TYPE Type);
	// * 显示 巡逻机信息
	void DrawPatrolDrone();
	// * 获取巡逻机状态
	std::string GetPatrolState(Patrol_State Type);
	// * 获取巡逻机颜色
	TRIVERTEX* GetPatrolColor(GAME_PATROL_MSG* pInfo, int Index);
private:
	static inline GameEsp*       m_Instance = nullptr;
	LocalStruct*                 m_Local;
	GamePlayer*					 m_GamePlayer;
	BYTE                         m_BoxColor[3];
	Vector                       m_ViewAngles, m_ViewOrigin;
	int                          m_PlayerNumer, m_CurrentBackDistanceUnits, m_CurrentPlayerIndex;
	std::deque<sound_t>          m_Sounds;
	std::deque<EntityStruct>	 m_Entity;
	//巡逻机信息
	GAME_PATROL_MSG              m_PatrolDrone;
	//巡逻机颜色
	TRIVERTEX					 m_PatrolColor;
};

