
// StructureExtractionView.cpp : CStructureExtractionView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "StructureExtraction.h"
#endif

#include "StructureExtractionDoc.h"
#include "StructureExtractionView.h"

#include "MemDC.h"

#include "LightDirectionDlg.h"
#include "WndSizeDlg.h"
#include "RobertsDlg.h"
#include "ResizeDlg.h"
#include "EuclideanDlg.h"
#include "FittingDlg.h"
#include "HistogramDlg.h"
#include "MatchDlg.h"
#include "CannyDlg.h"

#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStructureExtractionView

IMPLEMENT_DYNCREATE(CStructureExtractionView, CScrollView)

BEGIN_MESSAGE_MAP(CStructureExtractionView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_ZOOM_IN, &CStructureExtractionView::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &CStructureExtractionView::OnZoomOut)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_SAVE_AS, &CStructureExtractionView::OnFileSaveAs)
	ON_COMMAND(ID_BACK, &CStructureExtractionView::OnBack)
	ON_COMMAND(ID_FORWARD, &CStructureExtractionView::OnForward)
	ON_COMMAND(ID_LIGHT_DIRECTION, &CStructureExtractionView::OnLightDirection)
	ON_COMMAND(ID_IMAGE_AREA, &CStructureExtractionView::OnImageArea)
	ON_COMMAND(ID_DOWN, &CStructureExtractionView::OnDown)
	ON_COMMAND(ID_FILTER, &CStructureExtractionView::OnFilter)
	ON_COMMAND(ID_ASPECT_CALCULATION, &CStructureExtractionView::OnAspectCalculation)
	ON_COMMAND(ID_FALSE_ELEMINATE, &CStructureExtractionView::OnFalseEleminate)
	ON_COMMAND(ID_EUCLIDEAN_SEGMENT, &CStructureExtractionView::OnEuclideanSegment)
	ON_COMMAND(ID_RANSAC, &CStructureExtractionView::OnRansac)
	ON_COMMAND(ID_LMEDS, &CStructureExtractionView::OnLmeds)
	ON_COMMAND(ID_LMS, &CStructureExtractionView::OnLms)
	ON_COMMAND(ID_DEM, &CStructureExtractionView::OnDem)
	ON_COMMAND(ID_CORRECTION, &CStructureExtractionView::OnCorrection)
	ON_COMMAND(ID_PARAMETER, &CStructureExtractionView::OnParameter)
	ON_COMMAND(ID_TYPE, &CStructureExtractionView::OnType)
	ON_MESSAGE(WM_FITTED, &CStructureExtractionView::OnFitted)
	ON_MESSAGE(WM_SEGGED, &CStructureExtractionView::OnSegged)
	ON_WM_MOUSEHWHEEL()
	ON_COMMAND(ID_FILE_SAVE, &CStructureExtractionView::OnFileSave)
	ON_COMMAND(ID_EQUAL_HIST, &CStructureExtractionView::OnEqualHist)
	ON_COMMAND(ID_IMG_SEG, &CStructureExtractionView::OnImgSeg)
	ON_COMMAND(ID_ENRODE, &CStructureExtractionView::OnEnrode)
	ON_COMMAND(ID_LABEL, &CStructureExtractionView::OnLabel)
	ON_COMMAND(ID_MATCH, &CStructureExtractionView::OnMatch)
	ON_COMMAND(ID_CRATER_FITTING, &CStructureExtractionView::OnCraterFitting)
	ON_COMMAND(ID_HISTOGRAM, &CStructureExtractionView::OnHistogram)
	ON_COMMAND(ID_EDGE_CANNY, &CStructureExtractionView::OnEdgeCanny)
	ON_COMMAND(ID_EDGE_ROBERTS, &CStructureExtractionView::OnEdgeRoberts)
	ON_COMMAND(ID_MEDIAN, &CStructureExtractionView::OnMedian)
	ON_COMMAND(ID_RGB2GRAY, &CStructureExtractionView::OnRgb2gray)
END_MESSAGE_MAP()


volatile bool bExitSeg = FALSE;
volatile bool bExitFit = FALSE;

