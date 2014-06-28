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
}

}
