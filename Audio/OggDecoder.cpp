#include "StdAfx.h"
#include "OggDecoder.h"
#include "IO/IStream.hpp"
#include "IResourceItem.hpp"
#include "IAudioPlayer.hpp"
#include "AL/al.h"

#define OGG_CLEAN(handle) { ov_clear(handle); SAFE_DELETE(handle); }

namespace Dream
{

	OggDecoder::OggDecoder(IAudioPlayer* player) 
		: IDecoder(player)
	{
		m_handle = new OggVorbis_File();
	}

	OggDecoder::~OggDecoder()
	{
		OGG_CLEAN(m_handle);
		SAFE_DELETE_ARRAY(m_player->m_info.decodeBuffer);
	}

	bool OggDecoder::Decode()
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

			unsigned long writeSize = DecodeOgg();
			if(!writeSize)
				return false;

			alBufferData( bufID, info.format, info.decodeBuffer, writeSize, info.frequency);
			alSourceQueueBuffers( *sources, 1, &bufID);
			processed--;
		}

		return true;
	}

	bool OggDecoder::GetInfo()
	{
		// get stream
		IResourceItem* item = m_player->m_audioRes;
		IStream* stream = item->GetStream();

		// get callbacks
		ov_callbacks callbacks;
		callbacks.read_func = ReadOgg;
		callbacks.seek_func = SeekOgg;
		callbacks.tell_func = TellOgg;
		callbacks.close_func= CloseOgg;

		unsigned int* buffers = (unsigned int*)m_player->GetParam(IAudioPlayer::eBuffer);
		unsigned int* sources = (unsigned int*)m_player->GetParam(IAudioPlayer::eSource);
		if(!buffers || !sources)
			return false;

		// fill handle
		if (ov_open_callbacks( stream, m_handle, NULL, NULL, callbacks))
		{
			OGG_CLEAN(m_handle);
			return false;
		}


		// fill info
		vorbis_info* oggInfo = NULL;
		oggInfo = ov_info( m_handle, -1);
		if(!oggInfo)
		{
			OGG_CLEAN(m_handle);
			return false;
		}

		IAudioPlayer::AudioInfo& info = m_player->m_info;
		info.frequency = oggInfo->rate;
		info.channels = oggInfo->channels;

		// wav data is pcm so don't need to malloc decode buffer or for stream
		m_player->GetAudioFormat();

		info.decodeBuffer = new unsigned char[info.buffersize];

		for (int i = 0; i < AUDIO_BUFF_NUM; ++i)
		{
			unsigned long writesize = DecodeOgg();
			if (!writesize)
				break;

			alBufferData( buffers[i], info.format, info.decodeBuffer, writesize, info.frequency);
			if(alGetError() != AL_NO_ERROR)
				OutputDebugString("alBufferData failed\n");
			alSourceQueueBuffers( *sources, 1, &buffers[i]);
		}

		return true;
	}

	size_t OggDecoder::ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		IStream *pData = static_cast< IStream* >(datasource);
		size_t ret = pData->Read( ptr, size);
		return ret;
	}

	int OggDecoder::SeekOgg(void *datasource, ogg_int64_t offset, int whence)
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

	int OggDecoder::CloseOgg(void *datasource)
	{
		return NULL;
	}

	long OggDecoder::TellOgg(void *datasource)
	{
		IStream *pData = static_cast< IStream* >(datasource);
		size_t position = pData->GetPosition();
		return position;
	}

	unsigned long OggDecoder::DecodeOgg()
	{
		IAudioPlayer::AudioInfo& info = m_player->m_info;

		char* decodeBuffer = (char*)info.decodeBuffer;

		// ogg decode section
		int section;
		long decodeSize;
		unsigned long writeSize = 0;
		short *pSamples;

		{
			while (1)
			{
				decodeSize = ov_read( m_handle, decodeBuffer + writeSize, 
					info.buffersize - writeSize, 0, 2, 1, &section);

				if (decodeSize <= 0)
					break;

				writeSize += decodeSize;

				if (writeSize >= info.buffersize)
					break;
			}

			// Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
			// however 6-Channels files need to be re-ordered
			if (info.channels == 6)
			{		
				for (unsigned long ulSamples = 0; 
					ulSamples < ( info.buffersize >>1); ulSamples+=6)
				{
					pSamples = (short*)decodeBuffer;
					// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
					// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
					std::swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
					std::swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
					std::swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
				}
			}

		}

		return writeSize;
	}

}
