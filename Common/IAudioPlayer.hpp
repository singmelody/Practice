#pragma once

#include <string>

namespace Dream
{
	class IAudioPlayer
	{
	public:
		IAudioPlayer(){}
		virtual ~IAudioPlayer(){}

		virtual void Update(float deltaTime) = 0;

		virtual void SetName(const char* name) = 0;

		virtual const char* GetName() const { return m_name.c_str(); }

		virtual bool Play() = 0;

		virtual void Pause() = 0;

		virtual void Stop() = 0;

	protected:
		virtual bool LoadAudioResource() = 0;
		virtual bool GetAudioInfo() = 0;

		std::string m_name;
	};
}