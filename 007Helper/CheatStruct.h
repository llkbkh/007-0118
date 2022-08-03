#pragma once
#include "GameSDK.h"



enum TEAM
{
	SPECTATOR = 0,
	TERRORIST = 1,
	CT        = 2,
};

typedef struct
{
	double v1;
	double mouse_timer;
	double keyboard_timer;
}TAG_VGUI_INPUT, * PTAG_VGUI_INPUT;

typedef struct
{
	int id;
	Vector origin;
	std::string name;
	DWORD timestamp;
} sound_t;

typedef struct
{
	int		count;
	char* Ponint_un_name_1[1];
	char* Ponint_un_name_2[1];
	char* Ponint_item_code[1];
	char* Ponint_item_name[1];
	int     un_int_1;
	int     money;		                                //价格
	int     un_int_3;		                            //武器限制？？ 7是 通用
	int     un_int_4;
	int     type;		                                //武器类型
	float	un_float_1;		                            //一般都为-1
	float	un_float_2;

}TAG_GAME_ITEMS, * PTAG_GAME_ITEMS;

enum Game_ClassType
{
	ClassType_Unknown,
	ClassType_Equipment,			                    //装备
	ClassType_Class,				                    //角色
	ClassType_PeriodicItem,			                    //喷漆(周期性的)
	ClassType_Tattoo,				                    //纹身
	ClassType_zbRespawnEffect,		                    //生化复活效果
														//太多了 一般就用这两个
};
typedef struct
{
	char	RecourceName[MAX_PATH];				        //资源名
	char	ClassName[MAX_PATH];				        //类名
	ULONG	Category;							        //分类的ID
	ULONG	InGameID;							        //在游戏里的ID
	ULONG	SortingIndex;						        //排序ID
	Game_ClassType RecourceType;				        //资源类型
	WCHAR	ItemSrc[MAX_PATH];							//itemName
}GAME_ITEMMAP, * PGAME_ITEMMAP;

typedef struct
{
	int map_id;					                        //地图ID
	std::string map_name;			                    //地图名
	std::string map_code;			                    //地图代码
	std::wstring map_translation;                       //翻译的地图名
}GAME_MAPINFO, * PGAME_MAPINFO;



enum HITBOX_TYPE
{
	RESERVE = 0 ,
	HEAD,			//头
	NECK,			//颈部
	CHEAST,			//胸部
	STOMACH,		//腹部
	ALL,			//所有部位
	VITAL,			//重要部位
};





struct ColorEntry
{
	DWORD r, g, b, a;
	DWORD compiled;
	float fr, fg, fb;
	float onebased_r, onebased_g, onebased_b, onebased_a;
	ColorEntry() :r(255), g(255), b(255), a(255), fr(1.0), fg(1.0), fb(1.0),
		onebased_r(1.0), onebased_g(1.0), onebased_b(1.0), onebased_a(1.0),
		compiled(0xFFFFFFFF) {}
	void fill_missing();
};





#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}
struct CBasePlayerWeapon
{
	bool m_bDelayFire;
	unsigned int random_seed;
	int m_iWeaponState;
	int m_iFlags;
	int m_iClip;
	int m_iWeaponID;
	int m_iInReload;
	int iPenetration;
	int iBulletType;
	int iDamage1;
	int iDamage2;
	int m_iShotsFired;
	int iuser3;
	float prevtime;
	float curtime;
	float m_flDecreaseShotsFired;
	float m_flAccuracy;
	float m_flSpread;
	float m_flNextPrimaryAttack;
	float m_flNextSecondaryAttack;
	float m_flNextAttack;
	float flPenetrationDistance;
	float flWallPierce1;
	float flWallPierce2;
};
struct spread_info
{
	unsigned int random_seed;
	float gtime;
	float prevtime;
	float brokentime;
	float spreadvar;
	float recoiltime;
	bool firing;
	int WeaponState;
	int prcflags;
	int recoil;
};

struct LocalPlayerStruct
{
	Vector ViewOrg, vEye;
	int iFOV;
	DWORD Speed;
	int Team, Index;
	// 是否是活的实体
	bool isAlive, OnSurf;
	// 0 = 掉落  1 = 安装了炸弹  2 = null
	int BombDrop;
	Vector Bone[80];
};
struct KZStruct
{
	float fOnLadder;
	float flHeight;
	float flGroundAngle;
	float flFpsKbMessTime;
	float flDuckTimeMax;
	float flFrametime;
	float flJumpMessTime;
	float flFallSpeed;
	float flZspeed;
	float flJumpDist;
	float flXYspeed;
	float flMaxSpeed;
	float fAngleSpeed;
	float fsin_yaw;
	float fminus_cos_yaw;
	float sHudTimeDown;
	float fVSpeed;
	int iWaterLevel;
	int iHealth;
	int iFlags;
	int iUseHull;
	int iFOV;
	Vector vRight;
	Vector vForward;
	Vector vEye;
	Vector vt1, vt2;
	Vector vOrigin;
	Vector vNorecoilAng;
	Vector vPunchangle;
	Vector vVelocity;
	Vector vAngles;
	Vector StartJumpPos;
	Vector pmEyePos;
};



/*
	* 国服游戏服务器大区
*/
enum GameServerRegion
{
	D1,
	D2,
	D3,
	W1,
	W2
};
/*
	全服游戏服务器
*/
enum GameServer
{
	Chinese,		                                    // * 国服
	TaiWan,			                                    // * 台服
	Steam,			                                    // * CSNS
	Kor				                                    // * 韩服
};

