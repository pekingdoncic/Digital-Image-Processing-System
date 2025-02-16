#pragma once
#include "afxdialogex.h"


// CDlgFilter 对话框

class CDlgFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFilter)

public:
	CDlgFilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Filter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
