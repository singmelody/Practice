//--------------------------------------------------------------------------------------
// File: Sample.cpp
//
// This sample shows a simple example of the Microsoft Direct3D's High-Level 
// Shader Language (HLSL) using the Effect interface. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "resource.h"
#include "skinnedMesh.h"
#include <vector>
#include "Utils.h"
#include <string>
#include "PhysicsMgr.h"
#include "Morph.h"
#include "Face.h"
#include "FaceController.h"
#include "FaceFacory.h"
#include <stdlib.h>
#include "InverseKinematics.h"
#include "WavDecoder.h"
#include "D3D11RenderDevice.h"
#include "Flock.h"
#include "Crowd.h"
#include "Hair.h"
//#define DEBUG_VS   // Uncomment this line to debug vertex shaders 
//#define DEBUG_PS   // Uncomment this line to debug pixel shaders 

 #pragma warning(disable:4996)
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
ID3DXFont*                  g_pFont = NULL;         // Font for drawing text
ID3DXSprite*                g_pSprite = NULL;       // Sprite for batching draw text calls
bool                        g_bShowHelp = true;     // If true, it renders the UI control text
bool						g_bShowSkeloton = false;
bool						g_bAnimtion = false;
CModelViewerCamera          g_Camera;               // A model viewing camera
CDXUTDialogResourceManager  g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg             g_SettingsDlg;          // Device settings dialog
CDXUTDialog                 g_HUD;                  // manages the 3D UI
CDXUTDialog                 g_SampleUI;             // dialog for sample specific controls
bool                        g_bEnablePreshader;     // if TRUE, then D3DXSHADER_NO_PRESHADER is used when compiling the shader
D3DXMATRIXA16               g_mCenterWorld;
float						g_Angle = 0.0f;
float						g_RadiusObject = 0.0f;
std::vector<D3DXMATRIX>		g_postions;

#define MAX_LIGHTS 3
CDXUTDirectionWidget g_LightControl[MAX_LIGHTS];
float                       g_fLightScale;
int                         g_nNumActiveLights;
int                         g_nActiveLight;
std::vector<IDirect3DTexture9*> m_textures;
D3DMATERIAL9				white;
std::vector<ID3DXAnimationController*> g_animControllers;
float						g_showTime = 0.0f;
bool						g_ShowOBB = false;
std::vector<FaceController*> g_faceControllers;
FaceFacory*					g_pFaceFactory = NULL;
FaceController*				g_speakController = NULL;
InverseKinematics*			g_ik = NULL;
Flock*						g_Flock;
Crowd*						g_Crowd;
float						g_decalCooldown;
Hair*						g_pHair;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           3
#define IDC_CHANGEDEVICE        4
#define IDC_ENABLE_PRESHADER    5
#define IDC_NUM_LIGHTS          6
#define IDC_NUM_LIGHTS_STATIC   7
#define IDC_ACTIVE_LIGHT        8
#define IDC_LIGHT_SCALE         9
#define IDC_LIGHT_SCALE_STATIC  10


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed,
                                  void* pUserContext );
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext );
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext );
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                void* pUserContext );
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext );
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext );
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void CALLBACK OnLostDevice( void* pUserContext );
void CALLBACK OnDestroyDevice( void* pUserContext );

void InitApp();
HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh );
void RenderText( double fTime );
void TrackStatus();
void ShowCallBackUI(double fTime);


class CallbackHandler : public ID3DXAnimationCallbackHandler
{
public:
	HRESULT CALLBACK HandleCallback(THIS_ UINT Track, LPVOID pCallbackData)
	{
		g_showTime = 0.25f;
		return D3D_OK;
	}	
};

