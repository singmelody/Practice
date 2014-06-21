#include "StdAfx.h"
#include "D3D9VertexLayout.h"
#include "EngineConfig.h"

namespace Dream{


D3D9VertexLayout::~D3D9VertexLayout(void)
{
	SAFE_RELEASE(m_decl);
}

}