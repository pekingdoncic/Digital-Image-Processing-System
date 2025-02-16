// CDlgThresholdTrans.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgThresholdTrans.h"
#include "MainFrm.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

// CDlgThresholdTrans 对话框

IMPLEMENT_DYNAMIC(CDlgThresholdTrans, CDialogEx)

CDlgThresholdTrans::CDlgThresholdTrans(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ThresholdTransfor, pParent)
	, m_Threshold(0)
	, m_nMode(0)
{

}

CDlgThresholdTrans::~CDlgThresholdTrans()
{
}

void CDlgThresholdTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Threhold, m_Threshold);
	DDX_Radio(pDX, IDC_Iterative, m_nMode);
}


BEGIN_MESSAGE_MAP(CDlgThresholdTrans, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_EN_KILLFOCUS(IDC_Threhold, &CDlgThresholdTrans::OnEnKillfocusThrehold)
	ON_BN_CLICKED(IDC_Iterative, &CDlgThresholdTrans::OnBnClickedIterative)
	ON_BN_CLICKED(IDC_OTSU, &CDlgThresholdTrans::OnBnClickedOtsu)
	ON_BN_CLICKED(IDC_Histogram, &CDlgThresholdTrans::OnBnClickedHistogram)
	ON_BN_CLICKED(IDC_Adaption, &CDlgThresholdTrans::OnBnClickedAdaption)
	ON_BN_CLICKED(IDC_Calculate, &CDlgThresholdTrans::OnBnClickedCalculate)
	ON_BN_CLICKED(IDC_BUTTONPRE, &CDlgThresholdTrans::OnBnClickedButtonpre)
	ON_BN_CLICKED(IDC_BUTTONREC, &CDlgThresholdTrans::OnBnClickedButtonrec)
	//ON_BN_CLICKED(IDC_suit, &CDlgThresholdTrans::OnBnClickedsuit)
END_MESSAGE_MAP()


// CDlgThresholdTrans 消息处理程序

void My_local_adaptive(LPSTR srcImage, LPSTR dstImage, LONG lLineBytes, long lSrcHeight,
	long lSrcWidth, int areas_of_H, int areas_of_W)
{
	int height = lSrcHeight / areas_of_H;
	int width = lSrcWidth / areas_of_W;
	int Ostu_Threshold = 0;
	int size = height * width / areas_of_H / areas_of_W;

	for (int y = 0; y < areas_of_H; y++)
	{
		for (int x = 0; x < areas_of_W; x++)
		{
			unsigned char* lpSrc;
			float variance = 0;
			float maxVariance = 0, w1 = 0, w2 = 0, avgValue = 0;
			float u0 = 0, u1 = 0, u2 = 0;

			int pixels[256];
			float histgram[256];
			for (int i = 0; i < 256; i++)
			{
				pixels[i] = 0;
			}

			for (int j = y * height; j < ((y + 1 == areas_of_H) ? lSrcHeight : (y + 1) * height); j++)
			{
				for (int i = x * width; i < ((x + 1 == areas_of_W) ? lSrcWidth : (x + 1) * width); i++)
				{
					lpSrc = (unsigned char*)srcImage + lLineBytes * j + i;
					pixels[*lpSrc]++;
				}
			}

			for (int i = 0; i < 256; i++)
			{
				histgram[i] = pixels[i] * 1.0f / size;
			}

			for (int i = 0; i <= 255; i++)
			{
				w1 = 0;
				w2 = 0;
				u1 = 0;
				u2 = 0;

				for (int j = 0; j <= i; j++)
				{
					w1 += histgram[j];
					u1 += histgram[j] * j;
				}
				u1 = u1 / w1;

				w2 = 1 - w1;
				if (i == 255)
				{
					u2 = 0;
				}
				else
				{
					for (int j = i + 1; j <= 255; j++)
					{
						u2 += histgram[j] * j;
					}
				}
				u2 = u2 / w2;

				variance = w1 * w2 * (u1 - u2) * (u1 - u2);
				if (variance > maxVariance)
				{
					maxVariance = variance;
					Ostu_Threshold = i;
				}
			}


			for (int j = y * height; j < ((y + 1 == areas_of_H) ? lSrcHeight : (y + 1) * height); j++)
			{
				for (int i = x * width; i < ((x + 1 == areas_of_W) ? lSrcWidth : (x + 1) * width); i++)
				{
					unsigned char* lpSrc = (unsigned char*)srcImage + lLineBytes * j + i;
					unsigned char* lpDst = (unsigned char*)dstImage + lLineBytes * j + i;
					if (*lpSrc >= Ostu_Threshold)
					{
						*lpDst = 255;
					}
					else
					{
						*lpDst = 0;
					}
				}
			}
		}
	}
}
BOOL CDlgThresholdTrans::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//// 获取绘制直方图的标签
	//CWnd* pWnd = GetDlgItem(IDC_COORDT);

	//// 计算接受鼠标事件的有效区域
	//pWnd->GetClientRect(m_MouseRect);
	//pWnd->ClientToScreen(&m_MouseRect);

	//CRect rect;
	//GetClientRect(rect);
	//ClientToScreen(&rect);

	//m_MouseRect.top -= rect.top;
	//m_MouseRect.left -= rect.left;

	//// 设置接受鼠标事件的有效区域
	//m_MouseRect.top += 25;
	//m_MouseRect.left += 10;
	//m_MouseRect.bottom = m_MouseRect.top + 255;
	//m_MouseRect.right = m_MouseRect.left + 256;

	//// 初始化拖动状态
	//m_bIsDraging = FALSE;

	//
	//return TRUE;  // return TRUE unless you set the focus to a control
	//// 异常: OCX 属性页应返回 FALSE
	m_Click = false;

	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);

	//获取绘制图形的标签
	CWnd* pWnd = GetDlgItem(IDC_COORDT);
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
	m_bIsDraging = FALSE;
	
	//int palettesize = 0;
	//if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	//lpdstImg = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	//memcpy(lpdstImg, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	//pBmpBuf = lpdstImg + sizeof(BITMAPINFOHEADER) + palettesize;

	//m_nMode = 4;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgThresholdTrans::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	//获取绘制坐标的文本框
	CWnd* pWnd = GetDlgItem(IDC_COORDT);
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

	//更改成蓝色画笔
	pDC->SelectObject(pPenBlue);
	//画两条变换线，没有计算斜率,没有交互

	str.Format(L"%d", m_Threshold);
	pDC->TextOut(m_Threshold + 10, 281, str);

	pLeft.x = 10;
	pLeft.y = 280;
	//pDC->MoveTo(10,280);
	pDC->MoveTo(pLeft);
	pCenterTop.x = 10 + m_Threshold;//128;
	pCenterTop.y = 25;

	pCenterBottom.x = 10 + m_Threshold;
	pCenterBottom.y = 280;
	//pDC->LineTo(138,25);

	pDC->LineTo(pCenterBottom);
	pDC->LineTo(pCenterTop);
	pRight.x = 265;
	pRight.y = 25;
	//pDC->LineTo(265,280);
	pDC->LineTo(pRight);

	pDC->SelectObject(pOldPen);
	pDC->MoveTo(10, 25);
	pDC->LineTo(265, 25);
	pDC->LineTo(265, 280);

	delete pPenBlue;
	delete pPenRed;
	delete pPenGreen;
}


