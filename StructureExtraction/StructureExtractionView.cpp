
// StructureExtractionView.cpp : CStructureExtractionView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

//多线程函数,主要用于费时的操作
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
		AfxMessageBox("请重新输入欧式聚类参数");
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
		AfxMessageBox("请重新输入RANSAC拟合参数");
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
		AfxMessageBox("请重新输入RANSAC拟合参数");
		return 0;
	}
	return 0;
}

// CStructureExtractionView 构造/析构

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
	// TODO:  在此处添加构造代码
	saveImg = NULL;
	workImg = NULL;
	fx = NULL;
	fy = NULL;
	m_lpBmi = 0;

}

CStructureExtractionView::~CStructureExtractionView()
{
	
	if (saveImg)
		cvReleaseImage(&saveImg);           //  释放位图
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
		free(m_lpBmi);                      //  释放位图信息
}

BOOL CStructureExtractionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CStructureExtractionView 绘制

void CStructureExtractionView::OnDraw(CDC* pDC)
{
	CStructureExtractionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
	if (pDoc->pImg != NULL)	{	                //  有磁盘输入图像
		if (pDoc->m_Display == 0) {             //  尚未显示
			imageClone(pDoc->pImg, &saveImg);   //  复制到备份位图
			imageClone(saveImg, &workImg);      //  复制到工作位图
			m_DibFlag = 1;
			m_ImageType = imageType(workImg);
			m_SaveFlag = 1;
			pDoc->m_Display = 1;                //文档图像已经显示

			IplImage* img = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
			imageClone(workImg, &img);
			m_ImgList.push_back(img);
			m_index++;
		}
	}
	else
		return;

	if (m_DibFlag) {                            //  DIB 结构改变
		if (m_lpBmi)
			free(m_lpBmi);
		m_ImageType = imageType(workImg);
		m_lpBmi = CtreateMapInfo(workImg, m_ImageType);
		m_DibFlag = 0;

		CSize  sizeTotal;
		sizeTotal = CSize(int(workImg->width*m_Zoom), int(workImg->height*m_Zoom));
		SetScrollSizes(MM_TEXT, sizeTotal);               //  设置滚动条
	}

	char *pBits;
	pBits = workImg->imageData;

	if (workImg) {                                        //  刷新窗口画面,双缓冲视图
		/*CMemDC dcMem(*pDC, this);
		CDC& dc = dcMem.GetDC();*/
		MemDC memDC(pDC);
		SetStretchBltMode(memDC.m_hDC, STRETCH_HALFTONE);//解决图像缩放失真的问题
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
	// TODO:  计算此视图的合计大小
	sizeTotal.cx = 100;
	sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CStructureExtractionView 打印

BOOL CStructureExtractionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CStructureExtractionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CStructureExtractionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CStructureExtractionView 诊断

#ifdef _DEBUG
void CStructureExtractionView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStructureExtractionView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CStructureExtractionDoc* CStructureExtractionView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStructureExtractionDoc)));
	return (CStructureExtractionDoc*)m_pDocument;
}
#endif //_DEBUG


// CStructureExtractionView 消息处理程序
//视图放大
void CStructureExtractionView::OnZoomIn()
{
	// TODO:  在此添加命令处理程序代码
	m_Zoom *= 2.0; // 以2倍大小速率进行缩放
	SetScrollSizes(MM_TEXT, CSize(workImg->width*m_Zoom, workImg->height*m_Zoom));
	Invalidate();
}


//视图缩小
void CStructureExtractionView::OnZoomOut()
{
	// TODO:  在此添加命令处理程序代码
	m_Zoom /= 2.0;
	/*if (m_Zoom < 1)
		m_Zoom = 1;*/
	SetScrollSizes(MM_TEXT, CSize(workImg->width*m_Zoom, workImg->height*m_Zoom));
	Invalidate();
}


BOOL CStructureExtractionView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return TRUE;
}


