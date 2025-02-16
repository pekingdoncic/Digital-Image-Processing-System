#include "pch.h"
#include "stdafx.h"
#include "PointTrans.h"
#include "framework.h"
#include <math.h>
#include <direct.h>
#include <queue>
#include <random>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Dib.h"
#include "MFCImgExperimentDoc.h"
#include <afxwin.h>
#include "DIBAPI.h"
using namespace std;



#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define PI 3.14159
#define RADIAN(angle) ((angle)*PI/180.0) 

int temp[1000][1000];
int xy = 0;
//函数实现：
int PFC(int color1, int color2)
{
	int x, y, z;
	x = (color1 & 0xf) - (color2 & 0xf);
	y = ((color1 >> 4) & 0xf) - ((color2 >> 4) & 0xf);
	z = ((color1 >> 8) & 0xf) - ((color2 >> 8) & 0xf);
	return (x * x + y * y + z * z);
};
int Sort3(int* src, int* attach, int n)
{
	int i, j;
	int temp;
	for (i = 0; i < n - 1; i++)
		for (j = i + 1; j < n; j++)
		{
			if (src[i] < src[j])
			{
				temp = src[j];
				src[j] = src[i];
				src[i] = temp;

				temp = attach[j];
				attach[j] = attach[i];
				attach[i] = temp;

			}
		}


	return 0;
}
int Transfer(WORD* color24bit, LONG len, BYTE* Index, RGBQUAD* mainColor)
{
	int usedTimes[4096] = { 0 };
	int miniColor[4096];
	LONG i;
	for (i = 0; i < 4096; i++)
		miniColor[i] = i;
	i = 0;
	for (i = 0; i < len; i++)
	{
		assert(color24bit[i] < 4096);  //图像的列宽要为4的倍数！！！
		usedTimes[color24bit[i]]++;
	}
	LONG numberOfColors = 0;
	for (i = 0; i < 4096; i++)
	{
		if (usedTimes[i] > 0)
			numberOfColors++;
	}
	//对usedTimes进行排序，排序过程中minColor数组(保存了颜色值)也作与useTimes  
	//数组相似的交换  
	//Sort2(usedTimes, miniColor, 4096);   //ok 
	Sort3(usedTimes, miniColor, 4096);    //ok
	//usedTimes数组中是各颜色使用频率，从高到低排列，显然第numberOfColor个之后的都为0  
	//miniColor数组中是相应的颜色数据  
	//将前256个颜色数据保存到256色位图的调色盘中  
	for (i = 0; i < 256; i++)
	{
		mainColor[i].rgbBlue = (BYTE)((miniColor[i] >> 8) << 4);
		mainColor[i].rgbGreen = (BYTE)(((miniColor[i] >> 4) & 0xf) << 4);
		mainColor[i].rgbRed = (BYTE)((miniColor[i] & 0xf) << 4);
		mainColor[i].rgbReserved = 0;
	}

	int* colorIndex = usedTimes;//用原来的useTimes数组来保存索引值  
	memset(colorIndex, 0, sizeof(int) * 4096);

	if (numberOfColors <= 256)
	{
		for (i = 0; i < numberOfColors; i++)
			colorIndex[miniColor[i]] = i;
	}
	else//为第256之后的颜色在前256种颜色中找一个最接近的  
	{
		for (i = 0; i < 256; i++)
			colorIndex[miniColor[i]] = i;

		int index, tmp, tmp1;
		for (i = 256; i < numberOfColors; i++)
		{
			tmp = PFC(miniColor[0], miniColor[i]);
			index = 0;
			for (int j = 1; j < 256; j++)
			{
				tmp1 = PFC(miniColor[j], miniColor[i]);
				if (tmp > tmp1)
				{
					tmp = tmp1;
					index = j;
				}
			}
			colorIndex[miniColor[i]] = index;
		}
	}
	//记录各点颜色数据的索引值，即256色位图的颜色数据  
	for (i = 0; i < len; i++)
	{
		assert(colorIndex[color24bit[i]] < 256);
		Index[i] = colorIndex[color24bit[i]];
	}
	return 1;
}
BOOL WINAPI Tran24ColTo8Col(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	// 循环变量
	LONG	i;
	LONG	j;
	// 图像每行的字节数
	LONG	lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 24);
	BITMAPINFOHEADER* pHead;
	RGBQUAD* pColorTable = NULL;
	int lineByteNew;
	int biBitCount = 8;
	lineByteNew = (lWidth * biBitCount / 8 + 3) / 4 * 4;
	//灰度图像有颜色表，且颜色表表项为
	if (biBitCount == 8) {
		//申请颜色表所需要的空间，给颜色表赋值
		pColorTable = new RGBQUAD[256];
		memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
	}
	BYTE* Index = new BYTE[lineByteNew * lHeight]; //图像数据区的数据(保存在Index中)
	WORD* shortColor = new WORD[lineByteNew * lHeight]; //颜色的高4位
	memset(shortColor, 0, sizeof(WORD) * lineByteNew * lHeight); //清零

	int iRed, iGreen, iBlue;
	for (int i = 0; i < lHeight; i++)
	{//取RGB颜色的高4位  
		for (int j = 0; j < lWidth; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j * 3;
			iBlue = (*lpSrc) >> 4;
			iGreen = (*(++lpSrc)) >> 4;
			iRed = (*(++lpSrc)) >> 4;
			shortColor[lineByteNew * (lHeight - 1 - i) + j] = (iBlue << 8) + (iGreen << 4) + iRed;
		}
	}
	//调用转换函数  24color To->8color
	Transfer(shortColor, lineByteNew * lHeight, Index, pColorTable);

	//申请位图数据所需要的空间，读位图数据进内存
	unsigned char* pBmpBufNew;
	pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//申请新图像的空间
	memcpy(pBmpBufNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER));//信息头拷贝
	pHead = (BITMAPINFOHEADER*)pBmpBufNew;
	pHead->biBitCount = 8;//改变位数,
	pHead->biHeight = lHeight;
	pHead->biWidth = lWidth;
	pHead->biClrUsed = 256;
	pHead->biClrImportant = 0;
	pHead->biCompression = 0;
	pHead->biPlanes = 1;
	pHead->biSize = 40;
	pHead->biSizeImage = lineByteNew * lHeight;
	pHead->biXPelsPerMeter = 0;
	pHead->biYPelsPerMeter = 0;

	//拷贝
	memcpy(lpDIB, pBmpBufNew, sizeof(BITMAPINFOHEADER));
	memcpy(lpDIB + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
	memcpy(lpDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew * lHeight);

	delete[]Index;
	delete[]shortColor;

	return true;
}
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB) {
	//unsigned char* lpSrc;//指向DIB源像素的指针
	//FLOAT fTemp;//中间变量
	//for (int i = 0; i < lHeight; i++) {
	//	for (int j = 0; j < lWidth; j++) {
	//		lpSrc = (unsigned char*)lpDIBBits + lSrcLineBytes * (lHeight - 1 - i) + j;
	//		fTemp = (float)*lpSrc;
	//		fTemp = fA * fTemp + fB;
	//		//线性变换
	//		//判断是否超出范围
	//		if (fTemp > 255)
	//		//直接赋值为255 
	//			*lpSrc = 255;
	//		else if (fTemp < 0)
	//			// 四舍五入
	//			*lpSrc = 0;
	//		else {
	//			*lpSrc = (unsigned char)(fTemp + 0.5);
	//		}
	//	}
	//}
	//return TRUE;

	// 指向源图像的指针
	unsigned char* lpSrc;
	// 循环变量
	LONG	i;
	LONG	j;
	// 中间变量
	FLOAT	fTemp;
	// 图像每行的字节数
	LONG	lLineBytes;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	// 每行
	for (i = 0; i < lHeight; i++)
	{
		// 每列
		for (j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// 线性变换
			fTemp = fA * (*lpSrc) + fB;

			// 判断是否超出范围
			if (fTemp > 255)
			{
				// 直接赋值为255
				*lpSrc = 255;
			}
			else if (fTemp < 0)
			{
				// 直接赋值为0
				*lpSrc = 0;
			}
			else
			{
				// 四舍五入
				*lpSrc = (unsigned char)(fTemp + 0.5);
			}
		}
	}
	// 返回
	return TRUE;

}
BOOL WINAPI Segmental2_Linear_Tran(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int type, int threshold)
{
	// 指向源图像的指针
	unsigned char* lpSrc;

	// 循环变量
	LONG	i;
	LONG	j;

	// 图像每行的字节数
	LONG	lLineBytes;

	// 中间变量
	FLOAT	fTemp = 0, fA1 = 1, fB1 = 0, fA2 = 1, fB2 = 0;
	//中间灰度
	int seg_gray = 127;
	seg_gray = threshold;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//计算线性变换的参数
	if (type == 1)//倒V字型
	{
		if (threshold == 255) {
			return TRUE;
		}
		else if (threshold == 0) {
			float Fa = -1.0;
			float Fb = 255;
			LinerTrans(lpDIBBits, lWidth, lHeight, Fa, Fb);
		}
		else
		{
			fA1 = (float)(255 / threshold);//  2.0 if threshold==128;
			fB1 = (float)0;  //(y=2x,(2f))
			//y=255-(255/(255-threshold))*(x-threshold),
			fA2 = -(float)(255 / (255 - threshold));//-2.0 if threshold==128;
			fB2 = (float)(255 + fA2 * (-threshold));//(-2f+2*255)  if threshold==128;
		}

	}
	if (type == 2)//V字型
	{
		if (threshold == 255)
		{
			float Fa = -1.0;
			float Fb = 255;
			LinerTrans(lpDIBBits, lWidth, lHeight, Fa, Fb);
		}
		else if (threshold == 0) {
			return TRUE;
		}
		else {
			fA1 = -(float)(255 / threshold);
			fB1 = (float)255;  //(-2f+255)
			fA2 = (float)(255 / (255 - threshold));
			fB2 = -(float)(fA2 * (threshold));//(2f-255)
		}
	}
	if (type == 3)//倒L字型
	{
		if (threshold == 0) {
			// 每行
			for (i = 0; i < lHeight; i++)
			{
				// 每列
				for (j = 0; j < lWidth; j++)
				{
					// 指向DIB第i行，第j个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
					*lpSrc = 255;
				}
			}
			return TRUE;
		}
		else {
			fA1 = (float)(255 / threshold);
			fB1 = (float)0;  //(2f)
			fA2 = 0;
			fB2 = 255;//(255)
		}
	}
	if (type == 4)//L字型
	{
		if (threshold == 0)
		{
			// 每行
			for (i = 0; i < lHeight; i++)
			{
				// 每列
				for (j = 0; j < lWidth; j++)
				{
					// 指向DIB第i行，第j个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
					*lpSrc = 0;
				}
			}
		}
		else {
			fA1 = (float)(-255 / threshold);
			fB1 = 255;  //(-2f+255)
			fA2 = 0.0;
			fB2 = 0;//(0)
		}

	}
	// 每行
	for (i = 0; i < lHeight; i++)
	{
		// 每列
		for (j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 线性变换,分段
			if (*lpSrc >= 0 && *lpSrc <= seg_gray)
				fTemp = fA1 * (*lpSrc) + fB1;
			if (*lpSrc >= seg_gray + 1 && *lpSrc <= 255)
				fTemp = fA2 * (*lpSrc) + fB2;

			// 判断是否超出范围
			if (fTemp > 255)
			{
				// 直接赋值为255
				*lpSrc = 255;
			}
			else if (fTemp < 0)
			{
				// 直接赋值为0
				*lpSrc = 0;
			}
			else
			{
				// 四舍五入
				*lpSrc = (unsigned char)(fTemp + 0.5);
			}
		}
	}

	// 返回
	return TRUE;


}
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 循环变量
	LONG	i;
	LONG	j;

	// 灰度映射表
	BYTE	bMap[256];

	// 图像每行的字节数
	LONG	lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 计算灰度映射表
	for (i = 0; i <= bX1; i++)
	{
		// 判断bX1是否大于0（防止分母为0）
		if (bX1 > 0)
		{
			// 线性变换
			bMap[i] = (BYTE)bY1 * i / bX1;
		}
		else
		{
			// 直接赋值为0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// 判断bX1是否等于bX2（防止分母为0）
		if (bX2 != bX1)
		{
			// 线性变换
			bMap[i] = bY1 + (BYTE)((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// 直接赋值为bY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// 判断bX2是否等于255（防止分母为0）
		if (bX2 != 255)
		{
			// 线性变换
			bMap[i] = bY2 + (BYTE)((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// 直接赋值为255
			bMap[i] = 255;
		}
	}

	// 每行
	for (i = 0; i < lHeight; i++)
	{
		// 每列
		for (j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 计算新的灰度值
			*lpSrc = bMap[*lpSrc];
		}
	}

	// 返回
	return TRUE;
}
BOOL WINAPI BitPlaneSlicing(LPSTR lpDIBBits, LONG lmageWidth, LONG  lLineBytes, LONG lmageHeight, int Bit)
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
BOOL WINAPI LogTranslation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lLineBytes)
{
	long i;                 //行循环变量
	long j;                 //列循环变量
	unsigned char* lpSrcUnChr;	//指向像素的指针
	for (i = 0; i < lHeight; i++)// 行
	{
		for (j = 0; j < lWidth; j++)// 列
		{
			lpSrcUnChr = (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;// 指向DIB第i行，第j个像素的指针
			//*lpSrcUnChr=50*(unsigned char)log((*lpSrcUnChr+1));
			*lpSrcUnChr = 255 * (unsigned char)log((*lpSrcUnChr + 1)) / log(255);
		}
	}
	return TRUE;
}
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,int iTempH, int iTempW,int iTempMX, int iTempMY,FLOAT* fpArray, FLOAT fCoef)
{
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					// 保存象素值
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}

			// 乘上系数
			fResult *= fCoef;

			// 取绝对值
			fResult = (FLOAT)fabs(fResult);

			// 判断是否超过255
			if (fResult > 255)
			{
				// 直接赋值为255
				*lpDst = 255;
			}
			else
			{
				// 赋值
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;

}
unsigned char WINAPI GetMedianNum(unsigned char* bArray, int iFilterLen)
{
	// 循环变量
	int		i;
	int		j;

	// 中间变量
	unsigned char bTemp;

	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j++)
	{
		for (i = 0; i < iFilterLen - j - 1; i++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}

	// 返回中值
	return bTemp;
}
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,
	int iFilterH, int iFilterW,
	int iFilterMX, int iFilterMY)
{

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 指向复制图像的指针
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;

	// 指向滤波器数组的指针
	unsigned char* aValue;
	HLOCAL			hArray;

	// 循环变量
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;

	// 图像每行的字节数
	LONG			lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 暂时分配内存，以保存滤波器数组
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);

	// 判断是否内存分配失败
	if (hArray == NULL)
	{
		// 释放内存
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);

		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	aValue = (unsigned char*)LocalLock(hArray);

	// 开始中值滤波
	// 行(除去边缘几行)
	for (i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 读取滤波器数组
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;

					// 保存象素值
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}

			// 获取中值
			*lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);

	// 返回
	return TRUE;
}
BOOL WINAPI SaltNoiseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	//循环变量
	long i;
	long j;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//生成伪随机种子
	srand((unsigned)time(NULL));

	//在图像中加噪
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lLineBytes; i++)
		{
			if (rand() > 31500)
			{
				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lLineBytes * j + i;

				//图像中当前点置为黑
				*lpSrc = 0;
			}
		}
	}
	// 返回
	return true;
}
BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;

	//像素值
	double result;
	unsigned char pixel[4];

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	//使用水平方向的结构元素进行腐蚀
	for (j = lHeight - 1; j > 0; j--)
	{
		for (i = 0; i < lWidth - 1; i++)
		{
			//由于使用2×2的模板，为防止越界，所以不处理最下边和最右边的两列像素

			// 指向源图像第j行，第i个象素的指针			
			lpSrc = (char*)lpDIBBits + lWidth * j + i;

			// 指向目标图像第j行，第i个象素的指针			
			lpDst = (char*)lpNewDIBBits + lWidth * j + i;

			//取得当前指针处2*2区域的像素值，注意要转换为unsigned char型
			pixel[0] = (unsigned char)*lpSrc;
			pixel[1] = (unsigned char)*(lpSrc + 1);
			pixel[2] = (unsigned char)*(lpSrc - lWidth);
			pixel[3] = (unsigned char)*(lpSrc - lWidth + 1);

			//计算目标图像中的当前点
			result = sqrt((pixel[0] - pixel[3]) * (pixel[0] - pixel[3]) + \
				(pixel[1] - pixel[2]) * (pixel[1] - pixel[2]));
			*lpDst = (unsigned char)result;

		}
	}

	// 复制锐化后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

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

	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Sobel模板参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}
