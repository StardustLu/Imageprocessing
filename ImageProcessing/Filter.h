#pragma once
#include "DataManage.h"
#include <map>
#include <iostream>

class Filter
{
public:
	Filter(DataManage* tm) :manager(tm) {}
	~Filter() {}

	Image* majority(Image*, string& opath, int size = 3, int band = 0);


private:

	double getMapMax(std::map<int, int>&);

	DataManage* manager;

};

