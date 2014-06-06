#pragma once

#include <Windows.h>

namespace Dream {
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
public:
	virtual ~IEngine() {}

	virtual bool Init(IEngine** engine) = 0;

	virtual void Destroy();

	virtual void Update(float deltaTime) = 0;
};

extern IEngine*		gEngine;

}

#define DEFINE_ENGINE_INTERFACE  Dream::IEngine*  Dream::gEngine = NULL;

#define CREATE_ENGINE_DLL_FUNC     "CreateEngine"
typedef void(*CreateEngineFunc)(Dream::IEngine**);