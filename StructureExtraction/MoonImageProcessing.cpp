#include "stdafx.h"
#include "MoonImageProcessing.h"


MoonImageProcessing::MoonImageProcessing()
{
}


MoonImageProcessing::~MoonImageProcessing()
{
}

void MoonImageProcessing::innerExcluding(IplImage* image, IplImage* fx, IplImage* fy, double m_light)
{
	for (int i = 0; i < image->height; i++)
	{
		uchar * p = (uchar*)(image->imageData + i*image->widthStep);
		for (int j = 0; j < image->width; j++)
		{
			if (p[j] == 255)
			{
				double x = cvGet2D(fx, i, j).val[0];
				double y = cvGet2D(fy, i, j).val[0];
				double th = atan2(y, x)*180.0 / CV_PI;
				th = th + 180;
				if (th>360)
					th = th - 360;
				double derta = fabs(m_light - th);
				if (derta > 90)
				{
					p[j] = 0;
				}
			}
		}
	}
}

//ŷʽ����
void MoonImageProcessing::EuclideanCluster(const IplImage *canny, double min_dis, int min_clusterNum, int max_clusterNum, Indices& out)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr crater(new pcl::PointCloud<pcl::PointXYZ>);
	CvScalar P;
	pcl::PointXYZ temp;
	for (int i = 0; i < canny->height; i++)
	{
		for (int j = 0; j < canny->width; j++)
		{
			P = cvGet2D(canny, i, j);
			if (P.val[0] != 0)
			{
				temp.x = j;
				temp.y = i;
				temp.z = 0;
				crater->points.push_back(temp);
			}
		}
	}
	std::vector<pcl::PointIndices> indices;
	//����һ��kd�����͵�������ʽ  �������Ʒ���tree��
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud(crater);
	//����ŷʽ����ָ��һЩ����
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
	ec.setInputCloud(crater);
	ec.setSearchMethod(tree);
	//����������������֮�ڵ���һ������
	ec.setClusterTolerance(min_dis);
	//��С�Ĳ������С����������Ͳ�����������У�==�������Сֵѡ��Ϊ�ڽ���״�ĵ��� 
	ec.setMinClusterSize(min_clusterNum);
	//���Ĳ������������������Ͳ�����������У�==��Ҫ����ʦ�ֵĹ��캯����ѡ�������С����ͳ��һ������1500���� ��������ѡ���2000
	ec.setMaxClusterSize(max_clusterNum);
	ec.extract(indices);

	for (std::vector<pcl::PointIndices>::const_iterator it = indices.begin(); it != indices.end(); it++)
	{
		std::vector<MoonPoint> result;
		for (int i = 0; i < it->indices.size(); i++)
		{
			MoonPoint temp;
			temp.x = crater->points[it->indices[i]].x;
			temp.y = crater->points[it->indices[i]].y;
			result.push_back(temp);
		}
		out.push_back(result);
	}
}

