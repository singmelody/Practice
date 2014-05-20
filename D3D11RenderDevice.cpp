#include "DXUT.h"
#include "D3D11RenderDevice.h"
#include "Utils.h"

D3D11RenderDevice::D3D11RenderDevice(void)
	: m_d3d11Device(NULL),
	m_d3d11DeviceContext(NULL),
	m_featureLevel(new D3D_FEATURE_LEVEL()),
	m_m4xMsaaQuality(NULL),
	m_swapChain(NULL),
	m_renderTargetView(NULL),
	m_depthStencilView(NULL)
{

}


D3D11RenderDevice::~D3D11RenderDevice(void)
{
	Release();
}


void D3D11RenderDevice::Release()
{
	SAFE_DELETE(m_featureLevel);
	SAFE_RELEASE(m_depthStencilView);
	SAFE_RELEASE(m_renderTargetView);
	SAFE_RELEASE(m_swapChain);
	SAFE_RELEASE(m_d3d11DeviceContext);
}


bool D3D11RenderDevice::CreateDevice()
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[6] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDevice( 
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevels,
		sizeof(featureLevels)/sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&m_d3d11Device,
		m_featureLevel,
		&m_d3d11DeviceContext
		);

	if(FAILED(hr))
	{
		printf("D3D11CreateDevice Failed\n");
		return false;
	}

	if( *m_featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		printf("D3D_FEATURE_LEVEL_11_0 not supported\n");
		return false;
	}

	return true;
}

bool D3D11RenderDevice::CheckCaps()
{
	HRESULT hr;

	V( m_d3d11Device->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_m4xMsaaQuality));
	
	return m_m4xMsaaQuality > 0;
}

bool D3D11RenderDevice::CreateSwapChain()
{
	HRESULT hr;

	IDXGIDevice* dxgiDevice = NULL;
	V(m_d3d11Device->QueryInterface( __uuidof(IDXGIDevice), (void**)&dxgiDevice));
	
	IDXGIAdapter* dxgiAdapter = NULL;
	V(dxgiDevice->GetParent( __uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = NULL;
	V(dxgiAdapter->GetParent( __uuidof(IDXGIFactory), (void**)&dxgiFactory));

	DXGI_SWAP_CHAIN_DESC desc;
	desc.BufferDesc.Width = 640;
	desc.BufferDesc.Height = 480;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 4;	// todo msaa check
	desc.SampleDesc.Quality = m_m4xMsaaQuality - 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = DXUTGetHWND();
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0;


	V(dxgiFactory->CreateSwapChain( m_d3d11Device, &desc, &m_swapChain));

	SAFE_RELEASE(dxgiDevice);
	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	return true;
}

UINT D3D11RenderDevice::GetReference()
{
	Release();
	UINT reference = m_d3d11Device->Release();
	
	return reference;
}

bool D3D11RenderDevice::CreateRenderTargetView()
{
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer( NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	HRESULT hr = m_d3d11Device->CreateRenderTargetView( backBuffer, 0, &m_renderTargetView);
	SAFE_RELEASE(backBuffer);

	if( FAILED( hr ) )
		return false;

	return true;
}

bool D3D11RenderDevice::CreateDepthStencilBuffer()
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = 640;
	desc.Height= 480;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 4;
	desc.SampleDesc.Quality = m_m4xMsaaQuality - 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;
	HRESULT hr = m_d3d11Device->CreateTexture2D( &desc, 0, &depthStencilBuffer);
	if(FAILED(hr))
		return false;

	hr = m_d3d11Device->CreateDepthStencilView( depthStencilBuffer, NULL, &m_depthStencilView);
	if(FAILED(hr))
		return false;

	SAFE_RELEASE(depthStencilBuffer);

	return true;
}

bool D3D11RenderDevice::BindRTAndDepthToMS()
{
	m_d3d11DeviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView);
	return true;
}

bool D3D11RenderDevice::CreateViewPort()
{
	D3D11_VIEWPORT vp;
	vp.Width	= 640;
	vp.Height	= 480;
	vp.MaxDepth	= 1;
	vp.MinDepth = 0;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_d3d11DeviceContext->RSSetViewports( 1, &vp);

	return true;
}

bool D3D11RenderDevice::CreateVertexDecl()
{
	return true;
}

bool D3D11RenderDevice::Op()
{
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
	m_d3d11DeviceContext->ClearRenderTargetView( m_renderTargetView, ClearColor);
	m_d3d11DeviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return true;
}

bool D3D11RenderDevice::ShaderParse()
{
	LoadShader("Color.fx", &m_fx);

	return true;
}

D3D11RenderDevice& D3D11RenderDevice::Instance()
{
	return m_Instance;
}

D3D11RenderDevice D3D11RenderDevice::m_Instance;
