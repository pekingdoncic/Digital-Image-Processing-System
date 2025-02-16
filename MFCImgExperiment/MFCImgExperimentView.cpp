
// MFCImgExperimentView.cpp: CMFCImgExperimentView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCImgExperiment.h"
#include "CDlgHistogram.h"
#include "CDlgInverseColor.h"
#include "CDlgInverseVshaped.h"
#include "CDlgVshaped.h"
#include "CDlgInverseLshaped.h"
#include "CDlgLshaped.h"
#include "CDlgLinerPara.h"
#include "CDlgGrayScaleStretch.h"
#include "CDlgBitPlaneSlice.h"
#include "CDlgGammaVary.h"
#include "CDChangeBrightness.h"
#include "CDlgSmooth.h"
#include "CDlgMidFilter.h"
#include "CDlgManualBinarization.h"
#include "CDlgMorphology.h"
#include "CDlgThresholdTrans.h"
#include "CDlgWindowTran.h"
#include "CDlg_Filter.h"
#include "CDlgImagePanning.h"
#include "CDlgImageRotate.h"
#include "CDlgMosaic.h"
#include "CDlgRegionCount.h"
#include "CDlgRegionCountPre.h"
#include "CDlgWaterMark.h"
#endif

#include "MFCImgExperimentDoc.h"
#include "MFCImgExperimentView.h"
#include "CDlgImgInfo.h"
#include "PointTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#endif


// CMFCImgExperimentView

IMPLEMENT_DYNCREATE(CMFCImgExperimentView, CView)

BEGIN_MESSAGE_MAP(CMFCImgExperimentView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DispImgInfo, &CMFCImgExperimentView::OnDispimginfo)
	ON_COMMAND(ID_ChangeBrightness, &CMFCImgExperimentView::OnChangebrightness)
	ON_COMMAND(ID_ExchangeImages, &CMFCImgExperimentView::OnExchangeimages)
	ON_COMMAND(ID_24ColorTo8Color, &CMFCImgExperimentView::On24colorto8color)
	ON_COMMAND(ID_8ColorTo8Grey, &CMFCImgExperimentView::On8colorto8grey)
	ON_COMMAND(ID_HalfTone, &CMFCImgExperimentView::OnHalftone)
	ON_COMMAND(ID_DualizeGrey, &CMFCImgExperimentView::OnDualizegrey)
	ON_COMMAND(ID_Histogram, &CMFCImgExperimentView::OnHistogram)
	ON_COMMAND(ID_InverseColor, &CMFCImgExperimentView::OnInversecolor)
	ON_COMMAND(ID_Inverted_VShaped, &CMFCImgExperimentView::OnInvertedVshaped)
	ON_COMMAND(ID_VShaped, &CMFCImgExperimentView::OnVshaped)
	ON_COMMAND(ID_Inverted_LShaped, &CMFCImgExperimentView::OnInvertedLshaped)
	ON_COMMAND(ID_LShaped, &CMFCImgExperimentView::OnLshaped)
	ON_COMMAND(ID_LinerTrans, &CMFCImgExperimentView::OnLinertrans)
	ON_COMMAND(ID_GrayScaleStretch, &CMFCImgExperimentView::OnGrayscalestretch)
//	ON_COMMAND(ID_HisImageEnhance, &CMFCImgExperimentView::OnHisimageenhance)
ON_COMMAND(ID_BitplaneDecomposition, &CMFCImgExperimentView::OnBitplanedecomposition)
ON_COMMAND(ID_Logvary, &CMFCImgExperimentView::OnLogvary)
ON_COMMAND(ID_GammaVary, &CMFCImgExperimentView::OnGammavary)
ON_COMMAND(ID_Image_SMOOTH, &CMFCImgExperimentView::OnImageSmooth)
ON_COMMAND(ID_MedianFiltering, &CMFCImgExperimentView::OnMedianfiltering)
ON_COMMAND(ID_MAXFilter, &CMFCImgExperimentView::OnMaxfilter)
ON_COMMAND(ID_MINFilter, &CMFCImgExperimentView::OnMinfilter)
//ON_COMMAND(ID_MidPointFilter, &CMFCImgExperimentView::OnMidpointfilter)
ON_COMMAND(ID_MidPointFilter, &CMFCImgExperimentView::OnMidpointfilter)
ON_COMMAND(ID_adaptingFilter, &CMFCImgExperimentView::Onadaptingfilter)
ON_COMMAND(ID_RESTORE_SALTNOISE, &CMFCImgExperimentView::OnRestoreSaltnoise)
ON_COMMAND(ID_EnhaSharp_4, &CMFCImgExperimentView::OnEnhasharp4)
ON_COMMAND(ID_EnhaSharp_8, &CMFCImgExperimentView::OnEnhasharp8)
ON_COMMAND(ID_EDGE_ROBERT, &CMFCImgExperimentView::OnEdgeRobert)
ON_COMMAND(ID_EDGE_Sobel, &CMFCImgExperimentView::OnEdgeSobel)
ON_COMMAND(ID_EDGE_KIRSCH, &CMFCImgExperimentView::OnEdgeKirsch)
ON_COMMAND(ID_EDGE_GAUSS, &CMFCImgExperimentView::OnEdgeGauss)
ON_COMMAND(ID_EDGE_PREWITT, &CMFCImgExperimentView::OnEdgePrewitt)
ON_COMMAND(ID_Edge_Canny, &CMFCImgExperimentView::OnEdgeCanny)
ON_COMMAND(ID_ManualDualizeGrey, &CMFCImgExperimentView::OnManualdualizegrey)
ON_COMMAND(ID_Morphology, &CMFCImgExperimentView::OnMorphology)
ON_COMMAND(ID_ThresholdTransfor, &CMFCImgExperimentView::OnThresholdtransfor)
ON_COMMAND(ID_WindowTransfor, &CMFCImgExperimentView::OnWindowtransfor)
ON_COMMAND(ID_HistogramEqualization, &CMFCImgExperimentView::OnHistogramequalization)
ON_COMMAND(ID_Filter, &CMFCImgExperimentView::OnFilter)
ON_COMMAND(ID_ImageTranslate, &CMFCImgExperimentView::OnImagetranslate)
ON_COMMAND(ID_ImageRevolve, &CMFCImgExperimentView::OnImagerevolve)
ON_COMMAND(ID_Transpose, &CMFCImgExperimentView::OnTranspose)
ON_COMMAND(ID_MirrorX, &CMFCImgExperimentView::OnMirrorx)
ON_COMMAND(ID_MirrorY, &CMFCImgExperimentView::OnMirrory)
ON_COMMAND(ID_OilPainting, &CMFCImgExperimentView::OnOilpainting)
ON_COMMAND(ID_Mosaic, &CMFCImgExperimentView::OnMosaic)
ON_COMMAND(ID_FrostGlass, &CMFCImgExperimentView::OnFrostglass)
ON_COMMAND(ID_FrostRainbow, &CMFCImgExperimentView::OnFrostrainbow)
ON_COMMAND(ID_USM, &CMFCImgExperimentView::OnUsm)
ON_COMMAND(ID_RegionCount, &CMFCImgExperimentView::OnRegioncount)
//ON_COMMAND(ID_AddWatermark, &CMFCImgExperimentView::OnAddwatermark)
//ON_COMMAND(ID_ExtractWatermark, &CMFCImgExperimentView::OnExtractwatermark)
ON_COMMAND(ID_AddMark, &CMFCImgExperimentView::OnAddmark)
ON_COMMAND(ID_ExtractMark, &CMFCImgExperimentView::OnExtractmark)
//ON_COMMAND(ID_ExtractWatermark, &CMFCImgExperimentView::OnExtractwatermark)
END_MESSAGE_MAP()

// CMFCImgExperimentView 构造/析构

CMFCImgExperimentView::CMFCImgExperimentView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMFCImgExperimentView::~CMFCImgExperimentView()
{
}

BOOL CMFCImgExperimentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCImgExperimentView 绘图

void CMFCImgExperimentView::OnDraw(CDC* pDC)
{
	CMFCImgExperimentDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
	//以上部分用原有内容，根据项目名称不同
		// TODO: 在此处为本机数据添加绘制代码
	HGLOBAL hDIB = pDoc->GetHObject();

	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDibSection = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);

		// 获取DIB宽度
		int cxDIB = (int)pDoc->m_dib.GetWidth(lpDibSection);

		// 获取DIB高度
		int cyDIB = (int)pDoc->m_dib.GetHeight(lpDibSection);
		::GlobalUnlock((HGLOBAL)hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest = rcDIB;
		// 输出DIB
		pDoc->m_dib.DrawDib(pDC->m_hDC, &rcDest, pDoc->GetHObject(),
			&rcDIB, pDoc->GetDocPal());
	}

}


// CMFCImgExperimentView 打印

BOOL CMFCImgExperimentView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCImgExperimentView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCImgExperimentView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCImgExperimentView 诊断

#ifdef _DEBUG
void CMFCImgExperimentView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCImgExperimentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCImgExperimentDoc* CMFCImgExperimentView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCImgExperimentDoc)));
	return (CMFCImgExperimentDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCImgExperimentView 消息处理程序


void CMFCImgExperimentView::OnDispimginfo()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	CDlgImgInfo dlgPara;// 创建对话框		
	dlgPara.m_Width = lSrcWidth;
	dlgPara.m_Height = lSrcHeight;
	dlgPara.m_bitCount = lpSrcBitCount;

	if (dlgPara.DoModal() != IDOK)// 显示对话框， 
	{
		return;
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

}


void CMFCImgExperimentView::OnChangebrightness()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
	AfxMessageBox(L"对不起，不是256色位图！");// 警告
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
	return;									//返回
	}
	*/
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/*int i, j;*/
	int offset;
	unsigned char* lpSrc;
	CDChangeBrightness  dlgPara;// 创建对话框			
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	offset = dlgPara.m_Brightness;

	//ChangeGray(lpSrcStartBits, lpSrcBitCount, lSrcWidth, lSrcHeight, offset);//改变亮度
	  // 指向源图像的指针
	//unsigned char* lpSrc;
	// 循环变量
	LONG    i;
	LONG	j;
	// 图像每行的字节数
	LONG	lLineBytes;
	int temp, k;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lSrcWidth * lpSrcBitCount);
	int page = 1;
	if (lpSrcBitCount == 24) page = 3;
	if (lpSrcBitCount == 8) page = 1;
	//逐行扫描
	for (i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			for (k = 0; k < page; k++)
			{
				lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j * page + k;
				// 变换
				temp = *lpSrc + offset;
				if (temp > 255)
				{
					// 直接赋值为255
					*lpSrc = 255;
				}
				else if (temp < 0)
				{
					// 直接赋值为0
					*lpSrc = 0;
				}
				else
				{
					// 四舍五入
					*lpSrc = (unsigned char)(temp + 0.5);
				}

			}
		}
	}
	delete dlgPara;
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

}



void CMFCImgExperimentView::OnExchangeimages()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 24)// 判断是否是24-bpp位图
	{
	AfxMessageBox(L"对不起，不是24位图！");// 警告
	::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
	return;									//返回
	}
	*/
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

//法二：	
	//CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档LPCTSTR 
	//LPCTSTR pathname = pDoc->GetPathName(); 
	//pDoc->OnOpenDocument(pathname);
	//this->OnDraw(this->GetDC());

}


