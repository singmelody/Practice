#pragma once

namespace Dream
{
	class IStream;

	class IResourceItem
	{
	public:
		IResourceItem(){}
		virtual ~IResourceItem(){}

		void SetStream(IStream* stream) = 0;
		IStream* GetStream() = 0;

	};
}