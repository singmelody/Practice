#pragma once
#include "ISerialize.hpp"

namespace Dream
{
	class XMLSerialize : public ISerialize
	{
	public:
		XMLSerialize(){}
		virtual ~XMLSerialize() {}
	};
}