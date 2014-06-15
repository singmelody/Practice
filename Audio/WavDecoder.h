#pragma once
#include "IDecoder.hpp"

namespace Dream
{
	class WavDecoder : public IDecoder
	{
	public:
		WavDecoder(IAudioPlayer* player) : IDecoder(player) {}
		virtual ~WavDecoder();

		virtual bool Decode();
		virtual bool GetInfo();
	};
}
