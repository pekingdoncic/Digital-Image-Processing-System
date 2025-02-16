#pragma once
#include "afxdialogex.h"


// CDlgVshaped 对话框

class CDlgVshaped : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgVshaped)

public:
	CDlgVshaped(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgVshaped();
	
	//
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VShaped };
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
	afx_msg void OnEnKillfocusThreshold();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
