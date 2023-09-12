#pragma once
#include "DataManage.h"
#include <complex>
#include <cmath>
#include<iostream>

using namespace std;

const double pi = acos(-1);

class FourierTrans
{
public:
	FourierTrans(DataManage* tm, Image* im) :manage(tm), img(im), lim(1), rev_table(nullptr), data(nullptr), row(1), col(1) {}
	~FourierTrans();

	bool getinverseOrigin(Image*);

	Image* fftForward(string&, int band = -1);
	Image* fftInverse(string&, int band = -1);

	bool clear();

private:

	bool changeSize();

	bool img2temp(int band, bool isFourierImg);
	bool temp2data(int num, bool rowOrcol);
	bool data2temp(int num, bool rowOrcol);
	bool temp2img(int band, bool isFourierImg);

	bool fftshift(int band);

	bool sortTable();
	bool fft(int);														//执行fft,int用来标记正逆变换

	DataManage* manage;
	Image* img;
	Image* result;
	Image* outresult;

	int row, col;														//记录任意长度矩阵拓展为2次方后的长宽

	int length;
	int height;															//用来记录高度
	int lim;															//次数，用来离length最近的2次数
	int* rev_table;														//用来标记变换后元素的位置
	complex<double>* data;												//用来暂存数据：将原始图像暂存为复数，再由复数转存到原始图像
	complex<double>* trans;												//用于逆变换暂存数据

};

