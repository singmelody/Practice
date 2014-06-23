#pragma once

namespace Dream
{
	class IVertexLayout
	{
	public:
		IVertexLayout(){}
		virtual ~IVertexLayout(){}

		virtual int GetVertexSize() const { return m_vertexByteSize; }

	protected:
		int	m_vertexByteSize;
	};
}