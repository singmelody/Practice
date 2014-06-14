#include "StdAfx.h"
#include "EngineSystem.h"
#include "EngineConfig.h"
#include "DllHelper.hpp"

#include "IRenderer.hpp"
DEFINE_RENDER_INTERFACE

#include "IAudio.hpp"
DEFINE_AUDIO_INTERFACE

#include "Resource/ResourceManager.h"
DEFINE_RESOURCEMGR_INTERFACE

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
		// Create resource manager
		gIResourceManager = new ResourceManager();

		// Load Renderer Model
		const char* rendererDll = "RenderD3D9.dll";
		MODULE_HANDLE instance = LoadDll(rendererDll);
		if (!instance)
			return false;

		CreateRendererFunc func = (CreateRendererFunc)GetProc( instance, CREATE_RENDERER_DLL_FUNC);
		if (!func)
			return false;

		func(&gRenderer);

		bool result = gRenderer->Init(mainWnd);
		if(!result)
			return false;

		// Load Audio Model
		const char* audioDll = "Audio.dll";
		instance = LoadDll(audioDll);
		if(!instance)
			return false;

		CreateAudioFunc audioFunc = (CreateAudioFunc)GetProc( instance, CREATE_AUDIO_DLL_FUNC);
		if(!audioFunc)
			return false;

		audioFunc(&gAudio);

		result = gAudio->Init(this);
		if(!result)
			return false;

		return true;
	}

	void EngineSystem::Destroy()
	{
		gAudio->Destroy();
		gRenderer->Destroy();
	}

	void EngineSystem::Update(float deltaTime)
{
// 		gRenderer->Update(deltaTime);
// 
// 		gRenderer->Render();

		gAudio->Update(deltaTime);
	}

	IAudio* EngineSystem::GetIAudio()
	{
		return gAudio;
	}

	IResourceManager* EngineSystem::GetIResourceManager()
	{
		return gIResourceManager;
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