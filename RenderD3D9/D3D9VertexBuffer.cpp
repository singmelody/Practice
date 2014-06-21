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

}