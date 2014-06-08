#pragma once
#include "IAudio.hpp"

namespace Dream
{

class AudioComponent;

class AudioSystem : public IAudio
{
public:
	AudioSystem(void);
	virtual ~AudioSystem(void);

	virtual bool Init() = 0;
	virtual void Destroy() = 0;

	virtual void Update(float deltaTime);

protected:
	std::vector<AudioComponent*> m_coms;
};

}



