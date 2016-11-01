/*
*撞击坑，默认为圆形
*罗中飞，2015.12
*/

#pragma once

//中心
struct Center
{
	double x;
	double y;
};

//撞击坑的类型
enum TYPE{Simple,FlatComplex,UpComplex};

class Crater
{
public:
	Crater();
	~Crater();

	Center picCenter;//撞击坑在图像中的中心位置
	double picRadius;//撞击坑在图像中的半径大小
	Center demCenter;//撞击坑在DEM中的中心位置
	double demRadius;//撞击坑在DEM中的半径大小
	TYPE   type;     //撞击坑的类型
};


typedef std::vector<Crater> CraterSet;

