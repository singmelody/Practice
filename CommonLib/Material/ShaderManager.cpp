#include "stdafx.h"
#include "ShaderManager.h"
#include "IRenderer.hpp"
#include "IRenderDevice.hpp"
#include <IEngine.hpp>
DEFINE_ENGINE_INTERFACE

namespace Dream
{
	IShader* ShaderManager::CreateShader(const char* name)
	{
		IRenderer* renderer = gEngine->GetIRenderer();
		IRenderDevice* device = renderer->GetIRenderDevice();


		IShader* shader = device->CreateShader(name);
		if(!shader)
			return NULL;

		m_shaderDic.insert(std::make_pair( name, shader));
		return shader;
	}

	bool ShaderManager::Init(IEngine* engine)
	{
		gEngine = engine;
		return true;
	}

}