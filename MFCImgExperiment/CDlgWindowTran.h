#pragma once
#include "afxdialogex.h"


// CDlgWindowTran 对话框

class CDlgWindowTran : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWindowTran)

public:
	CDlgWindowTran(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgWindowTran();
	CRect	m_MouseRect;
	// 当前鼠标拖动状态，0表示未拖动，1表示正在拖动下限，2表示正在拖动上限。
	int		m_iIsDraging;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WindowTranfor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	BYTE m_bUp;
	BYTE m_bLow;
	afx_msg void OnBnClickedButtonrec();
	afx_msg void OnBnClickedButtonpre();
	afx_msg void OnEnKillfocusLowerlimit();
	afx_msg void OnEnKillfocusUpperlimit();
};
