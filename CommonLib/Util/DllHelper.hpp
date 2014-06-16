#pragma once

#include "EngineConfig.h"

namespace Dream
{
	MODULE_HANDLE LoadDll(const char* dllPath)
	{
		return LoadLibrary(dllPath);
	}

	void UnLoadDll(MODULE_HANDLE handle)
	{
		FreeLibrary(handle);
	}

	void * GetProc(MODULE_HANDLE handle, const char* funcName)
	{
		return (void*)GetProcAddress( handle, funcName);
	}
}