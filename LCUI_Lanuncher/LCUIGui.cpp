#include "LCUIGui.h"
#include <regex>
#include <CACEInterface.h>
#include <shlobj_core.h>
#include <sddl.h>


CLCUIGui::CLCUIGui()
{
	m_pInstance         = this;
	m_pWaitIconMessage  = nullptr;
	m_pUtil             = CACEInterface::GetInstance()->GetUtil();
}

CLCUIGui::~CLCUIGui()
{

}

bool CLCUIGui::Init()
{

	LCUI_Init();

	LCDesign_Init();
	
	LCUI_Widget pack = LCUIBuilder_LoadFile(xorstr_("./assets/views/main.xml"));
	if (!pack)
	{
		MessageBoxW(0, xorstr_(L"配置文件加载失败！请右键辅助以管路员身份运行"), 0, 0);
		return false;
	}
	LCUI_Widget root = LCUIWidget_GetRoot();

	Widget_Unwrap(pack);

	Widget_Append(root, pack);



	return true;
}

int CLCUIGui::Loop()
{
	return LCUI_Main();
}

BOOL CLCUIGui::ViewInit()
{
	Root_Init();
	Navigation_Init();
	
	return TRUE;
}


BOOL CLCUIGui::ConfigInit()
{
	m_Config.Load();
	
	return TRUE;
}

bool CLCUIGui::Navigation_Init()
{
	LCUI_Widget root = LCUIWidget_GetRoot();
	LCUI_Widget navbar = LCUIWidget_GetById(xorstr_("demo-navbar-links"));

	Widget_BindEvent(navbar, xorstr_("click"), OnClickNavbarLink, NULL, NULL);
	Widget_BindEvent(root, xorstr_("loaded.anchor"), OnViewLoaded, NULL, NULL);
	//激活默认连接-公告
	ActiveLink(navbar, "navbar-link-notice");

	
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_NOTICE)].init = NoticeView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_NOTICE)].free = NoticeView_free;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_LOGIN)].init = LoginView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_LOGIN)].free = LoginView_free;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_REG)].init = RegView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_REG)].free = RegView_free;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_PAY)].init = PayView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_PAY)].free = PayView_free;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_SETTING)].init = SettingView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_SETTING)].free = SettingView_free;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_CORE)].init = CoreView_Init;
	m_Nav.points[static_cast<int>(NavigationPointId::NAV_CORE)].free = CoreView_free;

	//Navgation_BlockEventClick();

	//隐藏导航栏-功能界面
	Widget_SetStyleString(LCUIWidget_GetById(xorstr_("navbar-link-core")) , "visibility", "hidden");

	//visibility:hidden
	return true;
}


bool CLCUIGui::Root_Init()
{
	LCUI_Widget root = LCUIWidget_GetRoot();



	//设置随机标题
	Widget_SetTitleW(root, m_pUtil->StringToWstring(m_Config.GetRandTitle()).data());
	return true;
}

LCUI_Widget CLCUIGui::GetNavigationItem(LCUI_Widget link)
{
	for (; link; link = link->parent)
	{
		if (Widget_HasClass(link, "nav-item")) 
		{
			return link;
		}
	}
	return NULL;
}
void CLCUIGui::ActiveLink(LCUI_Widget parent, const char* link_id)
{
	LCUI_Widget link;
	LCUI_WidgetEventRec ev = { 0 };

	if (!link_id) {
		return;
	}
	link = LCUIWidget_GetById(link_id);
	if (!link) {
		return;
	}
	ev.type = LCUI_WEVENT_CLICK;
	ev.button.button = 1;
	Widget_TriggerEvent(link, &ev, NULL);
}


