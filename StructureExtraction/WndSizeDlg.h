#pragma once


// CWndSizeDlg �Ի���

class CWndSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndSizeDlg)

public:
	CWndSizeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWndSizeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_WNDSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �˲����ڳߴ��С
	double m_wndSize;
};
