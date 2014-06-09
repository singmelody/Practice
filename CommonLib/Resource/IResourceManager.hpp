#pragma once

namespace Dream
{
	class IResourceItem;

	class IResourceManager
	{
	public:
		IResourceManager() {}
		virtual ~IResourceManager(){}

		virtual IResourceItem* LoadRes(const char* path) = 0;
		virtual void UnLoadRes() = 0;
	};
}