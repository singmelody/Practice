#include "DXUT.h"
#include "D3D11RenderDevice.h"

D3D11RenderDevice::D3D11RenderDevice(void)
	: m_d3d11Device(NULL),
	m_d3d11DeviceContext(NULL),
	m_featureLevel(new D3D_FEATURE_LEVEL())
{

}


D3D11RenderDevice::~D3D11RenderDevice(void)
{
	SAFE_RELEASE(m_d3d11DeviceContext);
	SAFE_RELEASE(m_d3d11Device);
	SAFE_DELETE(m_featureLevel);
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

	UINT m4xMsaaQuality;
	V( m_d3d11Device->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	
	return m4xMsaaQuality > 0;
}
