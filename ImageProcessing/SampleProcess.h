#pragma once
#include "DataManage.h"
#include <iostream>

class SampleProcess
{
public:
	SampleProcess(vector<vector<Image>>*);
	~SampleProcess() {}

	vector<vector<VectorXd>>* getSample(double per = 0.5);
	vector<vector<VectorXd>>* getTest(double per = 0.5);

	vector<vector<VectorXd>>* img2vec();					//������ת��Ϊ�������ɽ��ܵ�����
	MatrixXd* roiJudge();									//���������ɷ���� JM ����

private:
	// ��������������Ӷ�ά��̬�ֲ�  p_i~N(\mu_i,\Sigma_i)
	// ��ô ����� Bhattacharyya distance Ϊ��
	// D_B=\frac{1}{8} (\mu_1-\mu_2)^T \Sigma^{-1} (\mu_1-,u_2)+\frac{1}{2} ln(\frac{ det\Sigma }{ \sqrt{det \Sigma_1 det\Sigma_2}}  
	// \Sigma=\frac{\Sigma_1+\Sigma_2}{2}
	// �� JM ����Ϊ��
	// JM_cd=\sqrt{2(1-e^{-D_cd}}

	bool splitSample(double per);

	bool calMean();
	bool calVar();
	bool calCovar(int type_1, int band_1, int band_2);
	bool calBaBhattacharyya();

	int feature;
	int types;
	bool has_trans;
	bool has_split;

	vector<vector<Image>>* raw;
	vector<vector<VectorXd>> sample;
	vector<vector<VectorXd>> train_sample;
	vector<vector<VectorXd>> test_sample;

	vector<VectorXd> mean_vec;
	vector<MatrixXd> covar_mat;
	MatrixXd JM_mat;

};

