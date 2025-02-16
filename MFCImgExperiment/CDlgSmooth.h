#pragma once
#include "afxdialogex.h"


// CDlgSmooth 对话框

class CDlgSmooth : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSmooth)

public:
	CDlgSmooth(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgSmooth();
	// 模板元素数组指针
	FLOAT* m_fpArray;
	// 模板类型
	int		m_intType;

	// 模板系数
//	float	m_fTempC;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Image_Smooth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnEnKillfocusEditTempw();
	afx_msg void OnEnKillfocusEditTemph();
	virtual void OnOK();
	void UpdateEdit();

	// 模板高度
	int m_iTempH;
	// 模板宽度
	int m_iTempW;
	// 模板中心元素X坐标
	int m_iTempMX;
	// 模板中心元素Y坐标
	int m_iTempMY;
	// 模板系数
	float m_fTempC;
};
