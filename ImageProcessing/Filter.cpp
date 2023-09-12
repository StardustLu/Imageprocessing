#include "Filter.h"
#include "Filter.h"

Image* Filter::majority(Image* img, string& opath, int size, int band)
{
	Image* result = manager->create();
	result->create(img->getBand(), img->getRow(), img->getCol(), img, opath);

	img->loadData(band);
	result->prepareData(band);

	int c, d;
	int temp;
	for (int i = 0; i < img->getRow(); i += size)
	{
		for (int j = 0; j < img->getCol(); j += size)
		{
			std::map<int, int> sta_map;
			for (int a = 0; a < size; ++a)
			{
				for (int b = 0; b < size; ++b)
				{
					c = a;
					d = b;
					if (i + c >= img->getRow()) { c = -a; }
					if (j + d >= img->getCol()) { d = -b; }

					temp = img->at(band, i + c, j + d);

					sta_map[temp] = 0;

				}
			}

			for (int a = 0; a < size; ++a)
			{
				for (int b = 0; b < size; ++b)
				{
					c = a;
					d = b;
					if (i + c >= img->getRow()) { c = -a; }
					if (j + d >= img->getCol()) { d = -b; }

					temp = img->at(band, i + c, j + d);
					//cout << sta_map[temp] << endl;
					sta_map[temp]++;
					//cout << sta_map[temp] << endl;

				}
			}

			int maxDN = getMapMax(sta_map);

			for (int a = 0; a < size; ++a)
			{
				for (int b = 0; b < size; ++b)
				{
					c = a;
					d = b;
					if (i + c >= img->getRow()) { c = -a; }
					if (j + d >= img->getCol()) { d = -b; }

					result->set(band, i + c, j + d, maxDN);

				}
			}

		}
	}

	result->saveData(band);
	img->clear();

	return result;
}

double Filter::getMapMax(std::map<int, int>& temp)
{
	std::map<int, int>::iterator iter;
	std::map<int, int>::iterator maxiter = temp.begin();
	int maxnum = 0;

	for (iter = temp.begin(); iter != temp.end(); ++iter)
	{
		if (iter->second > maxnum)maxiter = iter;
	}

	return maxiter->first;
}
