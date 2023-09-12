#include "SampleProcess.h"

SampleProcess::SampleProcess(vector<vector<Image>>* thevec)
{
	has_trans = false;
	has_split = false;
	raw = thevec;
	feature = (*thevec)[0][0].getBand();
	types = (*raw).size();
	sample.resize(thevec->size());


	mean_vec.resize(types);
	covar_mat.resize(types);

	for (int i = 0; i < types; ++i)
	{
		mean_vec[i] = VectorXd::Constant(feature, 0);
		covar_mat[i] = MatrixXd::Constant(feature, feature, 0);
	}
	JM_mat = MatrixXd::Constant(types, types, 0);

}

vector<vector<VectorXd>>* SampleProcess::getSample(double per)
{
	if (!has_trans)img2vec();
	if (!has_split)splitSample(per);

	return &train_sample;
}

vector<vector<VectorXd>>* SampleProcess::getTest(double per)
{
	if (!has_trans)img2vec();
	if (!has_split)splitSample(per);
	return &test_sample;
}

vector<vector<VectorXd>>* SampleProcess::img2vec()
{
	for (int a = 0; a < (*raw).size(); ++a)
	{
		for (int b = 0; b < (*raw)[a].size(); ++b)
		{
			(*raw)[a][b].loadData();
			for (int i = 0; i < (*raw)[a][b].getRow(); ++i)
			{
				for (int j = 0; j < (*raw)[a][b].getCol(); ++j)
				{
					VectorXd temp(feature);
					for (int k = 0; k < feature; ++k)
					{
						temp(k) = (*raw)[a][b].at(k, i, j);
					}
					sample[a].push_back(temp);
				}
			}
			(*raw)[a][b].clear();
		}
	}
	has_trans = true;
	return &sample;
}

MatrixXd* SampleProcess::roiJudge()
{
	if (!has_trans)img2vec();
	calMean();
	calVar();
	calBaBhattacharyya();
	double t1;
	for (int i = 0; i < types; ++i)
	{
		for (int j = i + 1; j < types; ++j)
		{
			//cout << JM_mat(i, j) << endl << endl;
			JM_mat(i, j) = sqrt(2 * (1 - exp(-JM_mat(i, j))));
			//cout << JM_mat(i, j) << endl << endl;
			JM_mat(j, i) = JM_mat(i, j);
		}
	}

	return &JM_mat;
}

bool SampleProcess::splitSample(double per)
{
	train_sample.resize(sample.size());
	test_sample.resize(sample.size());

	int lenth = 1 / per;

	for (int i = 0; i < sample.size(); ++i)
	{
		for (int j = 0; j < (sample)[i].size(); ++j)
		{
			if (j % lenth == 0)
			{
				(train_sample)[i].push_back((sample)[i][j]);
			}
			else
			{
				(test_sample)[i].push_back((sample)[i][j]);
			}
		}
	}
	has_split = true;
	return true;
}

bool SampleProcess::calMean()
{
	//int t1,t2;
	for (int i = 0; i < sample.size(); ++i)
	{
		for (int j = 0; j < sample[i].size(); ++j)
		{
			for (int k = 0; k < feature; ++k)
			{
				mean_vec[i](k) += sample[i][j](k);
			}
		}
		//cout << mean_vec[i] << endl;
		for (int k = 0; k < feature; ++k)
		{
			mean_vec[i](k) /= sample[i].size();
		}
		//cout << mean_vec[i] << endl;
	}

	return true;
}

bool SampleProcess::calVar()
{
	for (int i = 0; i < types; ++i)
	{
		for (int a = 0; a < feature; ++a)
		{
			for (int b = a; b < feature; ++b)
			{
				calCovar(i, a, b);
				covar_mat[i](b, a) = covar_mat[i](a, b);
			}
		}
		//cout << covar_mat[i] << endl << endl;
	}
	return true;
}

bool SampleProcess::calCovar(int type_1, int band_1, int band_2)
{
	for (int i = 0; i < sample[type_1].size(); ++i)
	{
		covar_mat[type_1](band_1, band_2) += (sample[type_1][i](band_1) - mean_vec[type_1](band_1)) * (sample[type_1][i](band_2) - mean_vec[type_1](band_2));
	}
	covar_mat[type_1](band_1, band_2) /= double(sample[type_1].size() - 1);

	return true;
}

bool SampleProcess::calBaBhattacharyya()
{
	for (int i = 0; i < types; ++i)
	{
		for (int j = i + 1; j < types; ++j)
		{
			MatrixXd _sigma = (covar_mat[i] + covar_mat[j]) / 2;
			//cout << _sigma << endl << endl;
			VectorXd _mean = (mean_vec[i] - mean_vec[j]);
			//cout << _mean << endl << endl;
			MatrixXd left = _mean.transpose() * _sigma.inverse() * _mean;
			//cout << left << endl << endl;
			double right = 0.5 * log((_sigma.determinant()) / sqrt(covar_mat[i].determinant() * covar_mat[j].determinant()));
			//cout << right << endl << endl;
			JM_mat(i, j) = left(0, 0) * 0.125 + right;
			//cout << JM_mat(i, j) << endl;
		}
	}


	return true;
}
