// CDlgFilter.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgFilter.h"


// CDlgFilter 对话框

IMPLEMENT_DYNAMIC(CDlgFilter, CDialogEx)

CDlgFilter::CDlgFilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Filter, pParent)
{

}

CDlgFilter::~CDlgFilter()
{
}

void CDlgFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFilter, CDialogEx)
END_MESSAGE_MAP()


// CDlgFilter 消息处理程序
