#include "StdAfx.h"
#include "EngineSystem.h"
#include "EngineConfig.h"

DEFINE_RENDER_INTERFACE

namespace Dream
{
	EngineSystem::EngineSystem()
	{
		
	}

	EngineSystem::~EngineSystem( )
	{

	}

	bool EngineSystem::Init(const HWND mainWnd)
{
		const char* rendererDll = "RenderD3D9.dll";
		HINSTANCE instance = LoadLibrary(rendererDll);
		if (!instance)
			return false;

		CreateRendererFunc func = (CreateRendererFunc)GetProcAddress( instance, CREATE_RENDERER_DLL_FUNC);
		if (!func)
			return false;

		func(&gRenderer);

		bool result = gRenderer->Init(mainWnd);
		if(!result)
			return false;

		return true;
	}

	void EngineSystem::Destroy()
	{
		gRenderer->Destroy();
	}

	void EngineSystem::Update(float deltaTime)
{
		gRenderer->Update(deltaTime);

		gRenderer->Render();
	}

	//--------------------------------------------------------------------
	/// Dll Exports
	//--------------------------------------------------------------------
	extern "C" DLL_EXPORT void CreateEngine(IEngine** oEngine)
	{
		static EngineSystem  _sEngine;
		*oEngine = &_sEngine;
	}

}