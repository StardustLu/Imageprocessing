#include "ISODATA.h"

ISODATA::ISODATA(DataManage* tm, Image* theimage)
{
	tmanager = tm;
	max_times = 10;
	_rawImage = theimage;
	feature = _rawImage->getBand();
	first_open = true;
	_result = nullptr;

	srand(time(0));
}

ISODATA::~ISODATA()
{

}

Image* ISODATA::classify(int thek, int thek_0, int theN_min, double thesigma_max,int mindis,int max_iter)
{
	k = thek;
	current_K = thek;
	k_0 = thek_0;
	N_min = theN_min;
	sigma_max = thesigma_max * thesigma_max;
	//min_dis = (themin_dis*themin_dis)*feature;
	current_order = current_K;
	max_times = max_iter;

	bool minMergeit = false;
	bool splitit = false;
	bool mergeit = false;
	bool minmerge = false;
	bool maxsplit = false;

	int cycle_times = 0;
	int nextkind;



	//mean_vec.resize(current_order);
	amount_vec.resize(current_order);

	randomStart();

	while ((cycle_times < max_times) && (current_K != k_0))
	{
		minMergeit = true;														//����Ƿ����N_min����С����
		splitit = true;															//����Ƿ�����˷��Ѳ���
		mergeit = true;															//����Ƿ�����˺ϲ�����
		minmerge = true;														//����Ƿ���С�� k0/2 ���кϲ�
		maxsplit = true;														//����Ƿ������ 2k0 ���з���

		cluster();																//����
		//calnewMean();
		calnewVar();															//������������ά����������
		calMeanDis();															//��������ľ���


		if (underN_min())														//�ж��Ƿ����С�� ������ ��ֵ�����
		{
			for (int i = 0; i < underN_min_vec.size(); ++i)						//������ڣ����ÿһ��С�� ������ ��ֵ�����������������ϲ�	
			{
				calmindis(underN_min_vec[i], nextkind);							//�����뱾����������
				minMergeit = mergeClass(underN_min_vec[i], nextkind);			//������ϲ�
			}
			underN_min_vec.clear();												//�ϲ�֮�������ϲ�����С����ֵ�������ô�����С��N�ļ�¼
		}

		if ((current_K <= k_0 / 2) || current_K % 2 == 1)
		{
			calMaxVar();
			double themeandis = calAllMeanDis();
			for (int i = 0; i < current_order; ++i)
			{
				if (!judgeUsed(i))
				{
					if (maxvar_vec[i] > sigma_max)
					{
						if ((meandis_vec[i] > themeandis&& amount_vec[i] > 2 * (N_min + 1)) || (current_K <= k_0 / 2))
						{
							splitClass(i);
						}
					}
				}
			}
		}

		int a, b;
		findMinDis(a, b);
		mergeClass(a, b);

		++cycle_times;
	}

	defineColorTable();

	return _result;
}

bool ISODATA::initialize(string& opath)
{
	_result = tmanager->create();
	_result->create(1, _rawImage->getRow(), _rawImage->getCol(), _rawImage, opath);

	return true;
}



bool ISODATA::randomStart()
{
	int row, col, brow, bcol;
	vector<double> temp;
	temp.resize(_rawImage->getBand());
	//��Ӱ�������ѡ�񼸸���������Ϊ��������

	for (int i = 0; i < k; ++i)
	{
		brow = rand() % (_rawImage->getBlockRow());
		bcol = rand() % (_rawImage->getBlockCol());

		_rawImage->loadDatabyBlock(brow, bcol);
		//�����б�������һ��֮����Ϊ��һ�л���µ�ǰӰ���ĳ���
		row = rand() % (_rawImage->getEachBlockRow());
		col = rand() % (_rawImage->getEachBlockCol());

		for (int j = 0; j < _rawImage->getBand(); ++j)
		{
			temp[j] = _rawImage->atBlock(j, row, col);
			//cout << temp[j] << endl;
		}
		mean_vec.push_back(temp);

		_rawImage->clear();

	}

	return true;
}

