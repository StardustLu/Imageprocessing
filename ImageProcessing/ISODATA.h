/*---------------------------------------------ISODATA算法说明------------------------------------------------------------------
*	（公式以Latex格式书写）
*	ISODATA算法来源于 K-means算法，是一种无监督分类的聚类算法
*	ISODATA 需要输入四个参数：①预期聚类中心 k_0 ②每个类最少的样本个数 N_min ③最大方差 \sigma_max ④最小距离 d_min ⑤初始样本点个数 k
*	具体步骤如下：
*	①随机生成 k 个样本中心点
*	②按最小距离自动聚成 k 类
*	③重新计算样本中心点，计算每一类的方差
*	④如果某一类数量小于阈值，该类与最邻近类进行合并
*	⑤如果方差超过阈值，执行分裂操作。新的类别样本中心点为： m_1=m_0-\sigma_0   m_2=m_0+ \sigma_0, 再对原本属于 m_0 的样本按照 m_1 和 m_2 据类
*	⑥如果两类间距离小于阈值，则执行合并操作，样本中心点为： m_0= \frac{N_1*m_1+N_2*m_2}{N_1+N_2}, 并重新计算方差
*	⑦如果类别数小于	\frac{k_0}{2}，执行合并操作，合并最相近的两类
*	⑧如果类别数大于	2k_0 ，执行分裂操作，分裂方差最大的类
*	⑨如果达到 最大迭代次数 或 ④⑤⑥⑦⑧均为发生 ，则算法结束
*
*	说明:
*	由于本程序采用位置序号标记类别,因此当两个类别合并,需要将一个类别注册为空
*	current_k为当前存在的类别数,current_order为历史累计存在的类别数
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

	bool randomStart();								//产生 K_0 个起始向量
	bool cluster();									//按照已有的 聚类中心 进行据类
	//bool calnewMean();								//计算新的聚类中心
	bool calnewVar();								//计算新的方差
	bool calMeanDis();								//用于计算所有样本中心之间的距离

	int getkind(vector<int>&);						//用于标记样本点属于哪一个聚类

	bool underN_min();								//判断是否由小于 N_min的聚类
	bool judgeSigma();								//判断是否有超过 \sigma 的聚类
	bool judgemindis();								//判断是否有小于 mindis的聚类
	bool judgeUsed(int);							//判断该序号有没有被使用且删除

	bool findMaxSigma(int&);						//找到 \sigma最大的聚类
	bool findMinDis(int&, int&);					//找到 距离最小的两个聚类(两个聚类均未知)

	bool calmindis(int& origin, int& next);			//已知一个类别,求离此类别距离最小的类别
	bool calMaxVar();
	double calAllMeanDis();

	bool mergeClass(int, int);						//将两个类别合并为一个类别
	bool splitClass(int);							//拆分第 k 类

	bool defineColorTable();

	int feature;									//特征维度
	int max_times;									//最大循环次数
	int current_K;									//标记当前类别数
	int current_order;								//用以标记当前使用的序号，与current_k相比，只增不减
	int k_0, N_min, k;								//k_0 预期聚类中心数； N_min最少样本数，小于最小样本数则进行合并操作;k初始聚类中心数
	double sigma_max, min_dis;						//sigma_max最大方差，当超过最大方差则执行分裂； min_dis小于最小距离则进行合并


	vector<vector<double>> mean_vec;				//用于储存各类均值向量
	vector<vector<double>> var_vec;					//用于储存各类协方差
	vector<double> maxvar_vec;						//储存每一类的最大方差，最大方差：同一类别中，各特征向量的最大方差
	vector<unsigned long> amount_vec;				//用于储存每一类的个数
	MatrixXd all_dis;								//用于储存类与类之间的距离信息
	vector<double> meandis_vec;						//用于储存各类的平均距离

	vector<int> used_num;							//记录使用过且被删除的序号

	vector<int> underN_min_vec;						//用于储存小于 N_min 的聚类
	vector<int> overSigma_vec;						//用于储存大于 \sigma的聚类
	vector<vector<int>> undermindis_vec;			//用于储存小于 最小距离的距离

	DataManage* tmanager;
	Image* _rawImage;
	Image* _result;
	bool first_open;
};

