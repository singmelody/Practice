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
#include "CWaves.h"

#include <stdio.h>
#include <memory>

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

#define SUCCEED(hr) (hr) >= 0


bool wave_open(LPWAVEFILEINFO& handle, wave_callbacks& callbacks, void* datasource)
{
	handle = new WAVEFILEINFO;
	if(!handle)
		return false;

	memset( handle, 0, sizeof(WAVEFILEINFO));
	handle->datasource = datasource;
	handle->callbacks = callbacks;

	if( parse_waveData( handle ) != WR_OK)
	{
		delete handle;
		handle = NULL;
		return false;
	}

	return true;
}

void wave_close(LPWAVEFILEINFO& handle)
{
	if(handle)
	{
		delete handle;
		handle = NULL;
	}
}

WaveResult load_wavfile(LPWAVEFILEINFO handle)
{
	if (SUCCEED(parse_waveData(handle)))
		return WR_OK;

	return WR_BADWAVEFILE;
}

WaveResult parse_waveData(LPWAVEFILEINFO handle)
{
	WAVEFILEHEADER	waveFileHeader;
	RIFFCHUNK		riffChunk;
	WAVEFMT			waveFmt;
	WaveResult		wr = WR_BADWAVEFILE;

	if ( !handle )
		return WR_INVALIDPARAM;

	// Read Wave file header
	handle->callbacks.read_func( &waveFileHeader, sizeof(WAVEFILEHEADER), handle->datasource);
	if (!_strnicmp(waveFileHeader.szRIFF, "RIFF", 4) && !_strnicmp(waveFileHeader.szWAVE, "WAVE", 4))
	{
		while( handle->callbacks.read_func( &riffChunk, sizeof(RIFFCHUNK), handle->datasource) == sizeof(RIFFCHUNK) )
		{
			if (!_strnicmp(riffChunk.szChunkName, "fmt ", 4))
			{
				if (riffChunk.ulChunkSize <= sizeof(WAVEFMT))
				{
					handle->callbacks.read_func( &waveFmt, riffChunk.ulChunkSize, handle->datasource);

					// Determine if this is a WAVEFORMATEX or WAVEFORMATEXTENSIBLE wave file
					if (waveFmt.usFormatTag == WAVE_FORMAT_PCM)
					{
						handle->wfType = WF_EX;
						memcpy(&handle->wfEXT.Format, &waveFmt, sizeof(PCMWAVEFORMAT));
					}
					else if (waveFmt.usFormatTag == WAVE_FORMAT_EXTENSIBLE)
					{
						handle->wfType = WF_EXT;
						memcpy(&handle->wfEXT, &waveFmt, sizeof(WAVEFORMATEXTENSIBLE));
					}
				}
				else
				{
					handle->callbacks.seek_func( handle->datasource, riffChunk.ulChunkSize, SEEK_CUR);
				}
			}
			else if (!_strnicmp(riffChunk.szChunkName, "data", 4))
			{
				handle->datasize = riffChunk.ulChunkSize;
				handle->dataoffset = handle->callbacks.tell_func(handle->datasource);
				handle->callbacks.seek_func( handle->datasource, riffChunk.ulChunkSize, SEEK_CUR);
			}
			else
			{
				handle->callbacks.seek_func( handle->datasource, riffChunk.ulChunkSize, SEEK_CUR);
			}

			// Ensure that we are correctly aligned for next chunk
			if (riffChunk.ulChunkSize & 1)
				handle->callbacks.seek_func( handle->datasource, 1, SEEK_CUR);
		}

		if (handle->datasize && handle->dataoffset && ((handle->wfType == WF_EX) || (handle->wfType == WF_EXT)))
		{
			handle->callbacks.seek_func( handle->datasource, handle->dataoffset, SEEK_SET);
			wr = WR_OK;
		}

		else
			handle->callbacks.close_func(handle->datasource);
	}

	return wr;
}

WaveResult wave_read(LPWAVEFILEINFO handle, uchar* buffer, int buffersize, int& writeSize)
{
	writeSize = handle->callbacks.read_func( buffer, buffersize, handle->datasource);
	
	if( !writeSize )
		return WR_BADWAVEFILE;

	handle->dataoffset += writeSize;
	return WR_OK;
}