CallbackHandler g_callbackHandler;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // Set the callback functions. These functions allow DXUT to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then DXUT won't be able to 
    // recreate your device resources.
    DXUTSetCallbackD3D9DeviceAcceptable( IsDeviceAcceptable );
    DXUTSetCallbackD3D9DeviceCreated( OnCreateDevice );
    DXUTSetCallbackD3D9DeviceReset( OnResetDevice );
    DXUTSetCallbackD3D9FrameRender( OnFrameRender );
    DXUTSetCallbackD3D9DeviceLost( OnLostDevice );
    DXUTSetCallbackD3D9DeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackKeyboard( KeyboardProc );
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );

    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();

    // Initialize DXUT and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.
    DXUTInit( true, true ); // Parse the command line and show msgboxes
    DXUTSetHotkeyHandling( true, true, true );
    DXUTCreateWindow( L"Sample" );
    DXUTCreateDevice( true, 640, 480 );

    // Pass control to DXUT for handling the message pump and 
    // dispatching render calls. DXUT will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    g_bEnablePreshader = true;

    for( int i = 0; i < MAX_LIGHTS; i++ )
        g_LightControl[i].SetLightDirection( D3DXVECTOR3( sinf( D3DX_PI * 2 * i / MAX_LIGHTS - D3DX_PI / 6 ),
                                                          1, -cosf( D3DX_PI * 2 * i / MAX_LIGHTS - D3DX_PI / 6 ) ) );

    g_nActiveLight = 0;
    g_nNumActiveLights = 1;
    g_fLightScale = 1.0f;

    // Initialize dialogs
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_SampleUI.Init( &g_DialogResourceManager );

    g_HUD.SetCallback( OnGUIEvent ); int iY = 10;
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

    g_SampleUI.SetCallback( OnGUIEvent ); iY = 10;

    WCHAR sz[100];
    iY += 24;
    swprintf_s( sz, 100, L"# Lights: %d", g_nNumActiveLights );
    g_SampleUI.AddStatic( IDC_NUM_LIGHTS_STATIC, sz, 35, iY += 24, 125, 22 );
    g_SampleUI.AddSlider( IDC_NUM_LIGHTS, 50, iY += 24, 100, 22, 1, MAX_LIGHTS, g_nNumActiveLights );

    iY += 24;
    swprintf_s( sz, 100, L"Light scale: %0.2f", g_fLightScale );
    g_SampleUI.AddStatic( IDC_LIGHT_SCALE_STATIC, sz, 35, iY += 24, 125, 22 );
    g_SampleUI.AddSlider( IDC_LIGHT_SCALE, 50, iY += 24, 100, 22, 0, 20, ( int )( g_fLightScale * 10.0f ) );

    iY += 24;
    g_SampleUI.AddButton( IDC_ACTIVE_LIGHT, L"Change active light (K)", 35, iY += 24, 125, 22, 'K' );
    g_SampleUI.AddCheckBox( IDC_ENABLE_PRESHADER, L"Enable preshaders", 35, iY += 24, 125, 22, g_bEnablePreshader );
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning E_FAIL.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat,
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    // No fallback defined by this app, so reject any device that 
    // doesn't support at least ps2.0
    if( pCaps->PixelShaderVersion < D3DPS_VERSION( 2, 0 ) )
        return false;

    // Skip backbuffer formats that don't support alpha blending
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                         AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
                                         D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
        return false;

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// DXUT will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    assert( DXUT_D3D9_DEVICE == pDeviceSettings->ver );

    HRESULT hr;
    IDirect3D9* pD3D = DXUTGetD3D9Object();
    D3DCAPS9 caps;

    V( pD3D->GetDeviceCaps( pDeviceSettings->d3d9.AdapterOrdinal,
                            pDeviceSettings->d3d9.DeviceType,
                            &caps ) );

    // If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
    // then switch to SWVP.
    if( ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) == 0 ||
        caps.VertexShaderVersion < D3DVS_VERSION( 1, 1 ) )
    {
        pDeviceSettings->d3d9.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }

    // Debugging vertex shaders requires either REF or software vertex processing 
    // and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
    if( pDeviceSettings->d3d9.DeviceType != D3DDEVTYPE_REF )
    {
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
        pDeviceSettings->d3d9.BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
        pDeviceSettings->d3d9.BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    }
#endif
#ifdef DEBUG_PS
    pDeviceSettings->d3d9.DeviceType = D3DDEVTYPE_REF;
#endif
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        if( pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF )
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
    }

    return true;
}

void RandomizeAnimations()
{
	int numAnimControllers = (int)g_animControllers.size();

	for(int i=0; i<numAnimControllers; i++)
	{
		int numAnimations = g_animControllers[i]->GetMaxNumAnimationSets();
		ID3DXAnimationSet *anim = NULL;
		g_animControllers[i]->GetAnimationSet(rand()%numAnimations, &anim);
		g_animControllers[i]->SetTrackAnimationSet(0, anim);
		SAFE_RELEASE(anim);
	}
}

void RandomBlendAnimations()
{
	// Reset the animation controller's time
	for (int i = 0; i < CONTROLLER_NUM; ++i)
	{
		g_animControllers[i]->ResetTime();

		// Get two random animation
		int numSet = g_animControllers[i]->GetMaxNumAnimationSets();
		ID3DXAnimationSet* anim1;
		ID3DXAnimationSet* anim2;
		g_animControllers[i]->GetAnimationSet(rand()%numSet, &anim1);
		g_animControllers[i]->GetAnimationSet(rand()%numSet, &anim2);

		// Assgin set to track
		g_animControllers[i]->SetTrackAnimationSet( 0, anim1);
		g_animControllers[i]->SetTrackAnimationSet( 1, anim2);

		// random weight
		float randomWeight = rand()%1000/1000.0f;
		g_animControllers[i]->SetTrackWeight( 0, randomWeight);
		g_animControllers[i]->SetTrackWeight( 1, 1.0f - randomWeight);

		// random speed
		g_animControllers[i]->SetTrackSpeed( 0, rand()% 1000 / 500.0f);
		g_animControllers[i]->SetTrackSpeed( 1, rand()% 1000 / 500.0f);

		// track properties
		g_animControllers[i]->SetTrackPriority( 0, D3DXPRIORITY_HIGH);
		g_animControllers[i]->SetTrackPriority( 1, D3DXPRIORITY_HIGH);

		// enable tracks
		g_animControllers[i]->SetTrackEnable( 0, true);
		g_animControllers[i]->SetTrackEnable( 1, true);

		SAFE_RELEASE(anim1);
		SAFE_RELEASE(anim2);
	}
}

