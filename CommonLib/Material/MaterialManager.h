#pragma once

#include "IMaterialManager.hpp"

namespace Dream
{
	class IShader;
	class IEngine;

	class MaterialManager : public IMaterialManager
	{
	public:
		MaterialManager(){}
		virtual ~MaterialManager(){}

		virtual bool Init(IEngine* );
		virtual void Destroy(){}

		virtual IMaterial* CreateMaterial(const char* name);
	};
}