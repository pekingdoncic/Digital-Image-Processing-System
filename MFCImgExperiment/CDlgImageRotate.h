#pragma once
#include "afxdialogex.h"
#include "MFCImgExperimentDoc.h"

// CDlgImageRotate 对话框

class CDlgImageRotate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImageRotate)

public:
	CDlgImageRotate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgImageRotate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImageRotation };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_RotateAngle;
	LPSTR	lpSrcDib;		//指向源图象的指针
	CMFCImgExperimentDoc* pDoc;
	//CMFCImgExperimentDoc* pDoc = GetDocument();
	void SetDib(LPSTR Did) { lpSrcDib = Did; }
	void SetDoc(CMFCImgExperimentDoc* Doc) { pDoc = Doc; }
	afx_msg void OnBnClickedButtonpre();
	afx_msg void OnBnClickedButtonpre2();
};