typedef struct cvar7_s
{
	std::string	name;
	std::string	string;
	int		flags;
	float	value;
	DWORD	hotkey;
	struct cvar7_s* next;
} cvar7_s;
struct GameMpAddres
{
	DWORD	Map;			                            //本地换图地址
	DWORD	Hp;				                            //血量
	DWORD	HpArmor;		                            //护甲
	DWORD	dwBase;			                            //基址
	DWORD	dwMpBase;		                            //本地基址
	DWORD	dwGod;			                            //无敌
	DWORD	dwFly;			                            //飞天
	DWORD	dwGSpeed;		                            //地速  最高390
	DWORD	dwIsMpRoom;		                            //是否在本地房间
	DWORD	dwReleaseHold;	                            //解除刷枪限制
	DWORD	dwRecoil;		                            //反后坐力
	DWORD   dwhookDamage;
	DWORD   dwAlreadyOwnWeapon;                         //限制同一武器
	DWORD   dwBuyThis;		                            //无法购买此道具
	DWORD   dwBuyThisCallCheck;	                        //无法购买此道具上层Call 下面还有检测
	DWORD   dwBuyThisJmpAddres;	                        //无法购买此道具上一层CALL 的跳转地址
	DWORD   dwCheckBuyTimer;	                        //检查购买时间
	DWORD   dwMpRoomCheck;	                            //本地房间检查
	DWORD   dwEnoughMoney;	                            //资金不足检查
	DWORD   dwCannotBeDropped;                          //无法丢弃武器检查
	DWORD   dwHm_skill;		                            //技能限制
	DWORD   dwBuyKnife;		                            //购买刀限制
	DWORD   dwReciprocalTimerCheck;		                //倒数后 购买枪支限制

	DWORD   dwHm_skill_Z;				                //技能限制_生化Z
	DWORD   dwCantHeadShotUsed;			                //致命打击是否可用
	DWORD   dwHeadShot_Offset;			                //致命打击偏移
	DWORD   dwHm_skillDuration_Offset;                  //致命打击持续时间偏移
	DWORD   dwHm_skillDuration_Hook;                    //致命打击Hook持续地址

	DWORD   PVEDamageCall;	                            //PVE (灾变)
	DWORD   PVPDamageCall;	                            //PVP (团队)
	DWORD   enginefuncs_s;
	DWORD   dwKnifeOffset;
	DWORD   dwPlayerRoleOffset;

	DWORD   dwPlayerRpnOffset;		                    //人物复活_重选人物
	DWORD   dwPlayerRoleDetection;	                    //人物角色检查

	DWORD   dwReleaseHoldEx;	                        //解除限制 2019年11月6日 22:51:32 更新后用这个



														//----------------------与玩家相关的信息-----------

	DWORD dwPlayer;				                        //玩家相关的信息			
	DWORD dwPlayerOffSet;		                        //偏移

	DWORD dwNextPlayerOffSet;	                        //玩家之间的偏移

	DWORD dwCamp;				                        //阵营
	DWORD dwCampOffSet;			                        //阵营偏移

	DWORD dwMoney;				                        //金钱
	DWORD dwMoneyOffSet;		                        //金钱偏移

	DWORD dwSubWeapon;			                        //副武器
	DWORD dwSubWeaponOffSet;	                        //副武器偏移

	DWORD dwMainWeapon;			                        //主武器
	DWORD dwMainWeaponOffSet;	                        //主武器偏移

	DWORD dwWeaponBulletOffSet;                         //武器子弹
	DWORD dwWeaponRecoilOffSet;                         //武器的后坐力
	DWORD dwWeaponSpeedOffSet;                          //武器的加速

	DWORD dwAttackCall;
	DWORD dwAttack2Call;
};





struct Gamefunctions
{
	DWORD	ItemIdToItemCode;			                //ID->代码
	DWORD	ItemCodeToItemName;			                //代码->名称
	DWORD	DispatchEntry;				                //类
	DWORD	GetItemEntryCall;
};
struct GameAddres
{
	DWORD dwCampPtr;	                                //阵营   CT 2 T1
	DWORD dwAngletr;	                                //视角   上下 89 Y 
	DWORD dwCameraFOV;                                  //FOV    90
	DWORD dwMyBone;		                                //自己骨骼
	DWORD dwValid;		                                //死活
	DWORD dwIsGame;		                                //是否在游戏中
	DWORD dwDormant;	                                //休眠
	DWORD dwMixRoom;	                                //混房
	DWORD dwRetRoom;	                                //返回房间
	DWORD dwNightVision;                                //夜视
	DWORD dwShowHP;		                                //显血
	DWORD dwShowHP2;	                                //百分比显血
	DWORD dwThirdPerson;		                        //第三视角
	DWORD dwMsg;		                                //消息弹窗
	DWORD dwBeamPoints;
	DWORD dwPreSCall;
	DWORD dwBallistic;		                            //暴走效果

	DWORD dwGuiMenu;		                            //菜单
	DWORD dwNoKeyInput;		                            //挂机
	DWORD dwGameLogin;		                            //游戏登录Call	不再更新
	DWORD dwPacketCount;	                            //封包地址
	DWORD dwRoomMode;		                            //房间模式
	DWORD dwCheckOpengl;	                            //检查Opengl检测
	DWORD dwCheckProcess;	                            //检查进程
	DWORD dwOutline;		                            //描边透视
	DWORD dwMapBrightness;	                            //地图高亮
	DWORD dwCommandHook;	                            //指令HOOK
	DWORD dwItemPoint;		                            //物品指针
	DWORD dwItemEntry;		                            //主要查找ItemCount
	DWORD dwItemCount;		                            //主要查找↑ 不明所以
	DWORD dwbadWord;		                            //解除脏话
	DWORD dwbadWordJmp;		                            //解除脏话跳转地址
	DWORD dwAFK;			                            //挂机时间判断
	DWORD dwCallEngineSurfaceProc;
	DWORD dwPlayerWeaponInfo;
	DWORD dwVoxlThisPoint;	                            //主要作用还是判断缔造者的房间
	DWORD dwPlayerSequence;
	DWORD dwRandCallRetAddres;							//随机种子.返回地址
	DWORD dwPlayerMgrAddres;
	DWORD dwPlayerMgrCall;								//通过索引获取玩家名称 所需要的CALL
	DWORD dwGetRoundCall;		                        //获取灾变回合数CALL
	DWORD dwAdSpeed;
	DWORD dwGameMesge;			                        //连接超时 游戏通知
	DWORD dwGameVguiIpunt;			                    //鼠标键盘检测 
	DWORD dwGameVguiMouseIpunt;		                    //鼠标检测 
	DWORD dwGameVguiIpuntTime;		                    //输出时间

	DWORD dwNightVisionColor;		                    //夜视颜色
	DWORD dwCheckEngineAddres;							//游戏检查引擎地址

	DWORD dwSpeed[10];									//加速地址
	DWORD PreS_DynamicSoundCall;						//声音CALL
	DWORD dwIsHomeowners;								//是否是房主

};



typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t* pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t* pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s* ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s* ppmove);
	char(*HUD_PlayerMoveTexture) (char* name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_MouseRawInput) (int mstate);			    //2021-03-12 11:07:02  更新添加的
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s* cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float* ofs);
	struct kbutton_s* (*KB_Find) (const char* name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s* pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s* ent, const char* modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s* event, const struct cl_entity_s* entity);
	void(*HUD_PostRunCmd) (struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s* state, const struct clientdata_s* client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s* dst, const struct entity_state_s* src);
	void(*HUD_TxferPredictionData) (struct entity_state_s* ps, const struct entity_state_s* pps, struct clientdata_s* pcd, const struct clientdata_s* ppcd, struct weapon_data_s* wd, const struct weapon_data_s* pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char* buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float* mins, float* maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char* pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s** ppTempEntFree, struct tempent_s** ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s* pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s* pTemp, float damp));
	struct cl_entity_s* (*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int* x, int* y);
	void(*CLIENTFACTORY) (void);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
} cl_clientfunc_t;

typedef struct UserMsg
{
	int	number;
	int	size;
	char name[16];
	struct UserMsg* next;
	pfnUserMsgHook pfn;
} *PUserMsg;
namespace Engine
{
	extern cl_clientfunc_t* g_pClient;
	extern cl_clientfunc_t g_Client;
	extern cl_enginefunc_t* g_pEngine;
	extern cl_enginefunc_t g_Engine;
	extern engine_studio_api_t* g_pStudio;
	extern engine_studio_api_t g_Studio;
	extern playermove_t* g_pPlayerMove;
	extern PUserMsg g_pUserMsgBase;
	extern SCREENINFO g_Screen;
	extern screenfade_t* g_pScreenFade;
	extern enginefuncs_t* g_pEngfuncs;
	extern enginefuncs_t  g_Engfuncs;
}

struct PlayerHealthStruct
{
	bool Activate;				//是否在瞄准中
	bool TeamRelationship;		//团队关系
	int  PlayerID;				//玩家ID
	int  Health;				//健康状态
	PlayerHealthStruct()
	{
		RtlZeroMemory(this,sizeof(PlayerHealthStruct));
	}
	PlayerHealthStruct(bool Activate, bool TeamRelationship, int PlayerID, int Health)
	{
		this->Activate         = Activate;
		this->TeamRelationship = TeamRelationship;
		this->PlayerID         = PlayerID;
		this->Health           = Health;
	}

};



struct BasicsStruct
{
	cvar7_s* sk_basics_enable;
	cvar7_s* sk_basics_reconnect;		                //返回房间
	cvar7_s* sk_basics_maskgamemenu;		            //屏蔽游戏菜单
	cvar7_s* sk_basics_rpn;				                //开局复活
	cvar7_s* sk_basics_noblackfog;		                //去除黑雾
	cvar7_s* sk_basics_cardenable;		                //人物卡空
	cvar7_s* sk_basics_retroom;			                //返回房间
	cvar7_s* sk_basics_thirdperson;		                //第三人称 FLAG 0 = 第三视角 else 自由视角  1 = 开关位
	cvar7_s* sk_basics_viewdistance;		            //视角距离 
	cvar7_s* sk_basics_viewhighly;		                //视角高度
	cvar7_s* sk_basics_idealyaw;			            //偏航/角度

	cvar7_s* sk_basics_healthmode;		                //显血模式	0 = 左上角方框显血  1 = 瞄准文字显血   2 = 游戏头顶显血
	cvar7_s* sk_basics_nightvision;		                //人物夜视
	cvar7_s* sk_basics_ballistic;		                //暴走隐身
	cvar7_s* sk_basics_bighead;			                //大头模式
	cvar7_s* sk_basics_bigheadvalue;		            //大头倍数
	cvar7_s* sk_basics_play_card;		                //人物卡空按键控制
	cvar7_s* sk_basics_superop;			                //超级优化
	cvar7_s* sk_basics_pictureop;		                //画面优化


	cvar7_s* sk_basics_maplighten;		                //照亮地图

	cvar7_s* sk_basics_itemknifeid;		                //刀子ID
	cvar7_s* sk_basics_maskwords;		                //屏蔽私聊
	cvar7_s* sk_basics_masklottery;		                //屏蔽中奖
	cvar7_s* sk_basics_masknotice;		                //屏蔽公告
	cvar7_s* sk_basics_maskallmsg;		                //屏蔽所有消息
	cvar7_s* sk_basics_fly;				                //起飞
	cvar7_s* sk_basics_fly2;						    //起飞2

	cvar7_s* sk_basics_drone;			                //无限无人机
	cvar7_s* sk_basics_league;			                //联赛修改
	cvar7_s* sk_basics_damageskin;		                //伤害字体
	cvar7_s* sk_basics_respawneffect;	                //复活特效
	cvar7_s* sk_basics_console_option;	                //设置控制台
	cvar7_s* sk_basics_zbsunlock;			            //解除生化Z等级限制
	cvar7_s* sk_basics_quest;			                //显示荣誉进度
	cvar7_s* sk_basics_buymenu;			                //强制购买
	cvar7_s* sk_basics_vxllight;			            //缔造者关灯
	cvar7_s* sk_basics_antikick;			            //防踢
	cvar7_s* sk_basics_superretroom;			        //超级返回房间
	cvar7_s* sk_basics_displaykick;						//显示踢人信息
	cvar7_s* sk_basics_roomkick;						//防房主踢人
	cvar7_s* sk_basics_inventory_role;					//仓库改人物
	cvar7_s* sk_basics_inventory_spray;					//仓库改喷漆
	cvar7_s* sk_basics_inventory_weapon;				//仓库改武器
	cvar7_s* sk_basics_badword;							//解除脏话
	cvar7_s* sk_basics_deathbox;						//被击杀弹窗
	cvar7_s* sk_basics_deathico;						//经典击杀图标
	cvar7_s* sk_basics_soccer;							//屏蔽足球标志
	cvar7_s* sk_basics_fps;								//设置FPS
	cvar7_s* sk_basics_activate_window;					//判断是否激活窗口
	cvar7_s* sk_basics_item8530;						//神鬼开天
	cvar7_s* sk_basics_item8133;						//圣剑
	cvar7_s* sk_basics_discipline;						//战场纪律
	cvar7_s* sk_basics_obstp;							//观战瞬移


