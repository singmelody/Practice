#pragma once

namespace Dream
{
	class IAudioPlayer
	{
	public:
		IAudioPlayer(){}
		virtual ~IAudioPlayer(){}

		virtual void Update(float deltaTime) = 0;
	};
}