BOOL WINAPI KirschDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

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

	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Kirsch模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Kirsch模板2参数
	aTemplate[0] = -3.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板3参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板4参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板5参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板6参数
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板7参数
	aTemplate[0] = 5.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 拷贝源图像到缓存图像中
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// 设置Kirsch模板8参数
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}
BOOL WINAPI GaussDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

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

	//模板数组
	FLOAT aTemplate[25];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Gauss模板参数
	iTempW = 5;
	iTempH = 5;
	fTempC = 1.0;
	iTempMX = 3;
	iTempMY = 3;
	aTemplate[0] = -2.0;
	aTemplate[1] = -4.0;
	aTemplate[2] = -4.0;
	aTemplate[3] = -4.0;
	aTemplate[4] = -2.0;
	aTemplate[5] = -4.0;
	aTemplate[6] = 0.0;
	aTemplate[7] = 8.0;
	aTemplate[8] = 0.0;
	aTemplate[9] = -4.0;
	aTemplate[10] = -4.0;
	aTemplate[11] = 8.0;
	aTemplate[12] = 24.0;
	aTemplate[13] = 8.0;
	aTemplate[14] = -4.0;
	aTemplate[15] = -4.0;
	aTemplate[16] = 0.0;
	aTemplate[17] = 8.0;
	aTemplate[18] = 0.0;
	aTemplate[19] = -4.0;
	aTemplate[20] = -2.0;
	aTemplate[21] = -4.0;
	aTemplate[22] = -4.0;
	aTemplate[23] = -4.0;
	aTemplate[24] = -2.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}
