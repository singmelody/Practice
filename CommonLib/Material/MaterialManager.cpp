#include "stdafx.h"
#include "MaterialManager.h"
#include "IRenderer.hpp"
#include "IRenderDevice.hpp"
#include <IEngine.hpp>

namespace Dream
{
	IMaterial* MaterialManager::CreateMaterial(const char* name)
	{
		IRenderer* renderer = gEngine->GetIRenderer();
		IRenderDevice* device = renderer->GetIRenderDevice();


		IMaterial* material = device->CreateMaterial(name);
		if(!material)
			return NULL;

		m_materialDic.insert(std::make_pair( name, material));
		return material;
	}

	bool MaterialManager::Init(IEngine* engine)
	{
		gEngine = engine;
		return true;
	}

}