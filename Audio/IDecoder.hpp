#pragma once

namespace Dream
{
	class IAudioPlayer;

	class IDecoder
	{
	public:
		IDecoder(IAudioPlayer* player){ m_player = player; }
		virtual ~IDecoder() { m_player = NULL; }

		virtual bool Decode() = 0;
		virtual bool GetInfo() = 0;

	protected:
		IAudioPlayer* m_player;
	};
}