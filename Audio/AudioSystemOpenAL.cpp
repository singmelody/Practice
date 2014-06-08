#include "StdAfx.h"
#include "AudioSystemOpenAL.h"
#include "EngineConfig.h"


namespace Dream
{

AudioSystemOpenAL::AudioSystemOpenAL(void)
{
}


AudioSystemOpenAL::~AudioSystemOpenAL(void)
{
}

//--------------------------------------------------------------------
/// Dll Exports
//--------------------------------------------------------------------
extern "C" DLL_EXPORT void CreateAudio(IAudio** oEngine)
{
	static AudioSystemOpenAL  _sEngine;
	*oEngine = &_sEngine;
}

}
