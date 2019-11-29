
#include "core/Model.h"

namespace peak
{
	Model::Model() : Object3D()
	{
	}
	Model::~Model()
	{
	}
	
	bool Model::load(std::string name, bool graphics, bool physics)
	{
		return true;
	}
	bool Model::destroy(void)
	{
		return false;
	}
}

