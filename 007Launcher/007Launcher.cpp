
// 007Launcher.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "007Launcher.h"
#include "007LauncherDlg.h"
#include "../007Helper/ThreadPool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy007LauncherApp

BEGIN_MESSAGE_MAP(CMy007LauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMy007LauncherApp 构造

CMy007LauncherApp::CMy007LauncherApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CMy007LauncherApp 对象

CMy007LauncherApp theApp;


// CMy007LauncherApp 初始化
// *
bool CreatLinkToStartMenu(wchar_t* pszPeFileName)
{

	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IShellLink* pisl;
		hr = CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pisl);
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//这里是我们要创建快捷方式的原始文件地址
			pisl->SetPath(pszPeFileName);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**)&pIPF);
			if (SUCCEEDED(hr))
			{
				/////////////////////////////////////////////////////////////////////////////////////////////////////////////

				//这里是我们要创建快捷方式的目标地址
				wchar_t szStartPath[MAX_PATH] = { 0 };
				SHGetSpecialFolderPath(NULL, szStartPath, CSIDL_DESKTOP, 0);
				StrCatW(szStartPath, L"\\CSO_007辅助启动.lnk");

				USES_CONVERSION;
				LPCOLESTR lpOleStr = /*A2COLE*/(szStartPath);

				pIPF->Save(lpOleStr, FALSE);

				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}

	return true;
}

BOOL CMy007LauncherApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	VMProtectBegin(__FUNCTION__);

	auto HOOK    = std::make_unique<CACEHook>();
	auto Util    = std::make_unique<CACEUtil>();
	auto Module  = std::make_unique<CACEModule>();
	auto Process = std::make_unique<CACEProcess>();



	TAG_ACE_HOOK_MODULE_INFO AppList[] =
	{
		ACE_HOOK_MODULE_INFO("0","007Launcher.exe","54ED5FB2C037A6BE7ED4467553ED4C9E"),								//启动器
	};

	if (!CACEModule::GetModuleDetect(AppList, sizeof(AppList) / sizeof(TAG_ACE_HOOK_MODULE_INFO)))
	{
		std::string Text = HOOK->GetModuleNameByAddress((DWORD)HOOK->GetSelfModuleHandle());
		
		Text = fmt::format(xorstr_("{}{}"),xorstr_(u8"请勿修改进程名:"), Text);

		AfxMessageBox(Util->UTF8_To_UniCode(Text).data(), MB_ICONSTOP);

		return FALSE;
	}
	//检查多开
	if (!Module->DetectMutex())
	{
		AfxMessageBox(xorstr_(L"已经运行了一个程序,请勿同时运行多个！"), MB_ICONSTOP);
		return FALSE;
	}
	//检查自身路径 判断是否在压缩包内
	if (strstr(HOOK->GetSelfModulePath().data(),xorstr_("AppData\\Local\\Temp\\")))
	{
		AfxMessageBox(xorstr_(L"请勿在压缩包内运行,请解压文件夹到桌面.！\r\nDisable running in temporary directory!"), MB_ICONSTOP);
		return FALSE;
	}
#if !ANTI_DEBUG
	if (strstr(HOOK->GetSelfModulePath().data(), xorstr_("CSO_007\\Bin")) == 0)
	{
		AfxMessageBox(xorstr_(L"请勿修改文件夹名称,请完整解压目录. \r\nDo not change the folder name. Decompress the folder completely\r\n请把压缩包放到桌面,右键-解压到当前文件夹\r\nPlease place the package on the desktop, right-click - unzip to the current folder"), MB_ICONSTOP);
		return FALSE;
	}
#endif // !ANTI_DEBUG

	wchar_t ulModuleName[MAX_PATH];
	GetModuleFileNameW(NULL, ulModuleName, MAX_PATH);
	CreatLinkToStartMenu(ulModuleName);

	auto ProcessInfo =  Process->GetProcessArrayInfoW(xorstr_(L"cstrike-online.exe"));

	for (auto& ite: *ProcessInfo)
	{
		Process->KillProcess(ite.second.th32ProcessID);
	}



	CMy007LauncherDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	VMProtectEnd();

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
void to_json(nlohmann::json& j, const JSON_FileInfo& p)
{

	j = nlohmann::json
	{
		{"Card", p.Card},
		{"GamePath", p.GamePath},
		{"PASW", p.PASW},
		{"SAVA", p.SAVA} ,
		{"SteamPath", p.SteamPath} ,
		{"User", p.User},
		{"GameServer", p.GameServer},		
	};
	j["HttpProxy"] = { {"enable", p.Proxy.bEnable}, {"ip", p.Proxy.ip} ,{"port", p.Proxy.port} ,{"user", p.Proxy.user} ,{"password", p.Proxy.password} };

}

void from_json(const nlohmann::json& j, JSON_FileInfo& p)
{
	j.at("Card").get_to(p.Card);
	j.at("GamePath").get_to(p.GamePath);
	j.at("PASW").get_to(p.PASW);
	j.at("SAVA").get_to(p.SAVA);
	j.at("SteamPath").get_to(p.SteamPath);
	j.at("User").get_to(p.User);

	if (j.count("GameServer"))
	{
		j.at("GameServer").get_to(p.GameServer);
	}
	if (j.count("HttpProxy"))
	{
		j.at("HttpProxy").at("enable").get_to(p.Proxy.bEnable);
		j.at("HttpProxy").at("ip").get_to(p.Proxy.ip);
		j.at("HttpProxy").at("port").get_to(p.Proxy.port);
		j.at("HttpProxy").at("user").get_to(p.Proxy.user);
		j.at("HttpProxy").at("password").get_to(p.Proxy.password);
	}

}
void SavaJsonInfo()
{
	VMProtectBegin(__FUNCTION__);

	auto pFile = std::make_shared<CACEFile>(xorstr_("C:\\Loginkss.json"));

	DeleteFileA(pFile->GetFileName().data());

	HANDLE hFile = pFile->OpenFile();

	if (hFile == INVALID_HANDLE_VALUE)
		return;



	//重写
	pFile->SetFilePointer(FILE_BEGIN);

	theApp.m_FileInfo = theApp.m_JsonInfo;
	auto info = theApp.m_FileInfo.dump(4);
	pFile->WriteFile(info.data(), info.size());

	pFile->CloseFile();

	VMProtectEnd();
}