void CStructureExtractionView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO:  在此添加专用代码和/或调用基类
	
	pDC->SetBkColor((COLORREF)(((BYTE)(255) | ((WORD)((BYTE)(255)) << 8)) | (((DWORD)(BYTE)(255)) << 16)));
	CScrollView::OnPrepareDC(pDC, pInfo);
}

//存储当前图像
void CStructureExtractionView::OnFileSaveAs()
{
	// TODO:  在此添加命令处理程序代码
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
	//  文件存盘对话框
	if (FileDlg.DoModal() == IDOK) {           //选择了文件名
		strFileName = FileDlg.GetPathName();
		int filterIndex = FileDlg.m_ofn.nFilterIndex;
		strFileName += '.' + name[filterIndex];//添加文件后缀名
		CStructureExtractionDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		LPTSTR lpFileName = (LPTSTR)(LPCTSTR)strFileName;
		pDoc->Save(lpFileName, workImg);       //当前画面存盘
		imageClone(workImg, &saveImg);
	}
}

//回退
void CStructureExtractionView::OnBack()
{
	// TODO:  在此添加命令处理程序代码
	if (m_index > 0)
	{
		imageClone(m_ImgList[m_index - 1], &workImg);
		m_index--;
		m_DibFlag = 1;
		Invalidate();
	}
}

//前进
void CStructureExtractionView::OnForward()
{
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
	CLightDirectionDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_light = dlg.m_direction;
	}
	else
	{
		MessageBox("请输入光照信息!!!");
	}
}


void CStructureExtractionView::OnImageArea()
{
	// TODO:  在此添加命令处理程序代码
	CString csFilter = "TFW Files(*.TFW)|*.TFW|";

	CString name[] = { "", "tfw", "" };

	CString strFileName;
	CString strExtension;

	CFileDialog FileDlg(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, csFilter, this);
	//  文件读入对话框
	if (FileDlg.DoModal() == IDOK) {         //  选择了文件名
		strFileName = FileDlg.GetPathName();
		//int filterIndex = FileDlg.m_ofn.nFilterIndex;
		//strFileName += '.' + name[filterIndex];//添加文件后缀名
		LPTSTR lpFileName = (LPTSTR)(LPCTSTR)strFileName;
		std::fstream infile;
		infile.open(lpFileName, std::ios::in);
		if (!infile)
		{
			MessageBox("影像参数文件打开失败");
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
	// TODO:  在此添加命令处理程序代码
	CResizeDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IplImage* image = cvCreateImage(cvSize(workImg->width / dlg.m_resize, workImg->height/dlg.m_resize), workImg->depth, workImg->nChannels);
		cvResize(workImg,image);
		update(image);
	}
	else
	{
		MessageBox("重新输入!!");
		return;
	}
}


void CStructureExtractionView::OnFilter()
{
	// TODO:  在此添加命令处理程序代码
	
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
		MessageBox("请重新输入滤波窗口尺寸");
		return;
	}
}


void CStructureExtractionView::OnAspectCalculation()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
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
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	imageClone(workImg, &image);
	m_moonProcess.innerExcluding(image, fx, fy, m_light);
	update(image);
}


void CStructureExtractionView::OnEuclideanSegment()
{
	// TODO:  在此添加命令处理程序代码
	AfxBeginThread(WorkSeg, this);
}


void CStructureExtractionView::OnRansac()
{
	// TODO:  在此添加命令处理程序代码
	if (!bExitSeg)
	{
		MessageBox("聚类尚未完成");
		return;
	}
	AfxBeginThread(WorkFitting, this);
}


void CStructureExtractionView::OnLmeds()
{
	// TODO:  在此添加命令处理程序代码
}


void CStructureExtractionView::OnLms()
{
	// TODO:  在此添加命令处理程序代码
	if (!bExitSeg)
	{
		MessageBox("聚类尚未完成");
		return;
	}
	AfxBeginThread(WorkLMSFitting, this);
}


void CStructureExtractionView::OnDem()
{
	// TODO:  在此添加命令处理程序代码
}


void CStructureExtractionView::OnCorrection()
{
	// TODO:  在此添加命令处理程序代码
}


