#pragma once

#include "EngineConfig.h"
#include <string.h>
#include <ctype.h>

namespace Dream
{
	std::string GetExtension(const std::string& fileName)
	{
		int dotIdx = fileName.find_last_of('.');

		if(dotIdx == fileName.size() - 1)
			return "";

		std::string ext = fileName.substr(dotIdx + 1);
		for (size_t i=0; i<ext.size(); ++i)
			ext[i] = tolower(ext[i]);

		return ext;
	}

	size_t HashCode(const char* str)
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