/*RANSAC������ϣ�RANSAC���нϺõĿ�����
* Բ�ķ���Ϊ(x - a)^2 + (y - b)^2 = r^2
*/
void MoonImageProcessing::fittingByRansac(const Indices& indices, CraterSet& craterset, double threshold, double proportion)
{
	srand((unsigned)time(NULL));
	for (size_t i = 0; i < indices.size(); i++)
	{
		if (indices[i].size() <= 5)
			continue;
		//��ʼ��������
		int initial_iter = int(ceil(log10(0.01) / log10(1.0 - 3.0 / indices[i].size())));
		int iter(0);  //RANSACѭ������
		int inliers_l(0), inliers_n(0);//��һ��ѭ�����ڵ���Ŀ������ѭ�����ڵ���Ŀ 

		int randnum = 3;
		int* nIndex = new int[randnum];    //���������
		Crater crater;

		while (iter < initial_iter)
		{

			//��ȡ�����
			for (int index = 0; index < randnum; index++)
			{
				nIndex[index] = rand() % indices[i].size();
			}

			//�жϳ�ȡ������Ƿ��ظ�
			bool allsame(true);
			for (int index = 1; index < randnum; index++)
			{
				allsame = allsame&&nIndex[index] == nIndex[index - 1];
			}
			if (allsame)
				continue;

			//���̼���
			Eigen::MatrixXd A(randnum, randnum), L(randnum, 1), X(randnum, 1);
			for (int index = 0; index < randnum; index++)
			{
				double x = indices[i][nIndex[index]].x;
				double y = indices[i][nIndex[index]].y;
				A(index, 0) = x;
				A(index, 1) = y;
				A(index, 2) = 1.0;
				L(index, 0) = -pow(x, 2.0) - pow(y, 2.0);
			}
			/*Eigen::LLT<Eigen::MatrixXd> llt;
			llt.compute(A);*/
			X = /*A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(L)*//*llt.solve(L)*/
				(A.transpose()*A).inverse()*A.transpose()*L;
			double a, b, c;
			a = X(0, 0); b = X(1, 0); c = X(2, 0);
			double cA = -a / 2.0;
			double cB = -b / 2.0;
			double cR = sqrt(pow(a, 2.0) + pow(b, 2.0) - 4.0*c) / 2.0;

			inliers_n = 0;
			for (size_t index = 0; index < indices[i].size(); ++index)
			{
				double x = indices[i][index].x;
				double y = indices[i][index].y;
				double distance = fabs(sqrt(pow(x - cA, 2.0) + pow(y - cB, 2.0)) - cR);
				if (distance <= threshold)
				{
					inliers_n++;
				}
			}


			if (inliers_n > inliers_l)
			{
				inliers_l = inliers_n;
				initial_iter = int(ceil(log10(0.01) / log10(1.0 - pow((double)inliers_n / indices[i].size(), 3))));//����ѭ������
				crater.picCenter.x = cA;
				crater.picCenter.y = cB;
				crater.picRadius = cR;
				iter = 0;
				continue;//������һ��ѭ��
			}
			iter++;
		}
		delete[]nIndex;

		//ͨ���жϱ����Ƿ�Ϊײ����
		double regionNum[4] = { 0 };//���������ĸ�����,�ĸ�����
		for (size_t index = 0; index < indices[i].size(); ++index)
		{
			double x = indices[i][index].x;
			double y = indices[i][index].y;
			double dx = x - crater.picCenter.x;
			double dy = y - crater.picCenter.y;
			double distance = fabs(sqrt(pow(dx, 2.0) + pow(dy, 2.0)) - crater.picRadius);
			if (distance <= threshold)
			{
				double th = atan2(dy, dx) * 180 / CV_PI;
				if (th < 0)
					th += 360.0;
				int id = th / 90;
				regionNum[id] += 1.0;
			}
		}
		int meet(0);
		for (int id = 2; id < 4; id++)
		{
			regionNum[id] /= inliers_l;
			if (regionNum[id] > 1.0 / 16.0)
				meet++;
		}
		if (meet < 2)
			continue;
		craterset.push_back(crater);
	}
}

/*���¶��˷����*/
void MoonImageProcessing::fittingByLMS(Indices& indices, CraterSet& craterset, double in_threshold, double p_threshold)
{
	for (size_t i = 0; i < indices.size(); i++)
	{
		Crater crater;
		MoonPointCloud& cloud = indices[i];
		size_t num = cloud.size();
		Eigen::MatrixXd A(num, 3), L(num, 1), X(3, 1);
		for (int j = 0; j < num; j++)
		{
			double x = cloud[j].x;
			double y = cloud[j].y;
			A(j, 0) = x;
			A(j, 1) = y;
			A(j, 2) = 1.0;
			L(j, 0) = -pow(x, 2.0) - pow(y, 2.0);
		}
		/*Eigen::LLT<Eigen::MatrixXd> llt;
		llt.compute(A);*/
		X = /*A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(L)*//*llt.solve(L)*/
			(A.transpose()*A).inverse()*A.transpose()*L;
		double a, b, c;
		a = X(0, 0); b = X(1, 0); c = X(2, 0);
		double cA = -a / 2.0;
		double cB = -b / 2.0;
		double cR = sqrt(pow(a, 2.0) + pow(b, 2.0) - 4.0*c) / 2.0;
		crater.picCenter.x = cA;
		crater.picCenter.y = cB;
		crater.picRadius = cR;
		int inliers = 0;
		for (size_t k = 0; k < indices[i].size(); k++)
		{
			double dis = sqrt(pow(indices[i][k].x - cA, 2) + pow(indices[i][k].y - cB, 2));
			if (dis < in_threshold)
				inliers++;
		}
		double pro = (double)inliers / indices[i].size();
		if (pro>p_threshold)
			craterset.push_back(crater);
	}
}