//���̺߳���,��Ҫ���ڷ�ʱ�Ĳ���
UINT WorkSeg(LPVOID param)
{
	CStructureExtractionView * pView = (CStructureExtractionView *)param;
	CEuclideanDlg dlg(pView);
	if (IDOK == dlg.DoModal())
	{
		pView->m_moonProcess.EuclideanCluster(pView->getWorkImg(), dlg.m_minDis, dlg.m_minCluster, dlg.m_maxCluster, pView->getIndices());
	}
	else
	{
		AfxMessageBox("����������ŷʽ�������");
		return 0;
	}
	bExitSeg = TRUE;
	::PostMessage(pView->m_hWnd, WM_SEGGED, 0, 0);
	return 0;
}

UINT WorkFitting(LPVOID param)
{
	CStructureExtractionView * pView = (CStructureExtractionView *)param;
	CraterSet craterset;
	Indices indices = pView->getIndices();
	CFittingDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		pView->m_moonProcess.fittingByRansac(indices, craterset, dlg.m_threshold, dlg.m_proportion);
		CStructureExtractionDoc* pDoc = pView->GetDocument();
		pDoc->setCraters(craterset);
		bExitFit = TRUE;
		::PostMessage(pView->m_hWnd, WM_FITTED, 0, 0);
	}
	else
	{
		AfxMessageBox("����������RANSAC��ϲ���");
		return 0;
	}
	return 0;
}

UINT WorkLMSFitting(LPVOID param)
{
	CStructureExtractionView * pView = (CStructureExtractionView *)param;
	CraterSet craterset;
	Indices indices = pView->getIndices();
	CFittingDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		pView->m_moonProcess.fittingByLMS(indices, craterset, dlg.m_threshold, -1);
		CStructureExtractionDoc* pDoc = pView->GetDocument();
		pDoc->setCraters(craterset);
		bExitFit = TRUE;
		::PostMessage(pView->m_hWnd, WM_FITTED, 0, 0);
	}
	else
	{
		AfxMessageBox("����������RANSAC��ϲ���");
		return 0;
	}
	return 0;
}

// CStructureExtractionView ����/����

CStructureExtractionView::CStructureExtractionView()
: m_ImageType(0)
, m_DibFlag(0)
, m_SaveFlag(0)
, m_Zoom(1.0)
, m_index(-1)
, m_light(0)
, m_leftTopX(0)
, m_leftTopY(0)
, m_resolutionX(0)
, m_resolutionY(0)
, m_wndSize(0)
, fx(NULL)
, fy(NULL)
{
	// TODO:  �ڴ˴���ӹ������
	saveImg = NULL;
	workImg = NULL;
	fx = NULL;
	fy = NULL;
	m_lpBmi = 0;

}

CStructureExtractionView::~CStructureExtractionView()
{
	
	if (saveImg)
		cvReleaseImage(&saveImg);           //  �ͷ�λͼ
	if (workImg)
		cvReleaseImage(&workImg);

	for (size_t i = 0; i < m_ImgList.size(); i++)
	{
		cvReleaseImage(&m_ImgList[i]);
	}

	if (fx)
		cvReleaseImage(&fx);
	if (fy)
		cvReleaseImage(&fy);
	if (m_lpBmi)
		free(m_lpBmi);                      //  �ͷ�λͼ��Ϣ
}

BOOL CStructureExtractionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CStructureExtractionView ����

void CStructureExtractionView::OnDraw(CDC* pDC)
{
	CStructureExtractionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
	if (pDoc->pImg != NULL)	{	                //  �д�������ͼ��
		if (pDoc->m_Display == 0) {             //  ��δ��ʾ
			imageClone(pDoc->pImg, &saveImg);   //  ���Ƶ�����λͼ
			imageClone(saveImg, &workImg);      //  ���Ƶ�����λͼ
			m_DibFlag = 1;
			m_ImageType = imageType(workImg);
			m_SaveFlag = 1;
			pDoc->m_Display = 1;                //�ĵ�ͼ���Ѿ���ʾ

			IplImage* img = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
			imageClone(workImg, &img);
			m_ImgList.push_back(img);
			m_index++;
		}
	}
	else
		return;

	if (m_DibFlag) {                            //  DIB �ṹ�ı�
		if (m_lpBmi)
			free(m_lpBmi);
		m_ImageType = imageType(workImg);
		m_lpBmi = CtreateMapInfo(workImg, m_ImageType);
		m_DibFlag = 0;

		CSize  sizeTotal;
		sizeTotal = CSize(int(workImg->width*m_Zoom), int(workImg->height*m_Zoom));
		SetScrollSizes(MM_TEXT, sizeTotal);               //  ���ù�����
	}

	char *pBits;
	pBits = workImg->imageData;

	if (workImg) {                                        //  ˢ�´��ڻ���,˫������ͼ
		/*CMemDC dcMem(*pDC, this);
		CDC& dc = dcMem.GetDC();*/
		MemDC memDC(pDC);
		SetStretchBltMode(memDC.m_hDC, STRETCH_HALFTONE);//���ͼ������ʧ�������
		StretchDIBits(memDC.m_hDC,
			0, 0, int(workImg->width*m_Zoom), int(workImg->height*m_Zoom),
			0, 0, workImg->width, workImg->height,
			pBits, m_lpBmi, DIB_RGB_COLORS, SRCCOPY);

	}
}

