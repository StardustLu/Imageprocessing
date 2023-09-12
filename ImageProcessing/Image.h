/*
*	该类主要用于影像处理
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
	Image(Image&);																			//拷贝构造，只拷贝基础信息
	~Image();

	bool getPath(string&);																	//获取影像路径，并加载影像基础信息

	bool loadData(int band_1 = -1);															//按波段加载影像
	bool loadDatabyBlock(int brow, int bcol, int band_1 = -1);									//分块加载影像


	bool saveData(int band_1 = -1);																//按波段保存影像
	bool create(int band, int row, int col, Image* raw, string& opath, bool isthefloat = false);	//建立一个空的 影像对象，但要求指定变换前影像获得其空间参考及输出文件位置，一般用于将变换后影像保存
	bool prepareData(int band_ = -1);														//准备某一波段的数据空间
	bool saveDatabyBlock(int brow, int bcol, int band_1 = -1);								//分块io数据
	bool prepareDatabyBlock(int brow, int bcol, int band_1 = -1);							//分块准备某一波段空间

	bool isFloat() { return isfloat; }
	bool isByte() { return isbyte; }
	float at(int theband, int therow, int thecol);									//获取特定位置像元
	float atBlock(int theband, int therow, int thecol);							//按块读取时，获取影像块中像元

	bool set(int theband, int therow, int thecol, float value);								//设置特定位置像元
	bool setBlock(int theband, int therow, int thecol, float value);

	bool clear(int theband = -1);															//将影像所占空间清空，节省内存，处理完成后即执行

	int getBand() { return band; }
	int getRow() { return row; }
	int getCol() { return col; }

	int getEachBlockRow() { return currentblock_row; }										//获取当前单个影像块的row
	int getEachBlockCol() { return currentblock_col; }										//获取当前单个影像块的col
	int getBlockRow() { return b_row; }														//获取影像块个数row
	int getBlockCol() { return b_col; }														//获取影像块个数col
	int getDefaultBlockRow() { return block_row; }											//获取缺省时单个影像块的row
	int getDefaultBlockCol() { return block_col; }											//获取缺省时当个影像块的col

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

	int block_row, block_col;																//读取单个影像块长宽
	int currentblock_row, currentblock_col;													//当前单个影像块的长宽
	int b_row, b_col;																		//影像块个数

	double geotrans[6];

	vector<unsigned short*>	data;
	vector<float*>	fdata;
	bool isfloat;
	bool isbyte;

};