	PlayerHealthStruct	HeadlthArray[33];
	int m_CurActivateHeadlthPlayer;				//当前激活的目标
};

struct MenuStruct
{
	cvar7_s* sk_menu_language;						    //菜单语言
	cvar7_s* sk_menu_pos_x, * sk_menu_pos_y;			//菜单坐标
	cvar7_s* sk_menu_boxheight, * sk_menu_boxwidth;		//菜单线条
	cvar7_s* sk_menu_key;							    //菜单热键


	float BoxColor[4], BoxFillColor[4];
	float BoxSelectColor[4], TextColor[4];
};

struct ESPStruct
{
	cvar7_s* sk_esp_enable;
	cvar7_s* sk_esp_validcheck;				                 //死亡检查
	cvar7_s* sk_esp_playerbox;				                 //玩家方框
	cvar7_s* sk_esp_playerbox_team;			                 //玩家方框阵营
	cvar7_s* sk_esp_playerweapon;			                 //玩家武器			flag  1 - 翻译的名字 2 - 武器模型
	cvar7_s* sk_esp_playerweapon_team;		                 //显示武器阵营
	cvar7_s* sk_esp_playername;				                 //玩家显名
	cvar7_s* sk_esp_playername_team;		                 //玩家显名阵营
	cvar7_s* sk_esp_playerdistance;			                 //玩家距离
	cvar7_s* sk_esp_playerdistance_team;	                 //玩家距离阵营
	cvar7_s* sk_esp_playershine;			                 //人物发光
	cvar7_s* sk_esp_playershine_team;		                 //人物发光阵营
	cvar7_s* sk_esp_playerTraceMove;			             //玩家移动轨迹
	cvar7_s* sk_esp_playerTrackMove_team;		             //玩家移动轨迹阵营
	cvar7_s* sk_esp_playerTraceAngleLine;			         //玩家追踪视角线
	cvar7_s* sk_esp_playerTraceAngleLine_team;		         //玩家追踪视角线阵营
	cvar7_s* sk_esp_playerminiradar;		                 //迷你雷达
	cvar7_s* sk_esp_playerminiradar_team;	                 //迷你雷达阵营
	cvar7_s* sk_esp_playerminiradar_size;	                 //雷达方框大小
	cvar7_s* sk_esp_playerhintwarning;			             //敌人预警
	cvar7_s* sk_esp_playerhintwarning_team;		             //敌人预警阵营
	cvar7_s* sk_esp_playerhintwarning_distance;	             //预警距离
	cvar7_s* sk_esp_drawitems;					             //显示物品
	cvar7_s* sk_esp_grenades_parabola;			             //预测手雷抛物线
	cvar7_s* sk_esp_c4timer;					             //显示C4爆炸时间
	cvar7_s* sk_esp_prophunt;					             //躲猫猫_透视  热能透视 + 名称 + 距离 
	cvar7_s* sk_esp_patrol_info;					         //无人机
	cvar7_s* sk_esp_zombie_trap;					         //显示僵尸陷阱
	cvar7_s* sk_esp_supply;									 //显示补给箱
	cvar7_s* sk_esp_souds;									 //声音雷达
	cvar7_s* sk_esp_souds_team;								 //声音雷达 阵营
	cvar7_s* sk_esp_allitem;								 //显示全部物品

	float m_C4Coord[3];							             //C4坐标
	int   m_C4BombDropTick;

};
//添加物品的类型
enum ADD_ENTITY_TYPE
{
	WEAPON	=	0,			//武器
	ZOMBITRAP    ,			//僵尸陷阱 zombitrap
	SUPPLY       ,			//补给箱 supply
	VXL,					//捉迷藏

	
};


struct EntityStruct
{
	int Index;			            //索引
	std::string Name;	            //模型名
	Vector Origin;		            //坐标
	ADD_ENTITY_TYPE Type;			//类型
};

// 玩家当前携带武器信息
typedef struct
{
	ULONG unknown_1;
	ULONG PrimaryWeapon;		//主武器
	ULONG unknown_2[4];
	ULONG DeputyWeapon;			//副武器
	ULONG unknown_3[4];
	ULONG KnifeData;			//近身武器
	ULONG unknown_4[4];
	ULONG GRENADES;				//手雷
}GAME_PLAYER_WEAPON_INFO, * PGAME_PLAYER_WEAPON_INFO;

enum Patrol_State
{
	State_Destroy,				                        //已被破坏
	State_Standby,				                        //待机中
	State_Patrol,				                        //巡逻中
	State_Bomb,					                        //轰炸中

};
typedef struct
{
	int CurrentCore;	              //当前核心                                     
	Patrol_State  State;		      //状态

	int Duration;				      //持续时间
}GAME_PATROL_MSGINFO, * PGAME_PATROL_MSGINFO;


typedef struct
{
	GAME_PATROL_MSGINFO Core[4];	//核心
}GAME_PATROL_MSG, * PGAME_PATROL_MSG;


struct AimbotStruct
{
	cvar7_s* sk_aimbot_enable;				            //自瞄开关
	cvar7_s* sk_aimbot_position;			            //自瞄位置	1头部 2 颈部 3 胸部 4 腹部 5 全部 6 重要的
	
	cvar7_s* sk_aimbot_camp;				            //自瞄阵营	
	cvar7_s* sk_aimbot_key;					            //自瞄键
	cvar7_s* sk_aimbot_angle;				            //自瞄角度
	cvar7_s* sk_aimbot_barrier;				            //障碍物
	cvar7_s* sk_aimbot_aimtype;				            //瞄准类型	1 = 视野  2 = 距离 3 Cycle
	cvar7_s* sk_aimbot_aimmode;				            //自瞄方式
	cvar7_s* sk_aimbot_NoRecoil;				        //反后坐力
	cvar7_s* sk_aimbot_NoRecoil_Visual;				    //视觉反后坐力
	
	cvar7_s* sk_aimbot_showplayer;			            //显示瞄准的人
	cvar7_s* sk_aimbot_showposition;		            //显示瞄准点
	cvar7_s* sk_aimbot_showrecoil;			            //显示武器弹道

