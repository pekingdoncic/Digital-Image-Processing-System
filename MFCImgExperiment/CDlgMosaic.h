#pragma once
#include "afxdialogex.h"


// CDlgMosaic 对话框

class CDlgMosaic : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMosaic)

public:
	CDlgMosaic(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMosaic();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Mosaic };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_rect;
};
