// CMainDlg.cpp: 实现文件
//

#include "pch.h"
#include "007Launcher.h"
#include "afxdialogex.h"
#include "CMainDlg.h"
#include <sddl.h>
#include <CACEInterface.h>



// CMainDlg 对话框

IMPLEMENT_DYNAMIC(CMainDlg, CDialogEx)

CMainDlg::CMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
	, m_Edit_BinPath(_T(""))
{

}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_BinPath);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_BinPath);
	DDX_Control(pDX, IDC_BUTTON2, m_Button_StartCheat);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &CMainDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMainDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序


BOOL CMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	VMProtectBegin(__FUNCTION__);

	auto pUtil = std::make_shared<CACEUtil>();

	auto Info =  Verify.CloudQueryUserInfo(theApp.m_User);

	if (!std::get<0>(Info))
	{
		AfxMessageBox(pUtil->UTF8_To_UniCode(std::get<1>(Info)).data(), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}

	auto& dataVar = std::get<2>(Info);
	std::string VarText = dataVar.at(xorstr_("timeout"));


	auto Text = fmt::format(xorstr_("{}:{}"),xorstr_(u8"到期时间"), VarText);


	AfxMessageBox(pUtil->UTF8_To_UniCode(Text).data(), MB_OK| MB_ICONINFORMATION);


	m_Edit_BinPath = pUtil->UTF8_To_UniCode(theApp.m_JsonInfo.GamePath).data();


	if (!m_RCFManage.Init())
	{
		AfxMessageBox(pUtil->UTF8_To_UniCode(m_RCFManage.GetStrError()).data(), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}

	auto hModule =  LoadLibraryA(xorstr_("007Helper.dll"));

	if (!hModule)
	{
		AfxMessageBox(xorstr_(L"模块加载失败！"), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}



	std::thread th(&CMainDlg::ThreadHeat, this);
	th.detach();

#if !ANTI_DEBUG
	std::thread thModule(&CMainDlg::ThreadCheckGameModule, this);
	thModule.detach();

#endif
	

	
	ULONG ulGameServerIndex = theApp.m_JsonInfo.GameServer;

	switch (ulGameServerIndex)
	{
	case  0:
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		break;
	case  1:
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
		break;
	case  2:
		((CButton*)GetDlgItem(IDC_RADIO3))->SetCheck(1);
		break;
	case  3:
		((CButton*)GetDlgItem(IDC_RADIO4))->SetCheck(1);
		break;
	case  4:
		((CButton*)GetDlgItem(IDC_RADIO5))->SetCheck(1);
		break;
	case  5:
		((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(1);
		break;
	default:
		break;
	}

	UpdateData(FALSE);

	
	VMProtectEnd();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMainDlg::OnClose()
{
	CACEProcess Process;
	Process.ProcessSetToken();

	int nRet = AfxMessageBox(xorstr_(L"确定要关闭007么?关闭后辅助也会一起关闭！"),MB_YESNO);

	if (nRet == IDYES)
	{
		std::map<DWORD, PROCESSENTRY32W>* pMapProcess =  Process.GetProcessArrayInfoW(xorstr_(L"cstrike-online.exe"));

		for (auto& it : *pMapProcess)
		{
			Process.KillProcess(it.second.th32ProcessID);
		}
	}
	else
	{
		return;
	}

	exit(0);

	CDialogEx::OnClose();
}


void CMainDlg::OnBnClickedButton1()
{
	TCHAR szFolderPath[MAX_PATH] = { 0 };
	CString strFolderPath = L"";
	BROWSEINFO sInfo;
	ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = L"请选择游戏BIN目录";
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名
		if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
	}
	if (lpidlBrowse != NULL)
	{
		CoTaskMemFree(lpidlBrowse);
	}
	CACEFile File;
	CACEUtil Util;
	File.SetFileName(Util.UTF8_To_string(fmt::format(xorstr_("{}\\FileSystem_Nar.dll"), Util.UnicodeToUtf8(szFolderPath))));

	if (File.OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(xorstr_(L"Bin目录设置错误!不会找目录请到官网看教程！"), MB_ICONSTOP);
		return;
	}
	File.CloseFile();

	m_Edit_BinPath = strFolderPath;
	theApp.m_JsonInfo.GamePath = Util.UnicodeToUtf8(szFolderPath);



	UpdateData(FALSE);
}
enum class StartGameType
{
	Steam    = -1,				//steam 方式启动
	Launcher = 0,				//游戏启动器启动
	CN_D1    = 5,				//游戏主程序启动
	CN_D2    = 6,
	CN_W1    = 15,
};
typedef struct TAG_LAUNCHER_STRING
{
	int		Size;
	char	Name[MAX_PATH];
	TAG_LAUNCHER_STRING(char* Value) : Size(0)
	{
		Size = wsprintfA(Name, "%s", Value);
	}
}*PTAG_LAUNCHER_STRING;

typedef struct TAG_LAUNCHER_PARAM
{
	TAG_LAUNCHER_STRING	Name;
	TAG_LAUNCHER_STRING	Value;
}*PTAG_LAUNCHER_PARAM;
void SetValue(LPVOID Info, int& Index, int Size, ULONG* Value)
{
	memcpy((void*)((ULONG)Info + Index), Value, Size);
	Index = Index + Size;
}
bool  CsoLauncher(int GameRegion)
{
	VMProtectBegin(__FUNCTION__);

	char* Name = "CSO.SharedDict";

	ULONG	hSize = 0x8000;

	HANDLE hFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, Name);

	if (!hFile)
	{
		hFile = CreateFileMappingA(GetCurrentProcess(), NULL, PAGE_EXECUTE_READWRITE, 0, hSize, Name);
	}
	if (!hFile) return false;

	LPVOID	Info = MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, 0, hSize);

	if (!Info) return false;

	char	Region[MAX_PATH];
	wsprintfA(Region, "%d", GameRegion);

	TAG_LAUNCHER_PARAM	ArrayInfo[] =
	{
		TAG_LAUNCHER_STRING("launched")	,TAG_LAUNCHER_STRING("1"),
		TAG_LAUNCHER_STRING("mode")		,TAG_LAUNCHER_STRING(""),
		TAG_LAUNCHER_STRING("passport")	,TAG_LAUNCHER_STRING(""),
		TAG_LAUNCHER_STRING("region")	,TAG_LAUNCHER_STRING(Region),	//大区
		TAG_LAUNCHER_STRING("type")		,TAG_LAUNCHER_STRING("0"),
	};

	//填充数据
	ULONG	Number = sizeof(ArrayInfo) / sizeof(TAG_LAUNCHER_PARAM);;
	ULONG	Value = Number;
	int		Index = 4;
	SetValue(Info, Index, 4, &Value);	//参数数量

	for (int i = 0; i < (int)Number; i++)
	{
		PTAG_LAUNCHER_PARAM	ParamInfo = &ArrayInfo[i];

		Value = ParamInfo->Name.Size;

		SetValue(Info, Index, 4, &Value);
		SetValue(Info, Index, ParamInfo->Name.Size, (ULONG*)ParamInfo->Name.Name);

		Value = ParamInfo->Value.Size;
		SetValue(Info, Index, 4, &Value);
		SetValue(Info, Index, ParamInfo->Value.Size, (ULONG*)ParamInfo->Value.Name);
	}
	VMProtectEnd();
	return true;
}


void CMainDlg::OnBnClickedButton2()
{
	StartGameType Region = StartGameType::Launcher;
	CACEProcess Process;
	std::string GameApp;

	ClearRegedit();


	ULONG ulGameServerIndex = 0;
	ULONG uVersion          = 0;
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
	{
		Region            = StartGameType::CN_D1;
		ulGameServerIndex = 0;
		uVersion |= 1 << 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		Region = StartGameType::CN_D2;
		ulGameServerIndex = 1;
		uVersion |= 1 << 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())
	{
		Region            = StartGameType::CN_W1;
		ulGameServerIndex = 2;
		uVersion |= 1 << 0;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())
	{
		Region            = StartGameType::Steam;
		ulGameServerIndex = 3;
		uVersion |= 1 << 1;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck())
	{
		Region            = StartGameType::Launcher;
		ulGameServerIndex = 4;
		uVersion |= 1 << 2;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO6))->GetCheck())
	{
		Region            = StartGameType::Launcher;
		ulGameServerIndex = 5;
		uVersion |= 1 << 3;
	}

	theApp.m_JsonInfo.GameServer = ulGameServerIndex;

	if (theApp.m_JsonInfo.GamePath.empty())
	{
		AfxMessageBox(xorstr_(L"请先设置游戏BIN目录！"), MB_ICONSTOP);
		OnBnClickedButton1();
		return;
	}

	//检查是否支持当前服务器
	auto pVar = Verify.CloudGetTeletVar(theApp.m_User, xorstr_("CS_GameServerVersion"));

	if (std::get<0>(pVar))
	{
		auto& dataVar       = std::get<2>(pVar);
		std::string VarText = dataVar.at(xorstr_("variable"));

		if ((std::stoi(VarText) & uVersion) == FALSE)
		{
			AfxMessageBox(xorstr_(L"暂时不支持当前服务器启动 ,请关注官网通知 \nThe current server startup is not supported, please pay attention to the official website notice"), MB_ICONSTOP);
			return;
		}
	}
	else
	{
		AfxMessageBox(xorstr_(L"获取远程版本号超时.请检查网络连接"), MB_ICONSTOP);
		return;
	}


	//检查是否多开游戏
	//CACEProcess Pro;

	//if (Pro.GetProcessId(xorstr_("cstrike-online.exe")) > 1)
	//{
	//	AfxMessageBox(xorstr_(L""), MB_ICONSTOP);
	//}
#if 0
	auto OS = CACEInterface::GetInstance()->GetModule()->GetSystemOS();

	static BOOL isNotice = FALSE;

	if (!isNotice)
	{
		isNotice = TRUE;

		std::string TextNotice = std::get<0>(OS) == SYSTEM_OS::win7 ? xorstr_(R"(由于游戏反外挂更新,目前辅助对WIN7系统的检测.暂时无法修复.但是这不代表不能用,可以用.但是会有封号的风险\n
如果您是家庭用户,请尽快升级为WIN10系统.如果您是网吧用户只有WIN7.请用小号玩.
目前无法预知WIN7是不是100%封号,需要下周三(5月25号)游戏维护才知道.但是就算下周三不封号.也请尽快升级为WIN10系统.
如果您是小号封不封号无所谓.那请无视本条消息.请继续奔放！大号请勿用WIN7系统开挂！
如果您已知晓风险.请点[是],不同意请点[否]退出辅助！)") : xorstr_(R"(由于游戏反外挂更新,目前无法知晓会不会封号.请暂时用小号玩.下周三（5-25号）会出结果！

如果您已知晓风险.请点[是],不同意请点[否]退出辅助！)");

		if (AfxMessageBox(CACEInterface::GetInstance()->GetUtil()->StringToWstring(TextNotice).data(), MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			exit(0);
			return;
		}

	}
#endif
	

	





	if (Region != StartGameType::Launcher && Region != StartGameType::Steam)
	{
		//不用启动器启动
		GameApp = fmt::format(xorstr_("{}\\cstrike-online.exe"),theApp.m_JsonInfo.GamePath);
	
		CsoLauncher(static_cast<int>(Region));
	}
	else if(Region == StartGameType::Launcher)
	{
		GameApp = fmt::format(xorstr_("{}\\CSOLauncher.exe"), theApp.m_JsonInfo.GamePath);
	}
	else if (Region == StartGameType::Steam)
	{
		auto SteamPath = [&]()->BOOL
		{
			TCHAR szFolderPath[MAX_PATH] = { 0 };
			CString strFolderPath = L"";
			BROWSEINFO sInfo;
			ZeroMemory(&sInfo, sizeof(BROWSEINFO));

			sInfo.pidlRoot = 0;
			sInfo.lpszTitle = L"请选择Steam目录";
			sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
			sInfo.lpfn = NULL;

			// 显示文件夹选择对话框
			LPITEMIDLIST lpidlBrowse = SHBrowseForFolder(&sInfo);
			if (lpidlBrowse != NULL)
			{
				// 取得文件夹名
				if (SHGetPathFromIDList(lpidlBrowse, szFolderPath))
				{
					strFolderPath = szFolderPath;
				}
			}
			if (lpidlBrowse != NULL)
			{
				CoTaskMemFree(lpidlBrowse);
			}
			CACEFile File;
			CACEUtil Util;
			File.SetFileName(Util.UTF8_To_string(fmt::format(xorstr_("{}\\steam.exe"), Util.UnicodeToUtf8(szFolderPath))));
	

			if (File.OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
			{
				AfxMessageBox(xorstr_(L"Steam路径设置错误.请注意是Steam路径不是游戏路径"), MB_ICONSTOP);
				return FALSE;
			}
			File.CloseFile();

			theApp.m_JsonInfo.SteamPath = Util.UnicodeToUtf8(szFolderPath);

			return TRUE;
		};

		if (ULONG ulPid = Process.GetProcessId(xorstr_("steam.exe")))
		{
			Process.KillProcess(ulPid);
		}

		if (theApp.m_JsonInfo.SteamPath.empty())
		{
			if (!SteamPath())
			{
				return;
			}
			SavaJsonInfo();
		}
		GameApp = fmt::format(xorstr_("{}\\steam.exe"), theApp.m_JsonInfo.SteamPath);
	}

	STARTUPINFOA			StartInfo = { sizeof(StartInfo) };
	PROCESS_INFORMATION		ProcessInfo = { 0 };

	CACEUtil Util;
	BOOL	nRet = CreateProcessA(Util.UTF8_To_string(GameApp).data(), "", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, Util.UTF8_To_string(theApp.m_JsonInfo.GamePath).data(), &StartInfo, &ProcessInfo);

	SavaJsonInfo();
}

void CMainDlg::ThreadHeat()
{
	VMProtectBegin(__FUNCTION__);

	auto pUtil = std::make_shared<CACEUtil>();

	while (true)
	{
		auto pResult =  Verify.CloudHeartBeat(theApp.m_User);

		theApp.m_FreeYunStatus = std::get<0>(pResult);

		if (!theApp.m_FreeYunStatus)
		{
			m_RCFManage.SendAllClientMsg(std::get<1>(pResult));

			AfxMessageBox(pUtil->UTF8_To_UniCode(std::get<1>(pResult)).data(), MB_ICONSTOP);

			exit(0);
		}
		else
		{
			ACEDebugLog("[{}] Ok",__FUNCTION__);
		}
		Sleep(60 * 1000 * 5);
	}
	VMProtectEnd();
}

void CMainDlg::ThreadCheckGameModule()
{
	VMProtectBegin(__FUNCTION__);

	auto pUtil = std::make_shared<CACEUtil>();
	auto pProcess = std::make_shared<CACEProcess>();



	while (true)
	{
		BOOL GameInit    = FALSE;
		BOOL CheatInit   = FALSE;
		BOOL PassNGSInit = FALSE;

		auto pProcessInfo =  pProcess->GetProcessArrayInfoW(xorstr_(L"cstrike-online.exe"));

		for (auto & iter : *pProcessInfo)
		{
			auto CallBack = [&](MODULEENTRY32W* Module)
			{
				if (lstrcmpW(Module->szModule,xorstr_(L"mp.dll")) == 0)
				{
					GameInit = TRUE;

					if (!CheatInit || !PassNGSInit)
					{
						//结束进程
						pProcess->KillProcess(iter.second.th32ProcessID);

					}
				}
				else if (lstrcmpW(Module->szModule, xorstr_(L"007Helper.dll")) == 0)
				{
					CheatInit = TRUE;
				}
				else if (lstrcmpW(Module->szModule, xorstr_(L"AmberHelper.dll")) == 0)
				{
					PassNGSInit = TRUE;
				}
			};


			pProcess->EnumProcessModule(iter.second.th32ProcessID, CallBack);
		}

		if (GameInit)
		{
			if (!CheatInit)
			{
				AfxMessageBox(pUtil->UTF8_To_UniCode(xorstr_(u8"The game is not loading")).data(), MB_ICONSTOP);
			}
			if (!PassNGSInit)
			{
				AfxMessageBox(pUtil->UTF8_To_UniCode(xorstr_(u8"Module loading error!")).data(), MB_ICONSTOP);
			}
		}

		Sleep(10 * 1000);
	}

	VMProtectEnd();
}

void CMainDlg::ClearRegedit()
{
	CACEFile File;

	auto FileName = fmt::format("{}\\BlackCipher",theApp.m_JsonInfo.GamePath);

	auto ClearLog = [&](char* szFileName, char* szAttribFile)
	{
		char lpFileName[MAX_PATH];

		wsprintfA(lpFileName, "%s\\%s", szFileName, szAttribFile);

		WIN32_FIND_DATAA	FindDate;
		HANDLE	hFile = FindFirstFileA(lpFileName, &FindDate);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return;
		}
		while (FindNextFileA(hFile, &FindDate))
		{
			char sTempFileName[MAX_PATH];
			wsprintfA(sTempFileName, "%s\\%s", szFileName, FindDate.cFileName);

			DeleteFileA(sTempFileName);

			//ACEDebugLog("{}", sTempFileName);
		}
	};


	auto ClearSysHistoryRegistry = [&]()
	{
		std::string StrKeySID;
		WCHAR cUserNameBuffer[MAX_PATH] = { 0 };
		DWORD dwUserNameSize = MAX_PATH;

		if (GetUserNameW(cUserNameBuffer, &dwUserNameSize))
		{
			DWORD need1 = 0;
			DWORD need2 = 0;
			SID_NAME_USE snu;
			LookupAccountNameW(
				NULL,
				cUserNameBuffer,
				NULL, &need1,
				NULL, &need2,
				NULL
			);
			LPBYTE pSid = (BYTE*)VirtualAlloc(NULL, need1 * sizeof(BYTE), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

			LPWSTR pNoUser = (LPWSTR)VirtualAlloc(NULL, need2 * sizeof(TCHAR), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			LookupAccountNameW(
				NULL,
				cUserNameBuffer,
				pSid, &need1,
				pNoUser, &need2,//我试来试去都发现这两个不能省,虽然我没有使用他们
				&snu			//我试来试去都发现这两个不能省,虽然我没有使用他
			);
			char* lpszSID = NULL;
			ConvertSidToStringSidA((PSID)pSid, &lpszSID);

			StrKeySID = lpszSID;
		}
		else
		{
			
			return;
		}

		HKEY hKey = NULL; //保存注册表的句柄 
		DWORD dwIndexs = 0; //需要返回子项的索引 
		TCHAR keyName[MAX_PATH] = { 0 }; //保存子键的名称 
		DWORD charLength = MAX_PATH;  //想要读取多少字节并返回实际读取到的字符长度

		char* subKey[] = { xorstr_("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{F4E57C4B-2036-45F0-A9AB-443BCFE33D9F}\\Count"),xorstr_("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA}\\Count" ),xorstr_("\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{75048700-EF1F-11D0-9888-006097DEACF9}\\Count" )};

		/*
		{0D6D4F41-2994-4BA0-8FEF-620E43CD2812} –似乎特定于IE7的密钥
		{5E6AB780-7743-11CF-A12B-00AA004AE837} – IE收藏夹和其他IE工具栏对象
		{75048700-EF1F-11D0-9888-006097DEACF9} –已访问的应用程序，文件，链接和其他对象的列表。

		{CEBFF5CD-ACE2-4F4F-9178-9926F41749EA} –已访问的应用程序，文件，链接和其他对象的列表。
		{F4E57C4B-2036-45F0-A9AB-443BCFE33D9F} –列出用于启动程序的快捷链接
		*/

		for (int i = 0; i < 3; i++)
		{
			std::string strDelKey = StrKeySID;

			strDelKey.append(subKey[i]);

			char str[MAX_PATH];

			DWORD num = sizeof(str), index = 0;

			if (RegOpenKeyExA(HKEY_USERS, strDelKey.c_str(), 0, KEY_READ | KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
			{
				while (RegEnumValueA(hKey, index, str, &num, NULL, NULL, NULL, NULL) == 0)
				{
					index++;
					num = MAX_PATH;
					RegDeleteValueA(hKey, str);

					//ACEDebugLog("{}", str);
				}

			}

		}
		if (hKey)
		{
			RegCloseKey(hKey);
		}
	};


	ClearLog(FileName.data(), xorstr_("*.log"));

	ClearLog(xorstr_("C:\\Windows\\Prefetch"), xorstr_("*.pf"));


	// 清理1024次
	for (size_t i = 0; i < 1024; i++)
	{
		ClearSysHistoryRegistry();

	}
}