BOOL WINAPI PrewittDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向缓存图像的指针
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//循环变量
	long i;
	long j;

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

	//模板数组
	FLOAT aTemplate[9];

	// 暂时分配内存，以保存新图像
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// 暂时分配内存，以保存新图像
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// 拷贝源图像到缓存图像中
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// 设置Prewitt模板1参数
	iTempW = 3;
	iTempH = 3;
	fTempC = 1.0;
	iTempMX = 1;
	iTempMY = 1;
	aTemplate[0] = -1.0;
	aTemplate[1] = -1.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 1.0;
	aTemplate[8] = 1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// 设置Prewitt模板2参数
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// 调用Template()函数
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//求两幅缓存图像的最大值
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// 指向缓存图像1倒数第j行，第i个象素的指针			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// 指向缓存图像2倒数第j行，第i个象素的指针			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// 复制经过模板运算后的图像到源图像
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// 返回
	return TRUE;
}
//canny算子：
void MakeGauss(double sigma, double** pdKernel, int* pnWindowSize)
{
	// 循环控制变量
	int i;

	// 数组的中心点
	int nCenter;

	// 数组的某一点到中心点的距离
	double  dDis;

	//double PI = 3.14159;
	// 中间变量
	double  dValue;
	double  dSum;
	dSum = 0;

	// 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
	// 这些数据会覆盖绝大部分的滤波系数
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);

	// 中心
	nCenter = (*pnWindowSize) / 2;

	// 分配内存
	*pdKernel = new double[*pnWindowSize];

	for (i = 0; i < (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1 / 2) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * PI) * sigma);
		(*pdKernel)[i] = dValue;
		dSum += dValue;
	}

	// 归一化
	for (i = 0; i < (*pnWindowSize); i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}
void GaussianSmooth(unsigned char* pUnchImg, int nWidth, int nHeight,
	double sigma, unsigned char* pUnchSmthdImg)
{
	// 循环控制变量
	int y;
	int x;

	int i;

	// 高斯滤波器的数组长度

	int nWindowSize;

	//  窗口长度的1/2
	int	nHalfLen;

	// 一维高斯数据滤波器
	double* pdKernel;

	// 高斯系数与图象数据的点乘
	double  dDotMul;

	// 高斯滤波系数的总和
	double  dWeightSum;

	// 中间变量
	double* pdTmp;

	// 分配内存
	pdTmp = new double[nWidth * nHeight];

	// 产生一维高斯数据滤波器
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize);

	// MakeGauss返回窗口的长度，利用此变量计算窗口的半长
	nHalfLen = nWindowSize / 2;

	// x方向进行滤波
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			dDotMul = 0;
			dWeightSum = 0;
			for (i = (-nHalfLen); i <= nHalfLen; i++)
			{
				// 判断是否在图象内部
				if ((i + x) >= 0 && (i + x) < nWidth)
				{
					dDotMul += (double)pUnchImg[y * nWidth + (i + x)] * pdKernel[nHalfLen + i];
					dWeightSum += pdKernel[nHalfLen + i];
				}
			}
			pdTmp[y * nWidth + x] = dDotMul / dWeightSum;
		}
	}

	// y方向进行滤波
	for (x = 0; x < nWidth; x++)
	{
		for (y = 0; y < nHeight; y++)
		{
			dDotMul = 0;
			dWeightSum = 0;
			for (i = (-nHalfLen); i <= nHalfLen; i++)
			{
				// 判断是否在图象内部
				if ((i + y) >= 0 && (i + y) < nHeight)
				{
					dDotMul += (double)pdTmp[(y + i) * nWidth + x] * pdKernel[nHalfLen + i];
					dWeightSum += pdKernel[nHalfLen + i];
				}
			}
			pUnchSmthdImg[y * nWidth + x] = (unsigned char)(int)dDotMul / dWeightSum;
		}
	}

	// 释放内存
	delete[]pdKernel;
	pdKernel = NULL;

	delete[]pdTmp;
	pdTmp = NULL;
}

