// CDlgGrayScaleStretch.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgGrayScaleStretch.h"
#include "MainFrm.h"
#include "MFCImgExperimentDoc.h"
#include "PointTrans.h"


// CDlgGrayScaleStretch 对话框

IMPLEMENT_DYNAMIC(CDlgGrayScaleStretch, CDialogEx)

CDlgGrayScaleStretch::CDlgGrayScaleStretch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GrayScaleStretch, pParent)
	, m_Point1_X(0)
	, m_Point1_Y(0)
	, m_Point2_X(0)
	, m_Point2_Y(0)
{

}

CDlgGrayScaleStretch::~CDlgGrayScaleStretch()
{
}

void CDlgGrayScaleStretch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Point1_X, m_Point1_X);
	DDX_Text(pDX, IDC_Point1_Y, m_Point1_Y);
	DDX_Text(pDX, IDC_Point2_X, m_Point2_X);
	DDX_Text(pDX, IDC_Point2_Y, m_Point2_Y);
}


BEGIN_MESSAGE_MAP(CDlgGrayScaleStretch, CDialogEx)
	ON_EN_KILLFOCUS(IDC_Point1_X, &CDlgGrayScaleStretch::OnEnKillfocusPoint1X)
	ON_EN_KILLFOCUS(IDC_Point1_Y, &CDlgGrayScaleStretch::OnEnKillfocusPoint1Y)
	ON_EN_KILLFOCUS(IDC_Point2_X, &CDlgGrayScaleStretch::OnEnKillfocusPoint2X)
	ON_EN_KILLFOCUS(IDC_Point2_Y, &CDlgGrayScaleStretch::OnEnKillfocusPoint2Y)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTONPRE, &CDlgGrayScaleStretch::OnBnClickedButtonpre)
	ON_BN_CLICKED(IDC_BUTTONREC, &CDlgGrayScaleStretch::OnBnClickedButtonrec)
END_MESSAGE_MAP()


// CDlgGrayScaleStretch 消息处理程序


BOOL CDlgGrayScaleStretch::OnInitDialog()
{
	// 调用默认OnInitDialog函数
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	
	// 获取绘制参数曲线的标签
	CWnd* pWnd = GetDlgItem(IDC_COORD);
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
	m_bIsDraging = 0;

	// 返回TRUE

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgGrayScaleStretch::OnEnKillfocusPoint1X()
{
	// TODO: 在此添加控件通知处理程序代码
	// 更新
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_Point1_X > m_Point2_X)
	{
		// 互换
		BYTE bTemp = m_Point1_X;
		m_Point1_X = m_Point2_X;
		m_Point2_X = bTemp;
		bTemp = m_Point1_Y;
		m_Point1_Y = m_Point2_Y;
		m_Point2_Y = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);

}


void CDlgGrayScaleStretch::OnEnKillfocusPoint1Y()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_Point1_X > m_Point2_X)
	{
		// 互换
		BYTE bTemp = m_Point1_X;
		m_Point1_X = m_Point2_X;
		m_Point2_X = bTemp;
		bTemp = m_Point1_Y;
		m_Point1_Y = m_Point2_Y;
		m_Point2_Y = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);


}


void CDlgGrayScaleStretch::OnEnKillfocusPoint2X()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_Point1_X > m_Point2_X)
	{
		// 互换
		BYTE bTemp = m_Point1_X;
		m_Point1_X = m_Point2_X;
		m_Point2_X = bTemp;
		bTemp = m_Point1_Y;
		m_Point1_Y = m_Point2_Y;
		m_Point2_Y = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);


}


