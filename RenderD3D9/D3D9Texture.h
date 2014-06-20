#pragma once
#include "ITexture.hpp"
#include <d3dx9.h>

namespace Dream
{

class D3D9Texture : public ITexture
{
public:
	D3D9Texture(IDirect3DTexture9* tex);
	virtual ~D3D9Texture();

private:
	IDirect3DTexture9* m_tex;
};

}