void DirGrad(unsigned char* pUnchSmthdImg, int nWidth, int nHeight,
	int* pnGradX, int* pnGradY)
{
	// 循环控制变量
	int y;
	int x;

	// 计算x方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			pnGradX[y * nWidth + x] = (int)(pUnchSmthdImg[y * nWidth + min(nWidth - 1, x + 1)]
				- pUnchSmthdImg[y * nWidth + max(0, x - 1)]);
		}
	}

	// 计算y方向的方向导数，在边界出进行了处理，防止要访问的象素出界
	for (x = 0; x < nWidth; x++)
	{
		for (y = 0; y < nHeight; y++)
		{
			pnGradY[y * nWidth + x] = (int)(pUnchSmthdImg[min(nHeight - 1, y + 1) * nWidth + x]
				- pUnchSmthdImg[max(0, y - 1) * nWidth + x]);
		}
	}
}
void GradMagnitude(int* pnGradX, int* pnGradY, int nWidth, int nHeight, int* pnMag)
{

	// 循环控制变量
	int y;
	int x;

	// 中间变量
	double dSqtOne;
	double dSqtTwo;

	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			dSqtOne = pnGradX[y * nWidth + x] * pnGradX[y * nWidth + x];
			dSqtTwo = pnGradY[y * nWidth + x] * pnGradY[y * nWidth + x];
			pnMag[y * nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}
void NonmaxSuppress(int* pnMag, int* pnGradX, int* pnGradY, int nWidth,
	int nHeight, unsigned char* pUnchRst)
{
	// 循环控制变量
	int y;
	int x;
	int nPos;

	// x方向梯度分量
	int gx;
	int gy;

	// 临时变量
	int g1, g2, g3, g4;
	double weight;
	double dTmp1;
	double dTmp2;
	double dTmp;

	// 设置图象边缘部分为不可能的边界点
	for (x = 0; x < nWidth; x++)
	{
		pUnchRst[x] = 0;
		pUnchRst[nHeight - 1 + x] = 0;
	}
	for (y = 0; y < nHeight; y++)
	{
		pUnchRst[y * nWidth] = 0;
		pUnchRst[y * nWidth + nWidth - 1] = 0;
	}

	for (y = 1; y < nHeight - 1; y++)
	{
		for (x = 1; x < nWidth - 1; x++)
		{
			nPos = y * nWidth + x;

			// 如果当前象素的梯度幅度为0，则不是边界点
			if (pnMag[nPos] == 0)
			{
				pUnchRst[nPos] = 0;
			}
			else
			{
				// 当前象素的梯度幅度
				dTmp = pnMag[nPos];

				// x，y方向导数
				gx = pnGradX[nPos];
				gy = pnGradY[nPos];

				// 如果方向导数y分量比x分量大，说明导数的方向更加“趋向”于y分量。
				if (abs(gy) > abs(gx))
				{
					// 计算插值的比例
					weight = fabs(gx) / fabs(gy);

					g2 = pnMag[nPos - nWidth];
					g4 = pnMag[nPos + nWidth];

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx * gy > 0)
					{
						g1 = pnMag[nPos - nWidth - 1];
						g3 = pnMag[nPos + nWidth + 1];
					}

					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else
					{
						g1 = pnMag[nPos - nWidth + 1];
						g3 = pnMag[nPos + nWidth - 1];
					}
				}

				// 如果方向导数x分量比y分量大，说明导数的方向更加“趋向”于x分量
				// 这个判断语句包含了x分量和y分量相等的情况
				else
				{
					// 计算插值的比例
					weight = fabs(gy) / fabs(gx);

					g2 = pnMag[nPos + 1];
					g4 = pnMag[nPos - 1];

					// 如果x，y两个方向的方向导数的符号相同
					// C是当前象素，与g1-g4的位置关系为：
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx * gy > 0)
					{
						g1 = pnMag[nPos + nWidth + 1];
						g3 = pnMag[nPos - nWidth - 1];
					}
					// 如果x，y两个方向的方向导数的符号相反
					// C是当前象素，与g1-g4的位置关系为：
					//	     g1
					//	g4 C g2       
					//  g3     
					else
					{
						g1 = pnMag[nPos - nWidth + 1];
						g3 = pnMag[nPos + nWidth - 1];
					}
				}

				// 下面利用g1-g4对梯度进行插值
				{
					dTmp1 = weight * g1 + (1 - weight) * g2;
					dTmp2 = weight * g3 + (1 - weight) * g4;

					// 当前象素的梯度是局部的最大值
					// 该点可能是个边界点
					if (dTmp >= dTmp1 && dTmp >= dTmp2)
					{
						pUnchRst[nPos] = 128;
					}
					else
					{
						// 不可能是边界点
						pUnchRst[nPos] = 0;
					}
				}
			} //else
		} // for

	}
}
void EstimateThreshold(int* pnMag, int nWidth, int nHeight, int* pnThdHigh, int* pnThdLow,
	unsigned char* pUnchEdge, double dRatioHigh, double dRationLow)
{
	// 循环控制变量
	int y;
	int x;
	int k;

	// 该数组的大小和梯度值的范围有关，如果采用本程序的算法，那么梯度的范围不会超过pow(2,10)
	int nHist[1024];

	// 可能的边界数目
	int nEdgeNb;

	// 最大梯度值
	int nMaxMag;

	int nHighCount;

	nMaxMag = 0;

	// 初始化
	for (k = 0; k < 1024; k++)
	{
		nHist[k] = 0;
	}

	// 统计直方图，然后利用直方图计算阈值
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			// 只是统计那些可能是边界点，并且还没有处理过的象素
			if (pUnchEdge[y * nWidth + x] == 128)
			{
				nHist[pnMag[y * nWidth + x]]++;
			}
		}
	}

	nEdgeNb = nHist[0];
	nMaxMag = 0;
	// 统计经过“非最大值抑止(non-maximum suppression)”后有多少象素
	for (k = 1; k < 1024; k++)
	{
		if (nHist[k] != 0)
		{
			// 最大梯度值
			nMaxMag = k;
		}

		// 梯度为0的点是不可能为边界点的
		// 经过non-maximum suppression后有多少象素
		nEdgeNb += nHist[k];
	}

	// 梯度比高阈值*pnThdHigh小的象素点总数目
	nHighCount = (int)(dRatioHigh * nEdgeNb + 0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// 计算高阈值
	while ((k < (nMaxMag - 1)) && (nEdgeNb < nHighCount))
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// 设置高阈值
	*pnThdHigh = k;

	// 设置低阈值
	*pnThdLow = (int)((*pnThdHigh) * dRationLow + 0.5);
}

