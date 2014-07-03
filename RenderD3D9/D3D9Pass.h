#pragma once
#include "IPass.hpp"
#include <d3dx9.h>
namespace Dream
{

class RenderStateDesc;

class D3D9Pass : public IPass
{
public:
	D3D9Pass();
	virtual ~D3D9Pass();

	//IPass* GetPass(int idx);
	RenderStateDesc*	GetStateDesc();

	virtual void BeginRender(void *handle, int i);
	virtual void Render(void *handle);
	virtual void EndRender(void *handle);

protected:
	D3DXHANDLE	m_pass;
};

}