	cvar7_s* sk_aimbot_zb_notaim;			            //僵尸模式不自瞄
	cvar7_s* sk_aimbot_no_grenade;			            //手雷不自瞄

	cvar7_s* sk_aimbot_shoot;				            //自动射击
	cvar7_s* sk_aimbot_shoot_fov;			            //自动射击视角
	//cvar7_s* sk_aimbot_god_check;			            //无敌状态不自瞄

	cvar7_s* sk_aimbot_MouseMoveSpeed;			        //鼠标移动速度 Value = 0 锁定  flag = 1 螺旋移动  0 = 平移 
	cvar7_s* sk_aimbot_HitBoxMode;						//击中判定体积模式 0 = 自动判断 1 = 手动设置     flag =  0 = 命中框 1 - bone 2 - origin
	cvar7_s* sk_aimbot_RecoilCompensationPitch;		    //反冲补偿Pitch
	cvar7_s* sk_aimbot_RecoilCompensationYaw;		    //反冲补偿Yaw
	cvar7_s* sk_aimbot_RecoilCompensationMode;		    //反冲补偿模式 0 = 全局模式 1 = 枪支模式
	cvar7_s* sk_aimbot_RecoilCompensationMode_DEBUG;    //反冲补偿调试模式

	cvar7_s* sk_aimbot_WeaponCanAttack;					//判断武器是否可攻击.手雷和近身武器也会判断

	cvar7_s* sk_aimbot_MatrixAdjust;					//矩阵调整模式 0 = 关闭 1 = 全局 2 = 模型

	cvar7_s* sk_aimbot_MatrixAdjust_X;
	cvar7_s* sk_aimbot_MatrixAdjust_Y;
	cvar7_s* sk_aimbot_MatrixAdjust_Z;

};
enum Game_Mode
{
	//竞技模式
	CSO_GameMode_Original,
	//个人决战模式
	CSO_GameMode_DeathMatch,
	//团队决战模式
	CSO_GameMode_TeamDeathMatch,
	//竞技模式 (虚拟)
	CSO_GameMode_Bot,
	//,个人决战 (虚拟)
	CSO_GameMode_BotDM,
	//团队决战 (虚拟
	CSO_GameMode_BotTDM,
	//国际大赛模式
	CSO_GameMode_Official,
	//国际大赛模式加时赛
	CSO_GameMode_Official_TieBreak,
	//生化模式感染
	CSO_GameMode_Zombi,
	//生化模式异变
	CSO_GameMode_Zombi_Expand,
	//生化盟战-已删除
	CSO_GameMode_Zombi_Team,
	CSO_GameMode_NULL,
	//极限模式
	CSO_GameMode_Challenge,
	//生化盟战(歼灭)-已删除
	CSO_GameMode_Zombi_Team_Ann,
	//生化模式英雄
	CSO_GameMode_Zombi_3,
	//大灾变
	CSO_GameMode_Zombi_Survival,
	//足球模式
	CSO_GameMode_Soccer,
	//机甲风暴
	CSO_GameMode_Human_Scenario,
	// 合金战争模式-已删除
	CSO_GameMode_PVPVE,
	//道具战
	CSO_GameMode_TDM_Item,
	//生化大逃杀
	CSO_GameMode_Zombi_Escape,
	// 火箭炮模式-已删除
	CSO_GameMode_Bazooka,
	//枪王决战模式
	CSO_GameMode_GunDeath,
	//休闲竞技模式
	CSO_GameMode_LightOri,
	//生化危机
	CSO_GameMode_BotZombie,
	//据点突破模式-已删除
	CSO_GameMode_BattleRush,
	//昼夜求生模式-已删除
	CSO_GameMode_ZombieShelter,
	//怪物变身模式
	CSO_GameMode_Snowman,
	//昼夜求生II
	CSO_GameMode_ZombieShelterT,
	//生化4
	CSO_GameMode_Zombi_4,
	//机甲斗兽模式
	CSO_GameMode_ZombieGiant,
	//对抗模式-已删除
	CSO_GameMode_FightYard,
	//生化歼灭战
	CSO_GameMode_Zombi_Exterminate,
	//绝命档案
	CSO_GameMode_PVE,
	//绝命乱斗-已删除
	CSO_GameMode_SuddenDeath,
	//生化歼灭战大赛模式
	CSO_GameMode_ZombieOfficial,
	//生化歼灭战加时赛
	CSO_GameMode_ZombieOfficial_TieBreak,
	//真假尸王
	CSO_GameMode_ZombieTag,
	//缔造者-创造模式
	CSO_GameMode_VoxelCreate,
	//缔造者-游戏模式
	CSO_GameMode_VoxelPVE,
	//征战全明星
	CSO_GameMode_Allstar,
	//武装展示厅
	CSO_GameMode_PlayRoom,
	//排位赛：竞技模式
	CSO_GameMode_SeasonOriginal,
	//排位赛：生化歼灭战
	CSO_GameMode_SeasonZombieEx,
	//排位赛：生化模式(英雄)
	CSO_GameMode_SeasonZombi_3,
	//生化模式Z(进化)
	CSO_GameMode_Zombi_3z,
	//生化达阵模式
	CSO_GameMode_ZombieTouchDown,
	//排位赛：生化达阵模式
	CSO_GameMode_SeasonZombieTouchDown,
	//武器展示厅Ⅱ
	CSO_GameMode_PlayRoom2,
	//捉迷藏模式
	CSO_GameMode_VoxelPropHunt,
	//雇佣兵模式
	CSO_GameMode_partnermode,
	//生化猎场
	CSO_GameMode_ZHE,
	//昼夜求生Ⅱ
	CSO_GameMode_Vxlzshelter,
	CSO_GameMode_NULL2,
	//生化5-生化S
	CSO_GameMode_Zombi_5,
	CSO_GameMode_PlayRoom3,								//展示厅 CSO_GameMode_PlayRoom3
	CSO_GameMode_Lab,


};
struct WallHackStruct
{
	cvar7_s* sk_wall_enable;			                    //视觉功能开关
	cvar7_s* sk_wall_playerlighten;		                    //人物加亮
	cvar7_s* sk_wall_wallhacktype;			                //透视类型 1人物纯透视 2 透视人物+枪支 3半透视
	cvar7_s* sk_wall_halfwallhackalpha;	                    //透明度
	cvar7_s* sk_wall_wallhackteam;			                //阵营
	cvar7_s* sk_wall_showghost;			                    //显示鬼魂
	//cvar7_s* sk_wall_maplighten;		                    //地图加亮
	//cvar7_s* sk_wall_lambert;				                //子弹发亮
	cvar7_s* sk_wall_sightbead;			                    //准星类型  :1十字准星 2 红点准星 
	cvar7_s* sk_wall_nosmoke;				                // 防烟雾弹
	cvar7_s* sk_wall_noflash;				                // 防闪光弹
	cvar7_s* sk_wall_bleeding;				                // 出血模式	0:红色血 1 = 彩色血
	cvar7_s* sk_wall_outline;				                //描边透视
	cvar7_s* sk_wall_outline_team;			                //描边透视阵营
	cvar7_s* sk_wall_outline_ct_r;			                //CT阵营 RGB
	cvar7_s* sk_wall_outline_ct_g;			                //CT阵营 RGB
	cvar7_s* sk_wall_outline_ct_b;			                //CT阵营 RGB
	cvar7_s* sk_wall_outline_t_r;			                //T阵营 RGB
	cvar7_s* sk_wall_outline_t_g;			                //T阵营 RGB
	cvar7_s* sk_wall_outline_t_b;			                //T阵营 RGB
};
struct KreedzStruct
{
	cvar7_s* sk_kreedz_enable;					             //KZ功能开关
	cvar7_s* sk_kreedz_mode;					             //KZ功能模式
	cvar7_s* sk_kreedz_bunnyhop;				             //超级跳
	cvar7_s* sk_kreedz_bunnyhop_autoduck;		             //超级跳自动蹲
	cvar7_s* sk_kreedz_autostrafe;				             //自动同步
	cvar7_s* sk_kreedz_autostrafe_strafedi;		             //同步方向
	cvar7_s* sk_kreedz_autostrafe_autodir;		             //自动同步方向
	cvar7_s* sk_kreedz_strafe_sidemove;
	cvar7_s* sk_kreedz_strafe_angle;
	cvar7_s* sk_kreedz_strafe_speed;
	cvar7_s* sk_kreedz_strafehelper;			             //同步协助
	cvar7_s* sk_kreedz_groundstrafe;			             //MCJ
	cvar7_s* sk_kreedz_groundstrafe_mscj;		            //MSCJ
	cvar7_s* sk_kreedz_groundstrafe_noslowdown;	             //不减速
	cvar7_s* sk_kreedz_groundstrafea_speed;		             //限速
	cvar7_s* sk_kreedz_groundspeed;				             //地速278
	cvar7_s* sk_kreedz_jumpbug;					             //不掉血
	cvar7_s* sk_kreedz_spinhack;				             //人物摇头
	cvar7_s* sk_kreedz_spinhackSpeed;				         //人物摇头速度
	cvar7_s* sk_kreedz_info;								 //显示KZ信息

};
typedef void(*xcommand_t) (void);
typedef struct cmd_s
{
	struct cmd_s* next;
	char* name;
	xcommand_t function;
	int	flags;
} cmd_t, * pcmd_t;

