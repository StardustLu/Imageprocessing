#pragma once
#include "DataManage.h"


class OtsuSeg
{
public:
	OtsuSeg(DataManage* td, Image* ti) :tmanager(td), img(ti) {}
	~OtsuSeg() {}

	bool initialize();																			//初始，主要时初始化statistic对象
	bool calThreshold(int band_1 = -1);															//计算各波段阈值
	vector<int> getThreshold() { return threshold; }
	int getThreshold(int band_1);
	Image* splitit(string& path, int band_1 = -1);												//分割影像

private:

	double calPartVar(int band_1, int thres);													//计算当前阈值的类间方差

	DataManage* tmanager;
	Image* img;
	Statistic* sta;
	vector<vector<int>>* staTable;

	vector<int> threshold;																		//记录各波段阈值
	vector<double> var_class;																	//记录各波段方差
};

