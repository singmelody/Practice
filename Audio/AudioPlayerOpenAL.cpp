#include "StdAfx.h"

#include "AudioPlayerOpenAL.h"
#include "AL/al.h"
#include "AL/alc.h"
#include "IResourceItem.hpp"
#include "IO/IStream.hpp"
#include "IEngine.hpp"
#include "IResourceManager.hpp"
#include "CWaves.h"
#include "WavDecoder.h"
#include "OggDecoder.h"
#include "MP3Decoder.h"
#include "Util/StringHelper.hpp"

#define AL_ERROR_CHECK(state) if(alGetError() == AL_NO_ERROR) { OutputDebugString("AL state "#state" Success\n");  }else { OutputDebugString("AL state "#state" failed\n"); }

namespace Dream
{

AudioPlayerOpenAL::AudioPlayerOpenAL()
	:	m_source(NULL)
{
	alGenBuffers( AUDIO_BUFF_NUM, m_buffers);
	AL_ERROR_CHECK(alGenBuffers);

	alGenSources( 1, &m_source);
	AL_ERROR_CHECK(alGenSources);

	alSource3i( m_source, AL_POSITION, 0, 0, -1);
	AL_ERROR_CHECK(AL_POSITION);

	alSourcei( m_source, AL_SOURCE_RELATIVE, AL_TRUE);
	AL_ERROR_CHECK(AL_SOURCE_RELATIVE);
}


AudioPlayerOpenAL::~AudioPlayerOpenAL(void)
{
	gEngine->GetIResourceManager()->UnLoadRes(m_name.c_str());

	alSourceStop(m_source);
	alDeleteSources( 1, &m_source);
	alDeleteSources( AUDIO_BUFF_NUM, m_buffers);
}

void AudioPlayerOpenAL::Update(float deltaTime)
{
	ALint state;
	alGetSourcei( m_source, AL_SOURCE_STATE, &state);

	if(state != AL_PLAYING)
	{
		OutputDebugString("Audio Not Play now\n");
		return;
	}
	
	m_decoder->Decode();
}

bool AudioPlayerOpenAL::Play()
{
	alSourceRewind(m_source);
	AL_ERROR_CHECK(alSourceRewind);

	alSourcePlay( m_source );
	AL_ERROR_CHECK(alSourcePlay);

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

bool AudioPlayerOpenAL::GetAudioFormat()
{
	if (m_info.channels == 1)
	{
		m_info.format = AL_FORMAT_MONO16;
		// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
		m_info.buffersize = m_info.frequency >> 1;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		m_info.buffersize -= (m_info.buffersize % 2);
	}
	else if (m_info.channels == 2)
	{
		m_info.format = AL_FORMAT_STEREO16;
		// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
		m_info.buffersize = m_info.frequency;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		m_info.buffersize -= (m_info.buffersize % 4);
	}
	else if (m_info.channels == 4)
	{
		m_info.format = alGetEnumValue("AL_FORMAT_QUAD16");
		// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
		m_info.buffersize = m_info.frequency * 2;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		m_info.buffersize -= (m_info.buffersize % 8);
	}
	else if (m_info.channels == 6)
	{
		m_info.format = alGetEnumValue("AL_FORMAT_51CHN16");
		// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
		m_info.buffersize = m_info.frequency * 3;
		// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
		m_info.buffersize -= (m_info.buffersize % 12);
	}

	if(!m_info.buffersize)
		return false;

	return true;
}

void AudioPlayerOpenAL::SetName(const char* name)
{
	m_name = name; 
	bool b = LoadAudioResource(); 
	if(!b)
		return; 
	
	std::string ext = GetExtension(name);

	if(ext == "wav")
		m_decoder = new WavDecoder(this);
	else if( ext == "mp3")
		m_decoder = new MP3Decoder(this);
	else if( ext == "ogg" )
		m_decoder = new OggDecoder(this);
	else
	{
		OutputDebugString("invalid file extension\n");
		return;
	}

	b = m_decoder->GetInfo();
	if(!b)
		return;

	m_decoder->Decode();
}

void* AudioPlayerOpenAL::GetParam(PLAYERPARAM param)
{
	switch(param)
	{
	case eBuffer:
		return m_buffers;
	case eSource:
		return &m_source;
	}

	return NULL;
}

}