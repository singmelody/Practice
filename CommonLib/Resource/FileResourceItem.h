#pragma once
#include "IResourceItem.hpp"

namespace Dream
{
	class FileStream;

	class FileResourceItem : public IResourceItem
	{
	public:
		FileResourceItem();
		virtual ~FileResourceItem();

		void SetStream(IStream* stream) { m_fileStream = stream; }
		IStream* GetStream() { return m_fileStream; }
	
	protected:
		FileStream* m_fileStream;
	};
}

