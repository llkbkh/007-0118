#pragma once
#include "007Helper.h"

class GameSpeed :public IGameClient
{
public:
	GameSpeed();
	static GameSpeed* GetInstance();


	virtual bool vInit();
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);

	virtual void vHUD_Redraw(float time, int intermission);

	void GameMemorySpeed();

	void PlayerSpeed();
	void AdjustSpeed(double Speed);
	void TimeSpeed();

	static BOOL WINAPI extueryPerformanceCounter(LARGE_INTEGER* lp);


private:
	DWORD m_dwSpeed[10];

	std::shared_ptr<CACEMemory> m_pACEMemory;

	static inline float m_fSpeedTiemr = 1.0f;
	static inline GameSpeed* m_pInstance = nullptr;

};

