#include "DataManage.h"

DataManage::~DataManage()
{
	for (int i = 0; i < image_list.size(); ++i)
	{
		cancel(i);
	}
}

Image* DataManage::create()
{
	Image* temp = new Image;
	image_list.push_back(temp);

	Statistic* stemp = new Statistic(temp);
	sta_list.push_back(stemp);

	ShowImage* showtemp = new ShowImage(temp);
	show_list.push_back(showtemp);

	Enhance* etemp = new Enhance(showtemp);
	enhance_list.push_back(etemp);

	current = image_list.size() - 1;

	return temp;
}

bool DataManage::cancel(int num)
{
	if (image_list[num] != nullptr)
	{
		delete image_list[num];
		image_list[num] = nullptr;
	}

	if (sta_list[num] != nullptr)
	{
		delete sta_list[num];
		sta_list[num] = nullptr;
	}

	if (show_list[num] != nullptr)
	{
		delete show_list[num];
		show_list[num] = nullptr;
	}

	if (enhance_list[num] != nullptr)
	{
		delete enhance_list[num];
		enhance_list[num] = nullptr;
	}

	image_list.erase(image_list.cbegin() + num);
	sta_list.erase(sta_list.cbegin() + num);
	show_list.erase(show_list.cbegin() + num);
	enhance_list.erase(enhance_list.cbegin() + num);

	return true;
}

bool DataManage::cancel(Image* the)
{
	int t = find(the);
	cancel(t);
	return true;
}

int DataManage::find(Image* the)
{
	for (int i = 0; i < image_list.size(); ++i)
	{
		if (the == image_list[i]) { return i; }
	}

	return -1;
}

bool DataManage::setCurrent(Image* the)
{
	int t = find(the);
	if (t == -1) { return false; }
	current = t;
	return true;
}

Statistic* DataManage::getSta(Image* the)
{
	int t = find(the);

	if (t == -1) { return nullptr; }

	return sta_list[t];
}

ShowImage* DataManage::getshow(Image* the)
{
	int t = find(the);
	if (t == -1) { return nullptr; }

	return show_list[t];
}

Enhance* DataManage::getEnh(Image* the)
{
	int t = find(the);

	if (t == -1) { return nullptr; }

	return enhance_list[t];
}
