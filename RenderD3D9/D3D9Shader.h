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

	virtual void SetShaderParam(const char* varName, void* data, PARAMTYPE type);
protected:
	ID3DXEffect*	m_effect;
};

}
