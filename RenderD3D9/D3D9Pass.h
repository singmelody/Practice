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
protected:
	D3DXHANDLE	m_pass;
};

}
