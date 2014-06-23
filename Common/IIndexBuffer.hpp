#pragma once
#include <vector>

namespace Dream
{
	class IIndexBuffer
	{
	public:
		IIndexBuffer(){}
		virtual ~IIndexBuffer(){}

		virtual bool Fill(const void* ptr,int size) = 0;
	};
}