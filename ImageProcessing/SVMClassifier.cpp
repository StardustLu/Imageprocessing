#include "SVMClassifier.h"

SVMClassifier::SVMClassifier(DataManage* tm, Image* ti) :manager(tm), _ImageTobeClassified(ti)
{

}

SVMClassifier::~SVMClassifier()
{
	for (int i = 0; i < classifer_Matrix.size(); ++i)
	{
		for (int j = 0; j < classifer_Matrix[i].size(); ++j)
		{
			if (classifer_Matrix[i][j] != nullptr)
			{
				delete classifer_Matrix[i][j];
			}
		}
	}
}


bool SVMClassifier::getSample(vector<vector<VectorXd>>* thesample)
{
	_sample = thesample;
	types = thesample->size();
	classifer_Matrix.resize(types);
	for (int i = 0; i < types; ++i)
	{
		classifer_Matrix[i].resize(types);
		for (int j = 0; j < types; ++j)
		{
			classifer_Matrix[i][j] = nullptr;
		}
	}

	return true;
}

Image* SVMClassifier::getresult()
{
	return _resultImage;
}

bool SVMClassifier::train(KernelFunction the, int thec, int thegamma, int thed)
{
	for (int i = 0; i < types; ++i)
	{
		for (int j = i; j < types; ++j)
		{
			if (i != j)
			{
				classifer_Matrix[i][j] = new SVMBasic(manager, _ImageTobeClassified);
				classifer_Matrix[i][j]->getSample(&((*_sample)[i]), &((*_sample)[j]));
				classifer_Matrix[i][j]->train(the, thec, thegamma, thed);
			}
		}
	}

	cout << "Train Finished!" << endl;

	return true;
}

MatrixXd* SVMClassifier::getConfusionMatrix(vector<vector<VectorXd>>* thetest)
{
	_test = thetest;
	confusion_mat = MatrixXd::Constant(types, types, 0);
	//列为真值，行为预测值
	for (int i = 0; i < types; ++i)
	{
		for (int j = 0; j < (*_test)[i].size(); ++j)
		{
			confusion_mat(i, getkind((*_test)[i][j]))++;
		}
	}

	return &confusion_mat;
}

int SVMClassifier::getkind(VectorXd& the)
{
	vector<int> kinds;
	int tempkind;

	for (int i = 0; i < types; ++i)
	{
		for (int j = i; j < types; ++j)
		{
			if (i != j)
			{
				tempkind = classifer_Matrix[i][j]->getkind(the);							//返回0为负类，返回1为正类
				if (tempkind > 0) { tempkind = i; }													//如果时正类，则说明是属于i类
				else
				{
					tempkind = j;
				}																//如果是负类，则属于j类
				kinds.push_back(tempkind);															//记录所有被判别的类别
			}
		}
	}

	vector<int> amount;
	amount.resize(_sample->size());
	for (int i = 0; i < amount.size(); ++i)
	{
		amount[i] = 0;
	}

	for (int i = 0; i < kinds.size(); ++i)															//标记被判别给各个类别次数
	{
		amount[kinds[i]]++;
	}

	int max = amount[0];
	int maxpos = 0;
	for (int i = 0; i < amount.size(); ++i)															//找到判别次数最多的一类，并认为该像元属于这一类
	{
		if (max < amount[i])
		{
			max = amount[i];
			maxpos = i;
		}
	}

	return maxpos;
}

int SVMClassifier::getkind(int row, int col, VectorXd& pixel, bool block)
{
	vector<int> kinds;
	int tempkind;

	for (int i = 0; i < types; ++i)
	{
		for (int j = i; j < types; ++j)
		{
			if (i != j)
			{
				tempkind = classifer_Matrix[i][j]->getkind(row, col, pixel, block);							//返回0为负类，返回1为正类
				if (tempkind > 0) { tempkind = i; }													//如果时正类，则说明是属于i类
				else
				{
					tempkind = j;
				}																//如果是负类，则属于j类
				kinds.push_back(tempkind);															//记录所有被判别的类别
			}
		}
	}

	vector<int> amount;
	amount.resize(_sample->size());
	for (int i = 0; i < amount.size(); ++i)
	{
		amount[i] = 0;
	}

	for (int i = 0; i < kinds.size(); ++i)															//标记被判别给各个类别次数
	{
		amount[kinds[i]]++;
	}

	int max = amount[0];
	int maxpos = 0;
	for (int i = 0; i < amount.size(); ++i)															//找到判别次数最多的一类，并认为该像元属于这一类
	{
		if (max < amount[i])
		{
			max = amount[i];
			maxpos = i;
		}
	}

	return maxpos;
}


Image* SVMClassifier::classify(string& opath)
{
	_resultImage = manager->create();
	_resultImage->create(1, _ImageTobeClassified->getRow(), _ImageTobeClassified->getCol(), _ImageTobeClassified, opath);

	//leo::Matrix<unsigned short> temp(1, _ImageTobeClassified->getBands(), 0);
	VectorXd temp(_ImageTobeClassified->getBand());
	int process = 0, lastprocess = 0;
	cout << "Start to classify!" << endl;

	for (int i = 0; i < _ImageTobeClassified->getBlockRow(); ++i)
	{
		for (int j = 0; j < _ImageTobeClassified->getBlockCol(); ++j)
		{
			_resultImage->prepareDatabyBlock(i, j, 0);
			_ImageTobeClassified->loadDatabyBlock(i, j);

			for (int a = 0; a < _ImageTobeClassified->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < _ImageTobeClassified->getEachBlockCol(); ++b)
				{
					_resultImage->setBlock(0, a, b, getkind(a, b, temp, true));
				}
			}
			_resultImage->saveDatabyBlock(i, j, 0);
			_resultImage->clear();
			_ImageTobeClassified->clear();
			cout << i << " " << j << endl;
		}
	}

	return _resultImage;
}