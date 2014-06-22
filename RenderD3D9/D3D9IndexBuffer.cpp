#include "StdAfx.h"
#include "D3D9IndexBuffer.h"
#include "EngineConfig.h"

namespace Dream{

D3D9IndexBuffer::~D3D9IndexBuffer(void)
{
	SAFE_RELEASE(m_ib);
}

void D3D9IndexBuffer::Fill(const void* ptr, int size)
{
	void* pIndices;
	m_ib->Lock( 0, size, (void**)&pIndices, 0);
	
	memcpy( pIndices, ptr, size);

	m_ib->Unlock();
}

}