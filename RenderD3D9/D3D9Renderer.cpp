#include "StdAfx.h"
#include "D3D9Renderer.h"
#include <assert.h>
#include "EngineConfig.h"

namespace Dream{

//---- temp code -----
struct CUSTOMVERTEX
{
	FLOAT x, y, z; // The transformed position for the vertex
	DWORD color;        // The vertex color
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
//--------------------

D3D9Renderer::D3D9Renderer() :
	m_d3d9Object(NULL),
	m_d3d9Device(NULL),
	m_vb(NULL),
	m_ib(NULL)
{
}


D3D9Renderer::~D3D9Renderer()
{
}

bool D3D9Renderer::Init(const HWND mainWnd)
{
	// Step 1: Create the IDirect3D9 object.
	m_d3d9Object = Direct3DCreate9(D3D_SDK_VERSION);
	if(!m_d3d9Object)
		return false;

	// Step 2: Verify hardware support for specified formats in windowed and full screen modes.
	D3DDISPLAYMODE mode;
	m_d3d9Object->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mode);
	HR( m_d3d9Object->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true));
	HR( m_d3d9Object->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));


	// Step 3: Check for requested vertex processing and pure device.
	D3DCAPS9 caps;
	HR(m_d3d9Object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

	DWORD devBehaviorFlags = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// If pure device and HW T&L supported
	if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
		devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.
	D3DPRESENT_PARAMETERS d3dParam;
	memset( &d3dParam, 0, sizeof(d3dParam));
	d3dParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dParam.BackBufferFormat           = D3DFMT_UNKNOWN;
	d3dParam.Windowed                   = true;

	d3dParam.BackBufferWidth            = 0; 
	d3dParam.BackBufferHeight           = 0;
	d3dParam.BackBufferCount            = 0;
	d3dParam.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dParam.MultiSampleQuality         = 0;
 	d3dParam.hDeviceWindow              = mainWnd;
 	d3dParam.EnableAutoDepthStencil     = true; 
	d3dParam.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dParam.Flags                      = 0;
	d3dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

// 	ZeroMemory( &d3dParam, sizeof( d3dParam ) );
// 	d3dParam.Windowed = TRUE;
// 	d3dParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
// 	d3dParam.BackBufferFormat = D3DFMT_UNKNOWN;

	// Step 5: Create the device.
	HR(m_d3d9Object->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL,           // device type
		mainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
		&d3dParam,            // present parameters
		&m_d3d9Device));      // return created device


	InitCube();

	return true;
}

void D3D9Renderer::Destroy()
{
	SAFE_RELEASE(m_ib);
	SAFE_RELEASE(m_vb);

	if(m_d3d9Device)
	{
		size_t refCount = m_d3d9Device->Release();
		assert( refCount == 0 );
		m_d3d9Device = NULL;
	}

	SAFE_RELEASE(m_d3d9Object);
}

#include <mmsystem.h>
void D3D9Renderer::Update(float deltaTime)
{
	D3DXMATRIXA16 matWorld;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 1000.0f;
	D3DXMatrixRotationY( &matWorld, fAngle );
	m_d3d9Device->SetTransform( D3DTS_WORLD, &matWorld );

	D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	m_d3d9Device->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 800.0f/600.0f, 1.0f, 100.0f );
	m_d3d9Device->SetTransform( D3DTS_PROJECTION, &matProj );
}

void D3D9Renderer::Render()
{
	m_d3d9Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0), 1.0f, 0);
	m_d3d9Device->BeginScene();

	m_d3d9Device->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_d3d9Device->SetStreamSource( 0, m_vb, 0, sizeof( CUSTOMVERTEX ) );
	m_d3d9Device->SetIndices(m_ib);
	m_d3d9Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8, 0, 36);

	m_d3d9Device->EndScene();
	m_d3d9Device->Present( NULL, NULL, NULL, NULL);
}

//-- temp code---
void D3D9Renderer::InitCube()
{
// 	// Turn off culling, so we see the front and back of the triangle
 	m_d3d9Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
// 
// 	// Turn off D3D lighting, since we are providing our own vertex colors
 	m_d3d9Device->SetRenderState( D3DRS_LIGHTING, FALSE );

	// Initialize three Vertices for rendering a triangle
	CUSTOMVERTEX Vertices[] =
	{
		{ -1.0f, 1.0f, -1.0f, 0x0000ffff, },
		{  1.0f, 1.0f, -1.0f, 0x00ff00ff, },
		{  1.0f, 1.0f, 1.0f,  0x00ffffff, },
		{ -1.0f, 1.0f, 1.0f,  0xff0000ff, },
		{ -1.0f,-1.0f,-1.0f,  0xff00ffff, },
		{  1.0f,-1.0f,-1.0f,  0xffff00ff, },
		{  1.0f, -1.0f, 1.0f, 0xffffffff, },
		{ -1.0f, -1.0f, 1.0f, 0x000000ff, },
	};

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom Vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if( FAILED(m_d3d9Device->CreateVertexBuffer( sizeof( Vertices ),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_vb, NULL )) )
		return;

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the Vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if( FAILED( m_vb->Lock( 0, sizeof( Vertices ), ( void** )&pVertices, 0 ) ) )
		return;
	memcpy( pVertices, Vertices, sizeof( Vertices ) );
	m_vb->Unlock();

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	if( FAILED(m_d3d9Device->CreateIndexBuffer( 
		sizeof(indices), 0, 
		D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_ib, NULL) ))
		return;

	void* pIndices;
	m_ib->Lock(0,sizeof(indices),(void**)&pIndices,0);

	memcpy(pIndices,indices,sizeof(indices));

	m_ib->Unlock();

	return;
}

IRenderDevice* D3D9Renderer::GetIRenderDevice()
{

}

//--------------------------------------------------------------------
/// Dll Exports
//--------------------------------------------------------------------
extern "C" DLL_EXPORT void CreateRenderer(IRenderer** oRenderer)
{
	static D3D9Renderer  d3d9Renderer;
	*oRenderer = &d3d9Renderer;
}

}
