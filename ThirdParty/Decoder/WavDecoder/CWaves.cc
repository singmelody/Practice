/*
 * Copyright (c) 2006, Creative Labs Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and
 * 	     the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions
 * 	     and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *     * Neither the name of Creative Labs Inc. nor the names of its contributors may be used to endorse or
 * 	     promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "stdafx.h"

#include "CWaves.h"

#include <stdio.h>
#include <memory>
#include <string>

typedef struct 
{
	char			szRIFF[4];
	ulong			ulRIFFSize;
	char			szWAVE[4];
} WAVEFILEHEADER;

typedef struct
{
	char	szChunkName[4];
	ulong	ulChunkSize;
} RIFFCHUNK;

typedef struct
{
	ushort	usFormatTag;
	ushort	usChannels;
	ulong	ulSamplesPerSec;
	ulong	ulAvgBytesPerSec;
	ushort	usBlockAlign;
	ushort	usBitsPerSample;
	ushort	usSize;
	ushort  usReserved;
	ulong	ulChannelMask;                                  
	GUID    guidSubFormat;
} WAVEFMT;

typedef struct waveformat_tag {
	ushort		wFormatTag;        /* format type */
	ushort		nChannels;         /* number of channels (i.e. mono, stereo, etc.) */
	ulong		nSamplesPerSec;    /* sample rate */
	ulong		nAvgBytesPerSec;   /* for buffer estimation */
	ushort		nBlockAlign;       /* block size of data */
} WAVEFORMAT;

typedef struct pcmwaveformat_tag {
	WAVEFORMAT	  wf;
	ushort        wBitsPerSample;
} PCMWAVEFORMAT;

#define WAVE_FORMAT_PCM 1
#define  WAVE_FORMAT_EXTENSIBLE  0xFFFE

#define SPEAKER_FRONT_LEFT              0x1
#define SPEAKER_FRONT_RIGHT             0x2
#define SPEAKER_FRONT_CENTER            0x4
#define SPEAKER_LOW_FREQUENCY           0x8
#define SPEAKER_BACK_LEFT               0x10
#define SPEAKER_BACK_RIGHT              0x20
#define SPEAKER_FRONT_LEFT_OF_CENTER    0x40
#define SPEAKER_FRONT_RIGHT_OF_CENTER   0x80
#define SPEAKER_BACK_CENTER             0x100
#define SPEAKER_SIDE_LEFT               0x200
#define SPEAKER_SIDE_RIGHT              0x400
#define SPEAKER_TOP_CENTER              0x800
#define SPEAKER_TOP_FRONT_LEFT          0x1000
#define SPEAKER_TOP_FRONT_CENTER        0x2000
#define SPEAKER_TOP_FRONT_RIGHT         0x4000
#define SPEAKER_TOP_BACK_LEFT           0x8000
#define SPEAKER_TOP_BACK_CENTER         0x10000
#define SPEAKER_TOP_BACK_RIGHT          0x20000

#define SUCCEED(hr) hr >= 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWaves::CWaves()
{
	m_stream = NULL;
	memset(&m_callbacks, 0, sizeof(wave_callbacks));
	memset(&m_WaveIDs, 0, sizeof(m_WaveIDs));
}

CWaves::~CWaves()
{
	long lLoop;

	for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
	{
		if (m_WaveIDs[lLoop])
		{
			if (m_WaveIDs[lLoop]->pData)
				delete m_WaveIDs[lLoop]->pData;

			delete m_WaveIDs[lLoop];
			m_WaveIDs[lLoop] = 0;
		}
	}

	if(m_instance)
	{
		delete m_instance;
		m_instance = NULL;
	}
}

WaveResult CWaves::LoadWaveFile(uchar* nameOrData, size_t dataSize, WAVEID *id, int& channels)
{
	WaveResult wr = WR_OUTOFMEMORY;
	LPWAVEFILEINFO pWaveInfo;

	pWaveInfo = new WAVEFILEINFO;
	memset( pWaveInfo, 0, sizeof(WAVEFILEINFO));
	if (pWaveInfo)
	{
		pWaveInfo->pData = nameOrData;
		if (SUCCEED((wr = ParseData(nameOrData, pWaveInfo, channels))) )
		{
			// Allocate memory for sample data
			pWaveInfo->pData = new uchar[pWaveInfo->ulDataSize];
			if (pWaveInfo->pData)
			{
				// Seek to start of audio data
				//pData->Seek(pWaveInfo->ulDataOffset, IO::Stream::Begin);
				m_callbacks.seek_func( (void*)nameOrData, pWaveInfo->ulDataOffset, SEEK_SET);

				// Read Sample Data
				if(m_callbacks.read_func( pWaveInfo->pData, pWaveInfo->ulDataSize, (void*)nameOrData) == pWaveInfo->ulDataSize)
				{
					long lLoop = 0;
					for (lLoop = 0; lLoop < MAX_NUM_WAVEID; lLoop++)
					{
						if (!m_WaveIDs[lLoop])
						{
							m_WaveIDs[lLoop] = pWaveInfo;
							*id = lLoop;
							wr = WR_OK;
							break;
						}
					}

					if (lLoop == MAX_NUM_WAVEID)
					{
						delete pWaveInfo->pData;
						wr = WR_OUTOFMEMORY;
					}
				}
				else
				{
					delete pWaveInfo->pData;
					wr = WR_BADWAVEFILE;
				}
			}
			else
			{
				wr = WR_OUTOFMEMORY;
			}

			m_callbacks.close_func((void*)nameOrData);
		}
		
		if (wr != WR_OK)
			delete pWaveInfo;
	}

	return wr;
}

