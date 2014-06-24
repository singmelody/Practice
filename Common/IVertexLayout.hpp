#pragma once

namespace Dream
{
	typedef unsigned short	WORD;
	typedef unsigned long	DWORD;
	typedef unsigned char	BYTE;

	class IVertexLayout
	{
	public:
		enum VERTEXUSAGE
		{
			POSITION,
			COLOR,
			WEIGHT,
			SIZE,

			USAGE_COUNT
		};

		enum DATATYPE
		{
			FLOAT3,
			FLOAT2,
			DWORD
		};

		IVertexLayout(){}
		virtual ~IVertexLayout(){}

		virtual void AddSignature(WORD streamIdx, WORD Offset, BYTE type, BYTE method, BYTE usage, BYTE usageIdx) = 0;

		virtual int GetVertexSize() const { return m_vertexByteSize; }

	protected:
		int	m_vertexByteSize;
	};
}