#include "ShowImage.h"

ShowImage::ShowImage(Image* origin)
{
	raw = origin;
	isfloat = raw->isFloat();
	is_loaded = false;

	data.resize(3);
	for (int i = 0; i < data.size(); ++i)
	{
		data[i] = nullptr;
	}
	m_X = 0;
	m_Y = 0;
}

ShowImage::~ShowImage()
{
	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i] != nullptr)
		{
			delete data[i];
		}
	}

}

bool ShowImage::setWindowSize(int wrow, int wcol, int del)
{
	win_X = wcol;
	win_Y = wrow;
	delta = del;
	scale = 1;

	showcol = win_X + 2 * del;
	showrow = win_Y + 2 * del;

	while (!((win_X >= (raw->getCol() + 0.5) / scale) && (win_Y >= (raw->getRow() + 0.5) / scale)))
	{
		int t1 = (raw->getCol() + 0.5) / scale;
		int t2 = (raw->getRow() + 0.5) / scale;
		scale += 0.2;
	}

	return true;
}

bool ShowImage::setMoveLength(int mx, int my)
{
	m_X += mx;
	m_Y += my;
	return false;
}

bool ShowImage::setScale(int _scale)
{
	if (_scale > 0) { scale += 0.2; }
	else { scale -= 0.2; }

	return true;
}

bool ShowImage::loadData(int band1, int band2, int band3)
{
	poDataset = raw->getDataset();
	GDALRasterBand* tpoBand = poDataset->GetRasterBand(1);
	GDALDataType thetype = tpoBand->GetRasterDataType();

	if (thetype == GDT_Byte)isbyte = true;
	else isbyte = false;

	if (thetype == GDT_Float32 || thetype == GDT_Float64)
	{
		isfloat = true;
	}
	else
	{
		isfloat = false;
	}

	showcol = win_X + 2 * delta;
	showrow = win_Y + 2 * delta;

	if (showcol >= (raw->getCol() + 0.5) / scale) { showcol = (raw->getCol() + 0.5) / scale; }						//当x轴方向图像缩放后长度小于缓冲区大小，则以图像缩放长度为准
	if (showrow >= (raw->getRow() + 0.5) / scale) { showrow = (raw->getRow() + 0.5) / scale; }						//当y轴方向图像缩放后高度小于缓冲区大小，则以图像缩放高度为准

	start_X = (raw->getCol() + 0.5) / scale - (showcol + 0.5) / 2 + m_X;											//以图象中部开始计算，向左向上各一半缓冲区距离，再考虑鼠标移动距离
	start_Y = (raw->getRow() + 0.5) / scale - (showrow + 0.5) / 2 + m_Y;											//鼠标移动距离有正有负

	if (start_X <= 0) { start_X = 0; }																				//当x小于图像边界时，直接取0
	if (start_Y <= 0) { start_Y = 0; }																				//当y小于图像边界时，直接取0
	if (start_X + showcol * scale >= raw->getCol()) { start_X = raw->getCol() - showcol * scale; }
	if (start_Y + showrow * scale >= raw->getRow()) { start_Y = raw->getRow() - showrow * scale; }



	if (band2 == -1)
	{
		band = 1;
		bandsinf[0] = band1;
		bandsinf[1] = band1;
		bandsinf[2] = band1;

		data[0] = new unsigned short[showrow * showcol];
		data[1] = nullptr;//data[0];
		data[2] = nullptr;// data[0];

		GDALRasterBand* poBand;
		poBand = poDataset->GetRasterBand(band1 + 1);

		if (isfloat)
		{
			float* temp = new float[showrow * showcol];
			poBand->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, temp, showcol, showrow, GDT_Float32, 0, 0);
			float2uchort(temp, data[0]);
			delete temp;
		}
		else
		{
			poBand->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, data[0], showcol, showrow, GDT_UInt16, 0, 0);
		}
		

		
	}
	else
	{
		band = 3;
		bandsinf[0] = band1;
		bandsinf[1] = band2;
		bandsinf[2] = band3;

		data[0] = new unsigned short[showrow * showcol];
		data[1] = new unsigned short[showrow * showcol];
		data[2] = new unsigned short[showrow * showcol];

		GDALRasterBand* poBand1;
		GDALRasterBand* poBand2;
		GDALRasterBand* poBand3;

		poBand1 = poDataset->GetRasterBand(band1 + 1);
		poBand2 = poDataset->GetRasterBand(band2 + 1);
		poBand3 = poDataset->GetRasterBand(band3 + 1);

		
		if (isfloat)
		{
			float* t1 = new float[showrow * showcol];
			poBand1->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, t1, showcol, showrow, GDT_Float32, 0, 0);

			float2uchort(t1, data[0]);
			delete []t1;

			float* t2 = new float[showrow * showcol];
			poBand2->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, t2, showcol, showrow, GDT_Float32, 0, 0);
			float2uchort(t2, data[1]);
			delete []t2;

			float* t3 = new float[showrow * showcol];
			poBand3->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, t3, showcol, showrow, GDT_Float32, 0, 0);
			float2uchort(t3, data[2]);
			delete []t3;
		}
		else
		{
			poBand1->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, data[0], showcol, showrow, GDT_UInt16, 0, 0);
			poBand2->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, data[1], showcol, showrow, GDT_UInt16, 0, 0);
			poBand3->RasterIO(GF_Read, start_X, start_Y, showcol * scale, showrow * scale, data[2], showcol, showrow, GDT_UInt16, 0, 0);
		}

	}
	is_loaded = true;
	return true;
	
}

bool ShowImage::loadCurrent()
{
	loadData(bandsinf[0], bandsinf[1], bandsinf[2]);
	return false;
}

bool ShowImage::clear()
{
	if (band == 1)
	{
		if (data[0] != nullptr)
		{
			delete data[0];
			for (int i = 0; i < data.size(); ++i)
			{
				data[i] = nullptr;
			}
		}
	}

	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i] != nullptr)
		{
			delete data[i];
			data[i] = nullptr;
		}
	}
	is_loaded = false;
	return true;
}

unsigned short ShowImage::at(int theband, int therow, int thecol)
{
	return data[theband][showcol * therow + thecol];
}

bool ShowImage::float2uchort(float* f, unsigned short* s)
{
	double themin = 10000000000;
	double themax = -10000000000;

	for (int i = 0; i < showcol * showrow; ++i)
	{
		if (themin > f[i]) { themin = f[i]; }
		if (themax < f[i]) { themax = f[i]; }
	}

	for (int i = 0; i < showcol * showrow; ++i)
	{
		s[i] = (65535 / (themax - themin)) * f[i] - (65535 * themin) / (themax - themin) + 0.5;					//将 float 由最大最小值重新 离散化 到 unsigned short的区间
	}

	return true;
}
