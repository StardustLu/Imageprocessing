#pragma once
#include "DataManage.h"
#include "PCATrans.h"
#include <vector>

using namespace std;

class ToolManage
{
public:

	ToolManage();
	~ToolManage();

	bool createPCA(DataManage*,Image*);
	bool cancelPCA(Image*);


private:

	vector<PCATrans*> PCA_list;


};

