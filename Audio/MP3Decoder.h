#pragma once
#include "IDecoder.hpp"
#include "mpg123.h"
namespace Dream{

class MP3Decoder : public IDecoder
{
public:
	MP3Decoder(IAudioPlayer* player);
	virtual ~MP3Decoder(void);

	virtual bool Decode();
	virtual bool GetInfo();

protected:
	mpg123_handle*	m_handle;

	static ssize_t ReadMp3(void *handle, void *buf, size_t sz);
	static off_t SeekMp3(void *handle, off_t offset, int whence);
	static void CloseMp3(void *handle);
};

}