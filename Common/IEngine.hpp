#pragma once

#include <Windows.h>

namespace Dream {

class IRenderer;
class IAudio;
class IResourceManager;
class ITextureManager;

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

	virtual bool Init(const HWND mainWnd) = 0;

	virtual void Destroy() = 0;

	virtual void Update(float deltaTime) = 0;

	virtual IAudio* GetIAudio() = 0;

	virtual IResourceManager* GetIResourceManager() = 0;
	virtual ITextureManager* GetITextureManager() = 0;
	virtual IRenderer* GetIRenderer() = 0;
};

extern IEngine*		gEngine;

}

#define DEFINE_ENGINE_INTERFACE  Dream::IEngine*  Dream::gEngine = NULL;

#define CREATE_ENGINE_DLL_FUNC     "CreateEngine"
typedef void(*CreateEngineFunc)(Dream::IEngine**);