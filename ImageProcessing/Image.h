/*
*	������Ҫ����Ӱ����
*/

#pragma once
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "gdal_priv.h"
#include "cpl_conv.h"
using namespace std;
using namespace Eigen;

class Image
{
public:
	Image() :poDataset(nullptr), origin(nullptr), block_col(512), block_row(512) {}
	Image(Image&);																			//�������죬ֻ����������Ϣ
	~Image();

	bool getPath(string&);																	//��ȡӰ��·����������Ӱ�������Ϣ

	bool loadData(int band_1 = -1);															//�����μ���Ӱ��
	bool loadDatabyBlock(int brow, int bcol, int band_1 = -1);									//�ֿ����Ӱ��


	bool saveData(int band_1 = -1);																//�����α���Ӱ��
	bool create(int band, int row, int col, Image* raw, string& opath, bool isthefloat = false);	//����һ���յ� Ӱ����󣬵�Ҫ��ָ���任ǰӰ������ռ�ο�������ļ�λ�ã�һ�����ڽ��任��Ӱ�񱣴�
	bool prepareData(int band_ = -1);														//׼��ĳһ���ε����ݿռ�
	bool saveDatabyBlock(int brow, int bcol, int band_1 = -1);								//�ֿ�io����
	bool prepareDatabyBlock(int brow, int bcol, int band_1 = -1);							//�ֿ�׼��ĳһ���οռ�

	bool isFloat() { return isfloat; }
	bool isByte() { return isbyte; }
	float at(int theband, int therow, int thecol);									//��ȡ�ض�λ����Ԫ
	float atBlock(int theband, int therow, int thecol);							//�����ȡʱ����ȡӰ�������Ԫ

	bool set(int theband, int therow, int thecol, float value);								//�����ض�λ����Ԫ
	bool setBlock(int theband, int therow, int thecol, float value);

	bool clear(int theband = -1);															//��Ӱ����ռ�ռ���գ���ʡ�ڴ棬������ɺ�ִ��

	int getBand() { return band; }
	int getRow() { return row; }
	int getCol() { return col; }

	int getEachBlockRow() { return currentblock_row; }										//��ȡ��ǰ����Ӱ����row
	int getEachBlockCol() { return currentblock_col; }										//��ȡ��ǰ����Ӱ����col
	int getBlockRow() { return b_row; }														//��ȡӰ������row
	int getBlockCol() { return b_col; }														//��ȡӰ������col
	int getDefaultBlockRow() { return block_row; }											//��ȡȱʡʱ����Ӱ����row
	int getDefaultBlockCol() { return block_col; }											//��ȡȱʡʱ����Ӱ����col

	string& getName();
	string& getfilepath();
	GDALDataset* getDataset() { return poDataset; }

	unsigned short* operator[](int i) { return data[i]; }

private:

	Image* origin;

	GDALDataset* poDataset;

	const char* path;
	string file_path;
	string file_name;

	int band, row, col;
	int bytecol;

	int block_row, block_col;																//��ȡ����Ӱ��鳤��
	int currentblock_row, currentblock_col;													//��ǰ����Ӱ���ĳ���
	int b_row, b_col;																		//Ӱ������

	double geotrans[6];

	vector<unsigned short*>	data;
	vector<float*>	fdata;
	bool isfloat;
	bool isbyte;

};

