#pragma once


// CResizeDlg �Ի���

class CResizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CResizeDlg)

public:
	CResizeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResizeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �ز�������
	double m_resize;
};
