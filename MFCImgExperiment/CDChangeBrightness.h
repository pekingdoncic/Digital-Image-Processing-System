#pragma once
#include "afxdialogex.h"


// CDChangeBrightness 对话框

class CDChangeBrightness : public CDialogEx
{
	DECLARE_DYNAMIC(CDChangeBrightness)

public:
	CDChangeBrightness(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDChangeBrightness();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ChangeBrightness };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Brightness;
	virtual BOOL OnInitDialog();
};