bool ISODATA::cluster()
{
	int progress = 0;
	int lastprogress = -1;
	double t1, t2;
	vector<int> temp;
	temp.resize(feature);

	vector<vector<unsigned long long>> tempMean;
	tempMean.resize(current_order);

	amount_vec.clear();
	amount_vec.resize(current_order);

	for (int i = 0; i < current_order; ++i)										//�����������������ʼ��
	{
		amount_vec[i] = 0;														//����������������
		tempMean[i].resize(feature);

	}

	for (int i = 0; i < _rawImage->getBlockRow(); ++i)
	{
		for (int j = 0; j < _rawImage->getBlockCol(); ++j)
		{
			_rawImage->loadDatabyBlock(i, j);

			if (first_open) { _result->prepareDatabyBlock(i, j, 0); }
			else { _result->loadDatabyBlock(i, j, 0); }

			for (int a = 0; a < _rawImage->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < _rawImage->getEachBlockCol(); ++b)
				{
					for (int k = 0; k < _rawImage->getBand(); ++k)
					{
						temp[k] = _rawImage->atBlock(k, a, b);
					}

					_result->setBlock(0, a, b, getkind(temp));
					++amount_vec[_result->atBlock(0, a, b)];

					for (int r = 0; r < _rawImage->getBand(); ++r)
					{
						tempMean[_result->atBlock(0, a, b)][r] += temp[r];
					}

				}
			}
			_result->saveDatabyBlock(i, j, 0);
			_rawImage->clear();
			_result->clear();

		}
	}

	first_open = false;


	for (int i = 0; i < current_order; ++i)										//���Լ����ֵ
	{
		if (!judgeUsed(i))
		{
			for (int j = 0; j < feature; ++j)
			{
				t1 = amount_vec[i];
				t2 = tempMean[i][j];
				mean_vec[i][j] = t2 / t1;
			}
		}
		meandis_vec[i] /= amount_vec[i];
	}

	return true;
}

bool ISODATA::calnewVar()
{
	double temp;
	vector<vector<double>> temp_sum;
	temp_sum.resize(current_order);

	var_vec.clear();
	var_vec.resize(current_order);

	for (int i = 0; i < current_order; ++i)
	{
		temp_sum[i].resize(feature);
		var_vec[i].resize(feature);
	}

	for (int i = 0; i < _rawImage->getBlockRow(); ++i)
	{
		for (int j = 0; j < _rawImage->getBlockCol(); ++j)
		{

			_rawImage->loadDatabyBlock(i, j);
			_result->loadDatabyBlock(i, j, 0);
			for (int a = 0; a < _rawImage->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < _rawImage->getEachBlockCol(); ++b)
				{
					for (int k = 0; k < _rawImage->getBand(); ++k)
					{
						temp = _rawImage->atBlock(k, a, b) - mean_vec[(_result->atBlock(0, a, b))][k];
						temp_sum[(_result->atBlock(0, a, b))][k] = temp * temp;
					}
				}
			}
			_rawImage->clear();
			_result->clear();

		}
	}

	for (int i = 0; i < current_order; ++i)
	{
		for (int j = 0; j < feature; ++j)
		{
			if (!judgeUsed(i))
			{
				var_vec[i][j] = temp_sum[i][j] / (amount_vec[i] - 1);
			}
		}
	}

	return true;
}

bool ISODATA::calMeanDis()
{
	all_dis.resize(current_order, current_order);

	for (int i = 0; i < current_order; ++i)
	{
		for (int j = 0; j < current_order; ++j)
		{
			all_dis(i, j) = 0;
		}
	}


	for (int i = 0; i < current_order; ++i)
	{
		for (int j = 0; j < current_order; ++j)
		{
			if (judgeUsed(i) || judgeUsed(j))									//�� i��j ��Ϊ�Ѿ�ʹ�ù������
			{
				all_dis(i, j) = -1;
			}
			else
			{
				for (int k = 0; k < feature; ++k)
				{
					all_dis(i, j) += (mean_vec[i][k] - mean_vec[j][k]) * (mean_vec[i][k] - mean_vec[j][k]);
				}
			}
		}
	}

	return true;
}

int ISODATA::getkind(vector<int>& the)
{
	vector<double> dis;
	dis.resize(current_order);
	meandis_vec.resize(current_order);

	for (int i = 0; i < current_order; ++i)											//�������������������о���
	{
		dis[i] = 0;
		meandis_vec[i] = 0;
		if (!judgeUsed(i))
		{
			for (int k = 0; k < feature; ++k)
			{
				dis[i] += (mean_vec[i][k] - the[k]) * (mean_vec[i][k] - the[k]);
				meandis_vec[i] += dis[i];
			}
		}
	}

	double themin = 10000000000;
	int kind = 0;
	double t;

	for (int i = 0; i < current_order; ++i)											//�ҳ���С���룬����Ϊ����Ԫ���ڸ���С����
	{
		//t = dis[i];
		if ((dis[i] < themin) && (dis[i] > 0))
		{
			themin = dis[i];
			kind = i;
		}
	}

	return kind;
}

