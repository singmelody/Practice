#pragma once
#include "IDecoder.hpp"
#include "vorbisfile.h"

namespace Dream
{

class OggDecoder : public IDecoder
{
public:
public:
	OggDecoder(IAudioPlayer* player);
	virtual ~OggDecoder();

	virtual bool Decode();
	virtual bool GetInfo();

protected:
	OggVorbis_File*	m_handle;

	unsigned long DecodeOgg();

	static size_t ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource);
	static int SeekOgg(void *datasource, ogg_int64_t offset, int whence);
	static int CloseOgg(void *datasource);
	static long TellOgg(void *datasource);

};

}