// CPayDlg.cpp: 实现文件
//

#include "pch.h"
#include "007Launcher.h"
#include "afxdialogex.h"
#include "CPayDlg.h"


// CPayDlg 对话框

IMPLEMENT_DYNAMIC(CPayDlg, CDialogEx)

CPayDlg::CPayDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_PAY, pParent)
	, m_Edit_PayUser(_T(""))
	, m_Edit_PayCard(_T(""))
{

}

CPayDlg::~CPayDlg()
{
}

void CPayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_Edit_PayUser);
	//  DDX_Control(pDX, IDC_EDIT2, m_Edit_PayCard);
	DDX_Text(pDX, IDC_EDIT1, m_Edit_PayUser);
	DDX_Text(pDX, IDC_EDIT2, m_Edit_PayCard);
}


BEGIN_MESSAGE_MAP(CPayDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CPayDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPayDlg 消息处理程序


void CPayDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);


	if (m_Edit_PayCard.IsEmpty() || m_Edit_PayUser.IsEmpty())
	{
		AfxMessageBox(xorstr_(L"请把信息填写完整.充值账号与卡号缺一不可"), MB_ICONSTOP);

		return;
	}

	auto Util = std::make_shared<CACEUtil>();

	auto result = Verify.CloudPay(Util->UnicodeToUtf8(m_Edit_PayUser.GetBuffer()), Util->UnicodeToUtf8(m_Edit_PayCard.GetBuffer()));

	if (std::get<0>(result))
	{

		AfxMessageBox(xorstr_(L"充值成功！"));

		return;
	}

	AfxMessageBox(Util->UTF8_To_UniCode(std::get<1>(result)).data(), MB_ICONSTOP);
}
