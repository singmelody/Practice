#pragma once
#include "IRenderer.hpp"
#include "IEngine.hpp"

namespace Dream
{

class EngineSystem : public IEngine
{
public:
	EngineSystem();
	virtual ~EngineSystem();

	virtual bool Init(IEngine** engine);
	virtual void Destroy();

	virtual void Update(float deltaTime);
};

}

