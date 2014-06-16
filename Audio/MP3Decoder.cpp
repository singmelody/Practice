#include "StdAfx.h"
#include "MP3Decoder.h"
#include "IO/IStream.hpp"
#include "IResourceItem.hpp"
#include "IAudioPlayer.hpp"
#include "AL/al.h"

#define MPG123_CLEAN(handle) { mpg123_close(handle); mpg123_delete(handle); mpg123_exit(); handle = NULL; }

namespace Dream{

MP3Decoder::MP3Decoder(IAudioPlayer* player) : IDecoder(player)
{
	// one more call , just return OK in mpg123_init();
	int ret = mpg123_init();
	if(ret != MPG123_OK)
		OutputDebugString("init mpg123 failed\n");

	m_handle = mpg123_new(mpg123_decoders()[0], &ret);
	if(ret != MPG123_OK)
		MPG123_CLEAN(m_handle);
}

MP3Decoder::~MP3Decoder(void)
{
	MPG123_CLEAN(m_handle);
}

bool MP3Decoder::Decode()
{
	if(!m_player)
		return false;

	IAudioPlayer::AudioInfo& info = m_player->m_info;

	unsigned int* buffers = (unsigned int*)m_player->GetParam(IAudioPlayer::eBuffer);
	unsigned int* sources = (unsigned int*)m_player->GetParam(IAudioPlayer::eSource);
	char* decodeBuffer = (char*)info.decodeBuffer;
	if(!buffers || !sources || !decodeBuffer)
		return false;

	int processed = 0;
	alGetSourcei( *sources, AL_BUFFERS_PROCESSED, &processed);

	while(processed > 0)
	{
		ALuint bufID;
		alSourceUnqueueBuffers( *sources, 1, &bufID);

		size_t writesize;
		int ret = mpg123_read( m_handle, info.decodeBuffer, info.buffersize, &writesize);
		if (!writesize || ret != MPG123_OK)
			break;

		alBufferData( bufID, info.format, info.decodeBuffer, writesize, info.frequency);
		alSourceQueueBuffers( *sources, 1, &bufID);
		processed--;
	}

	return true;

	return true;
}

bool MP3Decoder::GetInfo()
{
	// get buffer & source
	unsigned int* buffers = (unsigned int*)m_player->GetParam(IAudioPlayer::eBuffer);
	unsigned int* sources = (unsigned int*)m_player->GetParam(IAudioPlayer::eSource);
	if(!buffers || !sources)
		return false;

	// get stream
	IResourceItem* item = m_player->m_audioRes;
	IStream* stream = item->GetStream();

	// get callbacks
	int ret = mpg123_replace_reader_handle( m_handle, 
		ReadMp3, SeekMp3, CloseMp3);
	if( ret != MPG123_OK )
	{
		MPG123_CLEAN(m_handle);
		return false;
	}

	// open handle
	ret = mpg123_open_handle( m_handle, stream );
	if( ret != MPG123_OK )
	{
		MPG123_CLEAN(m_handle);
		return false;
	}

	// fill info
	IAudioPlayer::AudioInfo& info = m_player->m_info;
	int encoding;
	ret = mpg123_getformat( m_handle, (long*)&info.frequency, &info.channels,  &encoding);
	if( ret != MPG123_OK )
	{
		MPG123_CLEAN(m_handle);
		return false;
	}

	// wav data is pcm so don't need to malloc decode buffer or for stream
	m_player->GetAudioFormat();

	info.decodeBuffer = new unsigned char[info.buffersize];
	if(!info.decodeBuffer)
	{
		MPG123_CLEAN(m_handle);
		return false;
	}


	for (int i = 0; i < AUDIO_BUFF_NUM; ++i)
	{
		size_t writesize;
		ret = mpg123_read( m_handle, info.decodeBuffer, info.buffersize, &writesize);
		if (!writesize || ret != MPG123_OK)
			break;

		alBufferData( buffers[i], info.format, info.decodeBuffer, writesize, info.frequency);
		if(alGetError() != AL_NO_ERROR)
			OutputDebugString("alBufferData failed\n");
		alSourceQueueBuffers( *sources, 1, &buffers[i]);
	}

	return true;
}

ssize_t MP3Decoder::ReadMp3(void *handle, void *buf, size_t size)
{
	IStream *pData = static_cast< IStream* >(handle);
	size_t ret = pData->Read( buf, size);
	return ret;
}

off_t MP3Decoder::SeekMp3(void *handle, off_t offset, int whence)
{
	IStream *pData = static_cast< IStream* >(handle);

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

void MP3Decoder::CloseMp3(void *handle)
{
	return;
}



}