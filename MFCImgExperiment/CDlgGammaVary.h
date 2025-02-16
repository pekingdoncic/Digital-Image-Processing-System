#pragma once
#include "afxdialogex.h"


// CDlgGammaVary 对话框

class CDlgGammaVary : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGammaVary)

public:
	CDlgGammaVary(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGammaVary();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GammaVary };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_fValGamma;
};
