#pragma once
#include "IDecoder.hpp"
#include "mpg123.h"
namespace Dream{

class MP3Decoder : public IDecoder
{
public:
	MP3Decoder(IAudioPlayer* player) : IDecoder(player) {}
	virtual ~MP3Decoder(void);

	virtual bool Decode();
	virtual bool GetInfo();

protected:
	unsigned char* m_decoderBuffer;
};

}