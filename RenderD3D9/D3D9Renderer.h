#pragma once
#include <d3dx9.h>
#include <DxErr.h>
#include "IRenderer.hpp"

namespace Dream{

class D3D9Renderer : public IRenderer
{
public:
	D3D9Renderer();
	virtual ~D3D9Renderer();

	virtual bool Init(const HWND mainHwnd);
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render();

	virtual IRenderDevice* GetIRenderDevice();
private:
	void InitCube();

	IDirect3DDevice9*	m_d3d9Device;
	IDirect3D9*			m_d3d9Object;

	IDirect3DVertexBuffer9* m_vb;
	IDirect3DIndexBuffer9*	m_ib;

};

}