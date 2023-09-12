#pragma once
#include "DataManage.h"
#include <opencv2/core.hpp>


class PCATrans
{
public:
	PCATrans(DataManage* tm, Image* ti) :tmanager(tm), img(ti), sta(nullptr) {}
	~PCATrans() {}

	bool initalize();

	Image* PCAforward(string&);											//PCA正变换
	Image* PCAinverse(string&);											//PCA逆变换

	bool calTransMat();											//计算PCA变换矩阵
	MatrixXd* getTansMat() { return &forward_Mat; }

private:


	DataManage* tmanager;
	Image* img;													//待变换影像
	Statistic* sta;												//待变换影像的统计类

	Image* img_forward;											//变换影像

	MatrixXd forward_Mat;
	MatrixXd inverse_Mat;

};

