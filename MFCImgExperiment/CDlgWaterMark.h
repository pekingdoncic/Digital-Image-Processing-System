#pragma once
#include "afxdialogex.h"


// CDlgWaterMark 对话框

class CDlgWaterMark : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWaterMark)

public:
	CDlgWaterMark(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgWaterMark();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WaterMark };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int num;
};
