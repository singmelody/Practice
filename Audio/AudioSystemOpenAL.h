#pragma once
#include "AudioSystem.h"
#include <vector>

namespace Dream
{

class AudioSystemOpenAL : public AudioSystem
{
public:
	AudioSystemOpenAL(void);
	virtual ~AudioSystemOpenAL(void);

	virtual bool Init(IEngine* engine);
	virtual void Destroy();

	virtual IAudioPlayer* CreateAudioPlayer();
};

}