// CDlgRegionCount.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgRegionCount.h"


// CDlgRegionCount 对话框

IMPLEMENT_DYNAMIC(CDlgRegionCount, CDialogEx)

CDlgRegionCount::CDlgRegionCount(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RegionCount, pParent)
	, m_Num(0)
	, m_Area(0)
	, m_Perimeter(0)
	, m_Circle(0)
	, m_Rectangle(0)
	, m_OffsetX(0)
	, m_OffsetY(0)
{

}

CDlgRegionCount::~CDlgRegionCount()
{
}

void CDlgRegionCount::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Num);
	DDX_Text(pDX, IDC_EDIT10, m_Area);
	DDX_Text(pDX, IDC_EDIT12, m_Perimeter);
	DDX_Text(pDX, IDC_EDIT13, m_Circle);
	DDX_Text(pDX, IDC_EDIT14, m_Rectangle);
	DDX_Text(pDX, IDC_EDIT15, m_OffsetX);
	DDX_Text(pDX, IDC_EDIT6, m_OffsetY);
}


BEGIN_MESSAGE_MAP(CDlgRegionCount, CDialogEx)
END_MESSAGE_MAP()


// CDlgRegionCount 消息处理程序
