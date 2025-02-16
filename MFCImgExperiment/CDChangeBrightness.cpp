// CDChangeBrightness.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDChangeBrightness.h"


// CDChangeBrightness 对话框

IMPLEMENT_DYNAMIC(CDChangeBrightness, CDialogEx)

CDChangeBrightness::CDChangeBrightness(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ChangeBrightness, pParent)
	, m_Brightness(0)
{

}

CDChangeBrightness::~CDChangeBrightness()
{
}

void CDChangeBrightness::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ChangeBrightness, m_Brightness);
}


BEGIN_MESSAGE_MAP(CDChangeBrightness, CDialogEx)
END_MESSAGE_MAP()


// CDChangeBrightness 消息处理程序


BOOL CDChangeBrightness::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
