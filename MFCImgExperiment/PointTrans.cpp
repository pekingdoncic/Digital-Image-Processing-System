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
//����ʵ�֣�
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
		assert(color24bit[i] < 4096);  //ͼ����п�ҪΪ4�ı���������
		usedTimes[color24bit[i]]++;
	}
	LONG numberOfColors = 0;
	for (i = 0; i < 4096; i++)
	{
		if (usedTimes[i] > 0)
			numberOfColors++;
	}
	//��usedTimes�����������������minColor����(��������ɫֵ)Ҳ����useTimes  
	//�������ƵĽ���  
	//Sort2(usedTimes, miniColor, 4096);   //ok 
	Sort3(usedTimes, miniColor, 4096);    //ok
	//usedTimes�������Ǹ���ɫʹ��Ƶ�ʣ��Ӹߵ������У���Ȼ��numberOfColor��֮��Ķ�Ϊ0  
	//miniColor����������Ӧ����ɫ����  
	//��ǰ256����ɫ���ݱ��浽256ɫλͼ�ĵ�ɫ����  
	for (i = 0; i < 256; i++)
	{
		mainColor[i].rgbBlue = (BYTE)((miniColor[i] >> 8) << 4);
		mainColor[i].rgbGreen = (BYTE)(((miniColor[i] >> 4) & 0xf) << 4);
		mainColor[i].rgbRed = (BYTE)((miniColor[i] & 0xf) << 4);
		mainColor[i].rgbReserved = 0;
	}

	int* colorIndex = usedTimes;//��ԭ����useTimes��������������ֵ  
	memset(colorIndex, 0, sizeof(int) * 4096);

	if (numberOfColors <= 256)
	{
		for (i = 0; i < numberOfColors; i++)
			colorIndex[miniColor[i]] = i;
	}
	else//Ϊ��256֮�����ɫ��ǰ256����ɫ����һ����ӽ���  
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
	//��¼������ɫ���ݵ�����ֵ����256ɫλͼ����ɫ����  
	for (i = 0; i < len; i++)
	{
		assert(colorIndex[color24bit[i]] < 256);
		Index[i] = colorIndex[color24bit[i]];
	}
	return 1;
}
BOOL WINAPI Tran24ColTo8Col(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ѭ������
	LONG	i;
	LONG	j;
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 24);
	BITMAPINFOHEADER* pHead;
	RGBQUAD* pColorTable = NULL;
	int lineByteNew;
	int biBitCount = 8;
	lineByteNew = (lWidth * biBitCount / 8 + 3) / 4 * 4;
	//�Ҷ�ͼ������ɫ������ɫ�����Ϊ
	if (biBitCount == 8) {
		//������ɫ������Ҫ�Ŀռ䣬����ɫ��ֵ
		pColorTable = new RGBQUAD[256];
		memset(pColorTable, 0, sizeof(RGBQUAD) * 256);
	}
	BYTE* Index = new BYTE[lineByteNew * lHeight]; //ͼ��������������(������Index��)
	WORD* shortColor = new WORD[lineByteNew * lHeight]; //��ɫ�ĸ�4λ
	memset(shortColor, 0, sizeof(WORD) * lineByteNew * lHeight); //����

	int iRed, iGreen, iBlue;
	for (int i = 0; i < lHeight; i++)
	{//ȡRGB��ɫ�ĸ�4λ  
		for (int j = 0; j < lWidth; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j * 3;
			iBlue = (*lpSrc) >> 4;
			iGreen = (*(++lpSrc)) >> 4;
			iRed = (*(++lpSrc)) >> 4;
			shortColor[lineByteNew * (lHeight - 1 - i) + j] = (iBlue << 8) + (iGreen << 4) + iRed;
		}
	}
	//����ת������  24color To->8color
	Transfer(shortColor, lineByteNew * lHeight, Index, pColorTable);

	//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
	unsigned char* pBmpBufNew;
	pBmpBufNew = new unsigned char[sizeof(BITMAPINFOHEADER) + 256 * 4];//������ͼ��Ŀռ�
	memcpy(pBmpBufNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER));//��Ϣͷ����
	pHead = (BITMAPINFOHEADER*)pBmpBufNew;
	pHead->biBitCount = 8;//�ı�λ��,
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

	//����
	memcpy(lpDIB, pBmpBufNew, sizeof(BITMAPINFOHEADER));
	memcpy(lpDIB + sizeof(BITMAPINFOHEADER), pColorTable, sizeof(RGBQUAD) * 256);
	memcpy(lpDIB + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256, Index, lineByteNew * lHeight);

	delete[]Index;
	delete[]shortColor;

	return true;
}
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB) {
	//unsigned char* lpSrc;//ָ��DIBԴ���ص�ָ��
	//FLOAT fTemp;//�м����
	//for (int i = 0; i < lHeight; i++) {
	//	for (int j = 0; j < lWidth; j++) {
	//		lpSrc = (unsigned char*)lpDIBBits + lSrcLineBytes * (lHeight - 1 - i) + j;
	//		fTemp = (float)*lpSrc;
	//		fTemp = fA * fTemp + fB;
	//		//���Ա任
	//		//�ж��Ƿ񳬳���Χ
	//		if (fTemp > 255)
	//		//ֱ�Ӹ�ֵΪ255 
	//			*lpSrc = 255;
	//		else if (fTemp < 0)
	//			// ��������
	//			*lpSrc = 0;
	//		else {
	//			*lpSrc = (unsigned char)(fTemp + 0.5);
	//		}
	//	}
	//}
	//return TRUE;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	// ѭ������
	LONG	i;
	LONG	j;
	// �м����
	FLOAT	fTemp;
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	// ÿ��
	for (i = 0; i < lHeight; i++)
	{
		// ÿ��
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// ���Ա任
			fTemp = fA * (*lpSrc) + fB;

			// �ж��Ƿ񳬳���Χ
			if (fTemp > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpSrc = 255;
			}
			else if (fTemp < 0)
			{
				// ֱ�Ӹ�ֵΪ0
				*lpSrc = 0;
			}
			else
			{
				// ��������
				*lpSrc = (unsigned char)(fTemp + 0.5);
			}
		}
	}
	// ����
	return TRUE;

}
BOOL WINAPI Segmental2_Linear_Tran(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int type, int threshold)
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ѭ������
	LONG	i;
	LONG	j;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// �м����
	FLOAT	fTemp = 0, fA1 = 1, fB1 = 0, fA2 = 1, fB2 = 0;
	//�м�Ҷ�
	int seg_gray = 127;
	seg_gray = threshold;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//�������Ա任�Ĳ���
	if (type == 1)//��V����
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
	if (type == 2)//V����
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
	if (type == 3)//��L����
	{
		if (threshold == 0) {
			// ÿ��
			for (i = 0; i < lHeight; i++)
			{
				// ÿ��
				for (j = 0; j < lWidth; j++)
				{
					// ָ��DIB��i�У���j�����ص�ָ��
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
	if (type == 4)//L����
	{
		if (threshold == 0)
		{
			// ÿ��
			for (i = 0; i < lHeight; i++)
			{
				// ÿ��
				for (j = 0; j < lWidth; j++)
				{
					// ָ��DIB��i�У���j�����ص�ָ��
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
	// ÿ��
	for (i = 0; i < lHeight; i++)
	{
		// ÿ��
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// ���Ա任,�ֶ�
			if (*lpSrc >= 0 && *lpSrc <= seg_gray)
				fTemp = fA1 * (*lpSrc) + fB1;
			if (*lpSrc >= seg_gray + 1 && *lpSrc <= 255)
				fTemp = fA2 * (*lpSrc) + fB2;

			// �ж��Ƿ񳬳���Χ
			if (fTemp > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpSrc = 255;
			}
			else if (fTemp < 0)
			{
				// ֱ�Ӹ�ֵΪ0
				*lpSrc = 0;
			}
			else
			{
				// ��������
				*lpSrc = (unsigned char)(fTemp + 0.5);
			}
		}
	}

	// ����
	return TRUE;


}
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2)
{

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ѭ������
	LONG	i;
	LONG	j;

	// �Ҷ�ӳ���
	BYTE	bMap[256];

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ����Ҷ�ӳ���
	for (i = 0; i <= bX1; i++)
	{
		// �ж�bX1�Ƿ����0����ֹ��ĸΪ0��
		if (bX1 > 0)
		{
			// ���Ա任
			bMap[i] = (BYTE)bY1 * i / bX1;
		}
		else
		{
			// ֱ�Ӹ�ֵΪ0
			bMap[i] = 0;
		}
	}
	for (; i <= bX2; i++)
	{
		// �ж�bX1�Ƿ����bX2����ֹ��ĸΪ0��
		if (bX2 != bX1)
		{
			// ���Ա任
			bMap[i] = bY1 + (BYTE)((bY2 - bY1) * (i - bX1) / (bX2 - bX1));
		}
		else
		{
			// ֱ�Ӹ�ֵΪbY1
			bMap[i] = bY1;
		}
	}
	for (; i < 256; i++)
	{
		// �ж�bX2�Ƿ����255����ֹ��ĸΪ0��
		if (bX2 != 255)
		{
			// ���Ա任
			bMap[i] = bY2 + (BYTE)((255 - bY2) * (i - bX2) / (255 - bX2));
		}
		else
		{
			// ֱ�Ӹ�ֵΪ255
			bMap[i] = 255;
		}
	}

	// ÿ��
	for (i = 0; i < lHeight; i++)
	{
		// ÿ��
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// �����µĻҶ�ֵ
			*lpSrc = bMap[*lpSrc];
		}
	}

	// ����
	return TRUE;
}
BOOL WINAPI BitPlaneSlicing(LPSTR lpDIBBits, LONG lmageWidth, LONG  lLineBytes, LONG lmageHeight, int Bit)
{
	int i;  //ѭ������
	int j;	//ѭ������
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	//����ֵ
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

			// ָ��Դͼ������j�У���i�����ص�ָ��			
			//lpSrc = (unsigned char *)lpDIBBits + lmageWidth * j + i;
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * j + i;
			//ȡ�õ�ǰָ�봦������ֵ
			pixeltemp = (unsigned char)*lpSrc;
			//Ŀ��ͼ���лҶ�ֵС������������Ϊ�ڵ�
			if (pixeltemp >= threshold1 && pixeltemp < threshold2)
			{
				//*lpSrc = (unsigned char)255;
			}
			else	//��������Ϊ�׵�
			{
				*lpSrc = (unsigned char)0;
			}
		}
	}
	return true;
}
BOOL WINAPI LogTranslation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lLineBytes)
{
	long i;                 //��ѭ������
	long j;                 //��ѭ������
	unsigned char* lpSrcUnChr;	//ָ�����ص�ָ��
	for (i = 0; i < lHeight; i++)// ��
	{
		for (j = 0; j < lWidth; j++)// ��
		{
			lpSrcUnChr = (unsigned char*)lpSrcStartBits + lLineBytes * (lHeight - 1 - i) + j;// ָ��DIB��i�У���j�����ص�ָ��
			//*lpSrcUnChr=50*(unsigned char)log((*lpSrcUnChr+1));
			*lpSrcUnChr = 255 * (unsigned char)log((*lpSrcUnChr + 1)) / log(255);
		}
	}
	return TRUE;
}
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,int iTempH, int iTempW,int iTempMX, int iTempMY,FLOAT* fpArray, FLOAT fCoef)
{
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// ����
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					// ��������ֵ
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}

			// ����ϵ��
			fResult *= fCoef;

			// ȡ����ֵ
			fResult = (FLOAT)fabs(fResult);

			// �ж��Ƿ񳬹�255
			if (fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpDst = 255;
			}
			else
			{
				// ��ֵ
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;

}
unsigned char WINAPI GetMedianNum(unsigned char* bArray, int iFilterLen)
{
	// ѭ������
	int		i;
	int		j;

	// �м����
	unsigned char bTemp;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j++)
	{
		for (i = 0; i < iFilterLen - j - 1; i++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}

	// ������ֵ
	return bTemp;
}
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,
	int iFilterH, int iFilterW,
	int iFilterMX, int iFilterMY)
{

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ָ����ͼ���ָ��
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;

	// ָ���˲��������ָ��
	unsigned char* aValue;
	HLOCAL			hArray;

	// ѭ������
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;

	// ͼ��ÿ�е��ֽ���
	LONG			lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��ʱ�����ڴ棬�Ա����˲�������
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);

	// �ж��Ƿ��ڴ����ʧ��
	if (hArray == NULL)
	{
		// �ͷ��ڴ�
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);

		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	aValue = (unsigned char*)LocalLock(hArray);

	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for (i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// ��ȡ�˲�������
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;

					// ��������ֵ
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}

			// ��ȡ��ֵ
			*lpDst = GetMedianNum(aValue, iFilterH * iFilterW);
		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);

	// ����
	return TRUE;
}
BOOL WINAPI SaltNoiseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	//ѭ������
	long i;
	long j;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	//����α�������
	srand((unsigned)time(NULL));

	//��ͼ���м���
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lLineBytes; i++)
		{
			if (rand() > 31500)
			{
				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lLineBytes * j + i;

				//ͼ���е�ǰ����Ϊ��
				*lpSrc = 0;
			}
		}
	}
	// ����
	return true;
}
BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;

	//����ֵ
	double result;
	unsigned char pixel[4];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);


	//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
	for (j = lHeight - 1; j > 0; j--)
	{
		for (i = 0; i < lWidth - 1; i++)
		{
			//����ʹ��2��2��ģ�壬Ϊ��ֹԽ�磬���Բ��������±ߺ����ұߵ���������

			// ָ��Դͼ���j�У���i�����ص�ָ��			
			lpSrc = (char*)lpDIBBits + lWidth * j + i;

			// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
			lpDst = (char*)lpNewDIBBits + lWidth * j + i;

			//ȡ�õ�ǰָ�봦2*2���������ֵ��ע��Ҫת��Ϊunsigned char��
			pixel[0] = (unsigned char)*lpSrc;
			pixel[1] = (unsigned char)*(lpSrc + 1);
			pixel[2] = (unsigned char)*(lpSrc - lWidth);
			pixel[3] = (unsigned char)*(lpSrc - lWidth + 1);

			//����Ŀ��ͼ���еĵ�ǰ��
			result = sqrt((pixel[0] - pixel[3]) * (pixel[0] - pixel[3]) + \
				(pixel[1] - pixel[2]) * (pixel[1] - pixel[2]));
			*lpDst = (unsigned char)result;

		}
	}

	// �����񻯺��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//ѭ������
	long i;
	long j;

	// ģ��߶�
	int		iTempH;

	// ģ����
	int		iTempW;

	// ģ��ϵ��
	FLOAT	fTempC;

	// ģ������Ԫ��X����
	int		iTempMX;

	// ģ������Ԫ��Y����
	int		iTempMY;

	//ģ������
	FLOAT aTemplate[9];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// ����Դͼ�񵽻���ͼ����
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Sobelģ�����
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

	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// ����Sobelģ�����
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;
			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// ����
	return TRUE;
}
BOOL WINAPI KirschDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//ѭ������
	long i;
	long j;

	// ģ��߶�
	int		iTempH;

	// ģ����
	int		iTempW;

	// ģ��ϵ��
	FLOAT	fTempC;

	// ģ������Ԫ��X����
	int		iTempMX;

	// ģ������Ԫ��Y����
	int		iTempMY;

	//ģ������
	FLOAT aTemplate[9];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// ����Դͼ�񵽻���ͼ����
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// ����Kirschģ��1����
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

	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// ����Kirschģ��2����
	aTemplate[0] = -3.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��3����
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = 5.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = 5.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��4����
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 5.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��5����
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = -3.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = 5.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);
	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��6����
	aTemplate[0] = -3.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = 5.0;
	aTemplate[8] = -3.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��7����
	aTemplate[0] = 5.0;
	aTemplate[1] = -3.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = 5.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ����Դͼ�񵽻���ͼ����
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);

	// ����Kirschģ��8����
	aTemplate[0] = 5.0;
	aTemplate[1] = 5.0;
	aTemplate[2] = -3.0;
	aTemplate[3] = 5.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -3.0;
	aTemplate[6] = -3.0;
	aTemplate[7] = -3.0;
	aTemplate[8] = -3.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// ����
	return TRUE;
}
BOOL WINAPI GaussDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	// ģ��߶�
	int		iTempH;

	// ģ����
	int		iTempW;

	// ģ��ϵ��
	FLOAT	fTempC;

	// ģ������Ԫ��X����
	int		iTempMX;

	// ģ������Ԫ��Y����
	int		iTempMY;

	//ģ������
	FLOAT aTemplate[25];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// ����Դͼ�񵽻���ͼ����
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// ����Gaussģ�����
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

	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// ����
	return TRUE;
}
BOOL WINAPI PrewittDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst1;
	LPSTR	lpDst2;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits1;
	HLOCAL	hNewDIBBits1;
	LPSTR	lpNewDIBBits2;
	HLOCAL	hNewDIBBits2;

	//ѭ������
	long i;
	long j;

	// ģ��߶�
	int		iTempH;

	// ģ����
	int		iTempW;

	// ģ��ϵ��
	FLOAT	fTempC;

	// ģ������Ԫ��X����
	int		iTempMX;

	// ģ������Ԫ��Y����
	int		iTempMY;

	//ģ������
	FLOAT aTemplate[9];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits1 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits1 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits1 = (char*)LocalLock(hNewDIBBits1);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits2 = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits2 == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits2 = (char*)LocalLock(hNewDIBBits2);

	// ����Դͼ�񵽻���ͼ����
	lpDst1 = (char*)lpNewDIBBits1;
	memcpy(lpNewDIBBits1, lpDIBBits, lWidth * lHeight);
	lpDst2 = (char*)lpNewDIBBits2;
	memcpy(lpNewDIBBits2, lpDIBBits, lWidth * lHeight);


	// ����Prewittģ��1����
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

	// ����Template()����
	if (!Template(lpNewDIBBits1, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	// ����Prewittģ��2����
	aTemplate[0] = 1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 1.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = -1.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = -1.0;

	// ����Template()����
	if (!Template(lpNewDIBBits2, lWidth, lHeight,
		iTempH, iTempW, iTempMX, iTempMY, aTemplate, fTempC))
	{
		return FALSE;
	}

	//����������ͼ������ֵ
	for (j = 0; j < lHeight; j++)
	{
		for (i = 0; i < lWidth - 1; i++)
		{

			// ָ�򻺴�ͼ��1������j�У���i�����ص�ָ��			
			lpDst1 = (char*)lpNewDIBBits1 + lWidth * j + i;

			// ָ�򻺴�ͼ��2������j�У���i�����ص�ָ��			
			lpDst2 = (char*)lpNewDIBBits2 + lWidth * j + i;

			if (*lpDst2 > *lpDst1)
				*lpDst1 = *lpDst2;

		}
	}

	// ���ƾ���ģ��������ͼ��Դͼ��
	memcpy(lpDIBBits, lpNewDIBBits1, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits1);
	LocalFree(hNewDIBBits1);

	LocalUnlock(hNewDIBBits2);
	LocalFree(hNewDIBBits2);
	// ����
	return TRUE;
}
//canny���ӣ�
void MakeGauss(double sigma, double** pdKernel, int* pnWindowSize)
{
	// ѭ�����Ʊ���
	int i;

	// ��������ĵ�
	int nCenter;

	// �����ĳһ�㵽���ĵ�ľ���
	double  dDis;

	//double PI = 3.14159;
	// �м����
	double  dValue;
	double  dSum;
	dSum = 0;

	// ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
	// ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);

	// ����
	nCenter = (*pnWindowSize) / 2;

	// �����ڴ�
	*pdKernel = new double[*pnWindowSize];

	for (i = 0; i < (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1 / 2) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * PI) * sigma);
		(*pdKernel)[i] = dValue;
		dSum += dValue;
	}

	// ��һ��
	for (i = 0; i < (*pnWindowSize); i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}
