
// StructureExtractionView.h : CStructureExtractionView ��Ľӿ�
//

#pragma once

#include <vector>
#include "MoonImageProcessing.h"
#include "D:\opencv2.4\opencv\build\include\opencv2\core\core.hpp"

//�̺߳���
UINT WorkSeg(LPVOID param);

UINT WorkFitting(LPVOID param);

#define WM_FITTED  WM_USER + 100
#define WM_SEGGED  WM_USER + 101



class CStructureExtractionView : public CScrollView
{
protected: // �������л�����
	CStructureExtractionView();
	DECLARE_DYNCREATE(CStructureExtractionView)

// ����
public:
	CStructureExtractionDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CStructureExtractionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

	//����ͼ��
	IplImage* workImg;
	//�洢ͼ��
	IplImage* saveImg;
	//ͼ���б�
	std::vector<IplImage*> m_ImgList;
	//��ɫ��
	LPBITMAPINFO m_lpBmi;
	// ͼ������
	int m_ImageType;
	// DIB�ṹ�Ƿ�ı�
	int m_DibFlag;
	// ͼ��洢��־
	int m_SaveFlag;
	// ͼ������ϵ��
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
	// ��ǰͼ�����б��е�λ��
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
	// ���շ���
	double m_light;
	// ���Ͻ�x����
	double m_leftTopX;
	// ���Ͻ�y����
	double m_leftTopY;
	// x����ֱ���
	double m_resolutionX;
	// y����ֱ���
	double m_resolutionY;
	// �˲����ڳߴ�
	double m_wndSize;
	// x�����¶�
	IplImage* fx;
	// y�����¶�
	IplImage* fy;
	// �����ĵ�
	Indices m_indices;
public:
	// ���ع���ͼ��
	IplImage* getWorkImg();
	// ���ؾ����ĵ�
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
	// ��ɫ��ǩ
	cv::Mat m_bLabel;
	// ��ɫ��ǩ
	cv::Mat m_wLabel;
public:
	afx_msg void OnMatch();
	afx_msg void OnCraterFitting();
	afx_msg void OnHistogram();
	// ��������ͼ��
	MoonImageProcessing m_moonProcess;
	afx_msg void OnEdgeCanny();
	afx_msg void OnEdgeRoberts();
	afx_msg void OnMedian();
	afx_msg void OnRgb2gray();
};

#ifndef _DEBUG  // StructureExtractionView.cpp �еĵ��԰汾
inline CStructureExtractionDoc* CStructureExtractionView::GetDocument() const
   { return reinterpret_cast<CStructureExtractionDoc*>(m_pDocument); }
#endif