void RandomCompressedCallbackAnimations()
{

	//Get the "Aim" Animation set
	ID3DXKeyframedAnimationSet* animSet = NULL;
	g_animControllers[0]->GetAnimationSet(1, (ID3DXAnimationSet**)&animSet);

	//Compress the animation set
	ID3DXBuffer* compressedData = NULL;
	animSet->Compress(D3DXCOMPRESS_DEFAULT, 0.5f, NULL, &compressedData);

	//Create one callback key
	const UINT numCallbacks = 1;
	D3DXKEY_CALLBACK keys[numCallbacks];

	// GetSourceTicksPerSecond() returns the number
	// animation key frame ticks that occur per second.
	// Callback keyframe times are tick based.
	double ticks = animSet->GetSourceTicksPerSecond();

	// Set the first callback key to trigger a callback
	// half way through the animation sequence.
	keys[0].Time = float(animSet->GetPeriod()/2.0f*ticks);
	keys[0].pCallbackData = (void*)&g_SkinnedMesh;

	// Create the ID3DXCompressedAnimationSet interface
	// with the callback keys.
	ID3DXCompressedAnimationSet* compressedAnimSet = NULL;
	D3DXCreateCompressedAnimationSet(animSet->GetName(), 
		animSet->GetSourceTicksPerSecond(),
		animSet->GetPlaybackType(), compressedData,
		numCallbacks, keys, &compressedAnimSet);

	//Delete the old keyframed animation set.
	g_animControllers[0]->UnregisterAnimationSet(animSet);

	// and then add the new compressed animation set.
	g_animControllers[0]->RegisterAnimationSet(compressedAnimSet);

	// Hook up the animation set to the first track.
	g_animControllers[0]->SetTrackAnimationSet(0, compressedAnimSet);

	SAFE_RELEASE(animSet);
	SAFE_RELEASE(compressedData);
	SAFE_RELEASE(compressedAnimSet);
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
                                 void* pUserContext )
{
	//_CrtSetBreakAlloc(3132);
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D9CreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnD3D9CreateDevice( pd3dDevice ) );
    // Initialize the font
    V_RETURN( D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
                              OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                              L"Arial", &g_pFont ) );

	// init white material
	white.Ambient = white.Specular = white.Diffuse  = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	white.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	white.Power = 1.0f;

    // Load the mesh

	// Load the skeleton
	g_SkinnedMesh = new SkinnedMesh();
	g_SkinnedMesh->Load(L"meshes\\soldier.X");

	// setup ik
	g_ik = new InverseKinematics(g_SkinnedMesh);

	// init multi animation position
	for (int i = 0; i < CONTROLLER_NUM; ++i)
	{
		D3DXMATRIX m;
		D3DXMatrixTranslation(&m, 0.0f, 0.0f, 0.0f);
		g_postions.push_back(m);
		g_animControllers.push_back( g_SkinnedMesh->GetController() );
	}

	srand(GetTickCount());

	// set still Anim
	ID3DXAnimationSet *anim = NULL;
	g_animControllers[0]->GetAnimationSet(2, &anim);
	g_animControllers[0]->SetTrackAnimationSet(0, anim);
	anim->Release();

//	RandomizeAnimations();
//	RandomBlendAnimations();
//	RandomCompressedCallbackAnimations();

    D3DXVECTOR3 vCenter = D3DXVECTOR3(0.0f,0.0f,0.0f);

    D3DXMatrixTranslation( &g_mCenterWorld, -vCenter.x, -vCenter.y, -vCenter.z );
    D3DXMATRIXA16 m;
    D3DXMatrixRotationY( &m, D3DX_PI );
    g_mCenterWorld *= m;
    D3DXMatrixRotationX( &m, D3DX_PI / 2.0f );
    g_mCenterWorld *= m;

	// Create Sprite
	D3DXCreateLine( pd3dDevice, &g_Line);
	g_Anim = new Animation();

    V_RETURN( CDXUTDirectionWidget::StaticOnD3D9CreateDevice( pd3dDevice ) );
    for( int i = 0; i < MAX_LIGHTS; i++ )
        g_LightControl[i].SetRadius( g_RadiusObject );

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DXSHADER_DEBUG;
    #endif

#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
    #endif
#ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
    #endif

    // Preshaders are parts of the shader that the effect system pulls out of the 
    // shader and runs on the host CPU. They should be used if you are GPU limited. 
    // The D3DXSHADER_NO_PRESHADER flag disables preshaders.
    if( !g_bEnablePreshader )
        dwShaderFlags |= D3DXSHADER_NO_PRESHADER;

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Sample.fx" ) );
    V_RETURN( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, NULL, &g_pEffect, NULL ) );

    // Set effect variables as needed
    D3DXCOLOR colorMtrlDiffuse( 1.0f, 1.0f, 1.0f, 1.0f );
    D3DXCOLOR colorMtrlAmbient( 0.35f, 0.35f, 0.35f, 0 );

    V_RETURN( g_pEffect->SetValue( "g_MaterialAmbientColor", &colorMtrlAmbient, sizeof( D3DXCOLOR ) ) );
    V_RETURN( g_pEffect->SetValue( "g_MaterialDiffuseColor", &colorMtrlDiffuse, sizeof( D3DXCOLOR ) ) );

	// create physcis
// 	g_physicsEngine = new PhysicsManager();
// 	g_physicsEngine->Init();

	// create morph anim
	g_Morph = new Morph();
	g_Morph->Init();

	// create multi morph
	g_MultiMorph = new MultiMorph();
	g_MultiMorph->Init();

	// create morph & skeleton
	g_MorphSkeleton = new MorphMesh();
	g_MorphSkeleton->Init();

	// create face
	g_Face = new Face();
	g_Face->Init();

	// create complex face
	g_ComplexFace = new ComplexFace("meshes\\face.x");

	// create face model
 	g_FaceModel = new FaceModel("meshes\\face.x");

	// create face factory
	g_pFaceFactory = new FaceFacory("meshes\\factory_face.x");
 
	g_FaceModelGenerate = g_pFaceFactory->GenerateRandomFace();
	g_FaceControllerGenerate = new FaceController(D3DXVECTOR3(0.0f,0.0f,0.0f), g_FaceModelGenerate);

	// Create face controllers
	g_faceControllers.push_back(new FaceController(D3DXVECTOR3(0.0f,0.0f,0.0f), g_FaceModel));
	g_faceControllers.push_back(new FaceController(D3DXVECTOR3(0.3f,0.0f,0.0f), g_FaceModel));
	g_faceControllers.push_back(new FaceController(D3DXVECTOR3(-0.3f,0.0f,0.0f), g_FaceModel));

	// create speak face controller
	g_speakController = new FaceController( D3DXVECTOR3(0.0f,0.0f,0.0f), g_FaceModel);

