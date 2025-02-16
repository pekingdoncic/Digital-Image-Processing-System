// CDlgBitPlaneSlice.cpp: 实现文件
//

#include "pch.h"
#include "MFCImgExperiment.h"
#include "afxdialogex.h"
#include "CDlgBitPlaneSlice.h"
#define DIB_MARKER   ((WORD) ('M' << 8) | 'B')

// CDlgBitPlaneSlice 对话框

IMPLEMENT_DYNAMIC(CDlgBitPlaneSlice, CDialogEx)

CDlgBitPlaneSlice::CDlgBitPlaneSlice(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BitplaneDecomposition, pParent)
	, m_BitPlane(0)
{

}

CDlgBitPlaneSlice::~CDlgBitPlaneSlice()
{
}

void CDlgBitPlaneSlice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Bit, m_BitPlane);
}


BEGIN_MESSAGE_MAP(CDlgBitPlaneSlice, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTSaveBitPlane, &CDlgBitPlaneSlice::OnBnClickedButsavebitplane)
	ON_BN_CLICKED(IDC_BUT_Slicing, &CDlgBitPlaneSlice::OnBnClickedButSlicing)
END_MESSAGE_MAP()


// CDlgBitPlaneSlice 消息处理程序


BOOL CDlgBitPlaneSlice::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	flag = 0;
	int palettesize = 0;
	if (lpSrcBitCount == 8) palettesize = 256 * sizeof(RGBQUAD);
	lpImgPlane = new BYTE[lSrcLineBytes * lSrcHeight + sizeof(BITMAPINFOHEADER) + palettesize];
	memcpy(lpImgPlane, lpSrcDib, sizeof(BITMAPINFOHEADER) + palettesize + lSrcHeight * lSrcLineBytes);
	pBmpBuf = lpImgPlane + sizeof(BITMAPINFOHEADER) + palettesize;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgBitPlaneSlice::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	unsigned char* lpSrc;
	int r, b, g;
	for (int i = 0; i <= lSrcHeight - 1; i++)
	{
		for (int j = 0; j <= lSrcWidth - 1; j++)
		{
			lpSrc = (unsigned char*)pBmpBuf + lSrcLineBytes * (lSrcHeight - 1 - i) + j;
			r = *lpSrc;
			g = *lpSrc;
			b = *lpSrc;
			dc.SetPixel((j + 50), (i + 50), RGB(b, g, r));
		}
	}
}

void CDlgBitPlaneSlice::OnBnClickedButsavebitplane()
{	//保存处理后的位平面
	if (flag == 0) return;

	BOOL isOpen = FALSE;//是否打开(否则为保存)
	CString defaultDir = L" ";//默认打开的文件路径
	CString fileName = L"test.bmp";//默认打开的文件名
	CString filter = L"文件 (*.bmp;  )|*.bmp;*.ppt;*.xls||";//文件过虑的类型
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L" ";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\" + fileName;
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}

	//如果位图数据指针为，则没有数据传入，函数返回
	errno_t err;
	if (!pBmpBuf)return;
	//颜色表大小，以字节为单位，灰度图像颜色表为字节，彩色图像颜色表大小为
	int colorTablesize = 0;
	if (lpSrcBitCount == 8)colorTablesize = 1024;
	//待存储图像数据每行字节数为的倍数
	//lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
	//lineByte = lpSrcStartBits;
	//以二进制写的方式打开文件

	CFile file;
	if (!file.Open(filePath, CFile::modeCreate |// 打开文件
		CFile::modeReadWrite | CFile::shareExclusive))
	{
		return;// 返回FALSE
	}


	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = DIB_MARKER;	// 填充文件头
	//bmp类型//bfSize是图像文件个组成部分之和
	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes;
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;
	//bfOffBits是图像文件前个部分所需空间之和
	fileHead.bfOffBits = 54 + colorTablesize;
	//写文件头进文件
	file.Write(&fileHead, sizeof(BITMAPFILEHEADER));// 写文件头		
	/*
	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head;
	head.biBitCount = lpSrcBitCount;
	head.biClrImportant = 0;
	if (lpSrcBitCount == 8)
		head.biClrUsed = 256;
	else
		head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = lSrcHeight;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = lSrcLineBytes*lSrcHeight;
	head.biWidth = lSrcWidth;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;
	//写位图信息头进内存
	file.Write(&head, sizeof(BITMAPFILEHEADER));// 写信息头

												//如果灰度图像，有颜色表，写入文件
	if (lpSrcBitCount == 8)
		file.Write(lpImgPlane, sizeof(RGBQUAD) * 256);


	file.Write(pBmpBuf, lSrcHeight*lSrcLineBytes);// 写像素
		*/

	file.Write(lpImgPlane, sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes);
	//关闭文件
	file.Close();
}

