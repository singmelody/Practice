#pragma once

#include "IVertexLayout.hpp"
#include <d3dx9.h>
#include <vector>

namespace Dream{

class D3D9VertexLayout : public IVertexLayout
{
public:
	D3D9VertexLayout(){  }
	virtual ~D3D9VertexLayout(){ }

	virtual void AddSignature(WORD streamIdx, WORD Offset, BYTE type, BYTE method, BYTE usage, BYTE usageIdx);
	virtual void Build();

	IDirect3DVertexDeclaration9* GetD3D9VertexDeclaration() const { return m_decl; }
protected:
	std::vector<D3DVERTEXELEMENT9>	m_elements;
	IDirect3DVertexDeclaration9*	m_decl;
};

}