WaveResult CWaves::ParseData( const uchar* data, LPWAVEFILEINFO pWaveInfo, int& channels)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;
	WaveResult		wr = WR_BADWAVEFILE;

	if (!data || !pWaveInfo)
		return WR_INVALIDPARAM;

	memset(pWaveInfo, 0, sizeof(WAVEFILEINFO));

	// Read Wave file header
	m_callbacks.read_func( pWaveInfo, sizeof(WAVEFILEHEADER), (void*)m_stream);
	if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
	{
		while( m_callbacks.read_func( &riffChunk, sizeof(RIFFCHUNK), (void*)m_stream) == sizeof(RIFFCHUNK) )
		{
			if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
			{
				if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
				{
					m_callbacks.read_func( &waveFmt, riffChunk.ulChunkSize, (void*)m_stream);

					// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
					if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
					{
						pWaveInfo->wfType = WF_EX;
						memcpy(&pWaveInfo->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
						channels = pWaveInfo->wfEXT.Format.nChannels;
					}
					else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
					{
						pWaveInfo->wfType = WF_EXT;
						memcpy(&pWaveInfo->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
						channels = pWaveInfo->wfEXT.Format.nChannels;
					}
				}
				else
				{
					m_callbacks.seek_func( pWaveInfo->pData, riffChunk.ulChunkSize, SEEK_CUR);
				}
			}
			else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
			{
				pWaveInfo->ulDataSize = riffChunk.ulChunkSize;
				pWaveInfo->ulDataOffset = m_callbacks.tell_func(m_stream);
				m_callbacks.seek_func( m_stream, riffChunk.ulChunkSize, SEEK_CUR);
			}
			else
			{
				m_callbacks.seek_func( m_stream, riffChunk.ulChunkSize, SEEK_CUR);
			}

			// Ensure that we are correctly aligned for next chunk
			if (riffChunk.ulChunkSize & 1)
				m_callbacks.seek_func( m_stream, 1, SEEK_CUR);
		}

		if (pWaveInfo->ulDataSize && pWaveInfo->ulDataOffset && ((pWaveInfo->wfType == WF_EX) || (pWaveInfo->wfType == WF_EXT)))
			wr = WR_OK;
		else
			m_callbacks.close_func(m_stream);
	}

	return wr;
}

WaveResult CWaves::GetWaveData(WAVEID WaveID, void **lplpAudioData)
{
	if (!IsWaveID(WaveID))
		return WR_INVALIDWAVEID;

	if (!lplpAudioData)
		return WR_INVALIDPARAM;

	*lplpAudioData = m_WaveIDs[WaveID]->pData;

	return WR_OK;
}

WaveResult CWaves::GetWaveSize(WAVEID WaveID, ulong *size)
{
	if (!IsWaveID(WaveID))
		return WR_INVALIDWAVEID;

	if (!size)
		return WR_INVALIDPARAM;

	*size = m_WaveIDs[WaveID]->ulDataSize;

	return WR_OK;
}


WaveResult CWaves::GetWaveFrequency(WAVEID WaveID, ulong *pulFrequency)
{
	WaveResult wr = WR_OK;

	if (IsWaveID(WaveID))
	{
		if (pulFrequency)
			*pulFrequency = m_WaveIDs[WaveID]->wfEXT.Format.nSamplesPerSec;
		else
			wr = WR_INVALIDPARAM;
	}
	else
	{
		wr = WR_INVALIDWAVEID;
	}

	return wr;
}
bool CWaves::IsWaveID(WAVEID WaveID)
{
	bool bReturn = false;

	if ((WaveID >= 0) && (WaveID < MAX_NUM_WAVEID))
	{
		if (m_WaveIDs[WaveID])
			bReturn = true;
	}

	return bReturn;
}

WaveResult CWaves::DeleteWaveFile(WAVEID WaveID)
{
	WaveResult wr = WR_OK;

	if (IsWaveID(WaveID))
	{
		if (m_WaveIDs[WaveID]->pData)
			delete[] m_WaveIDs[WaveID]->pData;

		delete m_WaveIDs[WaveID];
		m_WaveIDs[WaveID] = 0;
	}
	else
	{
		wr = WR_INVALIDWAVEID;
	}

	return wr;
}

WaveResult CWaves::SetCallbacks(void * steam, wave_callbacks& call_back)
{
	if(!m_stream || (!m_callbacks.read_func &&
		!m_callbacks.seek_func && !m_callbacks.tell_func && !m_callbacks.close_func) )
	{
		return WR_INVALIDPARAM;
	}

	m_stream = steam;
	m_callbacks = call_back;

	return WR_OK;
}

bool CWaves::LoadWav(uchar* nameOrData, size_t dataSize,  int& frequency, int& channels, wave_callbacks& callback)
{
	WAVEID			WaveID;
	int				iDataSize, iFrequency;
	char			*pData;

	if (SUCCEED(CWaves::InstancePtr()->LoadWaveFile(nameOrData, dataSize, &WaveID, channels)))
	{
		if ((SUCCEED(CWaves::InstancePtr()->GetWaveSize(WaveID, (unsigned long*)&iDataSize))) &&
			(SUCCEED(CWaves::InstancePtr()->GetWaveData(WaveID, (void**)&pData))) &&
			(SUCCEED(CWaves::InstancePtr()->GetWaveFrequency(WaveID, (unsigned long*)&iFrequency))) ) 
		{
			CWaves::InstancePtr()->m_callbacks = callback;
			frequency = iFrequency;
			return true;
		}

		CWaves::InstancePtr()->DeleteWaveFile(WaveID);
	}

	return false;
}

CWaves* CWaves::InstancePtr()
{
	if(!m_instance)
		m_instance = new CWaves();

	return m_instance;
}


CWaves* CWaves::m_instance = NULL;
