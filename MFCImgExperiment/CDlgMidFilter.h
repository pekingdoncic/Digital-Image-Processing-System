#pragma once
#include "afxdialogex.h"


// CDlgMidFilter 对话框

class CDlgMidFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMidFilter)

public:
	CDlgMidFilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMidFilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MidianFilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iFilterType;
	int m_iFilterH;
	int m_iFilterW;
	int m_iFilterMX;
	int m_iFilterMY;
	virtual void OnOK();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
};
