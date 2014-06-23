#pragma once
#include <d3dx9.h>

namespace Dream
{
	class IVertexLayout;

	class IVertexBuffer
	{
	public:
		IVertexBuffer(){}
		virtual ~IVertexBuffer(){}

		// get the vertex layout
		IVertexLayout* GetIVertexLayout() const { return m_vertexLayout; }
		void SetIVertexLayout(IVertexLayout* layout) { m_vertexLayout = layout; }

		// fill
		virtual bool Fill(const void* ptr, int size) = 0;
	protected:
		IVertexLayout* m_vertexLayout;
	};
}