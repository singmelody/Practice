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
}