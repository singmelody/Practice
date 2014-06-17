#include "gtest.h"

#include <string>
#include <Windows.h>
#include "EngineConfig.h"
#include "Util/DllHelper.hpp"

#include <IEngine.hpp>
DEFINE_ENGINE_INTERFACE

using namespace std;
using namespace Dream;

HMODULE gEngineInstance;
CreateEngineFunc gEngineInitfunc;
//---------------------Engine Test-----------------------
TEST( ENGINE_TEST, ENGINE_LOAD_TEST)
{
	const char* engineDll = "Engine.dll";
	gEngineInstance = LoadDll(engineDll);

	ASSERT_TRUE( gEngineInstance != NULL);

}

TEST( ENGINE_TEST, ENGINE_GETINIT_TEST)
{
	gEngineInitfunc = (CreateEngineFunc)GetProc( gEngineInstance, CREATE_ENGINE_DLL_FUNC);
	ASSERT_TRUE( gEngineInitfunc != NULL);
}

TEST( ENGINE_TEST, ENGINE_INIT_TEST)
{
	gEngineInitfunc(&gEngine);
	ASSERT_TRUE( gEngine != NULL);
}


//--------------------Engine Test End---------------------


//---------------------Audio Test-----------------------
TEST( AUDIO_TEST, ENGINE_INIT_TEST)
{
	gEngine->GetIAudio();

	//IAudioPlayer* player = gEngine->GetIAudio()->CreateAudioPlayer();
}
//--------------------Audio Test End---------------------

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);



	return RUN_ALL_TESTS();
}
