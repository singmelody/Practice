#include "StdAfx.h"
#include "D3D9Technique.h"
#include "EngineConfig.h"
#include "IPass.hpp"

namespace Dream
{

D3D9Technique::D3D9Technique(D3DXHANDLE handle) : ITechnique()
	,m_handle(handle)
{

}


D3D9Technique::~D3D9Technique()
{
}

void D3D9Technique::Render(const char* techName)
{
	for (int i = 0; i < m_passes.size(); ++i)
	{
		m_passes[i]->BeginRender(&m_handle, i);

		m_passes[i]->Render( &m_handle );

		m_passes[i]->EndRender( &m_handle);
	}
}


}
