#pragma once
#include <CACEInterface.h>
#include "LCUIStruct.h"

class Util
{
public:
	Util();
	~Util();

	// @检查多开
	 bool CheckMulti();
	
	// @检查自身路径是否在压缩包内
	 bool CheckSelfPathIsZipTempPath ();

	// @检查自身路径
	bool CheckSelfPath ();
	
	// @创建快捷方式
	bool CreateShortcut();
	
	// @exit game process
	void ExitGameProcess();

	// @create thread Monitorin Game
	void CreateThreadMonitorinGame();
	
	// @clear regedit
	void ClearRegedit(std::string GamePath);

	// @Start Game
	bool CsoLauncher(int GameRegion);

	// @设置注册表值
	bool SetRegeditValue(std::string sub_key, const std::string& key, int value);

private:
	std::shared_ptr<CACEUtil>   m_pACEUtil;
	std::shared_ptr<CACEModule> m_pACEModule;
	std::shared_ptr<CACEHook>   m_pACEHook;
};

extern Util g_Util;