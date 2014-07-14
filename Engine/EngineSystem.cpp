#include "StdAfx.h"
#include "EngineSystem.h"
#include "EngineConfig.h"
#include "Util/DllHelper.hpp"

#include "IRenderer.hpp"
DEFINE_RENDER_INTERFACE

#include "IAudio.hpp"
DEFINE_AUDIO_INTERFACE

#include "Resource/ResourceManager.h"
DEFINE_RESOURCEMGR_INTERFACE

#include "Texture/TextureManager.h"
DEFINE_TEXTUREMGR_INTERFACE

#include "Material/MaterialManager.h"
DEFINE_MATERIALMGR_INTERFACE

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
		// Load Renderer Model
		const char* rendererDll = "RenderD3D9.dll";
		MODULE_HANDLE instance = LoadDll(rendererDll);
		if (!instance)
			return false;

		CreateRendererFunc func = (CreateRendererFunc)GetProc( instance, CREATE_RENDERER_DLL_FUNC);
		if (!func)
			return false;

		func(&gRenderer);
		
		bool result = gRenderer->Init(mainWnd, this);
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

		// Create resource manager
		gIResourceManager = new ResourceManager();

		// Create texture manager
		gTextureManager = new TextureManager();
		gTextureManager->Init(this);

		// Create material manager
		gMaterialManager = new MaterialManager();
		gMaterialManager->Init(this);

		return true;
	}

	void EngineSystem::Destroy()
	{
		gAudio->Destroy();
		gRenderer->Destroy();
	}

	void EngineSystem::Update(float deltaTime)
{
		gRenderer->Update(deltaTime);

		gRenderer->Render();

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

	ITextureManager* EngineSystem::GetITextureManager()
	{
		return gTextureManager;
	}

	IRenderer* EngineSystem::GetIRenderer()
	{
		return gRenderer;
	}

	IMaterialManager* EngineSystem::GetMaterialManager()
	{
		return gMaterialManager;
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