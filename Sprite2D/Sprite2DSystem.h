#pragma once
#include "ISprite2D.hpp"
#include <vector>

namespace Dream
{

class Sprite2DSystem : public ISprite2D
{
public:
	Sprite2DSystem();
	virtual ~Sprite2DSystem();

	virtual bool Init(IEngine* );
	virtual void Destroy();

	virtual void Update(float deltaTime);
	virtual void Render();

	virtual Sprite* CreateSprite();
};

}