bool ISODATA::underN_min()
{
	underN_min_vec.clear();

	for (int i = 0; i < current_order; ++i)
	{
		if (!judgeUsed(i) && amount_vec[i] < N_min && (amount_vec[i] > 0))										//�ҳ������ڱ���
		{
			underN_min_vec.push_back(i);
		}
	}

	if (underN_min_vec.empty()) { return false; }

	return true;
}

bool ISODATA::judgeSigma()
{
	overSigma_vec.clear();
	//cout << "variance:" << endl;
	for (int i = 0; i < current_order; ++i)
	{
		if (!judgeUsed(i))
		{
			for (int j = 0; j < feature; ++j)
			{
				//cout << var_vec[i][j] << " ";
				if (var_vec[i][j] > sigma_max)
				{
					overSigma_vec.push_back(i);
				}
			}
			//cout << endl;
		}
	}
	//cout << endl;

	if (overSigma_vec.empty()) { return false; }

	return true;
}

bool ISODATA::judgemindis()
{
	undermindis_vec.clear();
	vector<int> temp;
	temp.resize(2);																	//��һ�� pair �����������
	//cout << "Distance:" << endl;
	for (int i = 0; i < current_order; ++i)
	{
		for (int j = i; j < current_order; ++j)
		{
			//cout << all_dis[i][j] << " ";
			if (judgeUsed(i) || judgeUsed(j)) { continue; }
			else
			{
				if ((all_dis(i, j) < min_dis) && (i != j))
				{
					temp[0] = i;
					temp[1] = j;
					undermindis_vec.push_back(temp);
				}
			}
		}
		//cout << endl;
	}
	//cout << endl;
	if (undermindis_vec.empty()) { return false; }

	return true;
}

bool ISODATA::judgeUsed(int kind)
{
	for (int i = 0; i < used_num.size(); ++i)
	{
		if (used_num[i] == kind) { return true; }
	}
	return false;
}

bool ISODATA::findMaxSigma(int& thekind)
{
	double themax = var_vec[0][0];
	thekind = 0;

	for (int i = 0; i < current_order; ++i)
	{
		if (!judgeUsed(i))
		{
			for (int j = 0; j < feature; ++j)
			{
				if (themax < var_vec[i][j])
				{
					themax = var_vec[i][j];
					thekind = i;
				}
			}
		}
	}

	return true;
}

bool ISODATA::findMinDis(int& origin, int& next)
{
	double themin = 10000000000;
	origin = 0;
	next = 0;

	for (int i = 0; i < current_order; ++i)
	{
		for (int j = i; j < current_order; ++j)
		{
			if ((all_dis(i, j) > 0) && (all_dis(i, j) < themin) && (i != j))
			{
				themin = all_dis(i, j);
				origin = i;
				next = j;
			}
		}
	}

	return true;
}

bool ISODATA::calmindis(int& origin, int& next)
{
	double themin = 100000000;
	int t = origin;


	for (int i = 0; i < current_order; ++i)
	{
		if (all_dis(origin, i) != -1 && all_dis(origin, i) < themin)
		{
			themin = all_dis(origin, i);
			next = i;
		}
	}

	return true;
}

bool ISODATA::calMaxVar()
{
	maxvar_vec.resize(current_order);

	double themax = 0;

	for (int i = 0; i < current_order; ++i)
	{
		if (!judgeUsed(i))
		{
			for (int j = 0; j < feature; ++j)
			{
				if (var_vec[i][j] > themax)
				{
					themax = var_vec[i][j];
				}
			}
			maxvar_vec[i] = themax;
		}
	}
	return true;
}

double ISODATA::calAllMeanDis()
{
	long amount = 0;
	double result = 0;

	for (int i = 0; i < current_order; ++i)
	{
		if (!judgeUsed(i))
		{
			result += meandis_vec[i] * amount_vec[i];
			amount += amount_vec[i];
		}
	}

	double divisor = amount;

	result /= divisor;

	return result;
}

