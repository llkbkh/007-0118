#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class CGameSurvival : public IGameClient, public IGameEngineMSG
{
public:
	CGameSurvival();

	static CGameSurvival* GetInstance();

	// ≥ı ºªØ
	virtual bool vInit();
	virtual void vHUD_Redraw(float time, int intermission);
	virtual void vHUD_Frame(double time);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);



	virtual int vMPToCL(const char* pszName, int iSize, void* pbuf);
	virtual int vMoney(const char* pszName, int iSize, void* pbuf);
	virtual int vBattery(const char* pszName, int iSize, void* pbuf);
	virtual int vStatusIcon(const char* pszName, int iSize, void* pbuf);
	virtual int vHealth(const char* pszName, int iSize, void* pbuf);
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);
	
private:
	void CalcDrugCD();

private:
	static inline CGameSurvival* m_pInstance = nullptr;
	GamePlayer*                  m_Player;
};

