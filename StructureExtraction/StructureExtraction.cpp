
// StructureExtraction.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "StructureExtraction.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "StructureExtractionDoc.h"
#include "StructureExtractionView.h"
#include "ImageSelectDlg.h"

#include <highgui.h>
#include <cv.h>
#include "Processing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStructureExtractionApp

BEGIN_MESSAGE_MAP(CStructureExtractionApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CStructureExtractionApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_IMG_SEL, &CStructureExtractionApp::OnImgSel)
END_MESSAGE_MAP()


// CStructureExtractionApp ����

CStructureExtractionApp::CStructureExtractionApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("StructureExtraction.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CStructureExtractionApp ����

CStructureExtractionApp theApp;


// CStructureExtractionApp ��ʼ��

BOOL CStructureExtractionApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_StructureExtracTYPE,
		RUNTIME_CLASS(CStructureExtractionDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CStructureExtractionView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	
	//��ʼ��ʱû���ĵ�
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CStructureExtractionApp::ExitInstance()
{
	//TODO:  �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CStructureExtractionApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CStructureExtractionApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStructureExtractionApp ��Ϣ�������





void CStructureExtractionApp::OnImgSel()
{
	// TODO:  �ڴ���������������
	CImageSelectDlg selectDlg;
	selectDlg.m_fileList = m_imageFileList;
	CString left, right;
	if (IDOK == selectDlg.DoModal())
	{
		left = selectDlg.m_left;
		right = selectDlg.m_right;
	}
	
	IplImage* leftImg; //��Ӱ��
	IplImage* rightImg; //��Ӱ��
	POSITION PosDocTemplate = GetFirstDocTemplatePosition();
	if (PosDocTemplate)
	{
		CDocTemplate* pDocTemplate = GetNextDocTemplate(PosDocTemplate);

		POSITION PosDoc = pDocTemplate->GetFirstDocPosition();
		while (PosDoc)
		{
			CStructureExtractionDoc* pDoc = (CStructureExtractionDoc*)pDocTemplate->GetNextDoc(PosDoc);
			/*POSITION PosView = pDoc->GetFirstViewPosition();
			CView *pView = (CView *)pDoc->GetNextView(PosView);*/
			if (left == (pDoc->m_title))
			{
				leftImg = cvCreateImage(cvSize(pDoc->pImg->width, pDoc->pImg->width), pDoc->pImg->depth, pDoc->pImg->nChannels);
				imageClone(pDoc->pImg, &leftImg);
			}
			if (right == (pDoc->m_title))
			{
				rightImg = cvCreateImage(cvSize(pDoc->pImg->width, pDoc->pImg->width), pDoc->pImg->depth, pDoc->pImg->nChannels);
				imageClone(pDoc->pImg, &rightImg);
			}
		}
	}
	
}
