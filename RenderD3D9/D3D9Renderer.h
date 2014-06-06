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

	virtual bool Init(const HWND mainHwnd, IRenderer** renderer);
	virtual void Destroy();

	virtual void Update();
	virtual void Render();

private:
	IDirect3DDevice9*	m_d3d9Device;
	IDirect3D9*			m_d3d9Object;
};

}