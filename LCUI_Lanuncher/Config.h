#pragma once
#include <CACEInterface.h>
#include "LCUIStruct.h"

class Config
{
public:
	Config();
	~Config();
	
	static Config* GetInstance();
	
	void Load();
	void Save();
	std::string GetRandTitle();
	
	JSON_FileInfo& GetConfig();
private:
	std::string m_Filename;

	//保存的配置结构体
	JSON_FileInfo  m_Config;
	//Json保存的配置
	nlohmann::json m_JsonInfo;
	
	//单例模式
	inline static Config* m_Instance = nullptr;
};

