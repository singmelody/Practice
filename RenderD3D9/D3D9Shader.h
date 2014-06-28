#pragma once
#include "IShader.hpp"
#include <d3dx9.h>
namespace Dream
{

class D3D9Shader : public IShader
{
public:
	D3D9Shader();
	virtual ~D3D9Shader();

	ITechnique* GetTechnique(const char * name);
protected:
	ID3DXEffect*	m_effect;
};

}