void CStructureExtractionView::OnParameter()
{
	// TODO:  在此添加命令处理程序代码
}


void CStructureExtractionView::OnType()
{
	// TODO:  在此添加命令处理程序代码
}


// 返回工作图像
IplImage* CStructureExtractionView::getWorkImg()
{
	return workImg;
}


// 返回聚类后的点
Indices& CStructureExtractionView::getIndices()
{
	//TODO: insert return statement here
	return m_indices;
}


afx_msg LRESULT CStructureExtractionView::OnFitted(WPARAM wParam, LPARAM lParam)
{
	MessageBox("拟合完成");
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
	MessageBox("欧式聚类完成");
	return 0;
}


//void CStructureExtractionView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// 此功能要求 Windows Vista 或更高版本。
//	// _WIN32_WINNT 符号必须 >= 0x0600。
//	// TODO:  在此添加消息处理程序代码和/或调用默认值
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
	// TODO:  在此添加命令处理程序代码
	CStructureExtractionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CString str = pDoc->m_title;
	LPTSTR lpFileName = (LPTSTR)(LPCTSTR)str;
	pDoc->Save(lpFileName, workImg);       //当前画面存盘
	imageClone(workImg, &saveImg);
}

//直方图均衡化
void CStructureExtractionView::OnEqualHist()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	cvEqualizeHist(workImg, image);
	update(image);
}


//影像分割
void CStructureExtractionView::OnImgSeg()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	m_moonProcess.ostu(workImg, image);
	update(image);
}

//图像腐蚀
void CStructureExtractionView::OnEnrode()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
		return;
	}
	IplImage* image = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	cvErode(workImg, image);
	update(image);
}


void CStructureExtractionView::OnLabel()
{
	// TODO:  在此添加命令处理程序代码
	IplImage* dst1 = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	IplImage* dst2 = cvCreateImage(cvSize(workImg->width, workImg->height), workImg->depth, workImg->nChannels);
	m_moonProcess.binary(workImg, dst1, 0);//黑
	m_moonProcess.binary(workImg, dst2, 255);//白
	//数据转换
	cv::Mat mat_dst1, mat_dst2;
	mat_dst1 = cv::Mat(dst1);
	mat_dst2 = cv::Mat(dst2);
	//进行贴标签
	m_moonProcess.label(mat_dst1, m_bLabel);
	m_moonProcess.label(mat_dst2, m_wLabel);
	cvReleaseImage(&dst1);
	cvReleaseImage(&dst2);
	MessageBox("标记完成");
}

//进行参数匹配
void CStructureExtractionView::OnMatch()
{
	// TODO:  在此添加命令处理程序代码
	std::vector<Lab> b_labs;
	std::vector<Lab> w_labs;
	CMatchDlg dlg;
	if (IDOK == dlg.DoModal())
		m_moonProcess.match(m_bLabel, m_wLabel, b_labs, w_labs, m_light, dlg.m_thetaT, dlg.m_areaRatioT, dlg.m_ratioT, m_indices);
	else
		MessageBox("重新输入参数");
}


void CStructureExtractionView::OnCraterFitting()
{
	// TODO:  在此添加命令处理程序代码
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
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
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
		MessageBox("重新输入分割参数");
}


void CStructureExtractionView::OnEdgeCanny()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
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
		MessageBox("Canny边缘提取完成");
	}
	
}


void CStructureExtractionView::OnEdgeRoberts()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels != 1)
	{
		MessageBox("只能处理灰度图像");
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
		MessageBox("请重新输入边缘提取窗口尺寸");
		return;
	}
}


void CStructureExtractionView::OnMedian()
{
	// TODO:  在此添加命令处理程序代码
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
		MessageBox("请重新输入滤波窗口尺寸");
		return;
	}
}


void CStructureExtractionView::OnRgb2gray()
{
	// TODO:  在此添加命令处理程序代码
	if (workImg->nChannels == 1)
	{
		MessageBox("已经是灰度图像");
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