bool ISODATA::mergeClass(int c1, int c2)
{
	//amount_vec.push_back(amount_vec[c1] + amount_vec[c2]);

	vector<double> temp;
	temp.resize(feature);

	for (int i = 0; i < feature; ++i)
	{
		temp[i] = ((mean_vec[c1][i] * amount_vec[c1]) + (mean_vec[c2][i] * amount_vec[c2])) / (amount_vec[c1] + amount_vec[c2]);
	}

	mean_vec.push_back(temp);

	used_num.push_back(c1);
	used_num.push_back(c2);

	current_K--;
	current_order += 1;

	return true;
}

bool ISODATA::splitClass(int c)
{
	vector<double> tmean1, tmean2;
	tmean1.resize(feature);
	tmean2.resize(feature);

	for (int i = 0; i < feature; ++i)
	{
		tmean1[i] = mean_vec[c][i] - sqrt(var_vec[c][i]);
		tmean2[i] = mean_vec[c][i] + sqrt(var_vec[c][i]);
	}

	mean_vec.push_back(tmean1);
	mean_vec.push_back(tmean2);

	used_num.push_back(c);

	current_K++;
	current_order += 2;
	return true;
}

bool ISODATA::defineColorTable()
{

	GDALDataset* poDataset = _result->getDataset();
	GDALRasterBand* poBand = poDataset->GetRasterBand(1);


	GDALColorTable* pColorTable;//��ɫ��
	GDALColorEntry* pColorEntry;//��ɫ��
	pColorTable = new GDALColorTable;//
	pColorEntry = new GDALColorEntry[24];

	for (int i = 0; i < 3; ++i)
	{
		pColorEntry[0 + 8 * i].c1 = 255; //������ɫֵ
		pColorEntry[0 + 8 * i].c2 = 0;
		pColorEntry[0 + 8 * i].c3 = 0;
		pColorEntry[0 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(0 + 8 * i, pColorEntry + 8 * i);

		pColorEntry[1 + 8 * i].c1 = 255; //������ɫֵ
		pColorEntry[1 + 8 * i].c2 = 127;
		pColorEntry[1 + 8 * i].c3 = 80;
		pColorEntry[1 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(1 + 8 * i, pColorEntry + 1 + 8 * i);

		pColorEntry[2 + 8 * i].c1 = 255; //������ɫֵ
		pColorEntry[2 + 8 * i].c2 = 255;
		pColorEntry[2 + 8 * i].c3 = 0;
		pColorEntry[2 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(2 + 8 * i, pColorEntry + 2 + 8 * i);

		pColorEntry[3 + 8 * i].c1 = 0; //������ɫֵ
		pColorEntry[3 + 8 * i].c2 = 255;
		pColorEntry[3 + 8 * i].c3 = 0;
		pColorEntry[3 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(3 + 8 * i, pColorEntry + 3 + 8 * i);

		pColorEntry[4 + 8 * i].c1 = 0; //������ɫֵ
		pColorEntry[4 + 8 * i].c2 = 128;
		pColorEntry[4 + 8 * i].c3 = 0;
		pColorEntry[4 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(4 + 8 * i, pColorEntry + 4 + 8 * i);

		pColorEntry[5 + 8 * i].c1 = 0; //������ɫֵ
		pColorEntry[5 + 8 * i].c2 = 255;
		pColorEntry[5 + 8 * i].c3 = 255;
		pColorEntry[5 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(5 + 8 * i, pColorEntry + 5 + 8 * i);

		pColorEntry[6 + 8 * i].c1 = 0; //������ɫֵ
		pColorEntry[6 + 8 * i].c2 = 0;
		pColorEntry[6 + 8 * i].c3 = 255;
		pColorEntry[6 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(6 + 8 * i, pColorEntry + 6 + 8 * i);

		pColorEntry[7 + 8 * i].c1 = 255; //������ɫֵ
		pColorEntry[7 + 8 * i].c2 = 0;
		pColorEntry[7 + 8 * i].c3 = 255;
		pColorEntry[7 + 8 * i].c4 = 0;
		pColorTable->SetColorEntry(7 + 8 * i, pColorEntry + 7 + 8 * i);
	}


	poBand->SetColorTable(pColorTable);

	delete pColorEntry;
	delete pColorTable;

	return true;
}


