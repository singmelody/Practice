#pragma once

namespace Dream
{
	class ITexture;

	class TextureManager
	{
	public:
		virtual ITexture* CreateTexture(const char* name);
	};
}