
/*
*����Ӱ�����⴦��
*2015.12
*/

#ifndef MoonImageProcessing_h
#define MoonImageProcessing_h

#include <pcl/ModelCoefficients.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/features/normal_3d.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>

#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

#include "highgui.h"
#include "cv.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>

#include "Crater.h"

class MoonPoint
{
public:
	double x;
	double y;
	MoonPoint(){ x = 0; y = 0; };
};

typedef std::vector<MoonPoint>  MoonPointCloud;

typedef std::vector<MoonPointCloud > Indices;

class Lab
{
public:
	MoonPointCloud cloud;
	MoonPoint center;
	int count;
	bool matched;
	Lab(){ count = 0; matched = false; };
};

class MoonImageProcessing
{
public:
	MoonImageProcessing();
	~MoonImageProcessing();

	//�ڱ�Ե�޳�
	void innerExcluding(IplImage* image, IplImage* fx, IplImage* fy, double m_light);

	//ŷʽ����
	void EuclideanCluster(const IplImage *canny, double min_dis, int min_clusterNum, int max_clusterNum, Indices& out);

	/*RANSAC������ϣ�RANSAC���нϺõĿ�����
	* Բ�ķ���Ϊ(x - a)^2 + (y - b)^2 = r^2
	*/
	void fittingByRansac(const Indices& indices, CraterSet& craterset, double threshold, double proportion);

	/*���¶��˷����*/
	void fittingByLMS(Indices& indices, CraterSet& craterset, double in_threshold, double p_threshold);

	/*ostu��ͼ����зָ�*/
	void ostu(const IplImage* src, IplImage*dst);


	//����ɨ�跨���б��,�ٶȿ�������������
	void label(const cv::Mat& binImg, cv::Mat& lableImg);

	//��ֵ��
	void binary(const IplImage*src, IplImage*dst, int T);

	/*����ͨ�����ƥ��
	*b_lable:��ɫ��ǩ,w_lable:��ɫ��ǩ,light:����
	*theta_T:����շ���н���ֵ,ratio_T:�ڰ׵���֮��,num2dis_T:���������֮��
	*/
	void match(cv::Mat b_label, cv::Mat w_label, std::vector<Lab>& b_labs, std::vector<Lab>& w_labs, double light, double theta_T, double ratio_T, double num2dis_T, Indices& indice);


	bool circleIsInImage(const IplImage* src, Crater crater);

	//ֱ��ͼ�ָ�
	void segByHistogram(const IplImage* src, IplImage* dst, int low, int high);

	//����Ӧcanny����
	void adaptiveFindThreshold(const CvArr* image, double *low, double *high, int aperture_size = 3);

private:
	//��ȡͼ��low-high��Χ�ڵ����ֵ
	int rmax(const IplImage* src, double low, double high);

	//��ȡͼ��low-high��Χ�ڵ���Сֵ
	int rmin(const IplImage* src, double low, double high);

	//��ȡͼ��low-high��Χ�ڵľ�ֵ
	double raverage(const IplImage* src, double low, double high);

	//��ȡͼ��ľ�ֵ
	double average(const IplImage* src);

	void InnerLabel(cv::Mat &label, std::vector<Lab> &labs, int bnum);

	void InnerMatch(Lab &b_lab, Lab &w_lab, double light, double theta_T, double ratio_T, double num2dis_T, const cv::Mat &b_label, const cv::Mat &w_label, Indices &indice);

	void isEdge(const Lab &lab, const cv::Mat &label, int * dx, int * dy, MoonPoint &center_point, double dis, MoonPointCloud &ptcloud);

	void _adaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high);
};

#endif