void CLCUIGui::OnClickNavbarLink(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{

	int id;
	LCUI_Widget link;

	link = GetNavigationItem(e->target);
	auto  name = Widget_GetAttribute(link, "nav-point");
	
	ACEDebugLog("[{}] {}", __FUNCTION__, name);

	//id = Navigation_GetPointId(link);
	//if (id == NAV_NONE)
	//{
	//	return;
	//}

	

}
void CLCUIGui::Navgation_BeforeActivePoint(LCUI_Widget link, NavigationPointId id)
{

	if (m_pInstance->m_Nav.point)
	{
		m_pInstance->m_Nav.point->free();
		m_pInstance->m_Nav.point = NULL;
	}
	if (m_pInstance->m_Nav.active)
	{
		Widget_RemoveClass(m_pInstance->m_Nav.active, "active");
		m_pInstance->m_Nav.active = NULL;
	}
	Widget_AddClass(link, "active");
}

void CLCUIGui::Navgation_ActivePoint(LCUI_Widget link, NavigationPointId id)
{
	if (id == NavigationPointId::NAV_NONE)
	{
		return;
	}
	Navgation_BeforeActivePoint(link, id);
	m_pInstance->m_Nav.point = &m_pInstance->m_Nav.points[static_cast<int>(id)];
	m_pInstance->m_Nav.point->init();
	m_pInstance->m_Nav.active = link;
	Widget_AddClass(link, "active");
}
void CLCUIGui::OnViewLoaded(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	

	ACEDebugLog("[{}]", __FUNCTION__);

	const char* link_id;
	NavigationPointId id;
	LCUI_Widget link = GetNavigationItem(e->target);

	if (link)
	{
		auto name = Widget_GetAttribute(link, "nav-point");

		if (strcmp(name, xorstr_("notice")) == 0) {
			id = NavigationPointId::NAV_NOTICE;
		}
		else if (strcmp(name, xorstr_("login")) == 0) {
			id = NavigationPointId::NAV_LOGIN;
		}
		else if (strcmp(name, xorstr_("register")) == 0) {
			id = NavigationPointId::NAV_REG;
		}
		else if (strcmp(name, xorstr_("pay")) == 0) {
			id = NavigationPointId::NAV_PAY;
		}
		else if (strcmp(name, xorstr_("setting")) == 0) {
			id = NavigationPointId::NAV_SETTING;
		}
		else if (strcmp(name, xorstr_("core")) == 0) {
			id = NavigationPointId::NAV_CORE;
		}
		
	}
	Navgation_ActivePoint(link, id);
}

void CLCUIGui::Navgation_BlockEventClick()
{
	auto navbar = LCUIWidget_GetById(xorstr_("demo-navbar-links"));
	Widget_BlockEvent(navbar, LCUI_WSTATE_NORMAL);
	
}

void CLCUIGui::NoticeView_Init()
{
	ACEDebugLog("[{}]", __FUNCTION__);

	/************************************************************************/
	/* 连接服务器,                                                          */
	/************************************************************************/
	m_pInstance->m_pWaitIconMessage = LCDesign_OpenLoadingMessage(xorstr_(L"请稍后正在连接服务器..."), 0);

	//工作线程
	LCUI_TaskRec task = { 0 };
	task.func = worker_thread_init;
	LCUI_PostAsyncTask(&task);

}

void CLCUIGui::NoticeView_free()
{

}
void CLCUIGui::To_LoginUIEdit()
{
	auto wUsername   = LCUIWidget_GetById(xorstr_("login-input-username"));
	auto wPassword   = LCUIWidget_GetById(xorstr_("login-input-password"));
	auto wCardNumber = LCUIWidget_GetById(xorstr_("input-card-number"));

	TextEdit_SetText(wUsername, m_Config.GetConfig().User.data());
	TextEdit_SetText(wPassword, m_Config.GetConfig().PASW.data());
	TextEdit_SetText(wCardNumber, m_Config.GetConfig().Card.data());
}

void CLCUIGui::LoginView_Init()
{
	// 账号登陆按钮
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("login-submit")), xorstr_("click"), OnClickUserLogin, NULL, NULL);
	//卡号登陆
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("login-card-submit")), xorstr_("click"), OnClickCardLogin, NULL, NULL);
	
	//卡号编辑框
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("input-card-number")), xorstr_("click"), OnclickCardNumber, NULL, NULL);

	//配置写入到编辑框
	m_pInstance->To_LoginUIEdit();

}

void CLCUIGui::LoginView_free()
{
	//就不解除绑定了,有的部件ID获取不到.

}



void CLCUIGui::Ui_Thread_CloseWaitMessage(void*, void*)
{
	LCDesign_CloseMessage(m_pInstance->m_pWaitIconMessage);
	m_pInstance->m_pWaitIconMessage = nullptr;
}

void CLCUIGui::worker_thread_init(void*, void*)
{
	//连接服务器初始化
	m_pInstance->m_Authorization.Init();
	
	//通知UI线程
	LCUI_PostSimpleTask(Ui_Thread_NetNotic, nullptr, nullptr);
}

void CLCUIGui::Ui_Thread_NetNotic(void*, void*)
{
	//get 网络公告
	auto result  = m_pInstance->m_Authorization.GetNotic();
	auto Status  =  std::get<0>(result);
	auto Text    =	std::get<1>(result);
	
	//关闭等待框
	Ui_Thread_CloseWaitMessage(nullptr,nullptr);
	
	if (!Status)
	{
		//弹出错误框.
		LCDesign_OpenErrorMessage(m_pInstance->m_pUtil->UTF8_To_UniCode(Text).data(), 2000);
		
		return ;
	}

	LCDesign_OpenSuccessNotification(xorstr_(L"Notification Title"), m_pInstance->m_pUtil->UTF8_To_UniCode(Text).data(), NULL, 1000*10);

}

void CLCUIGui::worker_thread_reg(void* pointer, void*)
{
	auto pInfo = (Register*)pointer;

	//注册用户
	m_pInstance->m_UiResult = m_pInstance->m_Authorization.Register(m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->wTextUser), m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->wTextPass), m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->wTextEmail));

	//通知UI线程
	LCUI_PostSimpleTask(Ui_Thread_Reg, nullptr, nullptr);
}

void CLCUIGui::Ui_Thread_Reg(void*, void*)
{
	//关闭等待框
	Ui_Thread_CloseWaitMessage(nullptr, nullptr);

	if (std::get<0>(m_pInstance->m_UiResult))
	{
		//注册成功
		LCDesign_OpenSuccessNotification(xorstr_(L"注册成功"), L"", NULL, 1000 * 10);
		//跳转到登陆界面
		//Navigation_ActivePoint(NavigationPointId::NAV_LOGIN);
	}
	else
	{
		//注册失败
		LCDesign_OpenErrorMessage(m_pInstance->m_pUtil->UTF8_To_UniCode(std::get<1>(m_pInstance->m_UiResult)).data(), 2000);
	}
}

