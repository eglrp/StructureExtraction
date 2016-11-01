#pragma once

#include "cv.h"
#include "highgui.h"
// CHistogramDlg �Ի���

class CHistogramDlg : public CDialog
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	// ֱ��ͼͳ��
	CvHistogram* hist;
	void setHist(CvHistogram* hist);
	// ����ֵ
	int m_lowThreshold;
	// ����ֵ
	int m_highThreshold;
};
