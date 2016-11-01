#pragma once

#include "cv.h"
#include "highgui.h"
// CHistogramDlg 对话框

class CHistogramDlg : public CDialog
{
	DECLARE_DYNAMIC(CHistogramDlg)

public:
	CHistogramDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	// 直方图统计
	CvHistogram* hist;
	void setHist(CvHistogram* hist);
	// 低阈值
	int m_lowThreshold;
	// 高阈值
	int m_highThreshold;
};
