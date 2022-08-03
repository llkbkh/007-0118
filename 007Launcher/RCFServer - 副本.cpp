#include "pch.h"
#include "RCFServer.h"
#include "007Launcher.h"
#include <RCF/ProxyEndpoint.hpp>
#include "RCF/ClientProgress.hpp"
#include "RCF/Enums.hpp"


CRCFServerManage::CRCFServerManage()
{
	m_pRcfInit     = nullptr;
	m_uPort        = 0;
	m_pSagaServer  = nullptr;
	m_pServer      = nullptr;
	m_pInstance    = this;
}

CRCFServerManage* CRCFServerManage::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CRCFServerManage();
	}
	return m_pInstance;
}

BOOL CRCFServerManage::Init(ULONG uPort)
{
	m_pRcfInit = new RCF::RcfInit();

	m_pSagaServer = new SagaService();

	try
	{
		m_pServer = new RCF::RcfServer(RCF::TcpEndpoint(xorstr_("0.0.0.0"), uPort));
		//启用代理端点
		m_pServer->setEnableProxyEndpoints(true);
		m_pServer->setThreadPool(RCF::ThreadPoolPtr(new RCF::ThreadPool(1, 10)));

		auto Ptr =  m_pServer->bind<I_SagaService>(*m_pSagaServer);

		m_pServer->start();


	}
	catch (const RCF::Exception& e)
	{
		m_StrError = e.getErrorMessage();

		return 0;
	}

	return m_pServer->isStarted();
}
std::vector<BYTE> SagaService::SRC4Decode(std::vector<BYTE> CipherByte)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, m_RC4Key.length(), (unsigned char*)m_RC4Key.c_str());
	std::vector<BYTE>DecodeText(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), DecodeText.data());     //解密
	return DecodeText;
}

std::vector<BYTE> SagaService::SRC4Encrypt(std::vector<BYTE> CipherByte)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, m_RC4Key.length(), (unsigned char*)m_RC4Key.c_str());
	std::vector<BYTE> Rc4Data(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), Rc4Data.data());		//加密
	return Rc4Data;
}

std::string& CRCFServerManage::GetStrError()
{
	return m_StrError;
}

RCF::RcfServer* CRCFServerManage::GetRcfServerPoInter()
{
	return m_pServer;
}

bool onPrintServiceAccess(int methodId)
{
	//如果（方法 ID == 0）
	//{
	//	// 始终允许调用 Login()。  login = 第几个方法
	//	返回 真；
	//}
	//	别的
	//{
	//	RCF::RcfSession & 会话 = RCF::getCurrentRcfSession();
	//	PrintServiceAuthenticationState& authState = 会话。getSessionObject <PrintServiceAuthenticationState>(真);
	//	返回authState.mAuthenticated;
	//}

	return false;
}
void onClientDisconnect(RCF::RcfSession& session)
{
	//断开回调
}
void onRemoteCallProgress(const  RCF::RemoteCallProgressInfo& info, RCF::RemoteCallAction& action)
{



	ACEDebugFileLog("[{}] {} {} {}" ,__FUNCTION__, info.mBytesTransferred, info.mBytesTotal, (ULONG)info.mPhase);
}



SagaService::SagaService()
{
	m_RC4Key = xorstr_("99BA5433DF5FA898C8E078B8BA55F251");
	m_Count = 0;
}

