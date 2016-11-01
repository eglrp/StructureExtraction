/*
*ײ���ӣ�Ĭ��ΪԲ��
*���зɣ�2015.12
*/

#pragma once

//����
struct Center
{
	double x;
	double y;
};

//ײ���ӵ�����
enum TYPE{Simple,FlatComplex,UpComplex};

class Crater
{
public:
	Crater();
	~Crater();

	Center picCenter;//ײ������ͼ���е�����λ��
	double picRadius;//ײ������ͼ���еİ뾶��С
	Center demCenter;//ײ������DEM�е�����λ��
	double demRadius;//ײ������DEM�еİ뾶��С
	TYPE   type;     //ײ���ӵ�����
};


typedef std::vector<Crater> CraterSet;

