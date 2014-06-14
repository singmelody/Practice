#pragma once

namespace Dream
{
	class IStream;

	class IResourceItem
	{
	public:
		IResourceItem(){}
		virtual ~IResourceItem(){}

		virtual void SetStream(IStream* stream) = 0;
		virtual IStream* GetStream() = 0;

		virtual void SetSize(int size){ m_size = size;}
		virtual int GetSize() const { return m_size; }
	protected:
		int m_size;
	};
}