void GaussianSmooth(unsigned char* pUnchImg, int nWidth, int nHeight,
	double sigma, unsigned char* pUnchSmthdImg)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int i;

	// ��˹�˲��������鳤��

	int nWindowSize;

	//  ���ڳ��ȵ�1/2
	int	nHalfLen;

	// һά��˹�����˲���
	double* pdKernel;

	// ��˹ϵ����ͼ�����ݵĵ��
	double  dDotMul;

	// ��˹�˲�ϵ�����ܺ�
	double  dWeightSum;

	// �м����
	double* pdTmp;

	// �����ڴ�
	pdTmp = new double[nWidth * nHeight];

	// ����һά��˹�����˲���
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize);

	// MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
	nHalfLen = nWindowSize / 2;

	// x��������˲�
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			dDotMul = 0;
			dWeightSum = 0;
			for (i = (-nHalfLen); i <= nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if ((i + x) >= 0 && (i + x) < nWidth)
				{
					dDotMul += (double)pUnchImg[y * nWidth + (i + x)] * pdKernel[nHalfLen + i];
					dWeightSum += pdKernel[nHalfLen + i];
				}
			}
			pdTmp[y * nWidth + x] = dDotMul / dWeightSum;
		}
	}

	// y��������˲�
	for (x = 0; x < nWidth; x++)
	{
		for (y = 0; y < nHeight; y++)
		{
			dDotMul = 0;
			dWeightSum = 0;
			for (i = (-nHalfLen); i <= nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if ((i + y) >= 0 && (i + y) < nHeight)
				{
					dDotMul += (double)pdTmp[(y + i) * nWidth + x] * pdKernel[nHalfLen + i];
					dWeightSum += pdKernel[nHalfLen + i];
				}
			}
			pUnchSmthdImg[y * nWidth + x] = (unsigned char)(int)dDotMul / dWeightSum;
		}
	}

	// �ͷ��ڴ�
	delete[]pdKernel;
	pdKernel = NULL;

	delete[]pdTmp;
	pdTmp = NULL;
}

