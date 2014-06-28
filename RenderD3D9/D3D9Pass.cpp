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

}
