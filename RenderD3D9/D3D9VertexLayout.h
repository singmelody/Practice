#pragma once

#include "IVertexLayout.hpp"
#include <d3dx9.h>

namespace Dream{

class D3D9VertexLayout : public IVertexLayout
{
public:
	D3D9VertexLayout(IDirect3DVertexDeclaration9* decl) : m_decl(decl){}
	virtual ~D3D9VertexLayout();

	IDirect3DVertexDeclaration9* GetD3D9VertexDeclaration() const { return m_decl; }
protected:
	IDirect3DVertexDeclaration9* m_decl;
};

}