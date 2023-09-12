
/*
*	本类用于管理 Image Ehance Statistic 类
*	凡是需要一个新的image对象，必须由此类 create
*/


#pragma once
#include "Image.h"
#include "Statistic.h"
#include "ShowImage.h"
#include "Enhance.h"

class DataManage
{
public:
	DataManage():current(-1) {}
	~DataManage();

	Image* create();
	bool cancel(int);
	bool cancel(Image*);
	int find(Image*);
	bool setCurrent(Image*);
	bool setCurrent(int t) { current = t; return true; }
	int getCurrent() { return current; }
	int size() { return image_list.size(); }

	Image* getimg(int t) { return image_list[t]; }
	Statistic* getSta(Image*);
	Statistic* getSta(int t) { return sta_list[t]; }
	ShowImage* getshow(Image*);
	ShowImage* getshow(int t) { return show_list[t]; }
	Enhance* getEnh(Image*);
	Enhance* getEnh(int t) { return enhance_list[t]; }

private:
	int current;
	vector<Image*>	image_list;
	vector<Statistic*>	sta_list;
	vector<ShowImage*>	show_list;
	vector<Enhance*>	enhance_list;

};

