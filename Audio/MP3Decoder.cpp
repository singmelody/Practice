#include "StdAfx.h"
#include "MP3Decoder.h"
#include "IO/IStream.hpp"
#include "IResourceItem.hpp"
#include "IAudioPlayer.hpp"
#include "AL/al.h"

namespace Dream{


MP3Decoder::~MP3Decoder(void)
{

}

bool MP3Decoder::Decode()
{
	return true;
}

bool MP3Decoder::GetInfo()
{
	return true;
}

}