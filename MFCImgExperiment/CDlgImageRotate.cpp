// CDlgImageRotate.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgImageRotate.h"
#include "MainFrm.h"
#include "MFCImgExperimentDoc.h"
#include "PointTrans.h"


// CDlgImageRotate 对话框

IMPLEMENT_DYNAMIC(CDlgImageRotate, CDialogEx)

CDlgImageRotate::CDlgImageRotate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ImageRotation, pParent)
	, m_RotateAngle(0)
{

}

CDlgImageRotate::~CDlgImageRotate()
{
}

void CDlgImageRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Rotate, m_RotateAngle);
}


BEGIN_MESSAGE_MAP(CDlgImageRotate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONPRE, &CDlgImageRotate::OnBnClickedButtonpre)
	ON_BN_CLICKED(IDC_BUTTONPRE2, &CDlgImageRotate::OnBnClickedButtonpre2)
END_MESSAGE_MAP()


// CDlgImageRotate 消息处理程序


void CDlgImageRotate::OnBnClickedButtonpre()
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
	//ImageTranslation(lpSrcStartBits, lSrcWidth, lSrcHeight, m_XOffset, m_YOffset);
		// 创建新DIB
	HGLOBAL hNewDIB = NULL;
	//RotateDIB1(lpSrcDib, dlgPara.m_jiaodu);

	// 调用RotateDIB()函数旋转DIB
	//hNewDIB = (HGLOBAL)RotateDIB(pDoc,lpSrcDib, iRotateAngle);
	hNewDIB = (HGLOBAL)RotateDIB(lpSrcDib, m_RotateAngle);

	// 判断旋转是否成功
	if (hNewDIB != NULL)
	{

		// 替换DIB，同时释放旧DIB对象

		pDoc->UpdateObject(hNewDIB);

		// 更新DIB大小和调色板
		//pDoc->InitDIBData();
		pDoc->SetDib();

		//// 设置脏标记
		//pDoc->SetModifiedFlag(TRUE);

		// 重新设置滚动视图大小
		//SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

		//// 更新视图
		//pDoc->UpdateAllViews(NULL);
	}
	UpdateData(TRUE);
	chuzhi->UpdateAllViews(NULL);
}


void CDlgImageRotate::OnBnClickedButtonpre2()
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
