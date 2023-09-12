
/*
*	ÿһ�� Enhance ����ֻͳ��Ψһ��Ӱ��
*	��һ�� Ehnace �����һ�� Image ����������������ͬ
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

	//ͳ�Ƹ�����������ǿ����С���ֵ ��ʽ: min max
	MatrixXd	enhance_sta;

	
	QGraphicsPixmapItem* imgitem;

	unsigned char* img;

};

