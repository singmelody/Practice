#include "StdAfx.h"
#include "D3D9IndexBuffer.h"
#include "EngineConfig.h"

namespace Dream{

D3D9IndexBuffer::~D3D9IndexBuffer(void)
{
	SAFE_RELEASE(m_ib);
}

}