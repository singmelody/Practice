#pragma once
#include "IIndexBuffer.hpp"
#include <d3dx9.h>
namespace Dream
{

class D3D9IndexBuffer : public IIndexBuffer
{
public:
	D3D9IndexBuffer(IDirect3DIndexBuffer9* ib) : m_ib(ib) {}
	virtual ~D3D9IndexBuffer();

	IDirect3DIndexBuffer9* GetD3D9IndexBuffer() const { return m_ib; }

	virtual bool Fill(const void* ptr, int size);
protected:
	IDirect3DIndexBuffer9* m_ib;
};

}