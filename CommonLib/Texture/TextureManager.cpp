#include "stdafx.h"
#include "TextureManager.h"

namespace Dream
{
	ITexture* TextureManager::CreateTexture(const char* name)
	{
		IRenderer* renderer = gEngine->GetIRenderer();
		D3DXCreateTextureFromFile( renderer, str, tex);
	}

}