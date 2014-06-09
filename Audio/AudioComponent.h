#pragma once

namespace Dream
{

class IAudioBuffer;

class AudioComponent
{
public:
	AudioComponent(void);
	~AudioComponent(void);

	void Update(float deltaTime);

private:
	IAudioBuffer* m_buffer;
};

}
