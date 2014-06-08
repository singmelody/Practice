#pragma once
#include "IAudio.hpp"

namespace Dream
{

class AudioSystem : public IAudio
{
public:
	AudioSystem(void);
	virtual ~AudioSystem(void);

	virtual bool Init();
	virtual void Destroy();

	virtual void Update(float deltaTime);
};

}



