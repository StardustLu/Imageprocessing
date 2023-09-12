#include "OtsuSeg.h"

bool OtsuSeg::initialize()
{
	sta = tmanager->getSta(img);
	sta->initialize();
	staTable = sta->numVec();

	threshold.resize(img->getBand());
	var_class.resize(img->getBand());

	for (int i = 0; i < img->getBand(); ++i)
	{
		threshold[i] = 0;
		var_class[i] = 0;
	}

	return true;
}

int OtsuSeg::getThreshold(int band_1)
{
	return threshold[band_1];
}

Image* OtsuSeg::splitit(string& path, int band_1)
{
	if (threshold.empty()) { return nullptr; }

	Image* segimg = tmanager->create();

	//这里主要采用了分块读写

	if (band_1 != -1)
	{
		segimg->create(1, img->getRow(), img->getCol(), img, path);

		for (int i = 0; i < img->getBlockRow(); ++i)
		{
			for (int j = 0; j < img->getBlockCol(); ++j)
			{
				img->loadDatabyBlock(i, j, band_1);
				segimg->prepareDatabyBlock(i, j, 0);

				for (int a = 0; a < img->getEachBlockRow(); ++a)
				{
					for (int b = 0; b < img->getEachBlockCol(); ++b)
					{
						if (img->atBlock(band_1, a, b) < threshold[band_1])
						{
							segimg->setBlock(0, a, b, 0);
						}
						else
						{
							segimg->setBlock(0, a, b, img->atBlock(band_1, a, b));
						}

					}
				}
				segimg->saveDatabyBlock(i, j, 0);
				segimg->clear();
				img->clear(band_1);

			}
		}
	}
	else
	{
		segimg->create(img->getBand(), img->getRow(), img->getCol(), img, path);

		for (int i = 0; i < img->getBlockRow(); ++i)
		{
			for (int j = 0; j < img->getBlockCol(); ++j)
			{
				for (int k = 0; k < img->getBand(); ++k)
				{
					img->loadDatabyBlock(i, j, k);
					segimg->prepareDatabyBlock(i, j, k);

					for (int a = 0; a < img->getEachBlockRow(); ++a)
					{
						for (int b = 0; b < img->getEachBlockCol(); ++b)
						{
							if (img->atBlock(k, a, b) < threshold[k])
							{
								segimg->setBlock(k, a, b, 0);
							}
							else
							{
								segimg->setBlock(k, a, b, 1);
							}
						}
					}
					segimg->saveDatabyBlock(i, j, k);
					segimg->clear();
					img->clear();
				}

			}
		}
	}

	return segimg;
}

/*
*	这里采取了一个抽稀优化。本算法利用直方图进行统计计算，这个直方图由该影像的 Statistic 对象负责计算和储存
*	但直方图 DN 包括 65562 个值（ushort），多次遍历效率较低，本算法以256为一个单位进行阈值查找，找到一个阈值后，再在该阈值 +-256的位置进行查找，如此效率较高
*/

bool OtsuSeg::calThreshold(int band_1)
{
	if (band_1 == -1)
	{
		for (int b = 0; b < img->getBand(); ++b)
		{
			int themin = sta->min(b);
			int themax = sta->max(b);

			for (int i = themin; i <= themax; i += 256)
			{
				double tempVar = calPartVar(b, i);
				if (tempVar > var_class[b]) { threshold[b] = i; var_class[b] = tempVar; }
			}

			int tempthreshold = threshold[b];

			for (int i = tempthreshold - 256; i < tempthreshold + 256; ++i)
			{
				double tempVar = calPartVar(b, i);
				if (tempVar > var_class[b]) { threshold[b] = i; var_class[b] = tempVar; }

			}

		}
	}
	else
	{
		int themin = sta->min(band_1);
		int themax = sta->max(band_1);

		int process = 0;
		int lastprocess = 0;

		for (int i = themin + 1; i <= themax; i += 256)
		{
			double tempVar = calPartVar(band_1, i);
			if (tempVar > var_class[band_1]) { threshold[band_1] = i; var_class[band_1] = tempVar; }

		}

		int tempthreshold = threshold[band_1];

		for (int i = tempthreshold - 256; i < tempthreshold + 256; ++i)
		{
			double tempVar = calPartVar(band_1, i);
			if (tempVar > var_class[band_1]) { threshold[band_1] = i; var_class[band_1] = tempVar; }
		}

	}

	return true;
}

double OtsuSeg::calPartVar(int band_1, int thres)
{
	long long lsum = 0, rsum = 0;
	double ldivisor = 0, rdivisor = 0;
	for (int i = sta->min(band_1); i <= sta->max(band_1); ++i)
	{
		if (i < thres)
		{
			lsum += (*staTable)[band_1][i] * i;
			ldivisor += (*staTable)[band_1][i];

		}
		else
		{
			rsum += (*staTable)[band_1][i] * i;
			rdivisor += (*staTable)[band_1][i];
		}
	}



	double lmean = lsum / ldivisor;
	double rmean = rsum / rdivisor;
	double lomega = ldivisor / (ldivisor + rdivisor);
	double romega = rdivisor / (ldivisor + rdivisor);

	return lomega * romega * (lmean - rmean) * (lmean - rmean);

}
