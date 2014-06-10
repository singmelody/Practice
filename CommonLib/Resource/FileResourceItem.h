#pragma once
#include "IResourceItem.hpp"

namespace Dream
{
	class FileResourceItem : public IResourceItem
	{
	public:
		FileResourceItem();
		virtual ~FileResourceItem();

		void SetStream(IStream* stream);
		IStream* GetStream();
	
	protected:
		
	};
}

