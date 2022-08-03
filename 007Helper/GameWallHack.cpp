#include "GameWallHack.h"

GameWallHack::GameWallHack()
{
	m_ModuleGL = nullptr;
	m_glHeight = 0;
	m_glWidth  = 0;
}

GameWallHack* GameWallHack::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new GameWallHack();
	}
	return m_Instance;
}


FARPROC WINAPI GameWallHack::extGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	GameWallHack* pInstance = GameWallHack::GetInstance();

	if (!pInstance->m_ModuleGL)
	{
		pInstance->m_ModuleGL = GetModuleHandleA(xorstr_("Opengl32.dll"));
	}

	if (pInstance->m_ModuleGL && hModule == pInstance->m_ModuleGL)
	{
		if (lstrcmpiA(lpProcName, xorstr_("glBegin")) == 0)
		{
			return (FARPROC)extlBegin;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("glGetFloatv")) == 0)
		{
			return (FARPROC)extglGetFloatv;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("glGetIntegerv")) == 0)
		{
			return (FARPROC)extglGetIntegerv;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("glViewport")) == 0)
		{
			return (FARPROC)extglViewport;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("glColor3f")) == 0)
		{
			return (FARPROC)extglColor3f;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("glVertex3f")) == 0)
		{
			return (FARPROC)extglVertex3f;
		}
		else if (lstrcmpiA(lpProcName, xorstr_("wglSwapBuffers")) == 0)
		{
			return (FARPROC)extglSwapBuffers;
		}

	}

	return ::GetProcAddress(hModule, lpProcName);
}

void WINAPI GameWallHack::extlBegin(unsigned int mode)
{
	if (!g_Vars.m_WallHack.sk_wall_enable || !g_Vars.m_WallHack.sk_wall_enable->value)
		return glBegin(mode);
	switch (mode)
	{
	case 4:
	case 5:
	case 6:
	{
		if (g_Vars.m_WallHack.sk_wall_playerlighten->value || g_Vars.m_WallHack.sk_wall_showghost->value)
		{
			cl_entity_s* ent = Engine::g_pStudio->GetCurrentEntity();
			if (ent != NULL && ent->index <= Engine::g_Engine.GetMaxClients())
			{
				if (mode != GL_TRIANGLES && g_Vars.m_WallHack.sk_wall_playerlighten->value)		//人物加亮
				{
					glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
					//GL_DECAL 忽略原色，只显示纹素。
				}
				
				if (mode != GL_TRIANGLES && g_Vars.m_WallHack.sk_wall_showghost->value)		//显鬼
				{
					glDisable(3042);
					glEnable(7);
					glTexEnvi(8960, 8704, 8449);
				}
			}
		}


		if (g_Vars.m_WallHack.sk_wall_wallhacktype->value == 2)				//透视
		{
			glDepthRange(0, 0.4);

		}
		break;
	}
	default:
	{
		if (g_Vars.m_WallHack.sk_wall_wallhacktype->value == 2)				//透视
		{
			glDepthRange(0.5, 1);
			
		}
		//if (g_Vars.m_WallHack.sk_wall_maplighten->value && mode != 7)		//地图高亮
		//{
		//	glTexEnvi(8960, 8704, 8449);
		//	glDisable(3553);
		//}
		if (g_Vars.m_WallHack.sk_wall_wallhacktype->value == 3 && mode == 9)	//半透视
		{
			float color[4];
			glGetFloatv(GL_CURRENT_COLOR, color);
			glEnable(GL_BLEND);
			glBlendFunc(770, 771);
			glColor4f(color[0], color[1], color[2], g_Vars.m_WallHack.sk_wall_halfwallhackalpha->value);
		}
		break;
	}
	}

	return glBegin(mode);
}

void WINAPI GameWallHack::extglGetFloatv(GLenum pname, GLfloat* params)
{
	glGetFloatv(pname, params);

	switch (pname)
	{
		case GL_DEPTH_RANGE:
		{
			params[0] = 0.0;
			params[1] = 1.0;
			break;
		}

	}
}

void WINAPI GameWallHack::extglGetIntegerv(GLenum pname, GLint* params)
{
	glGetIntegerv(pname, params);

	switch (pname)
	{
		case GL_DEPTH_FUNC:
		{
			params[0] = GL_LESS;
			break;
		}
	}
}

void WINAPI GameWallHack::extglViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	GameWallHack* pInstance = GameWallHack::GetInstance();
	pInstance->m_glHeight   = height;
	pInstance->m_glWidth    = width;

	glViewport(x, y, width, height);
}

