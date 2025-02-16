#pragma once
#include "afxdialogex.h"


// CDlgInverseColor 对话框

class CDlgInverseColor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInverseColor)

public:
	CDlgInverseColor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgInverseColor();
	/*int m_iLowGray, m_iUpGray;
	LONG m_lCount[256];*/


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InverseColor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
