#pragma once
#include "Image.h"
#include <cmath>
/*
*	每一个 Statistic 对象只统计唯一的影像
*	即一个 Statistic 对象绑定一个 Image 对象，其生命周期相同
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

	vector<double>	min_table;							//用于储存 各波段最小值
	vector<double>	max_table;							//用于储存 各波段最大值
	vector<double>	mean_table;							//用于储存 各波段均值
	vector<double>	mid_table;							//用于储存 各波段中值
	vector<double>	mode_table;							//用于储存 各波段众数

	MatrixXd	covar_mat;								//用于储存 影像波段间协方差
	MatrixXd	correlation_mat;						//用于储存 影像波段间相关系数


	vector<vector<int>>	num_table;						//用于储存 影像各波段像元统计表

};

