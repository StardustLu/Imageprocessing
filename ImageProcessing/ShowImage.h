
/*
*	本类用于显示影像，仅与一幅原始影像唯一确定
*/


#pragma once
#include "Image.h"

class ShowImage
{
public:

	ShowImage(Image*);
	~ShowImage();

	bool setWindowSize(int wrow, int wcol, int del = 300);										//获取显示窗口大小,缓冲区长度

	bool setMoveLength(int mx, int my);													//获取鼠标移动的距离
	bool setScale(int _scale);

	bool refreshRoll();
	bool refreshDrag();

	bool loadData(int band1, int band2 = -1, int band3 = -1);							//加载特定波段影像
	bool loadCurrent();
	//bool setpar(int startx, int starty, int lengthx, int lengthy);			//设定读取参数
	bool isLoaded() { return is_loaded; }
	bool isByte() { return isbyte; }

	bool clear();
	int getshowcol() { return showcol; }
	int getshowrow() { return showrow; }
	int getband() { return band; }
	int* getCurrentShowband() { return bandsinf; }
	unsigned short at(int theband, int therow, int thecol);								//访问特定位置的像素

private:

	bool float2uchort(float*, unsigned short*);

	GDALDataset* poDataset;
	bool isfloat;
	bool isbyte;

	bool is_loaded;

	double scale;																			//放大层数
	int m_X, m_Y;																		//鼠标移动距离
	int start_X, start_Y, length_X, length_Y;											//读取的起始位置(对于原始影像坐标），读取长度
	int win_X, win_Y;																	//窗口大小
	int delta;																			//向窗口外围的缓冲区长度
	int showrow, showcol, band;															//读取影像块的大小（showX和showY）
	Image* raw;																			//原影像
	const char* filepath;
	int bandsinf[3];																	//记录RGB分别是那几个波段

	double geotrans[6];
	vector<unsigned short*>	data;

};

