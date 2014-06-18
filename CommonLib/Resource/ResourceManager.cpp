#include <stdafx.h>
#include "ResourceManager.h"
#include "EngineConfig.h"
#include "FileResourceItem.h"
#include "IO/FileStream.h"
#include "Util/StringHelper.hpp"
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

}