void CLCUIGui::worker_thread_pay(void* pointer, void*)
{
	auto pInfo = (UserPay*)pointer;


	m_pInstance->m_UiResult = m_pInstance->m_Authorization.UserPay(m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->wTextUser), m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->wCardNumber));

	//通知UI线程
	LCUI_PostSimpleTask(Ui_Thread_pay, nullptr, nullptr);
}

void CLCUIGui::Ui_Thread_pay(void*, void*)
{
	//关闭等待框
	Ui_Thread_CloseWaitMessage(nullptr, nullptr);

	if (std::get<0>(m_pInstance->m_UiResult))
	{
		LCDesign_OpenSuccessMessage(xorstr_(L"充值成功！"), 2000);
	}
	else
	{
		//失败
		LCDesign_OpenErrorMessage(m_pInstance->m_pUtil->UTF8_To_UniCode(std::get<1>(m_pInstance->m_UiResult)).data(), 2000);
	}
}

void CLCUIGui::worker_thread_login(void* pointer, void*)
{
	auto pInfo = (MutualInfo*)pointer;
	
	//保存UI账号和密码
	m_pInstance->m_UiUsername = m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->Login.wUser);
	m_pInstance->m_UiPassword = m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->Login.wPassWord);
	
	m_pInstance->m_UiResult = m_pInstance->m_Authorization.UserLogin(m_pInstance->m_UiUsername, m_pInstance->m_UiPassword);

	//通知UI线程
	LCUI_PostSimpleTask(Ui_Thread_login, 1, nullptr);
}

void CLCUIGui::Ui_Thread_login(void* Type, void*)
{
	VMProtectBegin(__FUNCTION__);
	
	if (std::get<0>(m_pInstance->m_UiResult))
	{


		//保存配置
		BYTE bType = (BYTE)(Type);

		if (bType == 1)
		{
			m_pInstance->m_Config.GetConfig().User = m_pInstance->m_UiUsername;
			m_pInstance->m_Config.GetConfig().PASW = m_pInstance->m_UiPassword;
			m_pInstance->m_LoginUserName = m_pInstance->m_UiUsername;
		}
		else
		{
			m_pInstance->m_Config.GetConfig().Card = m_pInstance->m_UiCardNumber;
			m_pInstance->m_LoginUserName = m_pInstance->m_UiCardNumber;
		}

		m_pInstance->m_Authorization.SetUserName(m_pInstance->m_LoginUserName);
		
		m_pInstance->m_Config.Save();

		//-------------------------------------------------------------------
		// 校检远程变量
		//-------------------------------------------------------------------
		m_pInstance->m_UiResult = m_pInstance->m_Authorization.VerifyRemoteVariable(m_pInstance->m_LoginUserName);

		if (std::get<0>(m_pInstance->m_UiResult) == false)
			goto lab_error;
		
		m_pInstance->m_ExpireTime = std::get<1>(m_pInstance->m_Authorization.GetExpireTime(m_pInstance->m_LoginUserName));


		//检查版本更新
		std::string strVersion;
		m_pInstance->m_UiResult = m_pInstance->m_Authorization.CheckVersion(m_pInstance->m_LoginUserName, strVersion);
		
		if (std::get<0>(m_pInstance->m_UiResult) == false && strVersion.empty())
			goto lab_error;

		if (strVersion.empty()== false)
		{
			auto Updata = m_pInstance->m_Authorization.GetUpdateContent();

			if (std::get<0>(Updata) == false)
			{
				m_pInstance->m_UiResult = std::make_tuple(false, std::get<1>(Updata));
				goto lab_error;
			}
			m_pInstance->OpenUpdataDlg(strVersion, std::get<1>(Updata),std::get<2>(Updata));
			
			return;
		}


		//成功登陆
		
		Ui_Thread_CloseWaitMessage(nullptr, nullptr);
		
		m_pInstance->Navgation_BlockEventClick();
		
		LCUI_Widget navbar = LCUIWidget_GetById(xorstr_("demo-navbar-links"));
		//激活功能
		ActiveLink(navbar, xorstr_("navbar-link-core"));

		return ;
	}
lab_error:	
	//关闭等待框
	Ui_Thread_CloseWaitMessage(nullptr, nullptr);
	//失败
	LCDesign_OpenErrorMessage(m_pInstance->m_pUtil->UTF8_To_UniCode(std::get<1>(m_pInstance->m_UiResult)).data(), 2000);

	VMProtectEnd();
}

void CLCUIGui::worker_thread_card_login(void* pointer, void*)
{
	auto pInfo = (MutualInfo*)pointer;

	//保存卡号
	m_pInstance->m_UiCardNumber = m_pInstance->m_pUtil->UnicodeToUtf8(pInfo->Login.wUser);

	m_pInstance->m_UiResult = m_pInstance->m_Authorization.CardLogin(m_pInstance->m_UiCardNumber);

	//通知UI线程
	LCUI_PostSimpleTask(Ui_Thread_login, 2, nullptr);
}

