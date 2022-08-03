
// 007LauncherDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "007Launcher.h"
#include "007LauncherDlg.h"
#include "afxdialogex.h"
#include <ACEFile.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy007LauncherDlg 对话框



CMy007LauncherDlg::CMy007LauncherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY007LAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy007LauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB1, m_Tab);
}



void CMy007LauncherDlg::InitCfg()
{
	auto pFile = std::make_shared<CACEFile>(xorstr_("C:\\Loginkss.json"));

	HANDLE hFile = pFile->OpenFile();

	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(xorstr_(L"读取文件配置失败！请右键辅助 [以管理员身份运行]！"), MB_ICONSTOP);
		exit(0);
		return;
	}

	//读取文件
	auto is = pFile->ReadFile();

	try
	{
		// 默认是保存的
		theApp.m_JsonInfo.SAVA = 1;

		if (is.size())
		{
			theApp.m_FileInfo = nlohmann::json::parse(is.begin(), is.end());

			theApp.m_JsonInfo = theApp.m_FileInfo.get<JSON_FileInfo>();

			
		}
		else
		{
			//写入新的
			theApp.m_FileInfo = theApp.m_JsonInfo;
			auto info = theApp.m_FileInfo.dump(4);
			pFile->WriteFile(info.data(), info.size());

		}


	}
	catch (nlohmann::detail::exception& e)
	{
		AfxMessageBox(m_pACEUtil->UTF8_To_UniCode(e.what()).data(), MB_ICONSTOP);
	}

	pFile->CloseFile();
}

BEGIN_MESSAGE_MAP(CMy007LauncherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CMy007LauncherDlg 消息处理程序



BOOL CMy007LauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	VMProtectBegin(__FUNCTION__);

	theApp.m_pDlg = this;

	// TODO: 在此添加额外的初始化代码
	m_pACEUtil = std::make_shared<CACEUtil>();

	InitCfg();

	CACEHwid Hwid;
	auto pNet =  Hwid.GetActiveNetworkAdapter(FALSE);

	if (!pNet)
	{
		AfxMessageBox(xorstr_(L"信息读取失败.请检查是否有杀毒防火墙拦截"), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}
	TAG_ANTI_FREEYUN_PROXY Proxy;

	if (theApp.m_JsonInfo.Proxy.bEnable)
	{
		Proxy.IP       = theApp.m_JsonInfo.Proxy.ip;
		Proxy.Port     = theApp.m_JsonInfo.Proxy.port;
		Proxy.User     = theApp.m_JsonInfo.Proxy.user;
		Proxy.PassWord = theApp.m_JsonInfo.Proxy.password;
	}

	auto info = Verify.CloudInit(&TAG_ANTI_FREEYUN_INIT_INFO(xorstr_("7600"), xorstr_("27A5172AFA54D2F4A202EA76B4B43612"), xorstr_("COxlsBLt"), xorstr_("KzEWarV4N8"), xorstr_("9303"), pNet->GetMacAddress(), 0, Proxy));

	if (std::get<0>(info) == false)
	{
		AfxMessageBox(m_pACEUtil->UTF8_To_UniCode(std::get<1>(info)).data(), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}
	auto &data = std::get<2>(info);

	int nCode = data.at(xorstr_("code"));

	if (nCode != 1003)
	{
		AfxMessageBox(m_pACEUtil->UTF8_To_UniCode(Verify.GetErrorStr(nCode)).data(), MB_ICONSTOP);
		exit(0);
		return TRUE;
	}

	theApp.m_Notic = m_pACEUtil->UTF8_To_UniCode(data.at(xorstr_("notic"))).data();


	m_Page_Title[0] = xorstr_(L"账号|登陆");
	m_Page_Title[1] = xorstr_(L"账号|注册");
	m_Page_Title[2] = xorstr_(L"改密|充值");
	m_Tab.AddPage(m_Page_Title[0], &m_LoginPage, IDD_DIALOG_LOGIN);
	m_Tab.AddPage(m_Page_Title[1], &m_RegPage, IDD_DIALOG_REG);
	m_Tab.AddPage(m_Page_Title[2], &m_PayPage, IDD_DIALOG_PAY);


	m_Tab.Show();

	VMProtectEnd();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy007LauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy007LauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy007LauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

