#pragma once
#include <CACEInterface.h>
#include "LCUICommon.h"
#include "LCUIStruct.h"
#include "Config.h"
#include "FyAuthorization.h"
#include "RCFServer.h"
#include "Util.h"

class CLCUIGui
{
public:
	CLCUIGui();
	~CLCUIGui();
	
	/*
	*	@初始化
	*/
	bool Init();
	
	/*
	*	@主线程循环UI
	*/
	int  Loop();

	// @显示初始化
	BOOL ViewInit();

	// @配置初始化
	BOOL ConfigInit();

	// @获取剪贴板内容
	std::string GetClipboardText();
private:
	// @导航栏初始化
	bool Navigation_Init();

	// @Root初始化
	bool Root_Init();

	static void Navgation_BeforeActivePoint(LCUI_Widget link, NavigationPointId id);
	static void Navgation_ActivePoint(LCUI_Widget link, NavigationPointId id);

	static void ActiveLink(LCUI_Widget parent, const char* link_id);
	static LCUI_Widget GetNavigationItem(LCUI_Widget link);
	static void OnClickNavbarLink(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	static void OnViewLoaded(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);

	// @禁止导航栏点击事件
	void Navgation_BlockEventClick();
	
	//-------------------------------------------------------------------
	// 公告初始化
	//-------------------------------------------------------------------
	static void NoticeView_Init();
	static void NoticeView_free();


	//-------------------------------------------------------------------
	// 登录显示初始化
	//-------------------------------------------------------------------
	static void LoginView_Init();
	static void LoginView_free();
	
	//用户模式登陆
	static void OnClickUserLogin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//用户卡号登陆
	static void OnClickCardLogin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//卡号编辑框点击
	static void OnclickCardNumber(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	static void OnclickPayCardNumber(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//更新
	static void OnClickUpdata(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	
	//-------------------------------------------------------------------
	// 注册显示初始化
	//-------------------------------------------------------------------
	static void RegView_Init();
	static void RegView_free();
	
	//用户注册
	static void OnClickReg(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	
	//-------------------------------------------------------------------
	// 充值显示初始化
	//-------------------------------------------------------------------
	static void PayView_Init();
	static void PayView_free();
	
	//用户充值
	static void OnClickPay(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);

	//-------------------------------------------------------------------
	// 设置显示初始化
	//-------------------------------------------------------------------
	static void SettingView_Init();
	static void SettingView_free();
	
	//下拉语言菜单
	static void OnClickDropdownLangue(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//代理选择
	static void OnClickProxy(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//设置按钮按钮保存
	static void OnClickSettingSave(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);

	std::wstring GetLangue(char* id);

	void SettingView_Config();
	
	
	//判断输入的内容合法性
	std::tuple<BOOL, std::wstring>CheckInputValidity(CheckInputType Type,std::wstring Text);

	void  To_LoginUIEdit();

	static void Ui_Thread_CloseWaitMessage(void*, void*);

	//工作线程-网络初始化
	static void worker_thread_init(void*, void*);	
	static void Ui_Thread_NetNotic(void*, void*);
	static void worker_thread_reg(void*, void*);
	static void Ui_Thread_Reg(void*, void*);
	static void worker_thread_pay(void*, void*);
	static void Ui_Thread_pay(void*, void*);
	static void worker_thread_login(void*, void*);
	static void Ui_Thread_login(void*, void*);
	static void worker_thread_card_login(void*, void*);

	//弹出更新信息提示框
	void OpenUpdataDlg(std::string strVersion, std::string strUpdateContent, std::string strUpdateUrl);
	void RunUpdate(std::string Url);
	

	//-------------------------------------------------------------------
	// 核心功能设置
	//-------------------------------------------------------------------
	static void CoreView_Init();
	static void CoreView_free();
	//选择BIN路径
	static void OnClickPathBin(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	//窗口化游戏
	static void OnClickWindowsGame(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	
	static void OnClickSelectSv(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	static void OnClickStartGame(LCUI_Widget w, LCUI_WidgetEvent e, void* arg);
	
	
	//显示配置
	void ShowCoreConfig();

	void ThreadHeat();



	//导航栏信息
	NaviInfo                m_Nav;
	inline static CLCUIGui* m_pInstance = nullptr;
	//等待消息.)
	LCUI_Widget             m_pWaitIconMessage;
	Config                  m_Config;
	FyAuthorization			m_Authorization;


	std::shared_ptr<CACEUtil> m_pUtil;

	//UI 通知的结果
	std::tuple<bool, std::string> m_UiResult;

	std::string m_UiUsername;
	std::string m_UiPassword;
	std::string m_UiCardNumber;
	std::string m_ExpireTime;
	std::string m_LoginUserName;
	CRCFServerManage	m_RCFManage;
	std::string m_Sv;

};

