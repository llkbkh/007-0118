#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class GameWallHack :public IGameClient
{
public:
	GameWallHack();
	static GameWallHack* GetInstance();
	static FARPROC WINAPI extGetProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static void WINAPI extlBegin(unsigned int mode);
	static void WINAPI extglGetFloatv(GLenum pname, GLfloat* params);
	static void WINAPI extglGetIntegerv(GLenum pname, GLint* params);
	static void WINAPI extglViewport(GLint x, GLint y, GLsizei width, GLsizei height);
	static void WINAPI extglColor3f(GLfloat red, GLfloat green, GLfloat blue);
	static void WINAPI extglVertex3f(GLfloat x, GLfloat y, GLfloat z);
	void WINAPI glCrosshair();
	static BOOL WINAPI extglSwapBuffers(HDC hdc);

	virtual void vHUD_Redraw(float time, int intermission);
	virtual int vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	static int NewStudioDrawPlayer(int flags, struct entity_state_s* pplayer);
	virtual bool vInit();
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname);
private:
	// * ·ÀÉÁ¹âµ¯
	void NoFlash();
	// * ×¼ÐÇ
	void SightBead();
private:
	inline static GameWallHack* m_Instance = nullptr;
	HMODULE                     m_ModuleGL;
	GLsizei                     m_glWidth, m_glHeight;
	r_studio_interface_s        m_StudioInterface;
	engine_studio_api_s         m_IEngineStudio;
	GamePlayer*                 m_GamePlayer;

};

