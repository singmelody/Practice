#include "DXUT.h"
#include "WavDecoder.h"
#include <fstream>

WavDecoder::WavDecoder(void)
{
	m_numSamples = 0;
	m_sampleRate = 0;
	m_bitsPerSample = 0;
	m_numChannels = 0;
	m_raw = NULL;
}


WavDecoder::~WavDecoder(void)
{
	SAFE_DELETE_ARRAY(m_raw);
}


void WavDecoder::Load(const std::wstring& filename)
{
	std::ifstream in( filename.c_str(), std::ios::binary);
	
	// RIFF
	char ID[4];
	in.read( ID, 4);

	if(ID[0] != 'R' || ID[1] != 'I' || ID[2] != 'F' || ID[3] != 'F')		//4 first bytes should say 'RIFF'
	{
		printf("this is not valied meida file\n");
		in.close();
		return;
	}
	// RIFF chunk size
	long fileSize = 0;
	in.read( (char*)&fileSize, sizeof(long));

	//The actual file size is 8 bytes larger
	fileSize += 8;

	//Wave ID
	in.read(ID, 4);
	if(ID[0] != 'W' || ID[1] != 'A' || ID[2] != 'V' || ID[3] != 'E')		//ID should be 'WAVE'
	{
		printf( "File is not a valid Wave file\n"); 		
		in.close();
		return;
	}

	//Format Chunk ID
	in.read(ID, 4);
	if(ID[0] != 'f' || ID[1] != 'm' || ID[2] != 't' || ID[3] != ' ')		//ID should be 'fmt '
	{
		printf( "File is not a valid FMT file\n"); 		
		in.close();
		return;
	}

	//Format Chunk Size
	long formatSize = 0;
	in.read((char*)&formatSize, sizeof(long));

	//Audio Format
	short audioFormat = 0;
	in.read((char*)&audioFormat, sizeof(short));
	if(audioFormat != 1)
	{
		printf( "Only support uncompressed data\n"); 		
		in.close();
		return;
	}

	//Num Channels
	in.read((char*)&m_numChannels, sizeof(short));

	//Sample Rate 
	in.read((char*)&m_sampleRate, sizeof(long));

	//Byte Rate
	long byteRate = 0;
	in.read((char*)&byteRate, sizeof(long));

	//Block Align
	short blockAlign = 0;
	in.read((char*)&blockAlign, sizeof(short));

	//Bits Per Sample	
	in.read((char*)&m_bitsPerSample, sizeof(short));
	if(m_bitsPerSample != 16)
	{
		printf("this is not a 16-Bit wave file\n");
		in.close();
		return;
	}

	//Data Chunk ID
	in.read(ID, 4);
	if(ID[0] != 'd' || ID[1] != 'a' || ID[2] != 't' || ID[3] != 'a')		//ID should be 'data'
	{
		printf("no data chunk found\n");
		in.close();
		return;
	}

	//Size
	long dataSize;
	in.read((char*)&dataSize, sizeof(long));
	m_numSamples = dataSize / 2;	// divide by 2 ( short has 2 bytes)

	m_raw = new short[m_numSamples];
	in.read((char*)m_raw, dataSize);

	in.close();
}

short WavDecoder::GetMaxAmplitude()
{
	if(m_raw == NULL)
		return 0;

	//Find sample with highest amplitude
	short maxAmp = 0;

	for(int i=0; i<m_numSamples; i++)
	{
		if(abs(m_raw[i]) > maxAmp)
			maxAmp = abs(m_raw[i]);
	}

	return maxAmp;
}

short WavDecoder::GetAverageAmplitude(float startTime, float endTime)
{
	if(m_raw == NULL)
		return 0;

	// calculate start & end sample
	int startSample = (int)( m_sampleRate * startTime) * m_numChannels;
	int endSampe = (int)( m_sampleRate * endTime) * m_numChannels;

	if( startSample >= endSampe )
		return 0;

	// calcuate the avg amp between start and sample
	float c = 1.0f / (float)(endSampe - startSample);
	float avg = 0.0f;
	for (int i = startSample; i < endSampe && i < m_numSamples; ++i)
	{
		avg += abs(m_raw[i]) * c;
	}

	avg = min( avg, (float)(SHRT_MAX - 1));
	avg = max( avg, (float)(SHRT_MIN + 1));

	return (short)avg;
}

float WavDecoder::GetLength()
{
	return ( m_numSamples / m_numChannels) / (float)m_sampleRate;
}
