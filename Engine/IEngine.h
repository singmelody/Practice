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

	virtual IRenderer* GetRenderer() = 0;

	virtual void Update() = 0;
};

}

#define DEFINE_ENGINE_INTERFACE  Dream::IEngine*  Dream::gEngine = nullptr;

#define CREATE_RENDERDRV_DLL_FUNC     "Init"
typedef void(*Init)(Dream::IEngine**);