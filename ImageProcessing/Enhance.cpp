#include "Enhance.h"

Enhance::Enhance(ShowImage* raw) :img(nullptr),showraw(raw),imgitem(nullptr),checkcolor(false)
{

}

Enhance::~Enhance()
{
	clear();
}

bool Enhance::initialize()
{
	if (img == nullptr)
	{
		bytecol = (showraw->getshowcol() * 24 + 31) / 8;				//由于采用RGB排列，需要保证内存对齐
		img = new unsigned char[showraw->getshowrow() * bytecol * 3];
	}
	return true;
}

QGraphicsPixmapItem* Enhance::linearStretch(double s)
{
	if (showColour())
	{
		toColour();
	}
	else
	{
		linearSta(s);
		linearStre(s);
	}

	return trans2Item();
}

QGraphicsPixmapItem* Enhance::noStretch()
{
	if (showColour())
	{
		toColour();
	}
	else
	{
		noStretchit();
	}

	return trans2Item();
}

QGraphicsPixmapItem* Enhance::logStretch()
{
	if (showColour())
	{
		toColour();
	}
	else
	{
		for (int i = 0; i < showraw->getshowrow(); ++i)
		{
			for (int j = 0; j < showraw->getshowcol(); ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					img[bytecol * i + 3 * j + k] = log2(showraw->at(k, i, j)) * 16;
				}
			}
		}
		showraw->clear();
	}

	return trans2Item();
}

QGraphicsPixmapItem* Enhance::hisEqualization()
{
	if (showColour())
	{
		toColour();
	}
	else
	{
		noStretchit();
		hisequlize();
	}

	return trans2Item();
}

bool Enhance::clear()
{
	if (img != nullptr) { delete img; }
	img = nullptr;
	return true;
}

bool Enhance::clearItem(QGraphicsScene* thescene)
{

	//if (imgitem != nullptr)
	{
		thescene->removeItem(imgitem);
		delete imgitem;
		imgitem = nullptr;
	}
	
	return true;
}

bool Enhance::linearSta(double s)
{

	vector<vector<int>> sta_table;

	if (showraw->getband() == 1)
	{
		sta_table.resize(1);
		enhance_sta = MatrixXd::Constant(3, 2, 0);
	}
	else
	{
		sta_table.resize(3);
		enhance_sta = MatrixXd::Constant(3, 2, 0);
	}

	for (int i = 0; i < sta_table.size(); ++i)
	{
		sta_table[i].resize(65536);
		for (int j = 0; j < sta_table[i].size(); ++j)
		{
			sta_table[i][j] = 0;
		}
	}

	for (int k = 0; k < showraw->getband(); ++k)
	{
		for (int i = 0; i < showraw->getshowrow(); ++i)
		{
			for (int j = 0; j < showraw->getshowcol(); ++j)
			{
				sta_table[k][showraw->at(k, i, j)]++;
			}
		}
	}

	

	long long thesum = 0;
	int t = 0;
	double percent = 0;
	double divisor = showraw->getshowcol() * showraw->getshowrow();

	for (int i = 0; i < showraw->getband(); ++i)
	{
		for (int j = 0; j < 65536; ++j)
		{
			thesum += sta_table[i][j];
			percent = thesum / divisor;
			if (percent > (s / double(100)) && t == 0)
			{
				enhance_sta(i, 0) = j;
				t = 1;
			}
			if (percent > (1 - s / double(100)) && t == 1)
			{
				enhance_sta(i, 1) = j;
				break;
			}
		}
		t = 0;
		thesum = 0;
	}


	if (showraw->getband() == 1)
	{
		enhance_sta(1, 0) = enhance_sta(0, 0);
		enhance_sta(1, 1) = enhance_sta(0, 1);
		enhance_sta(2, 0) = enhance_sta(0, 0);
		enhance_sta(2, 1) = enhance_sta(0, 1);
	}

	return true;
}

