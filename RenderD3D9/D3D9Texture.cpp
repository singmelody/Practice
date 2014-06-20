#include "StdAfx.h"
#include "D3D9Texture.h"
#include "EngineConfig.h"
namespace Dream
{

D3D9Texture::D3D9Texture(IDirect3DTexture9* tex)
{
	m_tex = NULL;
}


D3D9Texture::~D3D9Texture()
{
	SAFE_RELEASE(m_tex);
}

}
