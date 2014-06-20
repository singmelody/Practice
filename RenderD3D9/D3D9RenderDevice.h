#pragma once
#include "IRenderDevice.hpp"
#include <d3dx9.h>
namespace Dream
{
	class ITexture;

	class D3D9RenderDevice : public IRenderDevice
	{
	public:
		D3D9RenderDevice();
		virtual ~D3D9RenderDevice();

		virtual bool CreateDevice(const void* mainWnd);
		virtual bool CheckCaps();

		virtual ITexture* CreateTexture(const char* path);
	private:
		DWORD				m_devBehaviorFlags;

		IDirect3DDevice9*	m_d3d9Device;
		IDirect3D9*			m_d3d9Object;
	};

}
