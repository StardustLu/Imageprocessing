/*---------------------------------------------SVM-------------------------------------------
*	采用软间隔最大化,利用SMO算法求解系数
*	核函数包括:Linear,Polynomial,RBF,Sigmond(仅实现Linear和RBF)
*	需要对其调参: C = 1
*				γ = 1/特征维度 (针对RBF)
*/

#pragma once
#include "Image.h"
#include "DataManage.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

enum KernelFunction															//采取的核函数类型
{
	Linear, Polynomial, RBF, Sigmoid											//RBF即高斯核函数
};

struct Sample
{
	VectorXd x;														//样本列向量
	int y;																	//样本类别，取 -1 或 +1
	Sample(const Sample&);
	Sample() {}
};


class SVMBasic
{
public:
	SVMBasic(DataManage*, Image*);
	~SVMBasic();

	bool getSample(vector<VectorXd>*, vector<VectorXd>*);						//获得 训练样本图像 以一个vector传入，vector的值为训练样本的图像的指针,
	bool getPath(string&);
	Image* classify();
	Image* getresult();
	bool train(KernelFunction, int thec = 1, int thegamma = 1 / double(100000), int thed = 3);												//训练参数，以何种核函数进行训练

	int getkind(int, int, VectorXd&, bool block = false);
	int getkind(VectorXd&);

private:

	double calKernel(VectorXd&, int);										//用于判别时的核函数计算
	int calKind(int, int, VectorXd&, bool block = false);										//计算像元的类别
	int calKind(VectorXd&);

	/*--------------------SMO--------------------------*/
	bool calAlpha2Un();														//计算未经剪切的 α2
	bool clipAlpha2();														//剪切 α2
	bool calAlpha1();														//计算 α1

	bool findAlpha2();														//给定一个 α1 ，按照SMO最优化方法找到 α2

	double calEi(int);														//计算 Ei
	double calEta(int, int);												//计算 η
	bool calb();															//计算新的b
	bool calH();
	bool calL();

	double kernelTrick(int, int);											//采用的核函数

	int min(vector<double>&);												//返回 vector 中最 小 的元素的位置
	int max(vector<double>&);												//返回 vector 中最 大 的元素的位置

	double b;																//判别函数的 b
	double c;																//α的上届
	int d;																	//用于多项式核函数的次方
	double gamma;															//用于高斯核函数
	double alpha_1old, alpha_2old;

	int current_1, current_2;												//当前的 α1 和 α2 的位置
	double H, L;
	int kernelType;

	vector<double> alpha;													//储存所有的 α 参数
	vector<double> E;														//储存所有的E
	vector<int> supportVec;													//储存supportVector的位置

	int features;															//样本特征维度

	vector<Sample> _thesample;												//将二类样本转换为一维数组

	Image* _ImageTobeClassified;
	Image* _resultImage;													//只需要记录该位置是第几类
	DataManage* manager;
};