void DirGrad(unsigned char* pUnchSmthdImg, int nWidth, int nHeight,
	int* pnGradX, int* pnGradY)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			pnGradX[y * nWidth + x] = (int)(pUnchSmthdImg[y * nWidth + min(nWidth - 1, x + 1)]
				- pUnchSmthdImg[y * nWidth + max(0, x - 1)]);
		}
	}

	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
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

	// ѭ�����Ʊ���
	int y;
	int x;

	// �м����
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int nPos;

	// x�����ݶȷ���
	int gx;
	int gy;

	// ��ʱ����
	int g1, g2, g3, g4;
	double weight;
	double dTmp1;
	double dTmp2;
	double dTmp;

	// ����ͼ���Ե����Ϊ�����ܵı߽��
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

			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if (pnMag[nPos] == 0)
			{
				pUnchRst[nPos] = 0;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pnMag[nPos];

				// x��y������
				gx = pnGradX[nPos];
				gy = pnGradY[nPos];

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx))
				{
					// �����ֵ�ı���
					weight = fabs(gx) / fabs(gy);

					g2 = pnMag[nPos - nWidth];
					g4 = pnMag[nPos + nWidth];

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx * gy > 0)
					{
						g1 = pnMag[nPos - nWidth - 1];
						g3 = pnMag[nPos + nWidth + 1];
					}

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else
					{
						g1 = pnMag[nPos - nWidth + 1];
						g3 = pnMag[nPos + nWidth - 1];
					}
				}

				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs(gy) / fabs(gx);

					g2 = pnMag[nPos + 1];
					g4 = pnMag[nPos - 1];

					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx * gy > 0)
					{
						g1 = pnMag[nPos + nWidth + 1];
						g3 = pnMag[nPos - nWidth - 1];
					}
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else
					{
						g1 = pnMag[nPos - nWidth + 1];
						g3 = pnMag[nPos + nWidth - 1];
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight * g1 + (1 - weight) * g2;
					dTmp2 = weight * g3 + (1 - weight) * g4;

					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if (dTmp >= dTmp1 && dTmp >= dTmp2)
					{
						pUnchRst[nPos] = 128;
					}
					else
					{
						// �������Ǳ߽��
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
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;

	// ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
	int nHist[1024];

	// ���ܵı߽���Ŀ
	int nEdgeNb;

	// ����ݶ�ֵ
	int nMaxMag;

	int nHighCount;

	nMaxMag = 0;

	// ��ʼ��
	for (k = 0; k < 1024; k++)
	{
		nHist[k] = 0;
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if (pUnchEdge[y * nWidth + x] == 128)
			{
				nHist[pnMag[y * nWidth + x]]++;
			}
		}
	}

	nEdgeNb = nHist[0];
	nMaxMag = 0;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for (k = 1; k < 1024; k++)
	{
		if (nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}

		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb + 0.5);

	k = 1;
	nEdgeNb = nHist[1];

	// �������ֵ
	while ((k < (nMaxMag - 1)) && (nEdgeNb < nHighCount))
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pnThdHigh = k;

	// ���õ���ֵ
	*pnThdLow = (int)((*pnThdHigh) * dRationLow + 0.5);
}