// 	bool result = D3D11RenderDevice::Instance().CreateDevice();
// 	result = D3D11RenderDevice::Instance().CheckCaps();
// 	result = D3D11RenderDevice::Instance().CreateSwapChain();
// 	result = D3D11RenderDevice::Instance().CreateRenderTargetView();
// 	result = D3D11RenderDevice::Instance().CreateDepthStencilBuffer();
// 	result = D3D11RenderDevice::Instance().BindRTAndDepthToMS();
// 	assert( D3D11RenderDevice::Instance().CreateViewPort() );
// 	assert( D3D11RenderDevice::Instance().ShaderParse() );
// 	assert( D3D11RenderDevice::Instance().CreateVertexDecl() );
// 	assert( D3D11RenderDevice::Instance().CreateGBuffer() );

	g_Flock = new Flock(50);

	g_Crowd = new Crowd(50);

	g_pHair = new Hair();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// This function loads the mesh and ensures the mesh has normals; it also optimizes the 
// mesh for the graphics card's vertex cache, which improves performance by organizing 
// the internal triangle list for less cache misses.
//--------------------------------------------------------------------------------------
// HRESULT LoadMesh( IDirect3DDevice9* pd3dDevice, WCHAR* strFileName, ID3DXMesh** ppMesh )
// {
//     ID3DXMesh* pMesh = NULL;
//     WCHAR str[MAX_PATH];
//     HRESULT hr;
// 
// 	//Load new mesh
// 	ID3DXBuffer * materialBfr = NULL;
// 	DWORD materialCount = NULL;
// 
//     V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, strFileName ) );
//     V_RETURN( D3DXLoadMeshFromX( str, D3DXMESH_MANAGED, pd3dDevice, NULL, &materialBfr, NULL, &materialCount, &pMesh ) );
// 
// 	D3DXMATERIAL *mtrls = (D3DXMATERIAL*)materialBfr->GetBufferPointer();
// 	for(int i=0;i<(int)materialCount;i++)
// 	{
// 		m_materials.push_back(mtrls[i].MatD3D);
// 
// 		//Load textures for each material
// 		if(mtrls[i].pTextureFilename != NULL)
// 		{			
// 			char textureFileName[90];
// 			strcpy(textureFileName, PATH_TO_TEXTURES);
// 			strcat(textureFileName, mtrls[i].pTextureFilename);
// 			const wchar_t* conv = GetWC(textureFileName);
// 			V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, conv) );
// 			delete conv;
// 
// 			IDirect3DTexture9 * newTexture = NULL;
// 
// 			V_RETURN( D3DXCreateTextureFromFileEx( pd3dDevice, str, D3DX_DEFAULT, D3DX_DEFAULT,
// 				D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
// 				D3DX_DEFAULT, D3DX_DEFAULT, 0,
// 				NULL, NULL, &newTexture ) );
// 			m_textures.push_back(newTexture);
// 		}
// 		else
// 		{
// 			m_textures.push_back(NULL);
// 		}
// 	}
// 
//     DWORD* rgdwAdjacency = NULL;
// 
//     // Make sure there are normals which are required for lighting
// 	DWORD vertexFormat = pMesh->GetFVF();
//     if( !( vertexFormat & D3DFVF_NORMAL ) )
//     {
//         ID3DXMesh* pTempMesh;
//         V( pMesh->CloneMeshFVF( pMesh->GetOptions(),
//                                 pMesh->GetFVF() | D3DFVF_NORMAL,
//                                 pd3dDevice, &pTempMesh ) );
//         V( D3DXComputeNormals( pTempMesh, NULL ) );
// 
//         SAFE_RELEASE( pMesh );
//         pMesh = pTempMesh;
//     }
// 
//     // Optimize the mesh for this graphics card's vertex cache 
//     // so when rendering the mesh's triangle list the vertices will 
//     // cache hit more often so it won't have to re-execute the vertex shader 
//     // on those vertices so it will improve perf.     
//     rgdwAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
//     if( rgdwAdjacency == NULL )
//         return E_OUTOFMEMORY;
// 
//     V( pMesh->GenerateAdjacency( 1e-6f, rgdwAdjacency ) );
//     V( pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE, rgdwAdjacency, NULL, NULL, NULL ) );
//     delete []rgdwAdjacency;
// 
//     *ppMesh = pMesh;
// 
// 	pMesh->Release();
// 
//     return S_OK;
// }


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// reset, which will happen after a lost device scenario. This is the best location to 
// create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever 
// the device is lost. Resources created here should be released in the OnLostDevice 
// callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,
                                const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnD3D9ResetDevice() );
    V_RETURN( g_SettingsDlg.OnD3D9ResetDevice() );

    if( g_pFont )
        V_RETURN( g_pFont->OnResetDevice() );
    if( g_pEffect )
        V_RETURN( g_pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &g_pSprite ) );

    for( int i = 0; i < MAX_LIGHTS; i++ )
        g_LightControl[i].OnD3D9ResetDevice( pBackBufferSurfaceDesc );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    g_Camera.SetProjParams( D3DX_PI / 4, fAspectRatio, 0.1f, 100.0f );
    g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );
    g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 300 );
    g_SampleUI.SetSize( 170, 300 );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// This callback function will be called once at the beginning of every frame. This is the