/*ostu��ͼ����зָ�*/
void MoonImageProcessing::ostu(const IplImage* src, IplImage*dst)
{
	int p1, p2, p3, p4;
	int t0 = 0, t1 = 127, t2 = 128, t3 = 255;
	t1 = average(src);
	t2 = t1 + 1;
	int T1, T2;
	p1 = rmax(src, t0, t1);
	p2 = rmin(src, t0, t1);
	T1 = (p1 + p2) / 2;
	while (1)
	{
		p1 = raverage(src, t0, T1);
		p2 = raverage(src, T1 + 1, t1);
		int T, dertaT;
		T = T1;
		T1 = (p1 + p2) / 2;
		dertaT = T1 - T;
		if (dertaT == 0)
			break;
	}

	p3 = rmax(src, t2, t3);
	p4 = rmin(src, t2, t3);
	T2 = (p3 + p4) / 2;
	while (1)
	{
		p3 = raverage(src, t2, T2);
		p4 = raverage(src, T2 + 1, t3);
		int T, dertaT;
		T = T2;
		T2 = (p3 + p4) / 2;
		dertaT = T2 - T;
		if (dertaT == 0)
			break;
	}
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j<src->width; j++)
		{
			uchar *pvalue = (uchar*)(src->imageData + i*src->widthStep + j);
			uchar *pdst = (uchar*)(dst->imageData + i*dst->widthStep + j);
			if (*pvalue >= 0 && *pvalue <= T1)
				*pdst = 0;
			else if (*pvalue>T1&&*pvalue < T2)
				*pdst = 128;
			else
				*pdst = 255;
		}
	}
}

//��ȡͼ��low-high��Χ�ڵ����ֵ
int MoonImageProcessing::rmax(const IplImage* src, double low, double high)
{
	int height = src->height;
	int width = src->width;
	int lineByte = src->widthStep;
	double max = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double pix_value = cvGet2D(src, i, j).val[0];
			if (pix_value >= max&&pix_value <= high&&pix_value >= low)
				max = pix_value;
		}
	}
	return max;
}

//��ȡͼ��low-high��Χ�ڵ���Сֵ
int MoonImageProcessing::rmin(const IplImage* src, double low, double high)
{
	int height = src->height;
	int width = src->width;
	int lineByte = src->widthStep;
	double min = INT_MAX;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double pix_value = cvGet2D(src, i, j).val[0];
			if (pix_value <= min&&pix_value <= high&&pix_value >= low)
				min = pix_value;
		}
	}
	return min;
}

//��ȡͼ��low-high��Χ�ڵľ�ֵ
double MoonImageProcessing::raverage(const IplImage* src, double low, double high)
{
	int height = src->height;
	int width = src->width;
	int lineByte = src->widthStep;
	double s = 0;
	int count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double pix_value = cvGet2D(src, i, j).val[0];
			if (pix_value <= high&&pix_value >= low)
			{
				s += pix_value;
				count++;
			}
		}
	}
	s = s / count;
	return s;
}

//��ȡͼ��ľ�ֵ
double MoonImageProcessing::average(const IplImage* src)
{
	int height = src->height;
	int width = src->width;
	int lineByte = src->widthStep;
	double s = 0;
	int count = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double pix_value = cvGet2D(src, i, j).val[0];
			s += pix_value;
			count++;
		}
	}
	s = s / count;
	return s;
}

