// Waves.h: interface for the CWaves class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CWAVES_H_
#define _CWAVES_H_

#define MAX_NUM_WAVEID			1024

typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

#ifndef HAS_GUID 
#define HAS_GUID
typedef struct _GUID {
	ulong  Data1;
	ushort Data2;
	ushort Data3;
	uchar  Data4[ 8 ];
} GUID;
#endif

enum WAVEFILETYPE
{
	WF_EX  = 1,
	WF_EXT = 2
};

enum WaveResult
{
	WR_OK = 0,
	WR_INVALIDFILENAME					= - 1,
	WR_BADWAVEFILE						= - 2,
	WR_INVALIDPARAM						= - 3,
	WR_INVALIDWAVEID					= - 4,
	WR_NOTSUPPORTEDYET					= - 5,
	WR_WAVEMUSTBEMONO					= - 6,
	WR_WAVEMUSTBEWAVEFORMATPCM			= - 7,
	WR_WAVESMUSTHAVESAMEBITRESOLUTION	= - 8,
	WR_WAVESMUSTHAVESAMEFREQUENCY		= - 9,
	WR_WAVESMUSTHAVESAMEBITRATE			= -10,
	WR_WAVESMUSTHAVESAMEBLOCKALIGNMENT	= -11,
	WR_OFFSETOUTOFDATARANGE				= -12,
	WR_FILEERROR						= -13,
	WR_OUTOFMEMORY						= -14,
	WR_INVALIDSPEAKERPOS				= -15,
	WR_INVALIDWAVEFILETYPE				= -16,
	WR_NOTWAVEFORMATEXTENSIBLEFORMAT	= -17
};

typedef struct  
{
	size_t (*read_func)  (void *ptr, size_t size, void *datasource);
	int    (*seek_func)  (void *datasource, long offset, int whence);
	int    (*close_func) (void *datasource);
	long   (*tell_func)  (void *datasource);
}wave_callbacks;

typedef struct tWAVEFORMATEX
{
	ushort    wFormatTag;
	ushort    nChannels;
	ulong	  nSamplesPerSec;
	ulong     nAvgBytesPerSec;
	ushort    nBlockAlign;
	ushort    wBitsPerSample;
	ushort    cbSize;
} WAVEFORMATEX;

typedef struct {
	WAVEFORMATEX    Format;
	union {
		ushort wValidBitsPerSample;       /* bits of precision  */
		ushort wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
		ushort wReserved;                 /* If neither applies, set to zero. */
	} Samples;
	ushort           dwChannelMask;      /* which channels are */
	/* present in stream  */                               
	GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;


typedef struct
{
	WAVEFILETYPE	wfType;
	WAVEFORMATEXTENSIBLE wfEXT;		// For non-WAVEFORMATEXTENSIBLE wavefiles, the header is stored in the Format member of wfEXT
	ulong			datasize;
	ulong			dataoffset;
	void*			datasource;
	wave_callbacks	callbacks;
} WAVEFILEINFO, *LPWAVEFILEINFO;



bool wave_open( LPWAVEFILEINFO& handle, wave_callbacks& callbacks, void* datasource);
void wave_close( LPWAVEFILEINFO& handle);
WaveResult load_wavfile(LPWAVEFILEINFO handle);
WaveResult parse_waveData(LPWAVEFILEINFO handle);
WaveResult wave_read( LPWAVEFILEINFO handle, uchar* buffer, int buffersize, int& writeSize);
#endif // _CWAVES_H_
