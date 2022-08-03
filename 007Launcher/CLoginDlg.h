#pragma once
#include "afxdialogex.h"


// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_Edit_LoginUser;
	CString m_Edit_LoginUser;
	CString m_Edit_LoginPassWord;
	CButton m_Button_LoginUser;
	afx_msg void OnBnClickedButton1();
	CString m_Edit_Notic;
	virtual BOOL OnInitDialog();
	CButton m_Check_Sava;
//	CString m_Edit_Card;
	afx_msg void OnBnClickedCheck1();
	
	BOOL CheckUp(std::string & ver);


	afx_msg void OnBnClickedButton2();
	CString m_Edit_Card;

	void Login();


	void RunUpdate(std::string Url);

};
