
// StructureExtractionDoc.cpp : CStructureExtractionDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CStructureExtractionDoc 构造/析构

CStructureExtractionDoc::CStructureExtractionDoc()
: pImg(NULL)
, m_Display(-1)
, m_title(_T(""))
{
	// TODO:  在此添加一次性构造代码

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

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CStructureExtractionDoc 序列化

void CStructureExtractionDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CStructureExtractionDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CStructureExtractionDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CStructureExtractionDoc 诊断

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


// CStructureExtractionDoc 命令


BOOL CStructureExtractionDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	BOOL state;
	Load(&pImg, lpszPathName);
	if (pImg != NULL)
	{
		CString path = lpszPathName;
		/*获取文件名*/
		CString title = path.Mid(path.ReverseFind('\\') + 1);
		m_title = title;
		((CStructureExtractionApp*)AfxGetApp())->m_imageFileList.push_back((LPTSTR)(LPCTSTR)title);
		state = true;
	}
	else state = false;
	return(state);
}


// 读入图像
bool CStructureExtractionDoc::Load(IplImage** pImg, LPCTSTR pszFilename)
{
	IplImage* Img = NULL;

	Img = cvLoadImage(pszFilename, -1);      //  读图像文件(DSCV)
	if (!Img) return(false);
	cvFlip(Img);                           //  与 DIB 像素结构一致
	if (*pImg) {
		cvReleaseImage(pImg);
	}
	(*pImg) = Img;
	m_Display = 0;
	return(true);
}


// 存储图像
bool CStructureExtractionDoc::Save(LPCTSTR pszFilename, IplImage* pImg)
{
	int   bl;

	cvFlip(pImg);                           //  恢复原 OpenCV 位图结构
	bl = cvSaveImage(pszFilename, pImg);        //  图像存盘
	return(bl);
}


// 设定撞击坑
void CStructureExtractionDoc::setCraters(CraterSet craters)
{
	this->m_craters = craters;
}


// 存储撞击坑
void CStructureExtractionDoc::saveCraters()
{

}


// 拟合的撞击坑
CraterSet CStructureExtractionDoc::getCraters()
{
	return m_craters;
}
