#pragma once


// CEuclideanDlg 对话框

class CEuclideanDlg : public CDialog
{
	DECLARE_DYNAMIC(CEuclideanDlg)

public:
	CEuclideanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEuclideanDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EUCLIDEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 聚类最小距离
	double m_minDis;
	// 最少点数
	int m_minCluster;
	// 最多点数
	int m_maxCluster;
};