void CDlgThresholdTrans::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nMode == 0) {
		((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
	}
	else
	{
		if (m_nMode == 1)
		{
			((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
		}
		else if (m_nMode == 3)
		{
			((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(TRUE);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
		}
	}

	//  当用户释放鼠标左键停止拖动
	if (m_bIsDraging)
	{
		//重新设置拖动状态
		m_bIsDraging = FALSE;
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgThresholdTrans::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//判断是否在有效区域
	if (m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_Threshold))
		{
			//设置拖动状态
			m_bIsDraging = TRUE;
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	else
	{
		if (m_nMode == 0) {
			((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
		}
		else
		{
			if (m_nMode == 1)
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
			}
			else if (m_nMode == 3)
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(TRUE);
			}
			else if(m_nMode == 2)
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
			}
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgThresholdTrans::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//  判断当前光标是否在绘制区域
	if (m_MouseRect.PtInRect(point))
	{
		//判断是否在拖动
		if (m_bIsDraging)
		{
			//更改阈值
			m_Threshold = (BYTE)(point.x - m_MouseRect.left);
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			//更新
			UpdateData(FALSE);
			//重绘
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_Threshold))
		{
			//更改光标
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	else
	{
		if (m_nMode == 0) {
			((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(TRUE);
			((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
			((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
		}
		else
		{
			if (m_nMode == 1)
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
			}
			else if (m_nMode == 3)
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(TRUE);
			}
			else
			{
				((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
				((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(TRUE);
				((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
			}
		}
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgThresholdTrans::OnEnKillfocusThrehold()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);

	UpdateData(TRUE);

	InvalidateRect(m_MouseRect, TRUE);
}


void CDlgThresholdTrans::OnBnClickedIterative()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
	(CEdit*)GetDlgItem(IDC_Threhold)->EnableWindow(TRUE);
	UpdateData(TRUE);
}


void CDlgThresholdTrans::OnBnClickedOtsu()
{
	// TODO: 在此添加控件通知处理程序代码
	//UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
	(CEdit*)GetDlgItem(IDC_Threhold)->EnableWindow(TRUE);
	UpdateData(TRUE);
}


void CDlgThresholdTrans::OnBnClickedHistogram()
{
	//m_Threshold = 0;
	//UpdateData(FALSE);
	//// TODO: 在此添加控件通知处理程序代码
	//((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
	//((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	//((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(TRUE);
	//((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);

	//CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	//CMFCImgExperimentDoc* pDoc = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc	
	//////////////////////////////////////////////////////////////////////////////////////////////////	
	//long lSrcLineBytes;		//图象每行的字节数
	//long	lSrcWidth;      //图象的宽度和高度
	//long	lSrcHeight;
	//int     lpSrcBitCount;       //图像的位深
	//LPSTR	lpSrcDib;		//指向源图象的指针	
	//LPSTR	lpSrcStartBits;	//指向源像素的指针
	//lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	//if (!lpSrcDib) return;

	//lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	//lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	//lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	//lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	//lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	//My_local_adaptive((LPSTR)pBmpBuf, lpSrcStartBits, WIDTHBYTES(lSrcWidth * 8), lSrcHeight, lSrcWidth, 2, 2);

	//UpdateData(TRUE);
	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(FALSE);
	(CEdit*)GetDlgItem(IDC_Threhold)->EnableWindow(FALSE);
	UpdateData(TRUE);
	m_Threshold = 0;
	m_Click = true;
	UpdateData(FALSE);
	InvalidateRect(m_MouseRect, FALSE);
}


void CDlgThresholdTrans::OnBnClickedAdaption()
{
	// TODO: 在此添加控件通知处理程序代码
	((CButton*)GetDlgItem(IDC_Iterative))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_OTSU))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Histogram))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_Adaption))->SetCheck(TRUE);
	(CEdit*)GetDlgItem(IDC_Threhold)->EnableWindow(TRUE);
	UpdateData(TRUE);
}


void CDlgThresholdTrans::OnBnClickedCalculate()
{
	// TODO: 在此添加控件通知处理程序代码
	long  lSrcLineBytes;		//图象每行的字节数
	long	  lSrcWidth;      //图象的宽度和高度
	long	  lSrcHeight;
	int    lpSrcBitCount;   //图像的位深
	LPSTR	lpSrcStartBits;	//指向源像素的指针

	lpSrcStartBits = m_dib->GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = m_dib->GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = m_dib->GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = m_dib->GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = m_dib->GetReqByteWidth(lSrcWidth * 8);		// 计算图象每行的字节数

	if (m_nMode == 0)//迭代法
	{
		LONG sum = 0;
		for (int i = 0; i < lSrcHeight * lSrcLineBytes; i++)
		{
			BYTE byte = *(lpSrcStartBits + i);
			sum += byte;
		}
		LONG T1 = sum / (lSrcHeight * lSrcWidth);//初始值
		LONG T0 = 20;//指定的差
		while (true) {
			LONG u1 = 0, u2 = 0, n1 = 0, n2 = 0;
			for (int i = 0; i < lSrcHeight * lSrcLineBytes; i++)
			{
				BYTE byte = *(lpSrcStartBits + i);
				if (byte < T1) {
					u1 += byte;
					n1++;
				}
				else
				{
					u2 += byte;
					n2++;
				}
			}
			u1 /= n1;
			u2 /= n2;
			BYTE T2 = (u1 + u2) / 2;
			if (abs(T1 - T2) < T0)
			{
				m_Threshold = T2;
				break;
			}
			else
			{
				T1 = T2;
			}
		};
	}
	else
		if (m_nMode == 1)//OTSU法
		{
			double histogram[256] = { 0 };
			BYTE* lpSrc;
			for (int i = 0; i < lSrcHeight; i++)
			{
				for (int j = 0; j < lSrcWidth; j++)
				{
					lpSrc = (BYTE*)lpSrcStartBits + lSrcLineBytes * i + j;
					histogram[*(lpSrc)]++;
				}
			}
			for (int i = 0; i < 256; i++)
				histogram[i] /= lSrcHeight * lSrcWidth;
			double avg = 0.0;
			for (int i = 0; i < 256; i++)
				avg += i * histogram[i];
			double va = 0.0;
			for (int i = 0; i < 256; i++)
				va += (i - avg) * (i - avg) * histogram[i];
			BYTE threshord = 20;
			double maxva = 0.0;
			double p = 0.0, a = 0.0, s = 0.0;
			for (int i = 0; i < 256; i++)
			{
				p += histogram[i];
				a += i * histogram[i];
				s = (avg * p - a) * (avg * p - a) / p / (1 - p);
				if (s > maxva)
				{
					threshord = i;
					maxva = s;
				}
			}
			m_Threshold = threshord;
		}
		else
			if (m_nMode == 3)//一维交叉熵值法
			{
				double dHistogram[256];         //灰度直方图  
				double dEntropy[256];           //每个像素的交叉熵  
				int N = lSrcWidth * lSrcHeight;           //总像素数  
				int i, j;
				for (int i = 0; i < 256; i++)
				{
					dHistogram[i] = 0.0;
					dEntropy[i] = 0.0;
				}
				for (i = 0; i < lSrcHeight; i++)
				{
					for (j = 0; j < lSrcWidth; j++)
					{
						// 指向DIB第i行，第j个象素的指针
						unsigned char nData = (unsigned char)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;
						dHistogram[nData]++;     //建立直方图  
					}
				}
				double Pa = 0.0;      //区域1平均灰度值  
				double Pb = 0.0;      //区域2平均灰度值  
				double P0 = 0.0;      //全局平均灰度值  
				double Wa = 0.0;      //第一部分熵  
				double Wb = 0.0;      //第二部分的熵  
				double dData1 = 0.0, dData2 = 0.0;  //中间值  
				double dData3 = 0.0, dData4 = 0.0;  //中间值  

				for (i = 0; i < 256; i++)     //计算全局平均灰度  
				{
					dHistogram[i] /= N;
					P0 += i * dHistogram[i];
				}

				for (i = 0; i < 256; i++)
				{
					Wa = Wb = dData1 = dData2 = dData3 = dData4 = Pa = Pb = 0.0;
					for (int j = 0; j < 256; j++)
					{
						if (j <= i)
						{
							dData1 += dHistogram[j];
							dData2 += j * dHistogram[j];
						}
						else
						{
							dData3 += dHistogram[j];
							dData4 += j * dHistogram[j];
						}
					}
					Pa = dData2 / dData1;
					Pb = dData4 / dData3;
					for (j = 0; j < 256; j++)
					{
						if (j <= i)
						{
							if ((Pa != 0) && (dHistogram[j] != 0))
							{
								double d1 = log(dHistogram[j] / Pa);
								Wa += j * dHistogram[j] * d1 / log(2);
							}
						}
						else
						{
							if ((Pb != 0) && (dHistogram[j] != 0))
							{
								double d2 = log(dHistogram[j] / Pb);
								Wb += j * dHistogram[j] * d2 / log(2);
							}
						}
					}
					dEntropy[i] = Wa + Wb;
				}
				//遍历熵值，求取最小交叉熵所对应的灰度值  
				double temp = dEntropy[0];
				for (i = 1; i < 256; i++)
				{
					if (dEntropy[i] < temp)
					{
						temp = dEntropy[i];
						m_Threshold = i;
					}
				}
			}
	m_Click = true;
	UpdateData(FALSE);
	InvalidateRect(m_MouseRect, FALSE);
}


void CDlgThresholdTrans::OnBnClickedButtonpre()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* chuzhi = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc 

	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度

	UpdateData(TRUE);

	BYTE bThre;//阈值
	bThre = m_Threshold;
	if (m_nMode !=2)
	{
		pDoc->m_dib.Thre_Trans(lpSrcStartBits, lSrcWidth, lSrcHeight, bThre);//通过阈值直接修改图像
	}
	if (m_nMode == 2)
	{
		int y, x;
		int nThd[2][2];
		int nLocAvg = 0;
		for (y = 0; y < lSrcHeight / 2; y++)//高度的一半
		{
			for (x = 0; x < lSrcWidth / 2; x++)//宽度的一半
			{
				nLocAvg += lpSrcStartBits[y * lSrcWidth + x];//左上角叠加
			}
		}
		nLocAvg /= ((lSrcHeight / 2) * (lSrcWidth / 2));
		nThd[0][0] = nLocAvg;//左上角
		for (y = 0; y < lSrcHeight / 2; y++)//以nThd[0][0]为界限，进行二值化
		{
			for (x = 0; x < lSrcWidth / 2; x++)
			{
				if (lpSrcStartBits[y * lSrcWidth + x] < nThd[0][0])
					lpSrcStartBits[y * lSrcWidth + x] = 0;
				else
					lpSrcStartBits[y * lSrcWidth + x] = 255;
			}
		}
		nLocAvg = 0;
		for (y = lSrcHeight / 2; y < lSrcHeight; y++)
		{
			for (x = 0; x < lSrcWidth / 2; x++)
			{
				nLocAvg += lpSrcStartBits[y * lSrcWidth + x];
			}
		}
		nLocAvg /= ((lSrcHeight - lSrcHeight / 2) * (lSrcWidth / 2));
		nThd[1][0] = nLocAvg;
		for (y = lSrcHeight / 2; y < lSrcHeight; y++)
		{
			for (x = 0; x < lSrcWidth / 2; x++)
			{
				if (lpSrcStartBits[y * lSrcWidth + x] < nThd[1][0])
					lpSrcStartBits[y * lSrcWidth + x] = 0;
				else
					lpSrcStartBits[y * lSrcWidth + x] = 255;
			}
		}
		nLocAvg = 0;
		for (y = 0; y < lSrcHeight; y++)
		{
			for (x = lSrcWidth / 2; x < lSrcWidth; x++)
			{
				nLocAvg += lpSrcStartBits[y * lSrcWidth + x];
			}
		}
		nLocAvg /= ((lSrcHeight / 2) * (lSrcWidth - lSrcWidth / 2));
		nThd[0][1] = nLocAvg;
		for (y = 0; y < lSrcHeight / 2; y++)
		{
			for (x = lSrcWidth / 2; x < lSrcWidth; x++)
			{
				if (lpSrcStartBits[y * lSrcWidth + x] < nThd[0][1])
					lpSrcStartBits[y * lSrcWidth + x] = 0;
				else
					lpSrcStartBits[y * lSrcWidth + x] = 255;
			}
		}
		nLocAvg = 0;
		for (y = lSrcHeight / 2; y < lSrcHeight; y++)
		{
			for (x = lSrcWidth / 2; x < lSrcWidth; x++)
			{
				nLocAvg += lpSrcStartBits[y * lSrcWidth + x];
			}
		}
		nLocAvg /= ((lSrcHeight - lSrcHeight / 2) * (lSrcWidth - lSrcWidth / 2));
		nThd[1][1] = nLocAvg;
		for (y = lSrcHeight / 2; y < lSrcHeight; y++)
		{
			for (x = lSrcWidth / 2; x < lSrcWidth; x++)
			{
				if (lpSrcStartBits[y * lSrcWidth + x] < nThd[1][1])
					lpSrcStartBits[y * lSrcWidth + x] = 0;
				else
					lpSrcStartBits[y * lSrcWidth + x] = 255;
			}
		}
		for (y = 0; y < lSrcHeight; y++)
		{
			for (x = 0; x < lSrcWidth; x++)
			{
				lpSrcStartBits[y * lSrcWidth + x] = 255 - lpSrcStartBits[y * lSrcWidth + x];
			}
		}
	}
	chuzhi->UpdateAllViews(NULL);
}


void CDlgThresholdTrans::OnBnClickedButtonrec()
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





//void CDlgThresholdTrans::OnBnClickedsuit()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	m_Threshold = 0;
//	m_nMode = 4;
//	m_Click = true;
//	//UpdateData(FALSE);
//	InvalidateRect(m_MouseRect, FALSE);
//	////////////////////////////////////////////////////////////////////////////////////////////////
//	
//
//}