struct ScriptStruct
{
	cvar7_s* sk_script_enable;			                    //脚本功能开关
	cvar7_s* sk_script_zbs_autogank_ct;			            //僵尸自动抓人
	cvar7_s* sk_script_zbs_counter;							//自动反击
	cvar7_s* sk_script_zb5_auto_skill;						//生化5自动使用技能
	cvar7_s* sk_script_god;									//无敌
	cvar7_s* sk_script_excalibur;							//圣剑无限格挡
	ULONG m_buy_excalibur_ex;		                        //购买小圣剑的状态
	

};
enum SE_SCRIPT
{
	SE_RAIER          = 1 << 0,		//切换圣剑
	SE_RAIEREX        = 1 << 1,		//圣剑自动格挡
	SE_HM_SKILL01     = 1 << 2,		//技能 5 -生化急速奔跑
	SE_HM_SKILL02     = 1 << 3,		//技能 6 -生化致命打击
	SE_HM_SKILL03     = 1 << 4,		//技能 7
	SE_HM_SKILL04     = 1 << 5,		//技能 8
	SE_ZB5_USE_SKILL  = 1 << 6,		//生化5 自动按V
	SE_SMAILLRAIER	  = 1 << 7,		//切换小圣剑

};


struct ExternalMenuStruct
{
	cvar7_s* sk_mfcMenu_hotkey;			                    //menu hot key
	cvar7_s* sk_mfcMenu_enable_tips;			            //menu tips
};

struct MPStruct
{
	cvar7_s* sk_mp_enable;			                    //mp
	cvar7_s* sk_mp_weapon;			                    //刷枪
	cvar7_s* sk_mp_mode;			                    //开启本地模式
	cvar7_s* sk_mp_gamemode;			                //锁定游戏模式
	cvar7_s* sk_mp_gamemap;							  //锁定游戏地图
	cvar7_s* sk_mp_GainMut;							  //进化点数不足
	cvar7_s* sk_mp_health;							  //锁血
	cvar7_s* sk_mp_god;								  //无敌
	cvar7_s* sk_mp_speed;							//地速
	cvar7_s* sk_mp_ammo_max;						//无限弹药
	cvar7_s* sk_mp_ammo_speed;						//子弹加速
	cvar7_s* sk_mp_weapon_recoil;					//反后坐力
	cvar7_s* sk_mp_highjump;						//人物高跳
	cvar7_s* sk_mp_soul;							//灵魂杀人
	cvar7_s* sk_mp_sop;								//全图吸人
	cvar7_s* sk_mp_rpn;								//快速复活
};

struct VXLStruct
{
	cvar7_s* sk_vxl_enable;		            //缔造者
	cvar7_s* sk_vxl_destroy;				//炸图
	cvar7_s* sk_vxl_sky;		            //换天空
	cvar7_s* sk_vxl_weather;		        //天气
	cvar7_s* sk_vxl_light;		            //关灯
	cvar7_s* sk_vxl_restart;	            //地图重启
	
};

