#include "StdAfx.h"
#include "D3D9VertexBuffer.h"
#include "EngineConfig.h"

namespace Dream
{

D3D9VertexBuffer::D3D9VertexBuffer(IDirect3DVertexBuffer9* vb)
	: m_vb(vb)
{

}


D3D9VertexBuffer::~D3D9VertexBuffer()
{
	SAFE_RELEASE(m_vb);
}

bool D3D9VertexBuffer::Fill(const void* ptr, int size)
{
	void* pVertices;
	if(FAILED(m_vb->Lock( 0, size, ( void** )&pVertices, 0)))
		return false;

	memcpy( pVertices, ptr, size);
	
	m_vb->Unlock();

	return true;
}

}