#pragma once
#include "afxdialogex.h"


// CDlg_Filter 对话框

class CDlg_Filter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Filter)

public:
	CDlg_Filter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlg_Filter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Filter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int flag = 0;//判断是什么算子
	int flag1;//预览时所用算子
	LPSTR	lpSrcStartBits;	//指向源像素的指针
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	double num1;
	double num2;
	double num3;
	double num4;
	double num5;
	double num6;
	double num7;
	double num8;
	double num9;
	double m_Coefficient;
	double m_Thre;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	afx_msg void OnBnClickedRadio10();
	afx_msg void OnBnClickedRadio11();
	afx_msg void OnBnClickedRadio12();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
