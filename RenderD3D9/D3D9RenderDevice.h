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

		virtual void Draw(size_t startVertex,size_t vertexCount,size_t startIdx,size_t idxCount);

		virtual void Present();

		virtual void SetVertexBuffer(int streamIndex, const IVertexBuffer* vb, int offsetIdx);
		virtual void SetIndexBuffer(const IIndexBuffer* ib);

		virtual ITexture* CreateTexture(const char* path);

		// D3D9 section
		bool CreateVertexBuffer(UINT length,DWORD usage,DWORD fvf,D3DPOOL pool,IDirect3DVertexBuffer9** vertexBuffer,HANDLE* sharedHandle);
		bool CreateIndexBuffer(UINT length,DWORD usage,D3DFORMAT format,D3DPOOL pool,IDirect3DIndexBuffer9** indexBuffer,HANDLE* sharedHandle);
		bool CreateVertexDeclaration(D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
		


		bool Clear( DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color,float Z, DWORD Stencil );


	protected:
		HWND				m_hwnd;
		DWORD				m_devBehaviorFlags;

		IDirect3DDevice9*	m_d3d9Device;
		IDirect3D9*			m_d3d9Object;
	};

}