struct SpeedStruct
{
	cvar7_s* sk_speed_enable;					            //加速开关
	cvar7_s* sk_speed_mode;				                    //加速模式
	cvar7_s* sk_speed_key;				                    //加速键
	cvar7_s* sk_speed_value;			                    //加速数值
	cvar7_s* sk_speed_reconnect;		                    //加速是否重连
	cvar7_s* sk_speed_reconnect_timer;	                    //加速重连时间
	BOOL     IsSpeed;
};

struct SurvivalStruct
{
	cvar7_s* sk_cata_enable;				            //灾变开关
	cvar7_s* sk_cata_armor;					            //防弹衣
	cvar7_s* sk_cata_attack;				            //攻击力
	cvar7_s* sk_cata_hp;					            //买血
	cvar7_s* sk_cata_grenades;				            //手雷
	cvar7_s* sk_cata_bullet;				            //主子弹
	cvar7_s* sk_cata_bullet2;				            //副子弹

	cvar7_s* sk_cata_autoarmor;							//自动买防弹衣
	cvar7_s* sk_cata_autobullet;			            //自动买子弹
	cvar7_s* sk_cata_atck_up;				            //自动攻击升级
	//cvar7_s* sk_cata_drug;					            //自动使用回血药品
	cvar7_s* sk_cata_drug_30;				            //30%的药品
	cvar7_s* sk_cata_drug_100;				            //100%的药品
	cvar7_s* sk_cata_afk;								//挂机防踢
	cvar7_s* sk_cata_role;								//自动选人物
	cvar7_s* sk_cata_start_game;						//开始游戏

	BOOL    m_IsbuyZone;						            //是否在购买区
	int     m_CurArmor;						                //当前护甲
	int     m_MaxArmor;						                //最大护甲
	int     m_CurHealth;						            //当前HP
	int     m_MaxHealth;						            //最大HP
	int	    m_CurMoney;						                //当前金钱
	int     m_GameZBSClearCount;				            //大灾变结算次数
	ULONG	m_CurCataStartTimer;			                //大灾变开始时间
	ULONG	m_CurCataRound;					                //大灾变回合数
	int     m_CurAtUpLv;							        //当前攻击力等级
	Vector  m_CataLackOrigin;					            //锁定的坐标

	typedef struct
	{
		ULONG ItemId;		                            //物品ID

		ULONG Count;		                            //物品数量

		ULONG UseCd;		                            //CD时长

		ULONG TickCount;

	}GAME_ZBS_ITEM, * PGAME_ZBS_ITEM;
	GAME_ZBS_ITEM	m_ZBS_HP30;					        //药品恢复30%
	GAME_ZBS_ITEM	m_ZBS_HP100;				        //药品恢复100%
};

struct ZshtStruct
{
	cvar7_s* sk_zsht_enable;					            //昼夜功能开关
	cvar7_s* sk_zsht_transfer;					            //材料转移到仓库
	cvar7_s* sk_zsht_contribute;					        //刷贡献
	cvar7_s* sk_zsht_resource;								//获取资源
	cvar7_s* sk_zsht_loopbuild;								//循环建造
	cvar7_s* sk_zsht_loopbuild_x;							//循环建造坐标差值x
	cvar7_s* sk_zsht_loopbuild_y;							//循环建造坐标差值y
	cvar7_s* sk_zsht_loopbuild_z;							//循环建造坐标差值z
};


