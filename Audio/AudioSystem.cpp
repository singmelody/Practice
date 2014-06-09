#include "StdAfx.h"
#include "AudioSystem.h"
#include "IAudioPlayer.hpp"

namespace Dream
{

AudioSystem::AudioSystem(void)
{
}


AudioSystem::~AudioSystem(void)
{
}

void AudioSystem::Update(float deltaTime)
{
	int count = m_players.size();
	for (int i = 0; i < count; ++i)
	{
		m_players[i]->Update(deltaTime);
	}
}

}