void TraceEdge(int y, int x, int nLowThd, unsigned char* pUnchEdge, int* pnMag, int nWidth)
{
	// 对8邻域象素进行查询
	int xNb[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	int yNb[8] = { 0, 1, 1, 1,0 ,-1,-1,-1 };

	int yy;
	int xx;

	int k;

	for (k = 0; k < 8; k++)
	{
		yy = y + yNb[k];
		xx = x + xNb[k];
		// 如果该象素为可能的边界点，又没有处理过
		// 并且梯度大于阈值
		if (pUnchEdge[yy * nWidth + xx] == 128 && pnMag[yy * nWidth + xx] >= nLowThd)
		{
			// 把该点设置成为边界点
			pUnchEdge[yy * nWidth + xx] = 255;

			// 以该点为中心进行跟踪
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
}

void Hysteresis(int* pnMag, int nWidth, int nHeight, double dRatioLow,
	double dRatioHigh, unsigned char* pUnchEdge)
{
	// 循环控制变量
	int y;
	int x;

	int nThdHigh;
	int nThdLow;

	int nPos;

	// 估计TraceEdge需要的低阈值，以及Hysteresis函数使用的高阈值
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh,
		&nThdLow, pUnchEdge, dRatioHigh, dRatioLow);

	// 这个循环用来寻找大于nThdHigh的点，这些点被用来当作边界点，然后用
	  // TraceEdge函数来跟踪该点对应的边界
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			nPos = y * nWidth + x;

			// 如果该象素是可能的边界点，并且梯度大于高阈值，该象素作为
			// 一个边界的起点
			if ((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
			{
				// 设置该点为边界点
				pUnchEdge[nPos] = 255;
				TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
			}
		}
	}

	// 那些还没有被设置为边界点的象素已经不可能成为边界点
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			nPos = y * nWidth + x;
			if (pUnchEdge[nPos] != 255)
			{
				// 设置为非边界点
				pUnchEdge[nPos] = 0;
			}
		}
	}
}
void Canny(unsigned char* pUnchImage, int nWidth, int nHeight, double sigma,
	double dRatioLow, double dRatioHigh, unsigned char* pUnchEdge)
{
	// 经过高斯滤波后的图象数据
	unsigned char* pUnchSmooth;

	// 指向x方向导数的指针
	int* pnGradX;

	// 指向y方向导数的指针
	int* pnGradY;

	// 梯度的幅度
	int* pnGradMag;

	pUnchSmooth = new unsigned char[nWidth * nHeight];
	pnGradX = new int[nWidth * nHeight];
	pnGradY = new int[nWidth * nHeight];
	pnGradMag = new int[nWidth * nHeight];

	// 对原图象进行滤波
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth);

	// 计算方向导数
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY);

	// 计算梯度的幅度
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag);

	// 应用non-maximum 抑制
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge);

	// 应用Hysteresis，找到所有的边界
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// 释放内存
	delete pnGradX;
	pnGradX = NULL;
	delete pnGradY;
	pnGradY = NULL;
	delete pnGradMag;
	pnGradMag = NULL;
	delete pUnchSmooth;
	pUnchSmooth = NULL;
}
//灰度图二值化运算：
BOOL WINAPI ManualBinarization(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int offset)
{
	unsigned char* lpSrc;

	LONG	i;
	LONG	j;

	unsigned char Threshold;

	Threshold = offset;

	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < lHeight; i++)
	{
		//逐列扫描
		for (j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
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
	return true;
}

//形态学运算：
//腐蚀运算：
BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],BOOL option)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if (option == TRUE)
	{
		if (nMode == 0)
		{
			//使用水平方向的结构元素进行腐蚀
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;
					/*	if(type==white)*/
						//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)0;

					//如果源图像中当前点自身或者左右有一个点不是黑色，
					//则将目标图像中的当前点赋成白色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + n - 1);
						if (pixel == 255)
						{
							*lpDst = (unsigned char)255;
							break;
						}
					}

				}
			}

		}
		else if (nMode == 1)
		{
			//使用垂直方向的结构元素进行腐蚀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)0;

					//如果源图像中当前点自身或者上下有一个点不是黑色，
					//则将目标图像中的当前点赋成白色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + (n - 1) * lWidth);
						if (pixel == 255)
						{
							*lpDst = (unsigned char)255;
							break;
						}
					}

				}
			}

		}
		else
		{
			//使用自定义的结构元素进行腐蚀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
					//和最上边和最下边的两列像素
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)0;

					//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
					//则将目标图像中的当前点赋成白色
					//注意在DIB图像中内容是上下倒置的
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (structure[m][n] == -1)
								continue;
							pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
							if (pixel == 255)
							{
								*lpDst = (unsigned char)255;
								break;
							}
						}
					}

				}
			}

		}
	}

	else if (option == FALSE)
	{
		if (nMode == 0)
		{
			//使用水平方向的结构元素进行腐蚀
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)255;

					//如果源图像中当前点自身或者左右有一个点不是白色，
					//则将目标图像中的当前点赋成黑色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + n - 1);
						if (pixel == 0)
						{
							*lpDst = (unsigned char)0;
							break;
						}
					}

				}
			}

		}
		else if (nMode == 1)
		{
			//使用垂直方向的结构元素进行腐蚀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)255;

					//如果源图像中当前点自身或者上下有一个点不是黑色，
					//则将目标图像中的当前点赋成白色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + (n - 1) * lWidth);
						if (pixel == 0)
						{
							*lpDst = (unsigned char)0;
							break;
						}
					}

				}
			}

		}
		else
		{
			//使用自定义的结构元素进行腐蚀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
					//和最上边和最下边的两列像素
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)255;

					//如果原图像中对应结构元素中为黑色的那些点中有一个不是黑色，
					//则将目标图像中的当前点赋成白色
					//注意在DIB图像中内容是上下倒置的
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (structure[m][n] == -1)
								continue;
							pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
							if (pixel == 0)
							{
								*lpDst = (unsigned char)0;
								break;
							}
						}
					}

				}
			}

		}
	}
	

	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
//膨胀运算：
BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], BOOL option)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//像素值
	unsigned char pixel;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if (option == TRUE)
	{
		if (nMode == 0)
		{
			//使用水平方向的结构元素进行膨胀
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && pixel != 0)
						return FALSE;

					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)255;

					//源图像中当前点自身或者左右只要有一个点是黑色，
					//则将目标图像中的当前点赋成黑色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + n - 1);
						if (pixel == 0)
						{
							*lpDst = (unsigned char)0;
							break;
						}
					}

				}
			}

		}
		else if (nMode == 1)
		{
			//使用垂直方向的结构元素进行膨胀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)255;

					//源图像中当前点自身或者上下只要有一个点是黑色，
					//则将目标图像中的当前点赋成黑色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + (n - 1) * lWidth);
						if (pixel == 0)
						{
							*lpDst = (unsigned char)0;
							break;
						}
					}

				}
			}

		}
		else
		{
			//使用自定义的结构元素进行膨胀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
					//和最上边和最下边的两列像素
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)255;

					//原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
					//则将目标图像中的当前点赋成黑色
					//注意在DIB图像中内容是上下倒置的
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (structure[m][n] == -1)
								continue;
							pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
							if (pixel == 0)
							{
								*lpDst = (unsigned char)0;
								break;
							}
						}
					}

				}
			}

		}
	}
	else if (option == FALSE)
	{
		if (nMode == 0)
		{
			//使用水平方向的结构元素进行膨胀
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && pixel != 0)
						return FALSE;

					//目标图像中的当前点先赋成黑色
					*lpDst = (unsigned char)0;

					//源图像中当前点自身或者左右只要有一个点是白色，
					//则将目标图像中的当前点赋成白色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + n - 1);
						if (pixel == 255)
						{
							*lpDst = (unsigned char)255;
							break;
						}
					}

				}
			}

		}
		else if (nMode == 1)
		{
			//使用垂直方向的结构元素进行膨胀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用1×3的结构元素，为防止越界，所以不处理最上边和最下边的两列像素

					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)0;

					//源图像中当前点自身或者上下只要有一个点是黑色，
					//则将目标图像中的当前点赋成黑色
					for (n = 0; n < 3; n++)
					{
						pixel = *(lpSrc + (n - 1) * lWidth);
						if (pixel == 255)
						{
							*lpDst = (unsigned char)255;
							break;
						}
					}

				}
			}

		}
		else
		{
			//使用自定义的结构元素进行膨胀
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//由于使用3×3的结构元素，为防止越界，所以不处理最左边和最右边的两列像素
					//和最上边和最下边的两列像素
					// 指向源图像倒数第j行，第i个象素的指针			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// 指向目标图像倒数第j行，第i个象素的指针			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//取得当前指针处的像素值，注意要转换为unsigned char型
					pixel = (unsigned char)*lpSrc;

					//目标图像中含有0和255外的其它灰度值
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//目标图像中的当前点先赋成白色
					*lpDst = (unsigned char)0;

					//原图像中对应结构元素中为黑色的那些点中只要有一个是黑色，
					//则将目标图像中的当前点赋成黑色
					//注意在DIB图像中内容是上下倒置的
					for (m = 0; m < 3; m++)
					{
						for (n = 0; n < 3; n++)
						{
							if (structure[m][n] == -1)
								continue;
							pixel = *(lpSrc + ((2 - m) - 1) * lWidth + (n - 1));
							if (pixel == 255)
							{
								*lpDst = (unsigned char)255;
								break;
							}
						}
					}

				}
			}

		}
	}
	
	// 复制膨胀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],BOOL option)
{
	//// 图像每行的字节数
	//LONG lLineBytes;
	//// 计算图像每行的字节数
	//lLineBytes = WIDTHBYTES(lWidth * 8);
	//lWidth = lLineBytes;

	if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure,option))
	{
		if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure,option))
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;

	return TRUE;
}
BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], BOOL option)
{
	if (DilationDIB(lpDIBBits, lWidth, lHeight, nMode, structure, option))
	{

		if (ErosionDIB(lpDIBBits, lWidth, lHeight, nMode, structure,option))
		{
			// 返回
			return TRUE;

		}
	}
	return FALSE;

	// 返回
	return TRUE;
}
//灰度窗口变换：
BOOL WINAPI WindowTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bLow, BYTE bUp)
{
	unsigned char* lpSrc;//指原图像指针

	LONG i, j;//循环变量

	LONG	lLineBytes;//每行字节数

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//低于下限的赋0，高于上限的赋255
			if (*lpSrc < bLow)
			{
				*lpSrc = 0;
			}
			else if (*lpSrc > bUp)
			{
				*lpSrc = 255;
			}
		}
	}

	return TRUE;
}