void WINAPI GameWallHack::extglColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
	if (g_Vars.m_WallHack.sk_wall_sightbead->value == 2 && g_Vars.m_WallHack.sk_wall_enable->value)
		GameWallHack::GetInstance()->glCrosshair();
	glColor3f(red, green, blue);
}

void WINAPI GameWallHack::extglVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	//glColor3f(1.0f, 1.0f, 1.0f);
	return glVertex3f(x, y, z);
}

void WINAPI GameWallHack::glCrosshair()
{
	float params[4];

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glGetFloatv(GL_CURRENT_COLOR, params);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);

	for (int i = 10; i != 0; i--)
	{
		glVertex2f(cos(0.6283185307179586f * i) * 3.0f + (m_glWidth / 2), sin(0.6283185307179586f * i) * 3.0f + (m_glHeight / 2));
	}

	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor3f(params[0], params[1], params[2]);
}

BOOL WINAPI GameWallHack::extglSwapBuffers(HDC hdc)
{
	return ::SwapBuffers(hdc);
}

void GameWallHack::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_WallHack.sk_wall_enable->value)
		return;

	if (g_Vars.m_WallHack.sk_wall_noflash->value)
	{
		NoFlash();
	}
	if (g_Vars.m_WallHack.sk_wall_sightbead->value == 1)
	{
		SightBead();
	}
}

int GameWallHack::vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
{
	memcpy(&m_IEngineStudio, pstudio, sizeof(engine_studio_api_t));

	memcpy(&m_StudioInterface, *ppinterface, sizeof(r_studio_interface_s));

	(*ppinterface)->StudioDrawPlayer = NewStudioDrawPlayer;

	//(*ppinterface)->StudioDrawModel = NewStudioDrawModel;

	return 1;
}

int GameWallHack::NewStudioDrawPlayer(int flags, struct entity_state_s* pplayer)
{
	int	nRet = 0;

	auto pInstance = GameWallHack::GetInstance();

	if (!g_Vars.m_WallHack.sk_wall_enable->value || g_Vars.m_WallHack.sk_wall_wallhacktype->value != 1)
		return pInstance->m_StudioInterface.StudioDrawPlayer(flags, pplayer);


	cl_entity_s* ent = Engine::g_pStudio->GetCurrentEntity();
	//人物纯透视
	if (ent != NULL && ent->index <= Engine::g_Engine.GetMaxClients())
	{

		if (pInstance->m_GamePlayer->IsSetTeam(ent->index, g_Vars.m_WallHack.sk_wall_wallhackteam->value))
		{
			glDepthRange(0, 0.1);
			nRet = pInstance->m_StudioInterface.StudioDrawPlayer(flags, pplayer);
			glDepthRange(0, 1);
		}
		else
			nRet = pInstance->m_StudioInterface.StudioDrawPlayer(flags, pplayer);

		return nRet;
	}

	return pInstance->m_StudioInterface.StudioDrawPlayer(flags, pplayer);
}
void (*pfnCreatesmokeFuns)(struct event_args_s* args) = nullptr;



void createsmoke(event_args_s* args)
{
	//防烟雾弹
	if (g_Vars.m_WallHack.sk_wall_nosmoke->value && g_Vars.m_WallHack.sk_wall_enable->value)
	{
		return;
	}
	pfnCreatesmokeFuns(args);
}


void pfnHookEvent(char* name, void (*pfnEvent)(struct event_args_s* args))
{
	//ACEDebugLog("[{}] name:{}",__FUNCTION__,name);

	//events/createsmoke.sc ->smoke事件
	if (name && lstrcmpA(name,xorstr_("events/createsmoke.sc")) == 0)
	{
		pfnCreatesmokeFuns = pfnEvent;
		Engine::g_Engine.pfnHookEvent(name, createsmoke);
		return;
	}

	return Engine::g_Engine.pfnHookEvent(name, pfnEvent);
}
unsigned short	pfnPrecacheEvent(int type, const char* psz)
{
	ACEDebugLog("[{}] type:{} psz:{}", __FUNCTION__, type, psz);

	return 1;
}


bool GameWallHack::vInit()
{
	m_GamePlayer = GamePlayer::GetInstance();

	return true;
}

int GameWallHack::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	pEnginefuncs->pfnHookEvent = pfnHookEvent;
	//pEnginefuncs->pfnPrecacheEvent = pfnPrecacheEvent;

	return 1;
}

