#include "StdAfx.h"
#include "D3D9Pass.h"
#include "EngineConfig.h"
#include "IEngine.hpp"
DEFINE_ENGINE_INTERFACE

#include "D3D9Renderer.h"

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

void D3D9Pass::Render(void *handle)
{
	IRenderer* renderer = gEngine->GetIRenderer();

	D3D9Renderer* renderer = static_cast<D3D9Renderer*>(renderer);

	renderer->Render();
}

void D3D9Pass::EndRender(void *handle)
{
	ID3DXEffect* effect = static_cast<ID3DXEffect*>(handle);
	effect->EndPass();
}

}
