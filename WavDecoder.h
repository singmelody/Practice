#pragma once
#include <string>

class WavDecoder
{
public:
	WavDecoder(void);
	~WavDecoder(void);

	void Load(const std::wstring& filename);
	short GetMaxAmplitude();
	short GetAverageAmplitude(float startTime, float endTime);
	float GetLength();

public:
	long m_numSamples;
	long m_sampleRate;
	short m_bitsPerSample;
	short m_numChannels;
	short *m_raw;
};