void CStructureExtractionView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	

	CSize sizeTotal;
	// TODO:  �������ͼ�ĺϼƴ�С
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CStructureExtractionView ��ӡ

BOOL CStructureExtractionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CStructureExtractionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CStructureExtractionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// CStructureExtractionView ���

#ifdef _DEBUG
void CStructureExtractionView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStructureExtractionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CStructureExtractionDoc* CStructureExtractionView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStructureExtractionDoc)));
	return (CStructureExtractionDoc*)m_pDocument;
}
#endif //_DEBUG


// CStructureExtractionView ��Ϣ�������
//��ͼ�Ŵ�
void CStructureExtractionView::OnZoomIn()
{
	// TODO:  �ڴ���������������
	m_Zoom *= 2.0; // ��2����С���ʽ�������
	SetScrollSizes(MM_TEXT, CSize(workImg->width*m_Zoom, workImg->height*m_Zoom));
	Invalidate();
}


//��ͼ��С
void CStructureExtractionView::OnZoomOut()
{
	// TODO:  �ڴ���������������
	m_Zoom /= 2.0;
	/*if (m_Zoom < 1)
		m_Zoom = 1;*/
	SetScrollSizes(MM_TEXT, CSize(workImg->width*m_Zoom, workImg->height*m_Zoom));
	Invalidate();
}


BOOL CStructureExtractionView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
}


void CStructureExtractionView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO:  �ڴ����ר�ô����/����û���
	
	pDC->SetBkColor((COLORREF)(((BYTE)(255) | ((WORD)((BYTE)(255)) << 8)) | (((DWORD)(BYTE)(255)) << 16)));
	CScrollView::OnPrepareDC(pDC, pInfo);
}

//�洢��ǰͼ��
void CStructureExtractionView::OnFileSaveAs()
{
	// TODO:  �ڴ���������������
	CString csBMP = "BMP Files(*.BMP)|*.BMP|";
	CString csJPG = "JPEG Files(*.JPG)|*.JPG|";
	CString csTIF = "TIF Files(*.TIF)|*.TIF|";
	CString csPNG = "PNG Files(*.PNG)|*.PNG|";
	CString csDIB = "DIB Files(*.DIB)|*.DIB|";
	CString csPBM = "PBM Files(*.PBM)|*.PBM|";
	CString csPGM = "PGM Files(*.PGM)|*.PGM|";
	CString csPPM = "PPM Files(*.PPM)|*.PPM|";
	CString csSR = "SR  Files(*.SR) |*.SR|";
	CString csRAS = "RAS Files(*.RAS)|*.RAS||";

	CString csFilter = csBMP + csJPG + csTIF + csPNG + csDIB
		+ csPBM + csPGM + csPPM + csSR + csRAS;

	CString name[] = { "", "bmp", "jpg", "tif", "png", "dib",
		"pbm", "pgm", "ppm", "sr", "ras", "" };

	CString strFileName;
	CString strExtension;

	CFileDialog FileDlg(false, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilter, this);
	//  �ļ����̶Ի���
	if (FileDlg.DoModal() == IDOK) {           //ѡ�����ļ���
		strFileName = FileDlg.GetPathName();
		int filterIndex = FileDlg.m_ofn.nFilterIndex;
		strFileName += '.' + name[filterIndex];//����ļ���׺��
		CStructureExtractionDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		LPTSTR lpFileName = (LPTSTR)(LPCTSTR)strFileName;
		pDoc->Save(lpFileName, workImg);       //��ǰ�������
		imageClone(workImg, &saveImg);
	}
}

