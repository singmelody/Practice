#pragma once
#include <d3dx9.h>
#include <DxErr.h>
#include "IRenderer.hpp"

namespace Dream{

class IRenderDevice;
class D3D9RenderDevice;
class IVertexBuffer;
class IIndexBuffer;

class D3D9Renderer : public IRenderer
{
public:
	D3D9Renderer();
	virtual ~D3D9Renderer();

	virtual bool Init(const void* wnd);
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render();

	virtual IRenderDevice* GetIRenderDevice();
private:

	void InitCube();

	D3D9RenderDevice* m_device;

	IVertexBuffer*	m_vb;
	IIndexBuffer*	m_ib;
};

}