// best location for your application to handle updates to the scene, but is not 
// intended to contain actual rendering calls, which should instead be placed in the 
// OnFrameRender callback.  
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    // Update the camera's position based on user input 
    g_Camera.FrameMove( fElapsedTime );
}


// just for wrapper render shadow
void RenderShadow()
{
	D3DXMATRIX identity, shadow;
	D3DXMatrixIdentity(&identity);

	//Set ground plane + light position
	D3DXPLANE ground(0.0f, -1.0f, 0.0f, 0.0f);
	D3DXVECTOR4 lightPos(20.0f, -75.0f, 120.0f, 0.0f);

	//Create the shadow matrix
	D3DXMatrixShadow(&shadow, &lightPos, &ground);

	g_SkinnedMesh->SetShadowMatrix(shadow);

#if SOFT
	g_SkinnedMesh->RenderSoft(NULL,"ShadowSoft","ShadowSoft", true);
#else
	g_SkinnedMesh->RenderHAL(NULL,"ShadowHAL","ShadowSoft", true);
#endif
}

void Speak()
{
	//Stop old sounds
	PlaySound(0, 0, 0);

	//play voice	
	std::wstring audioPath = GetWC("audio\\voice.wav");
	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, audioPath.c_str() );
	PlaySound(str,NULL,SND_FILENAME|SND_ASYNC); 

	//Create viseme queue
	std::vector<Viseme> visemes;
	visemes.push_back(Viseme(0, 0.0f,  0.0f));
	visemes.push_back(Viseme(1, 1.0f,  0.25f));
	visemes.push_back(Viseme(0, 0.0f,  0.5f));
	visemes.push_back(Viseme(1, 0.75f, 0.65f));
	visemes.push_back(Viseme(0, 0.0f,  1.0f));
	visemes.push_back(Viseme(0, 0.0f,  1.25f));
	visemes.push_back(Viseme(3, 1.0f,  1.35f));
	visemes.push_back(Viseme(5, 1.0f,  1.5f));
	visemes.push_back(Viseme(1, 1.0f,  1.58f));
	visemes.push_back(Viseme(5, 1.0f,  1.71f));
	visemes.push_back(Viseme(3, 0.75f, 1.83f));
	visemes.push_back(Viseme(3, 0.5f,  1.9f));
	visemes.push_back(Viseme(1, 1.0f,  2.1f));
	visemes.push_back(Viseme(4, 1.0f,  2.3f));
	visemes.push_back(Viseme(4, 0.0f,  2.5f));

	g_speakController->Speak(visemes);
}

void SpeakWav()
{
	std::wstring audioPath = GetWC("audio\\voice.wav");
	WCHAR str[MAX_PATH];
	DXUTFindDXSDKMediaFileCch( str, MAX_PATH, audioPath.c_str() );

	//Load Wave file
	WavDecoder wf;
	wf.Load(str);

	//Stop old sounds
	PlaySound(0, 0, 0);

	//play voice	

	PlaySound(str,NULL,SND_FILENAME|SND_ASYNC); 

	g_speakController->SpeakWav(wf);
}


//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, DXUT will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( fElapsedTime );
        return;
    }
	HRESULT hr;
	D3DXMATRIXA16 mViewProjection;
	D3DXVECTOR3 vLightDir[MAX_LIGHTS];
	D3DXCOLOR vLightDiffuse[MAX_LIGHTS];
	UINT iPass, cPasses;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;

//	D3D11RenderDevice::Instance().Render();

    // Clear the render target and the zbuffer 
    V( pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DXCOLOR( 0.0f, 0.25f, 0.25f, 0.55f ), 1.0f, 0 ) );

    // Render the scene
    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
		// Setup the camera's view parameters
//		g_Angle += fElapsedTime;
//		D3DXVECTOR3 vecEye( cos(g_Angle) * 0.8f, 0.0f, sin(g_Angle) * 0.8f);
		D3DXVECTOR3 vecEye( 0.0f, 6.0f, 8.0f);
		D3DXVECTOR3 vecAt ( 0.0f, 2.0f, 0.0f );
		g_Camera.SetViewParams( &vecEye, &vecAt );
//		g_Camera.SetRadius( g_RadiusObject * 3.0f, g_RadiusObject * 0.5f, g_RadiusObject * 10.0f );

		// set view projection prop
		mProj = *g_Camera.GetProjMatrix();
		mView = *g_Camera.GetViewMatrix();
		mViewProjection = mView * mProj;
		V( g_pEffect->SetMatrix( "g_mVP", &mViewProjection ) );
		V( g_pEffect->SetVector( "g_mCameraPos", &D3DXVECTOR4(vecEye,0.0f)));

		// set world prop
		D3DXMatrixIdentity(&mWorld); //= g_mCenterWorld * *g_Camera.GetWorldMatrix();
		V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );

		// Render the light arrow so the user can visually see the light dir
        for( int i = 0; i < g_nNumActiveLights; i++ )
        {
            D3DXCOLOR arrowColor = ( i == g_nActiveLight ) ? D3DXCOLOR( 1, 1, 0, 1 ) : D3DXCOLOR( 1, 1, 1, 1 );
            V( g_LightControl[i].OnRender9( arrowColor, &mView, &mProj, g_Camera.GetEyePt() ) );
            vLightDir[i] = g_LightControl[i].GetLightDirection(); //  D3DXVECTOR3(-20.0f, 75.0f, -120.0f);
            vLightDiffuse[i] = g_fLightScale * D3DXCOLOR( 1, 1, 1, 1 );
        }

        V( g_pEffect->SetValue( "g_LightDir", vLightDir, sizeof( D3DXVECTOR3 ) * MAX_LIGHTS ) );
        V( g_pEffect->SetValue( "g_LightDiffuse", vLightDiffuse, sizeof( D3DXVECTOR4 ) * MAX_LIGHTS ) );

        // Update the effect's variables.  Instead of using strings, it would 
        // be more efficient to cache a handle to the parameter by calling 
        // ID3DXEffect::GetParameterByName
        V( g_pEffect->SetFloat( "g_fTime", ( float )fTime ) );

        D3DXCOLOR vWhite = D3DXCOLOR( 1, 1, 1, 1 );
        V( g_pEffect->SetValue( "g_MaterialDiffuseColor", &vWhite, sizeof( D3DXCOLOR ) ) );
        V( g_pEffect->SetFloat( "g_fTime", ( float )fTime ) );
        V( g_pEffect->SetInt( "g_nNumLights", g_nNumActiveLights ) );

        // Render the scene with this technique 
        // as defined in the .fx file

		std::string techName;
        switch( g_nNumActiveLights )
        {
            case 1:
				techName = "RenderSceneWithTexture1Light"; break;
            case 2:
				techName = "RenderSceneWithTexture2Light"; break;
            case 3:
				techName = "RenderSceneWithTexture3Light"; break;
        }

