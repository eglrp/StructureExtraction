#pragma once


// CCannyDlg �Ի���

class CCannyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyDlg)

public:
	CCannyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCannyDlg();

// �Ի�������
	enum { IDD = IDD_CANNY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����ֵ
	double m_low;
	// ����ֵ
	double m_high;
};
