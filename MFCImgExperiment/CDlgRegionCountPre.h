#pragma once
#include "afxdialogex.h"


// CDlgRegionCountPre 对话框

class CDlgRegionCountPre : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegionCountPre)

public:
	CDlgRegionCountPre(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgRegionCountPre();
	int m_forecolor;//前景颜色
	int m_foreground;
	int m_link;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RegionCountPre };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioblack();
	afx_msg void OnBnClickedRadiowhite();
	afx_msg void OnBnClickedRadioauto();
	afx_msg void OnBnClickedRadioeight();
	afx_msg void OnBnClickedRadiofour();
	afx_msg void OnBnClickedAddcolor();
};
