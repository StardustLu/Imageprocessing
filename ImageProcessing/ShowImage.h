
/*
*	����������ʾӰ�񣬽���һ��ԭʼӰ��Ψһȷ��
*/


#pragma once
#include "Image.h"

class ShowImage
{
public:

	ShowImage(Image*);
	~ShowImage();

	bool setWindowSize(int wrow, int wcol, int del = 300);										//��ȡ��ʾ���ڴ�С,����������

	bool setMoveLength(int mx, int my);													//��ȡ����ƶ��ľ���
	bool setScale(int _scale);

	bool refreshRoll();
	bool refreshDrag();

	bool loadData(int band1, int band2 = -1, int band3 = -1);							//�����ض�����Ӱ��
	bool loadCurrent();
	//bool setpar(int startx, int starty, int lengthx, int lengthy);			//�趨��ȡ����
	bool isLoaded() { return is_loaded; }
	bool isByte() { return isbyte; }

	bool clear();
	int getshowcol() { return showcol; }
	int getshowrow() { return showrow; }
	int getband() { return band; }
	int* getCurrentShowband() { return bandsinf; }
	unsigned short at(int theband, int therow, int thecol);								//�����ض�λ�õ�����

private:

	bool float2uchort(float*, unsigned short*);

	GDALDataset* poDataset;
	bool isfloat;
	bool isbyte;

	bool is_loaded;

	double scale;																			//�Ŵ����
	int m_X, m_Y;																		//����ƶ�����
	int start_X, start_Y, length_X, length_Y;											//��ȡ����ʼλ��(����ԭʼӰ�����꣩����ȡ����
	int win_X, win_Y;																	//���ڴ�С
	int delta;																			//�򴰿���Χ�Ļ���������
	int showrow, showcol, band;															//��ȡӰ���Ĵ�С��showX��showY��
	Image* raw;																			//ԭӰ��
	const char* filepath;
	int bandsinf[3];																	//��¼RGB�ֱ����Ǽ�������

	double geotrans[6];
	vector<unsigned short*>	data;

};

