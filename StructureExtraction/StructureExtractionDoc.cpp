
// StructureExtractionDoc.cpp : CStructureExtractionDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "StructureExtraction.h"
#endif

#include "StructureExtractionDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStructureExtractionDoc

IMPLEMENT_DYNCREATE(CStructureExtractionDoc, CDocument)

BEGIN_MESSAGE_MAP(CStructureExtractionDoc, CDocument)
END_MESSAGE_MAP()


// CStructureExtractionDoc ����/����

CStructureExtractionDoc::CStructureExtractionDoc()
: pImg(NULL)
, m_Display(-1)
, m_title(_T(""))
{
	// TODO:  �ڴ����һ���Թ������

}

CStructureExtractionDoc::~CStructureExtractionDoc()
{
	if (pImg)
		cvReleaseImage(&pImg);
}

BOOL CStructureExtractionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CStructureExtractionDoc ���л�

void CStructureExtractionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CStructureExtractionDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CStructureExtractionDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CStructureExtractionDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CStructureExtractionDoc ���

#ifdef _DEBUG
void CStructureExtractionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStructureExtractionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStructureExtractionDoc ����


BOOL CStructureExtractionDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	BOOL state;
	Load(&pImg, lpszPathName);
	if (pImg != NULL)
	{
		CString path = lpszPathName;
		/*��ȡ�ļ���*/
		CString title = path.Mid(path.ReverseFind('\\') + 1);
		m_title = title;
		((CStructureExtractionApp*)AfxGetApp())->m_imageFileList.push_back((LPTSTR)(LPCTSTR)title);
		state = true;
	}
	else state = false;
	return(state);
}


// ����ͼ��
bool CStructureExtractionDoc::Load(IplImage** pImg, LPCTSTR pszFilename)
{
	IplImage* Img = NULL;

	Img = cvLoadImage(pszFilename, -1);      //  ��ͼ���ļ�(DSCV)
	if (!Img) return(false);
	cvFlip(Img);                           //  �� DIB ���ؽṹһ��
	if (*pImg) {
		cvReleaseImage(pImg);
	}
	(*pImg) = Img;
	m_Display = 0;
	return(true);
}


// �洢ͼ��
bool CStructureExtractionDoc::Save(LPCTSTR pszFilename, IplImage* pImg)
{
	int   bl;

	cvFlip(pImg);                           //  �ָ�ԭ OpenCV λͼ�ṹ
	bl = cvSaveImage(pszFilename, pImg);        //  ͼ�����
	return(bl);
}


// �趨ײ����
void CStructureExtractionDoc::setCraters(CraterSet craters)
{
	this->m_craters = craters;
}


// �洢ײ����
void CStructureExtractionDoc::saveCraters()
{

}


// ��ϵ�ײ����
CraterSet CStructureExtractionDoc::getCraters()
{
	return m_craters;
}
