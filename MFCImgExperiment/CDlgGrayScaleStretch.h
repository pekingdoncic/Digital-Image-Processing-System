#pragma once
#include "afxdialogex.h"


// CDlgGrayScaleStretch 对话框

class CDlgGrayScaleStretch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGrayScaleStretch)

public:
	CDlgGrayScaleStretch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgGrayScaleStretch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GrayScaleStretch };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	
	// 当前鼠标拖动状态，TRUE表示正在拖动。
	BOOL	m_bDrawed;

	// 保存鼠标左键单击时的位置
	CPoint	m_p1;

	// 保存鼠标拖动时的位置
	CPoint	m_p2;

	// 标识是否已经绘制橡皮筋线
	int	m_bIsDraging;// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动第一点，2表示正在拖动第二点。

	// 相应鼠标事件的矩形区域
	CRect	m_MouseRect;

	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusPoint1X();
	afx_msg void OnEnKillfocusPoint1Y();
	afx_msg void OnEnKillfocusPoint2X();
	afx_msg void OnEnKillfocusPoint2Y();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	int m_Point1_X;
	int m_Point1_Y;
	int m_Point2_X;
	int m_Point2_Y;
	afx_msg void OnBnClickedButtonpre();
	afx_msg void OnBnClickedButtonrec();
};
