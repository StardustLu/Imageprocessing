#include "SVMBasic.h"


SVMBasic::SVMBasic(DataManage* tm, Image* ti) :b(0), current_1(0), current_2(1), c(1)							//传言工业界更多使c=1，但也可以使之等于10，0.5，0.1，c越大，控制越严格。当c为正无穷时，为硬间隔
{
	manager = tm;
	_ImageTobeClassified = ti;
}

SVMBasic::~SVMBasic()
{
}

bool SVMBasic::getSample(vector<VectorXd>* positive, vector<VectorXd>* negative)				//将二维矩阵转换为一维带标签的向量
{
	features = (*positive)[0].rows();

	for (int i = 0; i < positive->size(); ++i)
	{
		Sample temp;
		temp.x = (*positive)[i];
		temp.y = 1;
		_thesample.push_back(temp);
	}

	for (int i = 0; i < negative->size(); ++i)
	{
		Sample temp;
		temp.x = (*negative)[i];
		temp.y = -1;
		_thesample.push_back(temp);
	}

	alpha.resize(_thesample.size());
	E.resize(_thesample.size());

	for (int i = 0; i < _thesample.size(); ++i)
	{
		alpha[i] = 0;
		E[i] = 0;
	}

	return true;
}

bool SVMBasic::getPath(string& opath)
{
	_resultImage = manager->create();
	_resultImage->create(1, _ImageTobeClassified->getRow(), _ImageTobeClassified->getCol(), _ImageTobeClassified, opath);

	return true;
}

Image* SVMBasic::classify()
{
	//leo::Matrix<unsigned short> temp(1, features);
	VectorXd temp(features);

	_resultImage->prepareData(0);

	for (int i = 0; i < _ImageTobeClassified->getBlockRow(); ++i)
	{
		for (int j = 0; j < _ImageTobeClassified->getBlockCol(); ++j)
		{
			_resultImage->prepareDatabyBlock(i, j);
			_ImageTobeClassified->prepareDatabyBlock(i, j);

			for (int a = 0; a < _ImageTobeClassified->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < _ImageTobeClassified->getEachBlockCol(); ++b)
				{
					_resultImage->setBlock(0, i, j, calKind(i, j, temp));
				}
			}

			_resultImage->saveDatabyBlock(i, j);
			_resultImage->clear();
			_ImageTobeClassified->clear();
		}
	}


	return _resultImage;
}

Image* SVMBasic::getresult()
{
	return _resultImage;
}

bool SVMBasic::train(KernelFunction thekind, int thec, int thegamma, int thed)
{
	kernelType = thekind;
	//需要优化的参数
	gamma = thegamma;
	c = thec;
	d = thed;
	int process = 0;
	int lastprocess = 0;
	int test = _thesample.size();
	for (int i = 0; i < _thesample.size(); ++i)												//外层大循环
	{
		current_1 = i;
		for (int j = 0; j < _thesample.size(); ++j)											//内层循环,计算所有的E,找到能使α2变化最大的α
		{
			calEi(j);
		}
		findAlpha2();																		//找到α2,current_2=α2

		calAlpha2Un();																		//计算未剪切的α2
		clipAlpha2();																		//剪切α2
		calAlpha1();																		//反算α1
		calb();																				//计算b

		process = 100 * i / double(_thesample.size());
		//cout << process << endl;
		if (process != lastprocess)
		{
			cout << process << endl;
			lastprocess = process;
		}
	}

	for (int i = 0; i < alpha.size(); ++i)													//将非0的α储存
	{
		if (alpha[i] != 0)
		{
			supportVec.push_back(i);
			cout << alpha[i] << endl;
		}
	}

	return true;
}

int SVMBasic::getkind(int row, int col, VectorXd& pix, bool block)
{
	return calKind(row, col, pix, block);
}

int SVMBasic::getkind(VectorXd& the)
{
	return calKind(the);
}


double SVMBasic::calEi(int thei)
{
	double temp = 0;
	for (int i = 0; i < _thesample.size(); ++i)
	{
		temp += alpha[i] * (_thesample[i].y) * kernelTrick(i, thei);
	}
	double t1 = b;
	double t2 = _thesample[thei].y;
	temp = temp + b - double((_thesample[thei].y));
	E[thei] = temp;
	return temp;
}

double SVMBasic::calEta(int one, int two)
{
	return kernelTrick(one, one) + kernelTrick(two, two) - 2 * kernelTrick(one, two);
}

bool SVMBasic::calb()
{
	double b1, b2;

	if (0 < alpha[current_1] && alpha[current_1] < c)
	{
		b1 = -E[current_1] - (_thesample[current_1].y) * kernelTrick(current_1, current_1) * (alpha[current_1] - alpha_1old) - (_thesample[current_2].y) * kernelTrick(current_2, current_1) * (alpha[current_2] - alpha_2old) + b;

		if (0 < alpha[current_2] && alpha[current_2] < c)
		{
			b2 = -E[current_2] - (_thesample[current_1].y) * kernelTrick(current_1, current_2) * (alpha[current_1] - alpha_1old) - (_thesample[current_2].y) * kernelTrick(current_2, current_2) * (alpha[current_2] - alpha_2old) + b;
			b = (b1 + b2) / 2;
		}
		b = b1;
	}
	if (0 < alpha[current_2] && alpha[current_2] < c)
	{
		b2 = -E[current_2] - (_thesample[current_1].y) * kernelTrick(current_1, current_2) * (alpha[current_1] - alpha_1old) - (_thesample[current_2].y) * kernelTrick(current_2, current_2) * (alpha[current_2] - alpha_2old) + b;
		b = b2;
	}

	return true;
}

