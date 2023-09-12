#pragma once
#include "DataManage.h"
#include <opencv2/core.hpp>


class PCATrans
{
public:
	PCATrans(DataManage* tm, Image* ti) :tmanager(tm), img(ti), sta(nullptr) {}
	~PCATrans() {}

	bool initalize();

	Image* PCAforward(string&);											//PCA���任
	Image* PCAinverse(string&);											//PCA��任

	bool calTransMat();											//����PCA�任����
	MatrixXd* getTansMat() { return &forward_Mat; }

private:


	DataManage* tmanager;
	Image* img;													//���任Ӱ��
	Statistic* sta;												//���任Ӱ���ͳ����

	Image* img_forward;											//�任Ӱ��

	MatrixXd forward_Mat;
	MatrixXd inverse_Mat;

};

