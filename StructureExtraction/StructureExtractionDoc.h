
// StructureExtractionDoc.h : CStructureExtractionDoc 类的接口
//


#pragma once

#include "Crater.h"

#include <vector>

class CStructureExtractionDoc : public CDocument
{
protected: // 仅从序列化创建
	CStructureExtractionDoc();
	DECLARE_DYNCREATE(CStructureExtractionDoc)

// 特性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CStructureExtractionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// 影像数据
	IplImage* pImg;
	// 显示状态
	int m_Display;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
protected:
	// 读入图像
	bool Load(IplImage** pImg, LPCTSTR pszFilename);
public:
	// 存储图像
	bool Save(LPCTSTR pszFilename, IplImage* pImg);
	// 文档标题名
	CString m_title;
private:
	// 撞击坑
	CraterSet m_craters;
public:
	// 设定撞击坑
	void setCraters(CraterSet craters);
	// 存储撞击坑
	void saveCraters();
	// 拟合的撞击坑
	CraterSet getCraters();
};
