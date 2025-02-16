// CDlgImagePanning.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgImagePanning.h"
#include "MainFrm.h"
#include "MFCImgExperimentDoc.h"
#include "PointTrans.h"

// CDlgImagePanning 对话框

IMPLEMENT_DYNAMIC(CDlgImagePanning, CDialogEx)

CDlgImagePanning::CDlgImagePanning(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImagePanning, pParent)
	, m_XOffset(0)
	, m_YOffset(0)
{
}

CDlgImagePanning::~CDlgImagePanning()
{
}

void CDlgImagePanning::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XOffset, m_XOffset);
	DDX_Text(pDX, IDC_YOffset, m_YOffset);
}


BEGIN_MESSAGE_MAP(CDlgImagePanning, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONPRE, &CDlgImagePanning::OnBnClickedButtonpre)
	ON_BN_CLICKED(IDC_BUTTONREC, &CDlgImagePanning::OnBnClickedButtonrec)
END_MESSAGE_MAP()


// CDlgImagePanning 消息处理程序

BOOL WINAPI TranslationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// 平移后剩余图像在源图像中的位置（矩形区域）
	CRect	rectSrc;

	// 平移后剩余图像在新图像中的位置（矩形区域）
	CRect	rectDst;

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向要复制区域的指针
	LPSTR	lpDst;

	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指明图像是否全部移去可视区间
	BOOL	bVisible;

	// 循环变量
	LONG	i;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 赋初值
	bVisible = TRUE;

	// 计算rectSrc和rectDst的X坐标
	if (lXOffset <= -lWidth)
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}
	else if (lXOffset <= 0)
	{
		// 移动后，有图区域左上角X坐标为0
		rectDst.left = 0;

		// 移动后，有图区域右下角X坐标为lWidth - |lXOffset| = lWidth + lXOffset
		rectDst.right = lWidth + lXOffset;
	}
	else if (lXOffset < lWidth)
	{
		// 移动后，有图区域左上角X坐标为lXOffset
		rectDst.left = lXOffset;

		// 移动后，有图区域右下角X坐标为lWidth
		rectDst.right = lWidth;
	}
	else
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}

	// 平移后剩余图像在源图像中的X坐标
	rectSrc.left = rectDst.left - lXOffset;
	rectSrc.right = rectDst.right - lXOffset;

	//  计算rectSrc和rectDst的Y坐标
	if (lYOffset <= -lHeight)
	{
		// Y轴方向全部移出可视区域
		bVisible = FALSE;
	}
	else if (lYOffset <= 0)
	{
		// 移动后，有图区域左上角Y坐标为0
		rectDst.top = 0;

		// 移动后，有图区域右下角Y坐标为lHeight - |lYOffset| = lHeight + lYOffset
		rectDst.bottom = lHeight + lYOffset;
	}
	else if (lYOffset < lHeight)
	{
		// 移动后，有图区域左上角Y坐标为lYOffset
		rectDst.top = lYOffset;

		// 移动后，有图区域右下角Y坐标为lHeight
		rectDst.bottom = lHeight;
	}
	else
	{
		// X轴方向全部移出可视区域
		bVisible = FALSE;
	}

	// 平移后剩余图像在源图像中的Y坐标
	rectSrc.top = rectDst.top - lYOffset;
	rectSrc.bottom = rectDst.bottom - lYOffset;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lLineBytes * lHeight);

	// 如果有部分图像可见
	if (bVisible)
	{

		// 平移图像
		for (i = 0; i < (rectSrc.bottom - rectSrc.top); i++)
		{
			// 要复制区域的起点，注意由于DIB图像内容是上下倒置的，第一行内容是保存在最后
			// 一行，因此复制区域的起点不是lpDIBBits + lLineBytes * (i + rectSrc.top) + 
			// rectSrc.left，而是 lpDIBBits + lLineBytes * (lHeight - i - rectSrc.top - 1) + 
			// rectSrc.left。

			lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - i - rectSrc.top - 1) + rectSrc.left;

			// 要目标区域的起点
			// 同样注意上下倒置的问题。
			lpDst = (char*)lpNewDIBBits + lLineBytes * (lHeight - i - rectDst.top - 1) +
				rectDst.left;

			// 拷贝每一行，宽度为rectSrc.right - rectSrc.left
			memcpy(lpDst, lpSrc, rectSrc.right - rectSrc.left);

		}
	}

	// 复制平移后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
void CDlgImagePanning::OnBnClickedButtonpre()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();   //调用主框架类，获得句柄
	CMFCImgExperimentDoc* chuzhi = (CMFCImgExperimentDoc*)pMainFrame->GetActiveDocument(); //从主框架类里面获得当前doc 

	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	//lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度

	UpdateData(TRUE);
	ImageTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight, m_XOffset, m_YOffset);

	chuzhi->UpdateAllViews(NULL);
}


void CDlgImagePanning::OnBnClickedButtonrec()
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
