// 007TestStartApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ACEConnect.h>
#include <RCF/RCF.hpp>
#include "../RCF007Interface.hpp"
#include <functional>
#pragma comment(lib,"RcfDll.lib")
class PrintServiceSession
{
public:
	std::string mUsername;
	std::uint32_t mMessagesPrinted = 0;
	PrintServiceSession()
	{
		ACEWarningLog("{}", __FUNCTION__);
	}
	~PrintServiceSession()
	{
		ACEWarningLog("{}", __FUNCTION__);
	}
};

class PrintServiceAuthenticationState
{
public:
	PrintServiceAuthenticationState() : mAuthenticated(false)
	{
		ACEWarningLog("{}", __FUNCTION__);
	}
	~PrintServiceAuthenticationState()
	{
		ACEWarningLog("{}", __FUNCTION__);
	}
	bool            mAuthenticated;
	std::string     mClientUsername;
};

void onClientDisconnect(RCF::RcfSession& session)
{
	ACEWarningLog("{}", "连接断开");// ...
}

typedef std::shared_ptr<PrintServiceSession> PrintServiceSessionPtr;
class DemoService
{
public:
	void Print(std::string& s)
	{

		ACEWarningLog("服务器接收{}",s);
		//std::cout << "Print服务器: " << s << std::endl;
		//// 获取当前连接的会话
		RCF::RcfSession& session = RCF::getCurrentRcfSession();
		//// 发送和收到的字节数
		//auto totalBytesReceived = session.getTotalBytesReceived();
		//std::cout << totalBytesReceived << std::endl;


		ACEWarningLog("获取此RcfSession的连接持续时间:{}", session.getConnectionDuration());
		//全部调用Call的次数.不是单个
		ACEWarningLog("调用次数:{}", session.getRemoteCallCount());
		//返回一个时间戳
		ACEWarningLog("返回客户端连接到RcfSession的时间:{}", session.getConnectedAtTime());
		//返回一个正在执行的RCF请求
		auto Request =  session.getRemoteCallRequest();
		//传输类型
		ACEWarningLog("getTransportType 传输类型:{}", session.getTransportType());
		//传输协议
		ACEWarningLog("getTransportProtocol 传输协议:{}", session.getTransportProtocol());
		//获取延迟
		ACEWarningLog("getPingBackIntervalMs 延迟ms:{}", session.getPingBackIntervalMs());
		//是否为单项调用
		ACEWarningLog("isOneway {}", session.isOneway());
		//客户端地址
		ACEWarningLog("ClientAddress {}", session.getClientAddress().string());

		session.getRcfServer().setConnectionIdleTimeoutMs(40);
		//服务器设置的超时时间
		ACEWarningLog("getConnectionIdleTimeoutMs {}", session.getRcfServer().getConnectionIdleTimeoutMs());
		//扫描空闲 间隔 30秒
		ACEWarningLog("getConnectionIdleScanIntervalMs {}", session.getRcfServer().getConnectionIdleScanIntervalMs());

		ACEWarningLog("getRuntimeVersion {}", session.getRcfServer().getRuntimeVersion());

		RCF::RcfServer& server = session.getRcfServer();
		std::string clientUsername = session.getClientUserName();
		if (/*clientUsername.size() > 0*/1)
		{
			std::uint32_t gcTimeoutMs = 10 * 1000;
			PrintServiceSessionPtr printSessionPtr = server.getServerObject<PrintServiceSession>(clientUsername,gcTimeoutMs);
			if (printSessionPtr->mUsername.empty())
			{
				printSessionPtr->mUsername = clientUsername;
			}
			++printSessionPtr->mMessagesPrinted;

			ACEWarningLog("mMessagesPrinted :{} {}", printSessionPtr->mMessagesPrinted, server.getServerObjectHarvestingIntervalS());

		}

		PrintServiceAuthenticationState& authState = session.getSessionObject<PrintServiceAuthenticationState>(true);
		if (!authState.mAuthenticated)
		{
			authState.mAuthenticated = true;
			authState.mClientUsername = session.getClientUserName();

		}

		//连接断开时 调用
		//auto onDestroy = [&](RCF::RcfSession& session) { (session); };
		//RCF::getCurrentRcfSession().setOnDestroyCallback(onClientDisconnect);


	}
	void T1(std::string& s)
	{


	}

};

// 回调连接创建完毕调用
void onCallbackConnectionCreated(RCF::RcfSessionPtr sessionPtr, RCF::ClientTransportUniquePtr transportPtr)
{
//	string& clientName = sessionPtr->getSessionObject<string>();
//	cout << "clientName" << clientName << endl;
//
//	// 保存
//	HelloWorldPtr helloWorldPtr(new RcfClient<I_HelloWorld>(transportPtr));
//	RCF::Lock lock(gCallbackClientMutex);
//	//gCallbackClients.push_back(helloWorldPtr);
//	gCallbackClients[clientName] = helloWorldPtr;
	MessageBoxA(0, 0, 0, 0);

	ACEWarningLog(__FUNCTION__);

}

bool onHelloWorldAccess(int dispatchId)
{
	ACEWarningLog("dispatchId:{}",dispatchId);

	return true;
}





