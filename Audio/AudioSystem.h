#pragma once
#include "IAudio.hpp"
#include <vector>

namespace Dream
{

class IAudioPlayer;

class AudioSystem : public IAudio
{
public:
	AudioSystem(void);
	virtual ~AudioSystem(void);

	virtual bool Init(IEngine*) = 0;
	virtual void Destroy() = 0;

	virtual IAudioPlayer* CreateAudioPlayer() = 0;


	virtual void Update(float deltaTime);

protected:
	std::vector<IAudioPlayer*> m_players;
};

}