BOOL BitPlaneSlicing1(LPSTR lpDIBBits, LONG lmageWidth, LONG  lLineBytes, LONG lmageHeight, int Bit)
{
	int i;  //循环变量
	int j;	//循环变量
	// 指向源图像的指针
	unsigned char* lpSrc;
	//像素值
	unsigned char pixeltemp;
	int threshold1, threshold2;
	switch (Bit)
	{
	case 7:
		threshold1 = 128;
		threshold2 = 256;
		break;
	case 6:
		threshold1 = 64;
		threshold2 = 128;
		break;
	case 5:
		threshold1 = 32;
		threshold2 = 64;
		break;
	case 4:
		threshold1 = 16;
		threshold2 = 32;
		break;
	case 3:
		threshold1 = 8;
		threshold2 = 16;
		break;
	case 2:
		threshold1 = 4;
		threshold2 = 8;
		break;
	case 1:
		threshold1 = 2;
		threshold2 = 4;
		break;
	case 0:
		threshold1 = 0;
		threshold2 = 2;
		break;
	default:
		return false;

	}
	for (j = 0; j < lmageHeight; j++)
	{
		for (i = 0; i < lmageWidth; i++)
		{

			// 指向源图像倒数第j行，第i个象素的指针			
			//lpSrc = (unsigned char *)lpDIBBits + lmageWidth * j + i;
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			//取得当前指针处的像素值
			pixeltemp = (unsigned char)*lpSrc;
			//目标图像中灰度值小于门限则设置为黑点
			if (pixeltemp >= threshold1 && pixeltemp < threshold2)
			{
				//*lpSrc = (unsigned char)255;
			}
			else	//否则设置为白点
			{
				*lpSrc = (unsigned char)0;
			}
		}
	}
	return true;
}
//void CDlgBitPlaneSlice::OnBnClickedButsavebitplane()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if (flag == 0) return;
//
//	BOOL isOpen = FALSE;//是否打开(否则为保存)
//	CString defaultDir = L" ";//默认打开的文件路径
//	CString fileName = L"test.bmp";//默认打开的文件名
//	CString filter = L"文件 (*.bmp;  )|*.bmp;*.ppt;*.xls||";//文件过虑的类型
//	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
//	openFileDlg.GetOFN().lpstrInitialDir = L" ";
//	INT_PTR result = openFileDlg.DoModal();
//	CString filePath = defaultDir + "\\" + fileName;
//	if (result == IDOK) {
//		filePath = openFileDlg.GetPathName();
//	}
//
//	//如果位图数据指针为，则没有数据传入，函数返回
//	errno_t err;
//	if (!pBmpBuf)return;
//	//颜色表大小，以字节为单位，灰度图像颜色表为字节，彩色图像颜色表大小为
//	int colorTablesize = 0;
//	if (lpSrcBitCount == 8)colorTablesize = 1024;
//	//待存储图像数据每行字节数为的倍数
//	//lineByte = (width * biBitCount / 8 + 3) / 4 * 4;
//	//lineByte = lpSrcStartBits;
//	//以二进制写的方式打开文件
//
//	CFile file;
//	if (!file.Open(filePath, CFile::modeCreate |// 打开文件
//		CFile::modeReadWrite | CFile::shareExclusive))
//	{
//		return;// 返回FALSE
//	}
//
//
//	//申请位图文件头结构变量，填写文件头信息
//	BITMAPFILEHEADER fileHead;
//	fileHead.bfType = DIB_MARKER;	// 填充文件头
//	//bmp类型//bfSize是图像文件个组成部分之和
//	fileHead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes;
//	fileHead.bfReserved1 = 0;
//	fileHead.bfReserved2 = 0;
//	//bfOffBits是图像文件前个部分所需空间之和
//	fileHead.bfOffBits = 54 + colorTablesize;
//	//写文件头进文件
//	file.Write(&fileHead, sizeof(BITMAPFILEHEADER));// 写文件头		
//	/*
//	//申请位图信息头结构变量，填写信息头信息
//	BITMAPINFOHEADER head;
//	head.biBitCount = lpSrcBitCount;
//	head.biClrImportant = 0;
//	if (lpSrcBitCount == 8)
//		head.biClrUsed = 256;
//	else
//		head.biClrUsed = 0;
//	head.biCompression = 0;
//	head.biHeight = lSrcHeight;
//	head.biPlanes = 1;
//	head.biSize = 40;
//	head.biSizeImage = lSrcLineBytes*lSrcHeight;
//	head.biWidth = lSrcWidth;
//	head.biXPelsPerMeter = 0;
//	head.biYPelsPerMeter = 0;
//	//写位图信息头进内存
//	file.Write(&head, sizeof(BITMAPFILEHEADER));// 写信息头
//
//												//如果灰度图像，有颜色表，写入文件
//	if (lpSrcBitCount == 8)
//		file.Write(lpImgPlane, sizeof(RGBQUAD) * 256);
//
//
//	file.Write(pBmpBuf, lSrcHeight*lSrcLineBytes);// 写像素
//		*/
//
//	file.Write(lpImgPlane, sizeof(BITMAPINFOHEADER) + colorTablesize + lSrcHeight * lSrcLineBytes);
//	//关闭文件
//	file.Close();
//}


void CDlgBitPlaneSlice::OnBnClickedButSlicing()
{
	// TODO: 在此添加控件通知处理程序代码
	//做位平面分解
	int Bit;
	UpdateData(TRUE);
	Bit = m_BitPlane;
	if (BitPlaneSlicing1((LPSTR)pBmpBuf, lSrcWidth, lSrcLineBytes, lSrcHeight, Bit)) flag = 1;
	//显示图像
	Invalidate();

}
