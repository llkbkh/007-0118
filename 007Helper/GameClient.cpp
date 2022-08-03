#include "GameClient.h"
#include "GameOffset.h"
#include "Util.h"
#include "Algorithm.h"
#include "Drawing.h"
#include "EngineMsg.h"
#include "GameCFG.h"

IGameConsole* g_pConsole = nullptr;
GameClient::GameClient()
{
	m_Hook    = std::make_unique<CACEHook>();
	m_Process = std::make_unique<CACEProcess>();


	m_pGameBasics      = GameBasics::GetInstance();
	m_pGamePlayer      = GamePlayer::GetInstance();
	m_pGameMenu        = new GameMenu();
	m_pGameEsp         = GameEsp::GetInstance();
	m_pGameAimbot      = GameAimBot::GetInstance();
	m_pGameWall        = GameWallHack::GetInstance();
	m_pGameSpeed       = GameSpeed::GetInstance();
	m_pGameKreedz      = GameKreedz::GetInstance();
	m_pGameScript      = GameScript::GetInstance();
	m_pExtMenu         = ExternalMenu::GetInstance();
	m_pDispatchRecv    = CDispatchRecv::GetInstance();
	m_pGameVXL         = CGameVXL::GetInstance();
	m_pGamePack        = CGamePack::GetInstance();
	m_pLuaScriptManage = CLuaScriptManage::GetInstance();
	m_pSagaClient      = CSagaClient::GetInstance();
	m_pGameSurvival    = CGameSurvival::GetInstance();
	m_pGameZsht        = CGameZsht::GetInstance();
	m_pGameMp          = CGameMp::GetInstance();
	m_nVecClientFun.push_back(m_pGameBasics);
	m_nVecClientFun.push_back(m_pGamePlayer);
	m_nVecClientFun.push_back(m_pGameMenu);
	m_nVecClientFun.push_back(m_pGameEsp);
	m_nVecClientFun.push_back(m_pGameAimbot);
	m_nVecClientFun.push_back(m_pGameWall);
	m_nVecClientFun.push_back(m_pGameSpeed);
	m_nVecClientFun.push_back(m_pGameKreedz);
	m_nVecClientFun.push_back(m_pGameScript);
	m_nVecClientFun.push_back(m_pExtMenu);
	m_nVecClientFun.push_back(m_pDispatchRecv);
	m_nVecClientFun.push_back(m_pGameVXL);
	m_nVecClientFun.push_back(m_pGamePack);
	m_nVecClientFun.push_back(m_pLuaScriptManage);
	m_nVecClientFun.push_back(m_pSagaClient);
	m_nVecClientFun.push_back(m_pGameSurvival);
	m_nVecClientFun.push_back(m_pGameZsht);
	m_nVecClientFun.push_back(m_pGameMp);


}

GameClient* GameClient::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameClient();
	}
	return m_pInstance;
}

void GameClient::HookClientFunction()
{
	while (Engine::g_pClient->HUD_PostRunCmd == nullptr)
	{
		Sleep(200);
	}

	//替换函数指针
	Engine::g_pClient->Initialize                    = Initialize;
	Engine::g_pClient->HUD_PostRunCmd				 = HUD_PostRunCmd;
	Engine::g_pClient->HUD_Frame                     = HUD_Frame_Init;
	Engine::g_pClient->HUD_Redraw					 = HUD_Redraw;
	Engine::g_pClient->HUD_Key_Event                 = HUD_Key_Event;
	Engine::g_pClient->HUD_AddEntity                 = HUD_AddEntity;
	Engine::g_pClient->HUD_PlayerMove                = HUD_PlayerMove;
	Engine::g_pClient->CL_IsThirdPerson              = CL_IsThirdPerson;
	Engine::g_pClient->CL_CreateMove                 = CL_CreateMove;
	Engine::g_pClient->V_CalcRefdef                  = V_CalcRefdef;
	//Engine::g_pClient->IN_MouseEvent                 = IN_MouseEvent;
	Engine::g_pClient->HUD_GetStudioModelInterface   = HUD_GetStudioModelInterface;

	//注册 Var
	g_Vars.RegisterVariable();
}

void GameClient::HUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{
	GameClient::GetInstance()->vHUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);
}

void GameClient::HUD_Redraw(float time, int intermission)
{
	GameClient::GetInstance()->vHUD_Redraw(time, intermission);
}

void GameClient::HUD_Frame_Init(double time)
{
	GameClient::GetInstance()->vInit();
}