void TraceEdge(int y, int x, int nLowThd, unsigned char* pUnchEdge, int* pnMag, int nWidth)
{
	// ��8�������ؽ��в�ѯ
	int xNb[8] = { 1, 1, 0,-1,-1,-1, 0, 1 };
	int yNb[8] = { 0, 1, 1, 1,0 ,-1,-1,-1 };

	int yy;
	int xx;

	int k;

	for (k = 0; k < 8; k++)
	{
		yy = y + yNb[k];
		xx = x + xNb[k];
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if (pUnchEdge[yy * nWidth + xx] == 128 && pnMag[yy * nWidth + xx] >= nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pUnchEdge[yy * nWidth + xx] = 255;

			// �Ըõ�Ϊ���Ľ��и���
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
}

void Hysteresis(int* pnMag, int nWidth, int nHeight, double dRatioLow,
	double dRatioHigh, unsigned char* pUnchEdge)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int nThdHigh;
	int nThdLow;

	int nPos;

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh,
		&nThdLow, pUnchEdge, dRatioHigh, dRatioLow);

	// ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	  // TraceEdge���������ٸõ��Ӧ�ı߽�
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			nPos = y * nWidth + x;

			// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
			// һ���߽�����
			if ((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
			{
				// ���øõ�Ϊ�߽��
				pUnchEdge[nPos] = 255;
				TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
			}
		}
	}

	// ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
	for (y = 0; y < nHeight; y++)
	{
		for (x = 0; x < nWidth; x++)
		{
			nPos = y * nWidth + x;
			if (pUnchEdge[nPos] != 255)
			{
				// ����Ϊ�Ǳ߽��
				pUnchEdge[nPos] = 0;
			}
		}
	}
}
void Canny(unsigned char* pUnchImage, int nWidth, int nHeight, double sigma,
	double dRatioLow, double dRatioHigh, unsigned char* pUnchEdge)
{
	// ������˹�˲����ͼ������
	unsigned char* pUnchSmooth;

	// ָ��x��������ָ��
	int* pnGradX;

	// ָ��y��������ָ��
	int* pnGradY;

	// �ݶȵķ���
	int* pnGradMag;

	pUnchSmooth = new unsigned char[nWidth * nHeight];
	pnGradX = new int[nWidth * nHeight];
	pnGradY = new int[nWidth * nHeight];
	pnGradMag = new int[nWidth * nHeight];

	// ��ԭͼ������˲�
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth);

	// ���㷽����
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY);

	// �����ݶȵķ���
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag);

	// Ӧ��non-maximum ����
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge);

	// Ӧ��Hysteresis���ҵ����еı߽�
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// �ͷ��ڴ�
	delete pnGradX;
	pnGradX = NULL;
	delete pnGradY;
	pnGradY = NULL;
	delete pnGradMag;
	pnGradMag = NULL;
	delete pUnchSmooth;
	pUnchSmooth = NULL;
}
//�Ҷ�ͼ��ֵ�����㣺
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
		//����ɨ��
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			// �任
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