// 		for (int i = 0; i < CONTROLLER_NUM; ++i)
// 		{
// 			//g_animControllers[i]->AdvanceTime( fElapsedTime * 0.5f, &g_callbackHandler);
// 			g_animControllers[i]->AdvanceTime( fElapsedTime, NULL);
// 			g_SkinnedMesh->SetPose( g_postions[i]);
// 
// 			// update ik
// 			if(g_ik)
// 			{
// 				g_ik->UpdateHeadIK();
// 				g_ik->UpdateArmIK();
// 			}
// #ifdef SOFT
// 			// Apply the SoftSkin technique contained in the effect  
// 			g_SkinnedMesh->RenderSoft(NULL, "SkinSoft", techName.c_str());
// #else
// 			// Apply the HALSkin technique contained in the effect
// 			g_SkinnedMesh->RenderHAL(NULL, "SkinHAL", techName.c_str());
// #endif
// 
// 		}

		// set view projection prop
		mProj = *g_Camera.GetProjMatrix();
		mView = *g_Camera.GetViewMatrix();
		mViewProjection = mView * mProj;
		V( g_pEffect->SetMatrix( "g_mVP", &mViewProjection ) );

		// set world prop
		D3DXMatrixIdentity(&mWorld); //= g_mCenterWorld * *g_Camera.GetWorldMatrix();
		V( g_pEffect->SetMatrix( "g_mWorld", &mWorld ) );

		// set texture
		g_pEffect->SetTexture("g_MeshTexture", NULL);
//		g_Morph->Render(techName.c_str());

// 		g_MultiMorph->Update(fElapsedTime);
// 		g_MultiMorph->Render(techName.c_str());

// 		g_MorphSkeleton->Update(fElapsedTime);
// 		g_MorphSkeleton->Render();

// 		g_Face->Update(fElapsedTime);
// 		g_Face->Render(techName.c_str());

// 		g_ComplexFace->Update(fElapsedTime);
// 		g_ComplexFace->Render(techName.c_str());

// 		// Update face controllers
// 		for (int i = 0; i < g_faceControllers.size(); ++i)
// 		{
// 			g_faceControllers[i]->Update(fElapsedTime);
// 			g_faceControllers[i]->Render(techName.c_str());
// 		}

// 		g_Flock->Update(fElapsedTime);
// 		g_Flock->Render(false);

		//Update crowd
		//g_Crowd->Update(fElapsedTime);
		//g_Crowd->Render();

		// Decals
		//Set Ray Org & Dir	
// 		POINT p;
// 		GetCursorPos(&p);
// 
// 		float x = (p.x - 320) / 640.0f;
// 		float y = (p.y - 240) / -480.0f;
// 		g_rayOrg = D3DXVECTOR3(0.0f, 1.0f, -3.0f);
// 		g_rayDir = D3DXVECTOR3(x, y, 1.0f);
// 		D3DXVec3Normalize(&g_rayDir, &g_rayDir);

		//render intersection ray
// 		{
// 			LineVertex vert[] = {LineVertex(g_rayOrg, 0xffff0000), LineVertex(g_rayOrg + g_rayDir * 3.0f, 0xff00ff00)};
// 			DXUTGetD3D9Device()->SetRenderState(D3DRS_LIGHTING, false);
// 			DXUTGetD3D9Device()->SetFVF(LineVertex::FVF);
// 			DXUTGetD3D9Device()->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vert[0], sizeof(LineVertex));
// 		}

		//Render Drone
// 		{
// 			D3DXMATRIX identity;
// 			D3DXMatrixIdentity(&identity);
// 			g_pEffect->SetMatrix("g_mWorld", &identity);
// 			g_pEffect->SetMatrix("g_mVP", &(mView * mProj));
// 
// 			g_animControllers[0]->AdvanceTime(fElapsedTime, NULL);
// 			g_SkinnedMesh->SetPose(identity);
// 			g_SkinnedMesh->RenderHAL(NULL, "SkinHAL", techName.c_str());
// 		}

// 		g_FaceControllerGenerate->Update(fElapsedTime);
// 		g_FaceControllerGenerate->Render(techName.c_str());

// 		g_speakController->SpeakUpdate(fElapsedTime);
// 		g_speakController->Render(techName.c_str());


		// Physics
