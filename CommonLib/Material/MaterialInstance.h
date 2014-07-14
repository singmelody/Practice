#pragma once

#include "IMaterial.hpp"

namespace Dream {

class MaterialInstance : public IMaterial
{
public:
	MaterialInstance();
	virtual ~MaterialInstance();

	virtual bool Load(){ return true; }
protected:

};

}