//��̬ѧ���㣺
//��ʴ���㣺
BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],BOOL option)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if (option == TRUE)
	{
		if (nMode == 0)
		{
			//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;
					/*	if(type==white)*/
						//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)0;

					//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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
			//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)0;

					//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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
			//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
					//�����ϱߺ����±ߵ���������
					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)0;

					//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
					//ע����DIBͼ�������������µ��õ�
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
			//ʹ��ˮƽ����ĽṹԪ�ؽ��и�ʴ
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)255;

					//���Դͼ���е�ǰ���������������һ���㲻�ǰ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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
			//ʹ�ô�ֱ����ĽṹԪ�ؽ��и�ʴ
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)255;

					//���Դͼ���е�ǰ���������������һ���㲻�Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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
			//ʹ���Զ���ĽṹԪ�ؽ��и�ʴ
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
					//�����ϱߺ����±ߵ���������
					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;


					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)255;

					//���ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ������һ�����Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
					//ע����DIBͼ�������������µ��õ�
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
	

	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
//�������㣺
BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3], BOOL option)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//����ֵ
	unsigned char pixel;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	if (option == TRUE)
	{
		if (nMode == 0)
		{
			//ʹ��ˮƽ����ĽṹԪ�ؽ�������
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && pixel != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)255;

					//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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
			//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)255;

					//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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
			//ʹ���Զ���ĽṹԪ�ؽ�������
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
					//�����ϱߺ����±ߵ���������
					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)255;

					//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
					//ע����DIBͼ�������������µ��õ�
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
			//ʹ��ˮƽ����ĽṹԪ�ؽ�������
			for (j = 0; j < lHeight; j++)
			{
				for (i = 1; i < lWidth - 1; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && pixel != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɺ�ɫ
					*lpDst = (unsigned char)0;

					//Դͼ���е�ǰ�������������ֻҪ��һ�����ǰ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɰ�ɫ
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
			//ʹ�ô�ֱ����ĽṹԪ�ؽ�������
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��1��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ��������ϱߺ����±ߵ���������

					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)0;

					//Դͼ���е�ǰ�������������ֻҪ��һ�����Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
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
			//ʹ���Զ���ĽṹԪ�ؽ�������
			for (j = 1; j < lHeight - 1; j++)
			{
				for (i = 0; i < lWidth; i++)
				{
					//����ʹ��3��3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ���������ߺ����ұߵ���������
					//�����ϱߺ����±ߵ���������
					// ָ��Դͼ������j�У���i�����ص�ָ��			
					lpSrc = (char*)lpDIBBits + lWidth * j + i;

					// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
					lpDst = (char*)lpNewDIBBits + lWidth * j + i;

					//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
					pixel = (unsigned char)*lpSrc;

					//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
					if (pixel != 255 && *lpSrc != 0)
						return FALSE;

					//Ŀ��ͼ���еĵ�ǰ���ȸ��ɰ�ɫ
					*lpDst = (unsigned char)0;

					//ԭͼ���ж�Ӧ�ṹԪ����Ϊ��ɫ����Щ����ֻҪ��һ���Ǻ�ɫ��
					//��Ŀ��ͼ���еĵ�ǰ�㸳�ɺ�ɫ
					//ע����DIBͼ�������������µ��õ�
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
	
	// �������ͺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int nMode, int structure[3][3],BOOL option)
{
	//// ͼ��ÿ�е��ֽ���
	//LONG lLineBytes;
	//// ����ͼ��ÿ�е��ֽ���
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
			// ����
			return TRUE;

		}
	}
	return FALSE;

	// ����
	return TRUE;
}
//�Ҷȴ��ڱ任��
BOOL WINAPI WindowTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bLow, BYTE bUp)
{
	unsigned char* lpSrc;//ָԭͼ��ָ��

	LONG i, j;//ѭ������

	LONG	lLineBytes;//ÿ���ֽ���

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//�������޵ĸ�0���������޵ĸ�255
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

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;

	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	//����
	BOOL bModified;

	//ѭ������
	long i;
	long j;
	int  n;
	int  m;

	//�ĸ�����
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//������
	unsigned char nCount;

	//����ֵ
	unsigned char pixel;

	//5��5������������ֵ
	unsigned char neighbour[5][5];

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);

	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char*)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);

	bModified = TRUE;

	while (bModified)
	{

		bModified = FALSE;
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
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

				//����ʹ��5��5�ĽṹԪ�أ�Ϊ��ֹԽ�磬���Բ�������Χ�ļ��кͼ�������

				// ָ��Դͼ������j�У���i�����ص�ָ��			
				lpSrc = (char*)lpDIBBits + lWidth * j + i;

				// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
				lpDst = (char*)lpNewDIBBits + lWidth * j + i;

				//ȡ�õ�ǰָ�봦������ֵ��ע��Ҫת��Ϊunsigned char��
				pixel = (unsigned char)*lpSrc;

				//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
				if (pixel != 255 && *lpSrc != 0)
					//return FALSE;
					continue;
				//���Դͼ���е�ǰ��Ϊ��ɫ��������
				else if (pixel == 255)
					continue;

				//��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0; m < 5; m++)
				{
					for (n = 0; n < 5; n++)
					{
						neighbour[m][n] = (255 - (unsigned char)*(lpSrc + ((4 - m) - 2) * lWidth + n - 2)) / 255;
					}
				}
				//				neighbour[][]
								//����ж�������
								//�ж�2<=NZ(P1)<=6
				nCount = neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
					+ neighbour[2][1] + neighbour[2][3] + \
					+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if (nCount >= 2 && nCount <= 6)
					bCondition1 = TRUE;

				//�ж�Z0(P1)=1
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

				//�ж�P2*P4*P8=0 or Z0(p2)!=1
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

				//�ж�P2*P4*P6=0 or Z0(p4)!=1
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
		// ���Ƹ�ʴ���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);


	}
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
//ֱ��ͼ���⻯��
BOOL WINAPI GrayEqualization(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	unsigned char* lpSrc;

	LONG i, j;//ѭ������

	// ����ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// �Ҷ�ӳ���
	BYTE	GrayMap[256];

	//�Ҷȼ�����
	LONG GrayCount[256];

	for (i = 0; i < 256; i++)
	{
		GrayCount[i] = 0;//��ʼ��Ϊ0
	}

	//ͳ�Ƹ����Ҷ�ֵ�ĸ���
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 256; j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			GrayCount[*lpSrc]++;//�����صĻҶ�ֵ����+1
		}
	}

	//�����Ҷ�ֵӳ���
	for (i = 0; i < 256; i++)
	{
		LONG temp;//�����ۼ�ֵ
		temp = 0;
		for (j = 0; j <= i; j++)
		{
			temp = temp + GrayCount[j];
		}
		//�õ��»Ҷ�ֵ
		GrayMap[i] = (BYTE)(temp * 255 / lWidth / lHeight);
	}

	//����ÿ�����ؾ��⻯��ĻҶ�ֵ
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
//�˾�ģ�壺
BOOL WINAPI MyTemplate(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef, FLOAT yuzhi)
{
	//����ͼ���ָ��
	LPSTR lpNewDIBBits;
	HLOCAL hNewDIBBits;

	//ԭͼ���ָ��
	unsigned char* lpSrc;

	//ָ���������ָ��
	unsigned char* lpDst;

	LONG i, j, k, l;

	FLOAT fResult;//������

	LONG lLineBytes;//ÿ���ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	// ��(��ȥ��Ե����)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			fResult = 0;

			// ����
			for (k = 0; k < iTempH; k++)
			{
				for (l = 0; l < iTempW; l++)
				{
					// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
						+ j - iTempMX + l;

					// ��������ֵ
					fResult += (*lpSrc) * fpArray[k * iTempW + l];
				}
			}

			// ����ϵ��
			fResult *= fCoef;

			//������ֵ
			fResult += yuzhi;

			// ȡ����ֵ
			fResult = (FLOAT)fabs(fResult);

			// �ж��Ƿ񳬹�255
			if (fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				*lpDst = 255;
			}
			else
			{
				// ��ֵ
				*lpDst = (unsigned char)(fResult + 0.5);
			}

		}
	}

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);



	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;

}
BOOL WINAPI ImageTranslation(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG XOffset, LONG YOffset)//ͼ��ƽ��
{
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ��Ҫ���������ָ��
	LPSTR	lpDst;

	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ������ƽ�ƺ�ͼ�������
	LONG	i;
	LONG	j;

	// ������ԭͼ�������
	LONG	i0;
	LONG	j0;


	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	for (i = 0; i < lHeight; i++)
	{
		for (j = 0; j < lWidth; j++)
		{
			//ƽ�ƺ����ص�
			lpDst = (char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// �����������ԭͼ���е�����
			i0 = i - XOffset;
			j0 = j - YOffset;

			// �ж��Ƿ���Դͼ��Χ��
			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��Դԭͼ���i0�У���j0�����ص�ָ��
				lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				*((unsigned char*)lpDst) = 255;
			}

		}
	}

	// ����ƽ�ƺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;

}

