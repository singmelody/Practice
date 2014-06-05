#include "StdAfx.h"
#include "D3D9Renderer.h"
#include <assert.h>
#include "EngineConfig.h"

namespace Dream{

D3D9Renderer::D3D9Renderer() :
	m_d3d9Object(NULL),
	m_d3d9Device(NULL)
{
}


D3D9Renderer::~D3D9Renderer(void)
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
	d3dParam.BackBufferWidth            = 0; 
	d3dParam.BackBufferHeight           = 0;
	d3dParam.BackBufferFormat           = D3DFMT_UNKNOWN;
	d3dParam.BackBufferCount            = 1;
	d3dParam.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dParam.MultiSampleQuality         = 0;
	d3dParam.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dParam.hDeviceWindow              = mainWnd;
	d3dParam.Windowed                   = true;
	d3dParam.EnableAutoDepthStencil     = true; 
	d3dParam.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dParam.Flags                      = 0;
	d3dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 5: Create the device.

	HR(m_d3d9Object->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		D3DDEVTYPE_HAL,           // device type
		mainWnd,          // window associated with device
		devBehaviorFlags,   // vertex processing
		&d3dParam,            // present parameters
		&m_d3d9Device));      // return created device

	return true;
}

void D3D9Renderer::Destroy()
{
	if(m_d3d9Device)
	{
		size_t refCount = m_d3d9Device->Release();
		assert( refCount == 0 );
		m_d3d9Device = NULL;
	}

	SAFE_RELEASE(m_d3d9Object);
}

void D3D9Renderer::Update()
{

}


void D3D9Renderer::Render()
{

}

}
