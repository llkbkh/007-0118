#pragma once
#include <RCF../../../RCF007Interface.hpp>
#include <RCF/RCF.hpp>
#include "openssl/rc4.h"
#include <PackReader.h>



class SagaServiceSession
{
public:
	SagaServiceSession()
	{
		m_Session.clear();
	}
	~SagaServiceSession()
	{
		
	}
	std::string& CreteSession()
	{
		CACEUtil Util;

		return m_Session = Util.GetTimeStampStr();
	}
	std::string& GetSessionID()
	{
		return m_Session;
	}
private:
	std::string m_Session;
};


class SagaService
{
public:
	SagaService();

	// @登陆
	BOOL Login(std::vector<BYTE>& v);

	// @获取远程变量
	std::string GetVar(std::vector<BYTE>& v);

	// @心跳
	ULONG HeartBeat();

	// @输出
	ULONG Print(std::string &v);

	/*
	*	**********************************************************
	*	**********************2022-04-24 11:05:12*****************
	*	 V1.1版本更新后 增加的.之前的全部弃用。
	*/

	// @版本验证
	BOOL Version(std::vector<BYTE>& v);





private:
	std::vector<BYTE> SRC4Decode(std::vector<BYTE> CipherByte);
	std::vector<BYTE> SRC4Encrypt(std::vector<BYTE> CipherByte);
private:
	ULONG           m_Count;
	std::string     m_RC4Key;
};



class CRCFServerManage
{
public:
	CRCFServerManage();
	static CRCFServerManage* GetInstance();
	// @初始化
	BOOL Init(ULONG uPort = 31251);



	std::string& GetStrError();

	RCF::RcfServer* GetRcfServerPoInter();

private:
	RCF::RcfInit*   m_pRcfInit;
	SagaService*    m_pSagaServer;
	ULONG           m_uPort;
	RCF::RcfServer* m_pServer;
	std::string     m_StrError;

	static inline CRCFServerManage* m_pInstance = nullptr;
};

