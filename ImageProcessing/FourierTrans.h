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
	bool fft(int);														//ִ��fft,int�����������任

	DataManage* manage;
	Image* img;
	Image* result;
	Image* outresult;

	int row, col;														//��¼���ⳤ�Ⱦ�����չΪ2�η���ĳ���

	int length;
	int height;															//������¼�߶�
	int lim;															//������������length�����2����
	int* rev_table;														//������Ǳ任��Ԫ�ص�λ��
	complex<double>* data;												//�����ݴ����ݣ���ԭʼͼ���ݴ�Ϊ���������ɸ���ת�浽ԭʼͼ��
	complex<double>* trans;												//������任�ݴ�����

};

