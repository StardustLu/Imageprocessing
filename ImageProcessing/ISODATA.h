/*---------------------------------------------ISODATA�㷨˵��------------------------------------------------------------------
*	����ʽ��Latex��ʽ��д��
*	ISODATA�㷨��Դ�� K-means�㷨����һ���޼ල����ľ����㷨
*	ISODATA ��Ҫ�����ĸ���������Ԥ�ھ������� k_0 ��ÿ�������ٵ��������� N_min ����󷽲� \sigma_max ����С���� d_min �ݳ�ʼ��������� k
*	���岽�����£�
*	��������� k ���������ĵ�
*	�ڰ���С�����Զ��۳� k ��
*	�����¼����������ĵ㣬����ÿһ��ķ���
*	�����ĳһ������С����ֵ�����������ڽ�����кϲ�
*	������������ֵ��ִ�з��Ѳ������µ�����������ĵ�Ϊ�� m_1=m_0-\sigma_0   m_2=m_0+ \sigma_0, �ٶ�ԭ������ m_0 ���������� m_1 �� m_2 ����
*	�������������С����ֵ����ִ�кϲ��������������ĵ�Ϊ�� m_0= \frac{N_1*m_1+N_2*m_2}{N_1+N_2}, �����¼��㷽��
*	����������С��	\frac{k_0}{2}��ִ�кϲ��������ϲ������������
*	��������������	2k_0 ��ִ�з��Ѳ��������ѷ���������
*	������ﵽ ���������� �� �ܢݢޢߢ��Ϊ���� �����㷨����
*
*	˵��:
*	���ڱ��������λ����ű�����,��˵��������ϲ�,��Ҫ��һ�����ע��Ϊ��
*	current_kΪ��ǰ���ڵ������,current_orderΪ��ʷ�ۼƴ��ڵ������
*
*----------------------------------------------------------------------------------------------------------------------------*/

#pragma once
#include "Image.h"
#include "DataManage.h"
#include <vector>
#include <ctime>
using namespace std;

class ISODATA
{
public:
	ISODATA(DataManage*, Image*);
	~ISODATA();

	Image* classify(int thek, int thek_0, int theN_min, double thesigma_max,int mindis,int max_iter=10);
	bool initialize(string&);

	//leo::Matrix<short>* getresult();

private:

	bool randomStart();								//���� K_0 ����ʼ����
	bool cluster();									//�������е� �������� ���о���
	//bool calnewMean();								//�����µľ�������
	bool calnewVar();								//�����µķ���
	bool calMeanDis();								//���ڼ���������������֮��ľ���

	int getkind(vector<int>&);						//���ڱ��������������һ������

	bool underN_min();								//�ж��Ƿ���С�� N_min�ľ���
	bool judgeSigma();								//�ж��Ƿ��г��� \sigma �ľ���
	bool judgemindis();								//�ж��Ƿ���С�� mindis�ľ���
	bool judgeUsed(int);							//�жϸ������û�б�ʹ����ɾ��

	bool findMaxSigma(int&);						//�ҵ� \sigma���ľ���
	bool findMinDis(int&, int&);					//�ҵ� ������С����������(���������δ֪)

	bool calmindis(int& origin, int& next);			//��֪һ�����,�������������С�����
	bool calMaxVar();
	double calAllMeanDis();

	bool mergeClass(int, int);						//���������ϲ�Ϊһ�����
	bool splitClass(int);							//��ֵ� k ��

	bool defineColorTable();

	int feature;									//����ά��
	int max_times;									//���ѭ������
	int current_K;									//��ǵ�ǰ�����
	int current_order;								//���Ա�ǵ�ǰʹ�õ���ţ���current_k��ȣ�ֻ������
	int k_0, N_min, k;								//k_0 Ԥ�ھ����������� N_min������������С����С����������кϲ�����;k��ʼ����������
	double sigma_max, min_dis;						//sigma_max��󷽲��������󷽲���ִ�з��ѣ� min_disС����С��������кϲ�


	vector<vector<double>> mean_vec;				//���ڴ�������ֵ����
	vector<vector<double>> var_vec;					//���ڴ������Э����
	vector<double> maxvar_vec;						//����ÿһ�����󷽲��󷽲ͬһ����У���������������󷽲�
	vector<unsigned long> amount_vec;				//���ڴ���ÿһ��ĸ���
	MatrixXd all_dis;								//���ڴ���������֮��ľ�����Ϣ
	vector<double> meandis_vec;						//���ڴ�������ƽ������

	vector<int> used_num;							//��¼ʹ�ù��ұ�ɾ�������

	vector<int> underN_min_vec;						//���ڴ���С�� N_min �ľ���
	vector<int> overSigma_vec;						//���ڴ������ \sigma�ľ���
	vector<vector<int>> undermindis_vec;			//���ڴ���С�� ��С����ľ���

	DataManage* tmanager;
	Image* _rawImage;
	Image* _result;
	bool first_open;
};

