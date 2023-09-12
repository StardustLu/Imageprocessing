#pragma once
#include "DataManage.h"
#include <cmath>
#include <ctime>
#include <random>
#include <chrono>
#include <iostream>

enum  Solver
{
	sgd, adam, sgdm
};

class BPNet
{
public:
	BPNet(DataManage* tm, Image* ti) :manager(tm), img(ti) {}
	~BPNet() {}

	bool initialize(int therow, int thecol, double theeta = 0.1, int max = 1000, Solver thesolver = sgd, int loss_iter = 2);				// 初始化网络

	bool train(vector<vector<VectorXd>>* thesample);
	Image* classify(string&);
	MatrixXd* getConfusionMatrix(vector<vector<VectorXd>>* thesample);																		//评价分类精度，返回一个混淆矩阵和其他精度指标

protected:

	vector<vector<VectorXd>>* sample;						// 记录训练样本
	vector<vector<VectorXd>>* test;							// 记录测试样本

	bool has_test;

	DataManage* manager;
	Image* img;												// 待分类影像
	Image* result;											// 分类结果

	Statistic* sta;
	vector<double>* mean_table;
	MatrixXd* cor_mat;

private:


	bool initialize_io();
	bool randomWeight();									// 获得样本向量维度/网络大小后，随机生成权
	double normDis(double var = 1);							// 产生正态分布随机数
	bool equalize(VectorXd&);								// 将向量归一化
	bool calSampleMean();
	bool calSampleVar();

	VectorXd mean_vec;
	VectorXd var_vec;
	Solver thesolver;

	int calForward(VectorXd&);								// 计算一次从输入层到输出层的过程(第a类第b个样本向量)
	bool activation(int k);									// 计算第 K 层输出向量经过 Sigmoid 函数变换后的输入向量		f(x)=\frac{1}{1+e^{-x}}
	bool activation(VectorXd&);
	VectorXd derivative(VectorXd&);

	bool calBackward(int);
	bool calDelta(VectorXd&);
	bool updateWeight();

	bool calLoss(VectorXd&);

	// FOR ADAM
	vector<MatrixXd> m_t;
	vector<MatrixXd> v_t;
	double beta_1, beta_2;
	double epsilon;


	// FOR NEURAUL NETWORK
	int features;											// 样本特征维度
	int net_col, net_row;									// 记录神经网络隐藏层层数及层神经元个数
	double eta;												// 学习率
	int max_iter;											// 记录迭代上限
	double loss;											// 记录误差
	double loss_threshold;									// 记录误差阈值
	int currentEpoch;

	vector<double> accurancy;								// 记录每一个epoch训练的精度

	VectorXd out;											//整个网络的最终输出
	VectorXd true_delta;
	vector<VectorXd> input;									//由output激活
	vector<VectorXd> output;								//线性求和相加
	vector<MatrixXd> weight;
	vector<MatrixXd> weight_delta;
	vector<VectorXd> bias;
	vector<VectorXd> delta;

	MatrixXd confusion_mat;

};