enum Packet_Type
{
	Packet_Version              = 0,		                                                 //版本号
	Packet_Reply                = 1,		                                                 //答复
	Packet_Transfer             = 2,
	Packet_ServerList           = 5,
	Packet_Character            = 6,		                                                 //创建人物名称
	Packet_Crypt                = 7,
	Packet_MileageBingo         = 15,
	Packet_Statistico           = 16,										                 //统计
	Packet_SessionID            = 17,		                                                 //会话ID
	Packet_Mobile               = 18,		                                                 //移动电话验证
	Packet_Policy               = 19,
	Packet_UpROOM               = 65,		                                                 //游戏房间内的操作->创建房间->开始游戏等
	Packet_ClientCheck          = 66,		                                                 //客户端检查
	Packet_UMsg                 = 67,		                                                 //服务器消息
	Packet_Host                 = 68,
	Packet_UpdateInfo           = 69,		                                                 //更新信息
	Packet_UDP                  = 70,
	Packet_Clan                 = 71,		                                                 //军团信息
	Packet_Shop                 = 72,		                                                 //商店
	Packet_Rank                 = 73,				                                         //2020年5月4日 19:05:47  Rank
	Packet_Ban                  = 74,
	Packet_Option               = 76,		                                                 //用户配置(控制台指令之类的)  来三次 0 1 2 
	Packet_Favorite             = 77,
	Packet_Item                 = 78,
	Packet_SearchRoom           = 80,
	Packet_HostServer           = 81,
	Packet_Report               = 83,
	Packet_Title                = 84,                                                        //荣誉完成后显示的.
	Packet_Buff	                = 85,
	Packet_QuickStart           = 86,
	Packet_UserSurvey           = 87,		                                                 //用户调查
	Packet_Quest                = 88,
	Packet_MiniGame             = 89,
	Packet_Hack                 = 90,
	Packet_Metadata             = 91,		                                                 //元数据 地图列表 游戏模式之类的
	Packet_PlayerInfo           = 93,		                                                 //在线列表->查看玩家信息
	Packet_Gift_Item            = 96,	                                                     //礼物
	Packet_2nd_Password         = 97,	                                                     //二级密码
	Packet_GameMatch            = 99,		                                                 //游戏比赛
	Packet_UpdateWarehouse      = 100,		                                                 //刷新仓库信息
	Packet_CleanSystem          = 101,		                                                 //
	Packet_Ribbon               = 102,		                                                 //成就记录
	Packet_VoxelOutUI           = 103,
	Packet_WeaponAuctionEvent   = 104,		                                                 //武器拍卖活动
	Packet_Analysis             = 105,
	Packet_CoDisassemble		= 107,
	Packet_MileageShop          = 108,
	Packet_Help                 = 109,
	Packet_PopularInfo          = 110,		                                                 //欢迎信息
	Packet_Kick                 = 112,
	Packet_HonorShop	        = 113,	                                                     //纪念商店
	Packet_EpicPieceShop        = 114,					                                     //史诗商店.一般是新年活动兑换年神
	Packet_Addon                = 115,		                                                 //插件
	Packet_QuestBadgeShop       = 116,		                                                 //问卷调查
	Packet_ReverseAuctionSystem = 117,		                                                 //反向拍卖系统？
	Packet_SeasonSystem         = 118,		                                                 //赛季系统
	Packet_GuideQuest           = 120,		                                                 //新手指引
	Packet_SantaAnim            = 122,		                                                 //圣诞老人
	Packet_UserStart            = 150,		                                                 //用户开始
	Packet_GameMatchRoomList    = 151,	                                                     //房间列表
	Packet_Inventory            = 152,		                                                 //仓库单
	Packet_Lobby                = 153,		                                                 //大厅 上线下线通知_可获取其他用户帐号和名称
	Packet_Inventory2           = 154,		                                                 //存货？
	Packet_CafeItems            = 155,
	Packet_DefaultItems         = 156,
	Packet_UserUpdateInfo       = 157,		                                                 //更新用户信息
	Packet_FabItems			    = 158,		                                                 //收藏夹.喜爱的
	Packet_Event                = 159,
	Packet_CostumeInven		    = 160,		                                                 //服装.可能是装饰
	Packet_ZombieScenarioMaps   = 161,	                                                     //僵尸场景地图
	Packet_RotationWeaponInven  = 162,
	Packet_Alarm                = 164,				                                         //公告
	Packet_MonthlyWeapon        = 165,			                                             //免费月武器_好像是吧
	Packet_LifeWeaponInven      = 166,
	Packet_VipSystem            = 167,                                                       //VIP系统
	Packet_FreePassWeaponInven  = 168,                                                       //免费武器
	Packet_ServerLog	        = 169,	                                                     //服务器日志
	Packet_Dictionary		    = 172,                                                       // 字典	-战场百科
	Packet_TestPacket           = 173,
	Packet_ZBSGoldenZB          = 174,
	Packet_Friend               = 175,                                                       //朋友
	Packet_EventItemInve        = 176,
	Packet_Expedition           = 177,                                                       //远征
	Packet_ScenarioTX           = 178,
	Packet_League               = 180,			                                             //联赛
	Packet_ClassInven           = 181,			                                             //类零件
	Packet_PartsInven           = 182,			                                             //部分件
	Packet_ItemInven            = 183,			                                             //物品零件》？
	Packet_SwitchConfig		    = 184,			                                     

};
typedef struct GAME_PACKET_TYPE_INFO
{
	Packet_Type		Type;			                    //封包类型
	ULONG			Addres;			                    //跳转的地址
	ULONG			SvAddres;		                    //保存的地址
	BOOL			Hook;			                    //是否Hook
	TAG_PEB_STACK	Stack;			                    //保存的栈信息
	std::string		name;

	inline GAME_PACKET_TYPE_INFO(Packet_Type Type, ULONG Addres, BOOL Hook, std::string name)
	{
		this->Type     = Type;
		this->Addres   = Addres;
		this->SvAddres = 0;
		this->Hook     = Hook;
		this->Stack    = TAG_PEB_STACK();
		this->name     = name;
	}
	inline GAME_PACKET_TYPE_INFO(Packet_Type Type, ULONG Addres, ULONG SvAddres,BOOL Hook, TAG_PEB_STACK Stack, std::string name)
	{
		this->Type     = Type;
		this->Addres   = Addres;
		this->SvAddres = SvAddres;
		this->Hook     = Hook;
		this->Stack    = Stack;
		this->name     = name;
	}

	inline GAME_PACKET_TYPE_INFO()
	{
		RtlZeroMemory(this,sizeof(GAME_PACKET_TYPE_INFO));
	}

}TAG_GAME_PACKET_TYPE_INFO, * PTAG_GAME_PACKET_TYPE_INFO;

typedef struct GAME_PACKET_TYPE_INFO_HOOK
{
	ULONG Class;	                                    //必须保存这个
	ULONG v1;
	ULONG v2;
	ULONG v3;											//mov edx, dword ptr ds:[eax+0x8]
	ULONG v4;			
	ULONG v5;			//this
	ULONG Arry[1024];
	GAME_PACKET_TYPE_INFO_HOOK(ULONG Addres)
	{
		RtlZeroMemory(this,sizeof(GAME_PACKET_TYPE_INFO_HOOK));
		v3 = Addres;
	}
}TAG_GAME_PACKET_TYPE_INFO_HOOK, * PTAG_GAME_PACKET_TYPE_INFO_HOOK;



typedef struct GAME_USER_INFO
{
	std::string m_UserAccount;			//游戏用户账号
	std::string m_UserName;				//游戏用户名称
	ULONG       m_KickCount;			//被举报次数

	inline GAME_USER_INFO()
	{
		RtlZeroMemory(this, sizeof(GAME_USER_INFO));
	}
}TAG_GAME_USER_INFO, * PTAG_GAME_USER_INFO;

typedef struct
{
	char	unknown_Variable[0xA8];		                //							E8
	int		UN[6];
	int		InGameID;					                //							AC

	int		unknown_0[0x10];
	float	flNextPrimaryAttack;		                //下次主攻击间隔			EC
	float	flNextSecondaryAttack;		                //下次二次攻击间隔			F0
	float	flTimeWeaponIdle;			                //不明所以 timer :20		F4
	int		unknown_1;					                //							F8
	int		iPrimaryAmmoType;			                //弹药类型					FC
	int		iSecondaryAmmoType;			                //二次弹药类型 -1			100
	int		iClip;						                //弹药数量					104	下断找偏移即可 -2021年7月7日 20:06:46 -偏移 10C
	//  [4/16/2022 007]
	//  10C -> 11C
	int		iClientClip;				                //客户端弹药数量			EC
	int		ClientWeaponState;
	int		unknown_2;
	int		unknown_3;
	int		fInReload;					                //+ 0x14
	int		unknown_Variable2[0xB];
	int		fInRecoil;					                //+ 0x40
	int		unknown_Variable3[0x6];
	int		iSpecialClip;				                //特殊弹药数量					60

}TAG_PLAYER_WEAPON, * PTAG_PLAYER_WEAPON;