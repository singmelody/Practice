#include "StdAfx.h"
#include "WavDecoder.h"
#include "IO/IStream.hpp"
#include "IResourceItem.hpp"
#include "IAudioPlayer.hpp"
#include "AL/al.h"

namespace Dream
{

WavDecoder::WavDecoder(IAudioPlayer* player) 
	: IDecoder(player),m_handle(NULL)
{

}

WavDecoder::~WavDecoder()
{
	wave_close(m_handle);
	m_handle = NULL;

	SAFE_DELETE_ARRAY(m_player->m_info.decoderBuffer);
}

bool WavDecoder::Decode()
{
	if(!m_player)
		return false;


	unsigned int* buffers = (unsigned int*)m_player->GetParam(IAudioPlayer::eBuffer);
	unsigned int* sources = (unsigned int*)m_player->GetParam(IAudioPlayer::eSource);
	if(!buffers || !sources)
		return false;
	
	IStream* stream = m_player->m_audioRes->GetStream();
	unsigned char* dataPtr = stream->GetRaw() + m_handle->dataoffset;
	unsigned dataSize = m_handle->datasize;
	if(!stream || !dataPtr || !dataSize)
		return false;

	IAudioPlayer::AudioInfo& info = m_player->m_info;
	int	format = info.format;
	size_t channels = info.channels;
	size_t frequency = info.frequency;
	if(!channels || !frequency || !format)
		return false;

	int processed = 0;
	alGetSourcei( *sources, AL_BUFFERS_PROCESSED, &processed);

	while(processed > 0)
	{
		ALuint bufID;
		alSourceUnqueueBuffers( *sources, 1, &bufID);

		int writeSize = 0;
		WaveResult result = wave_read( m_handle, info.decoderBuffer, info.buffersize, writeSize);
		if (result != WR_OK)
			return false;

		alBufferData( buffers[0], info.format, info.decoderBuffer, writeSize, info.frequency);
		alSourceQueueBuffers( *sources, 1, &buffers[0]);
		processed--;
	}


// 	alBufferData( buffers[0], format, dataPtr, dataSize, frequency);
// 	if(alGetError() != AL_NO_ERROR)
// 		return false;

	return true;
}

bool WavDecoder::GetInfo()
{
	// get stream
	IResourceItem* item = m_player->m_audioRes;
	IStream* stream = item->GetStream();

	// get callbacks
	wave_callbacks callbacks;
	callbacks.read_func = ReadWav;
	callbacks.seek_func = SeekWav;
	callbacks.tell_func = TellWav;
	callbacks.close_func= CloseWav;

	unsigned int* buffers = (unsigned int*)m_player->GetParam(IAudioPlayer::eBuffer);
	unsigned int* sources = (unsigned int*)m_player->GetParam(IAudioPlayer::eSource);
	if(!buffers || !sources)
		return false;

	// fill handle
	if(!wave_open( m_handle, callbacks, stream))
		return false;

	// fill info
	IAudioPlayer::AudioInfo& info = m_player->m_info;
	info.frequency = m_handle->wfEXT.Format.nSamplesPerSec;
	info.channels = m_handle->wfEXT.Format.nChannels;
	
	// wav data is pcm so don't need to malloc decode buffer or for stream
	m_player->GetAudioFormat();

	info.decoderBuffer = new unsigned char[info.buffersize];

	for (int i = 0; i < AUDIO_BUFF_NUM; ++i)
	{
		int writeSize = 0;
		WaveResult result = wave_read( m_handle, info.decoderBuffer, info.buffersize, writeSize);
		if (result != WR_OK)
			break;

		alBufferData( buffers[i], info.format, info.decoderBuffer, writeSize, info.frequency);
		alSourceQueueBuffers( *sources, 1, &buffers[i]);
	}

	return true;
}

size_t WavDecoder::ReadWav(void *ptr, size_t size, void *datasource)
{
	IStream *pData = static_cast< IStream* >(datasource);
	size_t ret = pData->Read( ptr, size);
	return ret;
}

int WavDecoder::SeekWav(void *datasource, long offset, int whence)
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

int WavDecoder::CloseWav(void *datasource)
{
	return NULL;
}

long WavDecoder::TellWav(void *datasource)
{
	IStream *pData = static_cast< IStream* >(datasource);
	size_t position = pData->GetPosition();
	return position;
}

}