//����
void CStructureExtractionView::OnBack()
{
	// TODO:  �ڴ���������������
	if (m_index > 0)
	{
		imageClone(m_ImgList[m_index - 1], &workImg);
		m_index--;
		m_DibFlag = 1;
		Invalidate();
	}
}

//ǰ��
void CStructureExtractionView::OnForward()
{
	// TODO:  �ڴ���������������
	if (m_index < m_ImgList.size()-1)
	{
		imageClone(m_ImgList[m_index + 1], &workImg);
		m_index++;
		m_DibFlag = 1;
		Invalidate();
	}
}


void CStructureExtractionView::OnLightDirection()
{
	// TODO:  �ڴ���������������
	CLightDirectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_light = dlg.m_direction;
	}
	else
	{
		MessageBox("�����������Ϣ!!!");
	}
}


void CStructureExtractionView::OnImageArea()
{
	// TODO:  �ڴ���������������
	CString csFilter = "TFW Files(*.TFW)|*.TFW|";

	CString name[] = { "", "tfw", "" };

	CString strFileName;
	CString strExtension;

	CFileDialog FileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilter, this);
	//  �ļ�����Ի���
	if (FileDlg.DoModal() == IDOK) {         //  ѡ�����ļ���
		strFileName = FileDlg.GetPathName();
		//int filterIndex = FileDlg.m_ofn.nFilterIndex;
		//strFileName += '.' + name[filterIndex];//����ļ���׺��
		LPTSTR lpFileName = (LPTSTR)(LPCTSTR)strFileName;
		std::fstream infile;
		infile.open(lpFileName, std::ios::in);
		if (!infile)
		{
			MessageBox("Ӱ������ļ���ʧ��");
			return;
		}
		infile >> m_resolutionX;
		double temp;
		infile >> temp;
		infile >> temp;
		infile >> m_resolutionY;
		infile >> m_leftTopX;
		infile >> m_leftTopY;
		infile.close();
	}
}


void CStructureExtractionView::OnDown()
{
	// TODO:  �ڴ���������������
	CResizeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IplImage* image = cvCreateImage(cvSize(workImg->width / dlg.m_resize, workImg->height/dlg.m_resize), workImg->depth, workImg->nChannels);
		cvResize(workImg,image);
		update(image);
	}
	else
	{
		MessageBox("��������!!");
		return;
	}
}


void CStructureExtractionView::OnFilter()
{
	// TODO:  �ڴ���������������
	
	CWndSizeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_wndSize = dlg.m_wndSize;
		IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
		cvSmooth(workImg, image, CV_BILATERAL, 0, 0, m_wndSize, m_wndSize);
		update(image);

	}
	else
	{
		MessageBox("�����������˲����ڳߴ�");
		return;
	}
}


void CStructureExtractionView::OnAspectCalculation()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	fx = cvCreateImage(cvSize(workImg->width, workImg->height),
		IPL_DEPTH_16S, workImg->nChannels);
	//y direction
	fy = cvCreateImage(cvSize(workImg->width, workImg->height),
		IPL_DEPTH_16S, workImg->nChannels);
	CWndSizeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		cvSobel(workImg, fx, 1, 0, dlg.m_wndSize);
		cvSobel(workImg, fy, 0, 1, dlg.m_wndSize);
	}
}


void CStructureExtractionView::OnFalseEleminate()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	imageClone(workImg, &image);
	m_moonProcess.innerExcluding(image, fx, fy, m_light);
	update(image);
}


void CStructureExtractionView::OnEuclideanSegment()
{
	// TODO:  �ڴ���������������
	AfxBeginThread(WorkSeg, this);
}


void CStructureExtractionView::OnRansac()
{
	// TODO:  �ڴ���������������
	if (!bExitSeg)
	{
		MessageBox("������δ���");
		return;
	}
	AfxBeginThread(WorkFitting, this);
}


void CStructureExtractionView::OnLmeds()
{
	// TODO:  �ڴ���������������
}


void CStructureExtractionView::OnLms()
{
	// TODO:  �ڴ���������������
	if (!bExitSeg)
	{
		MessageBox("������δ���");
		return;
	}
	AfxBeginThread(WorkLMSFitting, this);
}


void CStructureExtractionView::OnDem()
{
	// TODO:  �ڴ���������������
}


void CStructureExtractionView::OnCorrection()
{
	// TODO:  �ڴ���������������
}