void CDlgGrayScaleStretch::OnEnKillfocusPoint2Y()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	// 判断是否下限超过上限
	if (m_Point1_X > m_Point2_X)
	{
		// 互换
		BYTE bTemp = m_Point1_X;
		m_Point1_X = m_Point2_X;
		m_Point2_X = bTemp;
		bTemp = m_Point1_Y;
		m_Point1_Y = m_Point2_Y;
		m_Point2_Y = bTemp;

		// 更新
		UpdateData(FALSE);
	}

	// 重绘
	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgGrayScaleStretch::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//	// 判断当前光标是否在绘制区域
	//if (m_MouseRect.PtInRect(point))
	//{
	//	// 更改光标
	//	::SetCursor(::LoadCursor(NULL, IDC_CROSS));

	//	// 判断是否正在拖动
	//	if (m_bIsDraging!=0)
	//	{
	//		// 获取绘图的标签
	//		CWnd* pWnd = GetDlgItem(IDC_COORD);

	//		// 获取设备上下文
	//		CDC* pDC = pWnd->GetDC();

	//		// 设置绘制方式为异或模式
	//		int nOldDrawMode = pDC->SetROP2(R2_XORPEN);

	//		// 创建新的画笔
	//		CPen* pPen = new CPen;
	//		pPen->CreatePen(PS_DOT, 1, RGB(0, 0, 0));

	//		// 选中新画笔
	//		CGdiObject* pOldPen = pDC->SelectObject(pPen);

	//		// 判断是否已经画过橡皮筋线
	//		if (m_bDrawed)
	//		{
	//			// 擦去以前的橡皮筋线
	//			pDC->MoveTo(m_p1);
	//			pDC->LineTo(m_p2);
	//		}

	//		// 保存当前的坐标
	//		m_p2 = point;

	//		// 转换坐标系
	//		m_p2.x = m_p2.x - m_MouseRect.left + 10;
	//		m_p2.y = m_p2.y - m_MouseRect.top + 25;

	//		// 绘制一条新橡皮筋线
	//		pDC->MoveTo(m_p1);
	//		pDC->LineTo(m_p2);

	//		// 设置m_bDrawed为TRUE
	//		m_bDrawed = TRUE;

	//		// 选回以前的画笔
	//		pDC->SelectObject(pOldPen);

	//		// 恢复成以前的绘制模式
	//		pDC->SetROP2(nOldDrawMode);

	//		delete pPen;
	//		ReleaseDC(pDC);
	//	}
	//}
	//else
	//{
	//	// 判断是否正在拖动
	//	if (m_bIsDraging)
	//	{
	//		// 更改光标
	//		::SetCursor(::LoadCursor(NULL, IDC_NO));
	//	}
	//}

	//// 默认鼠标移动处理事件
	//CDialogEx::OnMouseMove(nFlags, point);

	if (m_MouseRect.PtInRect(point))
	{
		// 判断是否正在拖动
		if (m_bIsDraging != 0)
		{
			// 判断正在拖动点1还是点2
			if (m_bIsDraging == 1)
			{
				// 判断是否下限<上限
				if (point.x - m_MouseRect.left < m_Point2_X)
				{
					// 更改下限
					m_Point1_X = (BYTE)(point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为上限-1
					m_Point1_X = m_Point2_X - 1;

					// 重设鼠标位置
					point.x = m_MouseRect.left + m_Point2_X - 1;
				}

				// 更改Y坐标
				m_Point1_Y = (BYTE)(255 + m_MouseRect.top - point.y);
			}
			else
			{
				// 正在拖动点2

				// 判断是否上限>下限
				if (point.x - m_MouseRect.left > m_Point1_X)
				{
					// 更改下限
					m_Point2_X = (BYTE)(point.x - m_MouseRect.left);
				}
				else
				{
					// 下限拖过上限，设置为下限＋1
					m_Point2_X = m_Point1_X + 1;

					// 重设鼠标位置
					point.x = m_MouseRect.left + m_Point1_X + 1;
				}

				// 更改Y坐标
				m_Point2_Y = (BYTE)(255 + m_MouseRect.top - point.y);
			}

			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));

			// 更新
			UpdateData(FALSE);

			// 重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else
		{
			CRect	rectTemp1;
			CRect	rectTemp2;

			// 计算点1临近区域
			rectTemp1.left = m_MouseRect.left + m_Point1_X - 2;
			rectTemp1.right = m_MouseRect.left + m_Point1_X + 2;
			rectTemp1.top = 255 + m_MouseRect.top - m_Point1_Y - 2;
			rectTemp1.bottom = 255 + m_MouseRect.top - m_Point1_Y + 2;

			// 计算点2临近区域
			rectTemp2.left = m_MouseRect.left + m_Point2_X - 2;
			rectTemp2.right = m_MouseRect.left + m_Point2_X + 2;
			rectTemp2.top = 255 + m_MouseRect.top - m_Point2_Y - 2;
			rectTemp2.bottom = 255 + m_MouseRect.top - m_Point2_Y + 2;

			// 判断用户在点1或点2旁边
			if ((rectTemp1.PtInRect(point)) || (rectTemp2.PtInRect(point)))
			{
				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	// 默认鼠标移动处理事件
	CDialog::OnMouseMove(nFlags, point);
}


void CDlgGrayScaleStretch::OnPaint()
{

	// 设备上下文
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	// 字符串
	CString str;

	// 获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORD);

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
	pPenBlue->CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	// 选中当前红色画笔，并保存以前的画笔
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);

	// 绘制坐标轴
	pDC->MoveTo(10, 10);

	// 垂直轴
	pDC->LineTo(10, 280);

	// 水平轴
	pDC->LineTo(320, 280);

	// 写坐标
	str.Format(L"0");
	pDC->TextOut(10, 281, str);

	str.Format(L"255");
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

	// 更改成蓝色画笔
	pDC->SelectObject(pPenBlue);

	// 绘制坐标值
	str.Format(L"(%d, %d)", m_Point1_X, m_Point1_Y);
	pDC->TextOut(m_Point1_X + 10, 281 - m_Point1_Y, str);
	str.Format(L"(%d, %d)", m_Point2_X, m_Point2_Y);
	pDC->TextOut(m_Point2_X + 10, 281 - m_Point1_Y, str);

	// 绘制用户指定的变换直线
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_Point1_X + 10, 280 - m_Point1_Y);
	pDC->LineTo(m_Point2_X + 10, 280 - m_Point2_Y);
	pDC->LineTo(265, 25);

	// 绘制点边缘的小矩形
	CBrush  brush;
	brush.CreateSolidBrush(RGB(0, 255, 0));

	// 选中刷子
	CGdiObject* pOldBrush = pDC->SelectObject(&brush);

	// 绘制小矩形
	pDC->Rectangle(m_Point1_X + 10 - 2, 280 - m_Point1_Y - 2, m_Point1_X + 12, 280 - m_Point1_Y + 2);
	pDC->Rectangle(m_Point2_X + 10 - 2, 280 - m_Point2_Y - 2, m_Point2_X + 12, 280 - m_Point2_Y + 2);

	// 恢复以前的画笔
	pDC->SelectObject(pOldPen);

	// 绘制边缘
	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);

	// 删除新的画笔
	delete pPenRed;
	delete pPenBlue;
}


