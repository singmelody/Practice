#pragma once
#include <d3dx9.h>
#include <DxErr.h>
#include "IRenderer.hpp"

namespace Dream{

class IRenderDevice;
class D3D9RenderDevice;
class IVertexBuffer;
class IIndexBuffer;
class IShader;
class GlobalShader;

class D3D9Renderer : public IRenderer
{
public:
	D3D9Renderer();
	virtual ~D3D9Renderer();

	virtual bool Init(const void* wnd, IEngine* engine);
	virtual void Destroy();

	virtual IRenderDevice* GetIRenderDevice();

	virtual void Update(float deltaTime);
	virtual void Render();
	virtual void RenderCallBack();

	virtual void SetMaterial(IMaterial* material) ;

	int m_firstVertex;
	int m_vertexCount;
	int m_firstIdx;
	int m_idxCount;

private:

	void InitCube();

	IMaterial*		  m_curMaterial;
	GlobalShader*	  m_globalShader;
	D3D9RenderDevice* m_device;


	IVertexBuffer*	m_vb;
	IIndexBuffer*	m_ib;
};

}