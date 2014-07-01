#include "StdAfx.h"
#include "D3D9Shader.h"
#include "EngineConfig.h"

namespace Dream
{

D3D9Shader::D3D9Shader() : IShader()
	, m_effect(NULL)
{

}


D3D9Shader::~D3D9Shader()
{
	SAFE_RELEASE(m_effect);
}

ITechnique* D3D9Shader::GetTechnique(const char * name)
{
	D3DXHANDLE hTech = m_effect->GetTechniqueByName("RenderSceneWithTexture1Light");
	
	return NULL;

}

void D3D9Shader::SetShaderParam(const char* varName, void* data, PARAMTYPE type)
{
	switch(type)
	{
	case IShader::eFloat:
		m_effect->SetVector( "g_Vector4", (D3DXVECTOR4*)data);
		break;
	case IShader::eTexture:
		m_effect->SetTexture( "g_Texture", (LPDIRECT3DBASETEXTURE9)data);
		break;
	case IShader::eMatrix:
		m_effect->SetMatrix("g_mWorld", (D3DXMATRIX*)data);
		break;
	}

}

}
