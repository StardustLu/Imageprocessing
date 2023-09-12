#pragma once
#include "DataManage.h"


class OtsuSeg
{
public:
	OtsuSeg(DataManage* td, Image* ti) :tmanager(td), img(ti) {}
	~OtsuSeg() {}

	bool initialize();																			//��ʼ����Ҫʱ��ʼ��statistic����
	bool calThreshold(int band_1 = -1);															//�����������ֵ
	vector<int> getThreshold() { return threshold; }
	int getThreshold(int band_1);
	Image* splitit(string& path, int band_1 = -1);												//�ָ�Ӱ��

private:

	double calPartVar(int band_1, int thres);													//���㵱ǰ��ֵ����䷽��

	DataManage* tmanager;
	Image* img;
	Statistic* sta;
	vector<vector<int>>* staTable;

	vector<int> threshold;																		//��¼��������ֵ
	vector<double> var_class;																	//��¼�����η���
};

