
// 007Launcher.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#include "007LauncherDlg.h"


// CMy007LauncherApp:
// 有关此类的实现，请参阅 007Launcher.cpp
//

class CMy007LauncherApp : public CWinApp
{
public:
	CMy007LauncherApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

public:

	CString m_Notic;

	// 保存的文件信息
	nlohmann::json m_FileInfo;

	JSON_FileInfo m_JsonInfo;

	CMy007LauncherDlg * m_pDlg;

	std::string m_User;

	BOOL m_FreeYunStatus;

};

extern CMy007LauncherApp theApp;
