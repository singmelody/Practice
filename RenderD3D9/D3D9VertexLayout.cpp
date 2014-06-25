#include "StdAfx.h"
#include "D3D9VertexLayout.h"
#include "EngineConfig.h"

namespace Dream{


D3D9VertexLayout::~D3D9VertexLayout(void)
{
	SAFE_RELEASE(m_decl);
}

void D3D9VertexLayout::AddSignature(WORD streamIdx, WORD Offset, BYTE type, BYTE method, BYTE usage, BYTE usageIdx)
{
	m_elements
}

void D3D9VertexLayout::Build()
{
	int count = m_elements.size();
	D3DVERTEXELEMENT9*	elements = new D3DVERTEXELEMENT9[count];

	for (int i = 0; i < count; ++i)
		elements[i] = m_elements[i];

	

	delete[] elements;
}

}