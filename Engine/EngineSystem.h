#pragma once
#include "IEngine.hpp"

namespace Dream
{

class EngineSystem : public IEngine
{
public:
	EngineSystem();
	virtual ~EngineSystem();

	virtual bool Init(const HWND mainWnd);
	virtual void Destroy();

	virtual void Update(float deltaTime);

	virtual IAudio* GetIAudio();

	virtual IResourceManager* GetIResourceManager();
	virtual ITextureManager* GetITextureManager();
	virtual IShaderManager* GetShaderManager();
	virtual IRenderer* GetIRenderer();


};

}

