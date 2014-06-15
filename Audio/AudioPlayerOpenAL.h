#pragma once
#include "IAudioPlayer.hpp"

#define AUDIO_BUFF_NUM 1

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

	virtual bool GetAudioInfo();
private:
	unsigned int m_source;
	unsigned int m_buffers[AUDIO_BUFF_NUM];
	IResourceItem* m_audioRes;
	AudioInfo m_info;

	// For Ogg
	static size_t readWav(void *ptr, size_t size, void *datasource);
	static int SeekWav(void *datasource, long offset, int whence);
	static int CloseWav(void *datasource);
	static long TellWav(void *datasource);
};

}