int main()
{

	if (!CACEModule::GetModuleDetect(&TAG_ACE_HOOK_MODULE_INFO(xorstr_("007TestStartApp.exe")), 1))
	{
		return 0;
	}

	RCF::RcfInit rcfInit;

	std::string networkInterface = "0.0.0.0";
	int port = 50001;
	std::cout << "Starting server on " << networkInterface << ":" << port << "." << std::endl;

	DemoService demoService;
	RCF::RcfServer server(RCF::TcpEndpoint(networkInterface, port));
	server.setOnCallbackConnectionCreated(std::bind(&onCallbackConnectionCreated, std::placeholders::_1, std::placeholders::_2));
	RCF::ServerBindingPtr bindPtr = server.bind<I_007Service>(demoService);
	//每次调用远程函数时 都会调用这个 参数1 为 函数ID
	//bindPtr->setAccessControl(std::bind(&onHelloWorldAccess, std::placeholders::_1));
	bindPtr->setAccessControl(onHelloWorldAccess);

	//服务器传输设置
	//ACEInfoLog("[getTransportType]{}", server.getServerTransport().getTransportType());
	////最大传输大小
	//ACEInfoLog("[getMaxIncomingMessageLength]{:X}", server.getServerTransport().getMaxIncomingMessageLength());
	////服务器最大连接数
	//ACEInfoLog("[getConnectionLimit]{:X}", server.getServerTransport().getConnectionLimit());
	////返回服务器传输启动时创建的侦听连接的初始数量。
	//ACEInfoLog("[getInitialNumberOfConnections]{:X}", server.getServerTransport().getInitialNumberOfConnections());
	//设置上传带宽回调
	//server.setUploadBandwidthQuotaCallback(&uploadBandwidthQuotaCb);
	


	RCF::ThreadPoolPtr threadPoolPtr(new RCF::ThreadPool(1, 5));
	server.setThreadPool(threadPoolPtr);
	//连接超时
	server.setConnectionIdleTimeoutMs(10 * 1000);
	//检测连接超时
	server.setConnectionIdleScanIntervalMs(20 * 1000);


	server.setOnCallbackConnectionCreated(std::bind(&onCallbackConnectionCreated, std::placeholders::_1, std::placeholders::_2));
	server.getOnCallbackConnectionCreated();

	// Set max message length to 5 Mb.
	//server.getServerTransport().setMaxIncomingMessageLength(/*5 * 1024 * 1024*/);

	//RCF::setDefaultMaxMessageLength(1);
	server.start();

	// server.getServerTransport().setMaxIncomingMessageLength

	 //RCF::setDefaultMaxMessageLength(6000 * 1024); // 600k  服务器端默认接受大小为1Mb


	ACEInfoLog("[lib]{:X}", (ULONG)LoadLibraryA("007Helper.dll"));
	RCF::RcfSession* session = /*RCF::getCurrentRcfSession()*/ nullptr;
	

	ACEWarningLog("getConnectionIdleTimeoutMs {}", server.getConnectionIdleTimeoutMs());
	ACEWarningLog("getConnectionIdleScanIntervalMs {}", server.getConnectionIdleScanIntervalMs());
	ACEWarningLog("getDefaultMaxMessageLength {}", RCF::getDefaultMaxMessageLength());





	server.waitForStopEvent();


	while (true)
	{

		RCF::RcfSession& session = RCF::getCurrentRcfSession();


		

		continue;

		// 发送和收到的字节数
		auto totalBytesReceived = session.getTotalBytesReceived();
		std::cout << totalBytesReceived << std::endl;


		ACEWarningLog("获取此RcfSession的连接持续时间:{}", session.getConnectionDuration());
		//全部调用Call的次数.不是单个
		ACEWarningLog("调用次数:{}", session.getRemoteCallCount());
		//返回一个时间戳
		ACEWarningLog("返回客户端连接到RcfSession的时间:{}", session.getConnectedAtTime());
		//返回一个正在执行的RCF请求
		auto Request =  session.getRemoteCallRequest();
		//传输类型
		ACEWarningLog("getTransportType 传输类型:{}", session.getTransportType());
		//传输协议
		ACEWarningLog("getTransportProtocol 传输协议:{}", session.getTransportProtocol());
		//获取延迟
		ACEWarningLog("getPingBackIntervalMs 延迟ms:{}", session.getPingBackIntervalMs());
		//是否为单项调用
		ACEWarningLog("isOneway {}", session.isOneway());
		//客户端地址
		ACEWarningLog("ClientAddress {}", session.getClientAddress().string());

		session.getRcfServer().setConnectionIdleTimeoutMs(40);
		//服务器设置的超时时间
		ACEWarningLog("getConnectionIdleTimeoutMs {}", session.getRcfServer().getConnectionIdleTimeoutMs());
		//扫描空闲 间隔 30秒
		ACEWarningLog("getConnectionIdleScanIntervalMs {}", session.getRcfServer().getConnectionIdleScanIntervalMs());

		ACEWarningLog("getRuntimeVersion {}", session.getRcfServer().getRuntimeVersion());
		Sleep(2500);
	}




	







	getchar();
   
}
#pragma data_seg(".Shared")
namespace SharedData
{
	int		Key = 0x123;
	int		Value = 0x321;
};

#pragma comment(linker,"/section:.Shared,RWS")