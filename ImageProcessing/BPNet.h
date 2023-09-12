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

	bool initialize(int therow, int thecol, double theeta = 0.1, int max = 1000, Solver thesolver = sgd, int loss_iter = 2);				// ��ʼ������

	bool train(vector<vector<VectorXd>>* thesample);
	Image* classify(string&);
	MatrixXd* getConfusionMatrix(vector<vector<VectorXd>>* thesample);																		//���۷��ྫ�ȣ�����һ�������������������ָ��

protected:

	vector<vector<VectorXd>>* sample;						// ��¼ѵ������
	vector<vector<VectorXd>>* test;							// ��¼��������

	bool has_test;

	DataManage* manager;
	Image* img;												// ������Ӱ��
	Image* result;											// ������

	Statistic* sta;
	vector<double>* mean_table;
	MatrixXd* cor_mat;

private:


	bool initialize_io();
	bool randomWeight();									// �����������ά��/�����С���������Ȩ
	double normDis(double var = 1);							// ������̬�ֲ������
	bool equalize(VectorXd&);								// ��������һ��
	bool calSampleMean();
	bool calSampleVar();

	VectorXd mean_vec;
	VectorXd var_vec;
	Solver thesolver;

	int calForward(VectorXd&);								// ����һ�δ�����㵽�����Ĺ���(��a���b����������)
	bool activation(int k);									// ����� K ������������� Sigmoid �����任�����������		f(x)=\frac{1}{1+e^{-x}}
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
	int features;											// ��������ά��
	int net_col, net_row;									// ��¼���������ز����������Ԫ����
	double eta;												// ѧϰ��
	int max_iter;											// ��¼��������
	double loss;											// ��¼���
	double loss_threshold;									// ��¼�����ֵ
	int currentEpoch;

	vector<double> accurancy;								// ��¼ÿһ��epochѵ���ľ���

	VectorXd out;											//����������������
	VectorXd true_delta;
	vector<VectorXd> input;									//��output����
	vector<VectorXd> output;								//����������
	vector<MatrixXd> weight;
	vector<MatrixXd> weight_delta;
	vector<VectorXd> bias;
	vector<VectorXd> delta;

	MatrixXd confusion_mat;

};

