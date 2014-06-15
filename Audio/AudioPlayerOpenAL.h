#pragma once
#include "IAudioPlayer.hpp"


namespace Dream{

class IResourceItem;

class AudioPlayerOpenAL : public IAudioPlayer
{
public:
	AudioPlayerOpenAL(void);
	~AudioPlayerOpenAL(void);

	virtual void Update(float deltaTime);

	virtual bool Play();

	virtual void Pause();

	virtual void Stop();

	virtual void SetName(const char* name);
protected:
	virtual bool LoadAudioResource();
	virtual bool GetAudioFormat();
	virtual void* GetParam(PLAYERPARAM param);
private:
	unsigned int m_source;
	unsigned int m_buffers[AUDIO_BUFF_NUM];
};

}