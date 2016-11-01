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
	// 左影像
	IplImage* m_leftImage;
	// 右影像
	IplImage* m_rightImage;
public:
	// 设置影像数据
	void setLeftImage(IplImage* left, IplImage* right);
};

