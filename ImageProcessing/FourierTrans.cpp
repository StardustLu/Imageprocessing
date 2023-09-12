#include "FourierTrans.h"

FourierTrans::~FourierTrans()
{
	clear();
}

bool FourierTrans::getinverseOrigin(Image* the)
{
	result = the;
	return true;
}


Image* FourierTrans::fftForward(string& path, int band)
{
	if (band == -1)
	{
		changeSize();
		result = manage->create();
		result->create(img->getBand() * 3, row, col, img, path, true);

		for (int k = 0; k < img->getBand(); ++k)
		{
			trans = new complex<double>[row * col];
			img2temp(k, false);
			for (int i = 0; i < row; ++i)
			{
				temp2data(i, true);										//将复数矩阵加载到复数向量参与一维fft逆变换
				sortTable();
				fft(1);
				data2temp(i, true);

			}

			for (int i = 0; i < col; ++i)
			{
				temp2data(i, false);										//将复数矩阵加载到复数向量参与一维fft逆变换
				sortTable();
				fft(1);
				data2temp(i, false);
			}

			temp2img(k, false);
			delete trans;
		}


	}
	else
	{
		changeSize();
		result = manage->create();
		result->create(3, row, col, img, path, true);
		trans = new complex<double>[row * col];

		img2temp(band, false);
		for (int i = 0; i < row; ++i)
		{
			temp2data(i, true);										//将复数矩阵加载到复数向量参与一维fft逆变换
			sortTable();
			fft(1);
			data2temp(i, true);

		}

		for (int i = 0; i < col; ++i)
		{
			temp2data(i, false);										//将复数矩阵加载到复数向量参与一维fft逆变换
			sortTable();
			fft(1);
			data2temp(i, false);
		}

		temp2img(0, false);

		delete trans;
	}


	return result;
}

Image* FourierTrans::fftInverse(string& opath, int band)
{

	outresult = manage->create();
	outresult->create(img->getBand(), img->getRow(), img->getCol(), result, opath);

	for (int k = 0; k < img->getBand(); ++k)
	{
		trans = new complex<double>[row * col];
		img2temp(k, true);												//将数据加载到临时复数矩阵

		for (int i = 0; i < col; ++i)
		{
			temp2data(i, false);										//将复数矩阵加载到复数向量参与一维fft逆变换
			sortTable();												//计算最终顺序
			fft(-1);
			data2temp(i, false);										//将fft逆变换结果储存在复数矩阵中
		}
		//for (int i = 0; i < 10; ++i)cout << trans[512 * i+1] << endl;

		for (int i = 0; i < row; ++i)
		{
			temp2data(i, true);
			sortTable();
			fft(-1);
			data2temp(i, true);
		}


		//for (int i = 0; i < 10; ++i)cout << trans[512*i+1] << endl;

		temp2img(k, true);
		delete trans;
	}

	return outresult;
}

bool FourierTrans::clear()
{
	if (rev_table != nullptr)
	{
		delete rev_table;
	}
	if (data != nullptr)
	{
		delete data;
	}

	rev_table = nullptr;
	data = nullptr;
	lim = 1;
	height = 0;
	return true;
}



bool FourierTrans::changeSize()
{
	while (row < img->getRow())
	{
		row *= 2;
	}
	while (col < img->getCol())
	{
		col *= 2;
	}

	return true;
}

bool FourierTrans::img2temp(int band, bool isFourierImg)
{

	if (isFourierImg)
	{
		//result->loadData();
		result->loadData(3 * band);
		result->loadData(3 * band + 1);

		for (int i = 0; i < result->getRow(); ++i)
		{
			for (int j = 0; j < result->getCol(); ++j)
			{
				trans[result->getRow() * i + j].real(result->at(band * 3, i, j));
				trans[result->getRow() * i + j].imag(result->at(band * 3 + 1, i, j));
			}
		}

		result->clear();
	}
	else
	{
		img->loadData(band);

		for (int i = 0; i < img->getRow(); ++i)
		{
			for (int j = 0; j < img->getCol(); ++j)
			{
				trans[row * i + j].real(img->at(band, i, j));
			}
		}

		img->clear();
	}


	return true;
}

