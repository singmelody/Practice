#include "gtest.h"
#include "Demo.hpp"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"

using namespace std;
using namespace Dream;

HMODULE gEngineInstance;
CreateEngineFunc gEngineInitfunc;
DemoApp app;
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

TEST( ENGINE_TEST, ENGINE_CREATE_WINDOW)
{
	bool result = app.CreateWnd("Test",800,600);
	ASSERT_TRUE( result == true );

	HWND wnd = app.GetWnd();
	ASSERT_TRUE( wnd != NULL);

	result = gEngine->Init(wnd);
	ASSERT_TRUE( result != NULL);
}
//--------------------Engine Test End---------------------


//---------------------Audio Test-----------------------
TEST( AUDIO_TEST, AUDIO_INIT_TEST)
{
	IAudio* audioSystem =  gEngine->GetIAudio();
	ASSERT_TRUE( audioSystem != NULL);
}

TEST( AUDIO_TEST, AUDIO_PLAYER_WAV)
{
	IAudioPlayer* player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	// test wav
	player->SetName("../Media/audio/let_it_go.wav");
	bool result = player->Play();
	EXPECT_EQ( result, true);
}

TEST( AUDIO_TEST, AUDIO_PLAYER_OGG)
{
	IAudioPlayer* player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	// test ogg
	player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	player->SetName("../Media/audio/let_it_go.ogg");
	bool result = player->Play();
	EXPECT_EQ( result, true);
}

TEST( AUDIO_TEST, AUDIO_PLAYER_MP3)
{
	IAudioPlayer* player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	// test mp3
	player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	player->SetName("../Media/audio/let_it_go.mp3");
	bool result = player->Play();
	EXPECT_EQ( result, true);
}

TEST( AUDIO_TEST, AUDIO_PLAYER_INVALID)
{
	IAudioPlayer* player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true); 

	// test invalid
	player = gEngine->GetIAudio()->CreateAudioPlayer();
	EXPECT_EQ( player != NULL, true);

	player->SetName("../Media/audio/XXOO.mp3");
	bool result = player->Play();
	EXPECT_EQ( result, true);
}
//--------------------Audio Test End---------------------


//--------------------TexMgr Test Begin---------------------
TEST( TEXMGR_TEST, TEXMGR_INIT)
{
// 	ITextureManager* texMgr = gEngine->GetITextureManager();
// 	ASSERT_TRUE( texMgr != NULL );
// 
// 	ITexture* tex = texMgr->GetTexture("../meshes/brick01.dds");
// 	ASSERT_TRUE( tex != NULL);
}
//--------------------TexMgr Test End---------------------


//--------------------Math Lib Test Begin---------------------
TEST( MATH_TEST, MATH_MATRIX_TEST)
{
	Dream::Matrix mtx;

	mtx.Identity();

	ASSERT_TRUE( mtx.m[0][0] == 1.0f && mtx.m[1][1] == 1.0f 
		&& mtx.m[2][2] == 1.0f && mtx.m[3][3] == 1.0f);

	Vector3 vec( 1.0f, 2.0f, 3.0f);
	mtx.translation(vec);

	ASSERT_TRUE( mtx.m[3][0] == 1.0f && mtx.m[3][1] == 2.0f 
		&& mtx.m[3][2] == 3.0f && mtx.m[3][3] == 1.0f );
}

TEST( MATH_TEST, MATH_VEC_TEST)
{
	Vector3 vec( 1.0f, 2.0f, 3.0f);

	float value = vec.Dot(Vector3( 3.0f, 2.0f, 1.0f));
	ASSERT_TRUE( value == 10.0f);

	float length = vec.Length();
	ASSERT_TRUE( length == sqrt( 1.0f + 4.0f + 9.0f ));

	Vector3 mid = vec.Mid(vec);
	ASSERT_TRUE( vec == mid );

	vec.Normalize();
	ASSERT_FLOAT_EQ( vec.Length(), 1.0f );
}

TEST( MATH_TEST, MATH_QUA_TEST)
{
	Quaternion qua( 1.0f, 2.0f, 3.0f, 1.0f);
	Quaternion qua2( 3.0f, 2.0f, 1.0f, 1.0f);

// 
// 	Quaternion q = qua.Multiply(qua2);
// 	Quaternion result( 2.0f, 11.0f, -3.0f, -9.0f);

	qua.Normalize();

	ASSERT_FLOAT_EQ( qua.Length() , 1.0f );



	ASSERT_TRUE(q == result);
}
//--------------------TexMgr Test End---------------------

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);



	return RUN_ALL_TESTS();
}
