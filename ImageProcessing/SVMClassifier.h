#pragma once
#include "SVMBasic.h"


class SVMClassifier
{
public:
	SVMClassifier(DataManage*, Image*);
	~SVMClassifier();

	bool getSample(vector<vector<VectorXd>>*);						//��� ѵ������ͼ�� ��һ��vector���룬vector��ֵΪѵ��������ͼ���ָ��,
	//bool getImage(Image*);
	Image* classify(string&);
	Image* getresult();
	bool train(KernelFunction, int thec = 1, int thegamma = 1 / double(100000), int thed = 3);

	MatrixXd* getConfusionMatrix(vector<vector<VectorXd>>*);

private:
	int getkind(VectorXd&);
	int getkind(int, int, VectorXd&, bool block = false);

	int types;
	vector<vector<SVMBasic*>> classifer_Matrix;

	vector<vector<VectorXd>>* _sample;
	vector<vector<VectorXd>>* _test;

	Image* _ImageTobeClassified;
	Image* _resultImage;											//ֻ��Ҫ��¼��λ���ǵڼ���
	DataManage* manager;

	MatrixXd confusion_mat;

};


