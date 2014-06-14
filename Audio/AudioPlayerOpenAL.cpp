#include "StdAfx.h"

#define HAS_GUID

#include "AudioPlayerOpenAL.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "IResourceItem.hpp"
#include "IO/IStream.hpp"
#include "IEngine.hpp"
#include "IResourceManager.hpp"
#include "CWaves.h"

namespace Dream
{

AudioPlayerOpenAL::AudioPlayerOpenAL()
	:	m_source(NULL),
		m_audioRes(NULL)	
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
		printf("Audio Not Play now");

// 	int processed = 0;
// 	alGetSourcei( m_source, AL_BUFFERS_PROCESSED, &processed);
// 
// 	while(processed > 0)
// 	{
// 		ALuint bufid;
// 		alSourceUnqueueBuffers( m_source, 1, &bufid);
// 		processed--;
// 	}
}

bool AudioPlayerOpenAL::Play()
{
	//alSourceRewind(m_source);
	alSourcei( m_source, AL_BUFFER, m_buffers[0]);
	alSourcePlay( m_source );

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

	m_audioRes = item;

	return true;
}

bool AudioPlayerOpenAL::GetAudioInfo()
{
	int bufferSize = 0;

	if (m_info.channels == 1)
	{
		m_info.format = AL_FORMAT_MONO16;
		// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
		bufferSize = m_info.frequency >> 1;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= (bufferSize % 2);
	}
	else if (m_info.channels == 2)
	{
		m_info.format = AL_FORMAT_STEREO16;
		// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
		bufferSize = m_info.frequency;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= (bufferSize % 4);
	}
	else if (m_info.channels == 4)
	{
		m_info.format = alGetEnumValue("AL_FORMAT_QUAD16");
		// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
		bufferSize = m_info.frequency * 2;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= (bufferSize % 8);
	}
	else if (m_info.channels == 6)
	{
		m_info.format = alGetEnumValue("AL_FORMAT_51CHN16");
		// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
		bufferSize = m_info.frequency * 3;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		bufferSize -= (bufferSize % 12);
	}

	if(!bufferSize)
		return false;

	m_info.m_decoderBuff = new unsigned char[bufferSize];

	return true;
}

void AudioPlayerOpenAL::SetName(const char* name)
{
	m_name = name; 
	bool b = LoadAudioResource(); 
	if(!b)
		return; 

	IStream* stream = m_audioRes->GetStream();

	wave_callbacks call_backs;
	call_backs.read_func = readWav;
	call_backs.seek_func = SeekWav;
	call_backs.tell_func = TellWav;
	call_backs.close_func = CloseWav;

	b = CWaves::LoadWav( stream->GetRaw(), stream->GetSize(), m_info.frequency, m_info.channels, stream, call_backs);

	if(!b)
		return;

	GetAudioInfo(); 
	alBufferData( m_buffers[0], m_info.format, stream->GetRaw(), 282626, m_info.frequency);

}

size_t AudioPlayerOpenAL::readWav(void *ptr, size_t size, void *datasource)
{
	IStream *pData = static_cast< IStream* >(datasource);
	size_t ret = pData->Read( ptr, size);
	return ret;
}

int AudioPlayerOpenAL::SeekWav(void *datasource, long offset, int whence)
{
	IStream *pData = static_cast< IStream* >(datasource);

	switch (whence)
	{
	case SEEK_SET:
		pData->Seek((int)offset, IStream::eBegin);
		break;
	case SEEK_CUR:
		pData->Seek((int)offset, IStream::eCurrent);
		break;
	case SEEK_END:
		pData->Seek((int)offset, IStream::eEnd);
		break;
	}

	return NULL;
}

int AudioPlayerOpenAL::CloseWav(void *datasource)
{
	return NULL;
}

long AudioPlayerOpenAL::TellWav(void *datasource)
{
	IStream *pData = static_cast< IStream* >(datasource);
	size_t position = pData->GetPosition();
	return position;
}

}