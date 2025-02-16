// CDlgMidFilter.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgMidFilter.h"


// CDlgMidFilter 对话框

IMPLEMENT_DYNAMIC(CDlgMidFilter, CDialogEx)

CDlgMidFilter::CDlgMidFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MidianFilter, pParent)
	, m_iFilterH(0)
	, m_iFilterW(0)
	, m_iFilterMX(0)
	, m_iFilterMY(0)
{

}

CDlgMidFilter::~CDlgMidFilter()
{
}

void CDlgMidFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FH, m_iFilterH);
	DDX_Text(pDX, IDC_EDIT_FW, m_iFilterW);
	DDX_Text(pDX, IDC_EDIT_FMX, m_iFilterMX);
	DDX_Text(pDX, IDC_EDIT_FMY, m_iFilterMY);
}


BEGIN_MESSAGE_MAP(CDlgMidFilter, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgMidFilter::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgMidFilter::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CDlgMidFilter 消息处理程序


void CDlgMidFilter::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);

	// 判断设置是否有效
	if ((m_iFilterMX < 0) || (m_iFilterMX > m_iFilterW - 1) ||
		(m_iFilterMY < 0) || (m_iFilterMY > m_iFilterH - 1))
	{
		// 提示用户参数设置错误
		AfxMessageBox(L"参数设置错误！");

		// 返回
		return;
	}

	//退出：
	CDialogEx::OnOK();
}


void CDlgMidFilter::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	// 3×3模板
	m_iFilterH = 3;
	m_iFilterW = 3;
	m_iFilterMX = 1;
	m_iFilterMY = 1;

	// 设置文本框不可用
	(CEdit*)GetDlgItem(IDC_EDIT_FH)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT_FW)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT_FMX)->EnableWindow(FALSE);
	(CEdit*)GetDlgItem(IDC_EDIT_FMY)->EnableWindow(FALSE);

	// 更新
	UpdateData(FALSE);
}


void CDlgMidFilter::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 自定义模板
	(CEdit*)GetDlgItem(IDC_EDIT_FH)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT_FW)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT_FMX)->EnableWindow(TRUE);
	(CEdit*)GetDlgItem(IDC_EDIT_FMY)->EnableWindow(TRUE);
}
