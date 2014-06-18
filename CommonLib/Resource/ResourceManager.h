#pragma once

#include "IResourceManager.hpp"
#include <hash_map>
namespace Dream
{
	class ResourceManager : public IResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		virtual IResourceItem* LoadRes(const char* path);
		virtual void UnLoadRes(const char* str);

	private:
		typedef std::hash_map<int,IResourceItem*> HashMap;

		HashMap m_items;
	};
}