#pragma once

#include <Windows.h>

class IRenderer;

struct EngineStartupInfo
{
	int         winWidth;
	int         winHeight;
	HANDLE		winHandle;
	bool        fullScreen;
};

class IEngine
{
	virtual ~IEngine() {}

	virtual IRenderer* GetRenderer() = 0;

};