BOOL WINAPI ThiningDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向缓存图像的指针
	LPSTR	lpDst;

	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//脏标记
	BOOL bModified;

	//循环变量
	long i;
	long j;
	int  n;
	int  m;

	//四个条件
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//计数器
	unsigned char nCount;

	//像素值
	unsigned char pixel;

	//5×5相邻区域像素值
	unsigned char neighbour[5][5];

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	bModified = TRUE;

	while (bModified)
	{

		bModified = FALSE;
		// 初始化新分配的内存，设定初始值为255
		lpDst = (char*)lpNewDIBBits;
		memset(lpDst, (BYTE)255, lWidth * lHeight);

		for (j = 2; j < lHeight - 2; j++)
		{
			for (i = 2; i < lWidth - 2; i++)
			{

				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;

				//由于使用5×5的结构元素，为防止越界，所以不处理外围的几行和几列像素

				// 指向源图像倒数第j行，第i个象素的指针			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// 指向目标图像倒数第j行，第i个象素的指针			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//取得当前指针处的像素值，注意要转换为unsigned char型
				pixel = (unsigned char)*lpSrc;

				//目标图像中含有0和255外的其它灰度值
				if (pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//如果源图像中当前点为白色，则跳过
				else if (pixel == 255)
					continue;

				//获得当前点相邻的5×5区域内像素值，白色用0代表，黑色用1代表
				for (m = 0; m < 5; m++)
				{
					for (n = 0; n < 5; n++)
					{
						neighbour[m][n] = (255 - (unsigned char)*(lpSrc + ((4 - m) - 2) * lWidth + n - 2)) / 255;
					}
				}
				//				neighbour[][]
								//逐个判断条件。
								//判断2<=NZ(P1)<=6
				nCount = neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
					+ neighbour[2][1] + neighbour[2][3] + \
					+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if (nCount >= 2 && nCount <= 6)
					bCondition1 = TRUE;

				//判断Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;

				//判断P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2] * neighbour[2][1] * neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}

				//判断P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2] * neighbour[2][1] * neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				if (bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					*lpDst = (unsigned char)255;
					bModified = TRUE;
				}
				else
				{
					*lpDst = (unsigned char)0;
				}
			}
		}
		// 复制腐蚀后的图像
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);


	}
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
//直方图均衡化：
BOOL WINAPI GrayEqualization(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	unsigned char* lpSrc;

	LONG i, j;//循环变量

	// 计算图像每行的字节数
	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 灰度映射表
	BYTE	GrayMap[256];

	//灰度计数表
	LONG GrayCount[256];

	for (i = 0; i < 256; i++)
	{
		GrayCount[i] = 0;//初始化为0
	}

	//统计各个灰度值的个数
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			GrayCount[*lpSrc]++;//该像素的灰度值个数+1
		}
	}

	//制作灰度值映射表
	for (i = 0; i < 256; i++)
	{
		LONG temp;//计算累计值
		temp = 0;
		for (j = 0; j <= i; j++)
		{
			temp = temp + GrayCount[j];
		}
		//得到新灰度值
		GrayMap[i] = (BYTE)(temp * 255 / lWidth / lHeight);
	}

	//更新每个像素均衡化后的灰度值
	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			*lpSrc = GrayMap[*lpSrc];
		}
	}

	return TRUE;
}
//滤镜模板：
BOOL WINAPI MyTemplate(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef, FLOAT yuzhi)
{
	//复制图像的指针
	LPSTR lpNewDIBBits;
	HLOCAL hNewDIBBits;

	//原图像的指针
	unsigned char* lpSrc;

	//指向复制区域的指针
	unsigned char* lpDst;

	LONG i, j, k, l;

	FLOAT fResult;//计算结果

	LONG lLineBytes;//每行字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// 计算
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					// 保存象素值
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}

			// 乘上系数
			fResult *= fCoef;

			//加上阈值
			fResult += yuzhi;

			// 取绝对值
			fResult = (FLOAT)fabs(fResult);

			// 判断是否超过255
			if (fResult > 255)
			{
				// 直接赋值为255
				*lpDst = 255;
			}
			else
			{
				// 赋值
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);



	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;

}
BOOL WINAPI ImageTranslation(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG XOffset, LONG YOffset)//图像平移
{
	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向要复制区域的指针
	LPSTR	lpDst;

	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 象素在平移后图像的坐标
	LONG	i;
	LONG	j;

	// 象素在原图像的坐标
	LONG	i0;
	LONG	j0;


	// 计算图像每行的字节数
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			//平移后像素点
			lpDst = (char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 计算该象素在原图像中的坐标
			i0 = i - XOffset;
			j0 = j - YOffset;

			// 判断是否在源图范围内
			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 指向源原图像第i0行，第j0个象素的指针
				lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				*((unsigned char*)lpDst) = 255;
			}

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

//图像旋转：
HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle)
{
	// 源图像的宽度和高度
	LONG	lWidth;
	LONG	lHeight;

	// 旋转后图像的宽度和高度
	LONG	lNewWidth;
	LONG	lNewHeight;

	// 图像每行的字节数
	LONG	lLineBytes;

	// 旋转后图像的宽度（lNewWidth'，必须是4的倍数）
	LONG	lNewLineBytes;

	// 指向源图像的指针
	LPSTR	lpDIBBits;

	// 指向源象素的指针
	LPSTR	lpSrc;

	// 旋转后新DIB句柄
	HDIB	hDIB;

	// 指向旋转图像对应象素的指针
	LPSTR	lpDst;

	// 指向旋转图像的指针
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;

	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;

	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 循环变量（象素在新DIB中的坐标）
	LONG	i;
	LONG	j;

	// 象素在源DIB中的坐标
	LONG	i0;
	LONG	j0;

	// 旋转角度（弧度）
	float	fRotateAngle;

	// 旋转角度的正弦和余弦
	float	fSina, fCosa;

	// 源图四个角的坐标（以图像中心为坐标系原点）
	float	fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;

	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float	fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	// 两个中间常量
	float	f1, f2;

	// 找到源DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);

	// 获取图像的"宽度"（4的倍数）
	lWidth = ::DIBWidth(lpDIB);

	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// 获取图像的高度
	lHeight = ::DIBHeight(lpDIB);

	// 将旋转角度从度转换到弧度
	fRotateAngle = (float)RADIAN(iRotateAngle);

	// 计算旋转角度的正弦
	fSina = (float)sin((double)fRotateAngle);

	// 计算旋转角度的余弦
	fCosa = (float)cos((double)fRotateAngle);

	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float)(-(lWidth - 1) / 2);
	fSrcY1 = (float)((lHeight - 1) / 2);
	fSrcX2 = (float)((lWidth - 1) / 2);
	fSrcY2 = (float)((lHeight - 1) / 2);
	fSrcX3 = (float)(-(lWidth - 1) / 2);
	fSrcY3 = (float)(-(lHeight - 1) / 2);
	fSrcX4 = (float)((lWidth - 1) / 2);
	fSrcY4 = (float)(-(lHeight - 1) / 2);

	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// 计算旋转后的图像实际宽度
	lNewWidth = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

	// 计算新图像每行的字节数
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// 计算旋转后的图像高度
	lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	// 两个常数，这样不用以后每次都计算了
	f1 = (float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth - 1));
	f2 = (float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));

	// 分配内存，以保存新DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));

	// 判断是否内存分配失败
	if (hDIB == NULL)
	{
		// 分配内存失败
		return NULL;
	}

	// 锁定内存
	lpNewDIB = (char*)::GlobalLock((HGLOBAL)hDIB);

	// 复制DIB信息头和调色板
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));

	// 找到新DIB象素起始位置
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);

	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// 更新DIB中图像的高度和宽度
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// 对于Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// 对于其它格式的DIB
		lpbmc->bcWidth = (unsigned short)lNewWidth;
		lpbmc->bcHeight = (unsigned short)lNewHeight;
	}

	// 针对图像每行进行操作
	for (i = 0; i < lNewHeight; i++)
	{
		// 针对图像每列进行操作
		for (j = 0; j < lNewWidth; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			// 注意此处宽度和高度是新DIB的宽度和高度
			lpDst = (char*)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// 计算该象素在源DIB中的坐标
			i0 = (LONG)(-((float)j) * fSina + ((float)i) * fCosa + f2 + 0.5);
			j0 = (LONG)(((float)j) * fCosa + ((float)i) * fSina + f1 + 0.5);

			// 判断是否在源图范围内
			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// 指向源DIB第i0行，第j0个象素的指针
				lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// 复制象素
				*lpDst = *lpSrc;
			}
			else
			{
				// 对于源图中没有的象素，直接赋值为255
				*((unsigned char*)lpDst) = 255;
			}

		}

	}

	// 返回
	return hDIB;
}
//图像转置：
BOOL WINAPI TransposeDIB(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{


	// 指向源象素的指针
	LPSTR	lpSrc;

	// 指向转置图像对应象素的指针
	LPSTR	lpDst;

	// 指向转置图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFOHEADER lpbmi;

	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREHEADER lpbmc;

	// 循环变量
	LONG	i;
	LONG	j;

	// 图像每行的字节数
	LONG lLineBytes;

	// 新图像每行的字节数
	LONG lNewLineBytes;

	// 获取指针
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;


	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 计算新图像每行的字节数
	lNewLineBytes = (lHeight * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lNewLineBytes);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 针对图像每行进行操作
	for (i = 0; i < lHeight; i++)
	{
		// 针对每行图像每列进行操作
		for (j = 0; j < lWidth; j++)
		{

			// 指向源DIB第i行，第j个象素的指针
			lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// 指向转置DIB第j行，第i个象素的指针
			// 注意此处lWidth和lHeight是源DIB的宽度和高度，应该互换
			lpDst = (char*)lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;

			// 复制象素
			*lpDst = *lpSrc;

		}

	}

	// 复制转置后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lNewLineBytes);

	// 互换DIB中图像的高宽

		// 对于Windows 3.0 DIB
	lpbmi->biWidth = lHeight;

	lpbmi->biHeight = lWidth;


	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}
//图像镜像：
BOOL WINAPI MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection)
{

	// 指向源图像的指针
	LPSTR	lpSrc;

	// 指向要复制区域的指针
	LPSTR	lpDst;

	// 指向复制图像的指针
	LPSTR	lpBits;
	HLOCAL	hBits;

	// 循环变量
	LONG	i;
	LONG	j;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// 暂时分配内存，以保存一行图像
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpBits = (char*)LocalLock(hBits);

	// 判断镜像方式
	if (bDirection)
	{
		// 水平镜像

		// 针对图像每行进行操作
		for (i = 0; i < lHeight; i++)
		{
			// 针对每行图像左半部分进行操作
			for (j = 0; j < lWidth / 2; j++)
			{

				// 指向倒数第i行，第j个象素的指针
				lpSrc = (char*)lpDIBBits + lLineBytes * i + j;

				// 指向倒数第i行，倒数第j个象素的指针
				lpDst = (char*)lpDIBBits + lLineBytes * (i + 1) - j;

				// 备份一个象素
				*lpBits = *lpDst;

				// 将倒数第i行，第j个象素复制到倒数第i行，倒数第j个象素
				*lpDst = *lpSrc;

				// 将倒数第i行，倒数第j个象素复制到倒数第i行，第j个象素
				*lpSrc = *lpBits;
			}

		}
	}
	else
	{
		// 垂直镜像

		// 针对上半图像进行操作
		for (i = 0; i < lHeight / 2; i++)
		{

			// 指向倒数第i行象素起点的指针
			lpSrc = (char*)lpDIBBits + lLineBytes * i;

			// 指向第i行象素起点的指针
			lpDst = (char*)lpDIBBits + lLineBytes * (lHeight - i - 1);

			// 备份一行，宽度为lWidth
			memcpy(lpBits, lpDst, lLineBytes);

			// 将倒数第i行象素复制到第i行
			memcpy(lpDst, lpSrc, lLineBytes);

			// 将第i行象素复制到倒数第i行
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}

	// 释放内存
	LocalUnlock(hBits);
	LocalFree(hBits);

	// 返回
	return TRUE;
}
//油画风格：
BOOL WINAPI Oil_Painting(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight)//油画效果
{
	// 指向源图像的指针
	unsigned char* lpSrc;
	unsigned char* lpSrctemp;
	// 循环变量
	LONG	i;
	LONG	j;
	// 图像每行的字节数
	LONG	lLineBytes;
	int temp, k;
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * BitCount);
	int page = 1;
	if (BitCount == 24) page = 3;
	if (BitCount == 8) page = 1;
	int iModel;
	int mmin;
	if (lWidth < lHeight)//选择长宽小的
	{
		mmin = lWidth;
	}
	else
	{
		mmin = lHeight;
	}

	if (mmin < 500)//根据图像大小选择iModel
	{
		iModel = 8;
	}
	else if (mmin < 1000)
	{
		iModel = 12;
	}
	else
	{
		iModel = 20;
	}
	//逐行扫描
	for (i = 0; i <= lHeight - iModel; i++)
	{

		//逐列扫描
		for (j = 0; j <= lWidth - iModel; j++)
		{


			//指向DIB第i行，第j个象素的指针
			for (k = 0; k < page; k++)
			{
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + k;

				/*int ran = rand() % iModel;

				int x1 = i + ran, y1 = j + ran;
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - x1 - 1) + y1;
				lpSrctemp = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - i - 1) + j;
				*lpSrctemp = *lpSrc;*/

				int ran = rand() % iModel;

				int x1 = i + ran, y1 = j + ran;
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - x1 - 1) + y1 * page + k;
				lpSrctemp = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - i - 1) + j * page + k;
				*lpSrctemp = *lpSrc;
			}
		}
	}


	return true;
}
BOOL WINAPI USMSharp(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef, int BitCount)
{
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// 指向源图像的指针
	unsigned char* lpSrc;

	// 指向要复制区域的指针
	unsigned char* lpDst;

	// 循环变量
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;
	LONG    m;

	// 计算结果
	FLOAT	fResult;

	// 图像每行的字节数
	LONG lLineBytes;

	// 计算图像每行的字节数
	//lLineBytes = WIDTHBYTES(lWidth * 8);
	lLineBytes = WIDTHBYTES(lWidth * BitCount);

	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}

	// 锁定内存
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	int page = 1;
	if (BitCount == 24) page = 3;
	if (BitCount == 8) page = 1;

	// 行(除去边缘几行)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// 列(除去边缘几列)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			for (m = 0; m < page; m++) {
				// 指向新DIB第i行，第j个象素的指针
				lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + m;

				fResult = 0;

				// 计算
				for (k = 0; k < iTempH; k++)
				{
					for (l = 0; l < iTempW; l++)
					{
						// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
						lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
							+ j - iTempMX + l;

						// 保存象素值
						fResult += (*lpSrc) * fpArray[k * iTempW + l];
					}
				}

				// 乘上系数
				fResult *= fCoef;

				// 取绝对值
				fResult = (FLOAT)fabs(fResult);

				// 判断是否超过255
				if (fResult > 255)
				{
					// 直接赋值为255
					*lpDst = 255;
				}
				else
				{
					// 赋值
					*lpDst = (unsigned char)(fResult + 0.5);
				}


			}
			//// 指向新DIB第i行，第j个象素的指针
			//lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			//fResult = 0;

			//// 计算
			//for (k = 0; k < iTempH; k++)
			//{
			//	for (l = 0; l < iTempW; l++)
			//	{
			//		// 指向DIB第i - iTempMY + k行，第j - iTempMX + l个象素的指针
			//		lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
			//			+ j - iTempMX + l;

			//		// 保存象素值
			//		fResult += (*lpSrc) * fpArray[k * iTempW + l];
			//	}
			//}

			//// 乘上系数
			//fResult *= fCoef;

			//// 取绝对值
			//fResult = (FLOAT)fabs(fResult);

			//// 判断是否超过255
			//if (fResult > 255)
			//{
			//	// 直接赋值为255
			//	*lpDst = 255;
			//}
			//else
			//{
			//	// 赋值
			//	*lpDst = (unsigned char)(fResult + 0.5);
			//}

			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)+ j - iTempMX + l;
			//*lpDst = (*lpSrc) + 50 * (*lpDst);

		}
	}

	int temp;


	//逐行扫描
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		//逐列扫描
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			for (k = 0; k < page; k++)
			{
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + k;
				lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + k;
				// 变换
				temp = (((*lpSrc) - 0.3 * (*lpDst)) / 0.7);

				// 判断是否超过255
				if (temp > 255)
				{
					// 直接赋值为255
					*lpDst = 255;
				}
				else {
					*lpDst = temp;
				}
			}
		}
	}







	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 返回
	return TRUE;
}

