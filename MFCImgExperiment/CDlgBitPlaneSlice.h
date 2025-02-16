#pragma once
#include "afxdialogex.h"


// CDlgBitPlaneSlice 对话框

class CDlgBitPlaneSlice : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBitPlaneSlice)

public:
	CDlgBitPlaneSlice(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgBitPlaneSlice();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BitplaneDecomposition };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButsavebitplane();
	afx_msg void OnBnClickedButSlicing();
	int m_BitPlane;
	int flag;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	LPSTR	lpSrcDib;		//指向源图象的指针	
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	unsigned char* pBmpBuf;
	unsigned char* lpImgPlane;
};
