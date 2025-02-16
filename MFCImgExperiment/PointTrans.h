#pragma once
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#define _INC_PointTransAPI
#define PI 3.14159265
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define RADIAN(angle) ((angle)*PI/180.0) 
#include "MFCImgExperimentDoc.h"
#include "stdafx.h"
#include "DIBAPI.H"
//函数原型：
int PFC(int color1, int color2);
int Sort3(int* src, int* attach, int n);
int Transfer(WORD* color24bit, LONG len, BYTE* Index, RGBQUAD* mainColor);
BOOL WINAPI Tran24ColTo8Col(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI LinerTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, FLOAT fA, FLOAT fB);
BOOL WINAPI Segmental2_Linear_Tran(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int type, int threshold);
BOOL WINAPI GrayStretch(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bX1, BYTE bY1, BYTE bX2, BYTE bY2);
BOOL WINAPI BitPlaneSlicing(LPSTR lpDIBBits, LONG lmageWidth, LONG  lLineBytes, LONG lmageHeight, int Bit);
BOOL WINAPI LogTranslation(LPSTR lpSrcStartBits, long lWidth, long lHeight, long lLineBytes);
BOOL WINAPI Template(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef);
BOOL WINAPI MedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight,int iFilterH, int iFilterW,int iFilterMX, int iFilterMY);
unsigned char WINAPI GetMedianNum(unsigned char* bArray, int iFilterLen);
BOOL WINAPI SaltNoiseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI RobertDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI SobelDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI KirschDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI GaussDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI PrewittDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
void Canny(unsigned char* pUnchImage, int nWidth, int nHeight, double sigma,
	double dRatioLow, double dRatioHigh, unsigned char* pUnchEdge);
BOOL WINAPI ManualBinarization(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int offset);
//形态学运算：
BOOL WINAPI ErosionDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori, int structure[3][3], BOOL option);
BOOL WINAPI DilationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori, int structure[3][3], BOOL option);
BOOL WINAPI OpenDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori, int structure[3][3],BOOL option);
BOOL WINAPI CloseDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bHori, int structure[3][3],  BOOL option);
BOOL WINAPI WindowTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bLow, BYTE bUp);

BOOL WINAPI ThiningDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
//直方图均衡化：
BOOL WINAPI GrayEqualization(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);

BOOL WINAPI MyTemplate(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef, FLOAT yuzhi);

BOOL WINAPI ImageTranslation(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG XOffset, LONG YOffset);//图像平移
//图像旋转：
//HGLOBAL WINAPI RotateDI0B(CMFCImgExperimentDoc* pDoc, LPSTR lpDIB, int iRotateAngle);
HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle);
//图像转置
BOOL WINAPI TransposeDIB(LPSTR lpDIB, LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
//图像镜像：
BOOL WINAPI MirrorDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BOOL bDirection);
//油画风格：
BOOL WINAPI Oil_Painting(LPSTR lpDIBBits, int BitCount, LONG lWidth, LONG lHeight);//油画效果
BOOL WINAPI USMSharp(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, int iTempH, int iTempW, int iTempMX, int iTempMY, FLOAT* fpArray, FLOAT fCoef, int BitCount);

//区域计数：
void findsorrounding8(int* t, int j, int i, int  lSrcHeight, int x, int number);
void findsorrounding4(int* t, int j, int i, int  lSrcHeight, int x, int number);
double  findciucu8(int* t, int j, int i, int  lSrcHeight, int x, int number);
int funcfind(LPSTR lpSrcStartBits, int* t, int  lSrcHeight, int lSrcWidth, int lpSrcBitCount, int way, int bw);
double funccircu1(int* t, int  lSrcHeight, int x, int number);
int funcfindbig(int* t, int  lSrcHeight, int x, int number, int& area);
int WINAPI getCoreX(LONG lWidth, LONG lHeight, LONG area);
int WINAPI getCoreY(LONG lWidth, LONG lHeight, LONG area);