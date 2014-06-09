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
};

}

