#pragma once
#include "ITechnique.hpp"
#include <d3dx9.h>
namespace Dream
{

class IPass;

class D3D9Technique : public ITechnique
{
public:
	D3D9Technique(D3DXHANDLE handle)
	virtual ~D3D9Technique();

	virtual void Render(const char* techName);
protected:
	D3DXHANDLE	m_handle;

};

}