//����ɨ�跨���б��,�ٶȿ�������������
void MoonImageProcessing::label(const cv::Mat& binImg, cv::Mat& lableImg)
{
	if (binImg.empty() ||
		binImg.type() != CV_8UC1)
	{
		return;
	}

	// ��һ��ͨ·

	lableImg.release();
	binImg.convertTo(lableImg, CV_32SC1);

	int label = 1;
	std::vector<int> labelSet;
	labelSet.push_back(0);
	labelSet.push_back(1);

	int rows = binImg.rows - 1;
	int cols = binImg.cols - 1;
	for (int i = 1; i < rows; i++)
	{
		int* data_preRow = lableImg.ptr<int>(i - 1);
		int* data_curRow = lableImg.ptr<int>(i);
		for (int j = 1; j < cols; j++)
		{
			if (data_curRow[j] == 1)
			{
				std::vector<int> neighborLabels;
				neighborLabels.reserve(2);
				int leftPixel = data_curRow[j - 1];
				int upPixel = data_preRow[j];
				if (leftPixel > 1)
				{
					neighborLabels.push_back(leftPixel);
				}
				if (upPixel > 1)
				{
					neighborLabels.push_back(upPixel);
				}

				if (neighborLabels.empty())
				{
					labelSet.push_back(++label);  // ����ͨ����ǩ+1
					data_curRow[j] = label;
					labelSet[label] = label;
				}
				else
				{
					std::sort(neighborLabels.begin(), neighborLabels.end());
					int smallestLabel = neighborLabels[0];
					data_curRow[j] = smallestLabel;

					// ������С�ȼ۱�
					for (size_t k = 1; k < neighborLabels.size(); k++)
					{
						int tempLabel = neighborLabels[k];
						int& oldSmallestLabel = labelSet[tempLabel];
						if (oldSmallestLabel > smallestLabel)
						{
							labelSet[oldSmallestLabel] = smallestLabel;
							oldSmallestLabel = smallestLabel;
						}
						else if (oldSmallestLabel < smallestLabel)
						{
							labelSet[smallestLabel] = oldSmallestLabel;
						}
					}
				}
			}
		}
	}

	// ���µȼ۶��б�
	// ����С��Ÿ��ظ�����
	for (size_t i = 2; i < labelSet.size(); i++)
	{
		int curLabel = labelSet[i];
		int preLabel = labelSet[curLabel];
		while (preLabel != curLabel)
		{
			curLabel = preLabel;
			preLabel = labelSet[preLabel];
		}
		labelSet[i] = curLabel;
	};

	for (int i = 0; i < rows; i++)
	{
		int* data = lableImg.ptr<int>(i);
		for (int j = 0; j < cols; j++)
		{
			int& pixelLabel = data[j];
			pixelLabel = labelSet[pixelLabel];
		}
	}
}

//��ֵ��
void MoonImageProcessing::binary(const IplImage*src, IplImage*dst, int T)
{
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j < src->width; j++)
		{
			uchar *pvalue = (uchar*)(src->imageData + i*src->widthStep + j);
			uchar *pdst = (uchar*)(dst->imageData + i*dst->widthStep + j);
			if ((int)(*pvalue) == T)
				*pdst = 1;
			else
				*pdst = 0;
		}
	}
}

/*����ͨ�����ƥ��
*b_lable:��ɫ��ǩ,w_lable:��ɫ��ǩ,light:����
*theta_T:����շ���н���ֵ,ratio_T:�ڰ׵���֮��,num2dis_T:���������֮��
*/
void MoonImageProcessing::match(cv::Mat b_label, cv::Mat w_label, std::vector<Lab>& b_labs, std::vector<Lab>& w_labs, double light, double theta_T, double ratio_T, double num2dis_T, Indices& indice)
{
	if (!indice.empty())
		indice.clear();
	double blab_num, wlab_num;
	double temp;
	cv::minMaxIdx(b_label, &temp, &blab_num);
	cv::minMaxIdx(w_label, &temp, &wlab_num);
	int bnum = (int)blab_num;
	int wnum = (int)wlab_num;
	//std::vector<Lab> b_labs(bnum);//��ɫ��ǩ
	//std::vector<Lab> w_labs(wnum);//��ɫ��ǩ
	b_labs.resize(bnum);
	w_labs.resize(wnum);

	//�����ɫ��������
	InnerLabel(b_label, b_labs, bnum);


	//�����ɫ��������
	InnerLabel(w_label, w_labs, wnum);

	//��ʼ����ƥ��

	for (int i = 0; i < bnum; i++)
	{
		for (int j = 0; j < wnum; j++)
		{
			Lab& b_lab = b_labs[i];
			Lab& w_lab = w_labs[j];
			InnerMatch(b_lab, w_lab, light, theta_T, ratio_T, num2dis_T, b_label, w_label, indice);

		}
	}
}