// 		g_physicsEngine->Update(fElapsedTime);
// 		g_physicsEngine->Render(techName.c_str(),g_ShowOBB);


		// Render Shadow
		{
//			RenderShadow();
		}

		// Hair
		g_pHair->Update(fElapsedTime);
		g_pHair->Render();

		pd3dDevice->SetTransform(D3DTS_WORLD, &mWorld);
		pd3dDevice->SetTransform(D3DTS_VIEW, &mView);
		pd3dDevice->SetTransform(D3DTS_PROJECTION, g_Camera.GetProjMatrix());

		if(g_bShowSkeloton)
		{
			pd3dDevice->Clear(0L, NULL, D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0L);
			g_SkinnedMesh->RenderSkeleton(NULL, NULL, mWorld);

			g_Anim->Update(fElapsedTime);
			g_Anim->Draw();
		}


        g_HUD.OnRender( fElapsedTime );
        g_SampleUI.OnRender( fElapsedTime );

        RenderText( fTime );
//		TrackStatus();
		ShowCallBackUI( fElapsedTime );

        V( pd3dDevice->EndScene() );
    }

	pd3dDevice->Present(0, 0, 0, 0);
}

std::string IntToString(int i)
{
	char num[10];
	_itoa(i, num, 10);
	return num;
}

void TrackStatus()
{
	//Print the status of all the Tracks
	g_Line->SetWidth(100.0f);
	g_Line->Begin();
	D3DXVECTOR2 p[] = {D3DXVECTOR2(0, 550), D3DXVECTOR2(800, 550)};
	g_Line->Draw(p, 2, 0x88FFFFFF);
	g_Line->End();

 	int numTracks = g_animControllers[0]->GetMaxNumTracks();
	for(int i=0; i<numTracks; i++)
	{
		D3DXTRACK_DESC desc;
		ID3DXAnimationSet* anim = NULL;
		g_animControllers[0]->GetTrackDesc(i, &desc);
		g_animControllers[0]->GetTrackAnimationSet(i, &anim);

		std::string name = anim->GetName();
		while(name.size() < 10)name.push_back(' ');

		std::string s = std::string("Track #") + IntToString(i + 1) + name;
		s += std::string("Weight = ") + IntToString((int)(desc.Weight * 100)) + "%";
		s += std::string(", Position = ") + IntToString((int)(desc.Position * 1000)) + " ms";
		s += std::string(", Speed = ") + IntToString((int)(desc.Speed * 100)) + "%";

		RECT r = {10, 350 + i * 20, 0, 0};
		std::wstring conv = GetWC(s.c_str());
		g_pFont->DrawText(NULL, conv.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_NOCLIP, 0xAA00FF00);
	}

	RECT rc = {10, 330, 0, 0};
	g_pFont->DrawText(NULL, L"Press Return to randomize animations", -1, &rc, DT_LEFT | DT_TOP | DT_NOCLIP, 0x66000000);
}

void ShowCallBackUI(double deltaTime)
{
	if(g_showTime > 0.0f)
	{
		RECT rc = {0, 0, 640, 480};
		g_pFont->DrawText(NULL, L"BANG!", -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFF000000);
		SetRect(&rc, -5, -5, 635, 475);
		g_pFont->DrawText(NULL, L"BANG!", -1, &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, 0xFFFFFF00);

		g_showTime -= deltaTime;
	}
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText( double fTime )
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work fine however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves perf.
    CDXUTTextHelper txtHelper( g_pFont, g_pSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 2, 0 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );

    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
    txtHelper.DrawFormattedTextLine( L"fTime: %0.1f  sin(fTime): %0.4f", fTime, sin( fTime ) );

    // Draw help
    if( g_bShowHelp )
    {
        const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetD3D9BackBufferSurfaceDesc();
        txtHelper.SetInsertionPos( 2, pd3dsdBackBuffer->Height - 15 * 6 );
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 0.75f, 0.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Controls:" );

        txtHelper.SetInsertionPos( 20, pd3dsdBackBuffer->Height - 15 * 5 );
        txtHelper.DrawTextLine( L"Rotate model: Left mouse button\n"
                                L"Rotate light: Right mouse button\n"
                                L"Rotate camera: Middle mouse button\n"
                                L"Zoom camera: Mouse wheel scroll\n" );

        txtHelper.SetInsertionPos( 250, pd3dsdBackBuffer->Height - 15 * 5 );
        txtHelper.DrawTextLine( L"Hide help: F1\n"
                                L"Quit: ESC\n" );
    }
    else
    {
        txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        txtHelper.DrawTextLine( L"Press F1 for help" );
    }
    txtHelper.End();
}


