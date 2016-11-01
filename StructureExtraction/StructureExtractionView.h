
// StructureExtractionView.h : CStructureExtractionView 类的接口
//

#pragma once

#include <vector>
#include "MoonImageProcessing.h"
#include "D:\opencv2.4\opencv\build\include\opencv2\core\core.hpp"

//线程函数
UINT WorkSeg(LPVOID param);

UINT WorkFitting(LPVOID param);

#define WM_FITTED  WM_USER + 100
#define WM_SEGGED  WM_USER + 101



class CStructureExtractionView : public CScrollView
{
protected: // 仅从序列化创建
	CStructureExtractionView();
	DECLARE_DYNCREATE(CStructureExtractionView)

// 特性
public:
	CStructureExtractionDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CStructureExtractionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

	//工作图像
	IplImage* workImg;
	//存储图像
	IplImage* saveImg;
	//图像列表
	std::vector<IplImage*> m_ImgList;
	//颜色表
	LPBITMAPINFO m_lpBmi;
	// 图像类型
	int m_ImageType;
	// DIB结构是否改变
	int m_DibFlag;
	// 图像存储标志
	int m_SaveFlag;
	// 图像缩放系数
	double m_Zoom;
public:
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	afx_msg void OnFileSaveAs();
	afx_msg void OnBack();
	afx_msg void OnForward();
private:
	// 当前图像在列表中的位置
	int m_index;
public:
	afx_msg void OnLightDirection();
	afx_msg void OnImageArea();
	afx_msg void OnDown();
	afx_msg void OnFilter();
	afx_msg void OnAspectCalculation();
//	afx_msg void OnEdgeDetection();

	void update(IplImage* image)
	{
		m_ImageType = imageType(image);
		imageClone(workImg, &saveImg);
		imageClone(image, &workImg);
		if (m_index == m_ImgList.size() - 1)
		{
			m_ImgList.push_back(image);
			m_index++;
		}
		else
		{
			imageClone(image, &m_ImgList[m_index + 1]);
			m_index++;
			cvReleaseImage(&image);
		}
		m_DibFlag = 1;
		Invalidate();
	}

	afx_msg void OnFalseEleminate();
	afx_msg void OnEuclideanSegment();
	afx_msg void OnRansac();
	afx_msg void OnLmeds();
	afx_msg void OnLms();
	afx_msg void OnDem();
	afx_msg void OnCorrection();
//	afx_msg void OnEdgePoints();
	afx_msg void OnParameter();
	afx_msg void OnType();
private:
	// 光照方向
	double m_light;
	// 左上角x坐标
	double m_leftTopX;
	// 坐上角y坐标
	double m_leftTopY;
	// x方向分辨率
	double m_resolutionX;
	// y方向分辨率
	double m_resolutionY;
	// 滤波窗口尺寸
	double m_wndSize;
	// x方向坡度
	IplImage* fx;
	// y方向坡度
	IplImage* fy;
	// 聚类后的点
	Indices m_indices;
public:
	// 返回工作图像
	IplImage* getWorkImg();
	// 返回聚类后的点
	Indices& getIndices();
protected:
	afx_msg LRESULT OnFitted(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSegged(WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileSave();
	afx_msg void OnEqualHist();
	afx_msg void OnImgSeg();
	afx_msg void OnEnrode();
	afx_msg void OnLabel();
private:
	// 黑色标签
	cv::Mat m_bLabel;
	// 白色标签
	cv::Mat m_wLabel;
public:
	afx_msg void OnMatch();
	afx_msg void OnCraterFitting();
	afx_msg void OnHistogram();
	// 处理月球图像
	MoonImageProcessing m_moonProcess;
	afx_msg void OnEdgeCanny();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnMedian();
	afx_msg void OnRgb2gray();
};

#ifndef _DEBUG  // StructureExtractionView.cpp 中的调试版本
inline CStructureExtractionDoc* CStructureExtractionView::GetDocument() const
   { return reinterpret_cast<CStructureExtractionDoc*>(m_pDocument); }
#endif