void CDlgGrayScaleStretch::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_MouseRect.PtInRect(point))
	{
		CRect	rectTemp;

		// 计算点1临近区域
		rectTemp.left = m_MouseRect.left + m_Point1_X - 2;
		rectTemp.right = m_MouseRect.left + m_Point1_X + 2;
		rectTemp.top = 255 + m_MouseRect.top - m_Point1_Y - 2;
		rectTemp.bottom = 255 + m_MouseRect.top - m_Point1_Y + 2;

		// 判断用户是不是想拖动点1
		if (rectTemp.PtInRect(point))
		{

			// 设置拖动状态1，拖动点1
			m_bIsDraging = 1;

			// 更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
		}
		else
		{
			// 计算点2临近区域
			rectTemp.left = m_MouseRect.left + m_Point2_X - 2;
			rectTemp.right = m_MouseRect.left + m_Point2_X + 2;
			rectTemp.top = 255 + m_MouseRect.top - m_Point2_Y - 2;
			rectTemp.bottom = 255 + m_MouseRect.top - m_Point2_Y + 2;

			// 判断用户是不是想拖动点2
			if (rectTemp.PtInRect(point))
			{

				// 设置拖动状态为2，拖动点2
				m_bIsDraging = 2;

				// 更改光标
				::SetCursor(::LoadCursor(NULL, IDC_SIZEALL));
			}
		}
	}

	// 默认单击鼠标左键处理事件
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgGrayScaleStretch::	OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bIsDraging != 0)
	{
		// 重置拖动状态
		m_bIsDraging = 0;
	}

	// 默认释放鼠标左键处理事件
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgGrayScaleStretch::OnBnClickedButtonpre()
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

	BYTE bX1, bY1, bX2, bY2;

	bX1 = m_Point1_X;
	bY1 = m_Point1_Y;
	bX2 = m_Point2_X;
	bY2 = m_Point2_Y;

	GrayStretch(lpSrcStartBits, lSrcWidth, lSrcHeight, bX1, bY1, bX2, bY2);


	pDoc->UpdateAllViews(NULL);
}


void CDlgGrayScaleStretch::OnBnClickedButtonrec()
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