void CLCUIGui::OpenUpdataDlg(std::string strVersion, std::string strUpdateContent, std::string strUpdateUrl)
{
	Ui_Thread_CloseWaitMessage(nullptr, nullptr);
	
	//设置UI更新标签
	TextView_SetText(LCUIWidget_GetById(xorstr_("updata-text")), strUpdateContent.data());

	auto UpVersion = fmt::format(xorstr_(u8"当前版本:{} 最新版本:{}"), UP_007, strVersion);

	TextView_SetText(LCUIWidget_GetById(xorstr_("updata-version")), UpVersion.data());


	//绑定在线更新按钮事件
	static int UpdataEvent = 0;
	if (!UpdataEvent)
	{
		static std::string Url = strUpdateUrl;
		UpdataEvent = Widget_BindEvent(LCUIWidget_GetById(xorstr_("updata-btn")), xorstr_("click"), OnClickUpdata, &Url, nullptr);
	}
	//版本升级
	ActiveLink(nullptr, "updata-trigger");
}

void CLCUIGui::OnClickUserLogin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	//获取账号密码
	auto wUsername = LCUIWidget_GetById(xorstr_("login-input-username"));
	auto wPassword = LCUIWidget_GetById(xorstr_("login-input-password"));

	MutualInfo* pInfo = new MutualInfo;

	TextEdit_GetTextW(wUsername, 0, 1024, pInfo->Login.wUser);
	TextEdit_GetTextW(wPassword, 0, 1024, pInfo->Login.wPassWord);

	//校检输入的字符合法性
	auto result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_USERNAME, pInfo->Login.wUser);
	if (std::get<0>(result)== FALSE)
	{
		ACEWarningFileLogW(L"[username] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(),2000);
		return;
	}
	result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_PASSWORD, pInfo->Login.wPassWord);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[password] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	if (m_pInstance->m_pWaitIconMessage == nullptr)
	{
		m_pInstance->m_pWaitIconMessage = LCDesign_OpenLoadingMessage(xorstr_(L"Please Wait..."), 0);
	}
	else
	{
		LCDesign_OpenWarningMessage(xorstr_(L"请勿频繁操作！"), 1000);
		return;
	}
	//登陆线程
	LCUI_TaskRec task = { 0 };
	task.func = worker_thread_login;
	task.arg[0] = pInfo;
	LCUI_PostAsyncTask(&task);
}

void CLCUIGui::OnClickCardLogin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	//获取卡号
	auto wCard = LCUIWidget_GetById(xorstr_("input-card-number"));
	MutualInfo* pInfo = new MutualInfo;
	TextEdit_GetTextW(wCard, 0, 1024, pInfo->Login.wUser);
	
	
	auto result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_CARDNUMBER, pInfo->Login.wUser);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[CardNumber] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	if (m_pInstance->m_pWaitIconMessage == nullptr)
	{
		m_pInstance->m_pWaitIconMessage = LCDesign_OpenLoadingMessage(xorstr_(L"Please Wait..."), 0);
	}
	else
	{
		LCDesign_OpenWarningMessage(xorstr_(L"请勿频繁操作！"), 1000);
		return;
	}

	//登陆线程
	LCUI_TaskRec task = { 0 };
	task.func = worker_thread_card_login;
	task.arg[0] = pInfo;
	LCUI_PostAsyncTask(&task);

}

std::string CLCUIGui::GetClipboardText()
{
	std::string result;
	
	if (OpenClipboard(NULL))//打开剪贴板  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
		{
			HANDLE hClip;
			//读取数据  
			hClip = GetClipboardData(CF_TEXT);
			result = (char*)GlobalLock(hClip);
			GlobalUnlock(hClip);
			CloseClipboard();
		}
	}
	return result;
}


void CLCUIGui::OnclickCardNumber(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	auto Result = m_pInstance->GetClipboardText();

	if (Result.length() >= 25 && Result.length() <= 40)
	{
		//卡号序列
		auto w = LCUIWidget_GetById(xorstr_("input-card-number"));
		TextEdit_SetText(w, Result.data());
	}

}

void CLCUIGui::OnclickPayCardNumber(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	auto Result = m_pInstance->GetClipboardText();

	if (Result.length() >= 25 && Result.length() <= 40)
	{
		//卡号序列
		auto w = LCUIWidget_GetById(xorstr_("input-pay-card"));
		TextEdit_SetText(w, Result.data());
	}
}

void CLCUIGui::OnClickUpdata(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	std::string* Url = static_cast<std::string*>(e->data);

	ACEDebugLog("{}", Url->c_str());

	m_pInstance->RunUpdate(Url->c_str());
}

void CLCUIGui::OnClickReg(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	//获取注册账号密码邮箱
	auto wUsername = LCUIWidget_GetById(xorstr_("input-reg-username"));
	auto wPassword = LCUIWidget_GetById(xorstr_("input-reg-password"));
	auto wEmail    = LCUIWidget_GetById(xorstr_("input-reg-email"));

	Register *pRegInfo =  new Register();

	TextEdit_GetTextW(wUsername, 0, 1024, pRegInfo->wTextUser);
	TextEdit_GetTextW(wPassword, 0, 1024, pRegInfo->wTextPass);
	TextEdit_GetTextW(wEmail, 0, 1024, pRegInfo->wTextEmail);

	//校检输入的字符合法性
	auto result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_REG_USERNAME, pRegInfo->wTextUser);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[reg username] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_REG_PASSWORD, pRegInfo->wTextPass);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[reg password] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_REG_EMAIL, pRegInfo->wTextEmail);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[reg emial] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	
	if (m_pInstance->m_pWaitIconMessage == nullptr)
	{
		m_pInstance->m_pWaitIconMessage = LCDesign_OpenLoadingMessage(xorstr_(L"Please Wait..."), 0);
	}
	else
	{
		LCDesign_OpenWarningMessage(xorstr_(L"请勿频繁操作！"), 1000);
		return;
	}
	
	LCUI_TaskRec task = { 0 };
	task.func = worker_thread_reg;
	task.arg[0] = pRegInfo;
	// 没有进行内存释放
	//task.destroy_arg[0] = free;
	LCUI_PostAsyncTask(&task);
}

