#include "StdAfx.h"
#include "AudioSystem.h"

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
	int count = m_coms.size();
	for (int i = 0; i < count; ++count)
	{
		m_coms->Update();
	}
}

}
