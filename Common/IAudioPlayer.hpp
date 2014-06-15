#pragma once

#include <string>

#define AUDIO_BUFF_NUM 4

namespace Dream
{
	class IDecoder;
	class IResourceItem;

	class IAudioPlayer
	{
	public:
		struct AudioInfo
		{
			int format;
			int channels;
			int frequency;
			int buffersize;
			unsigned char* decoderBuffer;
		};

		enum PLAYERPARAM
		{
			eBuffer,
			eSource,
		};

		IAudioPlayer(){ memset( &m_info, 0, sizeof(AudioInfo)); }
		virtual ~IAudioPlayer(){}

		virtual void Update(float deltaTime) = 0;

		virtual void SetName(const char* name) = 0;
		virtual const char* GetName() const { return m_name.c_str(); }

		virtual bool Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;

	protected:
		virtual bool LoadAudioResource() = 0;
		virtual bool GetAudioFormat() = 0;
		virtual void* GetParam(PLAYERPARAM param) = 0;

		std::string		m_name;
		IDecoder*		m_decoder;
		IResourceItem*	m_audioRes;
		AudioInfo		m_info;

		friend class WavDecoder;
		friend class MP3Decoder;
		friend class OggDecoder;
	};
}