void CLCUIGui::OnClickPay(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	//获取充值的账号卡密
	auto wUsername   = LCUIWidget_GetById(xorstr_("input-pay-user"));
	auto wCradNumber = LCUIWidget_GetById(xorstr_("input-pay-card"));
	

	UserPay* pPayInfo = new UserPay();

	TextEdit_GetTextW(wUsername, 0, 1024, pPayInfo->wTextUser);
	TextEdit_GetTextW(wCradNumber, 0, 1024, pPayInfo->wCardNumber);

	//校检输入的字符合法性
	auto result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_REG_USERNAME, pPayInfo->wTextUser);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[pay username] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}
	result = m_pInstance->CheckInputValidity(CheckInputType::INPUT_CARDNUMBER, pPayInfo->wCardNumber);
	if (std::get<0>(result) == FALSE)
	{
		ACEWarningFileLogW(L"[pay cardnumber] {}", std::get<1>(result));
		LCDesign_OpenErrorMessage(std::get<1>(result).data(), 2000);
		return;
	}

	if (m_pInstance->m_pWaitIconMessage == nullptr)
	{
		m_pInstance->m_pWaitIconMessage = LCDesign_OpenLoadingMessage(xorstr_(L"Please Wait..."), 0);
	}
	else
	{
		LCDesign_OpenWarningMessage(xorstr_(L"请勿频繁操作！"), 1000);
		return;
	}
	


	LCUI_TaskRec task = { 0 };
	task.func = worker_thread_pay;
	task.arg[0] = pPayInfo;

	LCUI_PostAsyncTask(&task);
}


void CLCUIGui::RegView_Init()
{
	// 注册按钮
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("reg-submit")), xorstr_("click"), OnClickReg, NULL, NULL);
}

void CLCUIGui::RegView_free()
{

}

void CLCUIGui::PayView_Init()
{
	// 充值按钮
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("pay-submit")), xorstr_("click"), OnClickPay, NULL, NULL);
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("input-pay-card")), xorstr_("click"), OnclickPayCardNumber, NULL, NULL);
}

void CLCUIGui::PayView_free()
{
	
}
void CLCUIGui::OnClickDropdownLangue(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	if (Widget_HasClass(e->target, "dropdown-item"))
	{
		auto wlangue = LCUIWidget_GetById(xorstr_("languages-set"));

		auto TextLangue =  m_pInstance->GetLangue(e->target->id);

		if (TextLangue.empty())
		{
			//没有选择语言.
			return;
		}
		//显示到控件
		TextView_SetTextW(wlangue, TextLangue.data());

		//保存到配置文件
		m_pInstance->m_Config.GetConfig().Translate.language = e->target->id;
	}



}

void CLCUIGui::OnClickProxy(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	auto SwitchStatus = LCUIWidget_GetById(xorstr_("proxy-set"));
	auto IpAddress    = LCUIWidget_GetById(xorstr_("input-setting-proxyr"));
	auto Port         = LCUIWidget_GetById(xorstr_("input-setting-proxyrport"));
	
	auto  name = Widget_GetAttribute(SwitchStatus, xorstr_("checked"));

	BOOL ProxyStatus = FALSE;

	if (strcmp(name,xorstr_("unchecked"))== 0)
	{
		Widget_SetAttribute(SwitchStatus, xorstr_("checked"), xorstr_("checked"));
		ProxyStatus = TRUE;
	}
	else if (strcmp(name, xorstr_("checked")) == 0)
	{
		Widget_SetAttribute(SwitchStatus, xorstr_("checked"), xorstr_("unchecked"));
		ProxyStatus = FALSE;
	}
	Widget_SetDisabled(IpAddress, !ProxyStatus);
	Widget_SetDisabled(Port, !ProxyStatus);
	
	LCDesign_OpenWarningMessage(xorstr_(L"设置完毕,请重启软件生效!如不清楚功能请不要乱设置！"), 3500);

}

void CLCUIGui::OnClickSettingSave(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	auto SwitchStatus = LCUIWidget_GetById(xorstr_("proxy-set"));
	auto IpAddress    = LCUIWidget_GetById(xorstr_("input-setting-proxyr"));
	auto Port         = LCUIWidget_GetById(xorstr_("input-setting-proxyrport"));
	auto name         = Widget_GetAttribute(SwitchStatus, xorstr_("checked"));
	
	//状态
	m_pInstance->m_Config.GetConfig().Proxy.bEnable = strcmp(name, xorstr_("checked")) == 0;

	wchar_t wIp[1024]   = { 0 };
	wchar_t wPort[1024] = { 0 };
	TextEdit_GetTextW(IpAddress, 0, 1024, wIp);
	TextEdit_GetTextW(Port, 0, 1024, wPort);

	m_pInstance->m_Config.GetConfig().Proxy.ip = m_pInstance->m_pUtil->UnicodeToUtf8(wIp);
	m_pInstance->m_Config.GetConfig().Proxy.port = m_pInstance->m_pUtil->UnicodeToUtf8(wPort);

	m_pInstance->m_Config.Save();

	LCDesign_OpenSuccessMessage(xorstr_(L"保存完毕!"), 3500);
	
}

