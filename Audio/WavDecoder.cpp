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
}

bool WavDecoder::Decode()
{
	if(!m_player)
		return false;

	unsigned int* buffers = (unsigned int*)m_player->GetBuffer();
	if(!buffers)
		return false;
	
	IStream* stream = m_player->m_audioRes->GetStream();
	unsigned char* dataPtr = stream->GetRaw() + m_handle->dataoffset;
	unsigned dataSize = m_handle->datasize;
	if(!stream || !dataPtr || !dataSize)
		return false;

	int	format = m_player->m_info.format;
	size_t channels = m_player->m_info.channels;
	size_t frequency = m_player->m_info.frequency;
	if(!channels || !frequency || !format)
		return false;

	alBufferData( buffers[0], format, dataPtr, dataSize, frequency);
	if(alGetError() != AL_NO_ERROR)
		return false;

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

	// fill handle
	if(!wave_open( m_handle, callbacks, stream))
		return false;

	// fill info
	m_player->m_info.frequency = m_handle->wfEXT.Format.nSamplesPerSec;
	m_player->m_info.channels = m_handle->wfEXT.Format.nChannels;
	
	// wav data is pcm so don't need to malloc decode buffer
	m_player->GetAudioFormat();

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
