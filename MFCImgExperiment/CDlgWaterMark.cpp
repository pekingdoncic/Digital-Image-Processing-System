// CDlgWaterMark.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgWaterMark.h"


// CDlgWaterMark 对话框

IMPLEMENT_DYNAMIC(CDlgWaterMark, CDialogEx)

CDlgWaterMark::CDlgWaterMark(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WaterMark, pParent)
	, num(0)
{

}

CDlgWaterMark::~CDlgWaterMark()
{
}

void CDlgWaterMark::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, num);
}


BEGIN_MESSAGE_MAP(CDlgWaterMark, CDialogEx)
END_MESSAGE_MAP()


// CDlgWaterMark 消息处理程序