void GameClient::HUD_Frame(double time)
{
	GameClient::GetInstance()->vHUD_Frame(time);
}

void GameClient::IN_MouseEvent(int mstate)
{
	//  鼠标事件 0 = 移动  1 = 左键  2 = 右键
	//ACEInfoFileLog("{}", mstate);
}

int GameClient::HUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	return GameClient::GetInstance()->vHUD_Key_Event(down, keynum, pszCurrentBinding);
}

int GameClient::HUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{
	GameClient::GetInstance()->vHUD_AddEntity(type, ent, modelname);

	return Engine::g_Client.HUD_AddEntity(type, ent, modelname);
}

void GameClient::HUD_PlayerMove(struct playermove_s* ppmove, int server)
{

	GameClient::GetInstance()->vHUD_PlayerMove(ppmove, server);
}

int GameClient::CL_IsThirdPerson()
{
	// 开启第三人称 强制返回1 否则只会显示 手的模型
	if (g_Vars.m_Basi.sk_basics_enable->value && g_Vars.m_Basi.sk_basics_thirdperson->value && (g_Vars.m_Basi.sk_basics_thirdperson->flags & (1 << 1)))
	{
		return 1;
	}
	return Engine::g_Client.CL_IsThirdPerson();
}

void GameClient::CL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	GameClient::GetInstance()->vCL_CreateMove(frametime, cmd, active);
}

void GameClient::V_CalcRefdef(ref_params_s* pparams)
{
	GameClient::GetInstance()->vV_CalcRefdef(pparams);

}

void GameClient::RestoreRenderer()
{
	// Hook这个可以在有效范围内 都能获取到.其他函数必须在FOV内才可以
	Engine::g_Studio.RestoreRenderer();

	//才知道 单例模式 可以访问私有成员
	GameClient::GetInstance()->m_pGamePlayer->StudioEntityLight();

	//获取矩阵
	glGetDoublev(GL_MODELVIEW_MATRIX, GameClient::GetInstance()->m_modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, GameClient::GetInstance()->m_projview);
	glGetIntegerv(GL_VIEWPORT, GameClient::GetInstance()->m_viewport);

}

int GameClient::EnginetStudioCheckBBox()
{
	cl_entity_s* pEntity = Engine::g_pStudio->GetCurrentEntity();
	if (pEntity && pEntity->index != 0 && pEntity->index <= Engine::g_Engine.GetMaxClients())
	{
		if (GameClient::GetInstance()->m_pGamePlayer->GetPlayerValid(pEntity->index))
		{
			//return 1;
		}
	
	}
	return Engine::g_Studio.StudioCheckBBox();
}

void GameClient::StudioDrawPoints()
{
	if (g_Vars.m_Basi.sk_basics_enable->value && g_Vars.m_Basi.sk_basics_bighead->value)
	{
		//失效 .暂时不更新大头了
		//g_Basics.BigHead();
	}
	Engine::g_Studio.StudioDrawPoints();
}

void GameClient::StudioDynamicLight(struct cl_entity_s* ent, struct alight_s* plight)
{
	Engine::g_Studio.StudioDynamicLight(ent, plight);

}

int GameClient::HUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
{

	return GameClient::GetInstance()->vHUD_GetStudioModelInterface(version, ppinterface, pstudio);
}

int GameClient::Initialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	return GameClient::GetInstance()->vInitialize(pEnginefuncs, iVersion);
}

HHOOK	GameHook = NULL;
LRESULT WINAPI GameHookCallBack(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (((DWORD)lParam & 0x40000000) && (HC_ACTION == nCode)) //有键按下
	{

		GameClient::GetInstance()->vKeyBoardProc(nCode, wParam, lParam);

	}
	return ::CallNextHookEx(GameHook, nCode, wParam, lParam);
}

bool GameClient::vInit()
{
	//初始化 HookStudioFunction
	GameClient::GetInstance()->HookStudioFunction();
	//显示控制台信息
	GameClient::GetInstance()->Hud_Console007Info();
	//m_Hook->InlineHook(g_Offset.GetGameAddresInfo()->dwCallEngineSurfaceProc,0, GameHookWndProc,&_GameHookWndProc);

	GameHook = SetWindowsHookExA(WH_KEYBOARD, GameHookCallBack, NULL, m_Process->GetMainThreadId());

	g_pUtil->InitGameItemInfo();
	g_pUtil->InitinWeaponItem();

	g_GameCFG.LoadCore();

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vInit();
	}

	Engine::g_pClient->HUD_Frame = HUD_Frame;

	return true;
}

