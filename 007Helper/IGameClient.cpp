#include "IGameClient.h"

int IGameClient::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	return 1;
}

bool IGameClient::vInit()
{
	return true;
}

void IGameClient::vHUD_Frame(double time)
{

}

int IGameClient::vGameUI_WndProc(HWND hwnd, int nCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

int IGameClient::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void IGameClient::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{

}

void IGameClient::vV_CalcRefdef(ref_params_s* pparams)
{

}

void IGameClient::vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname)
{

}

void IGameClient::vHUD_Redraw(float time, int intermission)
{

}

void IGameClient::vHUD_PlayerMove(struct playermove_s* ppmove, int server)
{

}

int IGameClient::vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding)
{
	return 0;
}

void IGameClient::vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{

}

int IGameClient::vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio)
{
	return 1;
}
