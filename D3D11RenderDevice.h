#pragma once
#include "IRenderDevice.h"
#include <D3DX11.h>

class D3D11RenderDevice :
	public IRenderDevice
{
public:
	D3D11RenderDevice(void);
	virtual ~D3D11RenderDevice(void);

	bool CreateDevice();

	bool CheckCaps();

private:
	ID3D11Device*			m_d3d11Device;
	D3D_FEATURE_LEVEL*		m_featureLevel;
	ID3D11DeviceContext*	m_d3d11DeviceContext;
};