void CStructureExtractionView::OnParameter()
{
	// TODO:  �ڴ���������������
}


void CStructureExtractionView::OnType()
{
	// TODO:  �ڴ���������������
}


// ���ع���ͼ��
IplImage* CStructureExtractionView::getWorkImg()
{
	return workImg;
}


// ���ؾ����ĵ�
Indices& CStructureExtractionView::getIndices()
{
	//TODO: insert return statement here
	return m_indices;
}


afx_msg LRESULT CStructureExtractionView::OnFitted(WPARAM wParam, LPARAM lParam)
{
	MessageBox("������");
	IplImage* image = cvCreateImage(cvSize(m_ImgList[0]->width, m_ImgList[0]->height), m_ImgList[0]->depth, 3);
	CraterSet craterset;
	CStructureExtractionDoc* pDoc = GetDocument();
	craterset = pDoc->getCraters();
	if (m_ImgList[0]->nChannels < 3)
		cvCvtColor(m_ImgList[0], image, CV_GRAY2BGRA);
	else
		imageClone(m_ImgList[0], &image);
	for (size_t i = 0; i < craterset.size(); i++)
	{
		bool isIn = m_moonProcess.circleIsInImage(image, craterset[i]);
		if (isIn)
			cvCircle(image, cvPoint(craterset[i].picCenter.x, craterset[i].picCenter.y), craterset[i].picRadius, cvScalar(0, 0, 255), 2);
	}
	update(image);
	return 0;
}


afx_msg LRESULT CStructureExtractionView::OnSegged(WPARAM wParam, LPARAM lParam)
{
	MessageBox("ŷʽ�������");
	return 0;
}


//void CStructureExtractionView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// �˹���Ҫ�� Windows Vista ����߰汾��
//	// _WIN32_WINNT ���ű��� >= 0x0600��
//	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if (zDelta > 0)
//	{
//		OnZoomIn();
//	}
//	else
//		OnZoomOut();
//	CScrollView::OnMouseHWheel(nFlags, zDelta, pt);
//}


void CStructureExtractionView::OnFileSave()
{
	// TODO:  �ڴ���������������
	CStructureExtractionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString str = pDoc->m_title;
	LPTSTR lpFileName = (LPTSTR)(LPCTSTR)str;
	pDoc->Save(lpFileName, workImg);       //��ǰ�������
	imageClone(workImg, &saveImg);
}

//ֱ��ͼ���⻯
void CStructureExtractionView::OnEqualHist()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	cvEqualizeHist(workImg, image);
	update(image);
}


//Ӱ��ָ�
void CStructureExtractionView::OnImgSeg()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	m_moonProcess.ostu(workImg, image);
	update(image);
}

//ͼ��ʴ
void CStructureExtractionView::OnEnrode()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	cvErode(workImg, image);
	update(image);
}


void CStructureExtractionView::OnLabel()
{
	// TODO:  �ڴ���������������
	IplImage* dst1 = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	IplImage* dst2 = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	m_moonProcess.binary(workImg, dst1, 0);//��
	m_moonProcess.binary(workImg, dst2, 255);//��
	//����ת��
	cv::Mat mat_dst1, mat_dst2;
	mat_dst1 = cv::Mat(dst1);
	mat_dst2 = cv::Mat(dst2);
	//��������ǩ
	m_moonProcess.label(mat_dst1, m_bLabel);
	m_moonProcess.label(mat_dst2, m_wLabel);
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst2);
	MessageBox("������");
}

//���в���ƥ��
void CStructureExtractionView::OnMatch()
{
	// TODO:  �ڴ���������������
	std::vector<Lab> b_labs;
	std::vector<Lab> w_labs;
	CMatchDlg dlg;
	if (IDOK == dlg.DoModal())
		m_moonProcess.match(m_bLabel, m_wLabel, b_labs, w_labs, m_light, dlg.m_thetaT, dlg.m_areaRatioT, dlg.m_ratioT, m_indices);
	else
		MessageBox("�����������");
}


