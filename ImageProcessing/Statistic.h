#pragma once
#include "Image.h"
#include <cmath>
/*
*	ÿһ�� Statistic ����ֻͳ��Ψһ��Ӱ��
*	��һ�� Statistic �����һ�� Image ����������������ͬ
*
*/

class Statistic
{
public:
	Statistic(Image*);
	~Statistic() {}

	bool initialize();

	double min(int band);
	double max(int band);
	double mean(int band);
	double mid(int band);
	double mode(int band);
	double samplevariance(int band);
	double covariance(int band_1, int band_2);

	vector<double>* minVec();
	vector<double>* maxVec();
	vector<double>* meanVec();
	vector<double>* midVec();
	vector<double>* modeVec();
	vector<vector<int>>* numVec();

	MatrixXd* coVarMat();
	MatrixXd* correlationMat();

private:


	bool staTable();
	bool staFloat();
	bool staCoMat();
	bool is_sta;
	bool is_staco;

	bool isfloat;
	//	data:
	Image* raw;

	vector<double>	min_table;							//���ڴ��� ��������Сֵ
	vector<double>	max_table;							//���ڴ��� ���������ֵ
	vector<double>	mean_table;							//���ڴ��� �����ξ�ֵ
	vector<double>	mid_table;							//���ڴ��� ��������ֵ
	vector<double>	mode_table;							//���ڴ��� ����������

	MatrixXd	covar_mat;								//���ڴ��� Ӱ�񲨶μ�Э����
	MatrixXd	correlation_mat;						//���ڴ��� Ӱ�񲨶μ����ϵ��


	vector<vector<int>>	num_table;						//���ڴ��� Ӱ���������Ԫͳ�Ʊ�

};