void CMFCImgExperimentView::On24colorto8color()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

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
	
	if (lpSrcBitCount != 24)// 判断是否是24-bpp位图	pDoc->m_dib.GetColorNum(lpSrcDib) != 16777216
	{
		AfxMessageBox(L"对不起，不是24位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

//这种方法建立调色板的时候有问题，导致虽然变换了，但是变换之后软件并不识别为8位彩色位图，因为没有把调色板放进去！
	//Tran8ColTo8Col(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, lpSrcBitCount);// 8位彩色转8位灰度	
	//Tran24ColTo8Col(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, lpSrcBitCount);// 24位彩色转24位彩色
	//指向图像的指针
	//unsigned char* lpSrc;
	////循环变量
	//LONG	i;
	//LONG	j;
	//// 计算图像每行的字节数
	//LONG	lLineBytes, lineBytenew;
	////lLineBytes = WIDTHBYTES(lWidth * 8);
	////(((bits)+31) / 32 * 4)
	//lLineBytes = (lSrcWidth * lpSrcBitCount / 8 + 3) / 4 * 4;
	//lineBytenew = (lSrcWidth + 3) / 4 * 4;
	////lineBytenew = (lSrcWidth * lpSrcBitCount / 8 + 3) / 4 * 4;

	//// 灰度映射表
	//BYTE bMap[256];
	//RGBQUAD* pTable, * pTable1;//颜色表指针

	////LPSTR pBmpBufnew = NULL;
	//unsigned char* pBmpBufnew = new unsigned char[(lSrcWidth + 3) / 4 * 4 * lSrcHeight];
	//pTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER));

	////for (i = 0; i < 256; i++)//生成新的调色板,并转换灰度
	////{
	////	// 计算该颜色对应的灰度值g=0.299*r+0.587*g+0.114*b	
	////	bMap[i] = (BYTE)(0.299 * pTable->rgbRed + 0.587 * pTable->rgbGreen + 0.114 * pTable->rgbBlue + 0.5);
	////	pTable->rgbRed = (BYTE)i;
	////	pTable->rgbGreen = (BYTE)i;
	////	pTable->rgbBlue = (BYTE)i;
	////	pTable->rgbReserved = (BYTE)0;
	////	pTable++;
	////}

	//int x = 0;
	//for (int i = 0; i < 6; i++) {
	//	for (int j = 0; j < 6; j++) {
	//		for (int k = 0; k < 6; k++) {
	//			pTable[x].rgbRed = (BYTE)i * 51;
	//			pTable[x].rgbGreen = (BYTE)j * 51;
	//			pTable[x].rgbBlue = (BYTE)k * 51;
	//			//pTable[x].rgbReserved = (BYTE)0;
	//			//pTable++;
	//			x++;
	//		}
	//	}
	//}

	//int min, flag;
	//for (int i = 0; i < lSrcHeight; i++) {
	//	for (int k = 0; k < lLineBytes; k += 3) {//k += 3
	//		min = 1000000000;
	//		for (int j = 0; j < 216; j++) {//216
	//			int it = 0.114 * pow(lpSrcStartBits[i * lLineBytes + k] - pTable[j].rgbBlue, 2) + 0.587 * pow(lpSrcStartBits[i * lLineBytes + k + 1] - pTable[j].rgbGreen, 2) + 0.299 * pow(lpSrcStartBits[i * lLineBytes + k + 2] - pTable[j].rgbRed, 2);
	//			//int it = 0.114 * pow(lpDIBBits[i * lLineBytes + k] - pTable->rgbBlue, 2) + 0.587 * pow(lpDIBBits[i * lLineBytes + k + 1] - pTable->rgbGreen, 2) + 0.299 * pow(lpDIBBits[i * lLineBytes + k + 2] - pTable->rgbRed, 2);
	//			if (it < min) {
	//				min = it;
	//				flag = j;
	//			}
	//			//pTable++;
	//		}
	//		pBmpBufnew[i * lineBytenew + k / 3] = flag;
	//	}
	//}
	//lpSrcStartBits = (LPSTR)pBmpBufnew;
	//lpSrcBitCount = 8;

// 指向源图像的指针
	//unsigned char* lpSrc;
	//// 循环变量
	//LONG i = 0;
	//LONG j = 0;
	//// 图像每行的字节数
	//LONG lLineBytes;
	//// 计算图像每行的字节数
	//lLineBytes = (((lSrcWidth * 24) + 31) / 32 * 4);
	//BITMAPINFOHEADER* pHead;
	//RGBQUAD* pColorTable = NULL;
	//int lineByteNew;
	//int biBitCount = 8;
	//lineByteNew = (lSrcWidth * biBitCount / 8 + 3) / 4 * 4;
	////灰度图像有颜色表，且颜色表表项为
	//if (biBitCount == 8) {
	//	//申请颜色表所需要的空间，给颜色表赋值
	//	pColorTable = new RGBQUAD[256];
	//	memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
	//}
	//BYTE* Index = new BYTE[lineByteNew * lSrcHeight]; //图像数据区的数据(保存在Index中)
	//WORD* shortColor = new WORD[lineByteNew * lSrcHeight]; //颜色的高4位
	//memset(shortColor, 0, sizeof(WORD) * lineByteNew * lSrcHeight); //清零

	//int iRed, iGreen, iBlue;
	//for (int i = 0; i < lSrcHeight; i++)
	//{//取RGB颜色的高4位  
	//	for (int j = 0; j < lSrcWidth; j++)
	//	{
	//		lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j * 3;
	//		iBlue = (*lpSrc) >> 4;
	//		iGreen = (*(++lpSrc)) >> 4;
	//		iRed = (*(++lpSrc)) >> 4;
	//		shortColor[lineByteNew * (lSrcHeight - 1 - i) + j] = (iBlue << 8) + (iGreen << 4) + iRed;
	//	}
	//}
	////调用转换函数  24color To->8color
	//BitTran(shortColor, lineByteNew* lSrcHeight, Index, pColorTable);

	////申请位图数据所需要的空间，读位图数据进内存
	//unsigned char* pBmpBufNew;
	//pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//申请新图像的空间
	//memcpy(pBmpBufNew, (unsigned char*)lpSrcDib, sizeof(BITMAPINFOHEADER));//信息头拷贝
	//pHead = (BITMAPINFOHEADER*)pBmpBufNew;
	//pHead->biBitCount = 8;//改变位数,
	//pHead->biHeight = lSrcHeight;
	//pHead->biWidth = lSrcWidth;
	//pHead->biClrUsed = 256;
	//pHead->biClrImportant = 0;
	//pHead->biCompression = 0;
	//pHead->biPlanes = 1;
	//pHead->biSize = 40;
	//pHead->biSizeImage = lineByteNew * lSrcHeight;
	//pHead->biXPelsPerMeter = 0;
	//pHead->biYPelsPerMeter = 0;

	////拷贝
	//memcpy(lpSrcDib, pBmpBufNew, sizeof(BITMAPINFOHEADER));
	//memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
	//memcpy(lpSrcDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew* lSrcHeight);

	//delete[]Index;
	//delete[]shortColor;


//法二：
	// // 指向源图像的指针
	//unsigned char* lpSrc;
	//// 循环变量
	//LONG	i;
	//LONG	j;
	//// 图像每行的字节数
	//LONG	lLineBytes;
	//// 计算图像每行的字节数
	//lLineBytes = WIDTHBYTES(lWidth * 24);
	//BITMAPINFOHEADER* pHead;
	//RGBQUAD* pColorTable = NULL;
	//int lineByteNew;
	//int biBitCount = 8;
	//lineByteNew = (lWidth * biBitCount / 8 + 3) / 4 * 4;
	////灰度图像有颜色表，且颜色表表项为
	//if (biBitCount == 8) {
	//	//申请颜色表所需要的空间，给颜色表赋值
	//	pColorTable = new RGBQUAD[256];
	//	memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
	//}
	//BYTE* Index = new BYTE[lineByteNew * lSrcHeight]; //图像数据区的数据(保存在Index中)
	//WORD* shortColor = new WORD[lineByteNew * lSrcHeight]; //颜色的高4位
	//memset(shortColor, 0, sizeof(WORD) * lineByteNew * lSrcHeight); //清零

	//int iRed, iGreen, iBlue;
	//for (int i = 0; i < lSrcHeight; i++)
	//{//取RGB颜色的高4位  
	//	for (int j = 0; j < lSrcWidth; j++)
	//	{
	//		lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j * 3;
	//		iBlue = (*lpSrc) >> 4;
	//		iGreen = (*(++lpSrc)) >> 4;
	//		iRed = (*(++lpSrc)) >> 4;
	//		shortColor[lineByteNew * (lSrcHeight - 1 - i) + j] = (iBlue << 8) + (iGreen << 4) + iRed;
	//	}
	//}
	////调用转换函数  24color To->8color
	//Transfer(shortColor, lineByteNew * lSrcHeight, Index, pColorTable);

	////申请位图数据所需要的空间，读位图数据进内存
	//unsigned char* pBmpBufNew;
	//pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//申请新图像的空间
	//memcpy(pBmpBufNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER));//信息头拷贝
	//pHead = (BITMAPINFOHEADER*)pBmpBufNew;
	//pHead->biBitCount = 8;//改变位数,
	//pHead->biHeight = lSrcHeight;
	//pHead->biWidth = lSrcWidth;
	//pHead->biClrUsed = 256;
	//pHead->biClrImportant = 0;
	//pHead->biCompression = 0;
	//pHead->biPlanes = 1;
	//pHead->biSize = 40;
	//pHead->biSizeImage = lineByteNew * lSrcHeight;
	//pHead->biXPelsPerMeter = 0;
	//pHead->biYPelsPerMeter = 0;

	////拷贝
	//memcpy(lpDIB, pBmpBufNew, sizeof(BITMAPINFOHEADER));
	//memcpy(lpDIB + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
	//memcpy(lpDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew * lHeight);

	//delete[]Index;
	//delete[]shortColor;
	
//法三：
Tran24ColTo8Col(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight);// 24位彩色转8位彩色	 
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::On8colorto8grey()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	unsigned char* lpSrc;

	LONG	i;
	LONG	j;

	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lSrcWidth * 8);

	// 灰度映射表
	BYTE bMap[256];
	RGBQUAD* pTable, * pTable1;//颜色表指针

	pTable = (RGBQUAD*)(lpSrcDib + sizeof(BITMAPINFOHEADER));

	for (i = 0; i < 256; i++)//生成新的调色板,并转换灰度
	{
		// 计算该颜色对应的灰度值g=0.299*r+0.587*g+0.114*b	
		bMap[i] = (BYTE)(0.299 * pTable->rgbRed + 0.587 * pTable->rgbGreen + 0.114 * pTable->rgbBlue + 0.5);
		pTable->rgbRed = (BYTE)i;
		pTable->rgbGreen = (BYTE)i;
		pTable->rgbBlue = (BYTE)i;
		pTable->rgbReserved = (BYTE)0;
		pTable++;
	}

	for (i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j;
			// 变换
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnHalftone()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	//逐行扫描
	LONG i, j;
	unsigned char* lpSrc;
	// 计算图像每行的字节数
	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lSrcWidth * 8);
	int temp, k;
	unsigned char num;
	LONG x, y;
	BYTE BayerPattern[8][8] = {
	0,32,8,40,2,34,10,42,
	48,16,56,24,50,18,58,26,
	12,44,4,36,14,46,6,38,
	60,28,52,20,62,30,54,22,
	3,35,11,43,1,33,9,41,
	51,19,59,27,49,17,57,25,
	15,47,7,39,13,45,5,37,
	63,31,55,23,61,29,53,21
	};
	for (y = 0; y < lSrcHeight; y++)
	{
		//逐列扫描
		for (x = 0; x < lSrcWidth; x++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - y) + x;
			// 变换
			//num = (unsigned char)*lpSrcStartBits++;
			if ((*lpSrc >> 2) > BayerPattern[y & 7][x & 7]) //右移两位后做比较
				*(lpSrc++) = (unsigned char)255; //打白点
			else *(lpSrc++) = (unsigned char)0; //打黑点
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnDualizegrey()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	

	unsigned char* lpSrc;

	LONG	i;
	LONG	j;

	unsigned char Threshold=0;

	/*Threshold = getThreshold(lpDIB, lpDIBBits, lWidth, lHeight);*///得到该二值图阈值
	LONG w = lSrcWidth;
	LONG h = lSrcHeight;
	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lSrcWidth * 8);

	// 指向源图像的指针
	//unsigned char* lpSrc;

	/***************对灰度图的像素值进行统计************************/
	int graycolor[256];//灰度级存储数组
	for (int n = 0; n < 256; n++)  graycolor[n] = 0;//初始化

	unsigned char k;
	for (int i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (int j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j;
			k = *lpSrc;
			graycolor[k]++;
		}
	}
	double Prob_gray[256];              //存储每个直方图占总数的概率
	for (int a = 0; a < 256; a++)
	{
		Prob_gray[a] = (double)graycolor[a] / (w * h);
	}

	double Pa = 0, Pa1 = 0, Pb = 0, Pb1 = 0;      //存储概率
	double Wa[256], Wb[256], Wo[256], Wz[256], Wz1[256];//存储概率的数组,Wa为A区域的平均灰度值，Wb为B区域的平均灰度值，
	//Wo为图像全局的灰度平均值,Wz，Wz1 A、B两个区域的类间方差
	for (int t = 0; t < 256; t++)
	{
		Wa[t] = 0;
		Wb[t] = 0;
		Wo[t] = 0;
		Wz[t] = 0;
		Wz1[t] = 0;
	}
	for (int t = 0; t < 256; t++)
	{
		Pa = 0; Pb = 0; Pa1 = 0; Pb1 = 0;
		for (int p = 0; p < t + 1; p++)
		{
			Pa = Prob_gray[p] * p + Pa;
			Pa1 = Prob_gray[p] + Pa1;
		}
		Wa[t] = Pa / Pa1;        //每个t的前几项的概率方差
		//cout<<Wa[t]<<endl;
		for (int q = t + 1; q < 256; q++)
		{
			Pb = Prob_gray[q] * q + Pb;
			Pb1 = Prob_gray[q] + Pb1;
		}
		Wb[t] = Pb / Pb1;        //每个t的后几项的概率方差
		Wo[t] = Wa[t] * Pa1 + Wb[t] * Pb1;
		Wz[t] = Pa1 * (Wa[t] - Wo[t]) * (Wa[t] - Wo[t]) + Pb1 * (Wb[t] - Wo[t]) * (Wb[t] - Wo[t]);
		//cout<<Wz[t]<<endl;         //输出每个灰度级作为分割线后前后的方差
	}
	double max = 0;         //存储最大的方差值
	//unsigned char   Threshold = 0;   //存储作为阈值的灰度级
	for (int t = 0; t < 256; t++)
	{
		Wz1[t] = Wz[t];
		if (Wz1[t] > max) max = Wz1[t];
	}
	for (int t = 0; t < 256; t++)
	{
		if (Wz[t] >= max)
			Threshold = t;
	}

	//LONG	lLineBytes;
	//lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < lSrcHeight; i++)
	{
		//逐列扫描
		for (j = 0; j < lSrcWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lLineBytes * (lSrcHeight - 1 - i) + j;
			// 变换
			if (*lpSrc > Threshold)
			{
				*lpSrc = 255;
			}
			else
			{
				*lpSrc = 0;
			}
		}
	}
/////////////////////////////////////////////////////////////////////////////////////////////////		
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnHistogram()
{
	// TODO: 在此添加命令处理程序代码
	// 简单显示直方图

	// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;

	LONG i, j;
	LONG m_lCount[256];
	unsigned char* lpSrc;
	LONG m_lWidth, m_lHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());
	if (lpDIB == NULL)
	{
		AfxMessageBox(L"先打开图像！");
		return;
	}
	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB); 

	// 判断是否是8-bit位图（这里为了方便，只处理8-bit位图，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持查看256色位图灰度直方图！");

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();
	//计算直方图
	// 重置计数为0
	for (i = 0; i < 256; i++)
	{
		// 清零
		m_lCount[i] = 0;
	}
	// 图像每行的字节数
	LONG lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(pDoc->m_dib.GetWidth(lpDIB) * 8);
	// 计算各个灰度值的计数
	m_lWidth = pDoc->m_dib.GetWidth(lpDIB);
	m_lHeight = pDoc->m_dib.GetHeight(lpDIB);
	for (i = 0; i < m_lHeight; i++)
	{
		for (j = 0; j < m_lWidth; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * i + j;
			// 计数加1
			m_lCount[*(lpSrc)]++;
		}
	}
	// 创建对话框
	CDlgHistogram dlgHist;
	// 初始化变量值
	dlgHist.m_iLowGray = 0;
	dlgHist.m_iHighGray = 255;
	for (i = 0; i < 256; i++)
	{
		dlgHist.m_lCount[i] = m_lCount[i];
	}
	// 显示对话框
	if (dlgHist.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();


}



