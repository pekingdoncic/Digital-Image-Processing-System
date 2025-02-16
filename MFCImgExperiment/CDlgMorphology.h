#pragma once
#include "afxdialogex.h"


// CDlgMorphology 对话框

class CDlgMorphology : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMorphology)

public:
	CDlgMorphology(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgMorphology();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_Morphology };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_Type;//运算种类
	int m_nMode;//结构元素种类
	BOOL option;//判断前景
	CButton m_Control1;
	CButton m_Control2;
	CButton m_Control3;
	CButton m_Control4;
	CButton m_Control5;
	CButton m_Control6;
	CButton m_Control7;
	CButton m_Control8;
	CButton m_Control9;
	int m_nStructure1;
	int m_nStructure3;
	int m_nStructure2;
	int m_nStructure4;
	int m_nStructure5;
	int m_nStructure6;
	int m_nStructure7;
	int m_nStructure8;
	int m_nStructure9;
	afx_msg void OnBnClickedCorrosion();
	afx_msg void OnBnClickedExpansion();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedInedge();
	afx_msg void OnBnClickedOutedge();
	afx_msg void OnBnClickedHori();
	void UpdateEdit();//更新文本框状态
	afx_msg void OnBnClickedVert();
	afx_msg void OnBnClickedcustom();
	afx_msg void OnBnClickedStructure1();
	afx_msg void OnBnClickedStructure2();
	afx_msg void OnBnClickedStructure3();
	afx_msg void OnBnClickedStructure4();
	afx_msg void OnBnClickedStructure5();
	afx_msg void OnBnClickedStructure6();
	afx_msg void OnBnClickedStructure7();
	afx_msg void OnBnClickedStructure8();
	afx_msg void OnBnClickedStructure9();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedProspectblack();
	afx_msg void OnBnClickedProspectwhite();
};