bool FourierTrans::temp2data(int num, bool rowOrcol)
{

	if (rowOrcol)
	{
		length = col;
		rev_table = new int[col];
		for (int i = 0; i < col; ++i)
		{
			rev_table[i] = i;
		}
		data = new complex<double>[col];

		for (int i = 0; i < col; ++i)
		{
			data[i] = trans[num * col + i];
		}

	}
	else
	{
		length = row;
		rev_table = new int[row];
		for (int i = 0; i < row; ++i)
		{
			rev_table[i] = i;
		}
		data = new complex<double>[row];

		for (int i = 0; i < row; ++i)
		{
			data[i] = trans[num + row * i];
		}
	}

	return true;
}

bool FourierTrans::data2temp(int num, bool rowOrcol)
{
	if (rowOrcol)
	{
		for (int i = 0; i < col; ++i)
		{
			trans[num * col + i] = data[i];
		}
	}
	else
	{
		for (int i = 0; i < row; ++i)
		{
			trans[num + row * i] = data[i];
		}

	}
	clear();

	return true;
}



bool FourierTrans::temp2img(int band, bool isFourierImg)
{
	if (isFourierImg)
	{
		outresult->prepareData(band);

		for (int i = 0; i < img->getRow(); ++i)
		{
			for (int j = 0; j < img->getCol(); ++j)
			{
				outresult->set(band, i, j, trans[row * i + j].real());
			}
		}

		outresult->saveData(band);
		outresult->clear();
	}
	else
	{
		for (int k = 0; k < 3; ++k)
		{
			result->prepareData(3 * band + k);
			for (int i = 0; i < row; ++i)
			{
				for (int j = 0; j < col; ++j)
				{
					switch (k)
					{
					case 0:
						result->set(3 * band, i, j, trans[row * i + j].real());
						break;
					case 1:
						result->set(3 * band + 1, i, j, trans[row * i + j].imag());
						break;
					case 2:
						double temp = log(sqrt(trans[row * i + j].real() * trans[row * i + j].real() + trans[row * i + j].imag() * trans[row * i + j].imag()));
						result->set(3 * band + 2, i, j, temp);
						break;
					}
				}
			}
			if (k == 2)
			{
				fftshift(band);
			}
			result->saveData(3 * band + k);
			result->clear();
		}

	}

	return true;
}

bool FourierTrans::fftshift(int band)
{
	int thex, they;

	for (int i = 0; i < row / 2; ++i)
	{
		for (int j = 0; j < col / 2; ++j)
		{
			double temp = result->at(3 * band + 2, i, j);
			result->set(3 * band + 2, i, j, result->at(3 * band + 2, i + row / 2, j + col / 2));
			result->set(3 * band + 2, i + row / 2, j + col / 2, temp);
		}
	}

	for (int i = 0; i < row / 2; ++i)
	{
		for (int j = col / 2; j < col; ++j)
		{
			double temp = result->at(3 * band + 2, i + row / 2, j - col / 2);
			result->set(3 * band + 2, i + row / 2, j - col / 2, result->at(3 * band + 2, i, j));
			result->set(3 * band + 2, i, j, temp);
		}
	}

	return true;
}



bool FourierTrans::sortTable()
{
	while (lim < length)
	{
		lim <<= 1;
		height++;
	}

	for (int i = 0; i < lim; ++i)
	{
		rev_table[i] = (rev_table[i >> 1] >> 1) | ((i & 1) << (height - 1));
	}

	return true;
}

bool FourierTrans::fft(int fORi)
{
	for (int i = 0; i < lim; ++i)
	{
		if (i < rev_table[i])					//根据变换后的编号对原data进行排序
		{
			complex<double> temp = data[i];
			data[i] = data[rev_table[i]];
			data[rev_table[i]] = temp;
		}
	}

	for (int dep = 1; dep <= log2(length); ++dep)
	{
		int m = 1 << dep;

		complex<double> wn(cos(2 * pi / double(m)), fORi * sin(2 * pi / double(m)));

		for (int k = 0; k < lim; k += m)
		{
			complex<double> w(1, 0);
			for (int j = 0; j < m / 2; ++j)
			{
				complex<double> t = w * data[k + j + m / 2];
				complex<double> u = data[k + j];
				data[k + j] = u + t;
				data[k + j + m / 2] = u - t;
				w = w * wn;
			}
		}
	}


	if (fORi == -1)
	{
		for (int i = 0; i < lim; ++i)
		{
			data[i].real(data[i].real() / lim);
			data[i].imag(data[i].imag() / lim);
		}
	}

	return true;
}
