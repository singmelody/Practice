#pragma once

namespace Dream
{
	typedef unsigned short	WORD;
	typedef unsigned long	DWORD;
	typedef unsigned char	BYTE;

	class IVertexLayout
	{
	public:
		enum DATATYPE
		{
			FLOAT3,
			FLOAT2,
			DWORD
		};

		enum VERTEXUSAGE
		{
			POSITION,
			TEXCOORD,
			COLOR,
			WEIGHT,
			SIZE,

			USAGE_COUNT
		};

		enum METHOD
		{
			DEFAULT
		};

		IVertexLayout(){}
		virtual ~IVertexLayout(){}

		virtual void AddSignature(WORD streamIdx, WORD Offset, DATATYPE type, METHOD method, VERTEXUSAGE usage, BYTE usageIdx) = 0;

		virtual void Build() = 0;

		virtual int GetVertexSize() const { return m_vertexByteSize; }

	protected:
		int	m_vertexByteSize;
	};
}