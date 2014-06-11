#pragma once

namespace Dream
{
	class IEngine;
	class IAudioPlayer;

	class IAudio
	{
	public:
		IAudio(void){}
		virtual ~IAudio(void){}

		virtual bool Init(IEngine* ) = 0;
		virtual void Destroy() = 0;

		virtual void Update(float deltaTime) = 0;

		virtual IAudioPlayer* CreateAudioPlayer() = 0;
	};

	extern IAudio*	gAudio;
}

#define DEFINE_AUDIO_INTERFACE  Dream::IAudio*  Dream::gAudio = NULL;

#define CREATE_AUDIO_DLL_FUNC   "CreateAudio"
typedef void(*CreateAudioFunc)(Dream::IAudio**);