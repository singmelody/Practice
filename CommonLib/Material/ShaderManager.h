#pragma once

#include "IShaderManager.hpp"

namespace Dream
{
	class IShader;
	class IEngine;

	class ShaderManager : public IShaderManager
	{
	public:
		ShaderManager(){}
		virtual ~ShaderManager(){}

		virtual bool Init(IEngine* );
		virtual void Destroy(){}

		virtual IShader* CreateShader(const char* name);
	};
}