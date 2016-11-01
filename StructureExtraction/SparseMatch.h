#pragma once
#include <highgui.h>
#include <cv.h>
#include "Processing.h"

class SparseMatch
{
public:
	SparseMatch();
	SparseMatch(IplImage* left, IplImage* right);
	~SparseMatch();
private:
	// ��Ӱ��
	IplImage* m_leftImage;
	// ��Ӱ��
	IplImage* m_rightImage;
public:
	// ����Ӱ������
	void setLeftImage(IplImage* left, IplImage* right);
};

