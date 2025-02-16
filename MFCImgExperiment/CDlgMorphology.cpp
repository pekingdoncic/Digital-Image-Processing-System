// CDlgMorphology.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgMorphology.h"


// CDlgMorphology 对话框

IMPLEMENT_DYNAMIC(CDlgMorphology, CDialogEx)

CDlgMorphology::CDlgMorphology(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_Morphology, pParent)
	, m_Type(0)
	, m_nMode(0)
	, m_nStructure1(0)
	, m_nStructure3(0)
	, m_nStructure2(0)
	, m_nStructure4(0)
	, m_nStructure5(0)
	, m_nStructure6(0)
	, m_nStructure7(0)
	, m_nStructure8(0)
	, m_nStructure9(0)
{

}

CDlgMorphology::~CDlgMorphology()
{
}

void CDlgMorphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_Corrosion, m_Type);
	DDX_Radio(pDX, IDC_Hori, m_nMode);
	DDX_Control(pDX, IDC_Structure1, m_Control1);
	DDX_Control(pDX, IDC_Structure2, m_Control2);
	DDX_Control(pDX, IDC_Structure3, m_Control3);
	DDX_Control(pDX, IDC_Structure4, m_Control4);
	DDX_Control(pDX, IDC_Structure5, m_Control5);
	DDX_Control(pDX, IDC_Structure6, m_Control6);
	DDX_Control(pDX, IDC_Structure7, m_Control7);
	DDX_Control(pDX, IDC_Structure8, m_Control8);
	DDX_Control(pDX, IDC_Structure9, m_Control9);
	DDX_Radio(pDX, IDC_Structure1, m_nStructure1);
	DDX_Radio(pDX, IDC_Structure3, m_nStructure3);
	DDX_Radio(pDX, IDC_Structure2, m_nStructure2);
	DDX_Radio(pDX, IDC_Structure4, m_nStructure4);
	DDX_Radio(pDX, IDC_Structure5, m_nStructure5);
	DDX_Radio(pDX, IDC_Structure6, m_nStructure6);
	DDX_Radio(pDX, IDC_Structure7, m_nStructure7);
	DDX_Radio(pDX, IDC_Structure8, m_nStructure8);
	DDX_Radio(pDX, IDC_Structure9, m_nStructure9);
}


BEGIN_MESSAGE_MAP(CDlgMorphology, CDialogEx)
	
	ON_BN_CLICKED(IDC_Corrosion, &CDlgMorphology::OnBnClickedCorrosion)
	ON_BN_CLICKED(IDC_Expansion, &CDlgMorphology::OnBnClickedExpansion)
	ON_BN_CLICKED(IDC_Open, &CDlgMorphology::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_Close, &CDlgMorphology::OnBnClickedClose)
	ON_BN_CLICKED(IDC_InEdge, &CDlgMorphology::OnBnClickedInedge)
	ON_BN_CLICKED(IDC_OutEdge, &CDlgMorphology::OnBnClickedOutedge)
	ON_BN_CLICKED(IDC_Hori, &CDlgMorphology::OnBnClickedHori)
	ON_BN_CLICKED(IDC_Vert, &CDlgMorphology::OnBnClickedVert)
	ON_BN_CLICKED(IDC_custom, &CDlgMorphology::OnBnClickedcustom)
	ON_BN_CLICKED(IDC_Structure1, &CDlgMorphology::OnBnClickedStructure1)
	ON_BN_CLICKED(IDC_Structure2, &CDlgMorphology::OnBnClickedStructure2)
	ON_BN_CLICKED(IDC_Structure3, &CDlgMorphology::OnBnClickedStructure3)
	ON_BN_CLICKED(IDC_Structure4, &CDlgMorphology::OnBnClickedStructure4)
	ON_BN_CLICKED(IDC_Structure5, &CDlgMorphology::OnBnClickedStructure5)
	ON_BN_CLICKED(IDC_Structure6, &CDlgMorphology::OnBnClickedStructure6)
	ON_BN_CLICKED(IDC_Structure7, &CDlgMorphology::OnBnClickedStructure7)
	ON_BN_CLICKED(IDC_Structure8, &CDlgMorphology::OnBnClickedStructure8)
	ON_BN_CLICKED(IDC_Structure9, &CDlgMorphology::OnBnClickedStructure9)
	ON_BN_CLICKED(IDC_ProspectBlack, &CDlgMorphology::OnBnClickedProspectblack)
	ON_BN_CLICKED(IDC_ProspectWhite, &CDlgMorphology::OnBnClickedProspectwhite)
END_MESSAGE_MAP()


// CDlgMorphology 消息处理程序


void CDlgMorphology::UpdateEdit()
{
	BOOL	bEnable;

	// 循环变量
	int		i = 0;
	int		j = 0;

	// 判断是不是自定义模板
	if (m_nMode == 2)
	{
		bEnable = TRUE;
	}
	else
	{
		bEnable = FALSE;
	}

	// 设置文本框不可编辑
	(CEdit*)GetDlgItem(IDC_Structure1)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure2)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure3)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure4)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure5)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure6)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure7)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure8)->EnableWindow(bEnable);
	(CEdit*)GetDlgItem(IDC_Structure9)->EnableWindow(bEnable);

}

