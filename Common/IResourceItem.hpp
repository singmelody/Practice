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

	};
}