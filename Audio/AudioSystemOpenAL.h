#pragma once
#include "AudioSystem.h"

namespace Dream
{

class AudioSystemOpenAL : public AudioSystem
{
public:
	AudioSystemOpenAL(void);
	virtual ~AudioSystemOpenAL(void);
};

}