void CLCUIGui::SettingView_Init()
{
	//绑定下拉菜单语言
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("dropdown-languages")), xorstr_("click"), OnClickDropdownLangue, NULL, NULL);
	//绑定proxy
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("proxy-set")), xorstr_("click"), OnClickProxy, NULL, NULL);

	//bind setting save event
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("setting-sava")), xorstr_("click"), OnClickSettingSave, NULL, NULL);
	
	//显示配置
	m_pInstance->SettingView_Config();
}

void CLCUIGui::SettingView_free()
{
	//保存配置
	m_pInstance->m_Config.Save();
}

std::wstring CLCUIGui::GetLangue(char* id)
{
	std::wstring Text;
	
	if (strcmp(id, xorstr_("zh-cn")) == 0)
	{
		Text = xorstr_(L"简体中文");
	}
	else if (strcmp(id, xorstr_("zh-tw")) == 0)
	{
		Text = xorstr_(L"繁体中文");
	}
	else if (strcmp(id, xorstr_("en-US")) == 0)
	{
		Text = xorstr_(L"English");
	}
	return Text;
}

void CLCUIGui::SettingView_Config()
{
	auto wlangue = LCUIWidget_GetById(xorstr_("languages-set"));

	auto TextLangue = GetLangue(m_Config.GetConfig().Translate.language.data());

	if (TextLangue.empty() == false)
	{
		//显示到控件
		TextView_SetTextW(wlangue, TextLangue.data());
	}
	
	auto SwitchStatus = LCUIWidget_GetById(xorstr_("proxy-set"));
	auto IpAddress    = LCUIWidget_GetById(xorstr_("input-setting-proxyr"));
	auto Port         = LCUIWidget_GetById(xorstr_("input-setting-proxyrport"));


	//设置状态
	if (m_Config.GetConfig().Proxy.bEnable)
	{
		Widget_SetAttribute(SwitchStatus, xorstr_("checked"), xorstr_("checked"));
	}
	else
	{
		Widget_SetAttribute(SwitchStatus, xorstr_("checked"), xorstr_("unchecked"));
	}

	//设置是否禁用
	Widget_SetDisabled(IpAddress, !m_Config.GetConfig().Proxy.bEnable);
	Widget_SetDisabled(Port, !m_Config.GetConfig().Proxy.bEnable);
	
	//设置内容
	TextEdit_SetText(IpAddress, m_Config.GetConfig().Proxy.ip.data());
	TextEdit_SetText(Port, m_Config.GetConfig().Proxy.port.data());

}

std::tuple<BOOL, std::wstring> CLCUIGui::CheckInputValidity(CheckInputType Type, std::wstring wsText)
{
	
#if 1
	std::string Text = CACEInterface::GetInstance()->GetUtil()->WstringToString(wsText);
	
	std::regex r(xorstr_("^[a-zA-Z0-9]{4,18}$"));
	
	switch (Type)
	{
	case CheckInputType::INPUT_USERNAME:
	{
		
		bool bValid = std::regex_match(Text, r);

		if (wsText.empty())
		{
			return{ bValid,xorstr_(L"请输入账号") };
		}
		return{ bValid,xorstr_(L"账号格式错误\n请输入4-18位 字母+ 数字 (不能输入空格中文及其他非法字符！)")};
	}
	break;
	case CheckInputType::INPUT_PASSWORD:
	case CheckInputType::INPUT_REG_PASSWORD:

	{
		bool bValid = std::regex_match(Text, r);
		
		if (wsText.empty())
		{
			return{ bValid,xorstr_(L"请输入密码") };
		}
		return{ bValid,xorstr_(L"密码格式错误\n请输入4-18位 字母+ 数字 (不能输入空格中文及其他非法字符！)") };
	}
	case CheckInputType::INPUT_REG_USERNAME:
	{

		bool bValid = std::regex_match(Text, r);

		if (wsText.empty())
		{
			return{ bValid,xorstr_(L"请输入要注册的账号") };
		}
		return{ bValid,xorstr_(L"账号格式错误\n请输入4-18位 字母+ 数字 (不能输入空格中文及其他非法字符！)") };
	}
	case CheckInputType::INPUT_REG_EMAIL:
	{

		r = (xorstr_("^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$"));
		bool bValid = std::regex_match(Text, r);

		if (wsText.empty())
		{
			return{ bValid,xorstr_(L"请输入Email地址") };
		}
		return{ bValid,xorstr_(L"邮箱格式错误\n请输入正确的邮箱地址！") };
	}
	case CheckInputType::INPUT_CARDNUMBER:
	{

		r = (xorstr_("^[a-zA-Z0-9]{25,40}$"));
			bool bValid = std::regex_match(Text, r);;

		if (wsText.empty())
		{
			return{ bValid,xorstr_(L"请输入卡号") };
		}
		return{ bValid,xorstr_(L"卡号格式错误,请检查是否有空格") };
	}

	default:
		break;
	}
#endif
	return
		{
		TRUE,
		wsText
	};
}
void CLCUIGui::RunUpdate(std::string Url)
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

