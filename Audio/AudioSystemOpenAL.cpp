#include "StdAfx.h"
#include "AudioSystemOpenAL.h"
#include "EngineConfig.h"
#include "al/alc.h"
#include <stdio.h>

namespace Dream
{

AudioSystemOpenAL::AudioSystemOpenAL(void)
{
}

AudioSystemOpenAL::~AudioSystemOpenAL(void)
{
}

bool AudioSystemOpenAL::Init()
{
	ALCdevice *device;
	ALCcontext *ctx;

	// Open and initialize a device with default settings
	device = alcOpenDevice(NULL);
	if(!device)
	{
		printf("Could not open a device!\n");
		return false;
	}

	// create context according to the device
	ctx = alcCreateContext(device, NULL);
	if(ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
	{
		if(ctx != NULL)
			alcDestroyContext(ctx);

		alcCloseDevice(device);
		printf("Could not set a context!\n");
		return false;
	}

	printf("Opened \"%s\"\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	return true;
}

void AudioSystemOpenAL::Destroy()
{
	ALCdevice *device;
	ALCcontext *ctx;

	ctx = alcGetCurrentContext();
	if(ctx == NULL)
		return;

	device = alcGetContextsDevice(ctx);

	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
	alcCloseDevice(device);
}

//--------------------------------------------------------------------
/// Dll Exports
//--------------------------------------------------------------------
extern "C" DLL_EXPORT void CreateAudio(IAudio** oEngine)
{
	static AudioSystemOpenAL  _sEngine;
	*oEngine = &_sEngine;
}

}
