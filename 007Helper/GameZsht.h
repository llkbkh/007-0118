#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class CGameZsht :public IGameClient
{
public:
	static CGameZsht* GetInstance();
	//¼üÅÌ¹³×Ó
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	// ³õÊ¼»¯
	virtual bool vInit();



	void ZshtBuild(ULONG ulIndex, vec3_t origin);
private:
	static inline CGameZsht* m_pInstance = nullptr;
	GamePlayer* m_Player;
};

