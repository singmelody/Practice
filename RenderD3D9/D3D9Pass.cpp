#include "StdAfx.h"
#include "D3D9Pass.h"
#include "EngineConfig.h"

namespace Dream
{

D3D9Pass::D3D9Pass() : IPass()
	, m_pass(NULL)
{

}


D3D9Pass::~D3D9Pass()
{
}



RenderStateDesc* D3D9Pass::GetStateDesc()
{
	return NULL;
}

void D3D9Pass::BeginRender(void *handle, int i)
{
	ID3DXEffect* effect = static_cast<ID3DXEffect*>(handle);
	effect->BeginPass(i);
}

void D3D9Pass::EndRender(void *handle)
{
	ID3DXEffect* effect = static_cast<ID3DXEffect*>(handle);
	effect->EndPass();
}

}