//--------------------------------------------------------------------------------------
// Before handling window messages, DXUT passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    *pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;
    *pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    g_LightControl[g_nActiveLight].HandleMessages( hWnd, uMsg, wParam, lParam );

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	g_decalCooldown -= 0.3f;
    if( bKeyDown )
    {
        switch( nChar )
        {
            case VK_F1:
                g_bShowHelp = !g_bShowHelp; break;
			case VK_SPACE:
				g_bShowSkeloton = !g_bShowSkeloton; break;
			case VK_RETURN:
				{
					Sleep(300);
					RandomBlendAnimations();
					break;
				}
			case VK_NUMPAD1:
				{
					g_physicsEngine->ResetJoint(HINGE);
					break;
				}
			case VK_NUMPAD2:
				{
					g_physicsEngine->ResetJoint(TWISTCONE);
					break;
				}
			case VK_NUMPAD3:
				{
					g_physicsEngine->ResetJoint(BALLPOINT);
					break;
				}
			case VK_NUMPAD4:
				{
					g_ShowOBB  = !g_ShowOBB;
					break;
				}
			case VK_NUMPAD5:
				{
					float m_blend = g_Morph->GetBlend();
					float blend = min(m_blend + 0.5 * 0.5f, 2.0f);
					g_Morph->SetBlend(blend);
					g_Morph->Update();
					break;
				}
			case VK_NUMPAD6:
				{
					float m_blend = g_Morph->GetBlend();
					float blend = max(m_blend - 0.5 * 0.5f, -1.0f);
					g_Morph->SetBlend(blend);
					g_Morph->Update();
					break;
				}
			case VK_NUMPAD7:
				{
					SAFE_DELETE(g_FaceModelGenerate);
					SAFE_DELETE(g_FaceControllerGenerate);

					g_FaceModelGenerate = g_pFaceFactory->GenerateRandomFace();
					g_FaceControllerGenerate = new FaceController(D3DXVECTOR3(0.0f,0.0f,0.0f), g_FaceModelGenerate);

					break;
				}
			case VK_NUMPAD8:
				{
					Speak();
					break;
				}
			case VK_NUMPAD9:
				{
					SpeakWav();
					break;
				}
			case VK_NUMPAD0:
				{
					if( g_decalCooldown <= 0.0f )
					{
						g_SkinnedMesh->AddDecal();
						g_decalCooldown = 0.3f;
					}
				}

        }
    }
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{

    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen(); break;
        case IDC_TOGGLEREF:
            DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:
            g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;

        case IDC_ENABLE_PRESHADER:
        {
            g_bEnablePreshader = g_SampleUI.GetCheckBox( IDC_ENABLE_PRESHADER )->GetChecked();

            if( DXUTGetD3D9Device() != NULL )
            {
                OnLostDevice( NULL );
                OnDestroyDevice( NULL );
                OnCreateDevice( DXUTGetD3D9Device(), DXUTGetD3D9BackBufferSurfaceDesc(), NULL );
                OnResetDevice( DXUTGetD3D9Device(), DXUTGetD3D9BackBufferSurfaceDesc(), NULL );
            }
            break;
        }

        case IDC_ACTIVE_LIGHT:
            if( !g_LightControl[g_nActiveLight].IsBeingDragged() )
            {
                g_nActiveLight++;
                g_nActiveLight %= g_nNumActiveLights;
            }
            break;

        case IDC_NUM_LIGHTS:
            if( !g_LightControl[g_nActiveLight].IsBeingDragged() )
            {
                WCHAR sz[100];
                swprintf_s( sz, 100, L"# Lights: %d", g_SampleUI.GetSlider( IDC_NUM_LIGHTS )->GetValue() );
                g_SampleUI.GetStatic( IDC_NUM_LIGHTS_STATIC )->SetText( sz );

                g_nNumActiveLights = g_SampleUI.GetSlider( IDC_NUM_LIGHTS )->GetValue();
                g_nActiveLight %= g_nNumActiveLights;
            }
            break;

        case IDC_LIGHT_SCALE:
            g_fLightScale = ( float )( g_SampleUI.GetSlider( IDC_LIGHT_SCALE )->GetValue() * 0.10f );

            WCHAR sz[100];
            swprintf_s( sz, 100, L"Light scale: %0.2f", g_fLightScale );
            g_SampleUI.GetStatic( IDC_LIGHT_SCALE_STATIC )->SetText( sz );
            break;
    }

}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D9LostDevice();
    g_SettingsDlg.OnD3D9LostDevice();
    CDXUTDirectionWidget::StaticOnD3D9LostDevice();
    if( g_pFont )
        g_pFont->OnLostDevice();
    if( g_pEffect )
        g_pEffect->OnLostDevice();
    SAFE_RELEASE( g_pSprite );

}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnD3D9DestroyDevice();
    g_SettingsDlg.OnD3D9DestroyDevice();
    CDXUTDirectionWidget::StaticOnD3D9DestroyDevice();
    SAFE_RELEASE( g_pEffect );
    SAFE_RELEASE( g_pFont );
	SAFE_RELEASE( g_Line );
	SAFE_DELETE( g_SkinnedMesh );
	SAFE_DELETE( g_Anim );

	SAFE_RELEASE(g_physicsEngine);
	SAFE_DELETE( g_physicsEngine );

	for (int i = 0; i < g_animControllers.size(); ++i)
	{
		SAFE_RELEASE( g_animControllers[i] );
	}

	//Clear textures and materials
	for(size_t i=0;i<m_textures.size();i++)
	{
		SAFE_RELEASE( m_textures[i] );
	}

	SAFE_DELETE(g_Morph);
	SAFE_DELETE(g_MultiMorph);
	SAFE_DELETE(g_MorphSkeleton);
	SAFE_DELETE(g_Face);
	SAFE_DELETE(g_ComplexFace);
	SAFE_DELETE(g_FaceModel);
	SAFE_DELETE(g_FaceModelGenerate);
	SAFE_DELETE(g_FaceControllerGenerate);
	SAFE_DELETE(g_pFaceFactory);
	SAFE_DELETE(g_speakController);
	SAFE_DELETE(g_ik);
	SAFE_DELETE(g_Flock);
	SAFE_DELETE(g_Crowd);
	SAFE_DELETE(g_pHair);

	//assert( D3D11RenderDevice::Instance().GetReference() == 0);

	for (int i = 0; i < g_faceControllers.size(); ++i)
	{
		FaceController* controller = g_faceControllers[i];
		SAFE_DELETE(controller);
	}

	g_faceControllers.clear();

	g_animControllers.clear();
	g_postions.clear();
	m_textures.clear();	
}



