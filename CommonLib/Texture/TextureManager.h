#pragma once

#include "ITextureManager.hpp"

namespace Dream
{
	class ITexture;
	class IEngine;

	class TextureManager : public ITextureManager
	{
	public:
		TextureManager(){}
		virtual ~TextureManager(){}

		virtual bool Init(IEngine* );
		virtual void Destroy(){}

		virtual ITexture* CreateTexture(const char* name);
	};
}