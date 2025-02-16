#pragma once
#include "afxdialogex.h"
#include "MFCImgExperimentDoc.h"

// CDlgImagePanning 对话框

class CDlgImagePanning : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgImagePanning)

public:
	CDlgImagePanning(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgImagePanning();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ImagePanning };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMFCImgExperimentDoc* pDoc;
	LPSTR	lpSrcDib;		//指向源图象的指针
	long m_XOffset;
	long m_YOffset;
	void SetDib(LPSTR Did) { lpSrcDib = Did; }
	afx_msg void OnBnClickedButtonpre();
	afx_msg void OnBnClickedButtonrec();
};
