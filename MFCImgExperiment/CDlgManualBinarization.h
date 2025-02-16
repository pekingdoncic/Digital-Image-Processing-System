#pragma once
#include "afxdialogex.h"


// CDlgManualBinarization 对话框

class CDlgManualBinarization : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgManualBinarization)

public:
	CDlgManualBinarization(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgManualBinarization();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Manual_binarization };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_offset;
};
