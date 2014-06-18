#pragma once

#include "ITextureManager.hpp"
#include "IEngine.hpp"

namespace Dream
{
	class ITexture;

	class TextureManager : public ITextureManager
	{
	public:
		TextureManager(){}
		virtual ~TextureManager(){}

		virtual bool Init(IEngine* ){ return gEngine; }
		virtual void Destroy(){}

		virtual ITexture* CreateTexture(const char* name);
	};
}