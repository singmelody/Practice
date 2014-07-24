#pragma once

#include "IThread.hpp"
#include <vector.h>
namespace Dream
{

class ThreadPool
{
public:
	ThreadPool(void);
	~ThreadPool(void);

private:
	std::vector<IThread> m_threads;
};

}


