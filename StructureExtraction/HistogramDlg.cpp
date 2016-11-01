// Histogram.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg 对话框

IMPLEMENT_DYNAMIC(CHistogramDlg, CDialog)

CHistogramDlg::CHistogramDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistogramDlg::IDD, pParent)
	, hist(NULL)
	, m_lowThreshold(0)
	, m_highThreshold(0)
{

}

CHistogramDlg::~CHistogramDlg()
{
}

void CHistogramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOW, m_lowThreshold);
	DDX_Text(pDX, IDC_EDIT_HIGH, m_highThreshold);
}


BEGIN_MESSAGE_MAP(CHistogramDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramDlg 消息处理程序


void CHistogramDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	//定义变量
	CString str;
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_HIST);
	CPaintDC dc(pWnd);//
	pWnd->GetClientRect(&rect);
	//CStructureExtractionDoc* pDoc = CStructureExtractionDoc::GetDoc();
	

	//创建黑色画笔用于绘制坐标
	CPen* pPenblack = new CPen;
	pPenblack->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pPengary = new CPen;
	pPengary->CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	CGdiObject* pOldPen = dc.SelectObject(pPenblack);

	//绘制坐标轴
	dc.MoveTo(10, rect.Height() - 10);//绘图区左下角定义的原点。
	dc.LineTo(10, 0);//绘制Y轴
	dc.MoveTo(10, rect.Height() - 10);
	dc.LineTo(rect.Width(), rect.Height() - 10);//绘制X轴坐标
	//绘制X轴箭头
	dc.MoveTo(10, 0);
	dc.LineTo(4, 6);
	dc.MoveTo(10, 0);
	dc.LineTo(16, 6);
	//绘制Y轴箭头
	dc.MoveTo(rect.Width(), rect.Height() - 10);
	dc.LineTo(rect.Width() - 6, rect.Height() - 4);
	dc.MoveTo(rect.Width(), rect.Height() - 10);
	dc.LineTo(rect.Width() - 6, rect.Height() - 16);
	//删除画笔
	dc.SelectObject(pPengary);
	int interval = 30;
	int num = 256 / interval;
	int width = rect.Width() - 10;
	double step = (double)width / num;
	for (int j = 0; j < num; j++)
	{
		int m = int(8 + step*j);
		int n = j*interval;
		str.Format("%d", n);
		dc.TextOut(m, rect.Height() - 8, str);
	}
	int xrange = int(8 + step*(num-1));
	if (xrange<rect.Width())
		dc.TextOut(rect.Width()-2, rect.Height() - 8, "255");
	dc.SelectObject(pOldPen);
	delete pPenblack;
	delete pPengary;
	CPen* pPenblue = new CPen;
	pPenblue->CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	CGdiObject* pOldPen2 = dc.SelectObject(pPenblue);
	float max_value = 0;
	cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);
	double val_step = (double)(rect.Width() - 10) / 256;
	for (int k = 0; k < 256; k++)
	{
		float bin_val = cvQueryHistValue_1D(hist, k); //像素i的概率  
		int intensity = cvRound(bin_val*(rect.Height()-10) / max_value);  //要绘制的高度  
		dc.MoveTo(int(10 + val_step * k), rect.Height() - 10);
		dc.LineTo(int(10 + val_step * k), rect.Height() - 10 - intensity);
	}
	dc.SelectObject(pOldPen2);
	delete pPenblue;
}


void CHistogramDlg::setHist(CvHistogram* hist)
{
	this->hist = hist;
}
