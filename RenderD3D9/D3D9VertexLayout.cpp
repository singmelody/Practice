#include "StdAfx.h"
#include "D3D9VertexLayout.h"
#include "D3D9RenderDevice.h"
#include "IRenderer.hpp"
#include "EngineConfig.h"

namespace Dream{


D3D9VertexLayout::~D3D9VertexLayout(void)
{
	SAFE_RELEASE(m_decl);
}

void D3D9VertexLayout::AddSignature(WORD streamIdx, WORD Offset, DATATYPE type, METHOD method, VERTEXUSAGE usage, BYTE usageIdx)
{
	D3DVERTEXELEMENT9 element;
	element.Stream = streamIdx;
	element.Offset = Offset;
	element.Type = ConvertDataType(type);
	element.Method = ConvertMethod(method);
	element.Usage = ConvertVertexUsage(usage);
	element.UsageIndex = usageIdx;

	m_vertexByteSize = Offset + GetDataTypeSize(type);

	m_elements.push_back(element);
}

BYTE D3D9VertexLayout::ConvertMethod(METHOD method)
{
	switch(method)
	{
	case IVertexLayout::DEFAULT:
		return D3DDECLMETHOD_DEFAULT;
		break;
	
	}

	return D3DDECLMETHOD_DEFAULT;
}


BYTE D3D9VertexLayout::ConvertDataType(DATATYPE dataType)
{
	switch(dataType)
	{
	case FLOAT3:
		return D3DDECLTYPE_FLOAT3;
		break;
	case FLOAT2:
		return D3DDECLTYPE_FLOAT2;
		break;
	case DWORD:
		return D3DDECLTYPE_D3DCOLOR;
		break;

	}

	return D3DDECLTYPE_UNUSED;
}

BYTE D3D9VertexLayout::ConvertVertexUsage(VERTEXUSAGE usage)
{
	switch(usage)
	{
	case POSITION:
		return D3DDECLUSAGE_POSITION;
		break;
	case TEXCOORD:
		return D3DDECLUSAGE_POSITION;
		break;
	case COLOR:
		return D3DDECLUSAGE_COLOR;
		break;
	}

	return D3DDECLUSAGE_POSITION;
}

void D3D9VertexLayout::Build()
{
	int count = m_elements.size();
	D3DVERTEXELEMENT9*	elements = new D3DVERTEXELEMENT9[count];

	for (int i = 0; i < count; ++i)
		elements[i] = m_elements[i];

	D3D9RenderDevice* d3d9Device = static_cast<D3D9RenderDevice*>(gRenderer->GetIRenderDevice());
	d3d9Device->CreateVertexDeclaration( elements, &m_decl);

	delete[] elements;
}

int D3D9VertexLayout::GetDataTypeSize(DATATYPE type)
{
	switch(type)
	{
	case FLOAT3:
		return 12;
		break;
	case FLOAT2:
		return 8;
		break;
	case DWORD:
		return 4;
		break;
	}

	return 0;
}


}