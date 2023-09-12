#include "Statistic.h"

Statistic::Statistic(Image* theraw) :is_sta(false), is_staco(false),isfloat(false)
{
	raw = theraw;
}

double Statistic::min(int band)
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}

	return min_table[band];
}

double Statistic::max(int band)
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}

	return max_table[band];
}

double Statistic::mean(int band)
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return mean_table[band];
}

double Statistic::mid(int band)
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return mid_table[band];
}

double Statistic::mode(int band)
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return mode_table[band];
}

double Statistic::samplevariance(int band)
{
	staCoMat();
	return covar_mat(band, band);
}

double Statistic::covariance(int band_1, int band_2)
{
	staCoMat();
	return covar_mat(band_1, band_2);
}

vector<double>* Statistic::minVec()
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return &min_table;
}

vector<double>* Statistic::maxVec()
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return &max_table;
}

vector<double>* Statistic::meanVec()
{
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}
	return &mean_table;
}

vector<double>* Statistic::midVec()
{
	if (isfloat)
	{
		return nullptr;
	}
	else
	{
		staTable();
	}
	return &mid_table;
}

vector<double>* Statistic::modeVec()
{
	if (isfloat)
	{
		return nullptr;
	}
	else
	{
		staTable();
	}
	return &mode_table;
}

vector<vector<int>>* Statistic::numVec()
{
	if (isfloat)
	{
		
		return nullptr;
	}
	else
	{
		staTable();
	}
	return &num_table;
}

MatrixXd* Statistic::coVarMat()
{
	staCoMat();
	return &covar_mat;
}

MatrixXd* Statistic::correlationMat()
{
	staCoMat();
	return &correlation_mat;
}

bool Statistic::initialize()
{
	if (!min_table.empty()) { return true; }

	isfloat = raw->isFloat();
	int b = raw->getBand();

	min_table.resize(b);
	max_table.resize(b);
	mean_table.resize(b);
	mid_table.resize(b);
	mode_table.resize(b);
	num_table.resize(b);

	covar_mat = MatrixXd::Constant(b, b, -1000000);
	correlation_mat = MatrixXd::Constant(b, b, -1000000);

	for (int i = 0; i < b; ++i)
	{
		min_table[i] = -1000000;
		max_table[i] = -1000000;
		mean_table[i] = -1000000;
		mid_table[i] = -1000000;
		mode_table[i] = -1000000;

		if (!isfloat)
		{
			num_table[i].resize(65536);

			for (int j = 0; j < num_table.size(); ++j)
			{
				num_table[i][j] = 0;
			}
		}

	}



	return true;
}

bool Statistic::staTable()
{
	if (is_sta) { return true; }

	for (int i = 0; i < raw->getBand(); ++i)
	{
		raw->loadData(i);

		for (int j = 0; j < raw->getRow(); ++j)
		{
			for (int k = 0; k < raw->getCol(); ++k)
			{
				num_table[i][raw->at(i, j, k)]++;
			}
		}

		raw->clear();
	}

	long long sumcount = 0;				//用于计数
	long long sum = 0;					//用于求和
	double divisor = raw->getCol() * raw->getRow();
	double per = 0;
	int maxcount = 0;

	bool find_min = false;
	bool find_max = false;

	for (int i = 0; i < num_table.size(); ++i)
	{
		for (int j = 0; j < num_table[i].size(); ++j)
		{
			if (num_table[i][j] != 0 && (!find_min)) { min_table[i] = j; find_min = true; }
			if (num_table[i][65535 - j] != 0 && (!find_max)) { max_table[i] = 65535 - j; find_max = true; }

			sum += j * num_table[i][j];
			sumcount += num_table[i][j];
			per = sumcount / divisor;

			if ((sumcount / divisor < 0.5) && (sumcount + num_table[i][j] / divisor > 0.5) && j < 65534)
			{
				mid_table[i] = j;
			}

			if (num_table[i][j] > maxcount) { mode_table[i] = j; maxcount = num_table[i][j]; }
		}
		mean_table[i] = sum / divisor;
		sumcount = 0;
		sum = 0;
		per = 0;
		maxcount = 0;
		find_min = false;
		find_max = false;
	}

	is_sta = true;
	return is_sta;
}

bool Statistic::staFloat()
{
	if (is_sta) { return true; }

	double themin = 10000000;
	double themax = -10000000;
	double sum = 0;
	double divisor = raw->getCol() * raw->getRow();

	for (int i = 0; i < raw->getBand(); ++i)
	{
		raw->loadData(i);

		for (int j = 0; j < raw->getRow(); ++j)
		{
			for (int k = 0; k < raw->getCol(); ++k)
			{
				if (raw->at(i, j, k) < themin) { themin = raw->at(i, j, k); }
				if (raw->at(i, j, k) > themax) { themax = raw->at(i, j, k); }

				sum += raw->at(i, j, k);
			}
		}

		min_table[i] = themin;
		max_table[i] = themax;
		mean_table[i] = sum / divisor;
		themin = 10000000;
		themax = -10000000;
		sum = 0;

		raw->clear();
	}

	return true;
}

bool Statistic::staCoMat()
{
	if (is_staco) { return true; }
	if (isfloat)
	{
		staFloat();
	}
	else
	{
		staTable();
	}

	//raw->loadData();
	double divisor = raw->getCol() * raw->getRow();
	for (int i = 0; i < raw->getBand(); ++i)
	{
		raw->loadData(i);
		for (int j = i; j < raw->getBand(); ++j)
		{
			raw->loadData(j);
			double sum = 0;

			for (int a = 0; a < raw->getRow(); ++a)
			{
				for (int b = 0; b < raw->getCol(); ++b)
				{
					raw->at(i, a, b);
					raw->at(j, a, b);
					sum += (raw->at(i, a, b) - mean_table[i]) * (raw->at(j, a, b) - mean_table[j]);
				}
			}
			if (i != j)raw->clear(j);
			covar_mat(i, j) = sum / (divisor - 1);
			covar_mat(j, i) = covar_mat(i, j);
		}
		raw->clear(i);
	}
	raw->clear();
	for (int i = 0; i < raw->getBand(); ++i)
	{
		for (int j = i; j < raw->getBand(); ++j)
		{
			correlation_mat(i, j) = covar_mat(i, j) / (sqrt(covar_mat(i, i) * covar_mat(j, j)));
			correlation_mat(j, i) = correlation_mat(i, j);
		}
	}

	is_staco = true;
	return is_staco;
}


