#include "ToolManage.h"

ToolManage::~ToolManage()
{
	for (int i = 0; i < PCA_list.size(); ++i)
	{
		if (PCA_list[i] != nullptr)
		{
			delete PCA_list[i];
		}
	}

}
