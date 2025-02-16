#pragma once
#include "afxdialogex.h"


// CDlgInverseLshaped 对话框

class CDlgInverseLshaped : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgInverseLshaped)

public:
	CDlgInverseLshaped(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgInverseLshaped();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Inverted_LShaped };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_threshold;
	//响应鼠标的区域
	CRect m_MouseRect;
	//标识是否拖动
	BOOL m_bIsDrawing;
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEnKillfocusThreshold();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