//ͼ����ת��
HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle)
{
	// Դͼ��Ŀ�Ⱥ͸߶�
	LONG	lWidth;
	LONG	lHeight;

	// ��ת��ͼ��Ŀ�Ⱥ͸߶�
	LONG	lNewWidth;
	LONG	lNewHeight;

	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;

	// ��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG	lNewLineBytes;

	// ָ��Դͼ���ָ��
	LPSTR	lpDIBBits;

	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;

	// ��ת����DIB���
	HDIB	hDIB;

	// ָ����תͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;

	// ָ����תͼ���ָ��
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;

	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ѭ����������������DIB�е����꣩
	LONG	i;
	LONG	j;

	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;

	// ��ת�Ƕȣ����ȣ�
	float	fRotateAngle;

	// ��ת�Ƕȵ����Һ�����
	float	fSina, fCosa;

	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1, fSrcY1, fSrcX2, fSrcY2, fSrcX3, fSrcY3, fSrcX4, fSrcY4;

	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1, fDstY1, fDstX2, fDstY2, fDstX3, fDstY3, fDstX4, fDstY4;

	// �����м䳣��
	float	f1, f2;

	// �ҵ�ԴDIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);

	// ��ȡͼ���"���"��4�ı�����
	lWidth = ::DIBWidth(lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);

	// ��ȡͼ��ĸ߶�
	lHeight = ::DIBHeight(lpDIB);

	// ����ת�ǶȴӶ�ת��������
	fRotateAngle = (float)RADIAN(iRotateAngle);

	// ������ת�Ƕȵ�����
	fSina = (float)sin((double)fRotateAngle);

	// ������ת�Ƕȵ�����
	fCosa = (float)cos((double)fRotateAngle);

	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float)(-(lWidth - 1) / 2);
	fSrcY1 = (float)((lHeight - 1) / 2);
	fSrcX2 = (float)((lWidth - 1) / 2);
	fSrcY2 = (float)((lHeight - 1) / 2);
	fSrcX3 = (float)(-(lWidth - 1) / 2);
	fSrcY3 = (float)(-(lHeight - 1) / 2);
	fSrcX4 = (float)((lWidth - 1) / 2);
	fSrcY4 = (float)(-(lHeight - 1) / 2);

	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;

	// ������ת���ͼ��ʵ�ʿ��
	lNewWidth = (LONG)(max(fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2)) + 0.5);

	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);

	// ������ת���ͼ��߶�
	lNewHeight = (LONG)(max(fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2)) + 0.5);

	// �������������������Ժ�ÿ�ζ�������
	f1 = (float)(-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth - 1));
	f2 = (float)(0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));

	// �����ڴ棬�Ա�����DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));

	// �ж��Ƿ��ڴ����ʧ��
	if (hDIB == NULL)
	{
		// �����ڴ�ʧ��
		return NULL;
	}

	// �����ڴ�
	lpNewDIB = (char*)::GlobalLock((HGLOBAL)hDIB);

	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));

	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;

	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short)lNewWidth;
		lpbmc->bcHeight = (unsigned short)lNewHeight;
	}

	// ���ͼ��ÿ�н��в���
	for (i = 0; i < lNewHeight; i++)
	{
		// ���ͼ��ÿ�н��в���
		for (j = 0; j < lNewWidth; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char*)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;

			// �����������ԴDIB�е�����
			i0 = (LONG)(-((float)j) * fSina + ((float)i) * fCosa + f2 + 0.5);
			j0 = (LONG)(((float)j) * fCosa + ((float)i) * fSina + f1 + 0.5);

			// �ж��Ƿ���Դͼ��Χ��
			if ((j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;

				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				*((unsigned char*)lpDst) = 255;
			}

		}

	}

	// ����
	return hDIB;
}
//ͼ��ת�ã�
BOOL WINAPI TransposeDIB(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{


	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;

	// ָ��ת��ͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;

	// ָ��ת��ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;

	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;

	// ѭ������
	LONG	i;
	LONG	j;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ��ͼ��ÿ�е��ֽ���
	LONG lNewLineBytes;

	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpDIB;


	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = (lHeight * 8 + 31) / 32 * 4;

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lNewLineBytes);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ���ͼ��ÿ�н��в���
	for (i = 0; i < lHeight; i++)
	{
		// ���ÿ��ͼ��ÿ�н��в���
		for (j = 0; j < lWidth; j++)
		{

			// ָ��ԴDIB��i�У���j�����ص�ָ��
			lpSrc = (char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			// ָ��ת��DIB��j�У���i�����ص�ָ��
			// ע��˴�lWidth��lHeight��ԴDIB�Ŀ�Ⱥ͸߶ȣ�Ӧ�û���
			lpDst = (char*)lpNewDIBBits + lNewLineBytes * (lWidth - 1 - j) + i;

			// ��������
			*lpDst = *lpSrc;

		}

	}

	// ����ת�ú��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lNewLineBytes);

	// ����DIB��ͼ��ĸ߿�

		// ����Windows 3.0 DIB
	lpbmi->biWidth = lHeight;

	lpbmi->biHeight = lWidth;


	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}
//ͼ����
BOOL WINAPI MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection)
{

	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	// ָ��Ҫ���������ָ��
	LPSTR	lpDst;

	// ָ����ͼ���ָ��
	LPSTR	lpBits;
	HLOCAL	hBits;

	// ѭ������
	LONG	i;
	LONG	j;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = (lWidth * 8 + 31) / 32 * 4;

	// ��ʱ�����ڴ棬�Ա���һ��ͼ��
	hBits = LocalAlloc(LHND, lLineBytes);
	if (hBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpBits = (char*)LocalLock(hBits);

	// �жϾ���ʽ
	if (bDirection)
	{
		// ˮƽ����

		// ���ͼ��ÿ�н��в���
		for (i = 0; i < lHeight; i++)
		{
			// ���ÿ��ͼ����벿�ֽ��в���
			for (j = 0; j < lWidth / 2; j++)
			{

				// ָ������i�У���j�����ص�ָ��
				lpSrc = (char*)lpDIBBits + lLineBytes * i + j;

				// ָ������i�У�������j�����ص�ָ��
				lpDst = (char*)lpDIBBits + lLineBytes * (i + 1) - j;

				// ����һ������
				*lpBits = *lpDst;

				// ��������i�У���j�����ظ��Ƶ�������i�У�������j������
				*lpDst = *lpSrc;

				// ��������i�У�������j�����ظ��Ƶ�������i�У���j������
				*lpSrc = *lpBits;
			}

		}
	}
	else
	{
		// ��ֱ����

		// ����ϰ�ͼ����в���
		for (i = 0; i < lHeight / 2; i++)
		{

			// ָ������i����������ָ��
			lpSrc = (char*)lpDIBBits + lLineBytes * i;

			// ָ���i����������ָ��
			lpDst = (char*)lpDIBBits + lLineBytes * (lHeight - i - 1);

			// ����һ�У����ΪlWidth
			memcpy(lpBits, lpDst, lLineBytes);

			// ��������i�����ظ��Ƶ���i��
			memcpy(lpDst, lpSrc, lLineBytes);

			// ����i�����ظ��Ƶ�������i��
			memcpy(lpSrc, lpBits, lLineBytes);

		}
	}

	// �ͷ��ڴ�
	LocalUnlock(hBits);
	LocalFree(hBits);

	// ����
	return TRUE;
}
//�ͻ����
BOOL WINAPI Oil_Painting(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight)//�ͻ�Ч��
{
	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;
	unsigned char* lpSrctemp;
	// ѭ������
	LONG	i;
	LONG	j;
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	int temp, k;
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * BitCount);
	int page = 1;
	if (BitCount == 24) page = 3;
	if (BitCount == 8) page = 1;
	int iModel;
	int mmin;
	if (lWidth < lHeight)//ѡ�񳤿�С��
	{
		mmin = lWidth;
	}
	else
	{
		mmin = lHeight;
	}

	if (mmin < 500)//����ͼ���Сѡ��iModel
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
	//����ɨ��
	for (i = 0; i <= lHeight - iModel; i++)
	{

		//����ɨ��
		for (j = 0; j <= lWidth - iModel; j++)
		{


			//ָ��DIB��i�У���j�����ص�ָ��
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
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ָ��Դͼ���ָ��
	unsigned char* lpSrc;

	// ָ��Ҫ���������ָ��
	unsigned char* lpDst;

	// ѭ������
	LONG	i;
	LONG	j;
	LONG	k;
	LONG	l;
	LONG    m;

	// ������
	FLOAT	fResult;

	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;

	// ����ͼ��ÿ�е��ֽ���
	//lLineBytes = WIDTHBYTES(lWidth * 8);
	lLineBytes = WIDTHBYTES(lWidth * BitCount);

	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);

	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}

	// �����ڴ�
	lpNewDIBBits = (char*)LocalLock(hNewDIBBits);

	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);

	int page = 1;
	if (BitCount == 24) page = 3;
	if (BitCount == 8) page = 1;

	// ��(��ȥ��Ե����)
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			for (m = 0; m < page; m++) {
				// ָ����DIB��i�У���j�����ص�ָ��
				lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + m;

				fResult = 0;

				// ����
				for (k = 0; k < iTempH; k++)
				{
					for (l = 0; l < iTempW; l++)
					{
						// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
						lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
							+ j - iTempMX + l;

						// ��������ֵ
						fResult += (*lpSrc) * fpArray[k * iTempW + l];
					}
				}

				// ����ϵ��
				fResult *= fCoef;

				// ȡ����ֵ
				fResult = (FLOAT)fabs(fResult);

				// �ж��Ƿ񳬹�255
				if (fResult > 255)
				{
					// ֱ�Ӹ�ֵΪ255
					*lpDst = 255;
				}
				else
				{
					// ��ֵ
					*lpDst = (unsigned char)(fResult + 0.5);
				}


			}
			//// ָ����DIB��i�У���j�����ص�ָ��
			//lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;

			//fResult = 0;

			//// ����
			//for (k = 0; k < iTempH; k++)
			//{
			//	for (l = 0; l < iTempW; l++)
			//	{
			//		// ָ��DIB��i - iTempMY + k�У���j - iTempMX + l�����ص�ָ��
			//		lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)
			//			+ j - iTempMX + l;

			//		// ��������ֵ
			//		fResult += (*lpSrc) * fpArray[k * iTempW + l];
			//	}
			//}

			//// ����ϵ��
			//fResult *= fCoef;

			//// ȡ����ֵ
			//fResult = (FLOAT)fabs(fResult);

			//// �ж��Ƿ񳬹�255
			//if (fResult > 255)
			//{
			//	// ֱ�Ӹ�ֵΪ255
			//	*lpDst = 255;
			//}
			//else
			//{
			//	// ��ֵ
			//	*lpDst = (unsigned char)(fResult + 0.5);
			//}

			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iTempMY - k)+ j - iTempMX + l;
			//*lpDst = (*lpSrc) + 50 * (*lpDst);

		}
	}

	int temp;


	//����ɨ��
	for (i = iTempMY; i < lHeight - iTempH + iTempMY + 1; i++)
	{
		//����ɨ��
		for (j = iTempMX; j < lWidth - iTempW + iTempMX + 1; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			for (k = 0; k < page; k++)
			{
				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + k;
				lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j * page + k;
				// �任
				temp = (((*lpSrc) - 0.3 * (*lpDst)) / 0.7);

				// �ж��Ƿ񳬹�255
				if (temp > 255)
				{
					// ֱ�Ӹ�ֵΪ255
					*lpDst = 255;
				}
				else {
					*lpDst = temp;
				}
			}
		}
	}







	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);

	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// ����
	return TRUE;
}

//�������
void findsorrounding8(int* t, int j, int i, int  lSrcHeight, int x, int number)//Ѱ��8��ͨ����
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

void findsorrounding4(int* t, int j, int i, int  lSrcHeight, int x, int number)//Ѱ��4��ͨ����
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

//�����������
int funcfind(LPSTR lpSrcStartBits, int* t, int  lSrcHeight, int lSrcWidth, int lpSrcBitCount, int way, int bw)//�����������
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

//���������ܳ�
double funccircu1(int* t, int  lSrcHeight, int x, int number)//���������ܳ�
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

//�����������
int funcfindbig(int* t, int  lSrcHeight, int x, int number, int& area)//�����������
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
