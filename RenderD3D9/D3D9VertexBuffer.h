#pragma once
#include "IVertexBuffer.hpp"
namespace Dream{

class D3D9VertexBuffer : public IVertexBuffer
{
public:
	D3D9VertexBuffer(IDirect3DVertexBuffer9* vb);
	virtual ~D3D9VertexBuffer();

	IDirect3DVertexBuffer9*	GetD3D9VertexBuffer() const { return m_vb; }

	// fill
	virtual bool Fill(const void* ptr, int size);
protected:
	IDirect3DVertexBuffer9* m_vb;
};

}