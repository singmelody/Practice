#pragma once
#include "IRenderDevice.h"
#include <d3dx9.h>
namespace Dream
{
	class D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();
		virtual ~D3D9RenderDevice();

		bool CreateTexture();
	private:
		IDirect3DDevice9*	m_d3d9Device;
		IDirect3D9*			m_d3d9Object;
	};

}
