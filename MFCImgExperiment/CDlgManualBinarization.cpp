// CDlgManualBinarization.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgManualBinarization.h"


// CDlgManualBinarization 对话框

IMPLEMENT_DYNAMIC(CDlgManualBinarization, CDialogEx)

CDlgManualBinarization::CDlgManualBinarization(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Manual_binarization, pParent)
	, m_offset(0)
{

}

CDlgManualBinarization::~CDlgManualBinarization()
{
}

void CDlgManualBinarization::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_offset, m_offset);
}


BEGIN_MESSAGE_MAP(CDlgManualBinarization, CDialogEx)
END_MESSAGE_MAP()


// CDlgManualBinarization 消息处理程序
