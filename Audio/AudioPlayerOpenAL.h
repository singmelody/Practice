#pragma once
#include "IAudioPlayer.hpp"

#define AUDIO_BUFF_NUM 4

namespace Dream{

class IStream;

class AudioPlayerOpenAL : public IAudioPlayer
{
public:
	AudioPlayerOpenAL(void);
	~AudioPlayerOpenAL(void);

	virtual void Update(float deltaTime);

	virtual bool Play();

	virtual void Pause();

	virtual void Stop();

protected:
	struct AudioInfo
	{
		int format;
		int channels;
		unsigned int rate;
		unsigned char* m_decoderBuff;
	};

	virtual bool LoadAudioResource();

	void GetAudioInfo();
private:
	unsigned int m_source;
	unsigned int m_buffers[AUDIO_BUFF_NUM];
	IStream* m_stream;
	AudioInfo m_info;
};

}