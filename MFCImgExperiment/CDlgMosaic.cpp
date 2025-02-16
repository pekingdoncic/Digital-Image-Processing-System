// CDlgMosaic.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgMosaic.h"


// CDlgMosaic 对话框

IMPLEMENT_DYNAMIC(CDlgMosaic, CDialogEx)

CDlgMosaic::CDlgMosaic(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Mosaic, pParent)
	, m_rect(0)
{

}

CDlgMosaic::~CDlgMosaic()
{
}

void CDlgMosaic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_rect);
}


BEGIN_MESSAGE_MAP(CDlgMosaic, CDialogEx)
END_MESSAGE_MAP()


// CDlgMosaic 消息处理程序