void GameClient::vHUD_Frame(double time)
{
	memset((PVOID)g_Offset.GetGameAddresInfo()->dwCheckEngineAddres, 0xFFFFFFFF, sizeof(DWORD));

	Engine::g_Engine.pfnGetScreenInfo(&Engine::g_Screen);

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_Frame(time);
	}

	Engine::g_Client.HUD_Frame(time);
}

int GameClient::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//判断当前激活窗口是否游戏游戏窗口.
	if (g_Vars.m_Basi.sk_basics_activate_window->value == 0 || (g_Vars.m_Basi.sk_basics_activate_window->value == 1 && GetActiveWindow() == g_Vars.m_GameHwnd))
	{
		std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

		for (; itr != m_nVecClientFun.end(); ++itr)
		{
			(*itr)->vKeyBoardProc(nCode, wParam, lParam);
		}
	}
	return nCode;
}

void GameClient::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	Engine::g_Client.CL_CreateMove(frametime, cmd, active);

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vCL_CreateMove(frametime, cmd, active);
	}
}

void GameClient::vV_CalcRefdef(ref_params_s* pparams)
{
	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		if ((*itr) == m_pGameBasics)
		{
			continue;
		}
		(*itr)->vV_CalcRefdef(pparams);
	}

	Engine::g_Client.V_CalcRefdef(pparams);
	//修改视角要在之后
	m_pGameBasics->vV_CalcRefdef(pparams);
}

void GameClient::vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{
	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_AddEntity(type, ent, modelname);
	}
}

void GameClient::vHUD_Redraw(float time, int intermission)
{
	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_Redraw(time, intermission);
	}

	Engine::g_Client.HUD_Redraw(time, intermission);
}

void GameClient::vHUD_PlayerMove(struct playermove_s* ppmove, int server)
{
	Engine::g_Client.HUD_PlayerMove(ppmove, server);

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_PlayerMove(ppmove, server);
	}
}

int GameClient::vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_Key_Event(down, keynum, pszCurrentBinding);
	}

	return Engine::g_Client.HUD_Key_Event(down, keynum, pszCurrentBinding);
}

void GameClient::vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{
	Engine::g_Client.HUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_PostRunCmd(from, to, cmd, runfuncs, time, random_seed);
	}
}

int GameClient::vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
{
	int nRet = Engine::g_Client.HUD_GetStudioModelInterface(version, ppinterface, pstudio);

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vHUD_GetStudioModelInterface(version, ppinterface, pstudio);
	}
	return nRet;
}

int GameClient::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	Engine::g_pEngine = (cl_enginefunc_t*)pEnginefuncs;

	RtlCopyMemory(&Engine::g_Engine, pEnginefuncs, sizeof(cl_enginefunc_t));

	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vInitialize(pEnginefuncs, iVersion);
	}
	return  Engine::g_Client.Initialize(pEnginefuncs, iVersion);;
}

int GameClient::vGameUI_WndProc(HWND hwnd, int nCode, WPARAM wParam, LPARAM lParam)
{
	std::vector<IGameClient*>::iterator itr = m_nVecClientFun.begin();

	for (; itr != m_nVecClientFun.end(); ++itr)
	{
		(*itr)->vGameUI_WndProc(hwnd, nCode, wParam, lParam);
	}
	return 0;
}

bool GameClient::HookStudioFunction()
{
	g_Offset.FindStudioTable();

	//第二种获取骨骼的方法
	Engine::g_pStudio->RestoreRenderer = RestoreRenderer;

	//Engine::g_pStudio->StudioDynamicLight = StudioDynamicLight;
	
	Engine::g_pStudio->StudioCheckBBox = EnginetStudioCheckBBox;
	//修改骨骼 大头
	Engine::g_pStudio->StudioDrawPoints = StudioDrawPoints;

	//屏幕大小
	Engine::g_Screen.iSize = sizeof(SCREENINFO);

	//初始化字体	
	g_Font.InitText(xorstr_("Lucida Console"), 12, 8);
	g_Verdana.InitText(xorstr_("Verdana"), 13, 0);

	g_Offset.HUD_Frame();

	g_UserMsg.HookEngineMsg();

	return true;
}


