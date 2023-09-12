/*---------------------------------------------SVM-------------------------------------------
*	�����������,����SMO�㷨���ϵ��
*	�˺�������:Linear,Polynomial,RBF,Sigmond(��ʵ��Linear��RBF)
*	��Ҫ�������: C = 1
*				�� = 1/����ά�� (���RBF)
*/

#pragma once
#include "Image.h"
#include "DataManage.h"
#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

enum KernelFunction															//��ȡ�ĺ˺�������
{
	Linear, Polynomial, RBF, Sigmoid											//RBF����˹�˺���
};

struct Sample
{
	VectorXd x;														//����������
	int y;																	//�������ȡ -1 �� +1
	Sample(const Sample&);
	Sample() {}
};


class SVMBasic
{
public:
	SVMBasic(DataManage*, Image*);
	~SVMBasic();

	bool getSample(vector<VectorXd>*, vector<VectorXd>*);						//��� ѵ������ͼ�� ��һ��vector���룬vector��ֵΪѵ��������ͼ���ָ��,
	bool getPath(string&);
	Image* classify();
	Image* getresult();
	bool train(KernelFunction, int thec = 1, int thegamma = 1 / double(100000), int thed = 3);												//ѵ���������Ժ��ֺ˺�������ѵ��

	int getkind(int, int, VectorXd&, bool block = false);
	int getkind(VectorXd&);

private:

	double calKernel(VectorXd&, int);										//�����б�ʱ�ĺ˺�������
	int calKind(int, int, VectorXd&, bool block = false);										//������Ԫ�����
	int calKind(VectorXd&);

	/*--------------------SMO--------------------------*/
	bool calAlpha2Un();														//����δ�����е� ��2
	bool clipAlpha2();														//���� ��2
	bool calAlpha1();														//���� ��1

	bool findAlpha2();														//����һ�� ��1 ������SMO���Ż������ҵ� ��2

	double calEi(int);														//���� Ei
	double calEta(int, int);												//���� ��
	bool calb();															//�����µ�b
	bool calH();
	bool calL();

	double kernelTrick(int, int);											//���õĺ˺���

	int min(vector<double>&);												//���� vector ���� С ��Ԫ�ص�λ��
	int max(vector<double>&);												//���� vector ���� �� ��Ԫ�ص�λ��

	double b;																//�б����� b
	double c;																//�����Ͻ�
	int d;																	//���ڶ���ʽ�˺����Ĵη�
	double gamma;															//���ڸ�˹�˺���
	double alpha_1old, alpha_2old;

	int current_1, current_2;												//��ǰ�� ��1 �� ��2 ��λ��
	double H, L;
	int kernelType;

	vector<double> alpha;													//�������е� �� ����
	vector<double> E;														//�������е�E
	vector<int> supportVec;													//����supportVector��λ��

	int features;															//��������ά��

	vector<Sample> _thesample;												//����������ת��Ϊһά����

	Image* _ImageTobeClassified;
	Image* _resultImage;													//ֻ��Ҫ��¼��λ���ǵڼ���
	DataManage* manager;
};

