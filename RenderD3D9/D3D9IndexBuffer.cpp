#include "StdAfx.h"
#include "D3D9IndexBuffer.h"
#include "EngineConfig.h"

namespace Dream{

D3D9IndexBuffer::~D3D9IndexBuffer(void)
{
	SAFE_RELEASE(m_ib);
}

bool D3D9IndexBuffer::Fill(const void* ptr, int size)
{
	void* pIndices;
	if(FAILED(m_ib->Lock( 0, size, (void**)&pIndices, 0)))
		return false;
	
	memcpy( pIndices, ptr, size);

	m_ib->Unlock();

	return true;
}

}