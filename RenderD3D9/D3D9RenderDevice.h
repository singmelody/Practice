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

		virtual bool Init(const void* wnd);
		virtual void Destroy();

		virtual bool CheckCaps();
		virtual bool CreateDevice();

		virtual bool BeginScene();
		virtual void EndScene();

		virtual void Present(const void* mainWnd);

		virtual void SetVertexBuffer(int streamIndex, const IVertexBuffer* vb, int offsetIdx) = 0;
		virtual void SetIndexBuffer(const IIndexBuffer* ib) = 0;

		virtual ITexture* CreateTexture(const char* path);

		// D3D9 section
		bool Clear( DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color,float Z, DWORD Stencil );


	private:
		HWND				m_hwnd;
		DWORD				m_devBehaviorFlags;

		IDirect3DDevice9*	m_d3d9Device;
		IDirect3D9*			m_d3d9Object;
	};

}