void GameWallHack::vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{
	if (!g_Vars.m_WallHack.sk_wall_enable->value || !ent)
		return ;
	if (g_Vars.m_WallHack.sk_wall_bleeding->value)
	{
		{
			if (strstr(modelname, xorstr_("blood.spr")))
			{
				ent->curstate.renderamt = 1;
				int nFrame = g_Vars.m_WallHack.sk_wall_bleeding->value == 1 ? 0 : Engine::g_Engine.pfnRandomLong(0, 7);
				switch (nFrame)
				{
				case 0:
					ent->curstate.rendercolor.r = 255;
					ent->curstate.rendercolor.g = 0;
					ent->curstate.rendercolor.b = 0;
					break;
				case 1:
					ent->curstate.rendercolor.r = 0;
					ent->curstate.rendercolor.g = 255;
					ent->curstate.rendercolor.b = 0;
					break;
				case 2:
					ent->curstate.rendercolor.r = 0;
					ent->curstate.rendercolor.g = 0;
					ent->curstate.rendercolor.b = 255;
					break;
				case 3:
					ent->curstate.rendercolor.r = 255;
					ent->curstate.rendercolor.g = 255;
					ent->curstate.rendercolor.b = 0;
					break;
				case 4:
					ent->curstate.rendercolor.r = 0;
					ent->curstate.rendercolor.g = 255;
					ent->curstate.rendercolor.b = 255;
					break;
				case 5:
					ent->curstate.rendercolor.r = 255;
					ent->curstate.rendercolor.g = 0;
					ent->curstate.rendercolor.b = 255;
					break;
				case 6:
					ent->curstate.rendercolor.r = 20;
					ent->curstate.rendercolor.g = 20;
					ent->curstate.rendercolor.b = 255;
					break;
				case 7:
					ent->curstate.rendercolor.r = 20;
					ent->curstate.rendercolor.g = 255;
					ent->curstate.rendercolor.b = 20;
					break;
				}
			}
		}
	}
	if (g_Vars.m_WallHack.sk_wall_outline->value)
	{

		if (ent->index != m_GamePlayer->GetLocal()->Player.Index && (ent->index <= Engine::g_Engine.GetMaxClients()) && m_GamePlayer->GetPlayer(ent->index)->Valid)
		{

			if (m_GamePlayer->IsSetTeam(ent->index, g_Vars.m_WallHack.sk_wall_outline_team->value))
			{
				ent->outlineType = 7;
				// 判断阵营
				TEAM nTeam = (TEAM)m_GamePlayer->GetPlayer(ent->index)->Team;

				if (nTeam == TERRORIST)
				{
					ent->outline_R = g_Vars.m_WallHack.sk_wall_outline_t_r->value;
					ent->outline_G = g_Vars.m_WallHack.sk_wall_outline_t_g->value;
					ent->outline_B = g_Vars.m_WallHack.sk_wall_outline_t_b->value;

				}
				else if (nTeam == CT)
				{
					ent->outline_R = g_Vars.m_WallHack.sk_wall_outline_ct_r->value;
					ent->outline_G = g_Vars.m_WallHack.sk_wall_outline_ct_g->value;
					ent->outline_B = g_Vars.m_WallHack.sk_wall_outline_ct_b->value;
				}
				else
				{
					//0, 255, 127
					ent->outline_R = 0;
					ent->outline_G = 255;
					ent->outline_B = 127;
				}
			}
			else
			{
				ent->outlineType = 0;
			}

		}
	}

}

void GameWallHack::NoFlash()
{
	screenfade_s fade;
	Engine::g_Engine.pfnGetScreenFade(&fade);
	fade.fadealpha = 10; //浓度 0是直接干掉闪光
	Engine::g_Engine.pfnSetScreenFade(&fade);
}

void GameWallHack::SightBead()
{
	int centerX = Engine::g_Screen.iWidth / 2;
	int centerY = Engine::g_Screen.iHeight / 2;
	Engine::g_Engine.pfnFillRGBA(centerX - 14, centerY, 9, 2, 255, 0, 0, 100);
	Engine::g_Engine.pfnFillRGBA(centerX + 6, centerY, 9, 2, 255, 0, 0, 100);
	Engine::g_Engine.pfnFillRGBA(centerX, centerY - 14, 2, 9, 255, 0, 0, 100);
	Engine::g_Engine.pfnFillRGBA(centerX, centerY + 7, 2, 9, 255, 0, 0, 100);
	Engine::g_Engine.pfnFillRGBA(centerX, centerY, 2, 2, 255, 0, 0, 100);
}
