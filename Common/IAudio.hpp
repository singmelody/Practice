#pragma once

namespace Dream
{
	class IAudio
	{
	public:
		IAudio(void){}
		virtual ~IAudio(void){}

		virtual bool Init() = 0;
		virtual void Destroy() = 0;

		virtual void Update(float deltaTime) = 0;
	};

	extern IAudio*	gAuido;
}

#define DEFINE_AUDIO_INTERFACE  Dream::IAudio*  Dream::gIAudio = NULL;

#define CREATE_AUDIO_DLL_FUNC   "CreateAudio"
typedef void(*CreateAudioFunc)(Dream::IAudio**);