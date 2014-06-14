#include <stdafx.h>
#include "ResourceManager.h"
#include "EngineConfig.h"
#include "FileResourceItem.h"
#include "IO/FileStream.h"

namespace Dream
{
	ResourceManager::ResourceManager()
	{

	}

	ResourceManager::~ResourceManager()
	{
		for (int i = m_items.size() - 1; i >= 0; --i)
		{
			IResourceItem* item = m_items[i];
			SAFE_DELETE(item);
		}

		m_items.clear();
	}

	IResourceItem* ResourceManager::LoadRes(const char* path)
	{
		size_t hash = HashCode(path);

		HashMap::iterator itr = m_items.find(hash);
		if(itr != m_items.end())
			return (*itr).second;

		FILE* file = fopen( path, "rb");

		if(!file)
			return NULL;

		fseek( file, 0, SEEK_END);
		long size = ftell(file);
		rewind(file);

		unsigned char* buffer = new unsigned char[size];
		if( !buffer )
			return NULL;
		
		long readSize = fread( buffer, 1, size, file);
		if( !readSize )
			return NULL;

		fclose(file);

		IStream* stream = new FileStream();
		stream->SetRaw(buffer);
		stream->SetSize(size);

		IResourceItem* item = new FileResourceItem();
		item->SetStream(stream);


		m_items.insert(std::make_pair( hash, item));

		return item;
	}

	void ResourceManager::UnLoadRes(const char* str)
	{
		size_t hash = HashCode(str);
		
		HashMap::iterator itr = m_items.find(hash);
		if(itr == m_items.end())
			return;

		SAFE_DELETE((*itr).second);
		m_items.erase(itr);

	}

	size_t ResourceManager::HashCode(const char* str)
	{
		size_t hash = 0;
		size_t len = strlen(str);
		size_t i;
		for (i = 0; i < len; i++)
		{
			hash += str[i];
			hash += hash << 10;
			hash ^= hash >>  6;
		}
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		hash &= ~(1<<31);       // don't return a negative number (in case IndexT is defined signed)
		return hash;
	}

}