void CMFCImgExperimentView::OnInversecolor()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
		//////////////////////////////////////////////////////////////////
	//调用函数处理，图像反色
	FLOAT fA = -1.0;
	FLOAT fB = 255;
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fA, fB);//自主实现
	CDlgInverseColor     dlgPara;// 创建对话框	
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////		
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}



void CMFCImgExperimentView::OnInvertedVshaped()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgInverseVshaped dlgPara;// 创建对话框	
	dlgPara.m_threshold = 128;
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	int threshold = dlgPara.m_threshold;
	int type;
	type = 1;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);
	

	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnVshaped()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgVshaped dlgPara;// 创建对话框	
	dlgPara.m_threshold = 128;
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	int threshold = dlgPara.m_threshold;
	int type;
	type = 2;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);


	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}


void CMFCImgExperimentView::OnInvertedLshaped()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgInverseLshaped dlgPara;// 创建对话框	
	dlgPara.m_threshold = 128;
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	int threshold = dlgPara.m_threshold;
	int type;
	type = 3;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);


	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}


void CMFCImgExperimentView::OnLshaped()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgLshaped dlgPara;// 创建对话框	
	dlgPara.m_threshold = 128;
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	int threshold = dlgPara.m_threshold;
	int type;
	type = 4;
	Segmental2_Linear_Tran(lpSrcStartBits, lSrcWidth, lSrcHeight, type, threshold);


	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

}


void CMFCImgExperimentView::OnLinertrans()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgLinerPara dlgPara;// 创建对话框	
	dlgPara.m_fA= -1;
	dlgPara.m_fB = 255;
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	int fA = dlgPara.m_fA;
	int fB = dlgPara.m_fB;
	LinerTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, fA, fB);

	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}



void CMFCImgExperimentView::OnGrayscalestretch()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	// 创建对话框
	CDlgGrayScaleStretch  dlgPara;

	// 初始化变量值
	dlgPara.m_Point1_X = 50;
	dlgPara.m_Point1_Y = 30;
	dlgPara.m_Point2_X = 200;
	dlgPara.m_Point2_Y = 220;
	// 显示对话框，提示用户设定拉伸位置
	if (dlgPara.DoModal() != IDOK)// 显示对话框 
	{
		return;
	}
	// 点1坐标
	BYTE	Point1_X;
	BYTE	Point1_Y;
	// 点2坐标
	BYTE	Point2_X;
	BYTE	Point2_Y;
	Point1_X = dlgPara.m_Point1_X;
	Point1_Y = dlgPara.m_Point1_Y;
	Point2_X = dlgPara.m_Point2_X;
	Point2_Y = dlgPara.m_Point2_Y;
	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

	// 调用GrayStretch()函数进行灰度拉伸
	GrayStretch(lpSrcStartBits, lSrcWidth, lSrcHeight, Point1_X, Point1_Y, Point2_X, Point2_Y);
	//BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)

	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnBitplanedecomposition()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgBitPlaneSlice dlgPara;

	dlgPara.lSrcLineBytes = lSrcLineBytes;		//图象每行的字节数
	dlgPara.lSrcWidth = lSrcWidth;      //图象的宽度和高度
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lpSrcBitCount = lpSrcBitCount;       //图像的位深
	dlgPara.lpSrcDib = lpSrcDib;		//指向源图象的指针	
	dlgPara.lpSrcStartBits = lpSrcStartBits;	//指向源像素的指针

	//位平面分解
	if (dlgPara.DoModal() != IDOK) // 显示对话框 
	{
		return;
	}

	//////////////////////////
	delete dlgPara;
	
	
	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	EndWaitCursor();
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

void CMFCImgExperimentView::OnLogvary()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	BitPlaneSlicing(lpSrcStartBits, lSrcWidth, lSrcLineBytes, lSrcHeight, 6);
	LogTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight, lSrcLineBytes);
	EndWaitCursor();
	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

void CMFCImgExperimentView::OnGammavary()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	CDlgGammaVary dlgPara;
	//位平面分解
	if (dlgPara.DoModal() != IDOK) // 显示对话框 
	{
		return;
	}
	float gamma = dlgPara.m_fValGamma;

	long i;                 //行循环变量
	long j;                 //列循环变量
	
	unsigned char* lpSrc;//指向图像的指针：
	float val;
	for (i = 0; i < lSrcHeight; i++)// 行
	{
		for (j = 0; j < lSrcWidth; j++)// 列
		{
			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;// 指向DIB第i行，第j个像素的指针
			val = (*lpSrc);
			val = pow(val, gamma);
			val = 255 * val / pow(255, gamma);

			*lpSrc = (int)val;
		}
	}
	//////////////////////////
	delete dlgPara;
	EndWaitCursor();
	//////////////////////////////////////////////////////////////////
	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	//pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}

void CMFCImgExperimentView::OnImageSmooth()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////
	// 创建对话框
	CDlgSmooth dlgPara;
	// 模板元素数组
	FLOAT	aValue[25];
	// 给模板数组赋初值（为平均模板）
	aValue[0] = 1.0;
	aValue[1] = 1.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 1.0;
	aValue[6] = 1.0;
	aValue[7] = 1.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 1.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	// 初始化对话框变量值
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH = 3;
	dlgPara.m_iTempW = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC = (FLOAT)(1.0 / 9.0);
	dlgPara.m_fpArray = aValue;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;
	// 获取用户设定的平移量
	iTempH = dlgPara.m_iTempH;
	iTempW = dlgPara.m_iTempW;
	iTempMX = dlgPara.m_iTempMX;
	iTempMY = dlgPara.m_iTempMY;
	fTempC = dlgPara.m_fTempC;

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

	// 调用Template()函数平滑DIB
	if (::Template(lpSrcStartBits, lSrcWidth, lSrcHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}
	//AfxMessageBox(L"对不起，不是256色位图！");// 警告	
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定	

	// 恢复光标
	EndWaitCursor();

}

void CMFCImgExperimentView::OnMedianfiltering()
{
	//中值滤波
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	long lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////

	// 滤波器的高度
	int iFilterH;

	// 滤波器的宽度
	int iFilterW;

	// 中心元素的X坐标
	int iFilterMX;

	// 中心元素的Y坐标
	int iFilterMY;


	// 创建对话框
	CDlgMidFilter dlgPara;

	// 初始化变量值
	dlgPara.m_iFilterType = 0;
	dlgPara.m_iFilterH = 3;
	dlgPara.m_iFilterW = 1;
	dlgPara.m_iFilterMX = 0;
	dlgPara.m_iFilterMY = 1;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户的设定
	iFilterH = dlgPara.m_iFilterH;
	iFilterW = dlgPara.m_iFilterW;
	iFilterMX = dlgPara.m_iFilterMX;
	iFilterMY = dlgPara.m_iFilterMY;

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();

	// 调用MedianFilter()函数中值滤波
	if (MedianFilter(lpSrcStartBits, lSrcWidth, lSrcHeight,
		iFilterH, iFilterW, iFilterMX, iFilterMY))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}

void CMFCImgExperimentView::OnMaxfilter()
{
	//最大值滤波
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG lWidth;
	LONG lHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lWidth = pDoc->m_dib.GetWidth(lpDIB);
	lHeight = pDoc->m_dib.GetHeight(lpDIB);//获取图像高和宽

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	//模板参数(3X3)
	int iTempMY = 1, iTempMX = 1;//中心点坐标
	int iTempH = 3, iTempW = 3;


	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;


	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);


	LONG maxval = 0;//最大值

	//开始最大值获取
	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;



			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					//// 获取最大值
					if (maxval < *lpSrc) {
						maxval = *lpSrc;
					}
				}
			}
			// 赋值
			*lpDst = (unsigned char)(maxval);
			maxval = 0;
		}

	}
	// 更改光标形状
	BeginWaitCursor();
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

