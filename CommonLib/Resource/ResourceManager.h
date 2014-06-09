#pragma once

#include "IResourceManager.hpp"

namespace Dream
{
	class ResourceManager : public IResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		virtual IResourceItem* LoadRes(const char* path);
		virtual void UnLoadRes();
	};
}