bool Enhance::linearStre(double s)
{
	for (int i = 0; i < showraw->getshowrow(); ++i)
	{
		for (int j = 0; j < showraw->getshowcol(); ++j)
		{
			if (showraw->at(0, i, j) < enhance_sta(0, 0)) { img[bytecol * i + 3 * j] = 0; }
			else if (showraw->at(0, i, j) > enhance_sta(0, 1)) { img[bytecol * i + 3 * j] = 255; }
			else
			{
				img[bytecol * i + 3 * j] = (showraw->at(0, i, j) - enhance_sta(0, 0)) * 255 / (enhance_sta(0, 1) - enhance_sta(0, 0));
			}

			if (showraw->at(1, i, j) < enhance_sta(1, 0)) { img[bytecol * i + 3 * j + 1] = 0; }
			else if (showraw->at(1, i, j) > enhance_sta(1, 1)) { img[bytecol * i + 3 * j + 1] = 255; }
			else
			{
				img[bytecol * i + 3 * j + 1] = (showraw->at(1, i, j) - enhance_sta(1, 0)) * 255 / (enhance_sta(1, 1) - enhance_sta(1, 0));
			}
			if (showraw->at(2, i, j) < enhance_sta(2, 0)) { img[bytecol * i + 3 * j + 2] = 0; }
			else if (showraw->at(2, i, j) > enhance_sta(2, 1)) { img[bytecol * i + 3 * j + 2] = 255; }
			else
			{
				img[bytecol * i + 3 * j + 2] = (showraw->at(2, i, j) - enhance_sta(2, 0)) * 255 / (enhance_sta(2, 1) - enhance_sta(2, 0));
			}
		}
	}
	showraw->clear();

	return true;
}

bool Enhance::noStretchit()
{
	int divisor = 256;
	if (showraw->isByte())divisor = 1;

	for (int i = 0; i < showraw->getshowrow(); ++i)
	{
		for (int j = 0; j < showraw->getshowcol(); ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				img[bytecol * i + 3 * j + k] = showraw->at(k, i, j) / divisor;
			}
		}
	}
	showraw->clear();

	return true;
}

bool Enhance::hisequlize()
{
	vector<vector<int>> staTable;
	vector<vector<int>> mapTable;
	staTable.resize(3);
	mapTable.resize(3);
	for (int i = 0; i < 3; ++i)
	{
		staTable[i].resize(256);
		mapTable[i].resize(256);
		for (int j = 0; j < 256; ++j)
		{
			staTable[i][j] = 0;
			mapTable[i][j] = 0;
		}
	}

	for (int i = 0; i < showraw->getshowrow(); ++i)
	{
		for (int j = 0; j < showraw->getshowcol(); ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				staTable[k][img[bytecol * i + 3 * j + k]]++;
			}
		}
	}
	
	for (int i = 0; i < 3; ++i)
	{
		int sum = 0;
		for (int j = 0; j < 256; ++j)
		{
			sum += staTable[i][j];
			mapTable[i][j] = 255 * sum/(showraw->getshowcol()*showraw->getshowrow()) + 0.5;
		}
	}

	for (int i = 0; i < showraw->getshowrow(); ++i)
	{
		for (int j = 0; j < showraw->getshowcol(); ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				img[bytecol * i + 3 * j + k] = mapTable[k][img[bytecol * i + 3 * j + k]];
			}
		}
	}

	showraw->clear();
	return true;
}

bool Enhance::showColour()
{
	if (!checkcolor)
	{
		int themin = 1000000;
		int themax = -1000000;
		for (int i = 0; i < showraw->getshowrow(); ++i)
		{
			for (int j = 0; j < showraw->getshowcol(); ++j)
			{
				if (themax < showraw->at(0, i, j)) { themax = showraw->at(0, i, j); }
			}
		}
		if (themax > 1 && themax < 20)iscolor = true;
		else iscolor = false;

		checkcolor = true;
	}

	return iscolor;
}

bool Enhance::toColour()
{
	for (int i = 0; i < showraw->getshowrow(); ++i)
	{
		for (int j = 0; j < showraw->getshowcol(); ++j)
		{
			switch (showraw->at(0,i,j))
			{
			case 0:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 128;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 1:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 2:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 3:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 4:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 5:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 6:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 127;
				img[bytecol * i + 3 * j + 2] = 80;
				break;
			case 7:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 8:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 9:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 127;
				img[bytecol * i + 3 * j + 2] = 80;
				break;
			case 10:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 11:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 12:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 128;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 13:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 14:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 15:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 255;
				break;
			case 16:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 0;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 17:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 127;
				img[bytecol * i + 3 * j + 2] = 80;
				break;
			case 18:
				img[bytecol * i + 3 * j] = 255;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 19:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 255;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			case 20:
				img[bytecol * i + 3 * j] = 0;
				img[bytecol * i + 3 * j + 1] = 128;
				img[bytecol * i + 3 * j + 2] = 0;
				break;
			}
		}
	}

	return true;
}

QGraphicsPixmapItem* Enhance::trans2Item()
{
	//tempimg = new QImage(img, showraw->getshowcol(), showraw->getshowrow(), bytecol, QImage::Format_RGB888);
	QImage temp(img, showraw->getshowcol(), showraw->getshowrow(), bytecol, QImage::Format_RGB888);
	imgitem = new QGraphicsPixmapItem(QPixmap::fromImage(temp));

	return imgitem;
}
