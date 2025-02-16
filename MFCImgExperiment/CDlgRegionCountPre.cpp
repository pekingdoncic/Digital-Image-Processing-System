// CDlgRegionCountPre.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgRegionCountPre.h"


// CDlgRegionCountPre 对话框

IMPLEMENT_DYNAMIC(CDlgRegionCountPre, CDialogEx)

CDlgRegionCountPre::CDlgRegionCountPre(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RegionCountPre, pParent)
{

}

CDlgRegionCountPre::~CDlgRegionCountPre()
{
}

void CDlgRegionCountPre::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRegionCountPre, CDialogEx)
	ON_BN_CLICKED(IDC_RADIOBlack, &CDlgRegionCountPre::OnBnClickedRadioblack)
	ON_BN_CLICKED(IDC_RADIOWhite, &CDlgRegionCountPre::OnBnClickedRadiowhite)
	ON_BN_CLICKED(IDC_RADIOAuto, &CDlgRegionCountPre::OnBnClickedRadioauto)
	ON_BN_CLICKED(IDC_RADIOEight, &CDlgRegionCountPre::OnBnClickedRadioeight)
	ON_BN_CLICKED(IDC_RADIOFour, &CDlgRegionCountPre::OnBnClickedRadiofour)
	ON_BN_CLICKED(IDC_AddColor, &CDlgRegionCountPre::OnBnClickedAddcolor)
END_MESSAGE_MAP()


// CDlgRegionCountPre 消息处理程序


void CDlgRegionCountPre::OnBnClickedRadioblack()
{
	// TODO: 在此添加控件通知处理程序代码
	m_foreground = 0;

}


void CDlgRegionCountPre::OnBnClickedRadiowhite()
{
	// TODO: 在此添加控件通知处理程序代码
	m_foreground = 1;
}


void CDlgRegionCountPre::OnBnClickedRadioauto()
{
	// TODO: 在此添加控件通知处理程序代码
	m_foreground = 2;
}


void CDlgRegionCountPre::OnBnClickedRadioeight()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_foreground == 0)
		((CButton*)GetDlgItem(IDC_RADIOBlack))->SetCheck(TRUE);
	if (m_foreground == 1)
		((CButton*)GetDlgItem(IDC_RADIOWhite))->SetCheck(TRUE);
	if (m_foreground == 2)
		((CButton*)GetDlgItem(IDC_RADIOAuto))->SetCheck(TRUE);

	m_link = 0;
	UpdateData(TRUE);
}


void CDlgRegionCountPre::OnBnClickedRadiofour()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_foreground == 0)
		((CButton*)GetDlgItem(IDC_RADIOBlack))->SetCheck(TRUE);
	if (m_foreground == 1)
		((CButton*)GetDlgItem(IDC_RADIOWhite))->SetCheck(TRUE);
	if (m_foreground == 2)
		((CButton*)GetDlgItem(IDC_RADIOAuto))->SetCheck(TRUE);

	m_link = 1;
	UpdateData(TRUE);
}

void CDlgRegionCountPre::OnBnClickedAddcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	m_forecolor = 1;
}