void CStructureExtractionView::OnCraterFitting()
{
	// TODO:  �ڴ���������������
	CraterSet craterset;
	m_moonProcess.fittingByLMS(m_indices, craterset,0,-1);
	CStructureExtractionDoc* pDoc = GetDocument();
	pDoc->setCraters(craterset);
	IplImage* image = cvCreateImage(cvSize(m_ImgList[0]->width, m_ImgList[0]->height), m_ImgList[0]->depth, 3);
	if (m_ImgList[0]->nChannels < 3)
		cvCvtColor(m_ImgList[0], image, CV_GRAY2BGRA);
	else
		imageClone(m_ImgList[0], &image);
	for (size_t i = 0; i < craterset.size(); i++)
	{
		bool isIn = m_moonProcess.circleIsInImage(image, craterset[i]);
		if (isIn)
			cvCircle(image, cvPoint(craterset[i].picCenter.x, craterset[i].picCenter.y), craterset[i].picRadius, cvScalar(0, 0, 255), 2);
	}
	update(image);
}


void CStructureExtractionView::OnHistogram()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}

	int hist_size = 256;
	int hist_height = 256;
	CvHistogram* hist;
	{
		float g_ranges[] = { 0, 255 };
		float* ranges[] = { g_ranges };
		hist = cvCreateHist(
			1,
			&hist_size,
			CV_HIST_ARRAY,
			ranges,
			1);
	}

	cvCalcHist(&workImg, hist, 0, 0);
	cvNormalizeHist(hist, 1.0);
	CHistogramDlg histogram;
	histogram.setHist(hist);
	if (IDOK == histogram.DoModal())
	{
		IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
		m_moonProcess.segByHistogram(workImg, image, histogram.m_lowThreshold, histogram.m_highThreshold);
		update(image);
	}
	else
		MessageBox("��������ָ����");
}


void CStructureExtractionView::OnEdgeCanny()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	cvSet(image, cvScalar(0));
	double low, high;
	//m_moonProcess.adaptiveFindThreshold(workImg, &low, &high);
	CCannyDlg dlg;
	if (dlg.DoModal())
	{
		low = dlg.m_low;
		high = dlg.m_high;
		cvCanny(workImg, image, low, high);
		//m_moonProcess.binary(image, image, 10);
		update(image);
		MessageBox("Canny��Ե��ȡ���");
	}
	
}


void CStructureExtractionView::OnEdgeRoberts()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels != 1)
	{
		MessageBox("ֻ�ܴ���Ҷ�ͼ��");
		return;
	}
	CRobertsDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
		cvSet(image, cvScalar(0));
		if (fx)
			cvReleaseImage(&fx);
		fx = cvCreateImage(cvSize(workImg->width, workImg->height),
			IPL_DEPTH_16S, workImg->nChannels);
		if (fy)
			cvReleaseImage(&fy);
		fy = cvCreateImage(cvSize(workImg->width, workImg->height),
			IPL_DEPTH_16S, workImg->nChannels);

		cvSobel(workImg, fx, 1, 0, dlg.m_wndSize);
		cvSobel(workImg, fy, 0, 1, dlg.m_wndSize);

		for (int i = 0; i < workImg->height; i++)
		{
			for (int j = 0; j < workImg->width; j++)
			{
				//short* ps = (short*)(slope->imageData + i*slope->widthStep);
				double sx = cvGet2D(fx, i, j).val[0];
				double sy = cvGet2D(fy, i, j).val[0];
				double s = sqrt(pow(sx, 2) + pow(sy, 2));
				uchar *ptr = (uchar*)(image->imageData + i*image->widthStep);
				if (s > dlg.m_low)
				{
					ptr[j] = 255;
				}
			}
		}
		update(image);

	}
	else
	{
		MessageBox("�����������Ե��ȡ���ڳߴ�");
		return;
	}
}


void CStructureExtractionView::OnMedian()
{
	// TODO:  �ڴ���������������
	CWndSizeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_wndSize = dlg.m_wndSize;
		IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
		cvSmooth(workImg, image, CV_MEDIAN, 0, 0, m_wndSize, m_wndSize);
		update(image);

	}
	else
	{
		MessageBox("�����������˲����ڳߴ�");
		return;
	}
}


void CStructureExtractionView::OnRgb2gray()
{
	// TODO:  �ڴ���������������
	if (workImg->nChannels == 1)
	{
		MessageBox("�Ѿ��ǻҶ�ͼ��");
		return;
	}
	else
	{
		IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), IPL_DEPTH_8U, 1);
		cvCvtColor(workImg, image, CV_BGR2GRAY);
		//imageClone(workImg, &image);
		update(image);
	}
}