void CMFCImgExperimentView::OnMinfilter()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG lWidth;
	LONG lHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lWidth = pDoc->m_dib.GetWidth(lpDIB);
	lHeight = pDoc->m_dib.GetHeight(lpDIB);//获取图像高和宽

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	//模板参数(3X3)
	int iTempMY = 1, iTempMX = 1;//中心点坐标
	int iTempH = 3, iTempW = 3;


	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;


	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);


	LONG minval = 255;//最小值

	//开始最大值获取
	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;



			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					//// 获取最小值
					if (minval > *lpSrc) {
						minval = *lpSrc;
					}
				}
			}
			// 赋值
			*lpDst = (unsigned char)(minval);
			minval = 255;
		}

	}
	// 更改光标形状
	BeginWaitCursor();
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}

//void CMFCImgExperimentView::OnMidpointfilter()
//{
	// TODO: 在此添加命令处理程序代码
	
//}


void CMFCImgExperimentView::OnMidpointfilter()
{
	//中间点滤波
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG lWidth;
	LONG lHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lWidth = pDoc->m_dib.GetWidth(lpDIB);
	lHeight = pDoc->m_dib.GetHeight(lpDIB);//获取图像高和宽

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	//模板参数(3X3)
	int iTempMY = 1, iTempMX = 1;//中心点坐标
	int iTempH = 3, iTempW = 3;


	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;


	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);


	LONG minval = 255;//最小值

	//开始最大值获取
	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					//// 获取最小值
					if (minval > *lpSrc) {
						minval = *lpSrc;
					}
				}
			}
			// 赋值
			*lpDst = (unsigned char)(minval);
			minval = 255;
		}

	}
	// 更改光标形状
	BeginWaitCursor();
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::Onadaptingfilter()
{
	// TODO: 在此添加命令处理程序代码
	//自适应中值滤波

	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG lWidth;
	LONG lHeight;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	lWidth = pDoc->m_dib.GetWidth(lpDIB);
	lHeight = pDoc->m_dib.GetHeight(lpDIB);//获取图像高和宽

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	//模板参数(3X3)
	int iTempMY = 1, iTempMX = 1;//中心点坐标
	int iTempH = 3, iTempW = 3;


	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;

	// 指向滤波器数组的指针
	unsigned char* aValue;
	HLOCAL			hArray;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	// 暂时分配内存，以保存滤波器数组
	hArray = LocalAlloc(LHND, iTempH * iTempW);
	if (hArray == NULL)
	{
		// 释放内存
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);

		// 分配内存失败
		return;
	}

	// 锁定内存
	aValue = (unsigned char*)LocalLock(hArray);
	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);


	LONG minval = 255;//最小值
	LONG maxval = 0;//最大值
	LONG centerval = 0;//中心值

	//开始最大值获取
	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k) + j - iTempMX + l;
					// 保存象素值
					aValue[k * iTempW + l] = *lpSrc;
					//// 获取最小值
					if (minval > *lpSrc) {
						minval = *lpSrc;
					}
					//// 获取最大值
					if (maxval < *lpSrc) {
						maxval = *lpSrc;
					}
				}
			}

			if (*lpDst == minval || *lpDst == maxval) {
				*lpDst = GetMedianNum(aValue, iTempH * iTempW);
			}
			// 重新赋值
			maxval = 0;
			minval = 255;
			centerval = 0;
		}

	}
	// 更改光标形状
	BeginWaitCursor();
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}




