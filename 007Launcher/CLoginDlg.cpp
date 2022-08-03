// CLoginDlg.cpp: 实现文件
//

#include "pch.h"
#include "007Launcher.h"
#include "afxdialogex.h"
#include "CLoginDlg.h"
#include<fstream>
#include "CMainDlg.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, m_Edit_LoginUser(_T(""))
	, m_Edit_LoginPassWord(_T(""))
	, m_Edit_Notic(_T(""))
	, m_Edit_Card(_T(""))

{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_Edit_LoginUser);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_LoginUser);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_LoginPassWord);
	DDX_Control(pDX, IDC_BUTTON1, m_Button_LoginUser);
	DDX_Text(pDX, IDC_EDIT4, m_Edit_Notic);
	DDX_Control(pDX, IDC_CHECK1, m_Check_Sava);
	//  DDX_Text(pDX, IDC_EDIT3, m_Edit_Card);
	DDX_Text(pDX, IDC_EDIT3, m_Edit_Card);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CLoginDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	VMProtectBegin(__FUNCTION__);


	if (m_Edit_LoginUser.IsEmpty() || m_Edit_LoginPassWord.IsEmpty())
	{
		AfxMessageBox(xorstr_(L"账号或密码请填写完整."), MB_ICONSTOP);
		return;
	}

	auto  pUtil = std::make_shared<CACEUtil>();

	auto info = Verify.CloudLogin(pUtil->UnicodeToUtf8(m_Edit_LoginUser.GetBuffer()), pUtil->UnicodeToUtf8(m_Edit_LoginPassWord.GetBuffer()),"0");


	if (std::get<0>(info) == false)
	{
		int nCode = Verify.GetErrorCode();

		if (nCode == 1010)
		{

			//需解绑
			info = Verify.CloudUserUnBind(pUtil->UnicodeToUtf8(m_Edit_LoginUser.GetBuffer()), pUtil->UnicodeToUtf8(m_Edit_LoginPassWord.GetBuffer()));

			if (std::get<0>(info) == true)
			{
				AfxMessageBox(xorstr_(L"出现此提示,证明需要账号解绑[本次登陆电脑与上次登录电脑不符],已为您自动解绑.你需要再登陆一次！"));
				return;
			}
		}

		AfxMessageBox(pUtil->UTF8_To_UniCode(std::get<1>(info)).data(), MB_ICONSTOP);
		return ;
	}

	auto& data = std::get<2>(info);
	int nCode = data.at("code");
	if (nCode != 1014)
	{
		AfxMessageBox(pUtil->UTF8_To_UniCode(Verify.GetErrorStr(nCode)).data(), MB_ICONSTOP);
		return;
	}
	//登陆成功
	theApp.m_User = pUtil->UnicodeToUtf8(m_Edit_LoginUser.GetBuffer());

	//  保存本地配置
	if (theApp.m_JsonInfo.SAVA)
	{
		theApp.m_JsonInfo.User = pUtil->UnicodeToUtf8(m_Edit_LoginUser.GetBuffer());
		theApp.m_JsonInfo.PASW = pUtil->UnicodeToUtf8(m_Edit_LoginPassWord.GetBuffer());
	}

	Login();


	VMProtectEnd();
}


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	auto pUtil = std::shared_ptr<CACEUtil>();

	m_Check_Sava.SetCheck(theApp.m_JsonInfo.SAVA);

	if (theApp.m_JsonInfo.SAVA)
	{
		m_Edit_LoginUser     = pUtil->UTF8_To_UniCode(theApp.m_JsonInfo.User).data();
		m_Edit_LoginPassWord = pUtil->UTF8_To_UniCode(theApp.m_JsonInfo.PASW).data();
		m_Edit_Card          = pUtil->UTF8_To_UniCode(theApp.m_JsonInfo.Card).data();
	}




	m_Edit_Notic = theApp.m_Notic;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedCheck1()
{
	//theApp.m_JsonInfo.SAVA = m_Check_Sava.GetCheck();

	//SavaJsonInfo();

}

BOOL CLoginDlg::CheckUp(std::string& ver)
{
	BOOL bRet = FALSE;

	auto pVar =  Verify.CloudGetTeletVar(theApp.m_User, xorstr_("CS_Updata_New007"));

	if (std::get<0>(pVar))
	{

		auto& dataVar = std::get<2>(pVar);
		std::string VarText = dataVar.at(xorstr_("variable"));

		if (lstrcmpiA(VarText.data(), UP_007) == 0)
		{
			bRet = TRUE;
		}
		ver = VarText.data();
	}
	
	return bRet;
}


void CLoginDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	VMProtectBegin(__FUNCTION__);


	if (m_Edit_Card.IsEmpty() )
	{
		AfxMessageBox(xorstr_(L"卡号请填写完整.请注意不要有空格！"), MB_ICONSTOP);
		return;
	}

	auto  pUtil = std::make_shared<CACEUtil>();

	auto info = Verify.CloudCardLogin(pUtil->UnicodeToUtf8(m_Edit_Card.GetBuffer()) ,"0");



	if (std::get<0>(info) == false)
	{
		int nCode = Verify.GetErrorCode();

		if (nCode == 1010)
		{
			//需解绑
			info = Verify.CloudUserUnBind(pUtil->UnicodeToUtf8(m_Edit_Card.GetBuffer()));

			if (std::get<0>(info) == true)
			{
				AfxMessageBox(xorstr_(L"出现此提示,证明需要卡号解绑[本次登陆电脑与上次登录电脑不符],已为您自动解绑.你需要再登陆一次！"));
				return;
			}

		}

		AfxMessageBox(pUtil->UTF8_To_UniCode(std::get<1>(info)).data(), MB_ICONSTOP);
		return;
	}

	auto& data = std::get<2>(info);
	int nCode = data.at("code");
	if (nCode != 1014)
	{
		AfxMessageBox(pUtil->UTF8_To_UniCode(Verify.GetErrorStr(nCode)).data(), MB_ICONSTOP);
		return;
	}
	//登陆成功
	theApp.m_User = pUtil->UnicodeToUtf8(m_Edit_Card.GetBuffer());

	//  保存本地配置
	if (theApp.m_JsonInfo.SAVA)
	{
		theApp.m_JsonInfo.Card = pUtil->UnicodeToUtf8(m_Edit_Card.GetBuffer());
	}


	Login();

	VMProtectEnd();
}

void CLoginDlg::Login()
{
	VMProtectBegin(__FUNCTION__);

	auto  pUtil = std::make_shared<CACEUtil>();


	auto  pVar = Verify.CloudGetTeletVar(theApp.m_User, xorstr_("Cstrike_Login"));
	if (std::get<0>(pVar) == false)
	{
		AfxMessageBox(pUtil->UTF8_To_UniCode(std::get<1>(pVar)).data(), MB_ICONSTOP);
		return;
	}
	auto& dataVar = std::get<2>(pVar);
	std::string VarText = dataVar.at(xorstr_("variable"));
	if (lstrcmpiA(VarText.data(), xorstr_("#Check_OK")) != 0)
	{
		AfxMessageBox(xorstr_(L"登陆失败！"));
		return;
	}
	
	//检查版本更新
	std::string strNewVer;
	if (!CheckUp(strNewVer))
	{
		auto Text = fmt::format(xorstr_("{}\n{}:{}\n{}:{}"), xorstr_("当前不是最新版,请下载最新版本"), xorstr_("当前版本"), xorstr_(UP_007), xorstr_("最新版本"), strNewVer);

		auto upResult = Verify.CloudGetVersionInfo();

		if (std::get<0>(upResult) == false)
		{
			AfxMessageBox(pUtil->StringToWstring(Text).data());

			return;
		}
		auto& updataVar = std::get<2>(upResult);
		std::string VarText = updataVar.at(xorstr_("describe"));

		Text = fmt::format(xorstr_("{}:{} {}:{}\t{}\n{}"), xorstr_(u8"当前版本"), xorstr_(UP_007), xorstr_(u8"最新版本"), strNewVer, xorstr_(u8"自动下载如果失败.请到官网下载更新"), VarText);

		//std::string t = pUtil->UTF8_To_string(std::get<2>(upResult).dump());
		AfxMessageBox(pUtil->UTF8_To_UniCode(Text).data());

		RunUpdate(updataVar.at(xorstr_("url")));
		return;
	}

	CMainDlg* pDlg = new CMainDlg();
	pDlg->Create(IDD_DIALOG_MAIN, this);
	pDlg->ModifyStyleEx(0, WS_EX_APPWINDOW);//任务栏显示
	pDlg->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	pDlg->ShowWindow(SW_SHOW);
	pDlg->SetActiveWindow();

	theApp.m_pDlg->ShowWindow(SW_HIDE);

	SavaJsonInfo();

	VMProtectEnd();
}

void CLoginDlg::RunUpdate(std::string Url)
{
	CACEProcess Process;
	CACEModule  Module;

	// - 父进程名  - 保存的压缩包名 - 解压路径 - URL - 重启进程路径->(Client附加参数 -> MAC - 内网IP)
	std::string Command;
	Command.append(Module.GetModuleInfo()->AppName);
	Command.append("\t");
	Command.append(xorstr_("DownloadFile_007.zip"));
	Command.append("\t");
	//找到次级目录
	std::string UnZipPath = Module.GetModuleInfo()->ModulePath;

	for (size_t i = 0; i < 2; i++)
	{
		auto v2 = UnZipPath.rfind("\\");
		UnZipPath = UnZipPath.erase(v2, UnZipPath.length() - v2);
	}
	UnZipPath.append("\\");


	std::string Textv2 = Module.GetModuleInfo()->ModulePath;
	Textv2.append("\\");

	Command.append(UnZipPath);
	Command.append("\t");
	Command.append(Url);
	Command.append("\t");
	Command.append(Textv2);
	Command.append("\t");

	Command.append(std::to_string(GetCurrentProcessId()));
	Command.append("\t");
	auto UpdateTool = fmt::format(xorstr_("{}\\UpdateTemp.exe"), Module.GetModuleInfo()->ModulePath);
	CopyFileA(fmt::format(xorstr_("{}\\Update.exe"), Module.GetModuleInfo()->ModulePath).data(), UpdateTool.data(), FALSE);


	Process.Create(UpdateTool.data(), Command.data(), Module.GetModuleInfo()->ModulePath);
}
