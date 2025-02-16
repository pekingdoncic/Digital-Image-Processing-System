#pragma once
#include "afxdialogex.h"


// CDlgRegionCount 对话框

class CDlgRegionCount : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegionCount)

public:
	CDlgRegionCount(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgRegionCount();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RegionCount };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Num;
	double m_Area;
	double m_Perimeter;
	double m_Circle;
	double m_Rectangle;
	int m_OffsetX;
	int m_OffsetY;
};