BOOL SagaService::Login(std::vector<BYTE>& v)
{
	VMProtectBegin(__FUNCTION__);

	BOOL bRet = FALSE;

	// 获取当前连接的会话
	auto VecCode = SRC4Decode(v);
	auto lPackRead = std::make_shared<PackReader>(VecCode);

	std::string Account = lPackRead->ReadStr();
	std::string PassWord = lPackRead->ReadStr();

	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();

	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	if (Account == "admin" && PassWord == "123456")
	{
		if (SagaSession.GetSessionID().empty())
		{
			auto ValueSession =  SagaSession.CreteSession();

			bRet = TRUE;

			std::vector<BYTE> VecSession(ValueSession.length()+ 10);
			memcpy(VecSession.data(), ValueSession.data(), ValueSession.length());

			auto EnValue =  SRC4Encrypt(VecSession);

			RtlZeroMemory(v.data(),v.size());
		
			std::copy(EnValue.begin(),EnValue.end(),v.begin());


			auto onDestroy = [&](RCF::RcfSession& session)
			{ 
				onClientDisconnect(session); 
			};
			RCF::getCurrentRcfSession().setOnDestroyCallback(onDestroy);

		}
		else
		{
			//账号已经登陆

		}

	}
	else
	{
		// 账号密码不正确
	}

	VMProtectEnd();

	auto Test = [=]()
	{
		try
		{
			std::vector<std::string> proxyEndpointNames;
			CRCFServerManage::GetInstance()->GetRcfServerPoInter()->enumerateProxyEndpoints(proxyEndpointNames);




			while (TRUE)
			{
				if (proxyEndpointNames.size() != 0)
				{
					//RcfClient<I_SagaService> client(RCF::ProxyEndpoint(RCF::TcpEndpoint(xorstr_("127.0.0.1"), 12356), "wocaonima"));
					RcfClient<I_SagaService> client(RCF::ProxyEndpoint(*(CRCFServerManage::GetInstance()->GetRcfServerPoInter()), proxyEndpointNames.at(0)));
					std::string v2 = "Making call from server to client.";
					client.getClientStub().setRemoteCallProgressCallback(onRemoteCallProgress, 500);// 每500ms回调一次
					RCF::globals().setDefaultRemoteCallTimeoutMs(10000);

					ACEDebugFileLog("{}", client.getClientStub().getRemoteCallTimeoutMs());
					client.Print(v2);

					

					
				}
				Sleep(200);
			}



			


		}
		catch (const RCF::Exception& e)
		{

			ACEDebugFileLog("{}", e.getErrorMessage());


		}

	};

	//std::thread t(Test);
	//t.detach();



	return bRet;
}

std::string SagaService::GetVar(std::vector<BYTE>& v)
{
	VMProtectBegin(__FUNCTION__);

	std::string Result;

	auto VecCode                    = SRC4Decode(v);
	auto lPackRead                  = std::make_shared<PackReader>(VecCode);
	std::string RemoteVar           = lPackRead->ReadStr();
	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();

	std::string ClientSession =  ConnectSession.getRequestUserData();

	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	if (SagaSession.GetSessionID() != ClientSession)
	{
		Result = xorstr_("[Saga]Error:Session NotMatch !");
		return Result;
	}

	auto  pVar = Verify.CloudGetTeletVar(theApp.m_User, RemoteVar);

	if (std::get<0>(pVar) == false)
	{
		Result = std::get<1>(pVar);

		return Result;
	}

	Result = std::get<2>(pVar).at(xorstr_("variable"));

	VMProtectEnd();
	return Result;
}

ULONG SagaService::HeartBeat()
{
	VMProtectBegin(__FUNCTION__);

	RCF::RcfSession& ConnectSession = RCF::getCurrentRcfSession();

	std::string ClientSession = ConnectSession.getRequestUserData();

	SagaServiceSession& SagaSession = ConnectSession.getSessionObject<SagaServiceSession>(true);

	if (SagaSession.GetSessionID() != ClientSession)
	{
		return 0;
	}
	if (!theApp.m_FreeYunStatus)
	{
		return 0;
	}


	CACEUtil Util;

	ULONG UnixTimeStamp = Util.GetUnixTimeStamp();

	VMProtectEnd();

	return UnixTimeStamp;
}

ULONG SagaService::Print(std::string& v)
{
	ACEDebugFileLog("{}:{}",__FUNCTION__,v);

	return 0;
}

BOOL SagaService::Version(std::vector<BYTE>& v)
{
	BOOL bRet = FALSE;
	auto lPackRead = std::make_shared<PackReader>(v);








	return bRet;
}