void CDlgMorphology::OnBnClickedCorrosion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 0;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedExpansion()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 1;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 2;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 3;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedInedge()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 4;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedOutedge()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Type = 5;

	// 更新
	UpdateData(FALSE);
}


void CDlgMorphology::OnBnClickedHori()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMode = 0;

	m_nStructure4 = 1;
	m_nStructure6 = 1;
	m_nStructure5 = 1;
	m_nStructure1 = 0;
	m_nStructure2 = 0;
	m_nStructure3 = 0;
	m_nStructure7 = 0;
	m_nStructure8 = 0;
	m_nStructure9 = 0;
	((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(0);
	// 更新文本框状态
	UpdateEdit();

	// 更新
	UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedVert()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMode = 1;
	m_nStructure2 = 1;
	m_nStructure5 = 1;
	m_nStructure8 = 1;
	m_nStructure4 = 0;
	m_nStructure3 = 0;
	m_nStructure1 = 0;
	m_nStructure6 = 0;
	m_nStructure7 = 0;
	m_nStructure9 = 0;
	((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(0);
	// 更新文本框状态
	UpdateEdit();

	// 更新
	UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedcustom()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nMode = 2;
	m_nStructure4 = 0;
	m_nStructure6 = 0;
	m_nStructure5 = 0;
	m_nStructure1 = 0;
	m_nStructure2 = 0;
	m_nStructure3 = 0;
	m_nStructure7 = 0;
	m_nStructure8 = 0;
	m_nStructure9 = 0;
	((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(0);
	
	// 更新文本框状态
	UpdateEdit();

	// 更新
	UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure1 == 1)
	{
		m_nStructure1 = 0;
		((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(0);
	}

	else
	{
		m_nStructure1 = 1;
		((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure2 == 1)
	{
		m_nStructure2 = 0;
		((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(0);
	}

	else
	{
		m_nStructure2 = 1;
		((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure3 == 1)
	{
		m_nStructure3 = 0;
		((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(0);
	}

	else
	{
		m_nStructure3 = 1;
		((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure4 == 1)
	{
		m_nStructure4 = 0;
		((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(0);
	}

	else
	{
		m_nStructure4 = 1;
		((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure5 == 1)
	{
		m_nStructure5 = 0;
		((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(0);
	}

	else
	{
		m_nStructure5 = 1;
		((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure6 == 1)
	{
		m_nStructure6 = 0;
		((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(0);
	}

	else
	{
		m_nStructure6 = 1;
		((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure7 == 1)
	{
		m_nStructure7 = 0;
		((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(0);
	}
	

	else
	{
		m_nStructure7 = 1;
		((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure8 == 1)
	{
		m_nStructure8 = 0;
		((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(0);
	}

	else
	{
		m_nStructure8 = 1;
		((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


void CDlgMorphology::OnBnClickedStructure9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_nStructure9 == 1)
	{
		m_nStructure9 = 0;
		((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(0);
	}

	else
	{
		m_nStructure9 = 1;
		((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(1);
	}
	//// 更新
	//UpdateData(TRUE);
}


BOOL CDlgMorphology::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_Hori))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Vert))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_custom))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_Structure4))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure5))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure6))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure7))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure8))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_Structure9))->SetCheck(0);
	m_Control1.EnableWindow(FALSE);
	m_Control2.EnableWindow(FALSE);
	m_Control3.EnableWindow(FALSE);
	m_Control4.EnableWindow(FALSE);
	m_Control5.EnableWindow(FALSE);
	m_Control6.EnableWindow(FALSE);
	m_Control7.EnableWindow(FALSE);
	m_Control8.EnableWindow(FALSE);
	m_Control9.EnableWindow(FALSE);
	m_nStructure1 = 1;
	m_nStructure2 = 1;
	m_nStructure3 = 1;
	m_nStructure4 = 0;
	m_nStructure5 = 0;
	m_nStructure6 = 0;
	m_nStructure7 = 0;
	m_nStructure8 = 0;
	m_nStructure9 = 0;
	option = TRUE;//前景为黑色
	((CButton*)GetDlgItem(IDC_ProspectBlack))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_ProspectWhite))->SetCheck(0);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMorphology::OnBnClickedProspectblack()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (option == FALSE)
	{*/
		option = TRUE;
		((CButton*)GetDlgItem(IDC_ProspectBlack))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_ProspectWhite))->SetCheck(0);
	/*}*/
	//else 
	//{
	//	option == TRUE;
	//	((CButton*)GetDlgItem(IDC_ProspectBlack))->SetCheck(1);
	//	((CButton*)GetDlgItem(IDC_ProspectWhite))->SetCheck(0);
	//}
}


void CDlgMorphology::OnBnClickedProspectwhite()
{
	//怎么实现按钮按下去之后的互选！
	// TODO: 在此添加控件通知处理程序代码
	/*if (option == TRUE)
	{*/
		option = FALSE;
		((CButton*)GetDlgItem(IDC_ProspectBlack))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_ProspectWhite))->SetCheck(1);
	//}
	/*else 
	{
		option == FALSE;
		((CButton*)GetDlgItem(IDC_ProspectBlack))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_ProspectWhite))->SetCheck(1);
	}*/
}
