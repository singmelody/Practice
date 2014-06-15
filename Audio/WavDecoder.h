#pragma once
#include "IDecoder.hpp"
#include "CWaves.h"

namespace Dream
{

	class IAudioPlayer;

	class WavDecoder : public IDecoder
	{
	public:
		WavDecoder(IAudioPlayer* player);
		virtual ~WavDecoder();

		virtual bool Decode();
		virtual bool GetInfo();
	protected:
		WAVEFILEINFO*	m_handle;

		// For Wav
		static size_t ReadWav(void *ptr, size_t size, void *datasource);
		static int SeekWav(void *datasource, long offset, int whence);
		static int CloseWav(void *datasource);
		static long TellWav(void *datasource);
	};
}
