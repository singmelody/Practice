#pragma once
#include <vector>

namespace Dream
{
	class IIndexBuffer
	{
	public:
		IIndexBuffer(){}
		virtual ~IIndexBuffer(){}

		virtual void Fill(const void* ptr,int size) = 0;
// 		virtual void addIndex(unsigned int idx) { m_index.push_back(idx); }
// 
// 	protected:
// 		std::vector<unsigned int> m_index;
	};
}