bool SVMBasic::calH()
{
	double temp = 0;

	if (_thesample[current_1].y != _thesample[current_2].y)
	{
		temp = c + alpha[current_2] - alpha[current_1];

		if (temp > c)
		{
			H = c;
		}
		else
		{
			H = temp;
		}
		return true;
	}
	else
	{
		temp = alpha[current_2] + alpha[current_1];

		if (temp > c)
		{
			H = c;
		}
		else
		{
			H = temp;
		}
		return true;
	}
}

bool SVMBasic::calL()
{
	double temp = 0;

	if (_thesample[current_1].y != _thesample[current_2].y)
	{
		temp = alpha[current_2] - alpha[current_1];
		if (temp > 0) { L = temp; }
		else { L = 0; }
		return true;
	}
	else
	{
		temp = alpha[current_2] + alpha[current_1] - c;
		if (temp > 0) { L = temp; }
		else { L = 0; }
		return true;
	}
}

double SVMBasic::kernelTrick(int thei, int thej)
{
	double temp = 0;
	double distance = 0;

	double t1, t2, t3, t4;

	switch (kernelType)
	{
	case 0:

		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			temp += (_thesample[thei].x)(i) * (_thesample[thej].x)(i);
		}

		break;
	case 1:

		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			temp += (_thesample[thei].x)(i) * (_thesample[thej].x)(i);
		}
		temp++;
		temp = pow(temp, d);

		break;
	case 2:

		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			t1 = (_thesample[thei].x)(i);
			t2 = (_thesample[thej].x)(i);
			distance += ((_thesample[thei].x)(i) - (_thesample[thej].x)(i)) * ((_thesample[thei].x)(i) - (_thesample[thej].x)(i));
		}
		temp = exp(-distance * gamma);

		break;
	case 3:
		break;
	default:
		break;
	}

	return temp;

}

int SVMBasic::min(vector<double>& the)
{
	double temp = the[0];
	int themin = 0;

	for (int i = 0; i < the.size(); ++i)
	{
		if (temp > the[i])
		{
			themin = i;
		}
	}

	return themin;
}

int SVMBasic::max(vector<double>& the)
{
	double temp = the[0];
	int themax = 0;

	for (int i = 0; i < the.size(); ++i)
	{
		if (temp < the[i])
		{
			themax = i;
		}
	}

	return themax;
}

double SVMBasic::calKernel(VectorXd& pixel, int thei)
{
	double temp = 0;
	double distance = 0;
	double t1, t2;

	switch (kernelType)
	{
	case 0:

		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			temp += (_thesample[thei].x)(i) * pixel(i);
		}
		break;
	case 1:
		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			temp += (_thesample[thei].x)(i) * pixel(i);
		}
		temp++;
		temp = pow(temp, d);
		break;
	case 2:
		for (int i = 0; i < _thesample[thei].x.rows(); ++i)
		{
			t1 = (_thesample[thei].x)(i) - pixel(i);
			distance += ((_thesample[thei].x)(i) - pixel(i)) * ((_thesample[thei].x)(i) - pixel(i));
		}
		temp = exp(-distance * gamma);
		break;
	case 3:
		break;
	default:
		break;
	}

	return temp;
}

int SVMBasic::calKind(int row, int col, VectorXd& pixel, bool block)
{
	double temp = 0;

	if (block)
	{
		for (int i = 0; i < pixel.rows(); ++i)
		{
			pixel(i) = _ImageTobeClassified->atBlock(i, row, col);
		}
	}
	else
	{
		for (int i = 0; i < pixel.rows(); ++i)
		{
			pixel(i) = _ImageTobeClassified->at(i, row, col);
		}
	}



	for (int i = 0; i < supportVec.size(); ++i)
	{
		temp += (_thesample[supportVec[i]].y) * alpha[supportVec[i]] * calKernel(pixel, supportVec[i]);
	}

	temp += b;

	if (temp > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

int SVMBasic::calKind(VectorXd& pixel)
{
	double temp = 0;

	for (int i = 0; i < supportVec.size(); ++i)
	{
		temp += (_thesample[supportVec[i]].y) * alpha[supportVec[i]] * calKernel(pixel, supportVec[i]);
	}

	temp += b;

	if (temp > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool SVMBasic::calAlpha2Un()
{
	calH();
	calL();
	alpha_1old = alpha[current_1];
	alpha_2old = alpha[current_2];

	double eta = calEta(current_1, current_2);
	if (eta != 0)
	{
		alpha[current_2] = alpha_2old + (_thesample[current_2].y) * (E[current_1] - E[current_2]) / eta;
		return true;
	}
	else
	{
		return false;
	}
}

bool SVMBasic::clipAlpha2()
{
	if (alpha[current_2] > H)
	{
		alpha[current_2] = H;
	}
	if (alpha[current_2] < L)
	{
		alpha[current_2] = L;
	}

	return true;
}

bool SVMBasic::calAlpha1()
{
	alpha[current_1] = alpha_1old + (_thesample[current_1].y) * (_thesample[current_2].y) * (alpha_2old - alpha[current_2]);

	return true;
}

bool SVMBasic::findAlpha2()
{
	if (E[current_1] >= 0)
	{
		current_2 = min(E);
		if (current_2 == current_1)
		{
			if ((current_1 == current_2) && (current_1 == _thesample.size() - 1))
			{
				return false;
			}
			current_2++;
		}
	}
	else
	{
		current_2 = max(E);
		if (current_2 == current_1)
		{
			if ((current_1 == current_2) && (current_1 == _thesample.size() - 1))
			{
				return false;
			}
			current_2++;
		}
	}

	return true;
}

Sample::Sample(const Sample& other)
{
	this->x = (other.x);
	this->y = other.y;
}