//区域计数
void findsorrounding8(int* t, int j, int i, int  lSrcHeight, int x, int number)//寻找8连通像素
{
	t[j * x + i] = number;
	if (i < x - 1)
		if (t[j * x + i + 1] == 0)
		{
			t[j * x + i + 1] = number;
			findsorrounding8(t, j, i + 1, lSrcHeight, x, number);
		}
	if (i > 0)
		if (t[j * x + i - 1] == 0)
		{
			t[j * x + i - 1] = number;
			findsorrounding8(t, j, i - 1, lSrcHeight, x, number);
		}
	if (j < lSrcHeight - 1)
		if (t[(j + 1) * x + i] == 0)
		{
			t[(j + 1) * x + i] = number;
			findsorrounding8(t, j + 1, i, lSrcHeight, x, number);
		}
	if (j > 0)
		if (t[(j - 1) * x + i] == 0)
		{
			t[(j - 1) * x + i] = number;
			findsorrounding8(t, j - 1, i, lSrcHeight, x, number);
		}
	if (i < x - 1 && j < lSrcHeight - 1)
		if (t[(j + 1) * x + i + 1] == 0)
		{
			t[(j + 1) * x + i + 1] = number;
			findsorrounding8(t, j + 1, i + 1, lSrcHeight, x, number);
		}
	if (i > 0 && j < lSrcHeight - 1)
		if (t[(j + 1) * x + i - 1] == 0)
		{
			t[(j + 1) * x + i - 1] = number;
			findsorrounding8(t, j + 1, i - 1, lSrcHeight, x, number);
		}
	if (j > 0 && i < x - 1)
		if (t[(j - 1) * x + i + 1] == 0)
		{
			t[(j - 1) * x + i + 1] = number;
			findsorrounding8(t, j - 1, i + 1, lSrcHeight, x, number);
		}
	if (j > 0 && i > 0)
		if (t[(j - 1) * x + i - 1] == 0)
		{
			t[(j - 1) * x + i - 1] = number;
			findsorrounding8(t, j - 1, i - 1, lSrcHeight, x, number);
		}
}

