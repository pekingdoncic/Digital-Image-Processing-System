// CDlgGammaVary.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgGammaVary.h"


// CDlgGammaVary 对话框

IMPLEMENT_DYNAMIC(CDlgGammaVary, CDialogEx)

CDlgGammaVary::CDlgGammaVary(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GammaVary, pParent)
	, m_fValGamma(0)
{

}

CDlgGammaVary::~CDlgGammaVary()
{
}

void CDlgGammaVary::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Gamma, m_fValGamma);
}


BEGIN_MESSAGE_MAP(CDlgGammaVary, CDialogEx)
END_MESSAGE_MAP()


// CDlgGammaVary 消息处理程序