void CLCUIGui::CoreView_Init()
{
	VMProtectBegin(__FUNCTION__);

	if (!m_pInstance->m_RCFManage.Init())
	{
		LCDesign_OpenErrorMessage(m_pInstance->m_pUtil->UTF8_To_UniCode(m_pInstance->m_RCFManage.GetStrError()).data(), 5000);
		return ;
	}

	auto hModule = LoadLibraryA(xorstr_("007Helper.dll"));

	if (!hModule)
	{
		LCDesign_OpenErrorMessage(xorstr_(L"模块加载失败！请关闭杀毒且重新下载软件！"), 5000);
		return;
	}

	std::thread th(&CLCUIGui::ThreadHeat, m_pInstance);
	th.detach();

	// Bin目录
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("btn-gamepath-bin")), xorstr_("click"), OnClickPathBin, NULL, NULL);

	//窗口化游戏
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("windows-game")), xorstr_("click"), OnClickWindowsGame, NULL, NULL);

	//
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("radio-sv-select")), xorstr_("click"), OnClickSelectSv, NULL, NULL);

	//绑定启动游戏按钮
	Widget_BindEvent(LCUIWidget_GetById(xorstr_("btn-start-game")), xorstr_("click"), OnClickStartGame, NULL, NULL);

	
	//显示配置
	m_pInstance->ShowCoreConfig();

	VMProtectEnd();
}

void CLCUIGui::CoreView_free()
{
	
}

void CLCUIGui::OnClickPathBin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	TCHAR szFolderPath[MAX_PATH] = { 0 };

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
		
		LCDesign_OpenErrorMessage(xorstr_(L"Bin目录设置错误!不会找目录请到官网看教程！"), 2000);
		return;
	}
	File.CloseFile();

	m_pInstance->m_Config.GetConfig().GamePath = Util.UnicodeToUtf8(szFolderPath);

	TextEdit_SetTextW(LCUIWidget_GetById(xorstr_("edit-bin")), szFolderPath);
	
	m_pInstance->m_Config.Save();
}

void CLCUIGui::OnClickWindowsGame(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	static int Status = 1;


	g_Util.SetRegeditValue(xorstr_("Software\\Nexon\\CStrike-Online\\Settings"),"ScreenWindowed", Status);

	LCDesign_OpenInfoMessage(Status ? xorstr_(L"窗口化完毕") : xorstr_(L"全屏游戏完毕") , 2000);

	Status ^= 1;
}

void CLCUIGui::OnClickSelectSv(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	if (e->target->id != nullptr)
	{


	}
	else
	{
		// id = nullptr 查找父部件
		
		auto parent = e->target->parent;
		
		if (parent)
		{
			auto  name = Widget_GetAttribute(parent, xorstr_("value"));
			
			if (name)
			{
				m_pInstance->m_Sv = name;

			}

		}
	}
	
	

}