void findsorrounding4(int* t, int j, int i, int  lSrcHeight, int x, int number)//寻找4连通像素
{
	t[j * x + i] = number;
	if (i < x - 1)
		if (t[j * x + i + 1] == 0)
		{
			t[j * x + i + 1] = number;
			findsorrounding4(t, j, i + 1, lSrcHeight, x, number);
		}
	if (i > 0)
		if (t[j * x + i - 1] == 0)
		{
			t[j * x + i - 1] = number;
			findsorrounding4(t, j, i - 1, lSrcHeight, x, number);
		}
	if (j < lSrcHeight - 1)
		if (t[(j + 1) * x + i] == 0)
		{
			t[(j + 1) * x + i] = number;
			findsorrounding4(t, j + 1, i, lSrcHeight, x, number);
		}
	if (j > 0)
		if (t[(j - 1) * x + i] == 0)
		{
			t[(j - 1) * x + i] = number;
			findsorrounding4(t, j - 1, i, lSrcHeight, x, number);
		}
}

double  findciucu8(int* t, int j, int i, int  lSrcHeight, int x, int number)
{
	int num = 0;
	if (t[j * x + i] != number)
		return 0;
	if (i <= 0 || i >= x - 1 || j <= 0 || j >= lSrcHeight - 1)
		return 1;
	if (t[j * x + i + 1] != number)
	{
		num++;
	}
	if (t[j * x + i - 1] != number)
	{
		num++;
	}
	if (t[(j + 1) * x + i] != number)
	{
		num++;
	}
	if (t[(j - 1) * x + i] != number)
	{
		num++;
	}
	if (num == 1)
		return 1;
	if (num == 2)
		return sqrt(2);
	if (num == 3)
		return 2 * sqrt(1.25);
	if (num == 4)
		return 8 * sqrt(2);
	if (num == 0)
		return 0;
}

//返回物体个数
int funcfind(LPSTR lpSrcStartBits, int* t, int  lSrcHeight, int lSrcWidth, int lpSrcBitCount, int way, int bw)//返回物体个数
{
	bool flag = 0;
	int x = (lSrcWidth * lpSrcBitCount / 8 + 3) / 4 * 4;
	int number = 0;
	if (bw)
		for (int j = 0; j < lSrcHeight; j++)
			for (int i = 0; i < x; i++)
				t[j * x + i] = (unsigned char)lpSrcStartBits[j * x + i];
	else
		for (int j = 0; j < lSrcHeight; j++)
			for (int i = 0; i < x; i++)
			{
				t[j * x + i] = (unsigned char)lpSrcStartBits[j * x + i];
				t[j * x + i] = 255 - t[j * x + i];
			}
	for (int j = 0; j < lSrcHeight; j++)
	{
		for (int i = lSrcWidth; i < x; i++) {
			t[j * x + i] = 255;
		}
	}
	while (1) {
		for (int j = 0; j < lSrcHeight; j++)
		{
			for (int i = 0; i < x; i++) {
				if (t[j * x + i] == 0) {
					number++;
					if (way)
						findsorrounding8(t, j, i, lSrcHeight, x, number);
					else
						findsorrounding4(t, j, i, lSrcHeight, x, number);
					flag = 1;
					break;
				}
			}
			if (flag)
				break;
		}
		if (!flag)
			break;
		flag = 0;
	}
	return number;
}

//返回物体周长
double funccircu1(int* t, int  lSrcHeight, int x, int number)//返回物体周长
{
	double result = 0;
	for (int j = 0; j < lSrcHeight; j++)
	{
		for (int i = 0; i < x; i++) {
			result += findciucu8(t, j, i, lSrcHeight, x, number);
		}
	}
	return result;
}

//返回物体面积
int funcfindbig(int* t, int  lSrcHeight, int x, int number, int& area)//返回物体面积
{
	int* num = new int[1000000];
	int result = 0;
	int a;
	for (int j = 1; j < number + 1; j++)
		num[j] = 0;
	for (int j = 0; j < lSrcHeight; j++)
	{
		for (int i = 0; i < x; i++) {
			if (t[j * x + i] != 255)
				num[t[j * x + i]]++;
		}
	}
	for (int j = 1; j < number + 1; j++)
		if (num[j] > result)
		{
			a = j;
			result = num[j];
		}
	area = result;
	delete[]num;
	return a;
}

int WINAPI getCoreX(LONG lWidth, LONG lHeight, LONG area)
{
	int x;
	x = 0;
	for (int i = 0; i < lWidth; i++)
	{
		for (int j = 0; j < lHeight; j++)
		{
			if (temp[i][j] == xy)
			{
				x = x + i;
			}
		}
	}
	if (area == 0)
	{
		area = 1;
	}
	int CoreX = x / area / 20;
	return CoreX;
}

int WINAPI getCoreY(LONG lWidth, LONG lHeight, LONG area)
{
	int y;
	y = 0;
	for (int i = 0; i < lWidth; i++)
	{
		for (int j = 0; j < lHeight; j++)
		{
			if (temp[i][j] == xy)
			{
				y = y + j;
			}
		}
	}
	if (area == 0)
	{
		area = 1;
	}
	int CoreY = y / area / 20;
	return CoreY;
}
