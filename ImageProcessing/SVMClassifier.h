#pragma once
#include "SVMBasic.h"


class SVMClassifier
{
public:
	SVMClassifier(DataManage*, Image*);
	~SVMClassifier();

	bool getSample(vector<vector<VectorXd>>*);						//获得 训练样本图像 以一个vector传入，vector的值为训练样本的图像的指针,
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
	Image* _resultImage;											//只需要记录该位置是第几类
	DataManage* manager;

	MatrixXd confusion_mat;

};


