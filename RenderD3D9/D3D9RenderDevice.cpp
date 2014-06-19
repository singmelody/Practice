#include "StdAfx.h"
#include "D3D9RenderDevice.h"

namespace Dream
{
	D3D9RenderDevice::D3D9RenderDevice()
	{
	}


	D3D9RenderDevice::~D3D9RenderDevice()
	{
	}

	// we will add freeimage later
	ITexture* D3D9RenderDevice::CreateTexture(const char* path)
	{
		IDirect3DTexture9* d3d9Tex;
		D3DXCreateTextureFromFile( m_d3d9Device, path, &d3d9Tex);

	 	ITexture* tex =  new D3D9Texture(d3d9Tex);
		return tex;
		// m_d3d9Device->D3DXCreateTextureFromFile( , str, tex);
	}

}
