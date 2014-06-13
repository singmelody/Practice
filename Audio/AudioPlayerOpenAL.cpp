#include "StdAfx.h"
#include "AudioPlayerOpenAL.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "IResourceItem.hpp"
#include "IO/IStream.hpp"
#include "IEngine.hpp"
#include "IResourceManager.hpp"

namespace Dream
{

AudioPlayerOpenAL::AudioPlayerOpenAL()
	:	m_source(NULL),
		m_stream(NULL)	
{
	alGenBuffers( AUDIO_BUFF_NUM, m_buffers);

	alGenSources( 1, &m_source);

	alSource3i( m_source, AL_POSITION, 0, 0, -1);
	alSourcei( m_source, AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcei( m_source, AL_ROLLOFF_FACTOR, 0);
}


AudioPlayerOpenAL::~AudioPlayerOpenAL(void)
{
	gEngine->GetIResourceManager()->UnLoadRes(m_name.c_str());

	alDeleteSources( 1, &m_source);
	alDeleteSources( AUDIO_BUFF_NUM, m_buffers);

	SAFE_DELETE_ARRAY(m_info.m_decoderBuff);
}

void AudioPlayerOpenAL::Update(float deltaTime)
{
	ALint state;
	alGetSourcei( m_source, AL_SOURCE_STATE, &state);

	if(state != AL_PLAYING)
		return;

	int processed = 0;
	alGetSourcei( m_source, AL_BUFFERS_PROCESSED, &processed);

	while(processed > 0)
	{
		ALuint bufid;
		alSourceUnqueueBuffers( m_source, 1, &bufid);
		processed--;
	}
}

bool AudioPlayerOpenAL::Play()
{
	alSourceRewind(m_source);
	alSourcei( m_source, AL_BUFFER, 0);


	return true;
}

void AudioPlayerOpenAL::Pause()
{

}

void AudioPlayerOpenAL::Stop()
{

}

bool AudioPlayerOpenAL::LoadAudioResource()
{
	IResourceItem* item = gEngine->GetIResourceManager()->LoadRes(m_name.c_str());
	if(!item)
		return false;

	IStream* stream = item->GetStream();
	return true;
}

void AudioPlayerOpenAL::GetAudioInfo()
{

}

}