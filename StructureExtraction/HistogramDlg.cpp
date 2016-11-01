// Histogram.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "HistogramDlg.h"
#include "afxdialogex.h"


// CHistogramDlg �Ի���

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


// CHistogramDlg ��Ϣ�������


void CHistogramDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	//�������
	CString str;
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_HIST);
	CPaintDC dc(pWnd);//
	pWnd->GetClientRect(&rect);
	//CStructureExtractionDoc* pDoc = CStructureExtractionDoc::GetDoc();
	

	//������ɫ�������ڻ�������
	CPen* pPenblack = new CPen;
	pPenblack->CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pPengary = new CPen;
	pPengary->CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
	CGdiObject* pOldPen = dc.SelectObject(pPenblack);

	//����������
	dc.MoveTo(10, rect.Height() - 10);//��ͼ�����½Ƕ����ԭ�㡣
	dc.LineTo(10, 0);//����Y��
	dc.MoveTo(10, rect.Height() - 10);
	dc.LineTo(rect.Width(), rect.Height() - 10);//����X������
	//����X���ͷ
	dc.MoveTo(10, 0);
	dc.LineTo(4, 6);
	dc.MoveTo(10, 0);
	dc.LineTo(16, 6);
	//����Y���ͷ
	dc.MoveTo(rect.Width(), rect.Height() - 10);
	dc.LineTo(rect.Width() - 6, rect.Height() - 4);
	dc.MoveTo(rect.Width(), rect.Height() - 10);
	dc.LineTo(rect.Width() - 6, rect.Height() - 16);
	//ɾ������
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
		float bin_val = cvQueryHistValue_1D(hist, k); //����i�ĸ���  
		int intensity = cvRound(bin_val*(rect.Height()-10) / max_value);  //Ҫ���Ƶĸ߶�  
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
