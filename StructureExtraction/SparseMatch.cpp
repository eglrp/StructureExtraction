#include "stdafx.h"
#include "SparseMatch.h"


SparseMatch::SparseMatch()
: m_leftImage(NULL)
, m_rightImage(NULL)
{
}

SparseMatch::SparseMatch(IplImage* left, IplImage* right)
{
	m_leftImage = cvCreateImage(cvSize(left->width, left->height), left->depth, left->nChannels);
	m_rightImage = cvCreateImage(cvSize(right->width, right->height), right->depth, right->nChannels);
}

SparseMatch::~SparseMatch()
{
	if (!m_leftImage)
		cvReleaseImage(&m_leftImage);
	if (!m_rightImage)
		cvReleaseImage(&m_rightImage);
}


// 设置影像数据
void SparseMatch::setLeftImage(IplImage* left, IplImage* right)
{
	m_leftImage = cvCreateImage(cvSize(left->width, left->height), left->depth, left->nChannels);
	m_rightImage = cvCreateImage(cvSize(right->width, right->height), right->depth, right->nChannels);
	imageClone(left, &m_leftImage);
	imageClone(right, &m_rightImage);
}
