#pragma once


// CEuclideanDlg �Ի���

class CEuclideanDlg : public CDialog
{
	DECLARE_DYNAMIC(CEuclideanDlg)

public:
	CEuclideanDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEuclideanDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_EUCLIDEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ������С����
	double m_minDis;
	// ���ٵ���
	int m_minCluster;
	// ������
	int m_maxCluster;
};
