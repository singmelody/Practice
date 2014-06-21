#pragma once
#include <d3dx9.h>

namespace Dream
{
	class VertexLayout;

	class IVertexBuffer
	{
	public:
		IVertexBuffer(){}
		virtual ~IVertexBuffer(){}

		/// get the vertex layout
		const VertexLayout* GetVertexLayout() const { return m_vertexLayout; }

	protected:
		VertexLayout* vertexLayout;
	};
}