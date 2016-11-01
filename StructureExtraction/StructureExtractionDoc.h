
// StructureExtractionDoc.h : CStructureExtractionDoc ��Ľӿ�
//


#pragma once

#include "Crater.h"

#include <vector>

class CStructureExtractionDoc : public CDocument
{
protected: // �������л�����
	CStructureExtractionDoc();
	DECLARE_DYNCREATE(CStructureExtractionDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CStructureExtractionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// Ӱ������
	IplImage* pImg;
	// ��ʾ״̬
	int m_Display;
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
protected:
	// ����ͼ��
	bool Load(IplImage** pImg, LPCTSTR pszFilename);
public:
	// �洢ͼ��
	bool Save(LPCTSTR pszFilename, IplImage* pImg);
	// �ĵ�������
	CString m_title;
private:
	// ײ����
	CraterSet m_craters;
public:
	// �趨ײ����
	void setCraters(CraterSet craters);
	// �洢ײ����
	void saveCraters();
	// ��ϵ�ײ����
	CraterSet getCraters();
};
