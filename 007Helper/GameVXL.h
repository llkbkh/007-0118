#pragma once
#include "007Helper.h"


class CGameVXL :public IGameClient
{
public:
	CGameVXL();
	static CGameVXL* GetInstance();

	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

	virtual void vHUD_Frame(double time);

	virtual bool vInit();
private:
	static inline CGameVXL* m_pInstance = nullptr;

	std::shared_ptr<CACEUtil>	m_pACEUtil;
};