void CLCUIGui::OnClickStartGame(LCUI_Widget w, LCUI_WidgetEvent e, void* arg)
{
	
	StartGameType Region = StartGameType::Launcher;
	CACEProcess Process;
	std::string GameApp;
	
	g_Util.ClearRegedit(m_pInstance->m_Config.GetConfig().GamePath);


	ULONG ulGameServerIndex = 0;
	//-------------------------------------------------------------------
	//  1 << 0 CN
	//	1 << 1 TW
	//  1 << 2 KR
	//	1 << 3 NS
	//-------------------------------------------------------------------
	ULONG uVersion = 0;
	

	if (strcmp(m_pInstance->m_Sv.data(),xorstr_("sv-d1")) == 0)
	{
		Region = StartGameType::CN_D1;
		ulGameServerIndex = 0;
		uVersion |= 1 << 0;
	}
	else if (strcmp(m_pInstance->m_Sv.data(), xorstr_("sv-d2")) == 0 )
	{
		Region = StartGameType::CN_D2;
		ulGameServerIndex = 1;
		uVersion |= 1 << 0;
	}
	else if(strcmp(m_pInstance->m_Sv.data(), xorstr_("sv-w1")) == 0 )
	{
		Region = StartGameType::CN_W1;
		ulGameServerIndex = 2;
		uVersion |= 1 << 0;
	}
	else if (strcmp(m_pInstance->m_Sv.data(), xorstr_("sv-tw")) == 0)
	{
		Region = StartGameType::Launcher;
		ulGameServerIndex = 3;
		uVersion |= 1 << 1;
	}
	else if (strcmp(m_pInstance->m_Sv.data(), xorstr_("sv-kor")) == 0 )
	{
		Region = StartGameType::Launcher;
		ulGameServerIndex = 4;
		uVersion |= 1 << 2;
	}
	else if (strcmp(m_pInstance->m_Sv.data(), xorstr_("sv-csns")) == 0)
	{
		Region = StartGameType::Steam;
		ulGameServerIndex = 5;
		uVersion |= 1 << 3;
	}
	m_pInstance->m_Config.GetConfig().GameServer = ulGameServerIndex;

	if (m_pInstance->m_Config.GetConfig().GamePath.empty())
	{
		LCDesign_OpenErrorMessage(xorstr_(L"请先设置游戏目录"), 2000);
		return;
	}

	//检查是否支持当前服务器
	auto pVar = m_pInstance->m_Authorization.RemoteVariable(xorstr_("CS_GameServerVersion"));

	if (std::get<0>(pVar))
	{
		std::string VarText = std::get<1>(pVar);

		if ((std::stoi(VarText) & uVersion) == FALSE)
		{
			
			LCDesign_OpenErrorNotification(xorstr_(L"Notification Title"), xorstr_(L"暂时不支持当前服务器启动 ,请关注官网通知 \nThe current server startup is not supported, please pay attention to the official website notice"), NULL, 1000 * 10);

			return;
		}
	}
	else
	{

		LCDesign_OpenErrorMessage(xorstr_(L"获取远程版本号超时.请检查网络连接"), 2000);
		return;
	}

	//-------------------------------------------------------------------
	// 启动游戏检查
	//-------------------------------------------------------------------
	


	if (Region != StartGameType::Launcher && Region != StartGameType::Steam)
	{
		//不用启动器启动
		GameApp = fmt::format(xorstr_("{}\\cstrike-online.exe"), m_pInstance->m_Config.GetConfig().GamePath);
		g_Util.CsoLauncher(static_cast<int>(Region));
	}
	else if (Region == StartGameType::Launcher)
	{
		GameApp = fmt::format(xorstr_("{}\\CSOLauncher.exe"), m_pInstance->m_Config.GetConfig().GamePath);
	}
	else if (Region == StartGameType::Steam)
	{
		auto SteamPath = [&]()->BOOL
		{
			TCHAR szFolderPath[MAX_PATH] = { 0 };

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
				LCDesign_OpenErrorMessage(xorstr_(L"Steam路径设置错误.请注意是Steam路径不是游戏路径"), 2000);
	
				return FALSE;
			}
			File.CloseFile();

			m_pInstance->m_Config.GetConfig().SteamPath = Util.UnicodeToUtf8(szFolderPath);

			return TRUE;
		};

		if (ULONG ulPid = Process.GetProcessId(xorstr_("steam.exe")))
		{
			Process.KillProcess(ulPid);
		}

		if (m_pInstance->m_Config.GetConfig().SteamPath.empty())
		{
			if (!SteamPath())
			{
				return;
			}
			
		}
		GameApp = fmt::format(xorstr_("{}\\steam.exe"), m_pInstance->m_Config.GetConfig().SteamPath);
	}


	STARTUPINFOA			StartInfo = { sizeof(StartInfo) };
	PROCESS_INFORMATION		ProcessInfo = { 0 };

	CACEUtil Util;
	BOOL	nRet = CreateProcessA(Util.UTF8_To_string(GameApp).data(), "", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, Util.UTF8_To_string(m_pInstance->m_Config.GetConfig().GamePath).data(), &StartInfo, &ProcessInfo);


	m_pInstance->m_Config.Save();
}

void CLCUIGui::ShowCoreConfig()
{
	if (m_pInstance->m_Config.GetConfig().GamePath.empty() ==false)
	{
		TextEdit_SetText(LCUIWidget_GetById(xorstr_("edit-bin")), m_pInstance->m_Config.GetConfig().GamePath.data());
	}

	if (m_pInstance->m_ExpireTime.empty()==false)
	{
		TextView_SetText(LCUIWidget_GetById(xorstr_("core-user-information")), m_pInstance->m_ExpireTime.data());
	}

	auto SelectText = fmt::format(xorstr_("sv-select-{}"), m_pInstance->m_Config.GetConfig().GameServer + 1);
	Widget_SetAttribute(LCUIWidget_GetById(SelectText.data()), "checked", "checked");
	
	switch (m_pInstance->m_Config.GetConfig().GameServer)
	{
	case 0:
		m_pInstance->m_Sv = xorstr_("sv-d1");
		break;
	case 1:
		m_pInstance->m_Sv = xorstr_("sv-d2");
		break;
	case 2:
		m_pInstance->m_Sv = xorstr_("sv-w1");
		break;
	case 3:
		m_pInstance->m_Sv = xorstr_("sv-tw");
		break;
	case 4:
		m_pInstance->m_Sv = xorstr_("sv-kor");
		break;
	case 5:
		m_pInstance->m_Sv = xorstr_("sv-csns");
		break;
	default:
		break;
	}

}

void CLCUIGui::ThreadHeat()
{
	VMProtectBegin(__FUNCTION__);

	auto pUtil = std::make_shared<CACEUtil>();

	while (true)
	{
		auto pResult = m_Authorization.GetFreeYun()->CloudHeartBeat(m_pInstance->m_LoginUserName);

		auto FreeYunStatus = std::get<0>(pResult);

		if (!FreeYunStatus)
		{
			m_RCFManage.SendAllClientMsg(std::get<1>(pResult));

			LCDesign_OpenErrorMessage(pUtil->UTF8_To_UniCode(std::get<1>(pResult)).data(), 0);
		}
		else
		{
			ACEDebugLog("[{}] Ok", __FUNCTION__);
		}
		Sleep(60 * 1000 * 5);
	}
	VMProtectEnd();
}