void MoonImageProcessing::InnerLabel(cv::Mat &label, std::vector<Lab> &labs, int bnum)
{
	for (int i = 0; i < label.rows; ++i)
	{
		int* data_curRow = label.ptr<int>(i);
		for (int j = 0; j < label.cols; ++j)
		{
			int lab = data_curRow[j];
			if (lab >= 1)
			{
				labs[lab - 1].center.x += (double)j;
				labs[lab - 1].center.y += (double)i;
				labs[lab - 1].count++;
				MoonPoint point;
				point.x = (double)j;
				point.y = (double)i;
				labs[lab - 1].cloud.push_back(point);
			}
		}
	}

	for (int i = 0; i < bnum; ++i)
	{
		if (labs[i].count>0)
		{
			labs[i].center.x /= labs[i].count;
			labs[i].center.y /= labs[i].count;
		}
	}
}

void MoonImageProcessing::InnerMatch(Lab &b_lab, Lab &w_lab, double light, double theta_T, double ratio_T, double num2dis_T, const cv::Mat &b_label, const cv::Mat &w_label, Indices &indice)
{
	//���ڱ�Ե��ȡʱ�İ˷�������
	static int dx[] = { -1, -1, 0, 1, 1, 1, 0, -1 };	//X��
	static int dy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };	//y��
	if (!b_lab.matched&&!w_lab.matched&&b_lab.count > 1 && w_lab.count > 1)
	{
		double dis = sqrt(pow(b_lab.center.x - w_lab.center.x, 2) +
			pow(b_lab.center.y - w_lab.center.y, 2));
		double direction = 180.0*atan2((w_lab.center.y - b_lab.center.y),
			(w_lab.center.x - b_lab.center.x)) / CV_PI;
		if (direction < 0)
			direction += 360.0;
		double theta = fabs(direction - light);
		double num2dis = (b_lab.count + w_lab.count) / (CV_PI*dis*dis);
		double n1 = b_lab.count;
		double n2 = w_lab.count;
		double ratio = n1>n2 ? n1 / n2 : n2 / n1;

		MoonPoint center_point;
		center_point.x = (b_lab.center.x + w_lab.center.x) / 2;
		center_point.y = (b_lab.center.y + w_lab.center.y) / 2;

		if (theta < theta_T&&ratio<ratio_T&&num2dis>num2dis_T)
		{
			MoonPointCloud ptcloud;


			//��ɫ��ǩ����б�Ե����ж�
			isEdge(b_lab, b_label, dx, dy, center_point, dis, ptcloud);


			//��ɫ������б�Ե����ж�
			isEdge(w_lab, w_label, dx, dy, center_point, dis, ptcloud);

			b_lab.matched = true;
			w_lab.matched = true;
			indice.push_back(ptcloud);
		}
	}
}

void MoonImageProcessing::isEdge(const Lab &lab, const cv::Mat &label, int * dx, int * dy, MoonPoint &center_point, double dis, MoonPointCloud &ptcloud)
{
	for (int k = 0; k < lab.cloud.size(); k++)
	{
		int row = (int)(lab.cloud[k].y);
		int col = (int)(lab.cloud[k].x);
		MoonPoint point;
		point.x = (double)col;
		point.y = (double)row;
		const int* data = label.ptr<int>(row);
		bool edge(false);//��Ե��״̬�ж�
		for (int d = 0; d < 8; d++)
		{
			int xx = col + dx[d];
			int yy = row + dy[d];
			if (xx>0 && xx<label.cols&&yy>0 && yy < label.rows)
			{
				const int* ndata = label.ptr<int>(yy);
				if (ndata[xx] != data[col])
				{
					edge = true;
					break;
				}
			}
		}

		//�㵽��ǩ�����ĵľ���
		double d2center = sqrt(pow(point.x - center_point.x, 2) + pow(point.y - center_point.y, 2));
		if (edge&&d2center >= dis / 2)
		{
			ptcloud.push_back(point);
		}
	}
}

