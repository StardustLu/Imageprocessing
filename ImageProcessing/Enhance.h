
/*
*	每一个 Enhance 对象只统计唯一的影像
*	即一个 Ehnace 对象绑定一个 Image 对象，其生命周期相同
*/

#pragma once
#include "ShowImage.h"
#include <qgraphicsview.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <cmath>


class Enhance
{
public:
	Enhance(ShowImage*);
	~Enhance();

	bool initialize();

	QGraphicsPixmapItem* linearStretch(double s = 5);
	QGraphicsPixmapItem* noStretch();
	QGraphicsPixmapItem* logStretch();
	QGraphicsPixmapItem* hisEqualization();

	bool clear();
	bool clearItem(QGraphicsScene*);

	int getBytecol() { return bytecol; }


private:
	bool linearSta(double s = 5);
	bool linearStre(double s = 5);
	bool noStretchit();
	bool hisequlize();
	bool showColour();
	bool toColour();
	QGraphicsPixmapItem* trans2Item();

	bool iscolor;
	bool checkcolor;
	int bytecol;
	ShowImage* showraw;

	//统计各波段用于增强的最小最大值 格式: min max
	MatrixXd	enhance_sta;

	
	QGraphicsPixmapItem* imgitem;

	unsigned char* img;

};

