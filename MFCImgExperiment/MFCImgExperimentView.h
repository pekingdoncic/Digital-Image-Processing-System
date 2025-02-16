
// MFCImgExperimentView.h: CMFCImgExperimentView 类的接口
//

#pragma once


class CMFCImgExperimentView : public CView
{
protected: // 仅从序列化创建
	CMFCImgExperimentView() noexcept;
	DECLARE_DYNCREATE(CMFCImgExperimentView)

// 特性
public:
	CMFCImgExperimentDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCImgExperimentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDispimginfo();
	afx_msg void OnChangebrightness();
//	afx_msg void OnExchangewithbackgroundimages();
	afx_msg void OnExchangeimages();
	afx_msg void On24colorto8grey();
	afx_msg void On24colorto8color();
	afx_msg void On8colorto8grey();
	afx_msg void OnHalftone();
	afx_msg void OnDualizegrey();
	afx_msg void OnHistogram();
	afx_msg void OnInversecolor();
	afx_msg void OnInvertedVshaped();
	afx_msg void OnVshaped();
	afx_msg void OnInvertedLshaped();
	afx_msg void OnLshaped();
	afx_msg void OnLinertrans();
	afx_msg void OnGrayscalestretch();
//	afx_msg void OnHisimageenhance();
	afx_msg void OnBitplanedecomposition();
	afx_msg void OnLogvary();
	afx_msg void OnGammavary();
	afx_msg void OnImageSmooth();
	afx_msg void OnMedianfiltering();
	afx_msg void OnMaxfilter();
	afx_msg void OnMinfilter();
//	afx_msg void OnMidpointfilter();
	afx_msg void OnMidpointfilter();
	afx_msg void Onadaptingfilter();
	afx_msg void OnRestoreSaltnoise();
	afx_msg void OnEnhasharp4();
	afx_msg void OnEnhasharp8();
	afx_msg void OnEdgeRobert();
	afx_msg void OnEdgeSobel();
	afx_msg void OnEdgeKirsch();
	afx_msg void OnEdgeGauss();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeCanny();
	afx_msg void OnManualdualizegrey();
	afx_msg void OnMorphology();
	afx_msg void OnThresholdtransfor();
	afx_msg void OnWindowtransfor();
	afx_msg void OnHistogramequalization();
	afx_msg void OnFilter();
	afx_msg void OnImagetranslate();
	afx_msg void OnImagerevolve();
	afx_msg void OnTranspose();
	afx_msg void OnMirrorx();
	afx_msg void OnMirrory();
	afx_msg void OnOilpainting();
	afx_msg void OnMosaic();
	afx_msg void OnFrostglass();
	afx_msg void OnFrostrainbow();
	afx_msg void OnUsm();
	afx_msg void OnRegioncount();
//	afx_msg void OnAddwatermark();
//	afx_msg void OnExtractwatermark();
	afx_msg void OnAddmark();
	afx_msg void OnExtractmark();
//	afx_msg void OnExtractwatermark();
};

#ifndef _DEBUG  // MFCImgExperimentView.cpp 中的调试版本
inline CMFCImgExperimentDoc* CMFCImgExperimentView::GetDocument() const
   { return reinterpret_cast<CMFCImgExperimentDoc*>(m_pDocument); }
#endif