void MoonImageProcessing::_adaptiveFindThreshold(CvMat *dx, CvMat *dy, double *low, double *high)
{
	CvSize size;
	IplImage *imge = 0;
	int i, j;
	CvHistogram *hist;
	int hist_size = 255;
	float range_0[] = { 0, 256 };
	float* ranges[] = { range_0 };
	double PercentOfPixelsNotEdges = 0.7;
	size = cvGetSize(dx);
	imge = cvCreateImage(size, IPL_DEPTH_32F, 1);
	// �����Ե��ǿ��, ������ͼ����                                          
	float maxv = 0;
	for (i = 0; i < size.height; i++)
	{
		const short* _dx = (short*)(dx->data.ptr + dx->step*i);
		const short* _dy = (short*)(dy->data.ptr + dy->step*i);
		float* _image = (float *)(imge->imageData + imge->widthStep*i);
		for (j = 0; j < size.width; j++)
		{
			_image[j] = (float)(abs(_dx[j]) + abs(_dy[j]));
			maxv = maxv < _image[j] ? _image[j] : maxv;

		}
	}
	if (maxv == 0){
		*high = 0;
		*low = 0;
		cvReleaseImage(&imge);
		return;
	}

	// ����ֱ��ͼ                                                            
	range_0[1] = maxv;
	hist_size = (int)(hist_size > maxv ? maxv : hist_size);
	hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	cvCalcHist(&imge, hist, 0, NULL);
	int total = (int)(size.height * size.width * PercentOfPixelsNotEdges);
	float sum = 0;
	int icount = hist->mat.dim[0].size;

	float *h = (float*)cvPtr1D(hist->bins, 0);
	for (i = 0; i < icount; i++)
	{
		sum += h[i];
		if (sum > total)
			break;
	}
	// ����ߵ�����                                                          
	*high = (i + 1) * maxv / hist_size;
	*low = *high * 0.4;
	cvReleaseImage(&imge);
	cvReleaseHist(&hist);
}

bool MoonImageProcessing::circleIsInImage(const IplImage* src, Crater crater)
{
	double lx, rx, uy, dy;
	lx = crater.picCenter.x - crater.picRadius;
	rx = crater.picCenter.x + crater.picRadius;
	uy = crater.picCenter.y - crater.picRadius;
	dy = crater.picCenter.y + crater.picRadius;
	if (lx > 0 && rx<src->width&&uy>0 && dy < src->height)
	{
		return true;
	}
	return false;
}

//ֱ��ͼ�ָ�
void MoonImageProcessing::segByHistogram(const IplImage* src, IplImage* dst, int low, int high)
{
	for (int i = 0; i < src->height; i++)
	{
		for (int j = 0; j<src->width; j++)
		{
			uchar *pvalue = (uchar*)(src->imageData + i*src->widthStep + j);
			uchar *pdst = (uchar*)(dst->imageData + i*dst->widthStep + j);
			if (*pvalue >= 0 && *pvalue <= low)
				*pdst = 0;
			else if (*pvalue>low&&*pvalue < high)
				*pdst = 128;
			else
				*pdst = 255;
		}
	}
}

void MoonImageProcessing::adaptiveFindThreshold(const CvArr* image, double *low, double *high, int aperture_size /*= 3*/)
{
	cv::Mat src = cv::cvarrToMat(image);
	const int cn = src.channels();
	cv::Mat dx(src.rows, src.cols, CV_16SC(cn));
	cv::Mat dy(src.rows, src.cols, CV_16SC(cn));

	cv::Sobel(src, dx, CV_16S, 1, 0, aperture_size, 1, 0, cv::BORDER_REPLICATE);
	cv::Sobel(src, dy, CV_16S, 0, 1, aperture_size, 1, 0, cv::BORDER_REPLICATE);

	CvMat _dx = dx, _dy = dy;
	_adaptiveFindThreshold(&_dx, &_dy, low, high);
}