void CMFCImgExperimentView::OnRestoreSaltnoise()
{
	// TODO: 在此添加命令处理程序代码
	//图像加噪操作，在图像中加入椒盐噪声

	// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的模糊操作，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 找到DIB图像象素起始位置
	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	// 调用SaltNoiseDIB()函数对DIB进行加噪处理
	if (SaltNoiseDIB(lpDIBBits, lSrcWidth, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！"); 
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEnhasharp4()
{
	// TODO: 在此添加命令处理程序代码
	// 图像拉普拉斯4领域锐化

	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG DIBWidth;
	LONG DIBHeight;

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	// 模板元素数组
	FLOAT	aValue[9];

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	
	DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"对不起，不是256色位图！");// 警告

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}


	// 更改光标形状
	BeginWaitCursor();

	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aValue[0] = 0;
	aValue[1] = -1.0;
	aValue[2] = 0;
	aValue[3] = -1.0;
	aValue[4] = 5.0;
	aValue[5] = -1.0;
	aValue[6] = 0;
	aValue[7] = -1.0;
	aValue[8] = 0;

	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (::Template(lpDIBBits, DIBWidth, DIBHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！系统提示");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEnhasharp8()
{
	// TODO: 在此添加命令处理程序代码
	// 图像拉普拉斯8领域锐化

	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档
	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;

	//源图像高宽
	LONG DIBWidth;
	LONG DIBHeight;

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	// 模板元素数组
	FLOAT	aValue[9];

	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	
	DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"对不起，不是256色位图！");// 警告

		// 解除锁定
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}


	// 更改光标形状
	BeginWaitCursor();

	// 设置拉普拉斯模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aValue[0] = -1.0;
	aValue[1] = -1.0;
	aValue[2] = -1.0;
	aValue[3] = -1.0;
	aValue[4] = 9.0;
	aValue[5] = -1.0;
	aValue[6] = -1.0;
	aValue[7] = -1.0;
	aValue[8] = -1.0;

	// 调用Template()函数用拉普拉斯模板锐化DIB
	if (::Template(lpDIBBits, DIBWidth, DIBHeight,
		iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！系统提示");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEdgeRobert()
{
	// TODO: 在此添加命令处理程序代码
	// //Robert边缘检测运算
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
		// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回

		// 更改光标形状
	BeginWaitCursor();

	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	

	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 调用RobertDIB()函数对DIB进行边缘检测
	if (RobertDIB(lpDIBBits, lSrcLineBytes, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEdgeSobel()
{
	// TODO: 在此添加命令处理程序代码
		//Sobel边缘检测运算

	// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回

	// 更改光标形状
	BeginWaitCursor();


	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	

	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 调用SobelDIB()函数对DIB进行边缘检测
	if (SobelDIB(lpDIBBits, lSrcLineBytes, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEdgeKirsch()
{
	// TODO: 在此添加命令处理程序代码
		//Kirsch边缘检测运算

// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回

	// 更改光标形状
	BeginWaitCursor();


	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	

	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	// 更改光标形状
	BeginWaitCursor();

	// 调用KirschDIB()函数对DIB进行边缘检测
	if (KirschDIB(lpDIBBits, lSrcLineBytes, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEdgeGauss()
{
	// TODO: 在此添加命令处理程序代码
	//Gauss边缘检测运算

	// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回

	// 更改光标形状
	BeginWaitCursor();


	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	

	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	// 更改光标形状
	BeginWaitCursor();
	// 调用PrewittDIB()函数对DIB进行边缘检测
	if (GaussDIB(lpDIBBits, lSrcLineBytes, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();

}


void CMFCImgExperimentView::OnEdgePrewitt()
{
	// TODO: 在此添加命令处理程序代码
	//Prewitt边缘检测运算

	// 获取文档
	CMFCImgExperimentDoc* pDoc = GetDocument();

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的边缘检测，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回

	// 更改光标形状
	BeginWaitCursor();


	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	

	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	// 更改光标形状
	BeginWaitCursor();
	// 调用PrewittDIB()函数对DIB进行边缘检测
	if (PrewittDIB(lpDIBBits, lSrcLineBytes, lSrcHeight))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnEdgeCanny()
{
	// TODO: 在此添加命令处理程序代码
	//Canny边缘算子
	CMFCImgExperimentDoc* pDoc = GetDocument();
	long lSrcLineBytes;		//图象每行的字节数
	long lSrcWidth;      //图象的宽度和高度
	long lSrcHeight;
	int lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	//判断是否是8-bpp位图,不是则返回	
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 更改光标形状
	BeginWaitCursor();

	// 循环控制变量
	int y;
	int x;

	// 开辟内存，存储图像数据
	unsigned char* pUnchImage = new unsigned char[lSrcWidth * lSrcHeight];

	for (y = 0; y < lSrcHeight; y++)
	{
		for (x = 0; x < lSrcWidth; x++)
		{
			pUnchImage[y * lSrcWidth + x] = lpSrcStartBits[y * lSrcLineBytes + x];
		}
	}

	// canny 算子计算后的结果
	unsigned char* pUnchEdge = new unsigned char[lSrcWidth * lSrcHeight];

	// 调用 canny 函数进行边界提取
	Canny(pUnchImage, lSrcWidth, lSrcHeight, 0.4, 0.4, 0.79, pUnchEdge);

	for (y = 0; y < lSrcHeight; y++)
	{
		for (x = 0; x < lSrcWidth; x++)
		{
			lpSrcStartBits[y * lSrcWidth + x] = (unsigned char)(255 - pUnchEdge[y * lSrcWidth + x]);
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	delete[]pUnchEdge;
	pUnchEdge = NULL;
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标形状
	EndWaitCursor();

	// 刷新屏幕
	Invalidate();
}


void CMFCImgExperimentView::OnManualdualizegrey()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	int i, j;
	int offset;

	CDlgManualBinarization dlgPara;// 创建对话框			
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	offset = dlgPara.m_offset;

	ManualBinarization(lpSrcDib, lpSrcStartBits, lSrcWidth, lSrcHeight, offset);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
}


void CMFCImgExperimentView::OnMorphology()
{
	// TODO: 在此添加命令处理程序代码
	//形态学运算
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR	lpDIB;

	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	int lLineBytes;//


	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	// 找到DIB图像象素起始位置
	lpDIBBits = lpDIBBits = pDoc->m_dib.GetBits(lpDIB);

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的平滑！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}
	int nMode = 0;
	int intType = 0;
	// 创建对话框
	CDlgMorphology dlgPara;
	//初始化变量
	dlgPara.m_nMode = 0;
	dlgPara.m_Type = 0;

	
	

	// 模板元素数组
	int	aValue[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			aValue[i][j] = 0;
			//int int_value1 = aValue[i][j];          //int_value为需要现实的int值
			//CString temp_value1 = _T("");   //temp_value用来处理int值
			//temp_value1.Format(_T("%d"), int_value1);//固定格式
			//AfxMessageBox(temp_value1);
		}
	}
	//AfxMessageBox(L"数组0,0 的值：");
	//int int_value4 = aValue[0][0];          //int_value为需要现实的int值
	//CString temp_value4 = _T("");   //temp_value用来处理int值
	//temp_value4.Format(_T("%d"), int_value4);//固定格式
	//AfxMessageBox(temp_value4);


	// 初始化对话框变量值

	//获取前景颜色：
	BOOL option;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{

		// 返回
		return;
	}
	// 获取用户设定
	nMode = dlgPara.m_nMode;
	intType = dlgPara.m_Type;
	option = dlgPara.option;

	/*dlgPara.m_nStructure1 = 0;
	dlgPara.m_nStructure2 = 0;
	dlgPara.m_nStructure3 = 0;
	dlgPara.m_nStructure4 = 0;
	dlgPara.m_nStructure5 = 0;
	dlgPara.m_nStructure6 = 0;
	dlgPara.m_nStructure7 = 0;
	dlgPara.m_nStructure8 = 0;
	dlgPara.m_nStructure9 = 0;*/
	/*if(option==TRUE)
		AfxMessageBox(L"option==TRUE");
	if(option==FALSE)
		AfxMessageBox(L"option==FALSE");*/
	if (nMode == 2)
	{

		aValue[0][0] = dlgPara.m_nStructure1;
		aValue[0][1] = dlgPara.m_nStructure2;
		aValue[0][2] = dlgPara.m_nStructure3;
		aValue[1][0] = dlgPara.m_nStructure4;
		aValue[1][1] = dlgPara.m_nStructure5;
		aValue[1][2] = dlgPara.m_nStructure6;
		aValue[2][0] = dlgPara.m_nStructure7;
		aValue[2][1] = dlgPara.m_nStructure8;
		aValue[2][2] = dlgPara.m_nStructure9;
		//AfxMessageBox(L"对话框的值");
		//int int_value = dlgPara.m_nStructure1;          //int_value为需要现实的int值
		//CString temp_value = _T("");   //temp_value用来处理int值
		//temp_value.Format(_T("%d"), int_value);//固定格式
		//AfxMessageBox(temp_value);
		//AfxMessageBox(L"数组的值");
		//int int_value1 = aValue[0][0];          //int_value为需要现实的int值
		//CString temp_value1 = _T("");   //temp_value用来处理int值
		//temp_value1.Format(_T("%d"), int_value1);//固定格式
		//AfxMessageBox(temp_value1);
	
		//aValue[0][0] = 1;
		//AfxMessageBox(L"对话框的值");
		//int int_value1 = aValue[0][0];          //int_value为需要现实的int值
		//CString temp_value1 = _T("");   //temp_value用来处理int值
		//temp_value1.Format(_T("%d"), int_value1);//固定格式
		//AfxMessageBox(temp_value1);
	/*	aValue[0][1] = 0;
		aValue[0][2] = 1;
		aValue[1][0] = 0;
		aValue[1][1] = 1;
		aValue[1][2] = 0;
		aValue[2][0] = 1;
		aValue[2][1] = 0;
		aValue[2][2] = 1;*/
	}
	//AfxMessageBox(L"数组0,0 的值：");
	//int int_value = aValue[0][0];          //int_value为需要现实的int值
	//CString temp_value = _T("");   //temp_value用来处理int值
	//temp_value.Format(_T("%d"), int_value);//固定格式
	//AfxMessageBox(temp_value);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (aValue[i][j] == -1)
			{
				aValue[i][j] = 0;
			}
			if (aValue[i][j] == 0)
			{
				aValue[i][j] = 1;
			}
		}
	}
	
	for (int i = 0; i < 3; i++) {//判断模板数据是否合规
		for (int j = 0; j < 3; j++) {
			if (aValue[i][j] != 1 && aValue[i][j] != 0) {
			
				//AfxMessageBox(L"数组的位置i：");
				//int int_value = i;          //int_value为需要现实的int值
				//CString temp_value = _T("");   //temp_value用来处理int值
				//temp_value.Format(_T("%d"), int_value);//固定格式
				//AfxMessageBox(temp_value);
				//AfxMessageBox(L"数组的位置j：");
				//int int_valuej = j;          //int_value为需要现实的int值
				//CString temp_valuej = _T("");   //temp_value用来处理int值
				//temp_valuej.Format(_T("%d"), int_valuej);//固定格式
				//AfxMessageBox(temp_valuej);
				//AfxMessageBox(L"数组的值");
				//int int_value1 = aValue[i][j];          //int_value为需要现实的int值
				//CString temp_value1 = _T("");   //temp_value用来处理int值
				//temp_value1.Format(_T("%d"), int_value1);//固定格式
				//AfxMessageBox(temp_value1);
				/*AfxMessageBox(L"第%d个元素为： ",i+j);*/
				// 提示用户
				AfxMessageBox(L"结构元素中有除 1 和 0 外的违规数据！");
				// 解除锁定

				::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

				// 返回
				return;
			}
		}
	}
	// 删除对话框
	delete dlgPara;
	long DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	int lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	lLineBytes = pDoc->m_dib.GetReqByteWidth(DIBWidth * lpSrcBitCount);		// 计算图象每行的字节数
	// 更改光标形状
	BeginWaitCursor();

	// 调用morph()函数
	switch (intType) {
	case 0:
		//调用ErosionDIB()函数腐蚀DIB
		if (ErosionDIB(lpDIBBits, DIBWidth, DIBHeight, nMode, aValue, option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		break;
	case 1:
		// 调用DilationDIB()函数膨胀DIB
		if (DilationDIB(lpDIBBits, DIBWidth, DIBHeight, nMode, aValue, option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
				// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		break;
	case 2:
		// 调用OpenDIB()函数对DIB进行开运算
		if (OpenDIB(lpDIBBits, DIBWidth, DIBHeight, nMode, aValue,option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
				// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		break;
	case 3:

		// 调用CloseDIB()函数对DIB进行闭运算
		if (CloseDIB(lpDIBBits, DIBWidth, DIBHeight, nMode, aValue,option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
				// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		break;
	case 4:
		//内边界提取
		// // 指向缓存DIB图像的指针
		LPSTR	lpNewDIBBits1;
		HLOCAL	hNewDIBBits1;
		// 指向缓存图像的指针
		LPSTR	lpDst1;
		LPSTR	lpDst;
		// // 暂时分配内存，以保存新图像
		hNewDIBBits1 = LocalAlloc(LHND, lLineBytes * DIBHeight);

		if (hNewDIBBits1 == NULL)
		{
			// 分配内存失败
			return;
		}

		// 锁定内存
		lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);
		// 拷贝源图像到缓存图像中
		lpDst1 = (char*)lpNewDIBBits1;
		memcpy(lpNewDIBBits1, lpDIBBits, DIBWidth * DIBHeight);
		//调用ErosionDIB()函数腐蚀DIB
		if (ErosionDIB(lpNewDIBBits1, DIBWidth, DIBHeight, nMode, aValue,option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		//求两幅缓存图像的最大值
		for (int j = 0; j < DIBHeight; j++)
		{
			for (int i = 0; i < DIBWidth; i++)
			{

				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst1 = (char*)lpNewDIBBits1 + DIBWidth * j + i;
				// 指向源图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpDIBBits + DIBWidth * j + i;
				if (*lpDst != *lpDst1) {
					*lpDst1 = (unsigned char)0;
				}
				else {
					*lpDst1 = (unsigned char)255;
				}

			}
		}

		// 复制经过模板运算后的图像到源图像
		memcpy(lpDIBBits, lpNewDIBBits1, DIBWidth * DIBHeight);
		// 释放内存
		LocalUnlock(hNewDIBBits1);
		LocalFree(hNewDIBBits1);
		break;
	case 5:
		//外边界提取

		// // 暂时分配内存，以保存新图像
		hNewDIBBits1 = LocalAlloc(LHND, lLineBytes * DIBHeight);

		if (hNewDIBBits1 == NULL)
		{
			// 分配内存失败
			return;
		}

		// 锁定内存
		lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);
		// 拷贝源图像到缓存图像中
		lpDst1 = (char*)lpNewDIBBits1;
		memcpy(lpNewDIBBits1, lpDIBBits, DIBWidth * DIBHeight);
		// 调用DilationDIB()函数膨胀DIB
		if (DilationDIB(lpNewDIBBits1, DIBWidth, DIBHeight, nMode, aValue, option))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
			AfxMessageBox(L"分配内存失败！");

		}
		//求两幅缓存图像的最大值
		for (int j = 0; j < DIBHeight; j++)
		{
			for (int i = 0; i < DIBWidth; i++)
			{

				// 指向缓存图像1倒数第j行，第i个象素的指针			
				lpDst1 = (char*)lpNewDIBBits1 + DIBWidth * j + i;
				// 指向源图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpDIBBits + DIBWidth * j + i;
				if (*lpDst != *lpDst1) {
					*lpDst1 = (unsigned char)0;
				}
				else {
					*lpDst1 = (unsigned char)255;
				}

			}
		}

		// 复制经过模板运算后的图像到源图像
		memcpy(lpDIBBits, lpNewDIBBits1, DIBWidth * DIBHeight);
		// 释放内存
		LocalUnlock(hNewDIBBits1);
		LocalFree(hNewDIBBits1);
		break;
	default:break;
	}


	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnThresholdtransfor()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	/*BYTE* Byte;
	Byte = new BYTE[lSrcHeight * lSrcLineBytes];
	for (int i = 0; i < lSrcHeight * lSrcLineBytes; i++)
	{
		*(Byte + i) = *(lpSrcStartBits + i);
	}
	Q.push_back(Byte);*/

	CDlgThresholdTrans dlgPara;
	BYTE bThre;//阈值

	//long	lSrcLineBytes;		//图象每行的字节数
	//long	lSrcWidth;      //图象的宽度和高度
	//long	lSrcHeight;
	//int     lpSrcBitCount;       //图像的位深

	dlgPara.m_Threshold = 128;//初始化
	dlgPara.lSrcHeight = lSrcHeight;//赋值给对话框高度
	dlgPara.lSrcWidth = lSrcWidth;
	dlgPara.lSrcLineBytes = lSrcLineBytes;
	dlgPara.lpSrcBitCount = lpSrcBitCount;

	dlgPara.SetDib(lpSrcDib);

	//显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		return;
	}

	if (dlgPara.m_nMode == 0 || dlgPara.m_nMode == 1 || dlgPara.m_nMode == 3)//使用非自适应算法
	{
		bThre = dlgPara.m_Threshold;
		pDoc->m_dib.Thre_Trans(lpSrcStartBits, lSrcWidth, lSrcHeight, bThre);//通过阈值直接修改图像
	}
	if (dlgPara.m_nMode == 2)//自适应阈值分割
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
	delete dlgPara;
	BeginWaitCursor();
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
	EndWaitCursor();
	
}


void CMFCImgExperimentView::OnWindowtransfor()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	CDlgWindowTran dlgPara;//创建窗口

	dlgPara.m_bLow = 0;
	dlgPara.m_bUp = 255;


	// 显示对话框，提示用户设定窗口上下限
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	BYTE bLow;
	BYTE bUp;

	bLow = dlgPara.m_bLow;
	bUp = dlgPara.m_bUp;

	// 删除对话框
	delete dlgPara;

	// 更改光标形状
	BeginWaitCursor();


	WindowTrans(lpSrcStartBits, lSrcWidth, lSrcHeight, bLow, bUp);



	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnHistogramequalization()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	GrayEqualization(lpSrcStartBits, lSrcWidth, lSrcHeight);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnFilter()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档
	LONG	lSrcWidth;      //图象的宽度和高度
	LONG	lSrcHeight;
	////////////////////////////////////////////////////////////////////////////////////////////////	
	int    iTempH;		//模板高度
	int    iTempW;		//模板宽度
	FLOAT	fTempC;		//模板系数
	FLOAT   yuzhi;      //阈值
	int     iTempMX;       //模板元素中心X坐标
	int     iTempMY;       //模板元素中心Y坐标
	FLOAT	aValue[9];		//模板元素数组
	LPSTR	lpDIB;		//指向源图象的指针	
	LPSTR   lpDIBBits;	//指向源像素的指针

	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB

	if (!lpDIB) return;
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是256色位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}										//判断是否是8-bpp位图,不是则返回	

	lpDIBBits = pDoc->m_dib.GetBits(lpDIB);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度

	// 设定平移量（3*3）
	iTempH = 3;
	iTempW = 3;
	iTempMX = 1;
	iTempMY = 1;

	CDlg_Filter  dlgPara;// 创建对话框	

	//传给对话框数据
	dlgPara.lpSrcStartBits = lpDIBBits;
	dlgPara.lSrcHeight = lSrcHeight;
	dlgPara.lSrcWidth = lSrcWidth;

	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}

	// 给模板数组赋初值（为平均模板）
	aValue[0] = dlgPara.num1;
	aValue[1] = dlgPara.num2;
	aValue[2] = dlgPara.num3;
	aValue[3] = dlgPara.num4;
	aValue[4] = dlgPara.num5;
	aValue[5] = dlgPara.num6;
	aValue[6] = dlgPara.num7;
	aValue[7] = dlgPara.num8;
	aValue[8] = dlgPara.num9;
	fTempC = dlgPara.m_Coefficient;
	yuzhi = dlgPara.m_Thre;

	// 更改光标形状
	BeginWaitCursor();

	if (dlgPara.flag1 != 1)
	{
		// 调用Template()函数平滑DIB
		if (MyTemplate(lpDIBBits, lSrcWidth, lSrcHeight, iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC, yuzhi))
		{

			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);

			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		else
		{
			// 提示用户
			AfxMessageBox(L"内存分配错误！");// 警告	
		}
	}

	//恢复初始化
	dlgPara.flag1 = 0;
	dlgPara.m_Coefficient = 0;
	dlgPara.m_Thre = 0;
	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnImagetranslate()
{
	//图像平移
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数


	LONG XOffset, YOffset;
	//创建对话框
	CDlgImagePanning dlgPara;
	
	dlgPara.SetDib(lpSrcDib);
	
	dlgPara.m_XOffset = 0;
	dlgPara.m_YOffset = 0;

	// 显示对话框，提示用户设定平移量
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}
	// 获取用户设定的平移量
	XOffset = dlgPara.m_XOffset;
	YOffset = dlgPara.m_YOffset;

	//ImageTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight, XOffset, YOffset);


	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnImagerevolve()
{	//图像旋转：
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	//long	lSrcLineBytes;		//图象每行的字节数
	//long	lSrcWidth;      //图象的宽度和高度
	//long	lSrcHeight;
	//int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	//LPSTR	lpSrcStartBits;	//指向源像素的指针
	//unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	//lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置
	//lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	//lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	//lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	//lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 缩放比率
	int iRotateAngle;

	CDlgImageRotate dlgPara;// 创建对话框	
	dlgPara.SetDib(lpSrcDib);
	dlgPara.SetDoc(pDoc);
	// 初始化变量值
	dlgPara.m_RotateAngle = 90;
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定旋转角度
	{
		return;
	}
	////获取图像指针：
	//dlgPara.SetDib(lpSrcDib);
	// 获取用户设定的旋转量
	iRotateAngle = dlgPara.m_RotateAngle;
	
	// 删除对话框
	delete dlgPara;

	//::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

	// 创建新DIB
	HGLOBAL hNewDIB = NULL;
	//RotateDIB1(lpSrcDib, dlgPara.m_jiaodu);

	// 更改光标形状
	BeginWaitCursor();

	// 调用RotateDIB()函数旋转DIB
	//hNewDIB = (HGLOBAL)RotateDIB(pDoc,lpSrcDib, iRotateAngle);
	hNewDIB = (HGLOBAL)RotateDIB(lpSrcDib, iRotateAngle);

	// 判断旋转是否成功
	if (hNewDIB != NULL)
	{

		// 替换DIB，同时释放旧DIB对象
		
		pDoc->UpdateObject(hNewDIB);

		// 更新DIB大小和调色板
		//pDoc->InitDIBData();
		pDoc->SetDib();

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 重新设置滚动视图大小
		//SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");// 警告
	}

	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnTranspose()
{
	// TODO: 在此添加命令处理程序代码
	// 图像转置
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;



	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	int lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	long lLineBytes = pDoc->m_dib.GetReqByteWidth(DIBWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的操作！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 调用TransposeDIB()函数转置DIB
	if (TransposeDIB(lpDIB, lpDIBBits, DIBWidth, DIBHeight))

	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}


	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnMirrorx()
{
	//水平镜像
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;



	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	int lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	long lLineBytes = pDoc->m_dib.GetReqByteWidth(DIBWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的操作！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 调用MirrorDIB()函数水平镜像DIB
	if (MirrorDIB(lpDIBBits, DIBWidth, DIBHeight, TRUE))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}


	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnMirrory()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	// 指向DIB的指针
	LPSTR lpDIB;

	// 指向DIB象素指针
	LPSTR    lpDIBBits;



	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());

	// 找到DIB图像象素起始位置
	lpDIBBits = lpDIBBits = pDoc->m_dib.GetBits(lpDIB);
	long DIBWidth = pDoc->m_dib.GetWidth(lpDIB);					// 获取图象的宽度		
	long DIBHeight = pDoc->m_dib.GetHeight(lpDIB);					// 获取图象的高度	
	int lpSrcBitCount = pDoc->m_dib.GetBitCount(lpDIB);                    //获取图像位深
	long lLineBytes = pDoc->m_dib.GetReqByteWidth(DIBWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
	if (pDoc->m_dib.GetColorNum(lpDIB) != 256)
	{
		// 提示用户
		AfxMessageBox(L"目前只支持256色位图的操作！");

		// 解除锁定

		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

		// 返回
		return;
	}

	// 更改光标形状
	BeginWaitCursor();

	// 调用MirrorDIB()函数水平镜像DIB
	if (MirrorDIB(lpDIBBits, DIBWidth, DIBHeight, FALSE))
	{

		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);

		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		AfxMessageBox(L"分配内存失败！");
	}


	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnOilpainting()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	//if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	//{
	//	AfxMessageBox(L"对不起，不是8位图！");// 警告				
	//	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
	//	return;									//返回
	//}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	Oil_Painting(lpSrcStartBits, lpSrcBitCount, lSrcWidth, lSrcHeight);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
}


void CMFCImgExperimentView::OnMosaic()
{
	// TODO: 在此添加命令处理程序代码

	LONG i, j;

	unsigned char* lpSrc;
	unsigned char* lpDst;

	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
	int rectangleLenth = 9;//方格大小
	int Mx = 0, My = 0;//方格中心
	// 创建对话框
	CDlgMosaic  dlgPara;
	// 初始化变量值
	dlgPara.m_rect = 3;


	// 显示对话框，提示用户设定窗口上下限
	if (dlgPara.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户设定的旋转量
	rectangleLenth = dlgPara.m_rect;

	// 删除对话框
	delete dlgPara;



	// 更改光标形状
	BeginWaitCursor();

	Mx = My = rectangleLenth / 2;
	LONG fTemp;
	//每行
	int gap = 1;
	if (lpSrcBitCount == 8)gap = 1;
	else if (lpSrcBitCount == 24)gap = 3;
	for (i = Mx; i < lSrcHeight - rectangleLenth + Mx + 1; i += rectangleLenth) {
		// 每列
		for (j = My; j < lSrcWidth - rectangleLenth + My + 1; j += rectangleLenth) {
			// 指向DIB第i行，第j个像素的指针
			lpDst = (unsigned char*)lpSrcStartBits + lSrcLineBytes *
				(lSrcHeight - 1 - i) + j * gap;

			for (int k = 0; k < rectangleLenth; k++) {
				for (int m = 0; m < rectangleLenth; m++) {
					lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes *
						(lSrcHeight - 1 - i - My + k) + (j - Mx + m) * gap;
					for (int g = 0; g < gap; g++) {
						//在这里进行颜色的修改
						fTemp = *(lpDst + g);
						*(lpSrc + g) = (unsigned)fTemp;
					}
				}
			}
		}
	}
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnFrostglass()
{
	// TODO: 在此添加命令处理程序代码
	// 毛玻璃风格
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	LONG i, j;

	unsigned char* lpSrc;
	unsigned char* lpDst;

	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	/*
	if (pDoc->m_dib.GetColorNum(lpSrcDib) != 256)// 判断是否是8-bpp位图
	{
		   AfxMessageBox(L"对不起，不是256色位图！");// 警告
		   ::GlobalUnlock((HGLOBAL) pDoc->GetHObject());// 解除锁定
		   return;									//返回
	 }										//判断是否是8-bpp位图,不是则返回
	 */
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	/////////////////////////////////////////////////////////////////////////////////////////////////	
		// 更改光标形状
	BeginWaitCursor();
	int rectangleLenth = 8;//方格大小

	srand(time(0));//生成随机数种子
	FLOAT fTemp;
	//每行
	int gap = 1;
	if (lpSrcBitCount == 8)gap = 1;
	else if (lpSrcBitCount == 24)gap = 3;
	for (i = 0; i < lSrcHeight; i++) {
		// 每列
		for (j = 0; j < lSrcWidth; j++) {
			// 指向DIB第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j * gap;
			int index = rand() % rectangleLenth + 1;
			if (i + index >= lSrcHeight || j + index >= lSrcWidth) {
				continue;
			}
			lpDst = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i - index) + (j + index) * gap;
			for (int g = 0; g < gap; g++) {
				//在这里进行颜色的修改
				fTemp = *(lpDst + g);
				*(lpSrc + g) = (unsigned)fTemp;

			}

		}
	}
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnFrostrainbow()
{
	// TODO: 在此添加命令处理程序代码
	LONG i, j;

	unsigned char* lpSrc;//当前像素
	unsigned char* lpDstR;//同行像素
	unsigned char* lpDstD;//同列像素
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
	long	lSrcLineBytes;		//图象每行的字节数
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
		// 更改光标形状
	BeginWaitCursor();
	struct rgb {
		FLOAT r;
		FLOAT g;
		FLOAT b;
	}rgb[3]; //每个像素的rgb值


	//判断位深
	int gap = 1;
	if (lpSrcBitCount == 8)gap = 1;
	else if (lpSrcBitCount == 24) {
		gap = 3;
	}
	for (i = 0; i < lSrcHeight - 1; i++) {
		// 每列
		for (j = 0; j < lSrcWidth - 1; j++) {
			//初始化
			for (int p = 0; p < 3; p++) {

				rgb[p].r = 0;
				rgb[p].g = 0;
				rgb[p].b = 0;
			}
			// 指向DIB第i行，第j个像素的指针
			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + (j)*gap;
			lpDstR = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + (j + 1) * gap;
			lpDstD = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i - 1) + (j)*gap;
			//三个像素rgb赋值
			rgb[0].r = *(lpSrc); rgb[1].r = *(lpDstR); rgb[2].r = *(lpDstD);
			rgb[0].g = *(lpSrc + 1); rgb[1].g = *(lpDstR + 1); rgb[2].g = *(lpDstD + 1);
			rgb[0].b = *(lpSrc + 2); rgb[1].b = *(lpDstR + 2); rgb[2].b = *(lpDstD + 2);
			//相邻像素平方和计算
			for (int p = 0; p < 3; p++) {
				rgb[0].r = pow(rgb[0].r - rgb[1].r, 2) + pow(rgb[0].r - rgb[2].r, 2);
				rgb[0].g = pow(rgb[0].g - rgb[1].g, 2) + pow(rgb[0].g - rgb[2].g, 2);
				rgb[0].b = pow(rgb[0].b - rgb[1].b, 2) + pow(rgb[0].b - rgb[2].b, 2);
			}
			//相邻像素梯度计算
			for (int p = 0; p < 3; p++) {
				rgb[0].r = 2 * sqrt(rgb[0].r);
				rgb[0].g = 2 * sqrt(rgb[0].g);
				rgb[0].b = 2 * sqrt(rgb[0].b);

			}
			//边界处理
			for (int p = 0; p < 3; p++) {
				rgb[0].r = rgb[0].r > 255 ? 255 : rgb[0].r;
				rgb[0].b = rgb[0].b > 255 ? 255 : rgb[0].b;
				rgb[0].g = rgb[0].g > 255 ? 255 : rgb[0].g;

				rgb[0].r = rgb[0].r < 0 ? 0 : rgb[0].r;
				rgb[0].b = rgb[0].b < 0 ? 0 : rgb[0].b;
				rgb[0].g = rgb[0].g < 0 ? 0 : rgb[0].g;
			}
			//赋值

			*(lpSrc + 0) = (unsigned char)rgb[0].r;
			if (gap == 3) {
				*(lpSrc + 1) = (unsigned char)rgb[0].g;
				*(lpSrc + 2) = (unsigned char)rgb[0].b;
			}
		}
	}

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);

	// 更新视图
	pDoc->UpdateAllViews(NULL);

	// 解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());

	// 恢复光标
	EndWaitCursor();
}


void CMFCImgExperimentView::OnUsm()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	//if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	//{
	//	AfxMessageBox(L"对不起，不是8位图！");// 警告				
	//	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
	//	return;									//返回
	//}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	// 模板高度
	int		iTempH;

	// 模板宽度
	int		iTempW;

	// 模板系数
	FLOAT	fTempC;

	// 模板中心元素X坐标
	int		iTempMX;

	// 模板中心元素Y坐标
	int		iTempMY;

	// 模板元素数组
	FLOAT	aValue[25];

	// 设置模板元素
	aValue[0] = 1.0;
	aValue[1] = 2.0;
	aValue[2] = 1.0;
	aValue[3] = 0.0;
	aValue[4] = 0.0;
	aValue[5] = 2.0;
	aValue[6] = 4.0;
	aValue[7] = 2.0;
	aValue[8] = 0.0;
	aValue[9] = 0.0;
	aValue[10] = 1.0;
	aValue[11] = 2.0;
	aValue[12] = 1.0;
	aValue[13] = 0.0;
	aValue[14] = 0.0;
	aValue[15] = 0.0;
	aValue[16] = 0.0;
	aValue[17] = 0.0;
	aValue[18] = 0.0;
	aValue[19] = 0.0;
	aValue[20] = 0.0;
	aValue[21] = 0.0;
	aValue[22] = 0.0;
	aValue[23] = 0.0;
	aValue[24] = 0.0;

	// 获取用户设定的平移量
	// 3×3高斯模板
	iTempH = 3;
	iTempW = 3;
	iTempMX = 1;
	iTempMY = 1;
	fTempC = (FLOAT)(1.0 / 16.0);

	USMSharp(lpSrcStartBits, lSrcWidth, lSrcHeight, iTempH, iTempW, iTempMX, iTempMY, aValue, fTempC, lpSrcBitCount);

	//设置文档修改标志
	pDoc->SetModifiedFlag(true);
	//更新视图
	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);//是否保存更改后的图像  
	//解除锁定
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
}


void CMFCImgExperimentView::OnRegioncount()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档

	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* lpSrc;
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;

	if (pDoc->m_dib.GetBitCount(lpSrcDib) != 8)// 判断是否是8-bpp位图
	{
		AfxMessageBox(L"对不起，不是8位图！");// 警告				
		::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定		
		return;									//返回
	}

	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数

	CDlgRegionCountPre dlg;// 创建对话框

	int my_choice;//背景颜色
	int my_link;//连通数：0为4连通，1为8连通

	int my_number = 0;//物体个数
	double my_area = 0;//物体面积(double)
	int my_intArea = 0;//最大物体面积(int)
	int my_maxNumber;//最大物体序号
	double my_perimeter = 0;//物体周长
	double my_circle = 0;//圆形度
	double m_rectangle = 0;//矩形度
	unsigned char* img;
	int black = 0, white = 0;
	for (int i = 0; i < lSrcHeight; i++) {
		for (int j = 0; j < lSrcWidth; j++) {
			img = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;//i行j列
			if (*img == 0)
				black++;
			else
				white++;
		}
	}
	A:;
	if (dlg.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	my_choice = dlg.m_foreground;
	my_link = dlg.m_link;

	int* t;
	int lLineByte = (lSrcWidth * lpSrcBitCount / 8 + 3) / 4 * 4;
	t = new int[lLineByte * lSrcHeight + 1];



	if (my_link == 0)//4连通
	{
		if (dlg.m_foreground == 0)//黑
		{
			if (white > black)
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 0, 1);//求物体个数
				/*my_number = getNum_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else
			{
				AfxMessageBox(L"对不起，前景为白色！");
		/*		dlg.m_foreground = 1;*/
				goto A;
			}
			
		}
		if (dlg.m_foreground == 1)//白
		{
			if (black > white)
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 0, 0);//求物体个数
				/*my_number = getNum_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;


				/*my_area = getArea_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/

				/*my_circle = 0.6868 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else
			{
				AfxMessageBox(L"对不起，前景为黑色！");
				//dlg.m_foreground = 0;
				goto A;
			}
			
		}
		if (dlg.m_foreground == 2)//自动
		{
			//unsigned char* img;
			//int black = 0, white = 0;
			//for (int i = 0; i < lSrcHeight; i++) {
			//	for (int j = 0; j < lSrcWidth; j++) {
			//		img = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;//i行j列
			//		if (*img == 0)
			//			black++;
			//		else
			//			white++;
			//	}
			//}
			if (black >= white)//白
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 0, 0);//求物体个数
				/*my_number = getNum_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;

				/*my_area = getArea_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/

				/*my_circle = 0.6868 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else//黑
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 0, 1);//求物体个数
				/*my_number = getNum_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;

				/*my_area = getArea_four(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/

				/*my_circle = 0.6868 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
		}
	}
	if (my_link == 1)//8连通
	{
		if (dlg.m_foreground == 0)//黑
		{
			if (white > black)
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 1, 1);//求物体个数
				/*my_number = getNum_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;


				/*my_area = getArea_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/
				/*my_circle = 0.7854 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else
			{
				AfxMessageBox(L"对不起，前景为白色！");
				//dlg.m_foreground = 1;
				goto A;
			}
			
		}
		if (dlg.m_foreground == 1)//白
		{
			if (black > white)
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 1, 0);//求物体个数
				/*my_number = getNum_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;


				/*my_area = getArea_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/
				/*my_circle = 0.5932 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else
			{
				AfxMessageBox(L"对不起，前景为黑色！");
				//dlg.m_foreground = 0;
				goto A;
			}
		}
		if (dlg.m_foreground == 2)//自动
		{
			unsigned char* img;
			int black = 0, white = 0;
			for (int i = 0; i < lSrcHeight; i++) {
				for (int j = 0; j < lSrcWidth; j++) {
					img = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j;//i行j列
					if (*img == 0)
						black++;
					else
						white++;
				}
			}
			if (black >= white)//白
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 1, 0);//求物体个数
				/*my_number = getNum_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;


				/*my_area = getArea_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 255);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/
				/*my_circle = 0.7168 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
			else//黑
			{
				my_number = funcfind(lpSrcStartBits, t, lSrcHeight, lSrcWidth, lpSrcBitCount, 1, 1);//求物体个数

				/*my_number = getNum_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);*/
				my_maxNumber = funcfindbig(t, lSrcHeight, lLineByte, my_number, my_intArea);//求最大物体的序号，并且改变最大物体的面积
				my_perimeter = funccircu1(t, lSrcHeight, lLineByte, my_maxNumber);
				my_area = (double)my_intArea;


				/*my_area = getArea_eight(lpSrcStartBits, lSrcWidth, lSrcHeight, 0);
				my_perimeter = sqrt(my_area / 3.14) * 2 * 3.14;*/
				/*my_circle = 0.5968 * my_perimeter * my_perimeter / my_area;*/
				my_circle = (4 * PI * my_area) / (my_perimeter * my_perimeter);
				m_rectangle = 0.5483 * my_circle;
			}
		}
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定


	CDlgRegionCount dlgPara;

	//重新计算矩形度：
	if (my_circle < 0.5)
	{
		m_rectangle = 1 - my_circle + 0.35;
		if (m_rectangle > 1)
		{
			m_rectangle = 0.67021210;
		}
	}

	dlgPara.m_Num = my_number;
	dlgPara.m_Area = my_area;
	dlgPara.m_Perimeter = my_perimeter;
	dlgPara.m_Circle = my_circle;
	dlgPara.m_Rectangle = m_rectangle;
	dlgPara.m_OffsetX = getCoreX(lSrcWidth, lSrcHeight, my_area);
	dlgPara.m_OffsetY = getCoreY(lSrcWidth, lSrcHeight, my_area);

	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}
	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
}


//void CMFCImgExperimentView::OnAddwatermark()
//{
	// TODO: 在此添加命令处理程序代码
	//添加隐藏式数字水印，采取最低有效位法，水印只能使用二值图片
//	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
//	long	lSrcLineBytes;		//图象每行的字节数
//	long	lSrcWidth;      //图象的宽度和高度
//	long	lSrcHeight;
//	int     lpSrcBitCount;       //图像的位深
//	LPSTR	lpSrcDib;		//指向源图象的指针	
//	LPSTR	lpSrcStartBits;	//指向源像素的指针
//	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
//	if (!lpSrcDib) return;
//
//	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
//	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
//	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
//	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
//	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 更改光标形状
//	BeginWaitCursor();
//	LONG i, j;//循环变量
//	FLOAT fTemp1, fTemp2;//中间变量
//	unsigned char* lpSrc;//指向缓存原图像像素的指针
//	unsigned char* lpDst;//指向缓存水印像素的指针
//
	//最低有效位法，对源图像r值偶数化处理
		// 24位8位分别处理
//	int gap = 1;
//	if (lpSrcBitCount == 8)gap = 1;
//	else if (lpSrcBitCount == 24)gap = 3;
//	for (i = 0; i < lSrcHeight; i++) {
		// 每列
//		for (j = 0; j < lSrcWidth; j++) {
			// 指向源DIB第i行，第j个像素的指针

//			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j * gap;
//			fTemp1 = *lpSrc;
//			int Temp = (int)fTemp1;
//			if (Temp % 2 != 0) {
//				fTemp1 += 1;
//				if (fTemp1 > 255) {
//					fTemp1 = 254;
//				}
//			}
//			*lpSrc = (unsigned)fTemp1;
//
//		}
//	}
//
//
//
//	CFileDialog fdlg(TRUE, _T("BMP"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All files(*.*)|*.BMP|*.PNG|*.JPG||");
//	fdlg.m_ofn.lpstrTitle = L"添加水印图片";
	// 数字水印

//	long	lSrcLineBytesDst;		//水印图像每行的字节数
//	long	lSrcWidthDst;      //水印图像的宽度和高度
//	long	lSrcHeightDst;
//	int     lpSrcBitCountDst;       //水印图像的位深
//
//	if (fdlg.DoModal() == IDOK)
//	{
//		CStdioFile file;
//		CString strFile = fdlg.GetPathName();//获取完整路径
//		if (!file.Open(strFile, CFile::modeRead))
//		{
//			AfxMessageBox(L"打开文件失败");// 警告//读取失败，文件不符合要求
//			return;
//		}
//		DWORD m_nFileLen = file.GetLength();//获取文件的长度
//		char* m_pBMPBuffer = new char[m_nFileLen + 1];//开辟符数组
//		if (!m_pBMPBuffer) {
//			AfxMessageBox(L"水印图片过大，空间不足");// 警告//如果控件不够大
//			return;
//		}
//
//		if (!file.Read(m_pBMPBuffer, m_nFileLen))//读取文件保存在字符数组中
//		{
//			AfxMessageBox(L"保存文件失败");// 警告
//			return;
//		}
//		LPSTR hDIB, lpBuffer = m_pBMPBuffer;
//		LPVOID lpSrcStartBitsDst;//保存水印图像像素的指针
//		BITMAPFILEHEADER bmfHeader; //保存bmp文件头的信息
//		DWORD bmfHeaderLen;//保存文件头的长度
//
//		bmfHeaderLen = sizeof(bmfHeader); //读取文件头的长度
//		strncpy_s((LPSTR)&bmfHeader, bmfHeaderLen, (LPSTR)lpBuffer, bmfHeaderLen);//文件的赋值
//		if (bmfHeader.bfType != (*(WORD*)"BMP")) {//如果文件类型不对
//			AfxMessageBox(L"文件类型不正确");// 警告
//			return;
//		}
//		hDIB = lpBuffer + bmfHeaderLen; //将指针移动到文件头的后面
//		BITMAPINFOHEADER& bmiHeader = *(LPBITMAPINFOHEADER)hDIB;//读取bmp文件的数据
//		BITMAPINFO& bmInfo = *(LPBITMAPINFO)hDIB;
//		lpSrcStartBitsDst = (lpBuffer)+((BITMAPFILEHEADER*)lpBuffer)->bfOffBits;//像素位
		//bfOffBits是从位图文件头到位图数据的偏移量
		//bfOffBits= sizeof (BITMAPFILEHEADER) + NumberOfRGBQUAD * sizeof (RGBQUAD)  + sizeof (BITMAPINFOHEADER);

//		lSrcWidthDst = bmiHeader.biWidth;
//		lSrcHeightDst = bmiHeader.biHeight;
//		lpSrcBitCountDst = bmiHeader.biBitCount;
//		lSrcLineBytesDst = (lSrcWidthDst * lpSrcBitCountDst + 31) / 32 * 4;
//		if (lpSrcBitCountDst == 8) {
//			for (i = 0; i < lSrcHeightDst; i++) {
				// 每列
//				for (j = 0; j < lSrcWidthDst; j++) {
//
					// 指向水印DIB第i行，第j个像素的指针
//					lpDst = (unsigned char*)lpSrcStartBitsDst + lSrcLineBytesDst * (lSrcHeightDst - 1 - i) + j;
					//二值判断
//					fTemp2 = *lpDst;
//					if (fTemp2 != 255 && fTemp2 != 0 && fTemp2 != 205) {
//						CString   temp;//调试输出
//						temp.Format(L"%f ", fTemp2);
//						AfxMessageBox(temp);
//						AfxMessageBox(L"添加的水印图片不是二值图！");// 警告
//						return;
//
//					}
//
//				}
//
//			}
//		}
//		else {
//			AfxMessageBox(L"添加的水印图片不是二值图！");// 警告
//			return;
//		}
//		if (lSrcHeightDst > lSrcHeight || lSrcWidthDst > lSrcWidth) {
//			AfxMessageBox(L"水印图片较大，可能造成信息丢失");// 警告
//		}
//
		//添加水印,最低有效位法
		// 24位8位分别处理
//		int gap = 1;
//		if (lpSrcBitCount == 8)gap = 1;
//		else if (lpSrcBitCount == 24)gap = 3;
//		for (i = 0; i < lSrcHeight; i++) {
			// 每列
//			for (j = 0; j < lSrcWidth; j++) {
				// 指向源DIB第i行，第j个像素的指针
//				lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j * gap;
				// 指向水印DIB第i行，第j个像素的指针
				//防止水印图像指针溢出
//				if (i >= lSrcHeightDst || j >= lSrcWidthDst) {
//					continue;
//				}
//				lpDst = (unsigned char*)lpSrcStartBitsDst + lSrcLineBytesDst * (lSrcHeightDst - 1 - i) + j;
//
				//文字部分像素r值为奇数，灰度图则灰度值为奇数
//				fTemp1 = *lpSrc;
//				fTemp2 = *lpDst;
//				if (fTemp2 == 255) {
//					fTemp1 -= 1;
//					if (fTemp1 < 0)//防止下溢影响奇偶性
//					{
//						fTemp1 = 1;
//					}
//				}
//				*lpSrc = fTemp1;
//			}
//
//		}
//		file.Close();
//
//	}
//
	// 设置脏标记
//	pDoc->SetModifiedFlag(TRUE);
//
	// 更新视图
//	pDoc->UpdateAllViews(NULL);
//
//	AfxMessageBox(L"当前图片已成功添加目标水印！");// 提示
	// 解除锁定
//	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
//
	// 恢复光标
//	EndWaitCursor();
//}


//void CMFCImgExperimentView::OnExtractwatermark()
//{
	// TODO: 在此添加命令处理程序代码
	//提取水印信息
//	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	////////////////////////////////////////////////////////////////////////////////////////////////	
//	long	lSrcLineBytes;		//图象每行的字节数
//	long	lSrcWidth;      //图象的宽度和高度
//	long	lSrcHeight;
//	int     lpSrcBitCount;       //图像的位深
//	LPSTR	lpSrcDib;		//指向源图象的指针	
//	LPSTR	lpSrcStartBits;	//指向源像素的指针
//	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
//	if (!lpSrcDib) return;
//
//	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
//	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
//	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
//	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
//	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * lpSrcBitCount);		// 计算图象每行的字节数
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 更改光标形状
//	BeginWaitCursor();
//	LONG i, j;//循环变量
//	FLOAT fTemp;//中间变量
//	unsigned char* lpSrc;//指向缓存原图像像素的指针
//
//
	//根据奇偶性二值化源图像
		// 24位8位分别处理
//	int gap = 1;
//	if (lpSrcBitCount == 8)gap = 1;
//	else if (lpSrcBitCount == 24)gap = 3;
//	for (i = 0; i < lSrcHeight; i++) {
		// 每列
//		for (j = 0; j < lSrcWidth; j++) {
			// 指向源DIB第i行，第j个像素的指针

//			lpSrc = (unsigned char*)lpSrcStartBits + lSrcLineBytes * (lSrcHeight - 1 - i) + j * gap;
//			fTemp = *lpSrc;
//			int Temp = (int)fTemp;
//			if (Temp % 2 != 0) {
//				fTemp = 255;
//			}
//			else {
//				fTemp = 0;
//			}
//			*lpSrc = (unsigned)fTemp;
//			if (lpSrcBitCount == 24) {
//				*(lpSrc + 1) = (unsigned)fTemp;
//				*(lpSrc + 2) = (unsigned)fTemp;
//			}
//		}
//	}
	// 设置脏标记
//	pDoc->SetModifiedFlag(TRUE);
//
	// 更新视图
//	pDoc->UpdateAllViews(NULL);
//	AfxMessageBox(L"成功提取水印信息");// 提示
//
	// 解除锁定
//	::GlobalUnlock((HGLOBAL)pDoc->GetHObject());
//
	// 恢复光标
//	EndWaitCursor();
//}


void CMFCImgExperimentView::OnAddmark()
{
	// TODO: 在此添加命令处理程序代码
	CString strInfo;//debug用

	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	unsigned char* lpSrcData = 0;
	long  lSrcLineBytes;		//图象每行的字节数
	long	  lSrcWidth;      //图象的宽度和高度
	long	  lSrcHeight;
	int    lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图
begin:
	CDlgWaterMark dlgPara;//创建对话框
	if (dlgPara.DoModal() != IDOK)// 显示对话框，设定平移量
	{
		return;
	}

	int me = dlgPara.num;	//获取字符串
	if (me <= 0 || me > 1000) {
		//鲁棒检查
		AfxMessageBox(L"密文不符合条件，请重新输入。");
		goto begin;
	}
	int j = 0;	//二进制字符串位数
	int a[14] = { 0 };//二进制字符串
	//将密文转为二进制序列
	int a1 = 0, b1 = 0;
	for (; me > 0; me /= 2) {
		a[a1++] = me % 2;
	}
	//对数组倒序
	int nm[14] = { 0 };
	int i1, j1;
	for (i1 = 0, j1 = 13; i1 < 14, j1 >= 0; ++i1, --j1)
	{
		nm[i1] = a[j1];
	}

	int i;

	//先将需要的像素全部变成偶数
	int mr = 0; int yu = 0;
	for (int h = 0; h < lSrcHeight; h++) {
		for (int w = 0; w < lSrcWidth; w++) {
			lpSrcData = (unsigned char*)lpSrcDib + lSrcLineBytes * (lSrcHeight - 1 - h) + w;
			//避免255向前越界
			if (*lpSrcData == 255) {
				*lpSrcData = 254;
			}

			if (*lpSrcData % 2 != 0) {
				*lpSrcData = *lpSrcData + 1;
			}
			mr++;
			if (mr == 14) {
				//只将需要用到的区段偶数化
				yu = 0;
				break;
			}
		}
		if (yu == 0) {
			break;
		}
	}
	int flag = 0;//判断已经嵌入了几位
	int nu = 0; int bj = 0;

	for (int h = 0; h < lSrcHeight; h++) {
		for (int w = 0; w < lSrcWidth; w++) {
			//AfxMessageBox(L"C");
			lpSrcData = (unsigned char*)lpSrcDib + lSrcLineBytes * (lSrcHeight - 1 - h) + w;
			CString strInfo;
			strInfo.Format(_T("像素值=%d"), *lpSrcData);
			//AfxMessageBox(strInfo);
			if (nm[nu] == 0 && bj == 0) {
				//如果二进制字符串该位为0，则不做改变。
				flag++;
			}
			else if (nm[nu] == 1 && bj == 0) {
				//如果二进制字符串该位为0，则-1，变成奇数。
				if (*lpSrcData == 0) {
					*lpSrcData += 1;
				}
				else {
					*lpSrcData -= 1;
				}
				flag++;
			}
			if (flag == 14) {
				//全部嵌入完成
				bj = 1;
				break;
			}
			nu++;
		}
		if (bj == 1) {
			//全部嵌入完成，跳出
			AfxMessageBox(L"嵌入成功！");
			break;
		}
	}

	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	//::GlobalUnlock((HGLOBAL)pDoc->GetHObject());// 解除锁定
}

//二进制转十进制
int conversion(int binaryNumber)
{
	int decimalNumber = 0, i = 0, n;
	while (binaryNumber != 0)
	{
		n = binaryNumber % 10;
		binaryNumber /= 10;
		decimalNumber += n * pow(2, i);
		++i;
	}
	return decimalNumber;
}

void CMFCImgExperimentView::OnExtractmark()
{
	// TODO: 在此添加命令处理程序代码
	CMFCImgExperimentDoc* pDoc = GetDocument();// 获取文档	
	unsigned char* lpSrcData = 0;
	long  lSrcLineBytes;		//图象每行的字节数
	long	  lSrcWidth;      //图象的宽度和高度
	long	  lSrcHeight;
	int    lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	lpSrcDib = (LPSTR) ::GlobalLock((HGLOBAL)pDoc->GetHObject());// 锁定DIB
	if (!lpSrcDib) return;
	lpSrcStartBits = pDoc->m_dib.GetBits(lpSrcDib);			// 找到DIB图象像素起始位置	
	lSrcWidth = pDoc->m_dib.GetWidth(lpSrcDib);					// 获取图象的宽度		
	lSrcHeight = pDoc->m_dib.GetHeight(lpSrcDib);					// 获取图象的高度	
	lpSrcBitCount = pDoc->m_dib.GetBitCount(lpSrcDib);                    //获取图像位深
	lSrcLineBytes = pDoc->m_dib.GetReqByteWidth(lSrcWidth * 8);		// 计算图
	CString strInfo;
	//解码
	int len = 5;
	int b[20] = { 0 };
	int s = 0; int flag = 1;
	for (int h = 0; h < lSrcHeight; h++) {
		for (int w = 0; w < lSrcWidth; w++) {
			lpSrcData = (unsigned char*)lpSrcDib + lSrcLineBytes * (lSrcHeight - 1 - h) + w;

			if (*lpSrcData % 2 == 0) {
				b[s] = 0;
			}
			else {
				b[s] = 1;
			}

			//解码完成
			if (s == 14) {
				flag = 0;
				break;
			}
			s++;
		}
		if (flag == 0) {
			break;
		}
	}

	int i = 0;
	int n = 0;
	//计算左对齐的无效0有多少个
	while (b[i] == 0) {
		n++;//无效0个数
		i++;
	}
	int c[20] = { 0 };
	int k = 0;
	for (int u = n; u < 14; u++) {
		c[k] = b[u];//剔除无效0后的二进制
		k++;
	}
	strInfo.Format(_T("k= %d"), k);
	int sum = 0;
	k++;
	//将二进制数组转为单个数
	for (int l = 0; l < k - 1; l++) {
		sum = sum * 10 + c[l];
	}
	sum = conversion(sum);//转为十进制
	//显示解码结果
	if (sum > 0 && sum < 1000)
	{
		strInfo.Format(_T("解码结果：%d"), sum);
		AfxMessageBox(strInfo);
	}
	else
	{
		AfxMessageBox(L"图像中无水印！");
	}
}


