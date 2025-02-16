﻿// CDlgInverseLshaped.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgInverseLshaped.h"


// CDlgInverseLshaped 对话框

IMPLEMENT_DYNAMIC(CDlgInverseLshaped, CDialogEx)

CDlgInverseLshaped::CDlgInverseLshaped(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Inverted_LShaped, pParent)
	, m_threshold(0)
{

}

CDlgInverseLshaped::~CDlgInverseLshaped()
{
}

void CDlgInverseLshaped::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Threshold, m_threshold);
}


BEGIN_MESSAGE_MAP(CDlgInverseLshaped, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_EN_KILLFOCUS(IDC_Threshold, &CDlgInverseLshaped::OnEnKillfocusThreshold)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDlgInverseLshaped 消息处理程序


void CDlgInverseLshaped::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	//获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_Inverted_LShaped);
	//字符串
	CString str;
	CPoint pLeft, pRight, pCenterTop, pCenterBottom;
	//指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	//x1(left),y1(top),x2(right),y2(bottom)
	pDC->Rectangle(0, 0, 330, 300);
	//创建画笔对象
	CPen* pPenRed = new CPen;
	//红色画笔，红绿蓝
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	//创建画笔对象
	CPen* pPenBlue = new CPen;
	//蓝色画笔，红绿蓝
	pPenBlue->CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	//创建画笔对象
	CPen* pPenGreen = new CPen;
	//绿色画笔，红绿蓝
	pPenGreen->CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	//选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	//绘制坐标轴
	pDC->MoveTo(10, 10);
	//垂直轴
	pDC->LineTo(10, 280);
	//水平轴
	pDC->LineTo(320, 280);
	//写坐标
	str.Format(L"0");
	pDC->TextOut(10, 281, str);
	str.Format(L"255");
	pDC->TextOut(265, 281, str);
	pDC->TextOutW(11, 25, str);
	//绘制X箭头
	pDC->LineTo(315, 275);
	pDC->MoveTo(320, 280);
	pDC->LineTo(315, 285);
	//绘制Y箭头
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);
	//更改成绿色画笔,画垂直线

	pDC->SelectObject(pPenGreen);
	pCenterTop.x = 10 + m_threshold;//128;
	pCenterTop.y = 25;

	pCenterBottom.x = 10 + m_threshold;//128;
	pCenterBottom.y = 280;
	pDC->MoveTo(pCenterTop);
	pDC->LineTo(pCenterBottom);

	//更改成蓝色画笔
	pDC->SelectObject(pPenBlue);
	//画两条变换线，没有计算斜率,没有交互
	pLeft.x = 10;
	pLeft.y = 280;
	//pDC->MoveTo(10,280);
	pDC->MoveTo(pLeft);
	pCenterTop.x = 10 + m_threshold;//128;
	pCenterTop.y = 25;
	//pDC->LineTo(138,25);
	pDC->LineTo(pCenterTop);
	pRight.x = 265;
	pRight.y = 25;
	//pDC->LineTo(265,280);
	pDC->LineTo(pRight);
}


void CDlgInverseLshaped::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		//  当用户单击鼠标左键开始拖动
	//判断是否在有效区域
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_threshold))
		{
			//设置拖动状态
			m_bIsDrawing = TRUE;
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	//默认

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgInverseLshaped::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//  当用户释放鼠标左键停止拖动
	if (m_bIsDrawing)
	{
		//重新设置拖动状态
		m_bIsDrawing = FALSE;
	}
	//默认

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgInverseLshaped::OnEnKillfocusThreshold()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存用户设置
	UpdateData(TRUE);
	//重绘
	InvalidateRect(m_MouseRect, TRUE);

}


BOOL CDlgInverseLshaped::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//获取绘制图形的标签
	CWnd* pWnd = GetDlgItem(IDC_Inverted_LShaped);
	//计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);

	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	//设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	//初始化拖动状态
	m_bIsDrawing = FALSE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgInverseLshaped::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	//  判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point))
	{
		//判断是否在拖动
		if (m_bIsDrawing)
		{
			//更改阈值
			m_threshold = (BYTE)(point.x - m_MouseRect.left);
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			//更新
			UpdateData(FALSE);
			//重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_threshold))
		{
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
		//默认
		CDialogEx::OnMouseMove(nFlags, point);
}
