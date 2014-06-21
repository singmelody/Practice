#include "StdAfx.h"
#include "D3D9RenderDevice.h"
#include "D3D9Texture.h"
#include <assert.h>
#include <EngineConfig.h>

namespace Dream
{
	#define CAST(type, ptr)  static_cast<type*>(ptr); 
	#define CONST_CAST(type, ptr)  static_cast<type*>(ptr); 

	D3D9RenderDevice::D3D9RenderDevice() : IRenderDevice()
		,m_devBehaviorFlags(0)
	{

	}


	D3D9RenderDevice::~D3D9RenderDevice()
	{
	}

	bool D3D9RenderDevice::CreateDevice()
	{
		if(!m_d3d9Object)
			return false;

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
		d3dParam.hDeviceWindow              = m_hwnd;
		d3dParam.EnableAutoDepthStencil     = true; 
		d3dParam.AutoDepthStencilFormat     = D3DFMT_D24S8;
		d3dParam.Flags                      = 0;
		d3dParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		d3dParam.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

		// Step 5: Create the device.
		HRESULT hr = m_d3d9Object->CreateDevice(
			D3DADAPTER_DEFAULT,			// primary adapter
			D3DDEVTYPE_HAL,				// device type
			m_hwnd,						// window associated with device
			m_devBehaviorFlags,			// vertex processing
			&d3dParam,					// present parameters
			&m_d3d9Device);				// return created device

		if(FAILED(hr))
			return false;

		return true;
	}

	bool D3D9RenderDevice::CheckCaps()
	{
		D3DDISPLAYMODE mode;
		HRESULT hr;

		m_d3d9Object->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &mode);
		hr = m_d3d9Object->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true);
		if( FAILED(hr) )
			return false;

		hr = m_d3d9Object->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);
		if( FAILED(hr))
			return false;

		D3DCAPS9 caps;
		hr = m_d3d9Object->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		if( FAILED(hr) )
			return false;

		// If pure device and HW T&L supported
		DWORD devBehaviorFlags = 0;
		if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
			devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
			devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
			devBehaviorFlags |= D3DCREATE_PUREDEVICE;

		return true;
	}

	ITexture* D3D9RenderDevice::CreateTexture(const char* path)
	{
		IDirect3DTexture9* d3d9Tex;
		D3DXCreateTextureFromFile( m_d3d9Device, path, &d3d9Tex);

		ITexture* tex =  new D3D9Texture(d3d9Tex);
		if(!tex)
			return NULL;

		return tex;
	}

	bool D3D9RenderDevice::Init(const void* wnd)
	{
		HWND* hwnd = (HWND*)wnd;
		if(!hwnd)
			return false;
		
		m_hwnd = *hwnd;

		m_d3d9Object = Direct3DCreate9(D3D_SDK_VERSION);
		if(!m_d3d9Object)
			return false;

		return true;
	}

	void D3D9RenderDevice::Destroy()
	{
		if(m_d3d9Device)
		{
			size_t reference = m_d3d9Device->Release();
			assert( reference == 0 );
		}

		SAFE_RELEASE(m_d3d9Object);
	}

	bool D3D9RenderDevice::Clear(DWORD Count, const D3DRECT *pRects, DWORD Flags, D3DCOLOR Color,float Z, DWORD Stencil)
	{
		HRESULT result = m_d3d9Device->Clear(Count,pRects,Flags,Color,Z,Stencil);
		if(FAILED(result))
			return false;

		return true;
	}

	bool D3D9RenderDevice::BeginScene()
	{
		HRESULT hr = m_d3d9Device->BeginScene();
		if(FAILED(hr))
			return false;

		return true;
	}

	void D3D9RenderDevice::EndScene()
	{
		HRESULT hr = m_d3d9Device->EndScene();
		assert( FAILED(hr) != true );

	}

	void D3D9RenderDevice::Present(const void* mainWnd)
	{
		HWND* hwnd = (HWND*)mainWnd;

		m_d3d9Device->Present( NULL,NULL, *hwnd, NULL);
	}

	void D3D9RenderDevice::SetVertexBuffer(int streamIndex, const IVertexBuffer* vb, int offsetIdx)
	{
		const D3D9VertexBuffer* d3dVB = CAST( D3D9VertexBuffer, vb);
		IDirect3DVertexBuffer9* buffer = d3dVB->GetD3D9VertexBuffer();

		m_d3d9Device->SetStreamSource( streamIndex, buffer, vertexOffset, vertexByteSize);
	}

	void D3D9RenderDevice::SetIndexBuffer(const IIndexBuffer* ib)
	{

	}

}
