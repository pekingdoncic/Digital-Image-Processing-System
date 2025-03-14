﻿// CDlgWindowTran.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgWindowTran.h"
#include "MainFrm.h"
#include "MFCImgExperimentDoc.h"
#include "PointTrans.h"


// CDlgWindowTran 对话框

IMPLEMENT_DYNAMIC(CDlgWindowTran, CDialogEx)

CDlgWindowTran::CDlgWindowTran(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WindowTranfor, pParent)
	, m_bUp(0)
	, m_bLow(0)
{

}

CDlgWindowTran::~CDlgWindowTran()
{
}

void CDlgWindowTran::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_UpperLimit, m_bUp);
	DDX_Text(pDX, IDC_LowerLimit, m_bLow);
}


BEGIN_MESSAGE_MAP(CDlgWindowTran, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTONREC, &CDlgWindowTran::OnBnClickedButtonrec)
	ON_BN_CLICKED(IDC_BUTTONPRE, &CDlgWindowTran::OnBnClickedButtonpre)
	ON_EN_KILLFOCUS(IDC_LowerLimit, &CDlgWindowTran::OnEnKillfocusLowerlimit)
	ON_EN_KILLFOCUS(IDC_UpperLimit, &CDlgWindowTran::OnEnKillfocusUpperlimit)
END_MESSAGE_MAP()


// CDlgWindowTran 消息处理程序


void CDlgWindowTran::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		// 当用户单击鼠标左键开始拖动
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_bLow))
		{

			// 设置拖动状态1，拖动下限
			m_iIsDraging = 1;

			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_bUp))
		{

			// 设置拖动状态为2，拖动上限
			m_iIsDraging = 2;

			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	// 默认单击鼠标左键处理事件
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgWindowTran::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 当用户释放鼠标左键停止拖动
	if (m_iIsDraging != 0)
	{
		// 重置拖动状态
		m_iIsDraging = 0;
	}

	// 默认释放鼠标左键处理事件
	CDialog::OnLButtonUp(nFlags, point);
	//CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgWindowTran::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_iIsDraging != 0)
		{
			// 判断正在拖动上限还是下限
			if (m_iIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_bUp)
				{
					// 更改下限
					m_bLow = (BYTE)(point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_bLow = m_bUp - 1;

					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bUp - 1;
				}
			}
			else
			{
				// 正在拖动上限

				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_bLow)
				{
					// 更改下限
					m_bUp = (BYTE)(point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_bUp = m_bLow + 1;

					// 重设鼠标位置
					point.x = m_MouseRect.left + m_bLow + 1;
				}
			}

			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));

			// 更新
			UpdateData(FALSE);

			// 重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_bLow) || point.x == (m_MouseRect.left + m_bUp))
		{
			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}

	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
	//CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgWindowTran::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	// 字符串
	CString str;

	//// 设备上下文
	//CPaintDC dc(this);

	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORDWin);

	// 指针
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();

	pDC->Rectangle(0, 0, 330, 300);

	// 创建画笔对象
	CPen* pPenRed = new CPen;

	// 红色画笔
	pPenRed->CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	// 创建画笔对象
	CPen* pPenBlue = new CPen;

	// 蓝色画笔
	pPenBlue->CreatePen(PS_SOLID, 2, RGB(0, 0, 255));

	// 创建画笔对象
	CPen* pPenGreen = new CPen;

	// 绿色画笔
	pPenGreen->CreatePen(PS_DOT, 1, RGB(0, 255, 0));

	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	// 绘制坐标轴
	pDC->MoveTo(10, 10);

	// 垂直轴
	pDC->LineTo(10, 280);

	// 水平轴
	pDC->LineTo(320, 280);

	// 写坐标
	str.Format(_T("0"));
	pDC->TextOut(10, 281, str);

	str.Format(_T("255"));
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);


	// 绘制X轴箭头
	pDC->LineTo(315, 275);
	pDC->MoveTo(320, 280);
	pDC->LineTo(315, 285);

	// 绘制X轴箭头
	pDC->MoveTo(10, 10);
	pDC->LineTo(5, 15);
	pDC->MoveTo(10, 10);
	pDC->LineTo(15, 15);

	// 更改成绿色画笔
	pDC->SelectObject(pPenGreen);

	// 绘制窗口上下限
	pDC->MoveTo(m_bLow + 10, 25);
	pDC->LineTo(m_bLow + 10, 280);

	pDC->MoveTo(m_bUp + 10, 25);
	pDC->LineTo(m_bUp + 10, 280);

	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);

	// 绘制坐标值
	str.Format(_T("%d, %d"), m_bLow, m_bLow);
	pDC->TextOut(m_bLow + 10, 281 - m_bLow, str);
	str.Format(_T("%d, %d"), m_bUp, m_bUp);
	pDC->TextOut(m_bUp + 10, 281 - m_bUp, str);

	// 绘制用户指定的窗口（注意转换坐标系）
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bLow + 10, 280);
	pDC->LineTo(m_bLow + 10, 280 - m_bLow);
	pDC->LineTo(m_bUp + 10, 280 - m_bUp);
	pDC->LineTo(m_bUp + 10, 25);
	pDC->LineTo(265, 25);

	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);

	// 绘制边缘
	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);

	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;

}


BOOL CDlgWindowTran::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 获取绘制直方图的标签
	CWnd* pWnd = GetDlgItem(IDC_COORDWin);

	// 计算接受鼠标事件的有效区域
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);

	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;

	// 设置接受鼠标事件的有效区域
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;

	// 初始化拖动状态
	m_iIsDraging = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgWindowTran::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
		// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
	}

	// 默认处理事件
	CDialog::OnOK();
}


void CDlgWindowTran::OnBnClickedButtonrec()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* pDoc = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;


	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

//int lBitCountNew = 24;
	int lBitCountNew = lpSrcBitCount;
	long lLineBytesNew = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lBitCountNew);
	int PaletteSize = 0;
	if (lpSrcBitCount == 8) PaletteSize = 1024;
	memcpy(lpSrcDib, pDoc->lpImgCpy, sizeof(BITMAPINFOHEADER) + PaletteSize + lSrcHeight * lLineBytesNew);


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CDlgWindowTran::OnBnClickedButtonpre()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* pDoc = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc 

	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	LPSTR	lpSrcDib;		//指向源图象的指针	
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度

	UpdateData(TRUE);

	BYTE bLow;
	BYTE bUp;

	bLow = m_bLow;
	bUp = m_bUp;
	//MyTemplate(lpSrcStartBits, lSrcWidth, lSrcHeight, 3, 3, 1, 1, temp, xishu, yuzhi);
	//pDoc->m_dib.Thre_Trans(lpSrcStartBits, lSrcWidth, lSrcHeight, bThre);//通过阈值直接修改图像
	WindowTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, bLow, bUp);
	pDoc->UpdateAllViews(NULL);
}


void CDlgWindowTran::OnEnKillfocusLowerlimit()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgWindowTran::OnEnKillfocusUpperlimit()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_bLow > m_bUp)
	{
		// 互换
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}
