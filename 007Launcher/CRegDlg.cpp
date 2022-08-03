// CRegDlg.cpp: 实现文件
//

#include "pch.h"
#include "007Launcher.h"
#include "afxdialogex.h"
#include "CRegDlg.h"


// CRegDlg 对话框

IMPLEMENT_DYNAMIC(CRegDlg, CDialogEx)

CRegDlg::CRegDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_REG, pParent)
	, m_Edit_RegUser(_T(""))
	, m_Edit_RegPassWord(_T(""))
	, m_Edit_RegEmail(_T(""))
	, m_Edit_RegCode(_T(""))
{

}

CRegDlg::~CRegDlg()
{
}

void CRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON2, m_Button_Reg);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_RegUser);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_RegPassWord);
	DDX_Text(pDX, IDC_EDIT5, m_Edit_RegEmail);
	DDX_Text(pDX, IDC_EDIT6, m_Edit_RegCode);
}


BEGIN_MESSAGE_MAP(CRegDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CRegDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CRegDlg 消息处理程序


void CRegDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);


	if (m_Edit_RegUser.IsEmpty()|| m_Edit_RegPassWord.IsEmpty() || m_Edit_RegEmail.IsEmpty())
	{
		AfxMessageBox(xorstr_(L"请把信息填写完整.账号密码邮箱缺一不可,邀请码不知道可以不填！"), MB_ICONSTOP);

		return;
	}



	auto Util = std::make_shared<CACEUtil>();

	ANTI_FREEYUN_REG RegInfo = ANTI_FREEYUN_REG(Util->UnicodeToUtf8(m_Edit_RegUser.GetBuffer()), Util->UnicodeToUtf8(m_Edit_RegPassWord.GetBuffer()));

	RegInfo.AgentCode = Util->UnicodeToUtf8(m_Edit_RegCode.GetBuffer());
	RegInfo.Email     = Util->UnicodeToUtf8(m_Edit_RegEmail.GetBuffer());


	auto result =  Verify.CloudReg(&RegInfo);

	if (std::get<0>(result))
	{

		AfxMessageBox(xorstr_(L"注册成功！"));

		return;
	}

	AfxMessageBox(Util->UTF8_To_UniCode(std::get<1>(result)).data(), MB_ICONSTOP);

}
