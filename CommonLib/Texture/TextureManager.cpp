#include "stdafx.h"
#include "TextureManager.h"
#include "IRenderer.hpp"
#include "IRenderDevice.hpp"
#include <IEngine.hpp>
DEFINE_ENGINE_INTERFACE

namespace Dream
{
	ITexture* TextureManager::CreateTexture(const char* name)
	{
		IRenderer* renderer = gEngine->GetIRenderer();
		IRenderDevice* device = renderer->GetIRenderDevice();


		ITexture* tex = device->CreateTexture(name);
		if(!tex)
			return NULL;

		m_texDic.insert(std::make_pair( name, tex));
		return tex;
	}

	bool TextureManager::Init(IEngine* engine)
	{
		gEngine = engine;
		return true;
	}

}