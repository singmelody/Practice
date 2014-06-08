#pragma once

namespace Dream
{

class IAudioBuffer;

class AudioComponent
{
public:
	AudioComponent(void);
	~AudioComponent(void);

	void Update();

private:
	IAudioBuffer* m_buffer;
};

}
