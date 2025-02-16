#pragma once
#include "afxdialogex.h"
#include "MFCImgExperimentDoc.h"

// CDlgThresholdTrans 对话框

class CDlgThresholdTrans : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThresholdTrans)

public:
	CDlgThresholdTrans(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgThresholdTrans();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ThresholdTransfor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 当前鼠标拖动状态，TRUE表示正在拖动
	BYTE m_Threshold;
	unsigned char* pBmpBuf;
	unsigned char* lpdstImg;
	long	lSrcLineBytes;		//图象每行的字节数
	long	lSrcWidth;      //图象的宽度和高度
	long	lSrcHeight;
	int     lpSrcBitCount;       //图像的位深
	//LPSTR	lpSrcDib;		//指向源图象的指针	
	//LPSTR	lpSrcStartBits;	//指向源像素的指针
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	int m_nMode;
	BOOL m_Click;
	CRect m_MouseRect;
	BOOL m_bIsDraging;
	CDib* m_dib;
	LPSTR lpSrcDib;
	CMFCImgExperimentDoc* pDoc;
	void SetDIB(CDib* dib) { m_dib = dib; };
	void SetDoc(CMFCImgExperimentDoc* Doc) { pDoc = Doc; };
	void SetDib(LPSTR Did) { lpSrcDib = Did; }
	afx_msg void OnEnKillfocusThrehold();
	afx_msg void OnBnClickedIterative();
	afx_msg void OnBnClickedOtsu();
	afx_msg void OnBnClickedHistogram();
	afx_msg void OnBnClickedAdaption();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedButtonpre();
	afx_msg void OnBnClickedButtonrec();
	//afx_msg void OnBnClickedsuit();
};