void GameClient::Hud_Console007Info()
{
	CreateInterfaceFn hardware_factory = CaptureFactory(xorstr_("hw.dll"));
	CreateInterfaceFn gameui_factory = CaptureFactory(xorstr_("GameUI.dll"));

	g_pConsole = (IGameConsole*)(CaptureInterface(gameui_factory, xorstr_(GAMECONSOLE_INTERFACE_VERSION)));

	if (!g_pConsole->IsConsoleVisible())
		g_pConsole->Activate();

	char DateString[9], TimeString[9];
	_strdate_s(DateString);
	_strtime_s(TimeString);




	//载入时间
	g_pConsole->DPrintf(xorstr_("\n\tLoaded :%s %s\n\t"), TimeString, DateString);

	//当前服务器
	auto u  = std::make_unique<CACEUtil>();
	auto sv = g_pUtil->GetGameServer();

	if (std::get<0>(sv) == GameServer::Chinese)
	{

		//g_pConsole->DPrintf(xorstr_("\n\tGame Server :%s %s\n\t"), u->string_To_UTF8(std::get<1>(sv)).c_str(), u->string_To_UTF8(std::get<2>(Tool->GetGameServerRegion())).c_str());
	}
	else
	{
		//g_pConsole->DPrintf(xorstr_("\n\tGame Server :%s\n\t"), u->string_To_UTF8(std::get<1>(sv)).c_str());
	}
	

	//当前版本号
	//g_pConsole->DPrintf(xorstr_("\n\tVersion :%s\n\t"), xorstr_("1223"));

	//版权信息
	//g_pConsole->DPrintf(xorstr_("\n\tBy:James Bond\n\t\n\t"));

}

void* CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	PVOID dwPointer = NULL;

	while (!dwPointer)
	{
		dwPointer = (PVOID)(Interface(InterfaceName, 0));

		Sleep(100);
	}

	return dwPointer;
}
CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	CreateInterfaceFn Interface = 0;

	while (!Interface)
	{
		HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

		if (hFactoryModule)
		{
			Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, CREATEINTERFACE_PROCNAME));
		}

		Sleep(100);
	}

	return Interface;
}

Algorithm_s Algorithm;
bool WorldToScreen(float in[3], float out[2])
{

#if 0 //三角函数算法 现在不用了
	if (g_Vars.m_Dev.sk_developer_mxtn_mode->value)
	{

		CoorDinate WorldScreent = { 0,0,1 };
		CoorDinate TagCoorDinateobj;

		TagCoorDinateobj.x = in[0];
		TagCoorDinateobj.y = in[1];
		TagCoorDinateobj.z = in[2];


		CoorDinate* CoorDinateobj = (CoorDinate*)(g_Offset.GetGameAddresInfo()->dwMyBone);


		CoorDinate* Angle = (CoorDinate*)g_Offset.GetGameAddresInfo()->dwAngletr;

		if (Algorithm.Get_Angle_deviation(Angle->y, Algorithm.Seek_X_Angle(*CoorDinateobj, TagCoorDinateobj)) < 45)
		{
			FTPOV World = Algorithm.GetVisualAngle(*CoorDinateobj, *Angle, *(float*)g_Offset.GetGameAddresInfo()->dwCameraFOV);
			CoorDinate Screent;
			Screent.x = Engine::g_Screen.iWidth;
			Screent.y = Engine::g_Screen.iHeight;
			//目标坐标
			WorldScreent = Algorithm.MapCoordinatesToScreenCoordinates(World.Position, TagCoorDinateobj, *(float*)g_Offset.GetGameAddresInfo()->dwCameraFOV, World.RevolvingBody, Screent);

			out[0] = WorldScreent.x;
			out[1] = WorldScreent.y;

			return true;
		}
		else
		{
			return false;
		}


	}
	else
	{
		double View2D[3];
		bool Project = gluProject(in[0], in[1], in[2], g_Vars.m_modelview, g_Vars.m_projview, g_Vars.m_viewport, &View2D[0], &View2D[1], &View2D[2]);
		if (Project && View2D[2] < 1)
		{
			out[0] = View2D[0];
			out[1] = g_Vars.m_viewport[3] - View2D[1];

			return true;
		}
		return false;
	}
#endif
	
	GameClient* Client = GameClient::GetInstance();


	double View2D[3];
	bool Project = gluProject(in[0], in[1], in[2], Client->m_modelview, Client->m_projview, Client->m_viewport, &View2D[0], &View2D[1], &View2D[2]);
	if (Project && View2D[2] < 1)
	{
		out[0] = View2D[0];
		out[1] = Client->m_viewport[3] - View2D[1];

		return true;
	}

	return false;
}